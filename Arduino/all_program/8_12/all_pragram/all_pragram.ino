#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <String.h>
WiFiClient client;

char ssid[20] = "HUAWEI";     //修改为自己的路由器用户名
char password[20] = "zjwy1234"; //修改为自己的路由器密码
char onenettcp[16] = "192.168.43.1";
int  onenetport = 1811;
int i;

int lastTime;

void connectWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");   
  }
  Serial.println("\rWifi Connected\r");
}

int parseCMD(String data) {
    return 0;
}
void LoupInit() {
  if (millis() - lastTime >= 5000) {
    client.print("*");
    lastTime = millis();
  }
  while (!client.connected())//当client没有连接时触发
  {
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
//  Serial.println(onenettcp);
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
      Serial.println(line);
      char buf[line.length()];
      line.toCharArray(buf,line.length()+1);
      if(buf[0] == 0xAA && buf[1] == line.length()-1){
        switch(buf[2]){
          case 0x01:
             switch(buf[3]){
                case 0x01:
                while(buf[1]--){
                  ssid[line.length()-buf[1]-2] = buf[line.length()-buf[1]+1];
                }
                break;
                case 0x02:
                break;
                case 0x03:
                break;
                case 0x04:
                break;
                case 0x05:
                  client.flush();
                  Serial.println(ssid);
                  Serial.println(password);
                  Serial.println(onenettcp);
                  Serial.println(onenetport);
                  Init();
                break;
             }
          break;
            
        }



        
        
      }
      
    }
  }
  
}
