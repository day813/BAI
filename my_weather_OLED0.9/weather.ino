void get_weather_data(DynamicJsonDocument &doc);

//实时天气
struct weather_data {
  //API状态码
  int code = -1;
  //当前API的最近更新时间
  String updateTime = "";
  //数据观测时间
  String now_obsTime = "";
  //温度，默认单位：摄氏度
  String now_temp = "";
  //体感温度，默认单位：摄氏度
  int now_feelsLike = 0;
  //天气状况和图标的代码，图标可通过天气状况和图标下载
  String now_icon = "";
  //天气状况的文字描述，包括阴晴雨雪等天气状态的描述
  String now_text = "";
  //风向
  String now_windDir = "";
  //风向360角度
  String now_wind360 = "-1";
  //风力等级
  String now_windScale = "-1";
  //风速，公里/小时
  int now_windSpeed = -1;
  //相对湿度，百分比数值
  int now_humidity = -1;
  //当前小时累计降水量，默认单位：毫米
  int now_precip = -1;
  //大气压强，默认单位：百帕
  int now_pressure = -1;
  //能见度，默认单位：公里
  int now_vis = -1;
} wd;

//获取天气
void getWeather() {
  u8g2.clearBuffer();
  //检查WIFI连接
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    Serial.print("[HTTPS] begin...\n");
    echoScreen(0, 20, "请求天气接口...");

    //请求接口
    if (http.begin(client, "http://1024.sodevel.com/weather_api/?cityid=" + cityID + "&key=" + KEY)) {
      Serial.print("[HTTPS] GET...\n");
      //获取HTTP的状态码，一般200=成功，出现其他的比如404、500 均为各种报错
      int httpCode = http.GET();
      if (httpCode > 0) {
        echoScreen(0, 40, "开始解析数据...");
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          //从接口获取数据
          String payload = http.getString();
          Serial.println(payload);

          //解析json
          DynamicJsonDocument doc(4096);
          DeserializationError err = deserializeJson(doc, payload);
          if (err.code() == DeserializationError::Ok) {
            //将数据保存到weather_data的结构体，方便后续调用
            get_weather_data(doc);

            echoScreen(0, 60, "数据解析 OK！");
            delay(1000);
          } else {
            Serial.println("数据解析出错");
            echoScreen(0, 60, "数据解析出错 ERROR");
            delay(1000);
          }
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        echoScreen(0, 40, "请求失败，重启或稍等");
        delay(1000);
      }
      http.end();
    }
  } else {
    echoScreen(0, 20, "WIFI 掉线，正在重连...");
    delay(500);
    WiFi.disconnect();
    initWiFi();
  }
}

//将数据保存到weahter_data 的结构体，方便后续调用
void get_weather_data(DynamicJsonDocument &doc) {
  wd.code = doc["today"]["code"];
  wd.updateTime = doc["today"]["updateTime"].as<String>().substring(0, 16);
  wd.now_obsTime = doc["today"]["now"]["obsTime"].as<String>().substring(0, 16);

  wd.updateTime.replace("T", " ");
  wd.now_obsTime.replace("T", " ");

  wd.now_temp = doc["today"]["now"]["temp"].as<String>();
  wd.now_feelsLike = doc["today"]["now"]["feelsLike"].as<int>();
  wd.now_icon = doc["today"]["now"]["icon"].as<String>();
  wd.now_text = doc["today"]["now"]["text"].as<String>();
  wd.now_wind360 = doc["today"]["now"]["wind360"].as<String>();
  wd.now_windDir = doc["today"]["now"]["windDir"].as<String>();
  wd.now_windScale = doc["today"]["now"]["windScale"].as<String>();
  wd.now_windSpeed = doc["today"]["now"]["windSpeed"].as<int>();
  wd.now_humidity = doc["today"]["now"]["humidity"].as<int>();
  wd.now_precip = doc["today"]["now"]["precip"].as<int>();
  wd.now_pressure = doc["today"]["now"]["pressure"].as<int>();
  wd.now_vis = doc["today"]["now"]["vis"].as<int>();

  Serial.print("wd.code: ");
  Serial.println(wd.code);

  Serial.print("wd.updateTime: ");
  Serial.println(wd.updateTime);

  Serial.print("wd.now_obsTime: ");
  Serial.println(wd.now_obsTime);

  Serial.print("wd.now_temp: ");
  Serial.println(wd.now_temp);

  Serial.print("wd.now_feelsLike: ");
  Serial.println(wd.now_feelsLike);

  Serial.print("wd.now_icon: ");
  Serial.println(wd.now_icon);

  Serial.print("wd.now_text: ");
  Serial.println(wd.now_text);

  Serial.print("wd.now_wind360: ");
  Serial.println(wd.now_wind360);

  Serial.print("wd.now_windDir: ");
  Serial.println(wd.now_windDir);

  Serial.print("wd.now_windScale: ");
  Serial.println(wd.now_windScale);

  Serial.print("wd.now_windSpeed: ");
  Serial.println(wd.now_windSpeed);

  Serial.print("wd.now_humidity: ");
  Serial.println(wd.now_humidity);

  Serial.print("wd.now_precip: ");
  Serial.println(wd.now_precip);

  Serial.print("wd.now_pressure: ");
  Serial.println(wd.now_pressure);

  Serial.print("wd.now_vis: ");
  Serial.println(wd.now_vis);
}


/**
 * 向屏幕输出天气信息
 */
void echoWeather() {
  //擦除屏幕
  u8g2.clearBuffer();

  //定位光标
  u8g2.setCursor(0, 10);
  //显示内容
  u8g2.print(td.localDate);
  //定位光标
  u8g2.setCursor(75, 10);
  //显示内容
  u8g2.print(td.localTime);

  u8g2.setCursor(0, 40);
  u8g2.print("气温 " + String(wd.now_temp) + "℃");

  u8g2.setCursor(64, 40);
  u8g2.print("湿度 " + String(wd.now_humidity) + "%");

  u8g2.setCursor(0, 60);
  u8g2.print(wd.now_text);
  // u8g2.print("降雨量" + wd.now_precip);
  u8g2.setCursor(45, 60);
  u8g2.print(wd.now_windDir + "  " + wd.now_windSpeed + " KM");

  //将上面定义的数据，显示到屏幕上
  u8g2.sendBuffer();
}
