struct time_data {
  String localDate;
  String localTime;
} td;

//获取时间
void initNtp() {
  timeClient.begin();
  //28800 = +8时区（我们的北京时间）
  timeClient.setTimeOffset(28800);
}
//刷新时间
void loopNtp() {
  timeClient.update();
  //获取unix时间戳（1970年至今的总秒数）
  unsigned long epochTime = timeClient.getEpochTime();
  Serial.print("epochTime: ");
  Serial.println(epochTime);

  //格式化 - 时：分：秒
  td.localTime = timeClient.getFormattedTime();

  time_t rawtime = epochTime;
  struct tm* ti;
  ti = localtime(&rawtime);

  //计算 年-月-日
  uint16_t year = ti->tm_year + 1900;
  String yearStr = String(year);
  uint8_t month = ti->tm_mon + 1;
  String monthStr = month < 10 ? "0" + String(month) : String(month);
  uint8_t day = ti->tm_mday;
  String dayStr = day < 10 ? "0" + String(day) : String(day);
  String ymd = yearStr + "-" + monthStr + "-" + dayStr;
  //将得到的年月日写入weather_data 结构体
  td.localDate = ymd;

  //计算 时-分-秒
  uint8_t hours = ti->tm_hour;
  String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);
  uint8_t minutes = ti->tm_min;
  String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);
  uint8_t seconds = ti->tm_sec;
  String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);
}
