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

void ESP32_RMT_VAN_RX::Init(uint8_t channel, uint8_t rxPin, uint8_t ledPin)
{
    _rxPin = rxPin;
    _ledPin = ledPin;
    _channel = channel;
    rmt_van_rx_channel_init(channel, rxPin, ledPin);
}

void ESP32_RMT_VAN_RX::Stop(uint8_t channel)
{
    rmt_van_rx_channel_stop(channel);
}