//初始化屏幕
void initScreen(){
  u8g2.begin();
  //定义为UTF8字符集（否则不支持中文）
  u8g2.enableUTF8Print();
  //定义字体
  u8g2.setFont(u8g2_font_wqy14_t_gb2312);
}

//向屏幕输出内容
void echoScreen(int col,int row,const String text){
  //定位光标
  u8g2.setCursor(col,row);
  //显示内容
  u8g2.print(text);
  //发送到屏幕
  u8g2.sendBuffer();
}