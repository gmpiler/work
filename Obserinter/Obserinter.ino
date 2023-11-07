// 1. preprosessor 
#include "SR04.h" // Ultrasonic sensor
#include <Servo.h> // Servo

// 2. preparation
// 2-0. ObserveScale
#define ObserveScale 12 // This num is the max distance the printer can print, which is suggested to be the num 6 can divide without residue like 60.
int observeScale = ObserveScale;
int observeStep = observeScale / 6;
// 2-1. DC motor
#define debugMode 1 // if 1 then on
#define ENABLE2 6 // frontMotor
#define ENABLE 5  // backMotor
#define DIRC 7  // frontMotor
#define DIRD 8  // frontMotor
#define DIRA 4  // backMotor
#define DIRB 3  // backMotor
#define power 255 // spinSpeed
#define printMax 105 // the degree of the printer's upper limitation
#define printMin 80 // the degree of the printer's lower limitation
#define printDelay 100
// 2-2. Ultrasonic sensor
#define TRIG_PIN 12
#define ECHO_PIN 11
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long distance;
// 2-3. Servo
Servo myservo;
int printHandler = 0;
int printPosition = printMin;
int pastPrintPosition = printMin;
// 2-4. scrollLock
/*int scaleClass = 0;
*int pastScaleClass = -1;
*int scrollLock = 1;
*/
// 3. setUp
void setup() {
  // DC motor
  pinMode(ENABLE,OUTPUT);
  pinMode(ENABLE2, OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  pinMode(DIRC, OUTPUT);
  pinMode(DIRD, OUTPUT);
  Serial.begin(9600);
  // Servo
  myservo.attach(9);
  //printer
  printerInit();
}

// 4. functions
void stop(){
  digitalWrite(ENABLE, LOW);  
  digitalWrite(ENABLE2, LOW);
}
void forward(int runTime) {
  digitalWrite(DIRA, LOW);  
  digitalWrite(DIRB, HIGH);
  digitalWrite(DIRC, LOW);
  digitalWrite(DIRD, HIGH);
  analogWrite(ENABLE, power); // power speed
  analogWrite(ENABLE2, power);  // power speed
  delay(runTime);
  stop();
}
void backward(int runTime) {
  digitalWrite(DIRA, HIGH);
  digitalWrite(DIRB, LOW);
  digitalWrite(DIRC, HIGH);
  digitalWrite(DIRD, LOW);
  analogWrite(ENABLE, power); // power speed
  analogWrite(ENABLE2, power);  // power speed
  delay(runTime);
  stop();
}
void printerInit(){
  myservo.write(printMin);
  delay(printDelay);
}
void printer(int position){
    delay(printDelay);
    myservo.write(position);
}

// 5. mainFunction
void loop() {
  // preparation and initialization
  distance = sr04.Distance(); // ultrasonic sensor
  Serial.print(distance);
  Serial.println("cm");
 
  // debugObserveScale
  if(debugMode == 1){
      Serial.print("(obScale, obStep) = ( ");
      Serial.print(observeScale);
      Serial.print(", ");
      Serial.print(observeStep);
      Serial.println(")");
  }

  // if the position is NOT the same as the previous one, the printer stop scrolling.
  /*if(scaleClass == pastScaleClass){
  *  scrollLock = 1;
  *}else{
  *  scrollLock = 0;
  *}
  */
  // decide the print position
  if(0 <= distance && distance < (1 * observeStep)){
    // Memory the position to print a line
    pastPrintPosition = printPosition;
    printPosition = printMin;
    // Memory the scaleClass
    /*pastScaleClass = scaleClass;
    *scaleClass = 0;
    */
    if(debugMode == 1){ Serial.print("print pos ="); }
    if(debugMode == 1){ Serial.println(printPosition); }
  }else if((1 * observeStep) <= distance && distance < (2 * observeStep)){
    pastPrintPosition = printPosition;
    printPosition = 85;
    //pastScaleClass = scaleClass;
    //scaleClass = 1;
    if(debugMode == 1){ Serial.print("print pos ="); }
    if(debugMode == 1){ Serial.println(printPosition); }
  }else if((2 * observeStep) <= distance && distance < (3 * observeStep)){
    pastPrintPosition = printPosition;
    printPosition = 90;
    //pastScaleClass = scaleClass;
    //scaleClass = 2;
    if(debugMode == 1){ Serial.print("print pos ="); }
    if(debugMode == 1){ Serial.println(printPosition); }
  }else if((3 * observeStep) <= distance && distance < (4 * observeStep)){
    pastPrintPosition = printPosition;
    printPosition = 95;
    //pastScaleClass = scaleClass;
    //scaleClass = 3;
    if(debugMode == 1){ Serial.print("print pos ="); }
    if(debugMode == 1){ Serial.println(printPosition); }
  }else if((4 * observeStep) <= distance && distance < (5 * observeStep)){
    pastPrintPosition = printPosition;
    printPosition = 100;
    //pastScaleClass = scaleClass;
    //scaleClass = 4;
    if(debugMode == 1){ Serial.print("print pos ="); }
    if(debugMode == 1){ Serial.println(printPosition); }
  }else if((5 * observeStep) <= distance && distance < observeScale){
    pastPrintPosition = printPosition;
    printPosition = printMax;
    //pastScaleClass = scaleClass;
    //scaleClass = 5;
    if(debugMode == 1){ Serial.print("print pos! ="); }
    if(debugMode == 1){ Serial.println(printPosition); }
  }else{
    printPosition = pastPrintPosition;  // keep the position if an exception is occurred
    if(debugMode == 1){ Serial.print("print pos ="); }
    if(debugMode == 1){ Serial.println(printPosition); }
  }

  // print the position
  /*if(scrollLock){ forward(100); }
  *if(debugMode == 1) {
  *  Serial.print("scrollLock = ");
  *  Serial.println(scrollLock);
  *}
  */
  
  printer(printPosition);
  delay(1500);
  forward(200);
  delay(1500);
}