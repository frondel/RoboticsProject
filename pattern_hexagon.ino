#include <DualVNH5019MotorShield.h>

 // MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain
#include<Wire.h>
#include<math.h>
#define CONVERSION 3.9  // convert the raw readings to the g unit (1g = 9.8 m/s²) depends on sensor settings

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
int limit= 140;

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ,accelerationX,accelerationY,accelerationZ,pitch,roll,yaw;
int minVal = 265; 
int maxVal = 402;
double x,y,z;

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
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
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
    run();

}

void Angle(){
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
    AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
    AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    
        
    accelerationX = (int16_t)(AcX * CONVERSION);
    accelerationY = (int16_t)(AcY * CONVERSION);
    accelerationZ = (int16_t)(AcZ * CONVERSION);
    pitch = 180 * atan (accelerationX/sqrt(accelerationY*accelerationY + accelerationZ*accelerationZ))/M_PI;
    roll = 180 * atan (accelerationY/sqrt(accelerationX*accelerationX + accelerationZ*accelerationZ))/M_PI;
    yaw = 180 * atan (accelerationZ/sqrt(accelerationX*accelerationX + accelerationY*accelerationY))/M_PI;

    
    Serial.println();
    Serial.print("Angle X = "); Serial.print(pitch);
    Serial.print(" | Angle Y = "); Serial.print(roll);
    Serial.print(" | Angle Z = "); Serial.println(yaw);
    Serial.println();

}


void run(){
    Angle();
    checkCLoseObject();
    if(centerDistance < maxCenterDistance && rightDistance<maxRightDistance &&
          leftDistance < maxLeftDistance){
          reverse();
      }   
     moveRun(); 

}
void moveRun(){
    moveForward();
    moveForward();
    moveForward();
    moveForward();
    moveForward();
    turnLeft(); 
  
  
}

void checkCLoseObject(){
    checkCenterDistance();
    checkLeftDistance();
    checkRightDistance();
    
    if( (centerDistance < leftDistance) && (centerDistance < rightDistance) ){
         Serial.print("CLosest Object: ");Serial.println(centerDistance);
         
    }else if( (centerDistance > leftDistance) && (leftDistance < rightDistance)){
         Serial.println("CLosest Object: ");Serial.println(leftDistance);
    }
         
     else{
         Serial.println("CLosest Object: ");Serial.println(rightDistance);
     }
         
     checkWidthAndHight();

}

void checkWidthAndHight(){
  if(centerDistance < maxCenterDistance && leftDistance <maxCenterDistance && rightDistance < maxCenterDistance){
    
     Serial.println("Object hight&width is bigger than robot"); 
  }
  else if(centerDistance < maxCenterDistance && leftDistance <maxCenterDistance)
     Serial.println("Object hight&width is bigger than robot"); 
  else if(rightDistance < maxCenterDistance)
     Serial.println("Object hight&width is bigger than robot"); 
  else
     Serial.println("Object hight&width is bigger than robot");

}

void checkCenterDistance(){
    
  SonarSensor(trigPinC, echoPinC);
  centerDistance = distance;
  
}

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
  stopMotor();
  for (int i = 0; i <= limit; i++)
  {
    //left wheel moves forward
    md.setM1Speed(-i);
    stopIfFault();
    
    //right wheel moves forward
     md.setM2Speed(i+1);
     stopIfFault();

    delay(5);
  }
}


void stopMotor(){
   md.setM1Speed(0);
   md.setM2Speed(0);
   delay(5);

}

void reverse(){
 //loop to move the wheels backward
 for (int i = limit; i >= -limit; i--)
  {
    //left wheel moves backward
    md.setM1Speed(i);
    stopIfFault();
    
    //right wheel moves backwards
    md.setM2Speed(-i);
    stopIfFault();
    delay(5);
  }
  
    delay(200);

}


void turnLeft(){
  stopMotor();
  for (int i = 0; i <= limit; i++)
  {
    //left wheel moves forward
    md.setM1Speed(i);
    stopIfFault();
    
    md.setM2Speed(0);
    stopIfFault();

    delay(5);
  }
    delay(200);

}
void turnRight(){
  stopMotor();
  
  for (int i = 0; i <= limit; i++)
  {
    

    //right wheel moves forward
     md.setM1Speed(0);
     stopIfFault();
     md.setM2Speed(i);
     stopIfFault();
    


    delay(5);
  }
    delay(200);


}
