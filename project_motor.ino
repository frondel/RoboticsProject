#include <DualVNH5019MotorShield.h>

//library location /usr/share/arduino/libraries 

DualVNH5019MotorShield md;

const int trigPinL = 43; 
const int echoPinL = 45;

const int trigPinR = 51; 
const int echoPinR = 53;

const int trigPinC = 47; 
const int echoPinC = 49;

volatile float maxCenterDistance = 40.00;
volatile float centerDistance,leftDistance, rightDistance, distance,duration;
volatile float maxLeftDistance, maxRightDistance = 40.00;
int limit= 120;
void stopIfFault()
{
  if (md.getM1Fault())
  {
    Serial.println("M1 fault");
    while(1);
  }
  if (md.getM2Fault())
  {
    Serial.println("M2 fault");
    while(1);
  }
}

void setup(){
   Serial.begin(9600);
   
  pinMode(trigPinL, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinL, INPUT); // Sets the echoPin as an Input
  
  pinMode(trigPinR, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinR, INPUT); // Sets the echoPin as an Input
  
  pinMode(trigPinC, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinC, INPUT); // Sets the echoPin as an Input
  md.init();
}

void loop(){

    checkCenterDistance();
    checkLeftDistance();
    checkRightDistance();
    if(centerDistance < maxCenterDistance||rightDistance<maxRightDistance||
        leftDistance < maxLeftDistance){
        Serial.println("Too close");
      
        checkLeftDistance();
        delay(20);
        checkRightDistance();
        delay(20);
        
        if(leftDistance < rightDistance){
          turnRight();
          Serial.println("Turning Right...");
        }else if(leftDistance > rightDistance){
           turnLeft();
           Serial.println("Turning Left...");
        }
      }
    
      //else if(centerDistance <maxCenterDistance && rightDistance<maxRightDistance &&
        //  leftDistance < maxLeftDistance){
          //Serial.println("Reverse");
          //reverse();
      //}
      else{
          Serial.println("Center is ok");
          moveForward();
  }
  
  /*
  //check distance using left sensor
  checkLeftDistance();
  if(leftDistance < maxLeftDistance){
    Serial.println("Left too close");
    delay(20);
    checkLeftDistance();
    delay(20);
    checkRightDistance();
    delay(20);
    if(leftDistance > rightDistance)
      moveForward();
      else if(leftDistance < rightDistance){
        turnRight();
      }
  }
  
  //check distance using right sensor
  checkRightDistance();
  if(rightDistance < maxRightDistance){
    Serial.println("Right too close");
    delay(20);
    checkRightDistance();
    delay(20);
    checkLeftDistance();
    delay(20);
    if(leftDistance < rightDistance)
      moveForward();
    else if(leftDistance > rightDistance){
        turnLeft();
     }
  }
  
 */


  
}

void checkCenterDistance(){
    
  SonarSensor(trigPinC, echoPinC);
  centerDistance = distance;
  
}
cd 
void checkLeftDistance(){
  SonarSensor(trigPinL, echoPinL);
  leftDistance = distance;
}

void checkRightDistance(){
   
  SonarSensor(trigPinR, echoPinR);
  rightDistance = distance;
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


void moveForward(){
  for (int i = 0; i <= limit; i++)
  {
    //left wheel moves forward
    md.setM1Speed(-i);
    stopIfFault();
    
    //right wheel moves forward
     md.setM2Speed(i);
     stopIfFault();
    
    if (i%200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
      Serial.print("M2 current: ");
      Serial.println(md.getM2CurrentMilliamps());
    }
    delay(5);
  }
}

void reverse(){
 //loop to move the wheels backward
  for (int i = limit; i >= -limit; i--)
  {
    //left wheel moves backward
    md.setM1Speed(-i);
    stopIfFault();
    
    //right wheel moves backwards
    md.setM2Speed(i);
    stopIfFault();
     
    if (i%200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
      Serial.print("M2 current: ");
      Serial.println(md.getM2CurrentMilliamps());
    }
    delay(5);
  }
}

void turnLeft(){
  for (int i = 0; i <= limit; i++)
  {
    //left wheel moves forward
    md.setM1Speed(i);
    stopIfFault();
    
    md.setM2Speed(0);
    //stopIfFault();
    
    if (i%200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
      Serial.print("M2 current: ");
      Serial.println(md.getM2CurrentMilliamps());
    }
    delay(5);
  }
}
void turnRight(){
  for (int i = 0; i <= limit; i++)
  {
    
   //right wheel moves forward
     md.setM2Speed(i);
     stopIfFault();
    
    md.setM1Speed(0);
    //stopIfFault();
    
    if (i%200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
      Serial.print("M2 current: ");
      Serial.println(md.getM2CurrentMilliamps());
    }
    delay(5);
  }
}



