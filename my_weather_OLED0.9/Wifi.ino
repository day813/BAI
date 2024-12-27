//连接WIFI
void initWiFi() {
  u8g2.clearBuffer();

  Serial.print("Connecting to ");
  Serial.println(wifiName);
  echoScreen(0, 20, "WIFI：" + String(wifiName));

  //定义工作模式，8266不仅能连接到WIFI，也可以建议热点让别人连接自己
  WiFi.mode(WIFI_STA);

  //开始连接
  WiFi.begin(wifiName,wifiPassword);

  //检查连接是否成功
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  //设置自动重新连接到最近连接过的接入点
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  //连接完毕，打印自身IP地址
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");

  echoScreen(0, 40, "连接成功");
  echoScreen(0, 60, WiFi.localIP().toString());
  delay(1000);
}
