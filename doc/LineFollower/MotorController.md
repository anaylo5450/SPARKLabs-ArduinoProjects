Accurate as of ```2024-04-12 11:45:00```

- [Constants and Pins](#constants-and-pins)
    - [Motor Pins](#motor-pins)
    - [Input Pins](#input-pins)
    - [Output Pins](#output-pins)
    - [SENSOR\_CONSTANTS](#sensor_constants)
    - [CONTROL\_CONSTANTS](#control_constants)
- [Function Reference](#function-reference)
    - [setup()](#setup)
    - [config()](#config)
    - [loop()](#loop)
    - [sensorLogic(int, int, bool)](#sensorlogicint-int-bool)
    - [sensorLights()](#sensorlights)

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

```BUTTON_THRESHOLD```: voltage threshold for the button, as it is mapped to an analog pin. Why? See: [```startButtonpin```](#input-pins)

### CONTROL_CONSTANTS

```SPEED_MULTIPLIER```: float value for controlling the speed of the robot. Can go above 1.0, if your motors are capable. Do not set below 0.0.

```TURN_MULTIPLIER```: float value for controlling how powerful the turning actions are. Can go above 1.0, if your motors are capable. Do not set below 0.0.

```REVERSE_SPEED```: magic number setting the default reverse speed value

```TURN_SPEED```: magic number setting the default value on the turning motor

```FORWARDS_SPEED```: magic number setting the default forwards speed value

```STOP```: -1, which when passed to the ```adjustSpeed()``` function, will kill movement

# Function Reference

*The logic computations, initialization calls, and the FSM.*

### setup()

Like most arduino programs, this function sets pin directions & initializes communication over the serial port. Where this program differs is that the [config()](#config) function activates immediately before heading into [loop()](#loop).

### config()

Like a normal setup function, but on a loop. Within that loop, the robot is considered to be 'idling.' While idling, you can adjust the sensitivity of the IR sensors with the onboard potentiometer and see the sensor output through the blue sensor LEDs without the robot moving. This function exits when the state button is pressed.

Addtionally, this function controls the green LED representing the program's activity state.

### loop()

Take a normal loop function, and add a ton more encapsulation. That's what this program's loop is. In this function, the robot is considered to be 'active.' There is a tiny reading delay
to make the robot less jerky and the there is logic to compute whether the IR sensors are making a detection or not. Finally, there is a conditional for the 5v button that can send the robot back to 'idling.'

To elaborate on the IR sensor computation: map functions change the incoming IR value into one that is more palatable for the proceeding functions. `lightRight` & `lightLeft` are those values; pass those to [`sensorLogic(int, int bool)`](#sensorlogicint-int-bool) and not the raw values.

### sensorLogic(int, int, bool)

In order, takes a value for the left IR sensor's adjusted value, the right IR sensor's adjusted value, and the boolean constant, [`flipped`](#sensor_constants).

This function is the main controller for the Finite State Machine. Upon making a decision based on the adjusted IR sensor values and the [`PRESET_SENSITIVITY`](#sensor_constants) constant, it changes the global variable [`SENSOR_STATE`](#sensor_constants) to make decisions about where to send the robots. After making a decision, the function returns.

If a decision fails to be made, [`SENSOR_STATE`](#sensor_constants) is set to 0.

### sensorLights()


