# ESP32 RMT peripheral Vehicle Area Network (VAN bus) reader

Arduino friendly [VAN bus][van_network] reader library utilizing ESP32 RMT peripheral.

VAN bus is pretty similar to CAN bus. It was used in many cars (Peugeot, Citroen) made by PSA.

## Usage

The examples assume that the DATA line (VAN HIGH) is connected to the D22 pin of the ESP32 board.
Make sure to attach the GND line to your ESP32 as well.

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

