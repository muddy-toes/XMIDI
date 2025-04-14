
midi-over-usb control box to interface with xtoys for controlling an ossm running the xtoys v2 firmware

Pics in doc/

The buttons on the box, as pictured in doc/as-built.jpg:

  - The left green is speed mode, the right is position mode.
  - The yellow is pause/unpause
  - The red is full stop
  - The encoder is speed in speed mode
  - The left slider is depth
  - The right slider is stroke
  - The blue is to set minimum depth
  - The black resets minimum depth back to zero

Full stroke is left all the way down and right all the way up.

Left at the bottom and right halfway will be a half stroke.  If you then push
left up 20%, you'll have a stroke from 20-70.

If you then push the blue button, it'll set that to the minimum, which makes
the current slider position 20% up from where it currently is and it will ramp
to its new increased position, so if you want to set the minimum and keep it
there, hit blue and then drop the slider to zero.  The black button reset will
also ramp the low to adjust.


# Caution:

**Build this and use it at your own risk.**

Connect it and play with it while there isn't a living being on the business
end of the machine, yourself included.

Always be able to move away from the machine or to quickly cut its power!


# Xtoys:

The xtoys script is in xtoys/.  See its README for details.  I did not create Xtoys nor
do I have any affiliation with them.


# Components:

  - Arduino Pro Micro 5V
  - 6x 12mm momentary push buttons
  - 1x rotary encoder
  - 2x 75mm slider potentiometers, B10K, B103

Component reference info in doc/


# Parts:

STL files are in the printables/ directory


# Wiring notes:

  - Button - speed mode - pin 2 - key 36
  - Button - position mode - pin 3 - key 37
  - Button - Pause - pin 4 - key 38
  - Button - Stop/retract - pin 5 - key 39

  - Button - set min - pin 6 - key 40
  - Button - zero min - pin 7 - key 41

  - Encoder - Speed - pins 14 & 15

  - Slider - Stroke min - pin A9
  - Slider - Stroke range - pin A8

All components buttons are connected between the given pin and gnd

The sliders are 10k B pots between gnd and 5V

