#include <SD.h>
#include <SPI.h>
#include <Time.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS 2
#define BUFFLEN 17


LiquidCrystal_I2C lcd(0x20,16,2);  // set the LCD address to 0x20 for a 16 chars and 2 line display
File myFile;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
tmElements_t tm;
String sdDate ="";

char buff[BUFFLEN];
byte subSecond = 0, times = 0;
float temperature = 0.00;
unsigned long time = 0;

void setup() {
	sensors.begin();
	Serial.begin(9600);

	lcd.init();                      // initialize the lcd 
	lcd.backlight();

	//pinMode(10, OUTPUT);
    if (!SD.begin()) {
    	lcd.setCursor(14, 0);
	    lcd.print("SD");
	}
}

void loop() {
	RTC.read(tm);
	if (subSecond != second() ){
		time = millis();
		

		subSecond = second();
		sensors.requestTemperatures();
		temperature = sensors.getTempCByIndex(0);

		if (times > 10){
			times = 0;
			saveDate();
			sdDate = "";
		}
		else{
			times++;

			clearBuff();
		    sprintf(buff, "%d-%02d-%02d,%02d:", tmYearToCalendar(tm.Year), tm.Month, tm.Day, tm.Hour);
		    sdDate += buff;
		    
		    byte iTemp, decTemp;
			iTemp = (byte)temperature;
			decTemp = (temperature - iTemp) * 100;

		    clearBuff();
		    sprintf(buff, "%02d:%02d,%02d.%02d\n\r", tm.Minute, tm.Second, iTemp, decTemp);
		    Serial.print(buff);
		    sdDate += buff;
		}

		
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
	sprintf(buff, "%02d-%02d-%02d", tmYearToCalendar(tm.Year), tm.Month, tm.Day);
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

void saveDate(){
    clearBuff();
    sprintf(buff, "%02d%02d%02d%02d.txt", tm.Month, tm.Day, tm.Hour, tm.Minute);

    myFile = SD.open(buff, FILE_WRITE);
    // myFile = SD.open("log.txt", FILE_WRITE);
    if(myFile){
    	lcd.setCursor(14, 0);
        lcd.print("  ");
    }
    else{
    	lcd.setCursor(14, 0);
        lcd.print("FL");
        return;
    }

    myFile.print(sdDate);
    myFile.close();
}

void clearBuff(){
	for(byte i = 0; i < BUFFLEN; i++)
		buff[i] = ' ';
}

// void clearLCD(){
// 	clearBuff();
// 	lcd.setCursor(0, 0);
// 	lcd.print(buff);
// 	lcd.setCursor(0, 1);
// 	lcd.print(buff);
// }