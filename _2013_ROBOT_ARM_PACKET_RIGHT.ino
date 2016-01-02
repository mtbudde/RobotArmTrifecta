/*
*  West Ottawa Science Olympiad
*  Robot Arm
*  Matthew Budde & Jordan Vanderham
*/

#include <DynamixelSerial1.h>
#include <DynamixelSerial2.h>
#include <Sabertooth.h>

int dummyBase = 0, armBasePot = 1, dummyElbow = 2, clawRotate = 3;
int baseVal, elbowVal, wristVal, clawRotateVal;

int dummyWrist = 5, effectorVertical = 6;
int wristSlideVal, effectorVerticalVal;
int verticalLimitDown = 4;
int verticalStateDown;

int verticalLimitUp = 5;
int verticalStateUp;

int armBaseVal;
int basePID;

int clawPin = 4;
int clawPedal = 7;
int clawState;

int speedScale = 10;
int loopcount = 0;

Sabertooth base(128, Serial);

void setup()
{
  
  pinMode(clawPin, INPUT);
  FourPin.begin(1000000, 2);
  ThreePin.begin(1000000, 3);
  Serial.begin(9600);
  //setCMargin(1, I, P)
  FourPin.setCMargin(1, 0, 17);
  //setCSlope(1, D, 0)
  FourPin.setCSlope(1, 20, 0);
  FourPin.setMaxTorque(5, 350);
  
}

void loop()
{
  
  if( speedScale > 1 && loopcount == 25 ) {
    speedScale--;
    loopcount = 0;
  }
  
  //Base
  baseVal = analogRead( dummyBase );
  //baseVal = map(baseVal, 84, 942, 0, 1023);
  armBaseVal = analogRead( armBasePot );
  baseVal /= 3;
  armBaseVal /= 3;
  basePID = updatePid( baseVal, armBaseVal ) / speedScale;
  base.motor( 1, -basePID );
  
  //Elbow
  elbowVal = analogRead( dummyElbow );
  elbowVal = map( elbowVal, 0, 1023, 0, 4095 );
  FourPin.moveSpeed( 1, elbowVal, ( 200 / speedScale ) );
  
  //Wrist Flip
  wristVal = analogRead( dummyWrist );
  wristVal = map( wristVal, 0, 1023, 850, 475 );
  ThreePin.moveSpeed( 3, wristVal, ( 200 / speedScale ) );
  
  //Claw Rotate
  clawRotateVal = analogRead( clawRotate );
  clawRotateVal = map( clawRotateVal, 65, 918, 1023, 0 );
  FourPin.move( 4, clawRotateVal );
  
  //Effector Vertical
  effectorVerticalVal = analogRead( effectorVertical );
  effectorVerticalVal = map( effectorVerticalVal, 50, 777, 800, -500 );
  verticalStateDown = digitalRead( verticalLimitDown );
  verticalStateUp = digitalRead( verticalLimitUp );
  
  if( effectorVerticalVal > 50 ) {
    if( verticalStateUp != HIGH ) {
      FourPin.turn( 2, LEFT, effectorVerticalVal );
    } else {
      FourPin.turn( 2, LEFT, 50 );
    }
  } else if( effectorVerticalVal < -50 ) {
    if( verticalStateDown != LOW ) {
      FourPin.turn( 2, RIGTH, -effectorVerticalVal );
    } else {
      FourPin.turn( 2, RIGTH, 0 );
    }
  } else {
    FourPin.turn( 2, RIGTH, 0 ); 
  }
  
  //Claw
  clawState = digitalRead( clawPedal );
  if( clawState == 1 ) {
    FourPin.moveSpeed( 5, 50, 1023 );
  } else {
    FourPin.moveSpeed( 5, 1023, 1023 );
  }

  loopcount++;
  
}


/*
void closeArm()
{
  baseVal = analogRead(dummyBase);
  armBaseVal = analogRead(armBasePot);
  while(armBaseVal != 
  {
    baseVal = analogRead(dummyBase);
    armBaseVal = analogRead(armBasePot);
    baseVal /= 3;
    armBaseVal /= 3;
    basePID = updatePid(baseVal, armBaseVal);
    base.motor(1, -basePID);
  }
}
*/
