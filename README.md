# Music Box

ESP32 based mp3 sound files player. The aim of this project is to help people with speaking difficulties by playing previously stored words into an external Micro SD card. Such Words are played by pushing buttons. Sound is redirected to an external DAC, then to an amplifier and last to speakers.

Music Box works in two ways:

* **Words:** plays predefined words when buttons are pushed.
* **Music:** plays predefined songs when buttons are pushed.

Modes are switched by pushing mode button.

The project is powered using a 18650 ion lithium battery, which can be charged by a TP4056 charger. To save energy from the battery, the ESP32 is configured to enter in deep sleep mode if no sound has been played after a time interval. ESP32 is woken up when any of the buttons which trigger a sound is pushed. Time interval to enter into deep sleep mode is defined in **Configuration.h**

```c++
const uint16_t AWAKE_TIME_SECS = 300; // 5 minutes
```

Mp3 files are played using [earlephilhower ESP8266 Audio Library](https://www.addicore.com/TP4056-Charger-and-Protection-Module-p/ad310.htm) .

## Getting Started

These instructions will get you a copy of the project up and running. Project was developed using **PlatformIO** and **Visual Studio Code** ([check installation guide](https://platformio.org/install/ide?install=vscode)).

Project configuration can be found in file **Configuration.h**

### Prerequisites

When ESP32 is booted up, a welcome sound is played automatically. This mp3 file is stored in internal SPIFFS. This file can be found in the data folder ("init.mp3"). To load this file into SPIFFS, use platformIO's task **Upload File System Image**

In addition, mp3 files (words and songs) must be present in the MicroSD card before ESP32 is booted up. Such files must be stored in the root file of the Micro SD card and follow this name format:

* **Words:** [WORD_FILE_PREFIX][BUTTON_NUMBER].[FILE_EXTENSION]
* **Music:** [MUSIC_FILE_PREFIX][BUTTON_NUMBER].[FILE_EXTENSION]

Prefixes and file extension can be configured in file **Configuration.h**

The code is pretty well documented.

```c++
// File name format
const char * WORD_FILE_PREFIX = "/word";
const char * MUSIC_FILE_PREFIX = "/music";
const char * FILE_EXTENSION = ".mp3";
```

E.g: "/word1.mp3" or "/music4.mp3"

## Bill of materials

Amount | Part Type
------------ | -------------
 1 | ESP32 DEVKIT v1
 1 | SPI Micro SD Card Module
 1 | Micro SD Card
 1 | 18650 Ion-Lithium battery
 1 | TP4056 charger module
 1 | Low-dropout voltage regulator MCP1700-3302E
 1 | Electrolytic Capacitor; capacitance 100μF
 1 | Ceramic capacitor; capacitance 100nF
 1 | I2S DAC ( Adafruit I2S +3W amp DAC or generic PCM5102 based DAC + Stereo Amplifier Board)
 5 | Momentary push buttons
 4 | 10kΩ Resistor
 2 | Audio speakers

## Circuit diagram

![Circuit Diagram](/resources/CircuitDiagram.jpg)

### Power

I have used one [Panasonic NCR18650A 3100mAh - 6.2A battery](https://www.nkon.nl/es/panasonic-ncr18650a.html) to power the circuit.

The output voltage of the battery when fully charged is about 4.2v, hence we need to regulate the voltage to 3.3v to power the ESP32 via the Vin pin. This is by achivied using a Low-dropout voltage regulator MCP1700-3302E. LDOs should have a ceramic capacitor and an electrolytic capacitor connected in parallel to GND and Vout to smooth the voltage peaks. Here we’re using a 100uF electrolytic capacitor, and a 100nF ceramic capacitor.

The battery is charged using a [TP4056](https://www.addicore.com/TP4056-Charger-and-Protection-Module-p/ad310.htm) module.

Please check [this excellent tutorial](https://randomnerdtutorials.com/power-esp32-esp8266-solar-panels-battery-level-monitoring/) for further information.

### External I2S DAC and amplifier

As mentioned in [earlephilhower ESP8266 Audio Library](https://github.com/earlephilhower/ESP8266Audio), both the Adafruit [I2S +3W amp DAC](https://www.adafruit.com/product/3006) and a generic PCM5102 based DAC work with success.

#### Adafruit I2S DAC (showed in circuit diagram)

This is quite simple and only needs the GND, VIN, LRC, BCLK< and DIN pins to be wired. See the [Adafruit example page](https://learn.adafruit.com/adafruit-max98357-i2s-class-d-mono-amp) for more info.

#### PCM5102 DAC + amplifier

I've used this version of [PCM5102 DAC board](https://es.aliexpress.com/item/4000049720221.html?spm=a2g0o.productlist.0.0.57561484X5uWZL&algo_pvid=95811dec-fc4c-4f61-b2d6-5426af2ae936&algo_expid=95811dec-fc4c-4f61-b2d6-5426af2ae936-0&btsid=0b0a22a415966238166423401e6702&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_).

![I2S PCM5102 DAC](/resources/I2S_PCM5102_DAC.jpg)

There are several input configuration pins beyond the I2S interface itself that need to be wired:

* 3.3V from ESP32 -> VCC, 33V, XMT
* GND from ESP32 -> GND, FLT, DMP, FMT, SCL
* (Standard I2S interface) BCLK->BCK (GPIO 26), I2SO->DIN (GPIO 22), and LRCLK(WS)->LCK (GPIO 25).

Output from the DAC is then routed to the input of this [stereo amplifier board](https://es.aliexpress.com/item/32792986879.html?spm=a2g0s.9042311.0.0.274263c0m19beD) using a male/male minijack stereo audio wire.

![Stereo amplifier board](/resources/stereo_amplifier.jpg)

Output from the amp is then routed to this [speakers](https://es.aliexpress.com/item/33001758564.html?spm=a2g0s.9042311.0.0.274263c08rWGcV).

![Stereo speakers](/resources/stereo_speakers.jpg)

## Built With

* [ESP8266/ESP32 Audio Library](https://github.com/earlephilhower/ESP8266Audio) - The audio library used to play mp3 files from SPIFFS and Micro SD.
* [Forked Jack Christensen Arduino Button Library](https://github.com/JChristensen/JC_Button) - The library for managing push buttons.
* [PlatformIO](https://platformio.org/) - Professional collaborative platform for embedded development.
* [Visual Studio Code](https://code.visualstudio.com/) - C++ Development IDE.

## Authors

* **3K Medialab** - *Initial work* - [Instagram](https://www.instagram.com/3kmedialab)

## License

This project is licensed under the Apache License Version 2.0 - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* [Nerim](https://www.nerim.es/) team for the inspiration.
* All the 3K Medialab team members (Cesar Segura, Fernando Pérez and Rubén) for helping in the development of the project.
