/*
* MusicBox.cpp
*
* Music Box allows to play 8 mp3 sounds, splitted into 2 banks: words and music. Sounds are played by pushing 4 buttons.
* There is an additional button to change the sound bank. If after 5 minutes (configurable) no button was pushed, then 
* ESP32 enters in deep sleep mode to save energy from battery. Controler is woken up if any of the sound buttons is pushed.
*
* Words files must follow /wordX.mp3 name format, where X is 1, 2, 3 or 4. Music files must 
* follow /musicX.mp3 name format, where X is 1, 2, 3 or 4. Both type of files must 
* be stored in the root file of the external SD card.
* 
* The configuration of the Music Box is stored in the Configuration.h file
*
* Copyright 2020 3K MEDIALAB
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include <Arduino.h>
#include "SPIFFS.h"
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceSD.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "Button.h"
#include "Configuration.h"
#include <iostream>
#include <vector>
#include "stdlib.h"

enum Modes
{
  WORDS,
  MUSIC
}; // Music box modes

AudioGeneratorMP3 *mp3 = NULL; // MP3 files player
AudioFileSourceSD *sd = NULL; // Source files from external SD
AudioFileSourceSPIFFS *spiffs = NULL; // Source files from internal SPIFFS system
AudioOutputI2S *out = NULL; // Audio output using I2S and external DAC

TimerHandle_t sleepTimer;

uint8_t mode;

// Sound buttons
std::vector<Button> soundButtons = {{SOUND1_BUTTON_PIN, INTERNAL_PULL_UP, INVERT, DEBOUNCE_MS}, {SOUND2_BUTTON_PIN, INTERNAL_PULL_UP, INVERT, DEBOUNCE_MS}, {SOUND3_BUTTON_PIN, INTERNAL_PULL_UP, INVERT, DEBOUNCE_MS}, {SOUND4_BUTTON_PIN, INTERNAL_PULL_UP, INVERT, DEBOUNCE_MS}};

// Mode button
Button modeButton = Button(MODE_BUTTON_PIN, true, true, DEBOUNCE_MS);

/**
 * Stop current played mp3 file.
 * mp3: mp3 player.
 * sdFile: file being played.
 * out: audio output.
 */ 
void stopCurrentSound(AudioGeneratorMP3 *mp3, AudioFileSourceSD *sdFile, AudioOutputI2S *out)
{ 
  mp3->stop();
  out->stop();  

  delete sdFile;   
  sdFile = NULL;  
}

/**
 * Check if any sound button is pushed. If any, then stops current mp3 file being played, and prepare the new mp3 file.
 * mp3: mp3 player.
 * sdFile: file being played.
 * out: audio output.
 */
void processSoundButtons(AudioGeneratorMP3 *mp3, AudioFileSourceSD *sdFile, AudioOutputI2S *out)
{
  char fileName[15];

  for (std::vector<Button>::size_type i = 0; i != soundButtons.size(); i++)
  {
    soundButtons[i].read();
    if (soundButtons[i].wasReleased())
    {
      // Stop current played mp3 file if any
      if (mp3->isRunning())
      {
        stopCurrentSound(mp3, sdFile, out);
      }

      // Generate file name dependending on current mode
      switch (mode)
      {
      case WORDS:
        strcpy(fileName, WORD_FILE_PREFIX);
        break;
      case MUSIC:
        strcpy(fileName, MUSIC_FILE_PREFIX);
        break;
      }

      itoa(i + 1, fileName + strlen(fileName), 10);
      strcat(fileName + strlen(fileName), FILE_EXTENSION);
    
      // Load sound
      sdFile = new AudioFileSourceSD();   
      sdFile->open(fileName);     

      mp3->begin(sdFile, out);     
    }
  }
}

/**
 * Change Music Box mode and stop current being played mp3 file if any.
 * mp3: mp3 player.
 * sdFile: file being played.
 * out: audio output.
 */
void processModeButton(AudioGeneratorMP3 *mp3, AudioFileSourceSD *sdFile, AudioOutputI2S *out)
{
  modeButton.read();
  if (modeButton.wasReleased())
  {
    switch (mode)
    {
    case WORDS:
      mode = MUSIC;
      out->SetGain(OUTPUT_GAIN_WORDS);
      break;
    case MUSIC:    
      mode = WORDS;
      out->SetGain(OUTPUT_GAIN_MUSIC);
    }

    if (mp3->isRunning())
    {
      stopCurrentSound(mp3, sdFile, out);
    }
  }
}

/**
 * Callback function called when timer expires. Set ESP32 to deep sleep mode.
 * xTimer: timer handle.
 */
void vTimerCallback(TimerHandle_t xTimer)
{
  Serial.flush();
  esp_deep_sleep_start();
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  for (std::vector<Button>::size_type i = 0; i != soundButtons.size(); i++)
  {
    soundButtons[i].read();
  }

  modeButton.read();

  // Initial status
  mode = WORDS;

  if (!SPIFFS.begin())
  {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  audioLogger = &Serial;  

  // FreeRTOS task to create software timer
  sleepTimer = xTimerCreate(/* Just a text name, not used by the RTOS
                     kernel. */
                            "SleepTimer",
                            /* The timer period in ticks, must be
                     greater than 0. */
                            pdMS_TO_TICKS(AWAKE_TIME_SECS * mS_TO_S_FACTOR),
                            /* The timers will auto-reload themselves
                     when they expire. */
                            pdFALSE,
                            /* The ID is used to store a count of the
                     number of times the timer has expired, which
                     is initialised to 0. */
                            (void *)0,
                            /* Each timer calls the same callback when
                     it expires. */
                            vTimerCallback);

  if (sleepTimer == NULL)
  {
    /* The timer was not created. */
    Serial.println("Deep sleep timer couldn't be created");
    return;
  }
  else
  {
    /* Start the timer.  No block time is specified, and
    even if one was it would be ignored because the RTOS
    scheduler has not yet been started. */
    if (xTimerStart(sleepTimer, 0) != pdPASS)
    {
      Serial.println("Deep sleep timer couldn't be started");
      return;
    }
  }

  // Enable ext1 as wakeup source
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);

  // Init SD card
  if (!SD.begin(SD_CS_PIN))
  {
    Serial.println("Card Mount Failed");
    return;
  }

  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)
  {
    Serial.println("No SD card attached");
    return;
  }

  // Init sound is loaded from SPIFFS
  spiffs = new AudioFileSourceSPIFFS();
  
  // Output
  out = new AudioOutputI2S();  

  // Audio generator
  mp3 = new AudioGeneratorMP3();

  // Play init sound
  out->SetGain(OUTPUT_GAIN_MUSIC);
  spiffs->open(INIT_SOUND);
  mp3->begin(spiffs, out);

  if (mp3->isRunning())
  {
    while (mp3->loop())
    {
    }
  }

  mp3->stop();
  out->stop();
  
  delete spiffs; 
  spiffs = NULL;

  out->SetGain(OUTPUT_GAIN_WORDS);
}

void loop()
{
  // Play mp3 file
  if (mp3->isRunning())
  {
    if (!mp3->loop())
    {
      stopCurrentSound(mp3, sd, out);
    }
    else
    {
      if (xTimerReset(sleepTimer, 100) != pdTRUE)
      {
        Serial.println("Deep sleep timer couldn't be reset");
      }
    }
  }

  processSoundButtons(mp3, sd, out);
  processModeButton(mp3, sd, out);
}