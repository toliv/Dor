#ifndef DebouncedButton_h
#define DebouncedButton_h

#include "Arduino.h"

class DebouncedButton {
	public:
		bool isPressed();
		DebouncedButton(int buttonNumber, unsigned long debounceDel);
		DebouncedButton();
	private:
		int buttonNum;
		int buttonState;             // the current reading from the input pin
		int lastButtonState = LOW;   // the previous reading from the input pin
		unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
		unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
};

#endif