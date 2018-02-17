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

// D11 - backlight button pin
const int backlightButtonPin = 11; 
int BacklightButtonState = 0;
int lastBacklightButtonState = 0;

// D12 - select button pin
const int selectButtonPin = 12; 
int selectButtonState = 0; 
int lastSelectButtonState = 0;
int select = 0;

long interval = 5000;
long previousMillis = 0;

// Lines x-coordinates
int x_tempr = 18;
int x_press = 28;
int x_humid = 38;

void setup() {
  lcd.begin();
  lcd.setContrast(60);
  lcd.clearDisplay();
  Wire.begin();
  delay(500);
  bme.begin();
  Serial.begin(9600);

  pinMode(backlightPin,OUTPUT);
  digitalWrite(backlightPin,HIGH); //Turn Backlight OFF
  
  pinMode(backlightButtonPin, INPUT_PULLUP);
  digitalWrite(backlightButtonPin,HIGH);
  lastBacklightButtonState = 1;

  pinMode(selectButtonPin, INPUT_PULLUP);
  digitalWrite(selectButtonPin,HIGH);
  lastSelectButtonState = 1; 
}

void loop(void) { 
  BacklightButtonState = digitalRead(backlightButtonPin);
  selectButtonState = digitalRead(selectButtonPin);
  
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

  if (selectButtonState != lastSelectButtonState){
    if (select < 3){
      select += 1;
    } else {
      select = 0;
    }
    Serial.print(select);
    delay(500);
  }




  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    
    // Frame
    lcd.drawRect(1, 1, 83, 47, BLACK);
    
    // Menu black background
    lcd.fillRect(2, 2, 82, 13, BLACK);

    // Menu vertical separators
    lcd.drawLine(18, 2, 18, 13, WHITE);
    lcd.drawLine(34, 2, 34, 13, WHITE);
    lcd.drawLine(50, 2, 50, 13, WHITE);
    lcd.drawLine(66, 2, 66, 13, WHITE);

    // Active white rect
    lcd.fillRect(2, 2, 17, 13, WHITE);

    lcd.setTextSize(1);

    // Menu Symbols
    set_text( 6, 5, "#", BLACK);
    set_text(23, 5, "t", WHITE);
    set_text(39, 5, "P", WHITE);
    set_text(55, 5, "H", WHITE);
    set_text(71, 5, "Y", WHITE);


    lcd.setTextColor(BLACK);

    // Temperature line
    lcd.setCursor(8, x_tempr);
    lcd.print(bme.readTemperature()); 
    lcd.setCursor(50, x_tempr);
    lcd.print((char)9);lcd.print("C");

    // Pressure line
    lcd.setCursor(8,x_press);
    lcd.print(bme.readPressure()/133.3);lcd.print(" mmHg");

    // Humidity line
    lcd.setCursor(8,x_humid);
    lcd.print(bme.readHumidity());
    set_text(50,x_humid,"%",BLACK);

    lcd.clearDisplay();
  }
}

void set_text(int x, int y, String text, int color){
  lcd.setTextColor(color);
  lcd.setCursor(x,y);
  lcd.println(text);
  lcd.display();
}
