#include "Particle.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106_Particle.h>
#include <Average.h>
#include "blynk.h"

#define ohms 12
#define cutoffvoltage 1.0
int cutoffA0 = ((cutoffvoltage/2)/3.3) * 4096;


unsigned long timermillis, timermillis2;
Average<int> A0avg(36000);
Average<int> instA0avg(60);
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SH1106 display(OLED_RESET);
double milliamps, mw, volts, mwh, mwhtot;
double instmilliamps, instmw, instvolts, mah, matot, joules;
int aread, offcount;
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

    if (millis() - timermillis2 >= 60000)
    {
        
            instvolts = (instA0avg.mean() / 4096.0) * 6.6; //3.3 * 2 because of voltage divider
            instmilliamps = (instvolts / ohms) * 1000;
            matot += instmilliamps;
            joules += instmilliamps * instvolts;
            instA0avg.clear();
            timermillis2 = millis();
    }
    
if (millis() - timermillis > 10)
    {
    timermillis = millis();
        aread = analogRead(A0);
        
        volts = (A0avg.mean() / 4096.0) * 6.6; //3.3 * 2 because of voltage divider
        milliamps = (volts / ohms) * 1000;
        mw = milliamps * volts;
        mwh = mw * (endtime/3600000.0);
        mwhtot = joules / 60;
        mah = matot / 60;

        display.clearDisplay();
        display.setCursor(0,0);
        display.print("===BATTERY TESTER===");
        //if ((hasstarted)||(hasfinished)){display.print(instmws/3600);}
        display.setCursor(0,10);
        display.print("A: ");
        display.print(aread);  
        display.print("/");
        display.print(cutoffA0);
        display.print(" (");
        display.print((aread / 4096.0) * 6.6); 
        display.print("V)");
        
        if (hasstarted){
            RGB.color(20, 0, 0);
            endtime = (millis() - starttime);
            display.setCursor(0,20);
            display.print("Time: ");
            display.print(endtime/1000);
            display.print(", ");
            display.print(mah);
            display.print("mAh");
            display.setCursor(0,30);        
            display.print("Avg: "); 
            display.print(volts); 
            display.print("V, ");
            display.print(milliamps);
            display.print("mA");
            display.setCursor(0,40);        
            display.print("Avg mWh: ");
            display.print(mwh); 
            display.setCursor(0,50);        
            display.print("Tot mWh: ");
            display.print(mwhtot); 
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
            display.print("Avg mWh: ");
            display.print(mwh); 
            display.setCursor(0,50);        
            display.print("Tot mWh: ");
            display.print(mwhtot); 
            display.display();
            while(1);
        }
        display.display();
    }

        every(1000){
        if (!hasfinished) {
        instA0avg.push(aread);
         A0avg.push(aread);
        }
        
    }




    if ((aread > cutoffA0) && (!hasstarted)) {
        A0avg.clear();
        A0avg.push(aread);
        instA0avg.clear();
        instA0avg.push(aread);
        hasstarted = true;
        mwhtot = 0;
        starttime = millis();
        timermillis2 = millis();
        }

        if ((aread < cutoffA0) && (hasstarted)) {
        hasstarted = false;
        hasfinished = true;
        }       

}

//************************* End of Loop function *******************************
