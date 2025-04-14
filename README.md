
midi-over-usb control box to interface with xtoys for controlling an ossm running the xtoys v2 firmware

Pics in doc/

# Components:

  - Arduino Pro Micro 5V
  - 6x 12mm momentary push buttons
  - 1x rotary encoder
  - 2x 75mm slider potentiometers, B10K, B103

Component reference info in doc/

# Parts:

STL files are in the printables/ directory


# Wiring notes:

Button - speed mode - pin 2 - key 36
Button - position mode - pin 3 - key 37
Button - Pause - pin 4 - key 38
Button - Stop/retract - pin 5 - key 39

Button - set min - pin 6 - key 40
Button - zero min - pin 7 - key 41

Encoder - Speed - pins 14 & 15

Slider - Stroke min - pin A9
Slider - Stroke range - pin A8

All components buttons are connected between the given pin and gnd

The sliders are 10k B pots between gnd and 5V

