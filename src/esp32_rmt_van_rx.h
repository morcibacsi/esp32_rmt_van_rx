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

    void rmt_van_rx_receive(uint8_t *messageLength, uint8_t message[]) ;
    void rmt_van_rx_channel_init(uint8_t channel, uint8_t rxPin, uint8_t ledPin);
    void rmt_van_rx_channel_stop(uint8_t channel);

    #ifdef __cplusplus
    } // extern C
    #endif
#endif