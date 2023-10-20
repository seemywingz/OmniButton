#ifndef Button1_h
#define Button1_h

#include <Arduino.h>

class OmniButton {
 public:
  OmniButton(int buttonPin);
  void setDoublePressDelay(unsigned long doublePressDelay);
  void setLongHoldDelay(unsigned long longHoldDelay);
  void setLongPressDuration(unsigned long duration);
  void setSinglePressCallback(void (*callback)());
  void setDoublePressCallback(void (*callback)());
  void setTriplePressCallback(void (*callback)());
  void setLongPressCallback(void (*callback)());
  void setLongHoldCallback(void (*callback)());
  void setVerbose(bool verbose);
  void isr();
  int pin();

 private:
  volatile int _pressCount = 0;
  volatile bool _buttonPressed = false;
  volatile bool _runSinglePress = true;
  volatile bool _waitForDoublePress = false;
  volatile unsigned long _buttonPressedTime = 0;
  volatile unsigned long _buttonReleasedTime = 0;
  volatile unsigned long _longPressDuration = 300;
  volatile unsigned long _holdDelay = 90;
  volatile unsigned long _doublePressDelay = 210;

  int _buttonPin;
  bool _verbose = false;
  void (*_singlePressCallback)();
  void (*_doublePressCallback)();
  void (*_triplePressCallback)();
  void (*_longPressCallback)();
  void (*_longHoldCallback)();
};

#endif
