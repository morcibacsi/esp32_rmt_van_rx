/* RMT VAN bus receive example, displaying the received VAN messages on the console
 * 
 * The software is distributed under the MIT License
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"

#include "esp32_rmt_van_rx.h"

const uint8_t VAN_DATA_RX_RMT_CHANNEL = 0;
const uint8_t VAN_DATA_RX_PIN = 21;
const uint8_t VAN_DATA_RX_LED_INDICATOR_PIN = 2;

void rmt_van_rx_receive_task(void *pvParameter)
{
    uint8_t vanMessageLength;
    uint8_t vanMessage[34];
    for(;;)
    {
        rmt_van_rx_receive(&vanMessageLength, vanMessage);

        if (vanMessageLength > 0)
        {
            if(rmt_van_rx_is_crc_ok(vanMessage, vanMessageLength))
            {
                for(size_t i = 0; i < vanMessageLength; i++)
                {
                    if ( i != vanMessageLength-1)
                    {
                        printf("%02X ", vanMessage[i]);
                    }
                    else
                    {
                        printf("%02X", vanMessage[i]);
                    }
                }
                printf("\n");
            }
            else
            {
                printf("CRC ERROR!\n");
            }
        }

        // delay 10 milliseconds. No need to overheat the processor
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}

void app_main() {
    // initialize hardware
    rmt_van_rx_channel_init(VAN_DATA_RX_RMT_CHANNEL, VAN_DATA_RX_PIN, VAN_DATA_RX_LED_INDICATOR_PIN, RX_VAN_LINE_LEVEL_HIGH, RX_VAN_NETWORK_COMFORT);
    //rmt_van_rx_channel_init(VAN_DATA_RX_RMT_CHANNEL, VAN_DATA_RX_PIN, VAN_DATA_RX_LED_INDICATOR_PIN, RX_VAN_LINE_LEVEL_HIGH, RX_VAN_NETWORK_BODY);

    // start receive task
    xTaskCreate(rmt_van_rx_receive_task, "rmt_van_rx_receive_task", 1024*2, NULL, 10, NULL);
}
