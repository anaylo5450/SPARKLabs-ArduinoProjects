// Motor Cluster: pins 8-13
int motor1pin1 = 8; // OUT3 Positive
int motor1pin2 = 11; // OUT4 Negative
int motor1speed = 9; // ENB pin
int motor2pin1 = 12; // OUT2 Positve
int motor2pin2 = 13; // OUT1 Negative
int motor2speed = 10; //ENA pin

// Input Cluster: 
int lightsensRIGHTpin = A3; // ANALOG
int lightsensLEFTpin = A2; // ANALOG
int potentiometerpin = A1; // ANALOG
int startButtonpin = A0; // ANALOG

// Output Cluster:
int activeLEDpin = 2;
int leftLEDpin = 4;
int rightLEDpin = 3;

// Sensor vars
int SENSOR_STATE = 0;
bool flipped = true;

// Sensor Constants
const int PRESET_SENSITIVITY = 100;
const int BUTTON_THRESHOLD = 750;

// Drive Constants
const float SPEED_MULTIPLIER = .2;
const float TURN_MULTIPLIER = .5;

const int REVERSE_SPEED = -90;
const int TURN_SPEED = 75;
const int FORWARDS_SPEED = 100;
const int STOP = -1;

void setup() {
  Serial.begin(12000);
  // put your setup code here, to run once:

  // Initializing Motor Pins
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1,  OUTPUT);
  pinMode(motor2pin2, OUTPUT);

  // Initializing Motor Speed Pins
  pinMode(motor1speed,  OUTPUT); 
  pinMode(motor2speed, OUTPUT);

  // THIS BASTARD BUTTON
  pinMode(startButtonpin, INPUT);
  analogWrite(startButtonpin, 0);

  // Initializing Digital Inputs
  pinMode(potentiometerpin, INPUT);
  pinMode(lightsensRIGHTpin, INPUT);
  pinMode(lightsensLEFTpin, INPUT);

  // Initializing Digital Outputs
  pinMode(activeLEDpin, OUTPUT);
  pinMode(leftLEDpin, OUTPUT);
  pinMode(rightLEDpin, OUTPUT);

  // While loop to hold before button press
  config();
}

void config() {
  Serial.println("Entering Config");
  
  digitalWrite(activeLEDpin, LOW);
  //analogWrite(startButtonpin, 0);
  
  int adjLightReadRight;
  int adjLightReadLeft;
  int adjPoten;
  while (true) {
    delay(250);

    if (analogRead(startButtonpin) > BUTTON_THRESHOLD) {
      digitalWrite(activeLEDpin, HIGH);
      delay(1000);
      Serial.println("Start Robot");
      Serial.println(analogRead(startButtonpin));
      analogWrite(startButtonpin, 0);
      break;
    }
    
    adjPoten = map(analogRead(potentiometerpin), 0, 1023, 50, 500);
    adjLightReadRight = map((long) (analogRead(lightsensRIGHTpin) * (adjPoten / 100)), 0, 1023, 1, 100);
    adjLightReadLeft = map((long) (analogRead(lightsensLEFTpin) * (adjPoten / 100)), 0, 1023, 1, 100);

    Serial.print("poten: ");
    Serial.println(adjPoten);
    Serial.print("right: ");
    Serial.println(adjLightReadRight);
    Serial.print("left: ");
    Serial.println(adjLightReadLeft);
    Serial.print("THE BUTTON :: ");
    Serial.println(analogRead(startButtonpin));
    Serial.print("Sensor State :: ");
    Serial.println(SENSOR_STATE);

    sensorLogic(adjLightReadLeft, adjLightReadRight, flipped);
    sensorLights();
  }
}

int sensitivityScale;
int lightRight;
int lightLeft;

void loop() {
  digitalWrite(activeLEDpin, HIGH);
  //delay(10);

  sensitivityScale = map(analogRead(potentiometerpin), 0, 1023, 50, 500);
  lightRight = map((long) (analogRead(lightsensRIGHTpin) * (sensitivityScale / 100)), 0, 1023, 1, 100);
  lightLeft = map((long) (analogRead(lightsensLEFTpin) * (sensitivityScale / 100)), 0, 1023, 1, 100);

  sensorLogic(lightLeft, lightRight, flipped);
  sensorLights();
  motorDrive();

  if (analogRead(startButtonpin) > BUTTON_THRESHOLD) {
    setMotorDrive(LOW, LOW);

    Serial.print(analogRead(startButtonpin));
    Serial.println(" Big Red Button");
    analogWrite(startButtonpin, 0);

    delay(1000);
    config();
  }
}

void sensorLogic(int lightLeft, int lightRight, bool flipped) {
  if (flipped == false) {
    if (lightLeft < PRESET_SENSITIVITY && lightRight < PRESET_SENSITIVITY) {
      SENSOR_STATE = 1;
      return;
    } else if (lightLeft >= PRESET_SENSITIVITY && lightRight < PRESET_SENSITIVITY) {
      SENSOR_STATE = 2;
      return;
    } else if (lightLeft < PRESET_SENSITIVITY && lightRight >= PRESET_SENSITIVITY) {
      SENSOR_STATE = 3;
      return;
    }
  } else {
    if (lightLeft > PRESET_SENSITIVITY && lightRight > PRESET_SENSITIVITY) {
      SENSOR_STATE = 1;
      return;
    } else if (lightLeft <= PRESET_SENSITIVITY && lightRight > PRESET_SENSITIVITY) {
      SENSOR_STATE = 2;
      return;
    } else if (lightLeft > PRESET_SENSITIVITY && lightRight <= PRESET_SENSITIVITY) {
      SENSOR_STATE = 3;
      return;
    }
  }
  SENSOR_STATE = 0;
}

/* sensorLights - Sets light on when the corrosponding sensor sees tape
*/
void sensorLights() {
  switch (SENSOR_STATE) {
    case 0: // BOTH
      digitalWrite(leftLEDpin, HIGH);
      digitalWrite(rightLEDpin, HIGH);
      break;
    case 1: // NONE
      digitalWrite(leftLEDpin, LOW);
      digitalWrite(rightLEDpin, LOW);
      break;
    case 2: // LEFT
      digitalWrite(leftLEDpin, HIGH);
      digitalWrite(rightLEDpin, LOW);
      break;
    case 3: // RIGHT
      digitalWrite(leftLEDpin, LOW);
      digitalWrite(rightLEDpin, HIGH);
      break;
  } 
}

/* motorDrive - Logic behind motor drive
*/
void motorDrive() {
  switch (SENSOR_STATE) {
    case 0: // BOTH
      break;
    case 1: // NONE
      setMotorDrive(HIGH, HIGH);
      adjustSpeed(FORWARDS_SPEED, FORWARDS_SPEED, SPEED_MULTIPLIER);
      //delay(100);
      break;
    case 2: // LEFT
      setMotorDrive(HIGH, LOW);
      adjustSpeed(REVERSE_SPEED, TURN_SPEED, TURN_MULTIPLIER);
      //delay(10);
      break;
    case 3: // RIGHT
      setMotorDrive(LOW, HIGH);
      adjustSpeed(TURN_SPEED, REVERSE_SPEED, TURN_MULTIPLIER);
      //delay(10);
      break;
  } 
}

/* setMotorDrive - Sets motor drive direction
/  Inputs - int left (HIGH/LOW), int right (HIGH/LOW)
/  HIGH sets motor forwards, LOW sets motor backwards.
*/
void setMotorDrive(int left, int right) {
  if (left == LOW && right == LOW) {
    digitalWrite(motor2pin1, left);
    digitalWrite(motor2pin2, left);

    digitalWrite(motor1pin1, right);
    digitalWrite(motor1pin2, right);
  } else {
    digitalWrite(motor2pin1, left);
    digitalWrite(motor2pin2, !left);

    digitalWrite(motor1pin1, right);
    digitalWrite(motor1pin2, !right);
  }
}

// Accepts percentage values from 0-100 & a float as a speed multiplier.
// As of 3/29/24, also accepts negative values; they make the motors shut off for that condition.
void adjustSpeed(int left, int right, float mult) {
  if (right > 0 && left > 0) {
    analogWrite(motor1speed, map(right * mult, 0, 100, 50, 255)); 
    analogWrite(motor2speed, map(left * mult, 0, 100, 50, 255));
    return;
  } else if (right < 0 && left > 0) {
    analogWrite(motor1speed, map(-right * mult, 0, 100, 50, 255));
    Serial.println(map(-right * mult, 0, 100, 50, 255));
    analogWrite(motor2speed, map(left * mult, 0, 100, 50, 255));
    Serial.println(map(left * mult, 0, 100, 50, 255));
    return;
  } else if (right > 0 && left < 0) {
    analogWrite(motor1speed, map(right * mult, 0, 100, 50, 255)); 
    analogWrite(motor2speed, map(-left * mult, 0, 100, 50, 255)); 
    return;
  }

  // Negative number case is shutting the motors off.
  setMotorDrive(LOW, LOW);
}