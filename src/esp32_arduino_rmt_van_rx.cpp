#include "esp32_arduino_rmt_van_rx.h"

#ifdef __cplusplus
extern "C" {
#endif
    /* Include C code here */
#ifdef __cplusplus
}
#endif
ESP32_RMT_VAN_RX::ESP32_RMT_VAN_RX()
{
}

ESP32_RMT_VAN_RX::~ESP32_RMT_VAN_RX()
{
    rmt_van_rx_channel_stop(_channel);
}

void ESP32_RMT_VAN_RX::Receive(uint8_t *messageLength, uint8_t message[])
{
    rmt_van_rx_receive(messageLength, message);
}

void ESP32_RMT_VAN_RX::Init(uint8_t channel, uint8_t rxPin, uint8_t ledPin, VAN_LINE_LEVEL vanLineLevel, VAN_NETWORK_TYPE vanNetworkType)
{
    _rxPin = rxPin;
    _ledPin = ledPin;
    _channel = channel;
    rmt_van_rx_channel_init(channel, rxPin, ledPin, static_cast<RX_VAN_LINE_LEVEL>(vanLineLevel), static_cast<RX_VAN_NETWORK_TYPE>(vanNetworkType));
}

void ESP32_RMT_VAN_RX::Stop(uint8_t channel)
{
    rmt_van_rx_channel_stop(channel);
}

bool ESP32_RMT_VAN_RX::IsCrcOk(uint8_t vanMessage[], uint8_t vanMessageLength)
{
    return rmt_van_rx_is_crc_ok(vanMessage, vanMessageLength);
}