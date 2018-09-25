#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <String.h>
WiFiClient client;

int pos = 0;          // 变量pos用来存储舵机位置
int fob = 0;
int lor = 0;
int ins = 0;
int scr = 0;
int flag = 0;
long lastTime = 0;

int  timer = 0;

const char ssid[] = "TP-LINK_D2D2";     //修改为自己的路由器用户名//ChinaNet-fk
const char password[] = "zxc123456789"; //修改为自己的路由器密码//1l3o1v4e5y2o0u
char onenettcp[] = "183.230.40.40";
int  onenetport = 1811;

void connectWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");   
  }
  Serial.println("\rWifi Connected\r");
}

int parseCMD(String data, int* fob, int* lor, int* ins, int* scr) {
  if(data.substring(data.length()-1,data.length()).toInt() == 1){
    *fob = (data.substring(0, 1)).toInt();
    *lor = (data.substring(1, 2)).toInt();
    *ins = (data.substring(2, 3)).toInt();
    *scr = (data.substring(3, 4)).toInt();
    Serial.println("fob = " + String(*fob));
    Serial.println("lor = " + String(*lor));
    Serial.println("ins = " + String(*ins));
    Serial.println("scr = " + String(*scr));
  }
    return 0;
}
void LoupInit() {
  int n =digitalRead(12); 
  if(n==HIGH){
    int i;
    String s = "192.168.168.102";
    for(i = 0;i<s.length();i++)
     onenettcp[i] = s.charAt(i);
     onenetport = 5025;
    Serial.println("high");
  }
  connectWifi();
    Serial.println(onenettcp);
  if (client.connect(onenettcp, onenetport)) {
    Serial.println("Client Connected\r");
    client.print("*151331#mouse#lua*");     //修改为自己的设备id号，设备授权信息, 脚本名称
    delay(1000);
  }
  
  if (millis() - lastTime >= 5000) {
    client.print("*");
    lastTime = millis();
  }
//  while (!client.connected())//当client没有连接时触发
//  {
//    Serial.println("Reconnect TCP Server...");
//    //未连接到服务器
//    if (!client.connect(onenettcp, onenetport))
//    {
//      //未连接到WIFI
//      if (WiFi.status() != WL_CONNECTED)
//      {
//        //重连
//        Serial.println("Reconnect WIFI ...");
//        connectWifi();
//      }
//      delay(100);
//    } else {
//      Serial.println("Reconnect Falied");
//    }
//  }
}

void setup() {
  Serial.begin(9600);
  pinMode(4,  OUTPUT);
  pinMode(5,  OUTPUT);
  pinMode(16,  OUTPUT);
  pinMode(14,  OUTPUT);
  pinMode(12,INPUT);  
  
  digitalWrite(4, HIGH);           //将引脚全部 拉  高
  digitalWrite(5, HIGH);           //将引脚全部 拉  高
  digitalWrite(16, HIGH);           //将引脚全部 拉  高
  digitalWrite(14, HIGH);           //将引脚全部 拉  高

}

void loop() {
  if(flag++ == 0){
    LoupInit();
  }
  else{
    flag = 2;
  }
  while(1){
    client.print("1231");
    delay(1000);  
  
  while (client.available()) {
    String line = client.readStringUntil('*');
    Serial.println(line);
      if (parseCMD(line, &fob, &lor, &ins, &scr) == 0) {
        switch(fob)
        {
          case 1:
            digitalWrite(14, LOW); 
            delay(100);     
            digitalWrite(14, HIGH); 
          break;
          
          case 2:
            digitalWrite(16, LOW); 
            delay(100);     
            digitalWrite(16, HIGH); 
          break;
          
          case 3:
            digitalWrite(5, LOW); 
            delay(100);     
            digitalWrite(5, HIGH); 
          break;
          
          case 4:
            digitalWrite(4, LOW); 
            delay(100);     
            digitalWrite(4, HIGH); 
          break;
          
          
        }
        
    }
  }
  }
}
