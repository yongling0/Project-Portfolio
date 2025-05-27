from flask import Flask, request, abort, jsonify
from linebot import LineBotApi, WebhookHandler
from linebot.models import (
    MessageEvent, TextMessage, StickerMessage, ImageMessage,
    VideoMessage, LocationMessage, TextSendMessage, StickerSendMessage,
    ImageSendMessage, VideoSendMessage, LocationSendMessage
)
import google.generativeai as genai
import os
import requests
from linebot.models import *

LineAIBot = Flask(__name__)

# LINE credentials
LINE_CHANNEL_ACCESS_TOKEN = 'Mi0rTHp6hcztkvqbwyqOl/ogu6SKI4471vk9jE0VpO3RUjO1efUzdwdrG/9838WgKTEW5GdPMFGkwTDRfuPlW4ywc+tXOT8MxPMGzRSY7kr2PAuQpnl0zZTPkSm93aBH8rxHgGD15QLY12IhWNvYFwdB04t89/1O/w1cDnyilFU='
LINE_CHANNEL_SECRET = '47a592f45cab1dba341740b232d07c3a'
line_bot_api = LineBotApi(LINE_CHANNEL_ACCESS_TOKEN)
handler = WebhookHandler(LINE_CHANNEL_SECRET)

# Gemini API Key
GEMINI_API_KEY = "AIzaSyBfeFQphZ-EI7rPDNMehPCZcYQMrLxE4e8"
genai.configure(api_key=GEMINI_API_KEY)
model = genai.GenerativeModel('models/gemini-1.5-flash')

# 使用 dict 存歷史對話：{ user_id: [ (user_message, bot_reply), ... ] }
chat_history = {}

# CWB 天氣 API Key
CWB_API_KEY = 'CWA-0F1CA650-E2B8-413C-9043-A6BC57DCD9CA'

# 使用者狀態：{ user_id: {"stage": "select_type"/"select_city", "type": "today"/"week"} }
user_state = {}

# 所有縣市
cities = [
    "臺北市", "新北市", "桃園市", "臺中市", "臺南市", "高雄市",
    "基隆市", "新竹市", "新竹縣", "苗栗縣", "彰化縣", "南投縣",
    "雲林縣", "嘉義市", "嘉義縣", "屏東縣", "宜蘭縣", "花蓮縣", "臺東縣",
    "金門縣", "澎湖縣", "連江縣"
]

@LineAIBot.route("/webhook", methods=['POST'])
def webhook():
    signature = request.headers.get('X-Line-Signature', '')
    body = request.get_data(as_text=True)
    try:
        handler.handle(body, signature)
    except Exception as e:
        print("Webhook Error:", e)
        abort(400)
    return 'OK'


@handler.add(MessageEvent, message=TextMessage)
def handle_text_message(event):
    user_id = event.source.user_id
    #print(f"[使用者 ID] {user_id}") 
    user_msg = event.message.text.strip().lower()

    user_id = event.source.user_id
    user_msg = event.message.text.strip()

    # 查天氣流程
    if user_msg == "查天氣":
        user_state[user_id] = {"stage": "select_type"}
        buttons = TemplateSendMessage(
            alt_text='請選擇查詢類型',
            template=ButtonsTemplate(
                title="請選擇",
                text="你要查詢今天天氣還是一週天氣？",
                actions=[
                    MessageAction(label="今天天氣", text="今天天氣"),
                    MessageAction(label="一週天氣", text="一週天氣")
                ]
            )
        )
        line_bot_api.reply_message(event.reply_token, buttons)
        store_history(user_id, user_msg, "[選擇查詢類型]")
        return

    if user_id in user_state:
        state = user_state[user_id]

        if state["stage"] == "select_type":
            if user_msg in ["今天天氣", "一週天氣"]:
                state["type"] = "today" if user_msg == "今天天氣" else "week"
                state["stage"] = "select_city"
                state["page"] = 1
                store_history(user_id, user_msg, "[選擇天氣類型]")
                return send_city_quick_reply(event.reply_token, 1)

        elif state["stage"] == "select_city":
            if user_msg == "下一頁":
                state["page"] = 2
                store_history(user_id, user_msg, "[下一頁]")
                return send_city_quick_reply(event.reply_token, 2)
            elif user_msg == "上一頁":
                state["page"] = 1
                store_history(user_id, user_msg, "[上一頁]")
                return send_city_quick_reply(event.reply_token, 1)

            if user_msg not in cities:
                line_bot_api.reply_message(event.reply_token, TextSendMessage(text="請選擇有效的縣市名稱"))
                store_history(user_id, user_msg, "[選擇無效縣市]")
                return

            # 取得天氣資料
            if state["type"] == "today":
                weather = get_today_weather(user_msg)
            else:
                weather = get_week_weather(user_msg)

            line_bot_api.reply_message(event.reply_token, TextSendMessage(text=weather))
            store_history(user_id, user_msg, weather)
            del user_state[user_id]
            return


    if user_msg == "sticker":
        reply = StickerSendMessage(package_id='11537', sticker_id='52002735')
        line_bot_api.reply_message(event.reply_token, reply)
        store_history(user_id, user_msg, "[貼圖已回覆]")
        return

    elif user_msg == "image":
        img_url = 'https://img.lovepik.com/free-png/20211213/lovepik-say-hello-expression-pack-png-image_401558427_wh1200.png'
        reply = ImageSendMessage(original_content_url=img_url, preview_image_url=img_url)
        line_bot_api.reply_message(event.reply_token, reply)
        store_history(user_id, user_msg, "[圖片已回覆]")
        return

    elif user_msg == "video":
        video_link = "https://youtu.be/9WF7nP-x2PY?si=40IO8aV8F1sKrN5Z"
        reply = TextSendMessage(text=f"這是你要的影片連結 🎬\n{video_link}")
        line_bot_api.reply_message(event.reply_token, reply)
        store_history(user_id, user_msg, video_link)
        return

    elif user_msg == "location":
        reply = LocationSendMessage(
            title="桃園車站",
            address="桃園市桃園區中正路",
            latitude=25.000,
            longitude=121.316
        )
        line_bot_api.reply_message(event.reply_token, reply)
        store_history(user_id, user_msg, "[位置資訊已回覆]")
        return

    # Gemini AI 回覆
    try:
        response = model.generate_content(user_msg)
        reply_text = response.text
    except Exception:
        reply_text = "抱歉，AI 回覆失敗了 😢"

    line_bot_api.reply_message(event.reply_token, TextSendMessage(text=reply_text))
    store_history(user_id, user_msg, reply_text)

def send_city_quick_reply(reply_token, page):
    if page == 1:
        city_slice = cities[:11]
        nav = QuickReplyButton(action=MessageAction(label="下一頁", text="下一頁"))
    else:
        city_slice = cities[11:]
        nav = QuickReplyButton(action=MessageAction(label="上一頁", text="上一頁"))

    quick_reply_items = [QuickReplyButton(action=MessageAction(label=city, text=city)) for city in city_slice]
    quick_reply_items.append(nav)

    message = TextSendMessage(
        text="請選擇縣市",
        quick_reply=QuickReply(items=quick_reply_items)
    )
    line_bot_api.reply_message(reply_token, message)

def get_today_weather(city):
    url = f"https://opendata.cwa.gov.tw/api/v1/rest/datastore/F-C0032-001?Authorization={CWB_API_KEY}&locationName={city}"
    try:
        res = requests.get(url)
        data = res.json()
        info = data['records']['location'][0]['weatherElement']
        wx = info[0]['time'][0]['parameter']['parameterName']
        minT = info[2]['time'][0]['parameter']['parameterName']
        maxT = info[4]['time'][0]['parameter']['parameterName']
        pop = info[1]['time'][0]['parameter']['parameterName']
        return f"{city} 今天天氣：\n天氣狀況：{wx}\n氣溫：{minT}~{maxT}°C\n降雨機率：{pop}%"
    except Exception as e:
        print("get_today_weather error:", e)
        return "查詢天氣資料失敗，請稍後再試"

def get_week_weather(city):
    url = "https://opendata.cwa.gov.tw/fileapi/v1/opendataapi/F-C0032-005?Authorization=CWA-0F1CA650-E2B8-413C-9043-A6BC57DCD9CA&downloadType=WEB&format=JSON"
 
    try:
        res = requests.get(url)
        data = res.json()

        locations = data['cwaopendata']['dataset']['location']
        location = next((loc for loc in locations if loc['locationName'] == city), None)
        if not location:
            return f"找不到城市 {city} 的天氣資料。"

        # 取出 Wx, MaxT, MinT 的 time 陣列
        wx_time = next(elem for elem in location['weatherElement'] if elem['elementName'] == 'Wx')['time']
        maxT_time = next(elem for elem in location['weatherElement'] if elem['elementName'] == 'MaxT')['time']
        minT_time = next(elem for elem in location['weatherElement'] if elem['elementName'] == 'MinT')['time']

        result = f"🌤 {city} 一週天氣預報：\n"

        # CWB 一週天氣約有14筆時間段，通常一天兩筆 (日夜各一)，我們取每天白天（偶數筆）資料
        # 取出未來7天的資料（白天）
        for i in range(0, 14, 2):
            date = wx_time[i]['startTime'].split('T')[0]
            weather = wx_time[i]['parameter']['parameterName']
            max_temp = maxT_time[i]['parameter']['parameterName']
            min_temp = minT_time[i]['parameter']['parameterName']

            result += f"\n📅 {date}\n天氣：{weather}\n氣溫：{min_temp}°C ~ {max_temp}°C\n"

        return result

    except Exception as e:
        print("get_week_weather error:", e)
        return "查詢天氣資料失敗，請稍後再試"


@handler.add(MessageEvent, message=StickerMessage)
def handle_sticker_message(event):
    user_id = event.source.user_id
    line_bot_api.reply_message(
        event.reply_token,
        StickerSendMessage(package_id='11537', sticker_id='52002735')
    )
    store_history(user_id, "[收到使用者貼圖]", "[貼圖已回覆]")

@handler.add(MessageEvent, message=ImageMessage)
def handle_image_message(event):
    user_id = event.source.user_id
    reply_text = "我收到了你的圖片！謝謝你 📸"
    line_bot_api.reply_message(event.reply_token, TextSendMessage(text=reply_text))
    store_history(user_id, "[收到使用者圖片]", reply_text)

@handler.add(MessageEvent, message=VideoMessage)
def handle_video_message(event):
    user_id = event.source.user_id
    reply_text = "我收到你的影片了！🎥"
    line_bot_api.reply_message(event.reply_token, TextSendMessage(text=reply_text))
    store_history(user_id, "[收到使用者影片]", reply_text)

@handler.add(MessageEvent, message=LocationMessage)
def handle_location_message(event):
    user_id = event.source.user_id
    title = event.message.title or "你的位置"
    address = event.message.address
    lat = event.message.latitude
    lng = event.message.longitude
    reply_text = f"{title}\n地址：{address}\n座標：({lat}, {lng})"
    line_bot_api.reply_message(event.reply_token, TextSendMessage(text=reply_text))
    store_history(user_id, "[收到使用者位置]", reply_text)

# 儲存對話記錄
def store_history(user_id, user_msg, bot_reply):
    print("使用者：", user_id)
    if not user_id:
        print("[警告] user_id 是 None，無法儲存聊天記錄")
        return
    if user_id not in chat_history:
        chat_history[user_id] = []
    chat_history[user_id].append((user_msg, bot_reply))
    #print(f"[儲存成功] {user_id}: ({user_msg}, {bot_reply})")


# RESTful API：取得對話紀錄
@LineAIBot.route("/history/<user_id>", methods=["GET"])
def get_history(user_id):
    #print("使用者：", user_id)
    #print("目前對話紀錄：", chat_history)

    if not user_id:
        return jsonify({"message": "user_id 無效"}), 400

    if user_id in chat_history:
        print(f"對話紀錄: {chat_history[user_id]}")
        return jsonify(chat_history[user_id])
    return jsonify({"message": "查無此使用者對話"}), 404


# RESTful API：刪除對話紀錄
@LineAIBot.route("/history/<user_id>", methods=["DELETE"])
def delete_history(user_id):
    if user_id in chat_history:
        del chat_history[user_id]
        return jsonify({"message": f"已刪除 {user_id} 的對話紀錄"}), 200
    else:
        return jsonify({"message": "查無此使用者紀錄"}), 404

if __name__ == "__main__":
    LineAIBot.run(port=5000)