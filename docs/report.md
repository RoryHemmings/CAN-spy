# Report

# Table of Contents
* [Abstract](#abstract)
* [Introduction](#1-introduction)
* [Related Work](#2-related-work)
* [Technical Approach](#3-technical-approach)
* [Evaluation and Results](#4-evaluation-and-results)
* [Discussion and Conclusions](#5-discussion-and-conclusions)
* [References](#6-references)

# Abstract

Provide a brief overview of the project objhectives, approach, and results.

# 1. Introduction

This section should cover the following items:

## Motivation and Objective
The Controller Area Network (CAN) protocol is an extremely common serial communication protocol used frequently in large complex systems like automobiles, ships, trains, planes, factories, and other industrial technologies. It's priority based arbitration design makes it appealing to systems where reliability and real-time responses are critical. While robust and flexible, the standard is relatively old, and thus doesn't hold up under modern security standards. As a result, the standard is a serious weak point in critical infrastructure and thus in recent years it has drawn much attention from bad actors and security researchers alike. The goal of this project is to provide an extensible platform for developing CAN exploits for use by security researchers when protecting against these attacks.

## Current State of the Art
Currently, security researchers seeking to harden their system have a couple options. Most attacks in this field focus on automotive attacks as cars are the most prevalent users of CAN in our daily lives. As a result, a lot of the existing infrastructure is based around cars. One such example hardware is the OBD-II cable. OBD-II is the standard used to read information from a cars computer system. It provides a standard port which allows you to read from the internal CAN bus along with a couple other busses. This is almost always the hardware associated with CAN security research.

Once you have access to the physical CAN bus via the OBD-II cable, you can read or write from the CAN bus using `can-utils`. This is a repository of command line tools which you can use to interact with the CAN bus.

However, most security research is actually done in simulation. One such simulator is

Taking all of this into account, the current state of the art effectively involves developing your exploit in the simulator using `can-utils`, and then testing it using an OBD-II cable.

This can be effective for people trying to write attacks for existing cars, but it does little to address attacks while developing systems or non-automotive attacks. Additionally, using `can-utils` can be constricting if you need to develop more complex attacks. Also, testing and carrying out attacks require physical access to the CAN-bus. While there are wireless OBD-II readers, they are limited to systems using OBD-II. Finally, existing hardware solutions are relatively expensive, with wireless OBD-II readers costing up to $200.

Few projects have unified all these pieces into one simple, extensible, and easy to use framework, which is what this project aims to do.

## Novelty and Rationale

While no individual part of the project is particularly novel, the unificafion of all parts is where it becomes truly novel.

While wireless OBD-II readers exist, they are limited to automotive applications and have little purpose for security research due to their lack of support for complex attacks. Also, they almost exclusively use Bluetooth. While the initial iteration of this project uses WiFi, the hardware and software frameworks are modular open source, meaning it would be easy to extend the capabilities in the future to support much longer range attacks protocols like LoRa which could open the door to a huge amount of new attacks.

Additionally, this framework is geared towards the development of safer systems instead of interacting with existing systems. As such, it's hardware interface is raw CAN as opposed to OBD-II providing more flexibility across domains other than automotive.

As far as I know, there are no existing python frameworks that allow you to wirelessly read/write to a physical CAN bus, and thus this is the first of it's kind.

Finally, the device can likely be mass produced for less than $20 a piece given that it simply requires an MCU, CAN hardware, and some sort of wireless module.

## Potential impact

The overall goal of this project is to help improve the security of digital infrastructure by developing a tool which can be useful for security researchers to efficiently hunt for potential vulnerabilities in CAN systems. It can also be used by engineers when developing new systems to keep them more secure, as well as less experienced. The list of command line scripts (while currently small) of common attacks will also be an easy way for researchers to make sure their system holds up against the most trivial of attacks before needing to develop anything fancy.

The extensible python package is also great as it lowers the barrier to entry significantly for CAN security. Existing solutions require expert knowledge in C and linux which some people might not have. This project gives makes it easy to develop exploits and search for vulnerabilities without extensive knowledge of these systems. Even for experienced users, it may make the process of security research more efficient and less error prone.

## Challenges:
The main challenge in completing this project is developing the hardware. For reasons mentioned in the technical section, CAN is actually rather complicated and requires several additional pieces of hardware to get working. Testing the hardware also requires a mock system (testbed), which will need to be designed and implemented. This is compounted with my limited background in hardware and reasonably short time constraints. 

Another challenge is reliable websocket connections between the software C2 server and the hardware. In my experience, getting websockets to run reliably on Arduino is quite difficult. Many libraries are buggy, or simply don't work as intended. Using websockets in python also requires asynchronous programming paradigms which can be tricky to get correct without bugs.

Finally, reliably streaming CAN traffic via websocket is difficult given that the max bitrate of CAN is 1 megabit per second. While WiFi can easily handle this, we eventually want the project to support other wireless technologies such as LoRa and Bluetooth so we need to address this bottleneck.

## Requirements for Success:
In order for this project to succeed, I will need the available hardware and advice on how to implement the hardware for the actual device along with baseline electrical engineering skills and knowledge. This is all available via Amazon, IEEE, and the makerspace. The software skills required include Python programming, C programming, embedded C practices, and network programming (especially websockets) all of which I have proficiency in.

## Metrics for Success
Overall, the success of this project is determined by whether I can get the following features working.

* CAN system testbed with two mock ECU's, a sensor (IMU) and actuator (servo)
* Byte level read/write access to the physical CAN bus
* A python package which allows persistent websocket connection and remote read/write access to CAN bus via the device.
* Several out of the box command line utilities for common attack and workflows

# 2. Related Work

* existing CAN attacks in theory
  * replay
  * spoofing
  * dos - low priority flooding
* OBD-II readers and writers
  * many cables and devices while allow read/write access to the OBD-II port in a car. However these are limited in scope to OBD-II, and cannot be used to prototype attacks for more general systems.
* Standard Linux Software tools
  * There are linux software programs for reading and writing from the CAN bus but these require existing cables or connection devices, written in c, complicated platform for some engineers to understand or use. While someone extremely well versed in the CAN protocol, c programming, and linux untilities may be able to achieve similar results, most people aren't willing to go through that much effort as security is often an afterthought. The goal of this framework is to try and counteract this.

# 3. Technical Approach

## System Design

Three components

* CAN spy device
  * can tap into canh and canl lines, requires controller and transciever (more on that later)
  * wireless enabled to connect to C2 server on a laptop via websockets
  * reads and writes to bus and relays/listens via websockets
* C2 server running on laptop
  * Receives streamed CAN traffic from device
  * Sends commands to write to the bus via the device
  * running python with websocket based architecture
  * Uses python library
* Testbed for developing hardware and testing attacks, composed of Two mock ECUS
  * Sensor: IMU + MCU
    * relays accelerometer data across the CAN bus
  * Actuator: Servo + MCU
    * reads sensor data from CAN bus
    * matches Servo angle with accelerometer angle so servo and IMU are at proportional angles
  * Meant to mimic a real CAN system


![System Architecture](media/system-architecture.png)
*High level system architecture diagram*

## Hardware

* Mixture of 3.3v and 5v pieces
* Level shifters, 12v to 5v power supply and regulator to power entire system

![Hardware Schematic](media/hardware-schematic.png)
*Mid-detail hardware schematic*

![MCU to CAN interface](media/mcu-can-interface.png)
*Note: the nano operates at 3.3v and thus requires a 3.3V-5V level shifter (TXS0108E) to communicate with the MCP2515 controller*


![MCU Connections](media/mcu-connections.png)
*Detailed connection diagram for the various microcontrollers and CAN controller/transceiver chips*

### CAN Requirements

* Raw can is just two lines, canh and canl
* highly specialized, and runs at a pretty fast clock speed, requires specialized hardware to actually connect to the bus for reliable packet transfer
* CAN controller - eg. MCP2515
  * Convers packets into bitstream and vice-versa
  * Buffers bits into packet which is sent to mcu (via hardware interrupt in case of MCP2515)
  * Few micocontrollers have these built in
* CAN transceiver - eg. TJA1050
  * Actually reads and writes to `canh` and `canl`
  * Converts differential signals to logic levels and vice-versa
  * Acts as middle man between CAN controller and physical bus
  * Virtually no microcontrollers have these (except in extremely specialized cases eg. NXPLPC11C00)

### Test Bed

* Sensor
  * Arduino Nano 33 iot
  * 3.3V, built in IMU (LSM6DS3)
  * requires level shifter
  * MCP2515 + TJA1050 board (5v)
  * Simply reads ay from IMU, normalizes the value as an integer, and sends it across the wire every `10ms`
* Actuator
  * Arduino Uno (5v)
  * MCP2515 + TJA1050 board
  * Listens for IMU data from specific id
  * Writes to servo [SG90](http://www.ee.ic.ac.uk/pcheung/teaching/DE1_EE/stores/sg90_datasheet.pdf)
  * Matches servo position based on the reading every `1ms`

### CAN-spy

* Arduino Nano 33 iot
  * Operates at 3.3v
  * Supports Wifi and Bluetooth
  * 3.3v --> 5v level shifter + MCP2515 + TJA1050 board
  * Manages websocket connection with C2 Server
  * Listens for CAN traffic, buffers it, batch streams it to C2 server every `25ms`
  * Listens for incoming commands from C2 server websocket, and writes to bus based on those commands

You can view the full system hardware schematic [here](media/hardware-schematic.pdf).

![Hardware Setup with Multimeter](media/hardware-setup/multimeter.jpg)
*The entire testbed operates at ~115 milliamps*

![Hardware Setup](media/hardware-setup/standard.jpg)

![Hardware Setup top down view](media/hardware-setup/top-down.jpg)
*Top down view*

## Software

### Firmware

#### Can Driver
* Reads and writes using arduino-CAN library

#### Websocket Management

* Most widely supported streaming technology
* streaming a new websocket message each tick is too slow
* instead we queue up to 1000 CAN messages at a time and then broadcast every `25ms` in a single websocket message
  * simply calculates buffer size, puts each field of data into the buffer, appends data
* statically allocate these can packets for performance and to avoid heap fragementation given the high volume of messages

### Python Library

* Manages websocket connections asyncronously with [asyncio]() and [websockets]()
* Provides simple `CanPacket` abstraction which user can use.
* Reading and writing capabilities
  * Makes use of aforementioned serialization and deserialization scheme

![Software Diagram](media/software-diagram.png)
*Flowchart of data throughout software system*

You can view the full software diagram [here](media/software-diagram.pdf).

# 4. Evaluation and Results

* List of attacks along with demos

As previously stated, this project is not a traditional research project and thus the evaluation is more qualatative and will consist mostly of demos and example code.

The most powerful part of the python framework is that it allows you to arbitrarily read and write bytes to the CAN bus which means it is capable or remotely performing any protocol level attack without direct physical access to the CAN BUS. You can do so using the following python API.

```
api
```

While the read/write capability is probably the most useful for researchers developing custom attacks, some attacks are so common that I've written some out of the box command line programs to perform them automatically. These tools also act as a starting point for researchers as they can easily check their system for obvious or common vulnerabilities using these tools.

### Replay Attack

Probably the most common type of CAN attack is the replay attack. This attack consists of recording activity on the CAN bus, and then replaying it in some way. This allows you to control the behavior of the system by simulating previous activity. It's very common due to accessibility as it doesn't nessecarily require an understanding of the systems inner workings to perform.

Using our framework, you can perform replay attacks using a combination of two command line programs.

First you can use the `can-listen` program to record activity into a file. This file simply contains the serialized version of the list of `CanPacket`s.

```
can-listen -o example.can
```

Next you can use the `can-replay` program to read one of these files and write the recorded activity to the bus.

```
can-replay -f example.can
```

### Listening



### Dos Attack



# 5. Discussion and Conclusions

## Impact

* As previously mentioned, a lot of this technology already existed, but now it is unified in a modular and easy to use open source framework
* Anyone with basic python skills and high level knowledge about CAN can test attacks on their system to make it more secure
* Overall I spent a lot more time than expected on hardware design, but it paid off in the end
* Learned a lot about Embedded Systems design, espcially the hardware aspect

* Multiple can_spy devices?

## Future plans
* Potential for attack prototypes
* Potential for wireless attacks
* Future of wireless (eg. use lora instead of wifi)
* More configuration for scripts
* More hardware interfaces
  * Keep atleast 2 canh and canl lines exposed
  * Add OBD-II connector as this is the most common domain
* PCB for better form factor

Repl Environment for lower command and control latency. Used separate programs due to time constraints and for an MVP.

# 6. References

* CAN stuff, medium article for hardware, medium article for CAN intro, etc.
* https://www.totalphase.com/blog/2024/11/can-transceiver-vs-can-controller-what-are-differences-how-are-they-used/
