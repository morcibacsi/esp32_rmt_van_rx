
# ESP32 RMT peripheral Vehicle Area Network (VAN bus) reader

Arduino friendly [VAN bus][van_network] reader library utilizing ESP32 RMT peripheral.

VAN bus is pretty similar to CAN bus. It was used in many cars (Peugeot, Citroen) made by PSA.

### ESP8266 support
This library is built around the [RMT (Remote control) peripheral][rmt_peripheral] of the ESP32. Therefore **ESP8266 is NOT supported** by this library as it does not have the peripheral built in.

### Schematics

![schema](https://github.com/morcibacsi/esp32_rmt_van_rx/raw/master/schema/esp32-sn65hvd230-iso-a.png)

### Arduino
Copy the following files to your **documents\Arduino\libraries\esp32_arduino_rmt_van_rx** folder
  - esp32_rmt_van_rx.c
  - esp32_arduino_rmt_van_rx.cpp
  - esp32_arduino_rmt_van_rx.h
  - esp32_rmt_van_rx.h
  - keywords.txt
  - library.properties

Check the **esp32_arduino_van_monitor** folder for an example

### ESP-IDF

Check the **main** folder for an example



## See also
Check my [VAN Analyzer for Saleae Logic Analyzer][van_analyzer] also

[van_network]: https://en.wikipedia.org/wiki/Vehicle_Area_Network
[van_analyzer]: https://github.com/morcibacsi/VanAnalyzer/
[rmt_peripheral]: https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/peripherals/rmt.html
