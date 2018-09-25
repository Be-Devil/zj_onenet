#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <HttpPacket.h>

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
const char ssid[] = "TP-LINK_D2D2";     //修改为自己的路由器用户名
const char password[] = "zxc123456789"; //修改为自己的路由器密码
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
  int size = data.length();
  int semi[3];
  int num = 0;
  if (data.charAt(0) != '#')
    return -1;

  for (int i = 1; i < size; i++) {
    if (data.charAt(i) == ';')
      semi[num++] = i;
  }

  Serial.println("num = " + String(num));
  Serial.println("semi[0] = " + String(semi[0]));
  Serial.println("semi[1] = " + String(semi[1]));
  Serial.println("semi[2] = " + String(semi[2]));

  if (num != 3)
    return -1;

  *fob = (data.substring(1, semi[0])).toInt();
  *lor = (data.substring(semi[0] + 1, semi[1])).toInt();
  *ins = (data.substring(semi[1] + 1, semi[2])).toInt();
  *scr = (data.substring(semi[2] + 1, size)).toInt();

  Serial.println("fob = " + String(*fob));
  Serial.println("lor = " + String(*lor));
  Serial.println("ins = " + String(*ins));
  Serial.println("scr = " + String(*scr));

  if (*fob < 0 || *fob > 2 || *lor < 0 || *lor > 2 || *ins < 0 || *ins > 1 || *scr < 0 || *scr > 1)
    return -1;

  return 0;
}


void setup() {
  Serial.begin(9600);
  pinMode(FRONT,  OUTPUT);
  pinMode(BACK,   OUTPUT);
  pinMode(LEFT,   OUTPUT);
  pinMode(RIGHT,  OUTPUT);
  pinMode(INSERT, OUTPUT);
  pinMode(SCRAWL, OUTPUT);
  pinMode(SIGNAL1, INPUT);
  digitalWrite(FRONT, HIGH);           //将引脚全部 拉  高
  digitalWrite(BACK, HIGH);
  digitalWrite(LEFT, HIGH);
  digitalWrite(RIGHT, HIGH);
  digitalWrite(INSERT, HIGH);
  digitalWrite(SCRAWL, HIGH);
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
  //digitalWrite(0, sig1);  //测试红外信号时占用0号gpio用来显示红外状态，正常使用时需要注释掉

  while (client.available()) {
    String line = client.readStringUntil('*');
    Serial.println(line);
    if (parseCMD(line, &fob, &lor, &ins, &scr) == 0) {
      if (ins == 1) {
        digitalWrite(INSERT, LOW);
        delay(150);
        digitalWrite(INSERT, HIGH);
      } else {
        if (scr == 1) {
          digitalWrite(FRONT, HIGH);
          digitalWrite(BACK, HIGH);
          digitalWrite(LEFT, HIGH);
          digitalWrite(RIGHT, HIGH);

          digitalWrite(SCRAWL, LOW);
          delay(150);
          digitalWrite(SCRAWL, HIGH);

        } else {
          switch (fob) {
            case 0:
              digitalWrite(FRONT, HIGH);
              digitalWrite(BACK, HIGH);
              break;
            case 1:
              digitalWrite(FRONT, LOW);
              digitalWrite(BACK, HIGH);
              //delay(150);
              //digitalWrite(FRONT, HIGH);
              break;
            case 2:
              digitalWrite(FRONT, HIGH);
              digitalWrite(BACK, LOW);
              //delay(150);
              //digitalWrite(BACK, HIGH);
              break;
          }

          switch (lor) {
            case 0:
              digitalWrite(LEFT, HIGH);
              digitalWrite(RIGHT, HIGH);
              break;
            case 1:
              digitalWrite(LEFT, LOW);
              digitalWrite(RIGHT, HIGH);
              //delay(150);
              //digitalWrite(LEFT, HIGH);
              break;
            case 2:
              digitalWrite(LEFT, HIGH);
              digitalWrite(RIGHT, LOW);
              //delay(150);
              //digitalWrite(RIGHT, HIGH);
              break;
          }
        }
      }
    } else
      Serial.println("parseCMD is wrong");
  }

  //client.stop();
}
