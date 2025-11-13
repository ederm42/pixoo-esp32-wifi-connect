# Divoom Pixoo ESP32 Bluetooth Connection over WiFi 

You own a Divoom Pixoo (16x16) that only supports Bluetooth? 
You are fed up with how restrictive and invasive the Divoom app is?
You still want to send data over Wi-Fi to your Pixoo?

Here is your solution.

Connecting your Pixoo via Bluetooth to an ESP32, while running a webserver on the ESP32 that accepts raw Bluetooth data
via requests. Python handles the conversion of images to bluetooth and sends the requests containing the data of any image
you want to display.

```mermaid
flowchart LR
  A[Python Script] -->|HTTP POST over WiFi| B[ESP32]
  B -->|Bluetooth SPP| C[Pixoo Display]
```

## Dependencies

Use a third-party software such as [ArduinoIDE](https://www.arduino.cc/en/software/) to upload the Firmware to your 
ESP32.

## How to use

### ESP32 Setup

To set up and flash your ESP, open up `ESP32_wifi_to_bluetooth.ino`. 
Adapt the `PIXOO_ADDR[6]` to your Pixoo's MAC Address. Set `WIFI_SSID` and `WIFI_PASS`.

Upload to your ESP32. If the serial monitor isn't working, set `baud` to `115200`.

### Verify Connection to Pixoo

Once uploaded, the ESP32 should start up. Your Pixoo should, for a brief moment of like 500ms, flash a bluetooth 
icon on the screen.

If this is not the case, double-check your `PIXOO_ADDR[6]` or 
play around with `PIXOO_CHANNEL`. Default for the devices this was tested on is `PIXOO_CHANNEL=2`, but `1` or other 
values might also work.

### Send Data to Pixoo

Open the `pixoo.py` and set your `ESP32_IP`. Run the default script. This sends the image to `screen 0` (the first 
screen that comes after the black screen when cycling through them on the Pixoo). 

A smiley should show on screen 🙂

The ESPtoPixoo class provides many methods to connect and manage a Pixoo device.

* `draw_pic()`: draws a picture (resized to 16x16 pixels) from a PNG file
* `draw_anim()`: displays an animation on the Pixoo based on a GIF file (16x16 pixels)
* `set_system_brightness`: set the global brightness to a specific level (0-100)

## Useful Links
[Official Divoom Bluetooth protocol](https://docin.divoom-gz.com/web/#/5/146)

[Direct Divoom Pixoo client for Python3](https://github.com/virtualabs/pixoo-client)

