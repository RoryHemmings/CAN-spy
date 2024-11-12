# Engineering Plan

## Hardware
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
- voltage level attacks

## Software
- python repl environment
- buffers CAN traffic and can save to view in wireshark format

## Demo Setup

### Requirements
- Reads sensor data from an accelerometer and moves a motor which will match the rotation of the sensor's x axis rotation
- arduino 33 iot for sensor
- some other arduino to actuate the motor(s)
- servos for motors
- ~~CAN cable~~ replicate CAN with serial/GPIO ports

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

# Attacks
- Frame attacks 
	- spoofing - device pretends to be another one
	- 
- Protocol Attacks
	- attack defined behavior of protocol itsself
	- malware hijacking device and controlling can pins

- defense techniques
  - intrusion detection
  - encryption
  - security gateways
  - update can hardware

# References
- https://gedare.github.io/pdf/bloom_weepingcan_2021.pdf
- https://www.canislabs.com/downloads/2020-02-14-White-Paper-CAN-Security.pdf
- https://medium.com/@yogeshojha/car-hacking-101-practical-guide-to-exploiting-can-bus-using-instrument-cluster-simulator-part-i-cd88d3eb4a53
- https://github.com/arduino/ArduinoCore-renesas/tree/main/libraries/Arduino_CAN
- https://reference.arduino.cc/reference/en/libraries/wifinina/