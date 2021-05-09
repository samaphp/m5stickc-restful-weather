# M5stick RESTful weather
M5stick-c RESTful weather to fetch (tempreture, humidity and air pressure) using the DHT12 hat (ENV).

I created  this project to replace the commercial devices and to integrate these to my local home system as per my requirements.

# Installation
1. Clone this project or download it on your local machine.
1. Download [Arduino](https://www.arduino.cc/en/software) IDE.
1. Open Arduino IDE, and open the `m5stickc-restful-weather.ino` from this project.
1. Add the ESP32 Board manager by going to (Preferences) then Add `https://dl.espressif.com/dl/package_esp32_index.json` inside the textfield of (Additional Boards Manager URLs) And click (OK) to save this change.
1. Go to (Tools) menu then (Board) then (Boards manager) to allow the IDE to download the ESP32 board.
1. Search for `ESP32` and install it then close the window. (for me the version was 1.0.6)
1. Now go to the (Tools) menu then (Board:) then (ESP32 Arduino) then Choose (M5stick C)
1. Install the required libraries: `M5StickC` and `ESP32_HTTP_SERVER`. Go to (Sketch) menu then (Include library) then (Manage Libraries).
1. Search for `M5StickC` and install the latest version. (for me the version was 0.2.0)
1. Also, Search for `ESP32_HTTP_SERVER` and install the latest version. (for me the version was 1.0.0)
1. Also, Search for `Adafruit_BMP280` and install the latest version with it's all dependencies. (for me the version was 2.1.2)
1. Install `CP210x USB to UART` driver from (Download) section from this [link](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers).
1. Set your WiFi SSID and password by editing the `m5stickc-restful-weather.ino` file on line number 21 & 22.
1. Now, you are good to connect your M5StickC and compile the code. Click to (Upload) button.

----

Photo of my M5stickC showing live weather details and the M5stickC IP:
![M5stick photo showing the live data](https://github.com/samaphp/m5stick-restful-weather/raw/master/photo.png)

Screen shot of the JSON response when visiting the M5stickC IP:
![M5stick JSON format screenshot](https://github.com/samaphp/m5stick-restful-weather/raw/master/screenshot.png)

----

**Credits:** Weather functionality from the [M5stickC examples](https://github.com/m5stack/M5StickC/tree/master/examples/Hat/ENV) please go checkout the powerful examples in their repository.
