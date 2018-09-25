#include <ArduinoJson.h>
#include <ESP8266WiFi.h>     
#include <String.h>
#include <Servo.h>    // 声明调用Servo.h库
Servo myservo;        // 创建一个舵机对象
WiFiClient client;

char ssid[30] = "HUAWEI";     //修改为自己的路由器用户名
char password[30] = "zjwy1234"; //修改为自己的路由器密码
char onenettcp[20] = "192.168.43.1";
int  onenetport = 1811;
String null = "";
int current_Inf=0;
int next_Inf=0;
char buf[30];
int buf1[30];

int pos = 90;          // 变量pos用来存储舵机位置
int lastTime;
int timer = 0;
int current_timer = 0;
int i = 0;
char heat[] = {0xaa,0x03,0x03,0x00,0x2a};

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
    if(onenettcp[0] == '1'&&onenettcp[1] == '8'&&
        onenettcp[2] == '3'&&onenettcp[4] == '2'){
          client.print("*151331#public#lua*");   
          Serial.println("Client Connected\r"); 
        }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(16,  OUTPUT);
  pinMode(5,  OUTPUT);
  pinMode(4,  OUTPUT);
  pinMode(14,  OUTPUT);
  
  pinMode(0,  OUTPUT);
  pinMode(2,  OUTPUT);
  pinMode(15,  OUTPUT);
  pinMode(13,  OUTPUT);

  digitalWrite(16,  HIGH);
  digitalWrite(5,  HIGH);
  digitalWrite(4,  HIGH);
  digitalWrite(14,  HIGH);
  
  digitalWrite(0,  HIGH);
  digitalWrite(2,  HIGH);
  digitalWrite(15,  HIGH);
  digitalWrite(13,  HIGH);
  
  myservo.attach(12);  // 将引脚9上的舵机与声明的舵机对象连接起来
}

void loop() { 
  Init();
  while(1){
    LoupInit();
    if(client.available())
    while (client.available()) {
    buf[i] = client.read();
    i++;
    Serial.println(buf[i]);
    }
    i = 0;
      
//      String line = "";//client.readStringUntil('*');
//      buf1[0] = client.read();
//      Serial.println(line);
//      Serial.println(buf1[0],HEX);
//      if(line.substring(0,3).equals("one"))
//      line = line.substring(4,line.length());
//      char buf[line.length()];
//      line.toCharArray(buf,line.length()+1);
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
                break;
                
                case 0x03:
                null.toCharArray(onenettcp,20);
                while(buf[1]--){
                  onenettcp[line.length()-buf[1]-2] = buf[line.length()-buf[1]+2];
                }
                break;
                
                case 0x04:
                  onenetport = line.substring(4,buf[1]+1).toInt();
                break;
                
                case 0x05:
                  WiFi.disconnect();
                  Init();
               break;
                
                case 0x06:
                  client.stop();
                  delay(100);
                  if (client.connect(onenettcp, onenetport)) {
                    Serial.println("Client Connected\r");
                  }
               break;
          }
        break;
          
        case 0x03://妖娆花
          switch(buf[3]){
            case 0x00:
              heat[3] = 1;
              client.print("*151331#EnchantingFlower#lua*");     //修改为自己的设备id号，设备授权信息, 脚本名称
            break;
            case 0x01:
              if(current_timer == 0){
               digitalWrite(16, LOW); 
               delay(200);
               digitalWrite(16, HIGH); 
               current_timer = 1;
              } 
            break;
            case 0x02:
            if(current_timer > 0){
             digitalWrite(16, LOW); 
             delay(200);
             digitalWrite(16, HIGH);  
             current_timer = 0;     
            }
            break;
            case 0x03:
             digitalWrite(5, LOW); 
            break;            
            case 0x04:
             digitalWrite(5, HIGH); 
            break;
            case 0x05:
             digitalWrite(4, LOW); 
            break;
            case 0x06:
             digitalWrite(4, HIGH); 
            break;
          }
        break;
          
        case 0x04://逗猫棒
          switch(buf[3]){
            case 0x00:
              heat[3] = 2;
              client.print("*151331#SteeringEngine#lua*");     //修改为自己的设备id号，设备授权信息, 脚本名称
            break;
            case 0x01:
            if(pos == 90)
              for(pos = 90; pos>60; pos-=1) {    // 舵机从180°转回到0°，每次减小1°                               
                 myservo.write(pos);        // 写角度到舵机     
                 delay(15);                 // 延时15ms让舵机转到指定位置
              }
            break;
            case 0x02:
            if(pos == 60)
             for(pos = 60; pos < 90; pos += 1){    // 舵机从0°转到180°，每次增加1°          
                myservo.write(pos);           // 给舵机写入角度   
                delay(15);                    // 延时15ms让舵机转到指定位置
              }
            break;
          }
          break;
          
        case 0x05://扫地机器人
          switch(buf[3]){
            case 0x00:
              heat[3] = 3;
              client.print("*151331#FMoppingRobot#lua*");    
            break;
            case 0x01:
             digitalWrite(16, LOW); 
            break;
            case 0x02:
             digitalWrite(16, HIGH); 
          }
          break;
          
        case 0x06://玩具车
          switch(buf[3]){
            case 0x00:
              heat[3] = 4;
              client.print("*151331#ToyCar#lua*");    
            break;
            case 0x01:
             digitalWrite(4, LOW); 
            break;            
            case 0x02:
             digitalWrite(16, LOW); 
            break;
            case 0x03:
             digitalWrite(5, LOW); 
            break;
            case 0x04:
             digitalWrite(14, LOW); 
            break;
            case 0x05:
              digitalWrite(4, HIGH); 
              digitalWrite(16, HIGH); 
              digitalWrite(5, HIGH); 
              digitalWrite(14, HIGH); 
            break;
          }
          break;
          
        case 0x07://红外激光
          switch(buf[3]){
            case 0x00:
              heat[3] = 5;
              client.print("*151331#ToyCar#lua*");    
            break;
            case 0x01:
            case 0x02:
            case 0x03:
              next_Inf = buf[3];
              while(next_Inf%3 != current_Inf%3){
               delay(100);
               digitalWrite(16, LOW);        
               delay(100);
               digitalWrite(16, HIGH);           
               current_Inf++;
              }
            break;
          }
            break;
          
        case 0x08://喷水罐
          switch(buf[3]){
            case 0x00:
              heat[3] = 6;
              client.print("*151331#WaterSprayTank#lua*");    
            break;
            
            case 0x01:
             digitalWrite(16, LOW); 
            break;
            case 0x02:
             digitalWrite(16, HIGH); 
             Serial.println("HIGH");
          }
          break;
          
        case 0x09://玩具鼠
          switch(buf[3]){
            case 0x00:
              heat[3] =7;
              client.print("*151331#mouse#lua*");    
            break;
            
            case 0x01:
              digitalWrite(14, LOW); 
            break;
            case 0x02:
              digitalWrite(14, LOW); 
              digitalWrite(16, LOW); 
            break;
            case 0x03:
              digitalWrite(14, LOW); 
              digitalWrite(5, LOW); 
            break;
            case 0x04:
              digitalWrite(4, LOW); 
            break;
            case 0x05:
              digitalWrite(4, HIGH); 
              digitalWrite(16, HIGH); 
              digitalWrite(5, HIGH); 
              digitalWrite(14, HIGH); 
            break;
          }
          break;
          
        case 0x0a://玩具蛇
          switch(buf[3]){
            case 0x00:
              heat[3] =8;
              client.print("*151331#snake#lua*");    
            break;
            case 0x01:
              digitalWrite(16, LOW);
              delay(100); 
              digitalWrite(16, HIGH); 
            break;
            case 0x02:
              digitalWrite(5, LOW); 
              delay(100); 
              digitalWrite(5, HIGH); 
            break;
            case 0x03:
              digitalWrite(4, LOW); 
              delay(100); 
              digitalWrite(4, HIGH); 
            break;
            case 0x05:
              digitalWrite(16, LOW);
              delay(100); 
              digitalWrite(16, HIGH); 
            break;
          }
          break;
          
        case 0x0b://骰子机
          heat[3] = 9;
          switch(buf[3]){
              
          }
          break;
          
        case 0x0c://智能小车
          switch(buf[3]){
            case 0x00:
              heat[3] =10;
              client.print("*151331#SmartCar#lua*");    
            break;
            case 1:
              digitalWrite(16,LOW);  
              digitalWrite(5,HIGH);     
              digitalWrite(4,HIGH);  
              digitalWrite(14,LOW);
            break;
            
            case 2:
              digitalWrite(16,HIGH);  
              digitalWrite(5,LOW);     
              digitalWrite(4,HIGH);  
              digitalWrite(14,LOW);
            break;
            
            case 3:
              digitalWrite(16,LOW);  
              digitalWrite(5,HIGH);     
              digitalWrite(4,LOW);  
              digitalWrite(14,HIGH);
            break;
            
            case 4:
              digitalWrite(16,HIGH);  
              digitalWrite(5,LOW);     
              digitalWrite(4,LOW);  
              digitalWrite(14,HIGH);
            break;
            
            case 5:
              digitalWrite(16,LOW);  
              digitalWrite(5,LOW);     
              digitalWrite(4,LOW);  
              digitalWrite(14,LOW);
            break;
          }
          break;
          
            
        }
      }
      
    


/*
 * 心跳
 */

  if (millis() - lastTime >= 1000) {
    timer++;

    if (current_timer > 0) {
      current_timer++; 
    }
    if(current_timer == 11)
     {
       digitalWrite(16, LOW);           
       delay(200);
       digitalWrite(16, HIGH);         
       current_timer = 0;     
      }     
    if(timer%5 == 4){
      c_send(heat);
    }
    lastTime = millis();
  }
 
  }
  
}
