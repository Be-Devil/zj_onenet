#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <String.h>
WiFiClient client;

char ssid[30] = "TP-LINK_D2D2";     //修改为自己的路由器用户名
char password[30] = "zxc123456789"; //修改为自己的路由器密码
char onenettcp[20] = "192.168.100.26";
int  onenetport = 1811;
int i;
String null = "";

int lastTime;
int timer = 0;
char clientBuf[] = {0xaa,0x03,0x02,0x00,0x2a};

void connectWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");   
  }
  Serial.println("\rWifi Connected\r");
}

void c_send(char *s) {
     client.write((const uint8_t*)s, sizeof(s));
}

void LoupInit() {
  while (!client.connected())
  {
    Serial.println("Reconnect TCP Server...");
    if (!client.connect(onenettcp, onenetport)){
      if (WiFi.status() != WL_CONNECTED){
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
  if (client.connect(onenettcp, onenetport)) {
    Serial.println("Client Connected\r");
//    client.print("*151331#ToyCar#lua*");     //修改为自己的设备id号，设备授权信息, 脚本名称
//    delay(1000);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(4,  OUTPUT);
  pinMode(5,  OUTPUT);
  pinMode(16,  OUTPUT);
  pinMode(14,  OUTPUT);
  pinMode(13,  INPUT);
  pinMode(12,  INPUT);
  
  digitalWrite(4, HIGH);           //将引脚全部 拉  高
  digitalWrite(5, HIGH);           //将引脚全部 拉  高
  digitalWrite(16, HIGH);           //将引脚全部 拉  高
  digitalWrite(14,  HIGH);

}

void loop() { 
  Init();
  while(1){
    LoupInit();
    while (client.available()) {
      String line = client.readStringUntil('*');
                  Serial.println(password);
      Serial.println(line);
                  Serial.println(password);
      char buf[line.length()];
      line.toCharArray(buf,line.length()+1);
      if(buf[0] == 0xAA && buf[1] == line.length()-1){
        switch(buf[2]){
          case 0x01:
             switch(buf[3]){
                case 0x01:
                null.toCharArray(ssid,30);
                while(buf[1]--){
                  ssid[line.length()-buf[1]-2] = buf[line.length()-buf[1]+2];
                }
                break;
                
                case 0x02:
                null.toCharArray(password,30);
                while(buf[1]--){
                  password[line.length()-buf[1]-2] = buf[line.length()-buf[1]+2];
                }
                  Serial.println(password);
                break;
                
                case 0x03:
                null.toCharArray(onenettcp,20);
                while(buf[1]--){
                  onenettcp[line.length()-buf[1]-2] = buf[line.length()-buf[1]+2];
                }
                break;
                
                case 0x04:
                  onenetport = line.substring(4,buf[1]+1).toInt();
                  Serial.println(password);
                break;
                
                case 0x05:
                  Serial.println(password);
                  WiFi.disconnect();
                  Serial.println(ssid);
                  Serial.println(password);
                  Serial.println(onenettcp);
                  Serial.println(onenetport);
                  Init();
                break;
             }
          break;
          
        case 0x03://妖娆花
          clientBuf[3] = 1;
          switch(buf[3]){
              
          }
          break;
          
        case 0x04://舵机
          clientBuf[3] = 2;
          switch(buf[3]){
              
          }
          break;
          
        case 0x05://扫地机器人
          clientBuf[3] = 3;
          switch(buf[3]){
              
          }
          break;
          
        case 0x06://玩具车
          clientBuf[3] = 4;
          switch(buf[3]){
              
          }
          break;
          
        case 0x07://红外激光
          clientBuf[3] = 5;
          switch(buf[3]){
              
          }
          break;
          
        case 0x08://洒水罐
          clientBuf[3] = 6;
          switch(buf[3]){
              
          }
          break;
          
        case 0x09://玩具鼠
          clientBuf[3] = 7;
          switch(buf[3]){
              
          }
          break;
          
        case 0x0a://玩具蛇
          clientBuf[3] = 8;
          switch(buf[3]){
              
          }
          break;
          
        case 0x0b://骰子机
          clientBuf[3] = 9;
          switch(buf[3]){
              
          }
          break;
          
        case 0x0c://智能小车
          clientBuf[3] = 10;
          switch(buf[3]){
              
          }
          break;
          
            
        }
      }
      
    }


/*
 * 心跳
 */

  if (millis() - lastTime >= 1000) {
    timer++;
    if(timer%5 == 4){
      c_send(clientBuf);
    }
    lastTime = millis();
  }














    
  }
  
}
