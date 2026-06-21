# Node

Node is a shell that exposes LoRa functionality with different behaviours. Behaviours are described using plugins. Node expects 2 macros to be externally defined, `PLUGIN_H` & `NODE_ID`, via cli args `-DPLUGIN_H="humidity.h"`. All devices running this binary should be wired as in [#Wiring](Wiring)

## Plugin Spec

A plugin is a standard C++ class named `Plugin` implementing the interface defined in `plugin.hpp`. Basically override 3 public functions which are:

 - `void setup()`

 - `void loop()`

 - `void onPacketReceived(const Packet&)`

Default serial interface is already started with baud rate of 115200.

## Wiring

Node is designed to work with standard 38-pin esp32 and SX1278 module. Connections between them are as follows:

| SX1278 | esp32 | SX1278 | esp32 |
|--------|-------|--------|-------|
| MISO   | G19   | IO0    | G26   |
| SCK    | G18   | MOSI   | G23   |
| RST    | G14   | NSS    | G5    |
| GND    | GND   | 3V3    | 3V3   |

## Memo

To add external defines through arduino-cli, use the following command

```console
arduino-cli compile --build-property 'build.extra_flags=-DPLUGIN_H="plugins/default.hpp" -DNODE_ID=1' [...Rest of flags]
```