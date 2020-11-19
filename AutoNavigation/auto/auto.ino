// Declare left and right servos
#include <Servo.h>
Servo servoLeft;                        
Servo servoRight;




void setup() {
   //!!!!!!!!!!!!!!!!!!!
    servoLeft.attach(13);               // Attach left signal to pin 13
    servoRight.attach(12);              // Attach right signal to pin 12
    Serial.begin(9600);
}

void loop() {
  autoNavigation();
  // delay(50)
  // 这里delay 防止走太快

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

    
////黑线不足以覆盖两边的
//    if (L == 1 && R == 1){
//        goStraight();
//  
//      
//    }else if(L == 1 && R == 0){
//        goLeft();
//  
//        
//    }else if (L == 0 && R == 1){
//        goRight();
//        
//  
//    }else if (L == 0 && R == 0){
//        goBack();
//        
//    }


//// 如果我们tmd只用左边和中间的检测
//    if (L == 0 && C == 0 && R == 0){
//      route.add(8);
//      state = MANUAL;
//    }
//    if (L == 0 && C == 0){
//        goStraight();
//        route.add(0);
//      
//    }else if(L == 0 && C == 1){
//        goLeft();
//        route.add(1);
//        
//    }else if (L == 1 && C == 0){
//        goRight();
//        route.add(2);
//  
//    }else if (L == 1 && C == 1){
//        goBack();
//        route.add(3);
//    }
}

void goLeft()
{   
//     // Turn left in place
//    servoLeft.writeMicroseconds(1300);
//    servoRight.writeMicroseconds(1300);

    // Pivot forward-left
  servoLeft.writeMicroseconds(1500);   // Left wheel stop
  servoRight.writeMicroseconds(1300);  // Right wheel clockwise
  Serial.println("Left");
  delay(200);



}

void goRight()
{

//    servoLeft.writeMicroseconds(1700);
//    servoRight.writeMicroseconds(1700);

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
