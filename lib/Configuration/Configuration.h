#ifndef Configuration_h
#define Configuration_h

// Output gains
const float OUTPUT_GAIN_WORDS = 0.4;
const float OUTPUT_GAIN_MUSIC = 0.3;

// Conversion factor for milli seconds to seconds
const uint16_t mS_TO_S_FACTOR = 1000;

// Time in seconds to wait before enter in deep sleep mode
const uint16_t AWAKE_TIME_SECS = 300; // 5 minutes

// SD data pin
const uint8_t SD_CS_PIN = 5;

// File name format
const char * WORD_FILE_PREFIX = "/word";
const char * MUSIC_FILE_PREFIX = "/music";
const char * FILE_EXTENSION = ".mp3";

//-------------------------------- B U T T O N S  S E C T I O N ---------------------------------------------

/*************************************************
 * BUTTON CONFIGURATION
 *************************************************/

 //TRUE: use the Arduino's internal pullup resistor.
 const uint8_t INTERNAL_PULL_UP = false;

 //Since the pullup resistor will keep the pin high unless the switch is closed, this is negative logic, i.e. a high state
 //means the button is NOT pressed. (Assuming a normally open switch.)
 // Use TRUE when using Arduino' pullup resistor
 const uint8_t INVERT = false;

 //Debounce time for the buttons. A time of 20 milliseconds usually works well for tactile button switches.
 const uint8_t DEBOUNCE_MS = 20;

 /*************************************************
  * BUTTONS CONFIGURATION
  *************************************************/

  // Play sound buttons input pins
  const uint8_t SOUND1_BUTTON_PIN = 14;
  const uint8_t SOUND2_BUTTON_PIN = 15;
  const uint8_t SOUND3_BUTTON_PIN = 27;
  const uint8_t SOUND4_BUTTON_PIN = 13;

  // GPIO pins resources for external deep sleep wake up. Mask is related with aboove input pins
  // Check https://randomnerdtutorials.com/esp32-external-wake-up-deep-sleep/ for BITMASK generation
  const uint32_t BUTTON_PIN_BITMASK = 0x800E000;

  // Mode button input pin
  const uint8_t MODE_BUTTON_PIN = 12;

 //-------------------------------- E N D  B U T T O N S  S E C T I O N ---------------------------------------------

//-------------------------------- D E F A U L T  S O U N D S  S E C T I O N ---------------------------------------------------------

// Sound played during system setup
const char * INIT_SOUND = "/init.mp3";
//-------------------------------- E N D  O F  D E F A U L T  S O U N D S  S E C T I O N ---------------------------------------------

#endif
