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
int startButtonpin = 6; // DIGITAL

// Output Cluster:
int activeLEDpin = 2;
int leftLEDpin = 4;
int rightLEDpin = 3;
const int PRESET_SENSITIVITY = 100;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

  // Initializing Motor Pins
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1,  OUTPUT);
  pinMode(motor2pin2, OUTPUT);

  // Initializing Motor Speed Pins
  pinMode(motor1speed,  OUTPUT); 
  pinMode(motor2speed, OUTPUT);

  // Initializing Digital Inputs
  pinMode(startButtonpin, INPUT);
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
  digitalWrite(activeLEDpin, LOW);
  int controlButtonStatus = LOW;
  int adjLightReadRight;
  int adjLightReadLeft;
  int adjPoten;
  while (true) {
    delay(300);
    controlButtonStatus = digitalRead(startButtonpin);
    if (controlButtonStatus == HIGH) {
      delay(1000);
      break;
    }
    
    adjPoten = map(analogRead(potentiometerpin), 0, 1023, 50, 500);
    adjLightReadRight = map((long) (analogRead(lightsensRIGHTpin) * (adjPoten / 100)), 0, 1023, 1, 100);
    adjLightReadLeft = map((long) (analogRead(lightsensLEFTpin) * (adjPoten / 100)), 0, 1023, 1, 100);

    Serial.print("poten: ");
    Serial.print(adjPoten);
    Serial.print("\n");
    Serial.print("right: ");
    Serial.print(adjLightReadRight);
    Serial.print("\n");
    Serial.print("left: ");
    Serial.print(adjLightReadLeft);
    Serial.print("\n");

    if (adjLightReadRight < PRESET_SENSITIVITY && adjLightReadLeft < PRESET_SENSITIVITY) { // 0 - 0
      digitalWrite(leftLEDpin, LOW);
      digitalWrite(rightLEDpin, LOW);
    } else if (adjLightReadRight < PRESET_SENSITIVITY && adjLightReadLeft >= PRESET_SENSITIVITY) { // 0 - 1
      digitalWrite(leftLEDpin, LOW);
      digitalWrite(rightLEDpin, HIGH);
    } else if (adjLightReadRight >= PRESET_SENSITIVITY && adjLightReadLeft < PRESET_SENSITIVITY) { // 1 - 0
      digitalWrite(leftLEDpin, HIGH);
      digitalWrite(rightLEDpin, LOW);
    } else { // 1 - 1
      digitalWrite(leftLEDpin, HIGH);
      digitalWrite(rightLEDpin, HIGH);
    }

    //digitalWrite();

  }
}

int sensitivityScale;
int lightRight;
int lightLeft;

void loop() {
  digitalWrite(activeLEDpin, HIGH);
  // put your main code here, to run repeatedly:

  //Controlling speed (0  = off and 255 = max speed):
  // analogWrite(motor1speed, 255); 
  // analogWrite(motor2speed, 255); 

  // Both Run the same direction set up like this. WARNING flipping motors will flip directions.
  // digitalWrite(motor1pin1,  HIGH);
  // digitalWrite(motor1pin2, LOW);

  // digitalWrite(motor2pin1,  HIGH);
  // digitalWrite(motor2pin2, LOW);

  delay(10);

  sensitivityScale = map(analogRead(potentiometerpin), 0, 1023, 50, 500);
  lightRight = map((long) (analogRead(lightsensRIGHTpin) * (sensitivityScale / 100)), 0, 1023, 1, 100);
  lightLeft = map((long) (analogRead(lightsensLEFTpin) * (sensitivityScale / 100)), 0, 1023, 1, 100);

  if (digitalRead(startButtonpin) == HIGH) {
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);

    config();
  }

  // Low means "light", and high means "dark"
  // if it's "light", then the value is about 10

  if (lightRight < PRESET_SENSITIVITY && lightLeft < PRESET_SENSITIVITY) { // 0 - 0
    adjustSpeed(100, 100, 1);
    digitalWrite(leftLEDpin, LOW);
    digitalWrite(rightLEDpin, LOW);
    Serial.print("0 0\n");
  } else if (lightRight < PRESET_SENSITIVITY && lightLeft >= PRESET_SENSITIVITY) { // 0 - 1
    adjustSpeed(100, 75, 1);
    digitalWrite(leftLEDpin, LOW);
    digitalWrite(rightLEDpin, HIGH);
    Serial.print("0 1\n");
  } else if (lightRight >= PRESET_SENSITIVITY && lightLeft < PRESET_SENSITIVITY) { // 1 - 0
    adjustSpeed(75, 100, 1);
    digitalWrite(leftLEDpin, HIGH);
    digitalWrite(rightLEDpin, LOW);
    Serial.print("1 0\n");
  } else { // 1 - 1
    adjustSpeed(0, 0, 1);
    digitalWrite(leftLEDpin, HIGH);
    digitalWrite(rightLEDpin, HIGH);
    Serial.print("1 1\n");
  }

}

void adjustSpeed(int left, int right, float mult) {
  analogWrite(motor1speed, map(right, 0, 100, 50, 255) * mult); 
  analogWrite(motor2speed, map(left, 0, 100, 50, 255) * mult); 
}
