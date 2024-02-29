# Traffic Light Control System

This program is designed to control a traffic light system with pedestrian crossings. It is developed for a STM32 microcontroller and uses the STM32CubeIDE platform. The system includes functions for traffic light control, pedestrian button handling, and car presence detection. Below is a summary of its functionality:

## Initialization

The code initializes various peripherals such as GPIO, USART, SPI, and timers.

## Main Loop

The main function initializes the peripherals and then enters an infinite loop. Inside the loop, it continuously checks the state of pedestrian buttons and car presence to control the traffic lights accordingly.

## State Machine

The system operates based on a state machine with several states:

- `VerticalGreen`: Vertical road has green light.
- `VerticalWaiting`: Vertical road has red light, waiting for the horizontal road to clear.
- `HorizontalGreen`: Horizontal road has green light.
- `HorizontalWaiting`: Horizontal road has red light, waiting for the vertical road to clear.
- `Pedestrian1Waiting`: Pedestrian button 1 is pressed, waiting for the horizontal road to clear.
- `Pedestrian2Waiting`: Pedestrian button 2 is pressed, waiting for the vertical road to clear.

## Interrupt Handling

The code utilizes interrupt callbacks to handle events such as the pedestrian button being pressed.

## Shift Register Driver

The code includes functions to control shift registers for updating the physical lights of the traffic signals.

## Input Handling Functions

The code includes functions to handle pedestrian button presses and car presence detection.
