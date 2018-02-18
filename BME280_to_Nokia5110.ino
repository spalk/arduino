#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Adafruit_BME280.h>
#include <Wire.h>

// I2C
// SDA - A4 (D18)
// SCL - A5 (D19)
Adafruit_BME280 bme; 

// D7 - Serial clock out (CLK)
// D6 - Serial data out (DIN)
// D5 - Data/Command select (DC)
// D4 - LCD chip select (CE)
// D3 - LCD reset (RST)
Adafruit_PCD8544 lcd = Adafruit_PCD8544(7, 6, 5, 4, 3);

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
boolean pushed = false;

// Refresh rait on main screen
long interval = 5000;
long previousMillis = 0;

// Lines x-coordinates on main screen 
int x_tempr = 18;
int x_press = 28;
int x_humid = 38;

// Log data arrays
int log_values_number = 82;
int log_tempr[82];
int log_press[82];
int log_humid[82];

// Frequency of logging data
unsigned long previousLogMillis = millis();
long interval_tempr = 5000;
long interval_press = 5000;
long interval_humid = 5000;

// Log counters
int count_tempr = log_values_number;
int count_press = log_values_number;
int count_humid = log_values_number;


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

  log_sensor_values();

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
    lcd.clearDisplay();
    pushed = true;
    if (select < 4){
      select += 1;
    } else {
      select = 0;
    }
    Serial.print(select);
    delay(500);
  }

  unsigned long currentMillis = millis();
  switch(select){
    case 0:
      if(currentMillis - previousMillis > interval || pushed) {
        previousMillis = currentMillis;
        if (pushed){pushed = false;}
      
        draw_interface(select);

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
        
        lcd.display();
        lcd.clearDisplay();
      }
      break;
    case 1:
      if(currentMillis - previousMillis > interval || pushed) {
        previousMillis = currentMillis;
        if (pushed){pushed = false;}
      
        
        lcd.clearDisplay();

        draw_interface(select);

        draw_graph(log_tempr, 23, 35);
        //lcd.display();
        

      }
      break;
    case 2:
      if(currentMillis - previousMillis > interval || pushed) {
        previousMillis = currentMillis;
        if (pushed){pushed = false;}
      
        draw_interface(select);
      }
      break;
    case 3:
      if(currentMillis - previousMillis > interval || pushed) {
        previousMillis = currentMillis;
        if (pushed){pushed = false;}
      
        draw_interface(select);
      }
      break;
    case 4:
      if(currentMillis - previousMillis > interval || pushed) {
        previousMillis = currentMillis;
        if (pushed){pushed = false;}
      
        draw_interface(select);
      }
      break;
  }
}

void set_text(int x, int y, String text, int color){
  lcd.setTextColor(color);
  lcd.setCursor(x,y);
  lcd.println(text);
  lcd.display();
}

void draw_interface(int s){
    // Frame
    lcd.drawRect(1, 1, 83, 47, BLACK);
    lcd.display();
    
    // Menu header generating
    switch(s){
      case 0:
        set_text( 6, 5, "#", BLACK);  
        lcd.fillRect(19, 2, 83, 13, BLACK);
        set_text(23, 5, "t", WHITE);
        set_text(39, 5, "P", WHITE);
        set_text(55, 5, "H", WHITE);
        set_text(71, 5, "Y", WHITE);
        break;
      case 1:
        lcd.fillRect(2, 2, 16, 13, BLACK);
        set_text( 6, 5, "#", WHITE);  
        set_text(23, 5, "t", BLACK);
        lcd.fillRect(35, 2, 82, 13, BLACK);
        set_text(39, 5, "P", WHITE);
        set_text(55, 5, "H", WHITE);
        set_text(71, 5, "Y", WHITE);
        break;
      case 2:
        lcd.fillRect(2, 2, 32, 13, BLACK);
        set_text( 6, 5, "#", WHITE);  
        set_text(23, 5, "t", WHITE);
        set_text(39, 5, "P", BLACK);
        lcd.fillRect(51, 2, 82, 13, BLACK);
        set_text(55, 5, "H", WHITE);
        set_text(71, 5, "Y", WHITE);
        break;
      case 3:
        lcd.fillRect(2, 2, 48, 13, BLACK);
        set_text( 6, 5, "#", WHITE);  
        set_text(23, 5, "t", WHITE);
        set_text(39, 5, "P", WHITE);
        set_text(55, 5, "H", BLACK);
        lcd.fillRect(67, 2, 82, 13, BLACK);
        set_text(71, 5, "Y", WHITE);
        break;
      case 4:
        lcd.fillRect(2, 2, 64, 13, BLACK);
        set_text( 6, 5, "#", WHITE);  
        set_text(23, 5, "t", WHITE);
        set_text(39, 5, "P", WHITE);
        set_text(55, 5, "H", WHITE);
        set_text(71, 5, "Y", BLACK);
        break;
    }

    // Menu vertical separators
    lcd.drawLine(18, 2, 18, 13, WHITE);
    lcd.drawLine(34, 2, 34, 13, WHITE);
    lcd.drawLine(50, 2, 50, 13, WHITE);
    lcd.drawLine(66, 2, 66, 13, WHITE);
    lcd.display();

    lcd.setTextSize(1);

    lcd.setTextColor(BLACK);
    lcd.display();
}

void log_sensor_values(){
  unsigned long curLogMillis = millis();

  if (curLogMillis - previousLogMillis > interval_tempr){
    previousLogMillis = curLogMillis;

    int cur_tempr = bme.readTemperature();
    
    for (int i=0; i<log_values_number; i++){
      log_tempr[i] = log_tempr[i+1];
    }
    log_tempr[log_values_number-1] = cur_tempr;
  
      
    //for(int i = 0; i<log_values_number; i++){
    //  Serial.print(log_tempr[i]);
    //  Serial.print(" ");
    //}
    //Serial.println(" ");


    //Serial.print(cur_tempr);
    //Serial.print(" - ");
    //Serial.println(count_tempr);
  }

  //int cur_press = bme.readPressure()/133.3;
  //int cur_humid = bme.readHumidity();

  
  //Serial.println(cur_press);
  //Serial.println(cur_humid);
}


void draw_graph(int data[], int min_val, int max_val){

  // Calculating number of pixels in 1 vals. Zone for graph:
  // 16 - top Y coordinate of screen
  // 45 - bottom Y coordinate of screen
  int top_y = 16;
  int bot_y = 45;

  int pix = round((bot_y-top_y)/(max_val-min_val));
  for (int i; i<log_values_number; i++){
    if (data[i] < max_val || data[i] > min_val){
      lcd.drawPixel(i+2, top_y+pix*(max_val-data[i]), BLACK);
      lcd.display();
      /*Serial.print(i+2);
      Serial.print(" ");
      Serial.println(top_y+pix*(max_val-data[i]));
      Serial.println(data[i]);
      Serial.println(pix); */
    }
    
  }


}
