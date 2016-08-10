# Weighted MIDI keyboard

This is the arduino code for a weighted MIDI keyboard made from a used Yamaha P150 keybed and a Teensy 3.2.

## Credit
Thanks to Albin Stigo for a [similar project](https://github.com/ast/keyboard) that inspired me to get started with this one. While his code reads the keys a thousand times a second, I read the keys as often as possible. It might be less ressource-efficient, but it seems to be more accurate. I also added the logarithmic velocity response, which gives a much more natural feel. 
