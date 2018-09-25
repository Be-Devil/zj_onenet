#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <HttpPacket.h>
#include <String.h>

#define FRONT  0   //管脚D8
#define BACK   12  //管脚D6
#define LEFT   2   //管脚D9
#define RIGHT  13  //管脚D7
#define INSERT 4   //管脚D4
#define SCRAWL 5   //管脚D3

#define SIGNAL1 16 //管脚D2
//#define SIGNAL2 13 //管脚D7

WiFiClient client;
int count = 0;
int stat = 0;
int fob = 0;
int lor = 0;
int ins = 0;
int scr = 0;
int sig1 = 0;
int flag = 0;

int  timer = 0;

const char ssid[] = "ChinaNet-fk";     //修改为自己的路由器用户名//ChinaNet-fk
const char password[] = "1l3o1v4e5y2o0u"; //修改为自己的路由器密码//1l3o1v4e5y2o0u
const char onenettcp[] = "183.230.40.40";
const int  onenetport = 1811;

void connectWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");   
  }
  Serial.println("\rWifi Connected\r");
}

int parseCMD(String data, int* fob, int* lor, int* ins, int* scr) {

if(data.substring(data.length()-1,data.length()).toInt() == 1)
{
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


void setup() {
  Serial.begin(9600);
  pinMode(16,  OUTPUT);
  digitalWrite(16, HIGH);           //将引脚全部 拉  高
//  pinMode(BACK,   OUTPUT);
//  pinMode(LEFT,   OUTPUT);
//  pinMode(RIGHT,  OUTPUT);
//  pinMode(INSERT, OUTPUT);
//  pinMode(SCRAWL, OUTPUT);
//  pinMode(SIGNAL1, INPUT);
//  digitalWrite(FRONT, HIGH);           //将引脚全部 拉  高
//  digitalWrite(BACK, HIGH);
//  digitalWrite(LEFT, HIGH);
//  digitalWrite(RIGHT, HIGH);
//  digitalWrite(INSERT, HIGH);
//  digitalWrite(SCRAWL, HIGH);
  //digitalWrite(SIGNAL1, LOW);       // 拉高输入光眼信号的io口D2    //原来没有对这个io进行过初始操作
  connectWifi();
  if (client.connect(onenettcp, onenetport)) {
    Serial.println("Client Connected\r");
    client.print("*151331#huahua0001#huahua*");     //修改为自己的设备id号，设备授权信息, 脚本名称
    delay(1000);
  }
}


long lastTime = 0;
void loop() {
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
  sig1 = digitalRead(SIGNAL1);
  //Serial.println("sig1 = " + String(sig1));
  if (sig1 == 0 && stat == 0) {
    client.print("1");
    stat = 1;//防止重复发送
  } else if (sig1 == 1 && stat == 1) {
    stat = 0;
  }

  while (client.available()) {
    String line = client.readStringUntil('*');
    Serial.println(line);
    if (parseCMD(line, &fob, &lor, &ins, &scr) == 0) {
      
      if (fob == 1) {
         digitalWrite(16, LOW);           //将引脚全部 拉  高
         Serial.println("LOW");
         delay(150);
         digitalWrite(16, HIGH);           //将引脚全部 拉  高
         Serial.println("HIGH");
         if(flag > 0)
         flag = 0;
         else
         flag = 1;
      } 
      
      if (fob == 2) {
         digitalWrite(16, LOW);           //将引脚全部 拉  高
         Serial.println("LOW");
         delay(150);
         digitalWrite(16, HIGH);           //将引脚全部 拉  高
         Serial.println("HIGH");
      }
        
  }
  }
      if (flag > 0) {
        flag++;
        delay(1000);
         Serial.println(flag);
      }
      if(flag == 11)
       {
         digitalWrite(16, LOW);           //将引脚全部 拉  高
         Serial.println("LOW");
         delay(150);
         digitalWrite(16, HIGH);           //将引脚全部 拉  高
         Serial.println("HIGH");
         flag = 0;     
        } 
  //client.stop();
}
