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

The Controller Are Network (CAN) protocol is an extremely common serial communication protocol used frequently in large complex systems like automobiles, ships, trains, planes, factories, and other industrial technology. While robust and flexible, the standard is relatively old, and thus doesn't hold up under modern security standards. As a result, the standard is a serious weak point in critical infrastructure and thus in recent years it has drawn much attention from bad actors and security researchers alike. The goal of this project is to provide an extensible platform for developing CAN exploits for use by security researchers when protecting against these kinds of attacks.

# 2. Related Work

* existing CAN attacks in theory
* OBD-II readers and writers
* Standard Linux Software tools

# 3. Technical Approach

## System Design

* High Level view of entire system
* Purpose and rationale behind each piece

## Hardware

### CAN-spy

### Demo

## Software

* Websockets

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
