/*
* Ultrasonic Sensor HC-SR04 and Arduino Tutorial
 *
 * Crated by Dejan Nedelkovski,
 * www.HowToMechatronics.com
 *
 */
// defines pins numbers
//43 45 left sonar
//47 49 is for center sonar
//51 53 right sonar

const int trigPinL = 43; 
const int echoPinL = 45;

const int trigPinR = 51; 
const int echoPinR = 53;

const int trigPinC = 47; 
const int echoPinC = 49;

// defines variables
long duration, RightSensor, LeftSensor, CenterSensor;
int distance;

void setup() {
  pinMode(trigPinL, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinL, INPUT); // Sets the echoPin as an Input
  
  pinMode(trigPinR, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinR, INPUT); // Sets the echoPin as an Input
  
  pinMode(trigPinC, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinC, INPUT); // Sets the echoPin as an Input
  
  Serial.begin(9600); // Starts the serial communication
}
void loop() {
  
  SonarSensor(trigPinR, echoPinR);
  RightSensor = distance;
  
  SonarSensor(trigPinL, echoPinL);
  LeftSensor = distance;
  
  SonarSensor(trigPinC, echoPinC);
  CenterSensor = distance;

  // Prints the distance on the Serial Monitor
  Serial.print("Distance LEFT: ");
  Serial.println(LeftSensor);
  Serial.print("Distance RIGHT: ");
  Serial.println(RightSensor);
  Serial.print("Distance CENTER: ");
  Serial.println(CenterSensor);
  
  delay(1000);
}

void SonarSensor(int trigPin, int echoPin)
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance = duration*0.034/2;
}
