# Embedded Avionics Simulator

A modular embedded C simulation of avionics communication and control software.

## System Architecture

```mermaid
flowchart LR
    S[Altitude Sensor<br/>Sensor Simulator] -->|ARINC429 Message| A[ARINC429 Driver]
    A --> B[System Message Bus]
    B --> F[Flight Control Unit<br/>PID Controller]
    B --> M[Monitoring Module]
    F -->|CAN Message| C[CAN Bus]
    C --> D[Actuator Driver]
    C --> T[Telemetry Node]
    D --> E[Elevator Actuator]
```
## Features

- ARINC429 encoding and decoding

- parity validation

- CAN bus queue simulation

- PID-based altitude hold controller

- modular subsystem structure

- unit tests

## Build

This project can be compiled with a C compiler or Visual Studio C++ compiler.

## Tests

Includes tests for:

- ARINC429

- CAN bus

- flight control