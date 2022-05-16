#define LEFT_PWM   14          // LEFT motor PWM             GPIO14(D5)
#define LEFT_DIR   12          // LEFT motor DIRECTION       GPIO12(D6)
#define RIGHT_PWM  13          // RIGHT motor PWM            GPIO13(D7)
#define RIGHT_DIR  15          // RIGHT motor DIRECTION      GPIO15(D8)
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//#include <HTTPClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
String command,command_sudo;             //String to store app command state.
int speedCar = 0;
int speedCar1 = 0;  
int speedCar2 = 0;
String motor1 = "";
String motor2 = "";

Servo servo;
const uint16_t port = 6666;
const char *host = "192.168.0.8";
WiFiClient client;
//HTTPClient http;
void setup() {
  servo.attach(16);
  pinMode(LEFT_PWM, OUTPUT);
  pinMode(LEFT_DIR, OUTPUT);
  pinMode(RIGHT_PWM, OUTPUT);
  pinMode(RIGHT_DIR, OUTPUT);
  Serial.begin(57600);
  Serial.println("Connecting...\n");
  WiFi.mode(WIFI_STA);
  WiFi.begin("D-Link_DIR-600M", "uasdtu@2021"); // change it to your ussid and password
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  while (!client.connect(host, port)) {
    Serial.println("Waiting...");
    Serial.println();
  }
  Serial.println("Connected");
}
void drop(){
  Serial.println("drop");
  servo.write(180);
}

void goAhead() {
  digitalWrite(LEFT_DIR, HIGH);
  analogWrite(LEFT_PWM, speedCar1);

  digitalWrite(RIGHT_DIR, HIGH);
  analogWrite(RIGHT_PWM, speedCar2);
}


void stopRobot() {
  Serial.println("stopRobot");
  digitalWrite(LEFT_DIR, LOW);
  analogWrite(LEFT_PWM, 0);

  digitalWrite(RIGHT_DIR, LOW);
  analogWrite(RIGHT_PWM, 0);
}

void loop() {
  
  if(client.available()){
    command_sudo = client.read();
    command = "";
    while(command_sudo != ":"){
      if(client.available()){
        command = command + command_sudo;
    digitalWrite(RIGHT_DIR, HIGH);
        command_sudo = client.read();
      }
    }
        
    int sep = 0;
    for(int i=0;i<command.length();i++){
      if(command[i]==','){
        sep=i;
        break;
      }
    }
    
    motor1 = command.substring(1,sep);
    motor2 = command.substring(sep+1,command.length());
    speedCar1 = motor1.toInt();
    speedCar2 = motor2.toInt();
    
    if(command[0]=='F'){
      Serial.println("Going ahead.\nLeft PWM = ");
      Serial.println(speedCar1);
      Serial.println("Right PWM = ");
      Serial.println(speedCar2);
      goAhead();
    }
    if(command[0]=='S') stopRobot();
    if(command[0]=='X') drop();
  }

  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnected");
    client.stop();
    client.connect(host,port);
  }
}
