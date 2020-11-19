# ELEC-1601-Group-10
## Code

This the code from USDY ELEC 1601 R09 GROUP 10 (2020 Semester 2) 

1. The Slave and Master are the final version of our project

2. For convenience, we also have code for testing manual control and auto-navigation separately.

   + In the final version of slave, the manual part needs you to enter "w/a/s/d" continuously to remove

     However in the separate file for testing manual control, you just need to enter "w/a/s/d" once, then the car will move in the corresponding direction.

## Notice

1. Because we use LinkedList for memorizing route, you need to download LinkedList for Arduino.

   Of course, you can write your own LinkedList.

   + The link is here : 😊 https://www.arduino.cc/reference/en/libraries/linkedlist/

   + When using this Linked list, in addition to the precautions mentioned in the document

     + https://github.com/ivanseidel/LinkedList

     if you get this error, just remove tests.cpp file, then everything will be ok.👌

     ```C++
     fatal error: iostream: No such file or directory #include <iostream>
     ```

     

