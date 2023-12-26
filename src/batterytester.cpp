#include "Particle.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106_Particle.h>
#include <Average.h>

#define ohms 12
#define cutoffvoltage 1.0
int cutoffA0 = (cutoffvoltage/3.3) * 4096;
float milliamps = 414;

unsigned long timermillis;
Average<int> A0avg(36000);
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SH1106 display(OLED_RESET);
float mah, volts, mwh;
int seconds, aread, offcount;
unsigned long starttime, endtime;
bool hasstarted = false;
bool hasfinished = false;


#define every(interval) \
    static uint32_t __every__##interval = millis(); \
    if (millis() - __every__##interval >= interval && (__every__##interval = millis()))

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

void setup () {//Setup
RGB.control(true);
    Serial.begin(38400);
    display.begin(SH1106_SWITCHCAPVCC, 0x3C); 
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(0,1);
    display.setTextSize(1);
    display.print("Battery tester ");
    display.print(milliamps);
    display.display();

}//Setup

//************************* End of Setup function *******************************

void loop() {
if (millis() - timermillis > 10)
    {
    timermillis = millis();
        aread = analogRead(A0);
        volts = (A0avg.mean() / 4096.0) * 6.6; //3.3 * 2 because of voltage divider
        milliamps = (volts / ohms) * 1000;
        mah = milliamps * (endtime/3600000.0);
        mwh = mah * volts;

        display.clearDisplay();
        display.setCursor(0,0);
        display.print("Cutoff: ");
        display.print(cutoffA0);
        display.setCursor(0,10);
        display.print("A0: ");
        display.print(aread);  
        display.print(" (");
        display.print((aread / 4096.0) * 6.6); 
        display.print("V)");
        
        if (hasstarted){
            RGB.color(20, 0, 0);
            endtime = (millis() - starttime);
            display.setCursor(0,20);
            display.print("Time: ");
            display.print(endtime/1000);
            display.setCursor(0,30);        
            display.print("Avg: "); 
            display.print(volts); 
            display.print("V, ");
            display.print(milliamps);
            display.print("mA");
            display.setCursor(0,40);        
            display.print("mAh: ");
            display.print(mah); 
            display.setCursor(0,50);        
            display.print("mWh: ");
            display.print(mwh); 
        }
        if (hasfinished){
            RGB.color(0, 20, 0);
            display.setCursor(0,20);
            display.print("Time: ");
            display.print(endtime/1000);
            display.setCursor(0,30);        
            display.print("Avg: "); 
            display.print(volts); 
            display.print("V, ");
            display.print(milliamps);
            display.print("mA");
            display.setCursor(0,40);        
            display.print("mAh: ");
            display.print(mah); 
            display.setCursor(0,50);        
            display.print("mWh: ");
            display.print(mwh); 
            display.display();
            while(1);
        }
        display.display();
    }
    every (1000){
        if (!hasfinished) {A0avg.push(aread);}
        
    }
    if ((aread > cutoffA0) && (!hasstarted)) {
        A0avg.clear();
        A0avg.push(aread);
        hasstarted = true;
        starttime = millis();
        }

        if ((aread < cutoffA0) && (hasstarted)) {
        hasstarted = false;
        hasfinished = true;
        }       

}

//************************* End of Loop function *******************************
