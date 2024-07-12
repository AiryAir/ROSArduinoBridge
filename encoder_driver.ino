/* *************************************************************
   Encoder definitions
   
   Add an "#ifdef" block to this file to include support for
   a particular encoder board or library. Then add the appropriate
   #define near the top of the main ROSArduinoBridge.ino file.
   
   ************************************************************ */
   
#ifdef USE_BASE
int SCALING_FACTOR = 2;
#ifdef ROBOGAIA
  /* The Robogaia Mega Encoder shield */
  #include "MegaEncoderCounter.h"

  /* Create the encoder shield object */
  MegaEncoderCounter encoders = MegaEncoderCounter(4); // Initializes the Mega Encoder Counter in the 4X Count mode
  
  /* Wrap the encoder reading function */
  long readEncoder(int i) {
    if (i == LEFT) return encoders.YAxisGetCount();
    else return encoders.XAxisGetCount();
  }

  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT) return encoders.YAxisReset();
    else return encoders.XAxisReset();
  }
#elif defined(ARDUINO_ENC_COUNTER)



// Variables
volatile long encoder1Count = 0;
volatile long encoder2Count = 0;

// Interrupt Service Routine for Encoder 1
void encoder1_ISR() {
  static int prevState1 = 0;
  int currState1 = (digitalRead(encoder1PinA) << 1) | digitalRead(encoder1PinB);

  if (prevState1 == 0b00) {
    if (currState1 == 0b01) {
      encoder1Count++;  // Clockwise transition from state 00 to 01
    } else if (currState1 == 0b10) {
      encoder1Count--;  // Counterclockwise transition from state 00 to 10
    }
  } else if (prevState1 == 0b01) {
    if (currState1 == 0b00) {
      encoder1Count--;  // Counterclockwise transition from state 01 to 00
    } else if (currState1 == 0b11) {
      encoder1Count++;  // Clockwise transition from state 01 to 11
    }
  } else if (prevState1 == 0b11) {
    if (currState1 == 0b01) {
      encoder1Count++;  // Clockwise transition from state 11 to 01
    } else if (currState1 == 0b10) {
      encoder1Count--;  // Counterclockwise transition from state 11 to 10
    }
  } else if (prevState1 == 0b10) {
    if (currState1 == 0b00) {
      encoder1Count++;  // Clockwise transition from state 10 to 00
    } else if (currState1 == 0b11) {
      encoder1Count--;  // Counterclockwise transition from state 10 to 11
    }
  }

  prevState1 = currState1;  // Update previous state
}

// Pin Change Interrupt Service Routine for Encoder 2 (A4)
ISR(PCINT1_vect) {
  static int prevEncoded = 0;
  int currEncoded = (digitalRead(encoder2PinA) << 1) | digitalRead(encoder2PinB);

  if ((prevEncoded == 0b00 && currEncoded == 0b01) ||
      (prevEncoded == 0b01 && currEncoded == 0b11) ||
      (prevEncoded == 0b11 && currEncoded == 0b10) ||
      (prevEncoded == 0b10 && currEncoded == 0b00)) {
    encoder2Count++;  // Clockwise rotation
  } else if ((prevEncoded == 0b00 && currEncoded == 0b10) ||
             (prevEncoded == 0b10 && currEncoded == 0b11) ||
             (prevEncoded == 0b11 && currEncoded == 0b01) ||
             (prevEncoded == 0b01 && currEncoded == 0b00)) {
    encoder2Count--;  // Counterclockwise rotation
  }

  prevEncoded = currEncoded;
}

/* Wrap the encoder reading function */
long readEncoder(int i) {
  if (i == LEFT) return encoder1Count*SCALING_FACTOR;
  else return encoder2Count;
}

/* Wrap the encoder reset function */
void resetEncoder(int i) {
  if (i == LEFT){
    encoder1Count = 0L;
  } else { 
    encoder2Count = 0L;
  }
}

/* Wrap the encoder reset function */
void resetEncoders() {
  resetEncoder(LEFT);
  resetEncoder(RIGHT);
}

void setupEncoders() {
  // Encoder 1 pins setup
  pinMode(encoder1PinA, INPUT);
  pinMode(encoder1PinB, INPUT);

  // Encoder 2 pins setup (using PCINT for A4 and A5)
  PCICR |= (1 << PCIE1);     // Enable PCINT for PCINT8 (A4) to PCINT14 (A5)
  PCMSK1 |= (1 << PCINT12);  // Enable PCINT for PC4 (A4)
  PCMSK1 |= (1 << PCINT13);  // Enable PCINT for PC5 (A5)

  attachInterrupt(digitalPinToInterrupt(encoder1PinA), encoder1_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder1PinB), encoder1_ISR, CHANGE);
}

#else
  #error An encoder driver must be selected!
#endif

#endif
