#include <string.h>
#include "esp32_rmt_van_rx.h"

volatile uint8_t _rmt_van_rx_ledPin;
volatile uint8_t _rmt_van_rx_rxPin;
volatile uint8_t _rmt_van_rx_channel;
volatile RX_VAN_LINE_LEVEL _rmt_van_rx_van_line_level;

/* Initialize LED */
void rmt_van_rx_led_init(uint8_t ledPin)
{
    _rmt_van_rx_ledPin = ledPin;
    gpio_pad_select_gpio(ledPin);
    gpio_set_direction(ledPin, GPIO_MODE_OUTPUT);
}

/* Rounds a number to the nearest multiple */
uint8_t round_to_nearest(uint8_t numToRound, uint8_t multiple)
{
    if (multiple == 0)
        return numToRound;

    uint8_t remainder = numToRound % multiple;
    if (remainder == 0)
        return numToRound;

    uint8_t next = numToRound + multiple - remainder;
    uint8_t prev = next - multiple;

    if (next - numToRound < numToRound - prev)
    {
        return next;
    }
    else
    {
        return prev;
    }
}

/* Converts the RMT level and duration into a VAN byte */
bool rmt_van_rx_parse_byte(uint8_t level, uint32_t duration, uint8_t *bitCounter, uint8_t *tempByte, uint8_t *mask, uint8_t *finalByte)
{
    bool result = false;
    if (_rmt_van_rx_van_line_level == RX_VAN_LINE_LEVEL_LOW)
    {
        level = !level;
    }

    // on the bus the time slices are a little off from the multiple of 8 microseconds, so we round it to the nearest multiple of 8 before dividing
    uint8_t countOfTimeSlices = round_to_nearest(duration, 8) / 8;

    for (int i = 0; i < countOfTimeSlices; i++)
    {
        // every 5th bit is a manchester bit, we must skip them while we are building our byte
        bool isManchesterBit = (*bitCounter + 1) % 5 == 0;
        if (!isManchesterBit)
        {
            if (level == 1)
            {
                *tempByte |= *mask;
            }
            *mask = *mask >> 1;
        }
        else
        {
            // if we found the second manchester bit, then we have the full byte in the tempByte variable, so we place it in the finalByte and we can start building the next byte
            if (*bitCounter == 9)
            {
                *bitCounter = -1;
                *mask = 1 << 7;
                *finalByte = *tempByte;
                *tempByte = 0;
                result = true;
            }
        }
        *bitCounter = *bitCounter + 1;
    }

    return result;
}

/* RMT receiver function */
void rmt_van_rx_receive(uint8_t *vanMessageLength, uint8_t vanMessage[]) 
{
    *vanMessageLength = 0;

    size_t i;
    size_t rx_size = 0;
    rmt_item32_t* items = NULL;

    // define ringbuffer handle
    RingbufHandle_t rb;

    // get the ring buffer handle
    rmt_get_ringbuf_handle(_rmt_van_rx_channel, &rb);

    // get items, if there are any
    items = (rmt_item32_t*) xRingbufferReceive(rb, &rx_size, 8);
    if (items)
    {
        // turn on visible led
        gpio_set_level(_rmt_van_rx_ledPin, 1);

        uint8_t bitCounter = 0;
        uint8_t mask = 1 << 7;
        uint8_t tempByte = 0;
        uint8_t finalByte = 0;
        bool isCompleteByte = false;

        *vanMessageLength = 0;

        for ( i=0; i < rx_size / 4; i++ )
        {
            isCompleteByte = rmt_van_rx_parse_byte(items[i].level0, items[i].duration0, &bitCounter, &tempByte, &mask, &finalByte);
            if (isCompleteByte)
            {
                vanMessage[*vanMessageLength] = finalByte;
                *vanMessageLength = *vanMessageLength + 1;
            }

            isCompleteByte = rmt_van_rx_parse_byte(items[i].level1, items[i].duration1, &bitCounter, &tempByte, &mask, &finalByte);
            if (isCompleteByte)
            {
                vanMessage[*vanMessageLength] = finalByte;
                *vanMessageLength = *vanMessageLength + 1;
            }
        }

        // turn off visible led
        gpio_set_level(_rmt_van_rx_ledPin, 0);

        // free up data space
        vRingbufferReturnItem(rb, (void*) items);
    }
}

/* 
    Calculates the CRC-15 of the input data to according to the VAN ISO/11519–3 standard 
    Borrowed from here: http://graham.auld.me.uk/projects/vanbus/crc15.html
*/
uint16_t rmt_van_rx_crc15(uint8_t data[], uint8_t lengthOfData)
{
    // computes crc value
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t k = 0;
    uint8_t bit = 0;

    const uint8_t order = 15;
    uint8_t polynom[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x9D }; //0xF9D;
    uint8_t xor[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF };     //0x7FFF;
    uint8_t mask[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF };    //0x7FFF;

    uint16_t crc[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0x00 };

    // main loop, algorithm is fast bit by bit type
    for (i = 0; i < lengthOfData; i++)
    {
        uint8_t currentByte = data[i];

        // rotate one data byte including crcmask
        for (j = 0; j < 8; j++)
        {
            bit = 0;
            if ((crc[7 - ((order - 1) >> 3)] & (1 << ((order - 1) & 7)))>0)
            {
                bit = 1;
            }
            if ((currentByte & 0x80) > 0)
            {
                bit ^= 1;
            }
            currentByte <<= 1;
            for (k = 0; k < 8; k++)     // rotate all (max.8) crc bytes
            {
                crc[k] = ((crc[k] << 1) | (crc[k + 1] >> 7)) & mask[k];
                if (bit > 0)
                {
                    crc[k] ^= polynom[k];
                }
            }
        }
    }

    uint16_t finalCrc = 0;

    // perform xor value
    for (i = 0; i < 8; i++)
    {
        crc[i] ^= xor[i];
        if (crc[i] != 0)
        {
            if (finalCrc == 0)
            {
                finalCrc |= crc[i];
            }
            else
            {
                finalCrc = crc[i] | finalCrc << 8;
            }
        }
    }

    // multiply result by 2 to turn 15 bit result into 16 bit representation
    return finalCrc << 1;
}

/* Returns true if the supplied VAN message has a good CRC value at the last two bytes */
bool rmt_van_rx_is_crc_ok(uint8_t vanMessage[], uint8_t vanMessageLength)
{
    uint8_t crcByte1 = vanMessage[vanMessageLength - 2];
    uint8_t crcByte2 = vanMessage[vanMessageLength - 1];
    uint16_t crcValueInMessage = crcByte1 << 8 | crcByte2;

    uint8_t vanMessageWithIdWithoutCrc[32];
    memcpy(vanMessageWithIdWithoutCrc, vanMessage + 1, vanMessageLength - 3);

    uint16_t calculatedCrc = rmt_van_rx_crc15(vanMessageWithIdWithoutCrc, vanMessageLength - 3);

    return crcValueInMessage == calculatedCrc;
}

/* Initialize RMT receive channel */
void rmt_van_rx_channel_init(uint8_t channel, uint8_t rxPin, uint8_t ledPin, RX_VAN_LINE_LEVEL vanLineLevel)
{
    _rmt_van_rx_ledPin = ledPin;
    _rmt_van_rx_channel = channel;
    _rmt_van_rx_rxPin = rxPin;
    _rmt_van_rx_van_line_level = vanLineLevel;

    rmt_van_rx_led_init(_rmt_van_rx_ledPin);

    rmt_config_t rmt_rx;

    rmt_rx.channel       = channel;
    rmt_rx.gpio_num      = rxPin;
    rmt_rx.clk_div       = 80; // 1 MHz, 1 us - we  take samples every 1 microseconds so in our receive task we are going to have multiple of 8 us durations (8us is the so called 'time slice' on a 125kbs VAN bus)
    rmt_rx.mem_block_num = 2;
    rmt_rx.rmt_mode      = RMT_MODE_RX;

    rmt_rx.rx_config.filter_en           = false;
    rmt_rx.rx_config.filter_ticks_thresh = 0;
    rmt_rx.rx_config.idle_threshold      = 80; // we consider the packet whole after 80 ticks of unchanged line state

    rmt_config(&rmt_rx);
    rmt_driver_install(rmt_rx.channel, 1000, 0);

    // start receiving VAN data
    rmt_rx_start(_rmt_van_rx_channel, 1);
}

/* Uninstall the RMT driver */
void rmt_van_rx_channel_stop(uint8_t channel)
{
    rmt_driver_uninstall(channel);
}