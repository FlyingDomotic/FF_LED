/*!
	\file
	\brief	Implement few LED effects (fixed, blinking, pulsing) with brightness management
	\author	Flying Domotic
	\date	December 1st, 2024
*/

#include "FF_LED.h"

/*!

	\brief	Class constructor

	Initialize the class

	\param[in]	_ledPin: pin number where LED is connected to 
	\param[in]	_isReverted: LED reverted (turned on when pin level is low)? (default = false)
	\param[in]	_initialLevel: LED level (0-255) at startup (default = 0)
	\return	none

*/
FF_LED::FF_LED(uint8_t _ledPin, bool _isReverted, uint8_t _initialLevel) {
    ledPin = _ledPin;
    ledReverted = _isReverted;
    ledLevel = _initialLevel;
    ledMode = fixed;
}

FF_LED::~FF_LED(){
    pinMode(ledPin, INPUT);                     // Set pin mode to input (release it)
}

/*!

	\brief	Set LED level

	Set LED level and time to stay at this level

	\param[in]	_level: LED level (0-255) to set
	\param[in]	_delay: time to stay at this level (in ms)
	\return	none

*/
void FF_LED::setLed(uint8_t _level, unsigned long _delay) {
    //Serial.printf(PSTR("setLed: level:%d, delay: %u, blinks: %d/%d\n"), _level, _delay, ledBlinksDone, ledBlinksNeeded);
    ledLevel = _level;                          // Save current LED level
    ledDelay = _delay;                          // Save delay before next change
    ledLastTimeChanged = millis();              // Save change time
    if (ledLevel == 0) {
        digitalWrite(ledPin, ledReverted ? ledMaxLevel : ledMinLevel);
    } else if (ledLevel == 255) {
        digitalWrite(ledPin, ledReverted ? ledMinLevel : ledMaxLevel);
    } else {
        analogWrite(ledPin, ledReverted ? 255 - ledLevel : ledLevel);
    }
}

/*!

	\brief	Set LED blink count

	Start LED blinking sequence

	\param[in]	_blinkCount: number of  time to blink LED
	\param[in]	_onTime: time to light the LED (in ms)
	\param[in]	_offTime: time to keep LED off (in ms)
	\param[in]	_waitTime: time to wait after the blink sequence of _blinkCount
	\param[in]	_minLevel: minimum (OFF) LED level (0-255, default to 0)
	\param[in]	_maxLevel: maximum (ON) LED level (0-255, default to 255)
	\return	none

*/
void FF_LED::setBlink(uint8_t _blinkCount, unsigned long _onTime, unsigned long _offTime, unsigned long _waitTime, uint8_t _minLevel, uint8_t _maxLevel) {
    Serial.printf(PSTR("setBlink count:%d, on:%u, off:%u, wait:%u, min:%d, max: %d\n"), _blinkCount, _onTime, _offTime, _waitTime, _minLevel, _maxLevel);
    ledBlinksNeeded = _blinkCount;              // Set needed blinks count
    ledOnDelay = _onTime;
    ledOffDelay = _offTime;
    ledWaitDelay = _waitTime;
    ledMinLevel = _minLevel;
    ledMaxLevel = _maxLevel;
    ledBlinksDone = 0;                          // Clear done count
    ledMode = blink;                            // Set mode
    if (ledBlinksNeeded) {                      // Any blink?
        setLed(ledMaxLevel, ledOnDelay);        // Light LED
    } else {
        setLed(ledMinLevel, ledWaitDelay);      // Switch LED off
    }
}

/*!

	\brief	Set LED to fixed level

	Set LED permanent level

	\param[in]	_level: LED level to set (0-255)
	\return	none

*/
void FF_LED::setFixed(uint8_t _level) {
    Serial.printf(PSTR("setFixed level:%d\n"), _level);
    ledLevel = _level;
    ledMode = fixed;
    setLed(ledLevel, FF_LED_WAIT_FOR_EVER);
}

/*!

	\brief	Set LED continous pulse

	Start LED blinking sequence

	\param[in]	_increase: set to true to start from _minLevel, false to start from _maxLevel
	\param[in]	_upTime: time to wait between 2 increases (in ms)
	\param[in]	_downTime: time to wait between 2 decreases (in ms)
	\param[in]	_waitTime: time to wait after one pulse sequence
	\param[in]	_minLevel: minimum LED level (0-255, default to 0)
	\param[in]	_maxLevel: maximum LED level (0-255, default to 255)
	\return	none

*/
void FF_LED::setPulse(bool _increase, unsigned long _upTime, unsigned long _downTime, unsigned long _waitTime, uint8_t _minLevel, uint8_t _maxLevel) {
    Serial.printf(PSTR("setPulse increase:%d, up:%u, down:%u, wait:%u, min:%d, max: %d\n"), _increase, _upTime, _downTime, _waitTime, _minLevel, _maxLevel);
    ledIncrease = _increase;                               // Save given parameters
    ledMinLevel = _minLevel;
    ledMaxLevel = _maxLevel;
    ledOnDelay = _upTime;
    ledOffDelay = _downTime;
    ledWaitDelay = _waitTime;
    ledMode = pulse;
    if (ledIncrease) {
        ledPulseIncrement = 1;
        ledLevel = ledMinLevel;    // Set initial level
        ledDelay = ledOnDelay;     // Set first delay
    } else {
        ledPulseIncrement = -1;
        ledLevel = ledMaxLevel;    // Set initial level
        ledDelay = ledOffDelay;    // Set first delay
    }
    setLed(ledLevel, ledDelay);                             // Start pulsing
}

/*!

	\brief	Start class

	Start class. Should be called in setup().

	\return	none

*/
void FF_LED::begin(void) {
    setLed(ledLevel, FF_LED_WAIT_FOR_EVER);     // Turn LED to initial state
    pinMode(ledPin, OUTPUT);                    // Set pin mode to output
}

/*!

	\brief	Loop

	Loop part of class. Should be called in loop().

	\return	none

*/
void FF_LED::loop(void) {
    // Do we exceed wait delay for this level ?
    if ((millis() - ledLastTimeChanged) > ledDelay) {
        if (ledMode == blink) {                     // Mode is blink
            if (ledLevel == ledMaxLevel) {          // LED is on
                ledBlinksDone++;                    // Increment blink count
                setLed(ledMinLevel, ledOffDelay);   // Set LED off
            } else {                                // Do we done all blinks?
                if (ledBlinksDone >= ledBlinksNeeded) {
                    ledBlinksDone = 0;              // Clear blink count
                    setLed(ledMinLevel, ledWaitDelay); // Set LED off, wait for interval between 2 blinks sequences
                } else {
                    setLed(ledMaxLevel, ledOnDelay);// Set LED on
                }
            }
        } else if (ledMode == pulse) {
            //Serial.printf(PSTR("level:%d, increment: %d\n"), ledLevel, ledPulseIncrement);
            int16_t ledNewLevel = ledLevel + ledPulseIncrement;
            if (ledPulseIncrement > 0) {                        // Are we increasing level?
                if (ledNewLevel > ledMaxLevel) {
                    ledPulseIncrement = -1;                     // Revert way
                    if (ledIncrease) {                          // Should we increase?
                        setLed(ledMaxLevel, ledOffDelay);       // Decrease level
                    } else {
                        setLed(ledMaxLevel, ledWaitDelay);      // Wait for interval between 2 pulse sequences
                    }
                } else {
                    setLed((uint8_t) ledNewLevel, ledOnDelay);
                }
            } else {                                            // We are decreasing level
                if (ledNewLevel < ledMinLevel) {
                    ledPulseIncrement = 1;                      // Revert way
                    if (!ledIncrease) {                         // Should we decrease?
                        setLed(ledMinLevel, ledOnDelay);        // Increase level
                    } else {
                        setLed(ledMinLevel, ledWaitDelay);      // Wait for interval between 2 pulse sequences
                    }
                } else {
                    setLed((uint8_t) ledNewLevel, ledOffDelay);
                }
            }
        }
    }
}
