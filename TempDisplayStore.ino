#include <SD.h>
#include <SPI.h>
#include <Time.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define ONE_WIRE_BUS 2


LiquidCrystal_I2C lcd(0x20,16,2);  // set the LCD address to 0x20 for a 16 chars and 2 line display
File myFile;
DHT dht;
tmElements_t tm;

char buff[17];
byte subSecond = 0;
float temperature = 0.00, humidity = 0.00;
unsigned long time = 0;

void setup() {
	dht.setup(ONE_WIRE_BUS);
	Serial.begin(9600);

	lcd.init();                      // initialize the lcd 
	lcd.backlight();
}

void loop() {
	RTC.read(tm);
	if (subSecond != tm.Second){
		time = millis();

		subSecond = tm.Second;
		clearLCD();
		lcd.setCursor(0, 0);
		displayTime();
		lcd.setCursor(0, 1);
		displayTemp();

		time = millis() - time;
		Serial.println(time);
	}
}

void displayTime(){
	clearBuff();
	sprintf(buff, "%02d/%02d %02d:%02d:%02d", tm.Month, tm.Day, tm.Hour, tm.Minute, tm.Second);
	lcd.print(buff);
}

void displayTemp(){
	temperature = dht.getTemperature();
	humidity = dht.getHumidity();
	
	byte iTemp, decTemp, iHum, decHum;
	iTemp = (byte)temperature;
	decTemp = (temperature - iTemp) * 100;
	iHum = (byte)humidity;
	decHum = (humidity - iHum) * 100;

	clearBuff();
	sprintf(buff, "%02d.%02dC %02d.%02d%%", iTemp, decTemp, iHum, decHum);
	lcd.print(buff);
}

void clearBuff(){
	for(byte i = 0; i < 17; i++)
		buff[i] = ' ';
}

void clearLCD(){
	clearBuff();
	lcd.setCursor(0, 0);
	lcd.print(buff);
	lcd.setCursor(0, 1);
	lcd.print(buff);
}