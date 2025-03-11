/*
  Based on Sketch built by Gustavo Silveira (aka Music Nerd)
  Modified by Dolce Wang
  Modified by muddytoes

  This code is only for Arduinos that use ATmega32u4 (like Micro, Pro Micro, Leonardo...)
*/
#include <Arduino.h>
#include "MIDIUSB.h"

// Buttons
const int NButtons = 6; //***  total number of buttons
const int buttonPin[NButtons] = {2, 3, 4, 5, 6, 7}; //*** define Digital Pins connected from Buttons to Arduino; (ie {10, 16, 14, 15, 6, 7, 8, 9, 2, 3, 4, 5}; 12 buttons)
int buttonCState[NButtons] = {};        // stores the button current value
int buttonPState[NButtons] = {};        // stores the button previous value
unsigned long lastDebounceTime[NButtons] = {0};  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    //** the debounce time; increase if the output flickers

// Pots
//const int NPots = 7; //*** total number of pots (knobs and faders)
//const int potPin[NPots] = {A9, A8, A7, A6, A3, A2, A1}; //*** define Analog Pins connected from Pots to Arduino; Leave nothing in the array if 0 pots {}
const int NPots = 2; //*** total number of pots (knobs and faders)
const int potPin[NPots] = {A9, A8}; //*** define Analog Pins connected from Pots to Arduino; Leave nothing in the array if 0 pots {}
const int potMin[NPots] = {0, 10};
const int potMax[NPots] = {90, 100};
int potCState[NPots] = {0}; // Current state of the pot; delete 0 if 0 pots
int potPState[NPots] = {0}; // Previous state of the pot; delete 0 if 0 pots
int potVar = 0; // Difference between the current and previous state of the pot
int midiCState[NPots] = {0}; // Current state of the midi value; delete 0 if 0 pots
int midiPState[NPots] = {0}; // Previous state of the midi value; delete 0 if 0 pots
const int TIMEOUT = 300; //* Amount of time the potentiometer will be read after it exceeds the varThreshold
const int varThreshold = 10; //* Threshold for the potentiometer signal variation
boolean potMoving = true; // If the potentiometer is moving
unsigned long PTime[NPots] = {0}; // Previously stored time; delete 0 if 0 pots
unsigned long timer[NPots] = {0}; // Stores the time that has elapsed since the timer was reset; delete 0 if 0 pots

// Encoder
const int NEncoders = 1;
const int encoderPinA[NEncoders] = {15};
const int encoderPinB[NEncoders] = {14};
int encoderPState[NEncoders] = {0};
int encoderCState[NEncoders] = {0};
int encoderVal[NEncoders] = {0};

// MIDI Assignments 
byte midiCh = 1; //* MIDI channel to be used
byte note = 36; //* Lowest note to be used; 36 = C2; 60 = Middle C
byte cc = 1; //* Lowest MIDI CC to be used
byte encoderBaseOffset = 10;

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

void setup() {
  Serial.begin(31250); // MIDI baud rate 31250

  for( int i = 0; i < NButtons; i++ ) {
    pinMode(buttonPin[i], INPUT_PULLUP);
  }
}

void loop() {

  // Buttons
  for( int i = 0; i < NButtons; i++ ) {
    buttonCState[i] = digitalRead(buttonPin[i]);  // read pins from arduino

    if( (millis() - lastDebounceTime[i]) > debounceDelay ) {
      if( buttonPState[i] != buttonCState[i] ) {
        lastDebounceTime[i] = millis();

        if( buttonCState[i] == LOW ) {
          noteOn(midiCh, note + i, 127);  // channel, note, velocity

          // custom.  If speed mode button or stop button, reset speed to 0
          if( i == 0 || i == 3 ) {
              encoderVal[0] = 0;
          }

        } else {
          noteOff(midiCh, note + i, 0);  // channel, note, velocity
        }

        buttonPState[i] = buttonCState[i];
      }
    }
  }

  // Encoders
  for( int i = 0; i < NEncoders; i++ ) {
    encoderCState[i] = digitalRead(encoderPinA[i]);
    if( encoderCState[i] != encoderPState[i] ) {
      if( digitalRead(encoderPinB[i]) != encoderCState[i] )
        encoderVal[i]--;
      else
        encoderVal[i]++;

      if( encoderVal[i] > 100)
        encoderVal[i] = 100;
      else if( encoderVal[i] < 0 )
        encoderVal[i] = 0;

      controlChange(midiCh, encoderBaseOffset + cc + i, encoderVal[i]);

    encoderPState[i] = encoderCState[i];
    }
  }

  // Pots
  for( int i = 0; i < NPots; i++ ) {
    potCState[i] = analogRead(potPin[i]);
    midiCState[i] = map(potCState[i], 0, 1023, potMin[i], potMax[i]); // Maps the reading of the potCState to a value usable in midi
    potVar = abs(potCState[i] - potPState[i]); // Calculates the absolute value between the difference between the current and previous state of the pot

    if( potVar > varThreshold ) { // Opens the gate if the potentiometer variation is greater than the threshold
      PTime[i] = millis(); // Stores the previous time
    }

    timer[i] = millis() - PTime[i]; // Resets the timer 11000 - 11000 = 0ms

    if( timer[i] < TIMEOUT ) // If the timer is less than the maximum allowed time it means that the potentiometer is still moving
      potMoving = true;
    else
      potMoving = false;

    if( potMoving == true ) { // If the potentiometer is still moving, send the change control
      if( midiPState[i] != midiCState[i] ) {
        controlChange(midiCh, cc + i, midiCState[i]); //  (channel, CC number,  CC value)
        potPState[i] = potCState[i]; // Stores the current reading of the potentiometer to compare with the next
        midiPState[i] = midiCState[i];
      }
    }
  }

}
