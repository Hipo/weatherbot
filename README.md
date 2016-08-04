Weatherbot
==========

Firmware for a mobile weather station, built using ESP8266 ESP-01 and a DHT-22 temperature and humidity sensor. Device is capable of running on a 4xAA battery pack for a long time, delivering data at specific intervals over WiFi.

Repo also contains `weatherserver.py`, a simple Flask server for testing requests on a local network. It outputs incoming messages from the device on the stdout.

Schematic
---------

![Weatherbot Schematic](/weatherbot_schem.png?raw=true "Weatherbot Schematic")

Credits
-------

Weatherbot is developed by 
[Taylan Pince](http://taylanpince.com) and the [Hipo Team](http://hipolabs.com).
