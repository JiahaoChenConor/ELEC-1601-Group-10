// Declare left and right servos
#include <Servo.h>
Servo servoLeft;                        
Servo servoRight;




void setup() {
    servoLeft.attach(13);               // Attach left signal to pin 13
    servoRight.attach(12);              // Attach right signal to pin 12
    Serial.begin(9600);
}

void loop() {
  autoNavigation();

}

void autoNavigation()
{

    int R = digitalRead(5);
    int C = digitalRead(4);
    int L = digitalRead(3);

    if (L == 0 && R == 0){
        goStraight();
  
      
    }else if(L == 0 && R == 1){
        goLeft();
  
        
    }else if (L == 1 && R == 0){
        goRight();
        
  
    }else if (L == 1 && R == 1){
        goBack();
        
    }

    
void goLeft()
{   

    // Pivot forward-left
  servoLeft.writeMicroseconds(1500);   // Left wheel stop
  servoRight.writeMicroseconds(1300);  // Right wheel clockwise
  Serial.println("Left");
  delay(200);



}

void goRight()
{


     // Pivot forward-right
    servoLeft.writeMicroseconds(1700);   // Left wheel counterclockwise
    servoRight.writeMicroseconds(1500);  // Right wheel stop
    Serial.println("Right");
    delay(200);


    
}

void goStraight()
{
    servoLeft.writeMicroseconds(1600);
    servoRight.writeMicroseconds(1400);
    Serial.println("Straight");
    delay(200);


}

void goBack(){
    servoLeft.writeMicroseconds(1400);
    servoRight.writeMicroseconds(1600);
    Serial.println("back");
    delay(200);


}
