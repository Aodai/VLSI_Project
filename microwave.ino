#include <LiquidCrystal.h>

#define START 6
#define CANCEL 7
#define MIN10 8
#define MIN 9
#define SEC10 10
#define RUN	13

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int min10, min, sec10;
int triggerDelay = 500;
bool running = false;

void setup() {
  pinMode(RUN, OUTPUT);
  pinMode(START, INPUT);
  pinMode(CANCEL, INPUT);
  pinMode(MIN10, INPUT);
  pinMode(MIN, INPUT);
  pinMode(SEC10, INPUT);
  
  min10 = 0;
  min = 0;
  sec10 = 0;
}

void loop() {
  if(running)
    decrementTime();
  printTime();
  readButtons();
}

void readButtons() {
  static byte start_prev_state;
  static byte cancel_prev_state;
  static byte min10_prev_state;
  static byte min_prev_state;
  static byte sec10_prev_state;
  
  byte start_state = digitalRead(START);
  byte cancel_state = digitalRead(CANCEL);
  byte min10_state = digitalRead(MIN10);
  byte min_state = digitalRead(MIN);
  byte sec10_state = digitalRead(SEC10);
  
  if(min10_state && !min10_prev_state && !running) {
    if(min10+1>9)
      min10 = 0;
    else
    	min10++;
  }
  
  if(min_state && !min_prev_state && !running) {
    if(min+1>9)
      min = 0;
    else
    	min++;
  }
  
  if(sec10_state && !sec10_prev_state && !running) {
    if(sec10+10>50)
      sec10 = 0;
    else
    	sec10 += 10;
  }
  
  if(start_state && !start_prev_state && !running) {
    running = true;
    digitalWrite(RUN, HIGH);
  }
  
  if(cancel_state && !cancel_prev_state && running) {
    running = false;
    digitalWrite(RUN, LOW);
    min10 = 0;
    min = 0;
    sec10 = 0;
  }
  
  min10_prev_state = min10_state;
  min_prev_state = min_state;
  sec10_prev_state = sec10_state;
  start_prev_state = start_state;
  cancel_prev_state = cancel_state;
}

void printTime() {
  lcd.setCursor(5, 0);
  lcd.print(min10, DEC);
  lcd.print(min, DEC);
  lcd.print(":");
  if(sec10 < 10)
    lcd.print(0, DEC);
  lcd.print(sec10, DEC);
}

void decrementTime() {
  unsigned long currentTime = millis();
  static unsigned long previousTime = 0;
  const unsigned long interval = 1000;
  if(currentTime - previousTime >= interval) {
    previousTime += interval;
    if(sec10 - 1 > -1)
    	sec10--;
    if(sec10 == 0) {
      if(min - 1 >= 0) {
      	sec10 = 59;
        min--;
      }
    }
    if(min == 0) {
      if(min10 - 1 >= 0) {
        min = 9;
      	min10--;
      }
    }
  }
  if(min10 == 0 && min == 0 && sec10 == 0) {
    running = false;
    digitalWrite(RUN, LOW);
  }
}