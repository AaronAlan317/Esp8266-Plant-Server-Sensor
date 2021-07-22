# Esp8266-Plant-Server-Sensor

## Description

The Esp8266 is a low-cost IOT solution that is suitable for many applications. This board is equipped with wifi and memory
that allows for hosting a server. It has 16 general purpose input-output pins (GPIO) and 1 analog pin. For this application,
a few low-cost analog sensors need to be read, so the Esp8266 single will have to be extended.

To increase analog inputs, a multiplexer (mux) can be used. A multiplexer is a component that has multiple inputs that can be selected and read through the single analog pin on the board. Four digital pins will act as the selectors; by sending either HIGH or LOW the four pins can access
16 inputs and will tell the mux which input it should be reading and sending. 

In this application, the device will be used to help grow plants more efficiently by measuring data to track and gauge plant health, controlling actions such as 
sending notifications or turning on the watering system, and providing an interface to interact with the system.

In its current state, the device is serving the application that is displaying the sensor's real-time information. Current work is on adding the ability
to turn on a smart relay component and a scheduling system.

## Parts

Everthing was purchased online using standard parts.

| First Header  | Second Header |         |  
| ------------- | ------------- |---------|
| 1  | ESP8266 NodeMCU   | https://www.amazon.com/HiLetgo-Internet-Development-Wireless-Micropython/dp/B010O1        |
| 1  | 16 channel MUX   |   https://www.sparkfun.com/products/9056      |
|1  | DHT11 Temperature Humidity Sensor |https://www.amazon.com/Temperature-Humidity-Digital-3-3V-5V-Raspberry/dp/B07WT2HJ4F/ref=sr_1_3?dchild=1&keywords=sensor+arduino+dht11&qid=1626991863&s=electronics&sr=1-3 |
| 16  | Sensors  |         |
|   |  Capacitive Soil Moisture  | https://www.amazon.com/XM-electronic-Capacitive-Magnetron-Corrosion/dp/B07RYQLP9R/ref=sr_1_4?dchild=1&keywords=sensor+arduino+moisture&qid=1626991482&s=electronics&sr=1-4        |
|   |  Photo cell |    https://www.amazon.com/Adafruit-Photo-cell-photoresistor-ADA161/dp/B00XW2MI4E/ref=sr_1_15?dchild=1&keywords=sensor+arduino+photoresistor&qid=1626991766&s=electronics&sr=1-15     |


## Files

The file **dhtserver.ino** contains the code the sets ups the server and sensors. To use the flash memory on the board and the folder **data** is needed.
In that folder is the html file that gets viewed when connecting to ther server. 
      
