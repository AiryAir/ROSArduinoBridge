/* *************************************************************
   Encoder driver function definitions - by James Nugen
   ************************************************************ */
   
   
#ifdef ARDUINO_ENC_COUNTER
  //below can be changed, but should be PORTD pins; 
  //otherwise additional changes in the code are required
  #define LEFT_ENC_PIN_A 2  //pin 2
  #define LEFT_ENC_PIN_B 4 //pin 3
  
  //below can be changed, but should be PORTC pins
  #define RIGHT_ENC_PIN_A A4  //pin A4
  #define RIGHT_ENC_PIN_B A5   //pin A5

  const int encoder1PinA = 2;  // Channel A of Encoder 1 connected to pin 2
  const int encoder1PinB = 3;  // Channel B of Encoder 1 connected to pin 3
  const int encoder2PinA = A4; // Channel A of Encoder 2 connected to pin A4 (PC4)
  const int encoder2PinB = A5; // Channel B of Encoder 2 connected to pin A5 (PC5)
#endif
   
long readEncoder(int i);
void resetEncoder(int i);
void resetEncoders();
