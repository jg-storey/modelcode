//Arduino Code for U.S.S. Reliant Shield's Up display by J. Storey.
//MAX7219 - using Led Control library to display all leds one by one
#include <LedControl.h>
#include "pitches.h"

const int DIN = 11;
const int CS = 7;
const int CLK = 13;

LedControl lc=LedControl(DIN, CLK, CS,0);

const int buttonPin = 2;    // the number of the pushbutton pin
const int speakerPin = 8;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

int dt = 70;
int toneCount = 0;
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
bool shieldsUp = false;
bool hold = true;

int melody[] = {
  NOTE_B4
};

void setup() {
  
  lc.shutdown(0,false);
  lc.setIntensity(0,0);
  lc.clearDisplay(0);
  pinMode(buttonPin, INPUT);
  //tone(8, melody[0], 2*dt);
  
}

void loop() {

  if(hold){
    //wait for button press

    // read the state of the switch into a local variable:
    int reading = digitalRead(buttonPin);
  
    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:
  
    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState) {
      // reset the debouncing timer
      lastDebounceTime = millis();
    }
  
    if ((millis() - lastDebounceTime) > debounceDelay) {
      // whatever the reading is at, it's been there for longer than the debounce
      // delay, so take it as the actual current state:
  
      // if the button state has changed:
      if (reading != buttonState) {
        buttonState = reading;
  
        // only toggle the hold if the new button state is HIGH
        if (buttonState == HIGH) {
          hold = false;
        }
      }
    }
  
    // save the reading. Next time through the loop, it'll be the lastButtonState:
    lastButtonState = reading;
    
  }else{
    if(shieldsUp){
      //lower shields
      for(int j=0;j<8;j++){
         for(int i=0;i<6;i++){
         lc.setLed(0,j,i,false);
         if(toneCount%2==0){
          tone(speakerPin, melody[0], dt);
         }
         toneCount++; 
         delay(dt);
       }
      }
      shieldsUp = false;
    }else{
      //raise shields
      for(int j=0;j<8;j++){
         for(int i=0;i<6;i++){
           lc.setLed(0,j,i,true);
           if(toneCount%2==0){
            tone(speakerPin, melody[0], dt);
           }
           toneCount++; 
           delay(dt);
         }
      }
      shieldsUp = true;
    }
      hold = true;
      noTone(speakerPin);
      toneCount=0;
  }
}
