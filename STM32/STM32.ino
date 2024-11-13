//CPU、内存模拟仪表项目，采用STM32F103C8T6单片机，91C4-3V电压表。
#include <string>
#include "RGB.h"
const int CPUOutPin = PA9;  //CPU输出电压表接口，对应STM32单片机PA9引脚
const int RAMOutPin = PA10; //RAM输出电压表接口，对应STM32单片机PA10引脚
int CPU = 0;//CPU使用率
int RAM = 0;//内存使用率
String val = "";//用于存储串口接收到的数据，根据数据格式为20个字符。
int list[3]={};
int rgb_flag=5;
//串口输出显示，调试使用，可屏蔽
const uint32_t interval = 1000;
uint32_t lastTriggerTime = 0;
void rgb(void);
void str_chuli(void);
void setup() {
  Serial.begin(9600);//串口波特率9600
  pinMode(CPUOutPin, PWM);
  pinMode(RAMOutPin, PWM);
  strip.begin();
  delay(2000);
}
void loop() {
  //串口接收数据
  while (Serial.available() > 0) {
    val += char(Serial.read());  //每次读一个char字符，并相加
    delay(5);
  }
  if (val.length()>0){
    str_chuli();
  }
  rbg();   
  delay(17);
}
void str_chuli(){
  //数据处理
  int n=0;
    for(int i=0;i<val.length()-1;i++){
      if (val[i]!=','){
        if(val[i]!=' '){
        list[n]=(int)(list[n]*10+val[i]-'0');}
      }else{
        n++;
      }
    }
  CPU=list[0];
  RAM=list[1];
  //rgb_flag=list[2];
  //串口输出显示，调试使用，可屏蔽
  uint32_t currentTime = millis();
  if (currentTime - lastTriggerTime >= interval) {
    lastTriggerTime = currentTime;
    Serial.print("CPU:");
    Serial.print(CPU);
    Serial.print(',');
    Serial.println(map(CPU, 0, 110, 0, 65535));
    Serial.print("RAM:");
    Serial.print(RAM);
    Serial.print(',');
    Serial.println(map(RAM, 0, 110, 0, 65535));
  }
  pwmWrite(CPUOutPin, map(CPU, 0, 110, 0, 65535));//pwm输出CPU
  pwmWrite(RAMOutPin, map(RAM, 0, 110, 0, 65535));//pwm输出RAM
  list[0]=0;
  list[1]=0;
  val = "";//清空串口接收值
}
void rbg(){
unsigned long currentMillis = micros();
    if (currentMillis - pixelPrevious >= pixelInterval) {  //  Check for expired time
    pixelPrevious = currentMillis;                       //  Run current frame
    switch (rgb_flag) {
      case 0:
        rgb_off();
        break;
      case 1:
        if (CPU > 0) {
          pixelInterval = 150+CPU;
          strip2();
        } 
        break;
      case 2:
        pixelInterval = 100+RAM;
        strip2();
        break;
      case 3:
        pixelInterval = 20;
        strip3();
        break;
      case 4:
      pixelInterval = 20;
        strip1();
        break;
      case 5:
        rainbow1();
        pixelInterval = 20;
        break;
      case 6:
        rainbow2();
        pixelInterval =20;
        break;
      case 7:
        pulse_rainbow1();
        pixelInterval = 20;
        break;
    }
  }
}