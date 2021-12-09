# ECE-4180-Final-Project
#### By Christopher Jones and Lucas Baxley - Fall 2021

For our Final Project, the team chose to take a Tonka Truck and add an RC component to it. Using the UART and robot parts from the 4180 kit, the team was able to design and create an RC dumptruck with a working dumping mechanism, a sonar distance detector, and working LED headlights while allowing for motion.

![Dump Truck](dump_truck_photo.jpg)

### Block Diagram and Wiring

For the project to make work, the device must be wired up. Below is a block diagram listing signals sent to each device (Note: Every device in the system shares a common ground).

![Block Diagram](4180_Block_diagram.png)

Most of the devices in the system use the 6 V battery back. However, some of the devices additionally use the 4.5 V sent into the Mbed. These include the Piezo Buzzer and the two LED headlights.

Each system is highlighted below with it's wiring guide and code snippets.

#### LED Headlights

The LED Headlights are very simple to wire up. Using a 330 Ω resistor on each jumper, the LEDs are wired as follows:

| Signal      | Pin Location |
| ----------- | ----------- |
| LEDA      | P23       |
| LEDB   | P24        |
| GND   | GND        |

These are then written to by setting Digital Out Variables in the Mbed code. 
``` cpp
DigitalOut LEDA(p23);
DigitalOut LEDB(p24);
```


#### Piezo Buzzer

The Piezo Buzzer is wired as a digital out, just like the LEDs are. The wiring is as follows:
| Signal      | Pin Location |
| ----------- | ----------- |
| Speaker      | P10       |
| GND   | GND        |

To make the buzzer play sound, it does not require a tone. That means a digital out is sufficient. To change the value of the buzzer, do so by writing: 
``` cpp
DigitalOut speaker(p10);
```
Since the speaker should only play in the case of moving packwards, a ticker is used to check if the dumptruck is moving backwards. The ticker and the function setup are below:

``` cpp
Ticker audio_sampletick;

void Alert()
{
    if (!sound) {
        speaker = 0.0;
    } else {
        speaker = 1.0;
        wait(1);
        speaker = 0.0;
        wait(0.5);
    }
}
```
The sound boolean is set in the switch statement used for the UART. That will be examined shortly. 
