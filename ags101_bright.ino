#include <EEPROM.h>

int eeAdress = 0;     //address to save last bright

int bright = 200;
#define BRIGHT_COUNT 50
#define MAX_BRIGHT 250
#define MIN_BRIGHT 0
#define DEFAULT_BRIGHT 200

#define BLINK_TIMES 4
#define BLINK_WAIT 200

#define BTN_START 9           //  D9
#define BTN_A 10              // D10
#define BTN_B 11              // D11

#define OUTPUT_PWM 2          //  A2

bool btn_start_press = false;
bool btn_a_press = false;
bool btn_b_press = false;

bool btn_start_last = false;
bool btn_a_last = false;
bool btn_b_last = false;

void ledBlinking(int delay_time) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(delay_time);
  digitalWrite(LED_BUILTIN, LOW);
  delay(delay_time);
}

int getBright(int count, int actual_bright) {
  int value = actual_bright + count;
  if (value >= MAX_BRIGHT) { value = MAX_BRIGHT; }
  if (value <= MIN_BRIGHT) { value = MIN_BRIGHT; }
  return value;
}

void setBright(int value) {
  //set bright
  if (value >= 250 && value <= 0)   {
    errorBlinking();
    value = DEFAULT_BRIGHT;
  }
  analogWrite(OUTPUT_PWM, value);
}

void errorBlinking() {
  for (int n = 0; n<= 5; n++) {
    for (int i = 0; i<= 2; i++) {
      ledBlinking(100);
    }
    delay(300);
  }
}

void setup() {
  // Initializing PINs
  pinMode(BTN_START, INPUT_PULLUP);     //START
  pinMode(BTN_A, INPUT_PULLUP);         //A
  pinMode(BTN_B, INPUT_PULLUP);         //B

  pinMode(OUTPUT_PWM, OUTPUT);          //output pin

  //read EEPORM for last bright value
  int eeBright;  
  EEPROM.get(eeAdress,eeBright);
  if (eeBright >= 250 && eeBright <= 0) { //invalid value
    //if not a valid value, then set a default value
    //and update to the EEPROM
    EEPROM.update(eeAdress, DEFAULT_BRIGHT);
    bright = DEFAULT_BRIGHT;
  } else {
    //put the read value to bright variable
    bright = eeBright;
  }
  //set bright
  setBright(bright);
}

void loop() {
  //read PINs
  btn_start_press = !digitalRead(BTN_START);
  btn_a_press = !digitalRead(BTN_A);
  btn_b_press = !digitalRead(BTN_B);

  if (btn_start_press) {        //START PRESSED
    if (btn_a_press && btn_b_press ) {
      if (btn_a_press != btn_a_last && btn_b_press != btn_b_last) {
        //START+A+B = reset
        bright = DEFAULT_BRIGHT;
        setBright(bright);
        //blinking led a result of reset
        for (int i = 0; i <= BLINK_TIMES; i++) {
          ledBlinking(BLINK_WAIT);
        }
        //update EEPROM if it's necessary
        EEPROM.update(eeAdress,bright);
      }
    }
    else if (btn_a_press) {           //START + A
      //see if the state is the same as the last time
      if (btn_a_press != btn_a_last) {
        //Bright UP
        bright = getBright(BRIGHT_COUNT, bright);
        setBright(bright);
        //update EEPROM if it's necessary
        EEPROM.update(eeAdress,bright);
      }
    }
    else if (btn_b_press) {       //START + B
      //see if the state is the same as the last time
      
      if (btn_b_press != btn_b_last) {
        //Bright DOWN
        bright = getBright(-BRIGHT_COUNT, bright);
        setBright(bright);
        //update EEPROM if it's necessary
        EEPROM.update(eeAdress,bright);
      }
    }
  }
  
  //retain the state of each button
  btn_start_last = btn_start_press;
  btn_a_last = btn_a_press;
  btn_b_last = btn_b_press;
}
