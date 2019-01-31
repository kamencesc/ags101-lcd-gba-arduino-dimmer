// LCD Bright Dimmer for a AGS-101 LCD
// in a GBA bakclight MOD
// 
// Code by: Kamencesc
// 
// Site: http://www.kamencesc.com/
// Git: https://github.com/kamencesc/ags101-lcd-gba-arduino-dimmer/
// 31-01-2019
//--------------------------------------------------------------------

#include <EEPROM.h>

int eeAdress = 0;     //address to save last bright

int bright = 200;
#define BRIGHT_COUNT 50
#define MAX_BRIGHT 250
#define MIN_BRIGHT 0
#define DEFAULT_BRIGHT 200

#define BTN_START 9
#define BTN_A 10
#define BTN_B 11

#define BLINK_TIMES 3

bool btn_start_press = false;
bool btn_a_press = false;
bool btn_b_press = false;

bool btn_start_last = false;
bool btn_a_last = false;
bool btn_b_last = false;

int getBright(int count, int actual_bright) {
  int value = actual_bright + count;
  if (value >= MAX_BRIGHT) { value = MAX_BRIGHT; }
  if (value <= MIN_BRIGHT) { value = MIN_BRIGHT; }
  return value;
}

void setBright(int value) {
  //set bright
}

void setup() {
  // put your setup code here, to run once:
  pinMode(BTN_START, INPUT_PULLUP);     //START
  pinMode(BTN_A, INPUT_PULLUP);         //A
  pinMode(BTN_B, INPUT_PULLUP);         //B

  //power off led
  digitalWrite(LED_BUILTIN, LOW);

  int eeBright;  
  EEPROM.get(eeAdress,eeBright);
  if (eeBright >= 250 && eeBright <= 0) { //invalid value
    //write the default value
    EEPROM.update(eeAdress, DEFAULT_BRIGHT);
    bright = DEFAULT_BRIGHT;
  } else {
    //put the read value to bright variable
    bright = eeBright;
  }
  setBright(bright);
}

void loop() {
  // put your main code here, to run repeatedly:

  btn_start_press = !digitalRead(BTN_START);
  btn_a_press = !digitalRead(BTN_A);
  btn_b_press = !digitalRead(BTN_B);

  if (btn_start_press) {        //START PRESSED
    if (btn_a_press && btn_b_press ) {
      if (btn_a_press != btn_a_last && btn_b_press != btn_b_last) {
        //reset
        bright = DEFAULT_BRIGHT;
        setBright(bright);
        //update EEPROM if it's necessary
        EEPROM.update(eeAdress,bright);
        //blink led as reset signal
        for (int i = 0; i++; i <= BLINK_TIMES ) {
          delay(200);
          digitalWrite(LED_BUILTIN, HIGH);
          delay(200);
          digitalWrite(LED_BUILTIN, LOW);
        }
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
