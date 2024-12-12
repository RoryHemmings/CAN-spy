# Report

# Table of Contents
* Abstract
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

* Motivation & Objective: What are you trying to do and why? (plain English without jargon)
* State of the Art & Its Limitations: How is it done today, and what are the limits of current practice?
* Novelty & Rationale: What is new in your approach and why do you think it will be successful?
    * None of the pieces introduce new technology, the unification of the pieces and wireless python framework are the novelty
* Potential Impact: If the project is successful, what difference will it make, both technically and broadly?
* Challenges: What are the challenges and risks?
* Requirements for Success: What skills and resources are necessary to perform the project?
* Metrics of Success: What are metrics by which you would check for success?

The Controller Area Network (CAN) protocol is an extremely common serial communication protocol used frequently in large complex systems like automobiles, ships, trains, planes, factories, and other industrial technologies. While robust and flexible, the standard is relatively old, and thus doesn't hold up under modern security standards. As a result, the standard is a serious weak point in critical infrastructure and thus in recent years it has drawn much attention from bad actors and security researchers alike. The goal of this project is to provide an extensible platform for developing CAN exploits for use by security researchers when protecting against these kinds of attacks.

Currently the state of the art in this area is composed of three parts: OBD readers, actual attacks, and software simulators. However, few projects have actually combined all three into a single extensible platform.

Novelty:
* wireless, open source framework
* CANH, CANL instead of OBD, more general than automobiles
* flexible python framework
* Although much of the backbone technology already exists, the goal of this project is to unify everything into an easy to use flexible platform for developing CAN exploits to make it as easy as possible for engineers to harden their systems.

Potential impact 
* Allows for prototyping of new exploits
  * Useful tool for researchers
  * Useful tool for engineers who want to harden their system (can test attacks)
* Spread awareness of common attacks and lowers barrier of entry to using them
  * Especially for engineers who might not be proficient in C programming and protocol inner-workings
* Overall, make critical infrastructure safer

Challenges:
* Hardware development (CAN is rather complicated)
* Whole system is rather complicated relative to the time constraints
* Handling high bit rates CAN supports (batch optimization for streaming)

Requirements for Success:
* Byte level read/write access to the CAN bus along with python library to use it
* Stock out of the box command line tools
* CAN testbed system to demo the attacks


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

* High Level view of entire system
* Purpose and rationale behind each piece

![System Architecture](media/system-architecture.png)
*High level system architecture diagram*

## Hardware

![Hardware Schematic](media/hardware-schematic.png)
*Mid-detail hardware schematic*

![MCU to CAN interface](media/mcu-can-interface.png)
*Note: the nano operates at 3.3v and thus requires a 3.3V-5V level shifter (TXS0108E) to communicate with the MCP2515 controller*


![MCU Connections](media/mcu-connections.png)
*Detailed connection diagram for the various microcontrollers and CAN controller/transciever chips*

### CAN-spy

### Demo

You can view the full system hardware schematic [here](../hardware/hardware-schematic.pdf).

![Hardware Setup with Multimeter](media/hardware-setup/multimeter.jpg)
*The entire testbed operates at ~115 milliamps*

![Hardware Setup](media/hardware-setup/standard.jpg)

![Hardware Setup top down view](media/hardware-setup/top-down.jpg)
*Top down view*

## Software

* Websockets

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

First you can use the `listen` program to record activity into a file. This file simply contains the serialized version of the list of `CanPacket`s.

```
listen -o example.can
```

Next you can use the `replay` program to read one of these files and write the recorded activity to the bus.

```
replay -f example.can
```

### Listening



### Dos Attack



# 5. Discussion and Conclusions

* Potential for attack prototypes
* Potential for wireless attacks
* Future of wireless (eg. use lora instead of wifi)

Repl Environment for less latency. Used separate programs due to time constraints and for an MVP.

# 6. References

* CAN stuff, medium article for hardware, medium article for CAN intro, etc.
