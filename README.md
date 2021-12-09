# ECE-4180-Final-Project
#### By Christopher Jones and Lucas Baxley - Fall 2021

For our Final Project, the team chose to take a Tonka Truck and add an RC component to it. Using the UART and robot parts from the 4180 kit, the team was able to design and create an RC dumptruck with a working dumping mechanism, a sonar distance detector, and working LED headlights while allowing for motion.

![Dump Truck](dump_truck_photo.jpg)

### Block Diagram and Wiring

For the project to make work, the device must be wired up. Below is a block diagram listing signals sent to each device (Note: Every device in the system shares a common ground).

![Block Diagram](4180_Block_diagram.png)

Most of the devices in the system use the 6 V battery back. However, some of the devices additionally use the 4.5 V sent into the Mbed. These include the Piezo Buzzer and the two LED headlights.


