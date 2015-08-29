#include <SD.h>
#include <SPI.h>
#include <Time.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS 2


LiquidCrystal_I2C lcd(0x20,16,2);  // set the LCD address to 0x20 for a 16 chars and 2 line display
File myFile;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
tmElements_t tm;

char buff[17];
byte subSecond = 0;
float temperature = 0.00;
unsigned long time = 0;

void setup() {
	sensors.begin();
	Serial.begin(9600);

	lcd.init();                      // initialize the lcd 
	lcd.backlight();
}

void loop() {
	RTC.read(tm);
	if (subSecond != second() ){
		time = millis();
		
		subSecond = second();
		sensors.requestTemperatures();
		temperature = sensors.getTempCByIndex(0);
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
	sprintf(buff, "%02d/%02d/%02d", tmYearToCalendar(tm.Year), tm.Month, tm.Day);
	lcd.print(buff);
}

void displayTemp(){
	byte iTemp, decTemp;
	iTemp = (byte)temperature;
	decTemp = (temperature - iTemp) * 100;

	clearBuff();
	sprintf(buff, "%02d:%02d:%02d %02d.%02dC", tm.Hour, tm.Minute, tm.Second, iTemp, decTemp);
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