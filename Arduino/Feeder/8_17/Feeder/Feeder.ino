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

int i = 0;
int  timer = 0;

const char ssid[] = "TP-LINK_D2D2";     //修改为自己的路由器用户名//ChinaNet-fk
const char password[] = "zxc123456789"; //修改为自己的路由器密码//1l3o1v4e5y2o0u
char onenettcp[16] = "183.230.40.40";
int  onenetport = 1811;
String s1 = "192.168.168.102";
String s2 = "183.230.40.40";

int massage1[6] = {0x7A,0x03,0x04,0x01,0x08,0x55};
int massage2[6] = {0x7A,0x03,0x05,0x01,0x09,0x55};

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
    
  if(digitalRead(3) == LOW){
    s1.toCharArray(onenettcp,16);
    } 
    else{
    s2.toCharArray(onenettcp,16);
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
  connectWifi();
  Serial.println(onenettcp);
  if (client.connect(onenettcp, onenetport)) {
    Serial.println("Client Connected\r");
    client.print("*151331#Feeder#lua*");     //修改为自己的设备id号，设备授权信息, 脚本名称
    delay(1000);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(16,  OUTPUT);
  
  digitalWrite(16, HIGH);         

}

void loop() { 
  Init();
  while(1){
    LoupInit();
    while (client.available()) {
      String line = client.readStringUntil('*');
      Serial.println(line);
        if (parseCMD(line, &fob, &lor, &ins, &scr) == 0) {
          switch(fob)
          {
            case 1:
             digitalWrite(16, LOW);           
             delay(200);
             digitalWrite(16, HIGH);    
            break;
          }
          
      }
    }
  }
  
}
