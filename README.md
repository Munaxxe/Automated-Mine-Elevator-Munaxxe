# Automated Mine Elevator System

An Arduino-based automated mine elevator prototype demonstrating the
integration of embedded systems, biometric access control, load
monitoring, floor detection and electromechanical control.

## Project Overview

The **Automated Mine Elevator System** is an embedded systems and
industrial automation project developed to demonstrate the controlled
transportation of personnel between multiple levels in a mine-shaft
environment.

The prototype uses an Arduino-based control system to coordinate user
identification, destination assignment, load measurement, floor
detection, motor operation and door movement. It brings together
sensors, actuators and a user interface in one integrated
electromechanical system.

The project is intended as an educational demonstration of automation
and control principles. It is not designed or certified for use as an
operational mine hoist.

## Project Demonstration

A video demonstration of the prototype will be made available here.

**[Watch the project demonstration](https://www.mediafire.com/file/knt19hpyx4gbfcj/automated-elevator-munaxxe.mp4/file)**

## Key Features

-   **Biometric user identification:** A fingerprint sensor verifies
    users against enrolled fingerprint templates.
-   **Destination assignment:** The control program associates
    recognised fingerprint IDs with configured destination levels.
-   **Load monitoring:** A load cell connected through an HX711
    amplifier measures the elevator load.
-   **Overload indication:** The program compares the measured load
    against a configured threshold of approximately 3 kg.
-   **Multi-level detection:** Infrared sensors provide floor-position
    inputs for the ground level and three underground levels.
-   **Motor control:** A motor driver interfaces the Arduino with the
    elevator's DC motor.
-   **Automated door operation:** A servo motor operates the elevator
    door.
-   **LCD interface:** A 20×4 I2C LCD displays system and operating
    information.
-   **Audible and visual feedback:** A buzzer and indicator LEDs
    communicate selected system states.

## System Architecture

The system can be understood as three functional sections.

### 1. Input

-   Fingerprint sensor --- user identification.
-   Load cell --- load measurement.
-   Infrared sensors --- floor detection.

### 2. Processing

-   Arduino microcontroller --- central control unit.
-   Embedded program --- processes sensor inputs and coordinates the
    operating sequence.

### 3. Output

-   DC motor and motor driver --- elevator movement.
-   Servo motor --- door operation.
-   20×4 I2C LCD --- system information.
-   Buzzer --- audible alerts.
-   Red and green LEDs --- visual status indication.

## Hardware Components

  Component                            Function
  ------------------------------------ ----------------------------------------------
  Arduino-compatible microcontroller   Main control unit
  Fingerprint sensor                   Biometric identification
  Load cell                            Load measurement
  HX711 module                         Load-cell signal conditioning and conversion
  Infrared sensors (4)                 Floor-level detection
  DC motor                             Elevator movement
  Motor driver                         DC motor interfacing and control
  Servo motor                          Door operation
  20×4 I2C LCD                         System status display
  Buzzer                               Audible feedback
  Red and green LEDs                   Visual status indication

Confirm the exact microcontroller board and component models against the
physical prototype before treating this list as a final bill of
materials.

## Software and Libraries

The control program is written in Arduino C/C++ and is provided in
`AutomatedElevatorCode.ino`.

The source code includes the following libraries:

  Library                    Purpose
  -------------------------- -------------------------------------
  `Wire.h`                   I2C communication
  `LiquidCrystal_I2C.h`      LCD interfacing
  `HX711.h`                  Load-cell interface
  `Adafruit_Fingerprint.h`   Fingerprint sensor communication
  `Servo.h`                  Servo motor control
  `SoftwareSerial.h`         Serial communication where required

Install the required libraries through the Arduino IDE Library Manager
before compiling the sketch.

## Operating Sequence

The intended operating sequence is:

1.  **Initialisation:** The microcontroller starts the display, sensor
    interfaces and output devices.
2.  **User identification:** The fingerprint sensor checks the presented
    fingerprint against its enrolled templates.
3.  **Destination selection:** The program uses the recognised user ID
    to determine the configured destination.
4.  **Load assessment:** The load-cell reading is checked against the
    programmed load threshold.
5.  **Door operation:** The servo motor operates the elevator door
    according to the programmed sequence.
6.  **Movement control:** The motor driver controls the DC motor.
7.  **Floor detection:** Infrared sensors provide position feedback as
    the elevator moves between levels.
8.  **Status feedback:** The LCD, LEDs and buzzer communicate relevant
    system states.

The exact behaviour and safety response depend on the implementation in
the Arduino sketch and the connected hardware.

## Repository Structure

``` text
Automated-Mine-Elevator-Munaxxe/
├── AutomatedElevatorCode.ino
├── automated-hoist-munaxxe.png
└── README.md
```

## Getting Started

### Requirements

-   Arduino IDE
-   Compatible Arduino development board
-   Hardware components listed above
-   Required Arduino libraries
-   USB cable and suitable power arrangements for development and
    testing

### Installation

1.  Clone the repository:

    ``` bash
    git clone https://github.com/Munaxxe/Automated-Mine-Elevator-Munaxxe.git
    ```

2.  Open the project folder:

    ``` bash
    cd Automated-Mine-Elevator-Munaxxe
    ```

3.  Open `AutomatedElevatorCode.ino` in the Arduino IDE.

4.  Install the libraries listed in the [Software and
    Libraries](#software-and-libraries) section.

5.  Check the pin assignments in the sketch and connect the components
    accordingly.

6.  Select the correct board and serial port in the Arduino IDE.

7.  Compile the sketch and upload it to the board.

8.  Verify sensor readings and actuator operation under controlled,
    unloaded test conditions before conducting further prototype
    testing.

## Engineering Concepts

This project demonstrates the integration of:

-   Embedded systems programming
-   Industrial automation and control
-   Electromechanical system integration
-   Sensor interfacing and signal conditioning
-   Biometric identification
-   DC motor and servo control
-   Feedback and monitoring
-   Human-machine interface design

## Potential Future Improvements

-   Add independent, hardware-based safety interlocks.
-   Improve position measurement and stopping accuracy.
-   Implement controlled acceleration and deceleration.
-   Add fault detection and clearer fault-state reporting.
-   Improve load measurement calibration and overload response.
-   Introduce event logging and operational data monitoring.
-   Develop a graphical monitoring interface.
-   Document wiring diagrams, pin assignments and test results.

## Safety Disclaimer

This repository documents an educational prototype. It is **not a
certified mine hoisting or personnel-lifting system** and must not be
used to transport people or materials in an operational mine.

Real mine-hoisting equipment requires professional engineering design,
independently validated safety systems, appropriate braking and
emergency provisions, compliance with applicable regulations, and
rigorous inspection and testing. The prototype's sensors and programmed
logic must not be treated as substitutes for those safeguards.

## Author

**Munashe Walter Jojo**

Industrial Electronics Engineering \| Embedded Systems \| Industrial
Automation

-   GitHub: [@Munaxxe](https://github.com/Munaxxe)
-   Repository: [Automated Mine
    Elevator](https://github.com/Munaxxe/Automated-Mine-Elevator-Munaxxe)

------------------------------------------------------------------------

*An exploration of embedded electronics, automation and
electromechanical engineering for mining applications.*
