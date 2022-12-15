# VÂTHULT3000 - Electronic
based on an esp32 s2, the electronics must interface with:
- 6 12V solenoid valves
- 1 pump whose supply voltage is not yet defined
- 6 flow meters 
- 1 water infiltration sensor in the box
- Screen and buttons 
- WiFi

## How it works?
![Functionnal Diagram](https://github.com/normanalie/vathult3000/blob/8e3e63d5b4aee03f58972e7862d0e44c2d594944/hardware/electronic/BlocDiagram_Electronic.png?raw=true)

## Power supply
The VÂTHULT3000 is equiped with a 12V internal PSU but all the loggic is 3.3V. 
We use a Buck converter to step-down this voltage.

## I/O
For prototyping purposes we choose a multi-PCB design. So we decide to use I2C and PCF8574 I/O expander.

## Solenoid valves
Each valve draw around 10W of power but a more industrial built solenoid valve is around 20W. To be on the safe side, I choose 12V/3A of rating per output.

## Pump
We need a pump to pressurize the rain water but without testing we can't decide exactly which pump to choose. For this prototype I add 2 pump output, one output is 12V/3A and the other one is a relay that can handle mains power.

## Flow meters
To detect a clogged pipe or an empty water tank we use flow meters on inputs and outputs. Those we have choosen are hall-effect flow meters.
The hall-effect sensor need a power supply from5 to 15V. Since we only have 12 and 3.3V, we choose to use 12V to power the sensor and add a level-shifter before the I/O.

## Water infiltration sensor
This box full of electronic is design to stay outside. If the seal is broken this could result in catastrophic failure.
We decide to add a very simple infiltration sesnor using 2 bare wires in the box, connecting one to 3.3V and the othjer to an ADC and measuring the voltage accross them.
We use one of the integrated ADC of the ESP32

## Screen and buttons
To keep up with the times we choose a fairly cheap OLED display. It fit perfectly on the board with a simple 3 buttons navigation under it.
