/*
  Blink
  The basic Energia example.
  Turns on an LED on for one second, then off for one second, repeatedly.
  Change the LED define to blink other LEDs.

  Hardware Required:
  * LaunchPad with an LED

  This example code is in the public domain.
*/
#include "Nokia/Nokia.h"
#include "Nokia/Images.h"

// most launchpads have a red LED
#define LED RED_LED

//see pins_energia.h for more LED definitions
//#define LED GREEN_LED

int nokiaPinMap[eNbrOfPins]={
		P3_6,
		P3_5,
		P3_4,
		P2_2,
		P2_4,
		P4_3,
};

void writeNokiaPin(int pin,bool state){
	digitalWrite(nokiaPinMap[pin],state);
}

void initNokiaPin(){
	for(int i=0;i<eNbrOfPins;i++){
		pinMode(nokiaPinMap[i],OUTPUT);
	}
}

char lettre='a';

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
	cb_registerPins(writeNokiaPin);
	initNokiaPin();
	lcdReset();
	lcdInit();
	lcdChar('A');
	gr_drawImage(LoZimg);
	gr_updateScreen();
  pinMode(LED, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
	if(!digitalRead(PUSH1))
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
  lcdChar(lettre++);
}
