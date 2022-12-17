# ESP32 RMT peripheral Vehicle Area Network (VAN bus) reader

Arduino [VAN bus][van_network] reader library utilizing ESP32 RMT peripheral.

VAN bus is pretty similar to CAN bus. It was used in many cars (Peugeot, Citroen) made by PSA.

### ESP8266 support
This library is built around the [RMT (Remote control) peripheral][rmt_peripheral] of the ESP32. Therefore **ESP8266 is NOT supported** by this library as it does not have the peripheral built in. However on these boards you can use this library: [PSA VAN bus reader for Arduino-ESP8266][van_reader_for_esp8266]

### Schematics

![schema](https://github.com/morcibacsi/esp32_rmt_van_rx/raw/master/extras/schema/esp32-sn65hvd230-iso-a.png)

### Arduino
Copy the following files to your **documents\Arduino\libraries\esp32_arduino_rmt_van_rx** folder
  - esp32_arduino_rmt_van_rx.cpp
  - esp32_arduino_rmt_van_rx.h
  - Esp32RmtReader.cpp
  - Esp32RmtReader.h
  - keywords.txt
  - library.properties

Check the **esp32_arduino_van_monitor** folder for an example

### Breaking change

In version 2.0.0 the library was rewritten and ESP-IDF support was dropped in favor of simpler code in Arduino framework. The change also makes it possible to monitor the VAN_COMFORT and VAN_BODY bus simultaneously by creating two separate instances of the ```ESP32_RMT_VAN_RX``` class. The example was updated to reflect the change, so you can check that as well.

#### Example code
```cpp
// Instead of calling the Init function with the following parameters:
VAN_RX.Init(VAN_DATA_RX_RMT_CHANNEL, VAN_DATA_RX_PIN, VAN_DATA_RX_LED_INDICATOR_PIN, VAN_LINE_LEVEL_HIGH, VAN_NETWORK_TYPE_COMFORT);
//Receive() method was renamed to ReceiveData()
VAN_RX.Receive(&vanMessageLength, vanMessage)

// You need to pass the same parameters to the constructor like this:
VAN_RX = new ESP32_RMT_VAN_RX(VAN_DATA_RX_RMT_CHANNEL, VAN_DATA_RX_PIN, VAN_DATA_RX_LED_INDICATOR_PIN, VAN_LINE_LEVEL_HIGH, VAN_NETWORK_TYPE_COMFORT);
// And call the Start() method:
VAN_RX->Start();
// Receive data from the bus
VAN_RX->ReceiveData(&vanMessageLength, vanMessage);
```

## See also
- [VAN Analyzer for Saleae Logic Analyzer][van_analyzer]
- [TSS463/461 library for reading and also safely writing the VAN bus][tss_46x library]
- [VAN bus reader for STM32F103 (Blue Pill)][stm32_van_bus]

[van_network]: https://en.wikipedia.org/wiki/Vehicle_Area_Network
[van_analyzer]: https://github.com/morcibacsi/VanAnalyzer/
[rmt_peripheral]: https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/peripherals/rmt.html
[van_reader_for_esp8266]: https://github.com/0xCAFEDECAF/VanBus
[tss_46x library]: https://github.com/morcibacsi/arduino_tss463_van
[stm32_van_bus]: https://github.com/morcibacsi/stm32_arduino_van_bus
