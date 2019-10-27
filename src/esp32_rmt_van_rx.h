/*
    ESP32 library to monitor a 125kbps VAN bus
*/

#pragma once

#ifndef _esp32_rmt_van_rx_h
    #define _esp32_rmt_van_rx_h

    #include "driver/rmt.h"
    #include "driver/periph_ctrl.h"
    #include "soc/rmt_reg.h"

    #ifdef __cplusplus
    extern "C" {
    #endif

    typedef enum {
        RX_VAN_LINE_LEVEL_LOW   = 0,
        RX_VAN_LINE_LEVEL_HIGH  = 1,
    } RX_VAN_LINE_LEVEL;

    typedef enum {
        RX_VAN_NETWORK_BODY    = 0,
        RX_VAN_NETWORK_COMFORT = 1,
    } RX_VAN_NETWORK_TYPE;

    void rmt_van_rx_receive(uint8_t *messageLength, uint8_t message[]) ;
    void rmt_van_rx_channel_init(uint8_t channel, uint8_t rxPin, uint8_t ledPin, RX_VAN_LINE_LEVEL vanLineLevel, RX_VAN_NETWORK_TYPE vanNetworkType);
    void rmt_van_rx_channel_stop(uint8_t channel);
    bool rmt_van_rx_is_crc_ok(uint8_t vanMessage[], uint8_t vanMessageLength);

    #ifdef __cplusplus
    } // extern C
    #endif
#endif