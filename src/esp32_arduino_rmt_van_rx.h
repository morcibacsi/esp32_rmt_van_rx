/*
    Arduino wrapper around the ESP32 library to monitor a 125kbps VAN bus
*/
#pragma once

#ifndef _esp32_arduino_rmt_van_rx_h
    #define _esp32_arduino_rmt_van_rx_h

    #include "esp32_rmt_van_rx.h"


    #ifdef __cplusplus
    extern "C" {
    #endif
        /* Include C code here */
        typedef enum {
            VAN_LINE_LEVEL_LOW   = 0,
            VAN_LINE_LEVEL_HIGH  = 1,
        } VAN_LINE_LEVEL;
    #ifdef __cplusplus
    }
    #endif

class ESP32_RMT_VAN_RX
{
private:
    uint8_t _rxPin;
    uint8_t _ledPin;
    uint8_t _channel;

public:
    ESP32_RMT_VAN_RX();
    ~ESP32_RMT_VAN_RX();

    void Receive(uint8_t *messageLength, uint8_t message[]);
    void Init(uint8_t channel, uint8_t rxPin, uint8_t ledPin, VAN_LINE_LEVEL vanLineLevel);
    void Stop(uint8_t channel);
    bool IsCrcOk(uint8_t vanMessage[], uint8_t vanMessageLength);
};
#endif
