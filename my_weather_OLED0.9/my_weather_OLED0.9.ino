/**
  * 联网实时天气预报
  * by 段勇
  */

#include <Arduino.h>

//启用屏幕所需相关库文件
#include <U8g2lib.h>
#include <Wire.h>

//处理json数据
#include <ArduinoJson.h>

//连接WIFI
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

//从NTP服务器更新时间
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp.aliyun.com");

/************配置信息************/
/**
 * 你的wifi 账号和密码
 * 注意：切勿连接 5G的频率、不要使用双频融合（路由器）
 * 注意：账号密码必须完全正确，包括字母大小写、空格、中划线、下划线
 */
const char* wifiName = "ChinaNet-RL36";
const char* wifiPassword = "j6fg7h65";

//来自和风天气的密钥
const String KEY = "8a9b2df3317a40aba478cb9c884bd728";

//你的城市ID（在和风天气中获取）
const String cityID = "101181407";

//屏幕使用的针脚
const int pin_scl = D2;
const int pin_sda = D1;

/**
 * 设置刷新频率，由于我们需要给时间、天气设置不同的刷新频率
 * 因此不能使用 delay() 而改用这种更灵活专业的方法
 * 官方有例子：示例 - digital - BlinkWithoutDelay
 */
unsigned long last_ntp = 0;
const long interval_ntp = 1000;  //每秒钟刷新一次屏幕时间（不建议修改，低于一秒没有意义）

unsigned long last_weather = 0;
const long interval_weather = 1000 * 300;  //每5分钟刷新一次天气信息（刷新太快没有意义，因为接口数据更新也有延迟）

/******************程序可配置项【完毕】*******************************/

//根据屏幕的类型，配置并创建对象
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ pin_scl, /* data=*/ pin_sda, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  Serial.begin(115200);

  //初始化屏幕
  initScreen();
  echoScreen(25, 32, "系统启动中...");
  Serial.println("系统启动中...");
  //部分电脑在单片机启动时，串口容易故障，因此做一个等待和刷新的动作
  //它会影响系统启动时间（至少延长4秒）可以删除
  Serial.println();
  Serial.println();
  Serial.println();
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  //连接wifi
  initWiFi();
  //请求ntp时间
  initNtp();
  //请求天气数据
  getWeather();
}

void loop() {
  //获取单片机启动至今的毫秒数
  unsigned long currentMillis = millis();

  //更新时间
  if (currentMillis - last_ntp >= interval_ntp) {
    last_ntp = currentMillis;
    loopNtp();
    echoWeather();
  }

  //更新天气
  if (currentMillis - last_weather >= interval_weather) {
    //断网时不更新
    if (WiFi.status() == WL_CONNECTED) {
      last_weather = currentMillis;
      getWeather();
      echoWeather();
    }
  }
}
