#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  lcd.begin(16, 2);

  updateDisplay();
}


int high_threshold = 200;
int low_threshold = 150;
bool in_blink = false;
uint32_t counter = 0;
uint32_t blink_counter = 0;
unsigned long last_millis = 0;
unsigned long MILLIS_PER_HOUR = 3600000;
unsigned long current_load = 0;

void log(char* str) {
  Serial.print("// ");
  Serial.print(str);
  Serial.println();
}

void log(char* str, double dbl) {
  Serial.print("// ");
  Serial.print(str);
  Serial.print(dbl);
  Serial.println();
}

void log(char* str, unsigned long dbl) {
  Serial.print("// ");
  Serial.print(str);
  Serial.print(dbl);
  Serial.println();
}

void loop() {
  counter++;
  int current_light = analogRead(A0);

  if (in_blink) {
    if (current_light > high_threshold) {
      in_blink = false;
      log("Going unblink: ");
      log(high_threshold);

      // Serial.print(current_light);
      // Serial.println();
    }
  } else {
    if (current_light < low_threshold) {
      in_blink = true;
      blink_counter++;
      unsigned long new_millis = millis();
      if (last_millis != 0) {
        unsigned long delta_millis = new_millis - last_millis;
        current_load = MILLIS_PER_HOUR / delta_millis;

        log("MILLIS_PER_HOUR: ", MILLIS_PER_HOUR);

        log("Delta Millis: ", delta_millis);

        log("Current Load: ", current_load);

        Serial.print(blink_counter);
        Serial.println();
      }
      updateDisplay();
      last_millis = new_millis;
    }
  }
}

void updateDisplay() {

  lcd.clear();

  lcd.setCursor(0, 0);
  // lcd.print("counter: ");
  lcd.print(counter);

  lcd.setCursor(0, 1);
  // lcd.print("blinks: ");
  lcd.print(blink_counter);
  lcd.print(" (");
  if (current_load < 1000) {
    lcd.print(current_load);
    lcd.print(" W");
  } else {
    lcd.print(current_load / 1000.0);
    lcd.print(" kW");
  }
  lcd.print(")");
}

// delay(1000);
