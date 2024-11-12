# Project Proposal

## 1. Motivation & Objective

The CAN protocol is ubiquitously used in automotive and vehicular technology, in fact it is [mandated by law](https://law.stackexchange.com/questions/1317/since-when-is-can-bus-mandatory-for-new-vehicles) that it must be used in automobiles. However, it was not developed with modern security practices in mind and thus is vulnerable to a whole host of attacks. The goal of this project is to put these attacks into practice by building a device which can wirelessly stream CAN BUS traffic to a third party for reconnaissance and receive commands to perform specific attacks such as replay attacks, jamming, etc.  

My overall objectives are as follows.
1. Building a demo CAN BUS setup which matches the rotation of a servo motor with the rotation of an accelerometer.
2. Building a device with the ability to
	- listen to CAN traffic on the demo setup
	- wirelessly stream the traffic via bluetooth such that it can be examined via wireshark
	- wirelessly listen for commands and write to the CAN BUS
3. Building a software client which can connect to the proposed device and allow a deep level of customization for users by providing a python library allowing them to write new attacks.

Overall this project can be used to for reverse engineering purposes and to prototype CAN exploits, making it a powerful tool for security researchers.

## 2. State of the Art & Its Limitations

<!-- How is it done today, and what are the limits of current practice? -->

Currently, there exist hardware and software solutions that allows you to perform CAN BUS attacks. However, these suffer from several drawbacks.

First, there are very few platforms which fully integrate the entire attack chain. For example, there are hardware devices which listen to CAN packets, but don't write attacks, and there are software platforms which perform attacks, but only for specific hardware systems. Additionally, most of these platforms focus on development and debugging as opposed to versatility of attacks.

Regarding hardware, most platforms are car specific and rely on OBD connectors to deliver the payload. Similarly, they require direct physical access between the car and a computer. Software solutions suffer from the same drawbacks as hardware while additionally being often monolithic and limited in scope.

Finally, none of existing platforms make effective use of wireless streaming to allow for more flexible exploit development and new potential attacks.

## 3. Novelty & Rationale

<!-- What is new in your approach and why do you think it will be successful? -->

My approach has several novelties. Primarily, it aims to be as broad and modular as possible. Ideally, a user could use my platform to prototype and perform attacks on an CAN system in any environment.

Additionally, my project will aggregate many existing attacks into one place, spreading wider awareness of each attack, and helping researchers harden their system against many different types of attacks. If time permits, I will also attempt to implement theoretical voltage level attacks which have not yet been put into practice. This could help prove the existence of new attacks for researchers to defend against.

Finally, my approach essentially combines the logic analyzer and attacking device into one unit and then makes it wirelessly accessible for maximum flexibility.

## 4. Potential Impact

<!-- If the project is successful, what difference will it make, both technically and broadly? -->

Overall, my project will serve as a useful framework for security researchers by providing a flexible tool and repository of attacks, lowering the barrier of entry into CAN security. Given how universal the protocol is, this will help secure cars, boats, planes, and other critical infrastructure. If time permits, it could also prove or disprove the existence of certain theoretical attacks.

## 5. Challenges

<!-- What are the challenges and risks? -->

The main challenge is the amount of work required to implement the required features. Making such a flexible system essentially requires a new implementation of the CAN protocol so that users can create any frame they want, and wirelessly stream it into their target CAN BUS. The risk is that I might not have the time to finish the entire implementation.

Another risk is that it could bbe hard to make some of the mentioned attacks actually work.

Given the number of components in the system, there are also many points of failure meaning debugging could be difficult.

## 6. Requirements for Success

<!-- What skills and resources are necessary to perform the project? -->

The overall requirements are 

Skills
- Learning more about CAN
- Reading papers and understanding attacks
- Trial and error to create attacks
- Functioning testbed so I can create the attacks

Resources
- Papers with proposed attacks
- Servo Motor, arduino to control it, and arduino with an IMU onboard

## 7. Metrics of Success

<!-- What are metrics by which you would check for success? -->

Since my project is less research based, there aren't a ton of numerical metrics to measure success other than the number of attacks that can be performed reliably. Qualitatively, the main factor which checks for success in this case is whether I can have a working demo which satisfies all the following features:

- Working Demo CAN system
- Can wirelessly perform attacks on the demo setup using python client
- Can wirelessly analyze packets from demo setup in wireshark
- Can access all attacks and client functions using provided library

## 8. Execution Plan

<!-- Describe the key tasks in executing your project, and in case of team project describe how will you partition the tasks. -->

My project is composed of three components:

1. Demo Setup
2. Hardware for malicious device
3. Software repl and library

I have no other teammates and thus no partitioning is required.

In order to create the demo setup, I will need:
- A servo motor
- An arduino board which can control the servo while listening on CAN
- Another arduino board with an IMU which will serve as the controller for the servo
- two MPC 2515 CAN modules to actually write to the CAN bus

With these materials, I will create a CAN network in which the angle of the servo matches the orientation of the IMU. Every 20 milliseconds or so, a measurement will be taken from the IMU and a corresponding CAN message will be sent requesting for the angle to be matched.

In order to create the malicious device, I simply need a 3.3v arduino with wirelss capabilities. Luckily I already have an arduino nano 33 iot which is both wifi and bluetooth enabled. The firmware for this device will pretty much act as a bridge between the CAN BUS and the software tools meaning that it needs to be able to manage it's wifi connection and CAN bus connection simultaneously.

Finally, the software library will include a light implementation of the CAN protocol. And manage both writing and reading from the hardware device using wifi.

## 9. Related Work

### 9.a. Papers

<!-- List the key papers that you have identified relating to your project idea, and describe how they related to your project. Provide references (with full citation in the References section below). -->

The paper entitled "CAN Bus Security" authored by Dr. Ken Tindell serves as an overall guide to CAN bus attacks, and will be helpful in implementing established attacks as well as gaining a deep understanding of the CAN protocol. Similary, his framework Can Hack is pretty much the state of the art regarding CAN exploits, and thus I can take inspiration from certain parts of it for my implementation.

The paper "WeepingCAN: A stealthy CAN Bus-off Attack" provides more information on CAN hacking methods, which I may be able to implement if time permits.

### 9.b. Datasets

<!-- List datasets that you have identified and plan to use. Provide references (with full citation in the References section below). -->

No datasets are required for this project.

### 9.c. Software

<!-- List software that you have identified and plan to use. Provide references (with full citation in the References section below). -->

I will make use of several arduino libraries, namely

- `Arduino_CAN` which will allow me to actually write to the CAN BUS more easily. 
- `wifinina` to connect the arduino to wifi

Other than that, everything else will be built from scratch.

## 10. References

<!-- List references correspondign to citations in your text above. For papers please include full citation and URL. For datasets and software include name and URL. -->

1. Tindell K, (2020). "CAN Security: CAN Protocol Security White Paper". https://www.canislabs.com/downloads/2020-02-14-White-Paper-CAN-Security.pdf
2. Bloom B. (2021). "WeepingCAN: A stealthy CAN Bus-off Attack". https://gedare.github.io/pdf/bloom_weepingcan_2021.pdf
3. https://docs.arduino.cc/learn/communication/can/
4. https://github.com/arduino/ArduinoCore-renesas/tree/main/libraries/Arduino_CAN
5. https://reference.arduino.cc/reference/en/libraries/wifinina/ 