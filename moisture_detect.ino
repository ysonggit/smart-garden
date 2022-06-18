#include <LiquidCrystal_I2C.h>

#include <Wire.h>

LiquidCrystal_I2C lcd (0x27, 16, 2);

#define sensor A2
// |---------|--------|--------|---------|
// 305      330      400       500       580
// TooWet    |-------OK--------|  TooDry |    
const int WET = 330; // lowerbound of the wet soil
const int DRY = 490; // upperbound of the dry soil
const int MIN = 305; // put sensor into water cup, got 305
const int MAX = 580; // put sensor into almost dry soil, got 565
const int GREEN = 13;
const int YELLOW = 12;
const int RED = 11;



void setup()
{
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);
  Serial.begin(9600);
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
}

void resetLED() {
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
}

float normalize(int val, int Min, int Max) {
  return (val-Min)*1.0/(Max-Min)*1.0;
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = analogRead(sensor);
  Serial.println(value);
  resetLED();
  if (value >= DRY )
  {
    Serial.println("Too Dry");
    digitalWrite(RED, HIGH);
    delay(1000);
  }
  if (value > WET && value < DRY)
  {
    Serial.println("OK");
    digitalWrite(GREEN, HIGH);
    delay(1000);
  }
  if (value <= WET)
  {
    Serial.println("Too Wet");
    digitalWrite(YELLOW, HIGH);
    delay(1000);
  }
 
  lcd.setCursor(1, 0);
  lcd.print("Soil Moisture");
  lcd.setCursor(3, 1);
  float perc = (1.0 - normalize(value, MIN, MAX)) * 100.0;
  lcd.print(round(perc));
  lcd.print(" ");
  lcd.setCursor(6, 1);
  lcd.print("%");
  lcd.setCursor(9, 1);
  if (value <= DRY ) {
    lcd.print("Good");
  } else {
    lcd.print("Dry!");
  }
}
