Divoom Pixoo ESP32 Bluetooth Connection over WiFi 
===============================

You own a Divoom Pixoo (16x16) that only supports Bluetooth? 
You are fed up with how restrictive and invasive the Divoom app is?
You still want to send data over Wi-Fi to your Pixoo?

Here is your solution.

Connecting your Pixoo via Bluetooth to an ESP32, while running a webserver on the ESP32 that accepts raw Bluetooth data
via requests. Python handles the conversion of images to bluetooth and sends the requests containing the data.

```mermaid
flowchart LR
  A[Python Script] -->|HTTP POST over WiFi| B[ESP32]
  B -->|Bluetooth SPP| C[Pixoo Display]
```

Dependencies
------------

Use a third-party software such as [ArduinoIDE](https://www.arduino.cc/en/software/) to upload the Firmware to your 
ESP32.

How to use this class
---------------------

This class provides many methods to connect and manage a Pixoo device.

* `connect()̀`: creates a connection with the device and keeps it open while the script is active
* `draw_pic()`: draws a picture (resized to 16x16 pixels) from a PNG file
* `draw_anim()`: displays an animation on the Pixoo based on a GIF file (16x16 pixels)
* `set_system_brightness`: set the global brightness to a specific level (0-100)

Useful Links
---------------------
[Official Divoom Bluetooth protocol](https://docin.divoom-gz.com/web/#/5/146)

[Direct Divoom Pixoo client for Python3](https://github.com/virtualabs/pixoo-client)

