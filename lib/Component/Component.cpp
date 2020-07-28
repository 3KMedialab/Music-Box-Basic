/*
 * Component.cpp
 *
 * Class that represens an Input/Output component connected to Arduino
 *
 * Copyright 2017 3K MEDIALAB
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

#include "Component.h"

/*
* Constructor for Component Class.
* pin: Arduino's pin which the component is connected.
* type: connection mode of the component.
*/
Component::Component(uint8_t pin, uint8_t type)
{
  _pins[0] = pin;
  _pin = _pins[0];

  switch (type)
  {
    case ComponentType::INPUT_DIGITAL :
      pinMode(_pins[0], INPUT);
    break;

    case ComponentType::OUTPUT_DIGITAL :
      pinMode(_pins[0], OUTPUT);
    break;

    case ComponentType::INPUT_ANALOG :
    break;
  }
}

/*
* Constructor for Component Class.
* pins: list of Arduino's pins which the component is connected.
* number: number of pins to be set.
* type: connection modes of the component.
*/
Component::Component(uint8_t * pins, uint8_t number, uint8_t type)
{
  for (int i=0; i<number; i++)
  {
    if (i == MAX_PINS_NUMBER)
      break;
    else
    {
      _pins[i] = pins[i];

      switch (type)
      {
        case ComponentType::INPUT_DIGITAL :
          pinMode(_pins[i], INPUT);
        break;

        case ComponentType::OUTPUT_DIGITAL :
          pinMode(_pins[i], OUTPUT);
        break;

        case ComponentType::INPUT_ANALOG :
        break;
      }
    }
  }
}

/*
* Constructor for Component Class.
* pin: Arduino's pin which the component is connected.
* type: connection mode of the component.
* puEnable: flag to set the INPUT or INPUT PULLUP pin mode
*/
Component::Component(uint8_t pin, uint8_t type, uint8_t puEnable)
{
  _pins[0] = pin;
  _pin = _pins[0];

  switch (type)
  {
    case ComponentType::INPUT_DIGITAL :
      if (puEnable)
      {
        pinMode(_pins[0], INPUT_PULLUP);
      }
      else
      {
        pinMode(_pins[0], INPUT);
      }
    break;

    case ComponentType::OUTPUT_DIGITAL :
      pinMode(_pins[0], OUTPUT);
    break;

    case ComponentType::INPUT_ANALOG :
    break;
  }
}

/*
* Constructor for Component Class.
* pins: list of Arduino's pins which the component is connected.
* number: number of pins to be set.
* type: connection mode of the component.
* puEnable: flag to set the INPUT or INPUT PULLUP pin mode
*/
Component::Component(uint8_t * pins, uint8_t number, uint8_t type, uint8_t puEnable)
{
  for (int i=0; i<number; i++)
  {
    if (i == MAX_PINS_NUMBER)
      break;
    else
    {
      _pins[i] = pins[i];

      switch (type)
      {
        case ComponentType::INPUT_DIGITAL :
          if (puEnable)
          {
            pinMode(_pins[0], INPUT_PULLUP);
          }
          else
          {
            pinMode(_pins[0], INPUT);
          }
        break;

        case ComponentType::OUTPUT_DIGITAL :
          pinMode(_pins[0], OUTPUT);
        break;

        case ComponentType::INPUT_ANALOG :
        break;
      }
    }
  }
}

/*
* Returns the pin of the component
*/
uint8_t Component::getPin()
{
  return _pin;
}

/*
* Returns the pins of the component
*/
uint8_t * Component::getPins()
{
  return _pins;
}
