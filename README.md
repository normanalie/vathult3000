# VÂTHULT3000

VÂTHULT3000 is a smart multi-zones programmable watering system.
It is capable of controlling the watering of 4 distinct zones and has 2 water inlets.
One for a tap and one with a pump for a rainwater collector.


The VÂTHULT3000 is based on an ESP32. It communicates with a server through a API. In the future we can imagine integrating it into [Home Assistant](https://github.com/home-assistant).


This first prototype uses solenoid valves and 5 different PCBs. It has been designed to be simple to implement and simple to modify. Cost and size are not constraints.

## Home Assistant integration
Import the .yaml file into your ESPHome dashboard and flash the ESP.
After adoption you'lle have:
![Vathult in Home Assistant](V3K_ESPHOME.png?raw=true)

## How to use?
TODO.

## Why this project?
The VÂTHULT3000 is a school project realized in the 2nd year of the engineering preparatory class at [ISTY](https://www.isty.uvsq.fr/).

## For who?
This product is intended for people who have a garden and want to automate it or who do not have the time to water it as regularly as they should.

## How it works?
![Functionnal Diagram](BlocDiagram_Global.png?raw=true)
