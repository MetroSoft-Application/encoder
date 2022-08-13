#include <MsTimer2.h>
#include <TimerOne.h>
#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
const int pinA = 2;    // A
const int pinB = 3;    // B
const int pinZ = 21;   // Z
const int PIN_DHT = 8; // DHT
const int LED_PIN = 13;
const int resolution = 4000;  //
const int timeInterval = 500; //
volatile long enc_count = 0;
volatile long nowPos = 0;
volatile long prePos = 0;
volatile unsigned long count = 0;
int incomingByte = 0; //
DHT dht(PIN_DHT, DHT11);
int sequence = 0;
unsigned int counter = 0;
volatile int LED_STATE = LOW;
void enc_changedPinA();
void enc_changedPinB();
//////////////////////////////////////////////////////////////////
void setup()
{

  // lcd.init();
  // lcd.begin(20, 4);
  // lcd.backlight();
  Serial.begin(19200);
  Serial.println(0);
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinZ, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  attachInterrupt(0, enc_changedPinA, CHANGE); // pinAの信号変化に合わせて割り込み処理
  attachInterrupt(1, enc_changedPinB, CHANGE); // pinBの信号変化に合わせて割り込み処理
  // attachInterrupt(2, enc_changedPinZ, CHANGE); //pinZの信号変化に合わせて割り込み処理
  MsTimer2::set(timeInterval, Detect);
  MsTimer2::start();
  // Timer1.initialize(1000); //マイクロ秒単位で設定
  // Timer1.attachInterrupt(count_time);
  // dht.begin();
  // analogWrite(LED_PIN, 255);
}
//////////////////////////////////////////////////////////////////
void loop()
{
  // TIMSK0= 0;
  //シリアルコンソールに現在の値を出力する
  // Serial.println(enc_count);
  // delayMicroseconds(10);
  /*
    if (Serial.available() > 0) { // 受信したデータが存在する
    incomingByte = Serial.read(); // 受信データを読み込む

    Serial.print("I received:"); // 受信データを送りかえす
    Serial.println(incomingByte, DEC);

  }
  */
}
//////////////////////////////////////////////////////////////////
void PrintTemperatureHumidity()
{
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
  counter += 2;
  lcd.blink();
  LEDBlink();
}
//////////////////////////////////////////////////////////////////
void LEDBlink()
{
  LED_STATE = !LED_STATE;
  digitalWrite(LED_PIN, LED_STATE);
  // digitalWrite(LED_PIN, LOW);
}
//////////////////////////////////////////////////////////////////
void count_time()
{
  ++count;
}
//////////////////////////////////////////////////////////////////
void count_inc()
{
  // if(digitalRead(pinZ) == 1)  ++enc_count;
  // else ++enc_count;
  ++enc_count;
}
//////////////////////////////////////////////////////////////////
void count_dec()
{
  // if(digitalRead(pinZ) == 1) --enc_count;
  // else --enc_count;
  --enc_count;
}
//////////////////////////////////////////////////////////////////
// pinAの割り込み処理
void enc_changedPinA()
{
  if (digitalRead(pinA))
  {
    if (digitalRead(pinB))
      count_dec();
    else
      count_inc();
  }
  else
  {
    if (digitalRead(pinB))
      count_inc();
    else
      count_dec();
  }
}
// pinBの割り込み処理
void enc_changedPinB()
{
  if (digitalRead(pinB))
  {
    if (digitalRead(pinA))
      count_inc();
    else
      count_dec();
  }
  else
  {
    if (digitalRead(pinA))
      count_dec();
    else
      count_inc();
  }
}
//////////////////////////////////////////////////////////////////
int Detect()
{
  // Serial.println(0);
  nowPos = enc_count;
  float speedRPM = (nowPos - prePos) * (1000 / timeInterval) * 60 / resolution;
  Serial.println("speedRPM");
  Serial.println(speedRPM);
  Serial.println("nowPos");
  Serial.println(nowPos);
  Serial.println("prePos");
  Serial.println(prePos);
  prePos = nowPos;
  // float humidity = dht.readHumidity();
  // float temperature = dht.readTemperature();
  // PrintTemperatureHumidity();
  LEDBlink();
  Serial.println("LED_STATE");
  Serial.println(LED_STATE);
  return speedRPM;
}
