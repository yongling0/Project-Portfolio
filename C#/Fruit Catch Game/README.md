這是一個C# Windows Forms開發的接水果小遊戲\
**遊戲機制**
遊戲時間：120秒倒數計時\
操作方式：玩家用滑鼠左右移動碗來接住落下的水果\
得分系統：成功接到水果就加分，畫面顯示目前得分

**視覺效果**
水果種類：同時有3種不同水果（香蕉、草莓、番茄）從畫面上方落下\
動態背景：每10秒自動更換背景圖片（繡球花、企鵝、鬱金香），以半透明效果呈現\
流暢動畫：使用雙緩衝技術避免畫面閃爍

**遊戲邏輯**
水果會從隨機位置落下，當碰到底部時會重新從頂部隨機位置開始下落\
碗的移動範圍有限制，不會超出畫面邊界\
碰撞檢測：當水果落到特定高度且與碗的位置重疊時計分

**控制功能**
重新開始：透過選單可以重置遊戲，重新計時和計分\
即時顯示：畫面同時顯示剩餘時間和目前得分
