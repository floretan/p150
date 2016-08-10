#include "types.h"

// Teensy pins for the shift registers.
const int clockPin = 19;
const int dataPin = 21;

// Propagation delay in microseconds.
// This is the shorted value I was able to use while still having good readings.
const int propDelay = 3;

// The interval between the first and second switch while hitting keys with the
// highest velocity.
const float shortestInterval = 2000.0;

// Every time the interval between the first and second switch doubles, lower
// the midi velocity by this much:
const int velocityAttenuation = 20;

// Keep track of state.
Key keys[88];
bool pedalState;

void setup() {

  // Initialize column pins.
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);

  // The sustain pedal gets a dedicated pin.
  pinMode(12, INPUT_PULLUP);

  // Initialize the pins for the shift register that manages rows.
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  Serial.begin(9600);
  delay(500);

  // Init keys
  for ( int key = 0; key < 88; key++) {
    keys[key].midi_note = 21 + key;
    keys[key].t = 0;
    keys[key].state = KEY_IS_UP;
  }
}

void loop() {

  // Send a "ground" through the shift registers.
  digitalWrite(clockPin, LOW);
  delayMicroseconds(propDelay);
  digitalWrite(dataPin, LOW);
  delayMicroseconds(propDelay);
  digitalWrite(clockPin, HIGH);
  delayMicroseconds(propDelay);

  // Afterwards, don't send any ground.
  digitalWrite(dataPin, HIGH);

  for (int row = 0; row < 15; row++) {
    for (int col = 0; col < 6; col++) {
      // (14 - row) because I wired things backwards.
      // *6 because there are 6 columns in each matrix row
      // -2 because the first row is incomplete
      //     (otherwise there would be 90 keys, not 88)
      int key_index = (14 - row) * 6 + col - 2;

      bool switchA = !digitalRead(col);
      bool switchB = !digitalRead(col + 6);

      if (switchA) {

        if (switchB && keys[key_index].state == KEY_IS_GOING_DOWN) {
          keys[key_index].state = KEY_IS_DOWN;

          unsigned long t = micros() - keys[key_index].t;

          int velocity = 127 - log(t/shortestInterval)/log(2) * velocityAttenuation;

          if (velocity < 0) {
            velocity = 0;
          }

          usbMIDI.sendNoteOn(keys[key_index].midi_note, velocity, 0);
        }

        if (!switchB) {
          if (keys[key_index].state == KEY_IS_UP) {
            keys[key_index].state = KEY_IS_GOING_DOWN;
            keys[key_index].t = micros();
          }
          else if (keys[key_index].state == KEY_IS_DOWN) {
            keys[key_index].state = KEY_IS_GOING_UP;
          }
        }
      }
      else if (keys[key_index].state != KEY_IS_UP) {
        keys[key_index].state = KEY_IS_UP;
        usbMIDI.sendNoteOff(keys[key_index].midi_note, 127, 0);
      }
    }

    // Move values in the shift register.
    digitalWrite(clockPin, LOW);
    delayMicroseconds(propDelay);
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(propDelay);
  }

  bool pedalPressed = !digitalRead(12);
  if (pedalPressed != pedalState) {
    usbMIDI.sendControlChange(64, (pedalPressed ? 127 : 0), 0);
    pedalState = pedalPressed;
  }

}
