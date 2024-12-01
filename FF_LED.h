/*!
	\file
	\brief	Implement few LED effects (fixed, blinking, pulsing) with brightness management
	\author	Flying Domotic
	\date	December 1st, 2024

	Have a look at FF_LED.cpp for details

*/


#ifndef FF_LED_h
    #define FF_LED_h
    #include "Arduino.h"

    #ifdef __cplusplus
        #define FF_LED_WAIT_FOR_EVER (4294967295)               //!< Everyyyyy long time (arond 50 days)
        class FF_LED {
            /*!	\class FF_LED
                \brief Implement few LED effects (fixed, blinking, pulsing) with brightness management
            */
            public:
                FF_LED(uint8_t _ledPin, bool _isReverted = false, uint8_t _initialLevel = 0);
                ~FF_LED();
                enum ledModedType {fixed, blink, pulse};        //!< LED mode definition
                void begin(void);
                void loop(void);
                void setFixed(uint8_t _level);
                void setBlink(uint8_t _blinkCount, unsigned long _onTime, unsigned long _offTime, unsigned long _waitTime, uint8_t _minLevel = 0, uint8_t _maxLevel = 255);
                void setPulse(bool _increase, unsigned long _upTime, unsigned long _downTime, unsigned long _waitTime, uint8_t _minLevel = 0, uint8_t _maxLevel = 255);
            private:
                void setLed(uint8_t _level, unsigned long _delay);

                uint8_t ledPin = 0;                             //!< Pin where LEd is connected to
                bool ledReverted = false;                       //!< Is LED reverted (turned on when pin level is low)?
                uint8_t ledBlinksNeeded = 0;                    //!< Count of LED blinks needed
                uint8_t ledBlinksDone = 0;                      //!< Count of LED blinks already done
                uint8_t ledMinLevel = 0;                        //!< Minimum level for pulse
                uint8_t ledMaxLevel = 255;                      //!< Maximum level for pulse
                uint8_t ledLevel = 0;                           //!< Current LED level
                bool ledIncrease = false;                       //!< Requested pulse increase
                int8_t ledPulseIncrement = 0;                   //!< Current (signed) pulse increment
                ledModedType ledMode = fixed;                   //!< LED mode
                unsigned long ledOnDelay = 0;                   //!< Delay to turn led on (or increasing brightness)
                unsigned long ledOffDelay = 0;                  //!< Delay to turn led off (or decreasing brightness)
                unsigned long ledWaitDelay = 0;                 //!< Delay to wait before next cycle
                unsigned long ledDelay = 0;                     //!< Delay before next change
                unsigned long ledLastTimeChanged = 0;           //!< Last time led state changed
        };
    #endif
#endif