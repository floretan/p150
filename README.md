# Weighted MIDI keyboard

This is the arduino code for a weighted MIDI keyboard made from a used Yamaha P150 keybed and a Teensy 3.2.

## How it works

Under each key there are two switches, which are triggered one after the other. By measuring the interval between the two, we know how hard the key was hit. 

The Yamaha P150 has a keyboard matrix with 15 rows and 12 columns (6 for the first switches, 6 for the second switches). The 15 rows are connected to a shift register, where at any time only a single row outputs a LOW voltage (ground). The columns are connected to 12 dedicated pints on the teensy, which are set to use the internal pull-up resistor.

## Credit
Thanks to Albin Stigo for a [similar project](https://github.com/ast/keyboard) that inspired me to get started with this one. While his code reads the keys a thousand times a second, I read the keys as often as possible. It might be less ressource-efficient, but it seems to be more accurate. I also added the logarithmic velocity response, which gives a much more natural feel. 
