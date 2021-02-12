#include <MsTimer2.h>
#include <TimerOne.h>
#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
const int pinA = 2;//A�� ���荞��0
const int pinB = 3;//B�� ���荞��1
const int pinZ = 21;//Z��
const int PIN_DHT = 8;//DHT���x�Z���T
const int LED_PIN = 11;
const int resolution = 4000;//����\
const int timeInterval = 1000;//�T���v�����O����
volatile long enc_count = 0;
volatile long nowPos = 0;
volatile long prePos = 0;
volatile unsigned long count = 0;
int incomingByte = 0;  // ��M�f�[�^�p
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
  attachInterrupt(0, enc_changedPinA, CHANGE); //pinA�̐M���ω��ɍ��킹�Ċ��荞�ݏ���
  attachInterrupt(1, enc_changedPinB, CHANGE); //pinB�̐M���ω��ɍ��킹�Ċ��荞�ݏ���
  //attachInterrupt(2, enc_changedPinZ, CHANGE); //pinZ�̐M���ω��ɍ��킹�Ċ��荞�ݏ���
  MsTimer2::set(timeInterval,Detect);
  MsTimer2::start();
  //Timer1.initialize(1000); //�}�C�N���b�P�ʂŐݒ�
  //Timer1.attachInterrupt(count_time);
  dht.begin();
  analogWrite(LED_PIN, 255);
}
//////////////////////////////////////////////////////////////////
void loop()
{
  //TIMSK0= 0;
  //�V���A���R���\�[���Ɍ��݂̒l���o�͂���
  //Serial.println(enc_count);
  //delayMicroseconds(10);
  /*
    if (Serial.available() > 0) { // ��M�����f�[�^�����݂���
    incomingByte = Serial.read(); // ��M�f�[�^��ǂݍ���

    Serial.print("I received:"); // ��M�f�[�^�𑗂肩����
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
//pinA�̊��荞�ݏ���
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
//pinB�̊��荞�ݏ���
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
