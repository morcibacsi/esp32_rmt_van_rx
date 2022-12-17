#pragma once

#ifndef _esp32_arduino_rmt_van_rx_h
    #define _esp32_arduino_rmt_van_rx_h

    #include "Esp32RmtReader.h"

typedef enum {
    VAN_LINE_LEVEL_LOW   = 0,
    VAN_LINE_LEVEL_HIGH  = 1,
} VAN_LINE_LEVEL;

typedef enum {
    VAN_NETWORK_TYPE_BODY     = 0,
    VAN_NETWORK_TYPE_COMFORT  = 1,
} VAN_NETWORK_TYPE;

class ESP32_RMT_VAN_RX: public Esp32RmtReader
{
private:
    uint8_t _bitCounter = 0;
    uint8_t _tempByte = 0;
    uint8_t _mask = 1 << 7;
    uint8_t _timeSliceDivisor = 0;
    VAN_LINE_LEVEL _vanLineLevel = VAN_LINE_LEVEL_LOW;

    uint8_t RoundToNearest(uint8_t numToRound, uint8_t multiple);
    void BeforeProcessSignal() override;
    void AfterProcessSignal() override;
    void ProcessSignal(uint32_t level, uint32_t duration) override;

public:
    ESP32_RMT_VAN_RX(uint8_t channel, uint8_t rxPin, int8_t ledPin, VAN_LINE_LEVEL vanLineLevel, VAN_NETWORK_TYPE vanNetworkType) : Esp32RmtReader(80, 80, channel, rxPin, ledPin)
    {
        // Longer messages doesn't fit in one memory block of the RMT peripheral
        _memoryBlocks = 2;
        _vanLineLevel = vanLineLevel;
        if(vanNetworkType == VAN_NETWORK_TYPE_COMFORT)
        {
            _timeSliceDivisor = 8;
            _idleTreshold = 80; // we consider the packet whole after this number of ticks of unchanged line state
        }
        else if(vanNetworkType == VAN_NETWORK_TYPE_BODY)
        {
            _timeSliceDivisor = 16;
            _idleTreshold = 160; // we consider the packet whole after this number of ticks of unchanged line state
        }
    }

    ~ESP32_RMT_VAN_RX();
    
    // You can check the CRC of the received message with this function
    bool IsCrcOk(uint8_t vanMessage[], uint8_t vanMessageLength);

    // This function calculates the CRC of a message
    uint16_t Crc15(uint8_t data[], uint8_t lengthOfData);
};
#endif
