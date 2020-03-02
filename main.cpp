#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "InstagramStats.h"
#include <WiFiClientSecure.h>
#include <JsonStreamingParser.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN 150
#define SERVOMAX 600
WiFiClientSecure client;
InstagramStats instaStats(client);

char ssid[] = "Boteon MikroTik";
char password[] = "Boteon12345";
unsigned long delayBetweenChecks = 1000;
unsigned long whenDueToCheck = 0;
String userName = "pavla_s.r";
String inputString = "";
long followers;

int getInstagramStatsForUser()
{
  InstagramUserStats response = instaStats.getUserStats(userName);
  Serial.println(response.followedByCount);
  return response.followedByCount;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Wire.begin(21, 22);
  pwm.begin();
  pwm.setPWMFreq(60);
}

void loop() {
  // put your main code here, to run repeatedly:
  int followers = getInstagramStatsForUser();
  int angle = 0;

  if(followers == 0 && followers <= 100) angle = 0;
  if(followers > 100 && followers <= 150) angle = map(followers,100,150,0,40);
  if(followers > 150 && followers <= 250) angle = map(followers,150,250,40,73);
  if(followers > 250 && followers <= 500) angle = map(followers,250,500,73,102);
  if(followers > 500 && followers <= 1000) angle = map(followers,500,1000,102,135);
  if(followers > 1000 && followers <= 10000) angle = map(followers,1000,10000,135,161);
  if(followers > 10000 && followers <= 70000) angle = map(followers,10000,70000,161,180);
  pwm.setPWM(0,0,map(180 - angle,0,180,SERVOMIN,SERVOMAX));
  Serial.print("Angle = ");
  Serial.println(angle);
  delay(15);
}
