// Based on code from https://github.com/ast/keyboard

// Possible key states
typedef enum {
  KEY_IS_UP,
  KEY_IS_DOWN,
  KEY_IS_GOING_UP,    // We increment the timer in this state
  KEY_IS_GOING_DOWN,  // We increment the timer in this state
} state_t;

typedef struct {
  byte midi_note;
  state_t state; // Bit fields
  long t; // Microsecond timestamp
} Key;
