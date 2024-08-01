#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define BUTTON_PIN 2

LiquidCrystal_I2C lcd(0x3F, 16, 2);

byte customPlayer[] = {
  B01110,
  B01010,
  B01110,
  B10100,
  B01110,
  B00101,
  B01010,
  B10001
};

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Button Test");
  delay(200);
  lcd.clear();
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  lcd.setCursor(0, 0);
  lcd.print("Button State:");

  lcd.setCursor(0, 1);
  if (buttonState == LOW) {
    lcd.print("PRESSED     ");
  } else {
    lcd.print("NOT PRESSED ");
  }

  delay(100); // Update every 100 ms
}
