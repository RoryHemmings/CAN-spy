# CAN Spy

A practical hardware device and software tool to wirelessly monitor and exploit Controller Area Network (CAN) systems. Strictly for educational and security research purposes.

# Motivation

The CAN protocol is ubiquitously used in automotive and vehicular technology, in fact it is [mandated by law](https://law.stackexchange.com/questions/1317/since-when-is-can-bus-mandatory-for-new-vehicles) that it must be used in automobiles. However, it was not developed with modern security practices in mind and thus is vulnerable to certain attacks. The goal of this project is to put these attacks into practice by building a device which can stream CAN BUS traffic to a third party for reconnaissance and receive commands from the third party to perform specific attacks like replay attacks, jamming, etc.  

I plan on doing two things
- building a demo CAN BUS setup which performs some simple tasks based on a sensor reading
- building the proposed device which can
	- listen to the CAN traffic on the demo setup
	- stream the results via bluetooth or a related technology
	- listen for commands over bluetooth to perform attacks
- building client software which can connect to the proposed device to visualize can traffic and send commands

This device can be used to for reverse engineering purposes and to prototype CAN exploits for specific vehicles, making it a powerful tool for security researchers.

# Engineering Plan

## Attacker
Any MCU which can support 1 or 2 can ports and bluetooth/wifi
- arduino nano 33 iot
- raspberry pi zero

Attacks
- DOS -> via error frame and high priority overloading
- Spoofing
- Replay / Command Injection
- Sniffing
- ID collision
- Sybil attack -> Impersonate several nodes at once
- Checksum spoofing somehow

## Demo Setup

### Requirements
- Reads sensor data from an accelerometer and moves a motor which will match the rotation of the sensor's x axis rotation
- arduino 33 iot for sensor
- some other arduino to actuate the motor(s)
- servos for motors
- ~~CAN cable~~ replicate CAN with serial/GPIO ports

### Materials
## Client Software
- python repl environment
- buffers CAN traffic and can save to view in wireshark format

# Challenges
- requires reverse engineering CAN packets which requires a deep understanding of the CAN protocol
	- Arduino CAN library does exist
- CAN is faster than bluetooth
- What do we do about CAN ports?
	- I think we can just use regular serial/GPIO ports since can is just two wires with high and low

# CAN overview

- like ethernet but for sensors to communicate
- Allows many devices to communicate on a single bus - greatly simplifies wiring + communication setup
- id attached to messages
	- lower id -> higher priority
	- when messages collide, the higher priority message wins (priority based arbitration)
- messages consist of multiple parts
	- arbitration id (id)
	- data length code - length of data
	- data field - actual data
	- crc - checksum to guarantee validity
	- end of frame marker
