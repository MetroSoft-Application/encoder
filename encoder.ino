#include <MsTimer2.h>
#include <Wire.h>

const int pinA = 2;    // A
const int pinB = 3;    // B
const int pinZ = 21;   // Z
const int LED_PIN = 13;
const int resolution = 4000;  //
const int timeInterval = 500; //
volatile long enc_count = 0;
volatile long nowPos = 0;
volatile long prePos = 0;
volatile int LED_STATE = LOW;
void enc_changedPinA();
void enc_changedPinB();
//////////////////////////////////////////////////////////////////
void setup()
{
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
}
//////////////////////////////////////////////////////////////////
void loop()
{
}

//////////////////////////////////////////////////////////////////
void LEDBlink()
{
  LED_STATE = !LED_STATE;
  digitalWrite(LED_PIN, LED_STATE);
  // digitalWrite(LED_PIN, LOW);
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
  nowPos = enc_count;
  float speedRPM = (nowPos - prePos) * (1000 / timeInterval) * 60 / resolution;
  Serial.println("speedRPM");
  Serial.println(speedRPM);
  Serial.println("nowPos");
  Serial.println(nowPos);
  Serial.println("prePos");
  Serial.println(prePos);
  prePos = nowPos;
  LEDBlink();
  Serial.println("LED_STATE");
  Serial.println(LED_STATE);
  return speedRPM;
}
