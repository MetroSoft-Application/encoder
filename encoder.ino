#include <MsTimer2.h>
#include <TimerOne.h>
#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
const int pinA = 2;//A相 割り込み0
const int pinB = 3;//B相 割り込み1
const int pinZ = 21;//Z相
const int PIN_DHT = 8;//DHT温度センサ
const int LED_PIN = 11;
const int resolution = 4000;//分解能
const int timeInterval = 1000;//サンプリング周期
volatile long enc_count = 0;
volatile long nowPos = 0;
volatile long prePos = 0;
volatile unsigned long count = 0;
int incomingByte = 0;  // 受信データ用
DHT dht(PIN_DHT,DHT11);
int sequence = 0;
unsigned int counter = 0;
//////////////////////////////////////////////////////////////////
void setup()
{
  lcd.init();
  lcd.begin(20, 4);
  lcd.backlight();
  Serial.begin(9600);
  pinMode(pinA,INPUT);
  pinMode(pinB,INPUT);
  pinMode(pinZ,INPUT);
  attachInterrupt(0, enc_changedPinA, CHANGE); //pinAの信号変化に合わせて割り込み処理
  attachInterrupt(1, enc_changedPinB, CHANGE); //pinBの信号変化に合わせて割り込み処理
  //attachInterrupt(2, enc_changedPinZ, CHANGE); //pinZの信号変化に合わせて割り込み処理
  MsTimer2::set(timeInterval,Detect);
  MsTimer2::start();
  //Timer1.initialize(1000); //マイクロ秒単位で設定
  //Timer1.attachInterrupt(count_time);
  dht.begin();
  analogWrite(LED_PIN, 255);
}
//////////////////////////////////////////////////////////////////
void loop()
{
  //TIMSK0= 0;
  //シリアルコンソールに現在の値を出力する
  //Serial.println(enc_count);
  //delayMicroseconds(10);
  /*
    if (Serial.available() > 0) { // 受信したデータが存在する
    incomingByte = Serial.read(); // 受信データを読み込む

    Serial.print("I received:"); // 受信データを送りかえす
    Serial.println(incomingByte, DEC);
    
  }
  */
}
//////////////////////////////////////////////////////////////////
void PrintTemperatureHumidity() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humidity:    ");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Temperature: ");
  lcd.print(temperature);
  lcd.print("*C");
  lcd.setCursor(0, 2);
  lcd.print("counter:      ");
  lcd.print(counter);
  counter+=2;
  lcd.blink();
  LEDBlink();
}
//////////////////////////////////////////////////////////////////
void LEDBlink() {
  for (int i = 0; i <= 255; i++) {
    analogWrite(LED_PIN, i);
    delay(1);
  }
  analogWrite(LED_PIN, 0);
}
//////////////////////////////////////////////////////////////////
void count_time()
{
  ++count;
}
//////////////////////////////////////////////////////////////////
void count_inc()
{
  //if(digitalRead(pinZ) == 1)  ++enc_count;
  //else ++enc_count;
  ++enc_count;
}
//////////////////////////////////////////////////////////////////
void count_dec()
{
  //if(digitalRead(pinZ) == 1) --enc_count;
  //else --enc_count;
  --enc_count;
}
//////////////////////////////////////////////////////////////////
//pinAの割り込み処理
void enc_changedPinA()
{
  if(digitalRead(pinA))
  {
    if(digitalRead(pinB)) count_dec();
    else count_inc();
  } else {
    if(digitalRead(pinB)) count_inc();
    else count_dec();
  }
}
//pinBの割り込み処理
void enc_changedPinB()
{
  if(digitalRead(pinB))
  {
    if(digitalRead(pinA)) count_inc();
    else count_dec();
  } else {
    if(digitalRead(pinA)) count_dec();
    else count_inc();
  }
}
//////////////////////////////////////////////////////////////////
int Detect(){
  nowPos = enc_count;
  float speedRPM = (nowPos - prePos)* (1000/timeInterval) * 60 / resolution ;
  Serial.println(speedRPM);
  Serial.println(nowPos);
  Serial.println(prePos);
  prePos = nowPos;
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  //PrintTemperatureHumidity();
  LEDBlink();
  return speedRPM;
}
