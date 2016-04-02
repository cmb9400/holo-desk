holodesk
========

An arduino, some sensors, some projectors, and a table combine to make an interactive system.

![office lens_20160227_160848_processed](https://cloud.githubusercontent.com/assets/326557/14228694/18294bac-f8eb-11e5-8ad7-1dd16b162727.jpg)


Accelerometer
-------------
We are using a [ADXL345 Accelerometer](https://www.sparkfun.com/products/9836) to detect motion in the table. The primary use will be to determine if there is a tap on the table. We are looking into wether these will also help us deteremine tap position.

Arduinos
--------
We are using the [Arduino MEGA 2560](https://www.arduino.cc/en/Main/ArduinoBoardMega2560) to get information from the accelerometers. We are also using the [Arduino Leonardo](https://www.arduino.cc/en/Main/ArduinoBoardLeonardo) to act as a HID to have the system act effectively as a mouse.

Raspberry Pi
------------
We are using 2 [Raspberry Pi](https://www.raspberrypi.org/)s to process and transfer information between the two arduinos.

Kinect
------
We are using the [Kinect](https://developer.microsoft.com/en-us/windows/kinect) to detect hand motions and handle interactions that might happen off the table.
