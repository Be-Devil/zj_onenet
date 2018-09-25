#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <String.h>
WiFiClient client;

int fob = 0;
int lor = 0;
int ins = 0;
int scr = 0;
int flag = 0;
long lastTime = 0;

int current_flag=0;
int next_flag=0;

char ssid[20] = "TP-LINK_D2D2";     //修改为自己的路由器用户名//ChinaNet-fk
char password[20] = "zxc123456789"; //修改为自己的路由器密码//1l3o1v4e5y2o0u
char onenettcp[16] = "183.230.40.40";
int  onenetport = 1811;
String s1 = "192.168.168.101";
String s2 = "183.230.40.40";
String s3 = "192.168.43.1";
String s4 = "HUAWEI";
String s5 = "zjwy1234";

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
  if (millis() - lastTime >= 5000) {
    client.print("*");
    lastTime = millis();
  }
  while (!client.connected())//当client没有连接时触发
  {
    
if(digitalRead(12) == LOW)
  {
  s1.toCharArray(onenettcp,16);
  }else if(digitalRead(13) == LOW)
  {
  s3.toCharArray(onenettcp,16);
  s4.toCharArray(ssid,7);
  s5.toCharArray(password,9);
  }
    Serial.println("Reconnect TCP Server...");
    //未连接到服务器
    if (!client.connect(onenettcp, onenetport))
    {
      //未连接到WIFI
      if (WiFi.status() != WL_CONNECTED)
      {
        //重连
        Serial.println("Reconnect WIFI ...");
        connectWifi();
      }
      delay(100);
    } else {
      Serial.println("Reconnect Falied");
    }
  }
}
void Init() {
    
if(digitalRead(12) == LOW)
  {
  s1.toCharArray(onenettcp,16);
  }else if(digitalRead(13) == LOW)
  {
  s3.toCharArray(onenettcp,16);
  s4.toCharArray(ssid,7);
  s5.toCharArray(password,9);
  }
  connectWifi();
  Serial.println(onenettcp);
  if (client.connect(onenettcp, onenetport)) {
    Serial.println("Client Connected\r");
    client.print("*151331#InfraredLaser#lua*");     //修改为自己的设备id号，设备授权信息, 脚本名称
    delay(1000);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(14,  OUTPUT);
  pinMode(12,  INPUT);
  pinMode(13,  INPUT);
  
  digitalWrite(14,  HIGH);

}

void loop() { 
  Init();
  while(1){
    LoupInit();
    while (client.available()) {
      String line = client.readStringUntil('*');
      Serial.println(line);
        if (parseCMD(line, &fob, &lor, &ins, &scr) == 0) {
        next_flag = fob;
        while(next_flag%3 != current_flag%3)
        {
          delay(100);
         digitalWrite(14, LOW);           //将引脚全部 拉  高
         Serial.println("LOW");
          delay(100);
         digitalWrite(14, HIGH);           //将引脚全部 拉  高
         Serial.println("HIGH");
         current_flag++;
        }
        
      }
    }
  }
  
}
