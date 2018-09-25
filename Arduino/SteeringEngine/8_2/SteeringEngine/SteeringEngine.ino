#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <Servo.h>    // 声明调用Servo.h库
Servo myservo;        // 创建一个舵机对象

#define FRONT  0   //管脚D8
#define BACK   12  //管脚D6
#define LEFT   2   //管脚D9
#define RIGHT  13  //管脚D7
#define INSERT 4   //管脚D4
#define SCRAWL 5   //管脚D3

#define SIGNAL1 16 //管脚D2

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
const char onenettcp[] = "183.230.40.40";
const int  onenetport = 1811;

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

void setup() {
  Serial.begin(9600);
  myservo.attach(16);  // 将引脚9上的舵机与声明的舵机对象连接起来
  connectWifi();
  if (client.connect(onenettcp, onenetport)) {
    Serial.println("Client Connected\r");
    client.print("*151331#SteeringEngine#lua*");     //修改为自己的设备id号，设备授权信息, 脚本名称
    delay(1000);
  }
}

void loop() {
  LoupInit();
  while (client.available()) {
    String line = client.readStringUntil('*');
    Serial.println(line);
      if (parseCMD(line, &fob, &lor, &ins, &scr) == 0) {
        if (fob == 1) {
          if(flag++%2 == 0)
             for(pos = 0; pos < 30; pos += 1){    // 舵机从0°转到180°，每次增加1°          
                myservo.write(pos);           // 给舵机写入角度   
                delay(15);                    // 延时15ms让舵机转到指定位置
              }
          else
            for(pos = 30; pos>=1; pos-=1) {    // 舵机从180°转回到0°，每次减小1°                               
               myservo.write(pos);        // 写角度到舵机     
               delay(15);                 // 延时15ms让舵机转到指定位置
            }
        }
        
    }
  }
}
