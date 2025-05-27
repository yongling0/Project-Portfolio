這是使用python撰寫的作品區，以下是針對兩個專案的介紹:\
**Line AI Bot**\
這是一個基於 Flask 開發的 LINE Bot 應用程式，整合了多項功能\
**核心功能**\
**1. LINE Bot 基礎功能**\
接收和回應各種類型的 LINE 訊息（文字、貼圖、圖片、影片、位置）\
使用 LINE Messaging API 進行雙向溝通

**2. AI 對話功能**\
整合 Google Gemini AI（使用 gemini-1.5-flash 模型）\
當使用者發送一般文字訊息時，會透過 Gemini AI 生成智能回應

**3. 天氣查詢系統**\
這是程式的特色功能，提供互動式天氣查詢：\
流程設計：\
使用者輸入「查天氣」啟動功能\
透過按鈕選擇查詢類型（今天天氣 vs 一週天氣）\
使用快速回覆選單選擇台灣22個縣市\
支援分頁瀏覽縣市列表\
天氣資料來源：\
使用中央氣象署開放資料 API\
今日天氣：提供天氣狀況、溫度範圍、降雨機率\
一週天氣：提供7天詳細預報

**4. 多媒體回應**\
提供特定關鍵字觸發的回應：\
"sticker" → 回傳貼圖\
"image" → 回傳圖片\
"video" → 回傳 YouTube 影片連結\
"location" → 回傳桃園車站位置資訊

**5. 對話記錄系統**\
自動儲存每位使用者的對話歷史\
提供 RESTful API 管理對話記錄：\
GET /history/<user_id> - 查詢對話記錄\
DELETE /history/<user_id> - 刪除對話記錄

**6. 狀態管理**\
追蹤使用者在天氣查詢流程中的狀態\
支援多使用者同時使用，各自維護獨立狀態

**技術架構**\
主要套件：\
Flask：Web 框架\
line-bot-sdk：LINE Bot SDK\
google-generativeai：Google AI 整合\
requests：HTTP 請求處理

資料結構：\
chat_history：儲存對話記錄的字典\
user_state：追蹤使用者操作狀態\
cities：台灣縣市列表\
這個 Bot 結合了 AI 對話、實用工具（天氣查詢）和娛樂功能，提供了完整的使用者體驗。透過狀態管理和 RESTful API，也具備了良好的擴展性和資料管理能力
