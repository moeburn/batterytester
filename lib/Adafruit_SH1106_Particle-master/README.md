# SH1106 Driver for Particle
* Actual work was done by (forked from): https://github.com/wonho-maker/Adafruit_SH1106
* Ported By Gerrit Coetzee, gerritcoetzee.com 2017
* Stack-overflowed heavily from Paul Kourany's port of the Adafruit_SSD1306 library for particle 


Adafruit_SH1106
===============

Adafruit graphic library for SH1106 driver lcds.

some small oled lcd use SH1106 driver.

I change the adafruit SSD1306 to SH1106 

SH1106 driver similar to SSD1306. thus, just change the display() method. 
 
However, SH1106 driver don't provide several functions such as scroll commands.


 Adafruit-GFX-Library
 https://github.com/adafruit/Adafruit-GFX-Library
