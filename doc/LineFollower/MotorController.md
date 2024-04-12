Accurate as of ```2024-04-12 10:50:00```

- [Constants and Pins](#constants-and-pins)
      - [The most important part of an Arduino's codebase.](#the-most-important-part-of-an-arduinos-codebase)
    - [Motor Pins](#motor-pins)
    - [Input Pins](#input-pins)
    - [Output Pins](#output-pins)
    - [SENSOR\_CONSTANTS](#sensor_constants)



# Constants and Pins
*The most important part of an Arduino's codebase.*

### Motor Pins

```motor1pin1```: OUT3 on motor controller, positive voltage for the left-side motor

```motor1pin2```: OUT4 on motor controller, negative voltage for the left-side motor

```motor1speed```: ENB on motor controller, PWM required for controlling the speed of the left-side motor

```motor2pin1```: OUT2 on motor controller, positive voltage for the right-side motor

```motor2pin2```: OUT1 on motor controller, negative voltage for the right-side motor

```motor2speed```: ENA on motor controller, PWM required for controlling the speed of the right-side motor

### Input Pins

```lightsensRIGHTpin```: reads an analog value from the IR sensor on the right side of the chassis

```lightsensLEFTpin```: reads an analog value from the IR sensor on the left side of the chassis

```potentiometerpin```: reads an analog value from the potentiometer; used for tuning the IR sensors

```startButtonpin```: reads an analog amount of voltage from a 5v button. NOTE: This pin can probably be digital, but it is analog in this project due to some issues with odd voltage

### Output Pins

```activeLEDpin```: corresponds to a green LED that displays the state of the robot (idling or active)

```leftLEDpin```: corresponds to a blue LED that displays whether the left IR sensor has or hasn't made a detection

```rightLEDpin```: corresponds to a blue LED that displays whether the right IR sensor has or hasn't made a detection

### SENSOR_CONSTANTS

```SENSOR_STATE```: first and foremost, this is not a constant, it is a variable that maps an integer to each possible combination of IR sensor readings

```flipped```: this actually IS a constant but is not reflected as such in the codebase; controls whether the program is following a relatively reflective line or a relatively absorptive line

```PRESET_SENSITIVITY```: value decided against for determining whether an IR sensor's value can change the program's state

```BUTTON_THRESHOLD
