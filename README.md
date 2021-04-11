![CovShield - logo ](https://user-images.githubusercontent.com/76181375/113943057-30bd4900-9802-11eb-93b5-91e08ae2d2c9.png)
# CoVShield  - People counter in the room with measurement of their body temperature 🤒


## Table of contents
* General Info
* Libraries 
* Appearance of the device
* How it's working
* App appearance
* Where to follow us 

## General info 
Covshield- It is a project that is to make life easier for society in the era of COVID-19 and more. The main task of the device is to count the people in the room in which it is located, as well as to check the body temperature of the persona before its entrance. The project is innovative and ideally meets the needs of society in the times of the coronavirus pandemic. Our product is distinguished by a very low production price and a relatively small size. This is our first project and we are constantly learning, thanks to which the device will be constantly developed! 🔜📈

Until then, we had built our program without using git. Our latest TiP.ino program is now added. From now on, any changes to the program will be added to Github 📌📂

## [Libraries](https://github.com/Covshield/CoVShield/tree/main/Libraries)

* [sh1106.h](https://github.com/Covshield/CoVShield/blob/main/Libraries/sh1106.zip)
* [Wire.h] - installed in the arduino IDE
* [SparkFunMLX90614.h](https://github.com/Covshield/CoVShield/blob/main/Libraries/SparkFun_MLX90614_Arduino_Library-master.zip)
* [stdlib.h] - Standard C tools (no download needed)
* [ArduinoHttpClient.h](https://github.com/Covshield/CoVShield/blob/main/Libraries/ArduinoHttpClient.zip)
* [WiFi101.h](https://github.com/Covshield/CoVShield/blob/main/Libraries/WiFi101.zip)
* [ArduinoJson.h](https://github.com/Covshield/CoVShield/blob/main/Libraries/ArduinoJson.zip)

## Appearance of the device

![Appearance of the device](https://github.com/Covshield/CoVShield/blob/main/Assets/IMG_6045.JPG)


## How it's work 

This device is based on the principle of a sequence of three sensors. Two
that are dependent on each other in an appropriate manner.
The mutual cooperation of these sensors is to determine whether the movement is taking place
inside or out. Third sensor - the pyrometer is only activated
at the entrance to the facility. The command to measure the temperature is displayed on
the screen after detecting movement from the outside, and the measurement itself begins,
when the sensor captures the heat of the human body.

## The photos below show the operation of the device
#### The device does not detect any person
![entrance](https://github.com/Covshield/CoVShield/blob/main/Assets/IMG_6011.jpg)
#### The device detects the entering person
![persondetect](https://github.com/Covshield/CoVShield/blob/main/Assets/IMG_6021.JPG)
#### A person puts his forehead close to the pyrometer, the temperature is measured
![preparemeasurment](https://github.com/Covshield/CoVShield/blob/main/Assets/IMG_6029.JPG)
#### Reading a negative result, switching on the red diode, relay and the buzzer,(in this case, the maximum value is set to 35 ° C)
![wrongtemperature](https://github.com/Covshield/CoVShield/blob/main/Assets/IMG_6034.JPG)
#### Reading a positive result, switching on the green LED
![correcttemperature](https://github.com/Covshield/CoVShield/blob/main/Assets/IMG_6036.JPG)
#### The person goes inside, the device adds that person to the result and transmits it state to the application
![personadd](https://github.com/Covshield/CoVShield/blob/main/Assets/IMG_6039.JPG)

## App appearance
#### Phone 
![phone](https://github.com/Covshield/CoVShield/blob/main/Assets/Zrzut%20ekranu%202021-04-8%20o%2014.14.45.png)
#### PC
![pc](https://github.com/Covshield/CoVShield/blob/main/Assets/Zrzut%20ekranu%202021-04-8%20o%2014.17.49.png)

Link to app: [CovShield](http://cov-shield.herokuapp.com)

Link to presentation: [Presentation](https://drive.google.com/file/d/1Qz6SYl5HPVpwsYF9ErakJo_NrsfTmzBv/view?usp=sharing)

## Where follow us 

![Facebook](https://github.com/paulrobertlloyd/socialmediaicons/blob/main/facebook-48x48.png)[Facebook](https://www.facebook.com/CovShieldpl)

![Instagram](https://github.com/paulrobertlloyd/socialmediaicons/blob/main/instagram-48x48.png)[Instagram](https://www.instagram.com/covshieldpl)


