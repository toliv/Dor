#include "Arduino.h"
#include "DebouncedButton.h"

DebouncedButton::DebouncedButton(int buttonNumber, unsigned long debounceDel){
	this->buttonNum = buttonNumber;
	this->debounceDelay = 50;
	pinMode(buttonNumber, INPUT);
}

DebouncedButton::DebouncedButton(){
  int i =0;
}

bool DebouncedButton::isPressed(){
	int reading = digitalRead(this->buttonNum);
  // Serial.printf("Reading is %i\n", reading);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != this->lastButtonState) {
    // reset the debouncing timer
    this->lastDebounceTime = millis();
  }

  if ((millis() - this->lastDebounceTime) > this->debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != this->buttonState) {
      this->buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (this->buttonState == HIGH) {
        // Serial.println("True button press detected!");
        this->lastButtonState = reading;
        return 1;
      }
    }
  }
  this->lastButtonState = reading;
  return 0;
}