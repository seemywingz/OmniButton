#include "OmniButton.h"

OmniButton::OmniButton(int buttonPin) {
  _buttonPin = buttonPin;
  _singlePressCallback = nullptr;
  _doublePressCallback = nullptr;
  _longPressCallback = nullptr;
  pinMode(_buttonPin, INPUT_PULLUP);
}

int OmniButton::pin() { return _buttonPin; }
int OmniButton::pressCount() { return _pressCount; }
void OmniButton::setVerbose(bool verbose) { _verbose = verbose; }
void OmniButton::setSinglePressCallback(void (*callback)()) {
  _singlePressCallback = callback;
}
void OmniButton::setDoublePressDelay(unsigned long doublePressDelay) {
  _doublePressDelay = doublePressDelay;
}
void OmniButton::setHoldDelay(unsigned long holdDelay) {
  _holdDelay = holdDelay;
}
void OmniButton::setMultiPressCallback(void (*callback)()) {
  _multiPressCallback = callback;
}
void OmniButton::setDoublePressCallback(void (*callback)()) {
  _doublePressCallback = callback;
}
void OmniButton::setLongPressDuration(unsigned long duration) {
  _longPressDuration = duration;
}
void OmniButton::setLongPressCallback(void (*callback)()) {
  _longPressCallback = callback;
}
void OmniButton::setHoldCallback(void (*callback)()) {
  _holdCallback = callback;
}

void OmniButton::listen() {
  if (digitalRead(_buttonPin) == LOW) {  // button pressed
    if (!_buttonPressed) {
      if (_verbose) {
        Serial.println("\nButton Pressed");
      }
      _buttonPressedTime = millis();
      _buttonReleasedTime = 0;
      _buttonPressed = true;
      _pressCount++;

      unsigned int holdDuration = 0;
      while (digitalRead(_buttonPin) == LOW && holdDuration < _holdDelay) {
        interrupts();
        holdDuration = millis() - _buttonPressedTime;
      }
      while (digitalRead(_buttonPin) == LOW) {
        if (_verbose) {
          Serial.println("Holding...");
        }
        if (_holdCallback != nullptr) {
          _holdCallback();
        }
      }
    }
  } else {  // button released
    if (_buttonPressed) {
      _buttonPressed = false;
      _buttonReleasedTime = millis();
      unsigned long pressDuration = _buttonReleasedTime - _buttonPressedTime;
      if (_verbose) {
        Serial.println("Button Released");
        Serial.print("Overall Duration: ");
        Serial.println(pressDuration);
      }

      if (pressDuration < _longPressDuration) {
        interrupts();
        delay(_doublePressDelay);
        switch (_pressCount) {
          case 1:
            if (_verbose) {
              Serial.println("- Single Press -");
            }
            if (_singlePressCallback != nullptr) {
              _singlePressCallback();
            }
            break;
          case 2:
            if (_verbose) {
              Serial.println("-- Double Press --");
            }
            if (_doublePressCallback != nullptr) {
              _doublePressCallback();
            }
            break;
          default:
            if (_pressCount > 2) {
              if (_verbose) {
                Serial.println("--- Multi Press ---");
              }
              if (_multiPressCallback != nullptr) {
                _multiPressCallback();
              }
            }
            break;
        }
        _pressCount = 0;

      } else if (pressDuration >= _longPressDuration) {
        if (_verbose) {
          Serial.println("~~~~~~ Long Press ~~~~~~");
        }
        if (_longPressCallback != nullptr) {
          _longPressCallback();
        }
        _pressCount = 0;
      }
    }
  }
}
