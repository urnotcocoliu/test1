#include <Adafruit_NeoPixel.h>
#include <Wire.h>//调用收发数据所使用的库函数
#include "I2Cdev.h"
#include <SHT2x.h> 
#include <avr/sleep.h>
#include <avr/power.h>
#include <IRremote.h>
#include <U8glib.h>
//-------字体设置，大、中、小
#define setFont_L u8g.setFont(u8g_font_7x13)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_fixed_v0r)
#define setFont_SS u8g.setFont(u8g_font_fub25n)
#define PIXEL_PIN    A0//定义LED的接口
#define PIXEL_COUNT  1//定义LED的个数
#define INTERVAL_LCD 20//定义OLED刷新时间间隔
#define temp1    20//#define定义常量
#define temp2    26
#define val_max 255
#define val_min 0   
#define TEM 0x1FEF807    //增加+
#define HUM 0x1FE708F   //减少-
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
int RECV_PIN = 10;//定义红外接收传感器的接口
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long lcd_time = millis();                 //OLED刷新时间计时器
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);     //设置OLED型号
long previousMillis = 0;        // 存储LED最后一次的更新
long interval = 1000*60*60*2;           // 闪烁的时间间隔（毫秒）
unsigned long currentMillis=0;
float senser_tem;
float senser_hum;
void Interrupt(void) {
    /* 中断唤醒 */
 
    /*当中断引脚为低电平时关闭中断*/
    Serial.println("hey guys!");
    detachInterrupt(0);
}
void enterSleep(void) {

    attachInterrupt(0, Interrupt, LOW);
    delay(100);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
    sleep_disable();
}
void read()
{
  senser_tem = SHT2x.GetTemperature() ;//把获得的温度值赋给变量sensor_tem
  senser_hum = SHT2x.GetHumidity() ;//把获得的湿度值赋给变量sensor_light
  Serial.println(senser_tem);//将数据从Arduino传递到PC且单独占据一行，此数据可在串口监视器中看到
  Serial.print("--");   
  Serial.println(SHT2x.GetHumidity());
  delay(100);
}
void setup() {
    Serial.begin(9600);
    irrecv.enableIRIn(); // 启动红外解码
    Serial.println("Initialisation complete.");
    pinMode(RECV_PIN, INPUT);
    Wire.begin(); 
    strip.begin();
    strip.show(); // 关灯
}

void loop() {
  read();//调用自定义函数
    if (irrecv.decode(&results)) {
      Serial.println(results.value, HEX);
  
      u8g.firstPage();
      do {
          setFont_L;
          u8g.setPrintPos(4, 10);
          u8g.print("remperature ");
          u8g.setPrintPos(4,25);
          u8g.print(senser_tem);
          u8g.setPrintPos(4, 40);
          u8g.print("humidity");
          u8g.setPrintPos(4, 55);
          u8g.print(senser_hum);

      }while( u8g.nextPage() );
      
      irrecv.resume(); // 接收下一个值
      previousMillis = currentMillis;
    }

    if(currentMillis - previousMillis > interval) {
      previousMillis = currentMillis; 
      Serial.println("Entering sleep");
      enterSleep();
    }
  if(currentMillis<previousMillis)
    previousMillis=currentMillis=0;

  }
 
n
