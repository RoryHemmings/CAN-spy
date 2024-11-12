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

Currently, there exist devices 

Software Solutions
    - too specific
    - not modular or customizable
    - too car focused (OBD to USB etc.)
    - mostly focused on reading CAN data, not a lot on attacks

Network solutions are bad

Voltage Level attacks are impossible

## 3. Novelty & Rationale

<!-- What is new in your approach and why do you think it will be successful? -->

- broader and more modular
- tries new attacks
- wireless monitoring

## 4. Potential Impact

<!-- If the project is successful, what difference will it make, both technically and broadly? -->

Techincally:
- provides central repository of CAN attacks allowing researchers to harden their systems against many attacks
- could prove practicality of new attacks

Broadly:
- will be useful framework for security researchers
- could improve security of critical systems like cars, boats, infrastructure, etc.

## 5. Challenges

<!-- What are the challenges and risks? -->

Risks:
- not getting attacks to work

## 6. Requirements for Success

<!-- What skills and resources are necessary to perform the project? -->

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

- Number of attacks that can be performed reliably
- Number of theoretical attacks that I can put into practice

## 8. Execution Plan

<!-- Describe the key tasks in executing your project, and in case of team project describe how will you partition the tasks. -->

1. Demo Setup
2. Hardware for malicious device
3. Software repl and library

I am alone, so I have to do all tasks myself.

## 9. Related Work

### 9.a. Papers

<!-- List the key papers that you have identified relating to your project idea, and describe how they related to your project. Provide references (with full citation in the References section below). -->

### 9.b. Datasets

<!-- List datasets that you have identified and plan to use. Provide references (with full citation in the References section below). -->

### 9.c. Software

<!-- List software that you have identified and plan to use. Provide references (with full citation in the References section below). -->

## 10. References

<!-- List references correspondign to citations in your text above. For papers please include full citation and URL. For datasets and software include name and URL. -->

1. [Intro to Can Hacking]()
2. [Can-Utils](https://github.com/linux-can/can-utils)
