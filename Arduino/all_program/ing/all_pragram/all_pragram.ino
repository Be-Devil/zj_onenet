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
String line = "";
char s1;
char s2;


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
    if(client.available()){
      null.toCharArray(buf,30);
      while (client.available()) {
        buf[i] = client.read();
        Serial.print(buf[i],HEX);
        Serial.print(" ");
        if(buf[i] == 0x2a&&buf[1] == i-1){//&&buf[1] == i-2&&buf[0] == 0xaa
          break;
          }
        i++;
        /*
         * onenet
         */
        if(buf[0] == 'o'&&buf[1] == 'n'&&
          buf[2] == 'e'&&buf[3] == '='){
            i = 0;
            line = client.readStringUntil('*');
            Serial.println(line);
            Serial.println(line.length());
            line.toUpperCase();
            while(1){
              if(line.charAt(0)>64)
                s1 = line.charAt(0) - 7;
              else
                s1 = line.charAt(0);
              if(line.charAt(1)>64)
                s2 = line.charAt(1) - 7;
              else
                s2 = line.charAt(1);
              
              buf[i] = (s1-0x30)*16 + (s2-0x30);

              Serial.print(buf[i],HEX);
              Serial.print(" ");
              if(line.length() > 2)
                line = line.substring(5,line.length());
              else
                break;
              i++;
            }
          break;
        }
        
      }
    Serial.println("");
    }
      
//      String line = "";//client.readStringUntil('*');
//      buf1[0] = client.read();
//      Serial.println(line);
//      Serial.println(buf1[0],HEX);
//      if(line.substring(0,3).equals("one"))
//      line = line.substring(4,line.length());
//      char buf[line.length()];
//      line.toCharArray(buf,line.length()+1);
      if(buf[i] == 0x2a&&buf[1] == i-1&&buf[0] == 0xaa){
        switch(buf[2]){
            case 0x00:
              heat[3] = 1;
              client.stop();
              delay(100);
              client.connect(onenettcp, onenetport);
              client.print("*151331#public#lua*");     
          case 0x01:
             switch(buf[3]){
                case 0x01:
                null.toCharArray(ssid,30);
                while(buf[1]-- != 3){
                  ssid[i-2-buf[1]] = buf[i+2-buf[1]];
                }
                Serial.println(ssid);
                break;
                
                case 0x02:
                null.toCharArray(password,30);
                while(buf[1]-- != 3){
                  password[i-2-buf[1]] = buf[i+2-buf[1]];
                }
                Serial.println(password);
                break;
                
                case 0x03:
                null.toCharArray(onenettcp,20);
                while(buf[1]-- != 3){
                  onenettcp[i-2-buf[1]] = buf[i+2-buf[1]];
                }
                Serial.println(onenettcp);
                break;
                
                case 0x04:
                onenetport = 0;
                while(buf[1]-- != 3){
                  onenetport = onenetport*10+(buf[i+2-buf[1]] - 0x30);
                }
                Serial.println(onenetport);
                break;
                
                case 0x05:
                  WiFi.disconnect();
                  Init();
                  Serial.println("5");
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
              client.stop();
              delay(100);
              client.connect(onenettcp, onenetport);
              client.print("*151331#EnchantingFlower#lua*");     
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
              client.stop();
              delay(100);
              client.connect(onenettcp, onenetport);
              client.print("*151331#SteeringEngine#lua*");     
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
              client.stop();
              delay(100);
              client.connect(onenettcp, onenetport);
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
              client.stop();
              delay(100);
              client.connect(onenettcp, onenetport);
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
              client.stop();
              delay(100);
              client.connect(onenettcp, onenetport);
              client.print("*151331#InfraredLaser#lua*");    
            break;
            case 0x01:
            case 0x02:
            case 0x03:
              next_Inf = buf[3] - 1;
              Serial.println(next_Inf);
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
              client.stop();
              delay(100);
              client.connect(onenettcp, onenetport);
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
              heat[3] = 7;
              client.stop();
              delay(100);
              client.connect(onenettcp, onenetport);
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
              heat[3] = 8;
              client.stop();
              delay(100);
              client.connect(onenettcp, onenetport);
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
            case 0x00:
              heat[3] = 9;
              client.stop();
              delay(100);
              client.connect(onenettcp, onenetport);
              client.print("*151331#sack#lua*");    
            break;
          }
          break;
          
        case 0x0c://智能小车
          switch(buf[3]){
            case 0x00:
              heat[3] = 10;
              client.stop();
              delay(100);
              client.connect(onenettcp, onenetport);
              client.print("*151331#Toycar#lua*");    
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
      i = 0;
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
