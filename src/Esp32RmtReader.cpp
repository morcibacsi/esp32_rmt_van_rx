#include "Esp32RmtReader.h"
#include <string.h>

Esp32RmtReader::Esp32RmtReader(uint8_t clkDiv, uint16_t idleTreshold, uint8_t channel, uint8_t rxPin, int8_t ledPin)
{
    _clkDiv = clkDiv;
    _idleTreshold = idleTreshold;
    _channel = channel;
    _rxPin = rxPin;
    _ledPin = ledPin;
    _memoryBlocks = 1;
    _messageLength = 0;
}

void Esp32RmtReader::InitLed()
{
    if (_ledPin > -1)
    {
        gpio_pad_select_gpio(_ledPin);
        gpio_set_direction((gpio_num_t)_ledPin, GPIO_MODE_OUTPUT);
    }
}

void Esp32RmtReader::SwitchLed(uint8_t state)
{
    if (_ledPin > -1)
    {
        gpio_set_level((gpio_num_t)_ledPin, state);
    }
}

int32_t Esp32RmtReader::Start()
{
    InitLed();

    rmt_config_t rmt_rx;

    rmt_rx.rx_config.idle_threshold = _idleTreshold; // we consider the packet whole after this number of ticks of unchanged line state

    rmt_rx.channel       = (rmt_channel_t)_channel;
    rmt_rx.gpio_num      = (gpio_num_t)_rxPin;
    rmt_rx.clk_div       = _clkDiv; //80 = 1 MHz, 1 us - we  take samples every 1 microseconds
    rmt_rx.mem_block_num = _memoryBlocks;
    rmt_rx.rmt_mode      = RMT_MODE_RX;
    rmt_rx.flags         = 0;

    rmt_rx.rx_config.filter_en           = false;
    rmt_rx.rx_config.filter_ticks_thresh = 0;

    rmt_config(&rmt_rx);
    rmt_driver_install(rmt_rx.channel, 1000, 0);

    // start receiving data
    esp_err_t result = rmt_rx_start((rmt_channel_t)_channel, true);

    return result;
}

void Esp32RmtReader::Stop()
{
    rmt_driver_uninstall((rmt_channel_t)_channel);
}

void Esp32RmtReader::ReceiveData(uint8_t *messageLength, uint8_t message[])
{
    *messageLength = 0;
    _messageLength = 0;
    memset(_message, 0x00, RMT_READER_MAX_DATA*sizeof(*_message));

    size_t i;
    size_t rx_size = 0;
    rmt_item32_t* items = NULL;

    // define ringbuffer handle
    RingbufHandle_t rb;

    // get the ring buffer handle
    esp_err_t result = rmt_get_ringbuf_handle((rmt_channel_t)_channel, &rb);
    
    if (result == ESP_OK)
    {
        // get items, if there are any
        items = (rmt_item32_t*) xRingbufferReceive(rb, &rx_size, 8);
        if (items)
        {
            // turn on visible led
            SwitchLed(1);

            // call to virtual method which should be implemented in derived class
            BeforeProcessSignal();

            for (i = 0; i < rx_size / 4; i++)
            {
                // call to virtual method which should be implemented in derived class
                ProcessSignal(items[i].level0, items[i].duration0);
                ProcessSignal(items[i].level1, items[i].duration1);
            }
            *messageLength = _messageLength;
            memcpy(message, _message, _messageLength);

            // call to virtual method which should be implemented in derived class
            AfterProcessSignal();

            // turn off visible led
            SwitchLed(0);

            // free up data space
            vRingbufferReturnItem(rb, (void*) items);
        }
    }
}
