//-----------------------------------------------------------------------------------------------------------//
//                                                                                                           //
//  Slave_ELEC1601_Student_2019_v3                                                                           //
//  The Instructor version of this code is identical to this version EXCEPT it also sets PIN codes           //
//  20191008 Peter Jones                                                                                     //
//                                                                                                           //
//  Bi-directional passing of serial inputs via Bluetooth                                                    //
//  Note: the void loop() contents differ from "capitalise and return" code                                  //
//                                                                                                           //
//  This version was initially based on the 2011 Steve Chang code but has been substantially revised         //
//  and heavily documented throughout.                                                                       //
//                                                                                                           //
//  20190927 Ross Hutton                                                                                     //
//  Identified that opening the Arduino IDE Serial Monitor asserts a DTR signal which resets the Arduino,    //
//  causing it to re-execute the full connection setup routine. If this reset happens on the Slave system,   //
//  re-running the setup routine appears to drop the connection. The Master is unaware of this loss and      //
//  makes no attempt to re-connect. Code has been added to check if the Bluetooth connection remains         //
//  established and, if so, the setup process is bypassed.                                                   //
//                                                                                                           //
//-----------------------------------------------------------------------------------------------------------//
#include <LinkedList.h>
#include <Servo.h>
#include <SoftwareSerial.h>   //Software Serial Port

#define RxD 7
#define TxD 6
#define ConnStatus A1

#define DEBUG_ENABLED  1

// ##################################################################################
// ### EDIT THE LINES BELOW TO MATCH YOUR SHIELD NUMBER AND CONNECTION PIN OPTION ###
// ##################################################################################

int shieldPairNumber = 18;

// CAUTION: If ConnStatusSupported = true you MUST NOT use pin A1 otherwise "random" reboots will occur
// CAUTION: If ConnStatusSupported = true you MUST set the PIO[1] switch to A1 (not NC)

boolean ConnStatusSupported = true;   // Set to "true" when digital connection status is available on Arduino pin

// #######################################################

// The following two string variable are used to simplify adaptation of code to different shield pairs

String slaveNameCmd = "\r\n+STNA=Slave";   // This is concatenated with shieldPairNumber later

SoftwareSerial blueToothSerial(RxD,TxD);

// Left to right sensors links 3 4 5
// 2 is for detecting

// Declare left and right servos
Servo servoLeft;                        
Servo servoRight;
LinkedList<int> route = LinkedList<int>();

void setup()
{
    Serial.begin(9600);
    blueToothSerial.begin(38400);                    // Set Bluetooth module to default baud rate 38400
    
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    pinMode(ConnStatus, INPUT);

    //  Check whether Master and Slave are already connected by polling the ConnStatus pin (A1 on SeeedStudio v1 shield)
    //  This prevents running the full connection setup routine if not necessary.

    if(ConnStatusSupported) Serial.println("Checking Slave-Master connection status.");

    if(ConnStatusSupported && digitalRead(ConnStatus)==1)
    {
        Serial.println("Already connected to Master - remove USB cable if reboot of Master Bluetooth required.");
    }
    else
    {
        Serial.println("Not connected to Master.");
        
        setupBlueToothConnection();   // Set up the local (slave) Bluetooth module

        delay(1000);                  // Wait one second and flush the serial buffers
        Serial.flush();
        blueToothSerial.flush();
    }


    servoLeft.attach(13);               // Attach left signal to pin 13
    servoRight.attach(12);              // Attach right signal to pin 12
}


// Program states
typedef enum
{
    INITIAL = 0,
    MANUAL,
    AUTO_NAVIGATION,
    RETURN_FORWARD,
    RETURN_REVERSE
} robot_state_t;


int state = INITIAL;

void autoNavigation()
{

    int R = digitalRead(5);
    int C = digitalRead(4);
    int L = digitalRead(3);

    if (L == 0 && R == 0){
        goStraight();
        route.add(0);
      
    }else if(L == 0 && R == 1){
        goLeft();
        route.add(1);
        
    }else if (L == 1 && R == 0){
        goRight();
        route.add(2);
  
    }else if (L == 1 && R == 1){
        goBack();
        route.add(3);
    }


}


void returnReverse()
{
    for (int i = route.size(); i >= 0; i--){
        switch (route.get(i))
        {
          case 0: // straight
              goStraightR(); break;
          case 1: // Left
              goLeftR(); break;
          case 2: // Right
              goRightR();
              break;
          case 3: 
              goBackR();
              break;
          case -8:
              state = MANUAL;
              break;
        } 
    }   
}




void returnForward()
{
    for (int i = 0; i < route.size(); i++){
        switch (route.get(i))
        {
            case 0: // straight
              goStraight();
              break;
            case 1: // Left
              goLeft();
              break;
            case 2: // Right
              goRight();
              break;
            case 3:
              goBack();
              break;
            case 8:
              state = MANUAL;
              break;
        }
    }
}





void loop()
{
    char recvChar;
    // determine when to change the state
    if (blueToothSerial.available())
    {
        recvChar = blueToothSerial.read();
        Serial.println(recvChar);
        switch (recvChar)
        {
          case 'i':
              state = INITIAL;
              route.clear();
              route.add(-8);
              break;
          case 'm':
              state = MANUAL;
              break;
          case 'n':
              state = AUTO_NAVIGATION;
              break;
          case 'e':
              state = MANUAL;
              route.add(8);
              break;
          case 'f':
              state = RETURN_FORWARD;
              break;
          case 'r':
              state = RETURN_REVERSE;
              break;
        }
    }


    switch (state)
    {
      case INITIAL:
          state = MANUAL;
          break;
      case MANUAL:
          if (recvChar != 0)
          {   
              Serial.print(recvChar);
              if (recvChar == 'w'){
                goStraight();
                route.add(0);
              }else if (recvChar == 'a'){
                goLeft();
                route.add(1);
                
              }else if (recvChar == 'd'){
                goRight();
                route.add(2);
                
              }else if (recvChar == 's'){
                goBack();
                route.add(3);
              }
              // Stop
               servoLeft.writeMicroseconds(1500);  
               servoRight.writeMicroseconds(1500); 
          }

          
          
          break;
        
    case AUTO_NAVIGATION:
        autoNavigation();
        if (digitalRead(2) == 1){  // 如果没有用记得去掉
            blueToothSerial.print('!');
            state = MANUAL;
            route.add(8);
          }
        //delay(500);
        
        break;
        
    case RETURN_FORWARD: // Repeat the process from start to collection
        returnForward();
        
        break;
        
    case RETURN_REVERSE: // Return from object collection point to start point (including manual)
        returnReverse();
   
        break;

    }
}

void goLeft()
{   

    // Pivot forward-left
 servoLeft.writeMicroseconds(1500);   // Left wheel stop
 servoRight.writeMicroseconds(1300);  // Right wheel clockwise
 Serial.println("Left");
 delay(150);
 
 


}

void goLeftR()
{
     // Pivot backward-left
   servoLeft.writeMicroseconds(1500);   // Left wheel stop
   servoRight.writeMicroseconds(1700);  // Right wheel counterclockwise
   Serial.println("Left Return");
   delay(150);


}

void goRight()
{

     // Pivot forward-right
    servoLeft.writeMicroseconds(1700);   // Left wheel counterclockwise
    servoRight.writeMicroseconds(1500);  // Right wheel stop
    Serial.println("Right");
    delay(150);

    
}

void goRightR()
{
    // Pivot backward-right
     servoLeft.writeMicroseconds(1300);     // Left wheel clockwise
     servoRight.writeMicroseconds(1500);    // Right wheel stop
    Serial.println("Right Return");
    delay(150);


}

void goStraight()
{
    servoLeft.writeMicroseconds(1600);
    servoRight.writeMicroseconds(1400);
    Serial.println("Straight");
    delay(150);


}

void goStraightR()
{
    servoLeft.writeMicroseconds(1400);
    servoRight.writeMicroseconds(1600);
    Serial.println("back");
    delay(150);


}


void goBack(){
    servoLeft.writeMicroseconds(1400);
    servoRight.writeMicroseconds(1600);
    Serial.println("back");
    delay(150);


}

void goBackR(){
     servoLeft.writeMicroseconds(1600);
    servoRight.writeMicroseconds(1400);
    Serial.println("Straight");
    delay(150);

}


void setupBlueToothConnection()
{
    Serial.println("Setting up the local (slave) Bluetooth module.");

    slaveNameCmd += shieldPairNumber;
    slaveNameCmd += "\r\n";

    blueToothSerial.print("\r\n+STWMOD=0\r\n");      // Set the Bluetooth to work in slave mode
    blueToothSerial.print(slaveNameCmd);             // Set the Bluetooth name using slaveNameCmd
    blueToothSerial.print("\r\n+STAUTO=0\r\n");      // Auto-connection should be forbidden here
    blueToothSerial.print("\r\n+STOAUT=1\r\n");      // Permit paired device to connect me
    
    //  print() sets up a transmit/outgoing buffer for the string which is then transmitted via interrupts one character at a time.
    //  This allows the program to keep running, with the transmitting happening in the background.
    //  Serial.flush() does not empty this buffer, instead it pauses the program until all Serial.print()ing is done.
    //  This is useful if there is critical timing mixed in with Serial.print()s.
    //  To clear an "incoming" serial buffer, use while(Serial.available()){Serial.read();}

    blueToothSerial.flush();
    delay(2000);                                     // This delay is required

    blueToothSerial.print("\r\n+INQ=1\r\n");         // Make the slave Bluetooth inquirable
    
    blueToothSerial.flush();
    delay(2000);                                     // This delay is required
    
    Serial.println("The slave bluetooth is inquirable!");
}
