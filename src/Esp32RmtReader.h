// Esp32RmtReader.h
#pragma once

#ifndef _Esp32RmtReader_h
    #define _Esp32RmtReader_h

#include <stdint.h>
#include "driver/rmt.h"
#include "driver/periph_ctrl.h"
#include "soc/rmt_reg.h"

/*
    This is a base class around the RMT peripheral of the ESP32. Can't be used directly, you need to create a derived class
    and implemement the virtual methods.
*/
class Esp32RmtReader
{
    #define RMT_READER_MAX_DATA 255
    protected:
        uint16_t _idleTreshold;
        uint8_t _clkDiv;
        uint8_t _channel;
        uint8_t _memoryBlocks;
        uint8_t _rxPin;
        int8_t _ledPin;
        uint8_t _messageLength;
        uint8_t _message[RMT_READER_MAX_DATA];

        Esp32RmtReader(uint8_t clkDiv, uint16_t idleTreshold, uint8_t channel, uint8_t rxPin, int8_t ledPin);
        void InitLed();
        void SwitchLed(uint8_t state);

        /* 
        It is called before the ProcessSignal is executed. You can initialize variables, prepare for incoming data etc... 
        The derived class should implement it.
        */
        virtual void BeforeProcessSignal() = 0;
        /* 
        It is called after the ProcessSignal is executed. The derived class should implement it.
        */
        virtual void AfterProcessSignal() = 0;
        // Used to convert the level and duration to real data. It is up to the derived class to implement the parsing logic.
        virtual void ProcessSignal(uint32_t level, uint32_t duration) = 0;
    public:
        // Start receiving RMT data
        int32_t Start();
        // Stop receiving RMT data
        void Stop();
        // This should be called regularly from a loop to get the parsed data from the RMT peripheral.
        void ReceiveData(uint8_t *messageLength, uint8_t message[]);
};

#endif
