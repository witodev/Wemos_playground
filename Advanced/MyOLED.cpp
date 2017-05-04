// 
// 
// 

#include "MyOLED.h"

void MyOLEDClass::init()
{
	display = new Adafruit_SSD1306(OLED_RESET);

	// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
	display->begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
												// init done

												// Show image buffer on the display hardware.
												// Since the buffer is intialized with an Adafruit splashscreen
												// internally, this will display the splashscreen.
	display->display();
	delay(2000);

	// Clear the buffer.
	display->clearDisplay();
	delay(1000);
}

void MyOLEDClass::print(const char * msg)
{
	display->clearDisplay();
	display->setTextSize(4);
	display->setTextColor(WHITE);
	display->setCursor(0, 20);
	display->println(msg);
	display->display();
}

void MyOLEDClass::print(float temp)
{
	display->clearDisplay();
	display->setTextSize(4);
	display->setTextColor(WHITE);
	display->setCursor(0, 20);
	display->println(temp);
	display->display();
}


MyOLEDClass MyOLED;

