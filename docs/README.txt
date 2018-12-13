Francis Mendoza
06:19:52 hrs MST
12/13/2018

# Documentation README
------------------------------------------------------------------------------
## -- INTRODUCTION -- 
My project is an IoT Weather Sensor that measures both temperature
and humidity and feeds it into the cloud. The data is then displayed 
via a very simple webpage in real-time.

My project used an ESP8266- a lightweight version of the Arduino that 
has Wifi capabilities and that can only load firmware. 

I wired a DHT11 weather and temperature sensor, as well as LED's to signify
if values tripped above a certain limit 

Back-end used Amazon Web Services (AWS), Node.js and MongoDB

In terms of functionality, I used Nodemailer to send an email alert 
whenever the temperature exceeds 100*F, and humidity 50% in an interval
of every 5 minutes 

In terms of front-end, I used the C3.js library to display real-time data
to my simple webpage (pure html) and to display past data stored via MongoDB
------------------------------------------------------------------------------
## -- NOTES --
* Languages used were Javascript (Microcontroller and Server-side code),
and html

* One needs to go to the AWS terminal to run node.js first before the webpage
can receive data from the sensor 

* One needs to use specific powerbanks to supply the ESP8266 with power, as some
automatically shut off for some unknown reason
------------------------------------------------------------------------------