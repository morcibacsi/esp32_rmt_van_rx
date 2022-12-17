/* Arduino version of the ESP32 RMT VAN bus receive example, 
 * displaying the received VAN messages on the serial console
 * 
 * The software is distributed under the MIT License
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
*/
#include <esp32_arduino_rmt_van_rx.h>

TaskHandle_t VANReadDataTask;

ESP32_RMT_VAN_RX *VAN_RX;

const uint8_t VAN_DATA_RX_RMT_CHANNEL = 0;
const uint8_t VAN_DATA_RX_PIN = 21;
const uint8_t VAN_DATA_RX_LED_INDICATOR_PIN = 2;

uint8_t vanMessageLength;
uint8_t vanMessage[34];

uint32_t lastMillis = 0;

void VANReadDataTaskFunction(void * parameter)
{
    VAN_RX = new ESP32_RMT_VAN_RX(VAN_DATA_RX_RMT_CHANNEL, VAN_DATA_RX_PIN, VAN_DATA_RX_LED_INDICATOR_PIN, VAN_LINE_LEVEL_HIGH, VAN_NETWORK_TYPE_COMFORT);
    //VAN_RX = new ESP32_RMT_VAN_RX(VAN_DATA_RX_RMT_CHANNEL, VAN_DATA_RX_PIN, VAN_DATA_RX_LED_INDICATOR_PIN, VAN_LINE_LEVEL_HIGH, VAN_NETWORK_TYPE_BODY);
    VAN_RX->Start();

    for (;;)
    {
        VAN_RX->ReceiveData(&vanMessageLength, vanMessage);

        if (vanMessageLength > 0)
        {
            if(!VAN_RX->IsCrcOk(vanMessage, vanMessageLength))
            {
              printf("CRC ERROR: ");
            }
            {
                for (size_t i = 0; i < vanMessageLength; i++)
                {
                    if (i != vanMessageLength - 1)
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
        }
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}

void setup()
{
    Serial.begin(500000);
    printf("ESP32 Arduino VAN bus monitor\n");

    xTaskCreatePinnedToCore(
        VANReadDataTaskFunction,        // Function to implement the task
        "VANReadDataTask",              // Name of the task
        20000,                          // Stack size in words
        NULL,                           // Task input parameter
        1,                              // Priority of the task (higher the number, higher the priority)
        &VANReadDataTask,               // Task handle.
        0);                             // Core where the task should run
}

void loop()
{
    vTaskDelay(50 / portTICK_PERIOD_MS);
}
