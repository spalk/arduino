#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Adafruit_BME280.h>
#include <Wire.h>

Adafruit_BME280 bme; // I2C
// SDA - A4 (D18)
// SCL - A5 (D19)

Adafruit_PCD8544 lcd = Adafruit_PCD8544(7, 6, 5, 4, 3);
// D7 - Serial clock out (CLK)
// D6 - Serial data out (DIN)
// D5 - Data/Command select (DC)
// D4 - LCD chip select (CE)
// D3 - LCD reset (RST)

// D9 - backlight
const int backlightPin = 9;
boolean backlight = false;

// D11 - backlight button 
const int backlightButtonPin = 11; 
int BacklightButtonState = 0;
int lastBacklightButtonState = 0;

long interval = 5000;
long previousMillis = 0;

void setup() {
  lcd.begin();
  lcd.setContrast(60);
  lcd.clearDisplay();
  Wire.begin();
  delay(1000);
  bme.begin();
  Serial.begin(9600);

  pinMode(backlightPin,OUTPUT);
  digitalWrite(backlightPin,HIGH); //Turn Backlight OFF
  
  pinMode(backlightButtonPin, INPUT_PULLUP);
  digitalWrite(backlightButtonPin,HIGH);
  lastBacklightButtonState = 1;
}

void loop(void) { 
  BacklightButtonState = digitalRead(backlightButtonPin);
  
  if (BacklightButtonState != lastBacklightButtonState){
    if (backlight){
      backlight = false;
    } else {
      backlight = true;
    }
    delay(500);
  }
  
  if (backlight){
    digitalWrite(backlightPin, LOW);
  } else {
    digitalWrite(backlightPin, HIGH);
  }

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;   
  
    lcd.setTextSize(1);
    lcd.setCursor(8,1);
    lcd.print(bme.readPressure()/133.3);lcd.print("  mmHg");
    lcd.setCursor(8,17);
    lcd.print(bme.readTemperature());
    lcd.setCursor(69,14);
    lcd.print((char)9);lcd.print("C");
    lcd.setCursor(8,34);
    lcd.print(bme.readHumidity());
    set_text(69,34,"%",BLACK);
    lcd.clearDisplay();
  }
}

void set_text(int x,int y,String text,int color){
  lcd.setTextColor(color);
  lcd.setCursor(x,y);
  lcd.println(text);
  lcd.display();
}
