# CAN Spy

A practical hardware device and software tool for wirelessly monitoring and exploiting Controller Area Network (CAN) systems. Strictly for educational and security research purposes. This project was developed as the course project for UCLA ECE 202A Embedded Systems taken Fall 2024 with professor Mani Srivastava.

# Abstract

The CAN protocol is ubiquitously used in automotive and vehicular technology, in fact it is [mandated by law](https://law.stackexchange.com/questions/1317/since-when-is-can-bus-mandatory-for-new-vehicles) that it must be used in automobiles. However, it was not developed with modern security practices in mind and thus is vulnerable to a whole host of attacks. The goal of this project is to put these attacks into practice by building a device which can wirelessly stream CAN BUS traffic to a third party for reconnaissance and receive commands to perform specific attacks such as replay attacks, jamming, etc.  

I plan on doing three things
- building a demo CAN BUS setup which matches the rotation of a servo motor with the rotation of an accelerometer.
- building the proposed device which will be able to
	- listen to the CAN traffic on the demo setup
	- wirelessly stream the results via bluetooth in a format analyzable by wireshark
	- wirelessly listen for commands to perform attacks
- building client software which can connect to the proposed device and allow a deep level of customization for users by providing a python library allowing them to write specific attacks

This device can be used to for reverse engineering purposes and to prototype CAN exploits for specific vehicles, making it a powerful tool for security researchers.

# Team

- Rory Hemmings

# Links

- [Proposal](proposal)
- [Midterm Checkpoint Presentation Slides](https://docs.google.com/presentation/d/1n-uSZGRHBY1_UirP5JK-bZleSnxEKyPHQbpkVeXoZRA/edit?usp=sharing)
- [Final Presentation Slides](https://docs.google.com/presentation/d/1_Wyi8IJNu2JlXjtLVpOUpCHX8CuP_i6esAKe-fJg5Rk/edit?usp=sharing)
- [Final Report](report)
- [RFC](rfc)
