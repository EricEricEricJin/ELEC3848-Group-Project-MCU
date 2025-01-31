# ELEC3848 Final Project Code on Arduino MCU

In this project we made a autonomous model plane towing truck for model plane airfields. It uses two Arduino mega 2560 boards and one Nvidia Jetson Nano. 

This repo holds the program that runs on two Arduino Mega 2560 MCU boards, 
the host computer (nvidia jetson nano)'s program is held at [https://github.com/Zou-2004/ELEC3848-Jetson/tree/main/tracker_test_v3](https://github.com/Zou-2004/ELEC3848-Jetson/tree/main/tracker_test_v3).


Two applications run on two boards are selected by jump-wire as in `application/init`:
- `application/chassis`: for chassis control board 
- `application/roboarm`: for roboarm control board 

Final report: [ELEC3848_Final_report.pdf](ELEC3848_Final_report.pdf)

Coding style is heavily inspired by and some modules are borrown from the projects below:  
- DJI Robomaster. RoboRTS-Firmware. [https://github.com/RoboMaster/roboRTS-Firmware/](https://github.com/RoboMaster/roboRTS-Firmware/)
