// ATtiny84 setup for driving those tiny 7-segment surface mount dinguses
// THIS IS NOT GOOD CODE I WAS HUNGOVER AND DOING THIS IN THE MIDDLE OF A CON

#define PIN_1L  1  // Pin 1 of module L is connected to PA1 of the ATtiny84 (denoted "1" in ATtinyCore)
#define PIN_1R PIN_1L
#define PIN_2L  3  // Pin 2 module L: PA3 (attinycore 3)
#define PIN_2R  8  // Pin 2 module R: PB2 (attinycore 8)
#define PIN_3L  5  // Pin 3 module L: PA5 (attinycore 5)
#define PIN_3R 10  // Pin 3 module R: PB0 (attinycore 10)
#define PIN_4L  0  // Pin 4 module L: PA0 (attinycore 0)
#define PIN_4R PIN_4L
#define PIN_5L  2  // Pin 5 module L: PA2 (attinycore 2)
#define PIN_5R PIN_5L
#define PIN_6L  7  // Pin 6 module L: PA7 (attinycore 7)
#define PIN_6R  9  // Pin 6 module R: PB1 (attinycore 9)

#define PORTA_OUTPUT_MASK 0b10101111
#define PORTB_OUTPUT_MASK 0b00000111

#define OFF 0b00000000

// Arrangement of PORTA and PORTB inputs to illuminate the left module's segments, one at a time
#define SEG_A_L_PA 0b10100111
#define SEG_B_L_PA 0b10001000
#define SEG_C_L_PA 0b00000001
#define SEG_D_L_PA 0b00000100
#define SEG_E_L_PA 0b10100001
#define SEG_F_L_PA 0b10001111
#define SEG_G_L_PA 0b10100100

#define SEG_A_L_PB 0b00000111
#define SEG_B_L_PB 0b00000010
#define SEG_C_L_PB 0b00000010
#define SEG_D_L_PB 0b00000010
#define SEG_E_L_PB 0b00000010
#define SEG_F_L_PB 0b00000111
#define SEG_G_L_PB 0b00000010

// Arrangement of PORTA and PORTB inputs to illuminate the right module's segments, one at a time
#define SEG_A_R_PA 0b10101111
#define SEG_B_R_PA 0b10000000
#define SEG_C_R_PA 0b10000001
#define SEG_D_R_PA 0b10000100
#define SEG_E_R_PA 0b10000001
#define SEG_F_R_PA 0b10101111
#define SEG_G_R_PA 0b10000100

#define SEG_A_R_PB 0b00000011
#define SEG_B_R_PB 0b00000110
#define SEG_C_R_PB 0b00000000
#define SEG_D_R_PB 0b00000000
#define SEG_E_R_PB 0b00000011
#define SEG_F_R_PB 0b00000110
#define SEG_G_R_PB 0b00000011

// Some way of defining each segment and a whole character display
#define XA 0b00000001   // Segment A
#define XB 0b00000010   // Segment B (etc...)
#define XC 0b00000100
#define XD 0b00001000
#define XE 0b00010000
#define XF 0b00100000
#define XG 0b01000000

#define CHAR_0 XA | XB | XC | XD | XE | XF
#define CHAR_1 XB | XC
#define CHAR_2 XA | XB | XG | XE | XD
#define CHAR_3 XA | XB | XG | XC | XD
#define CHAR_4 XF | XG | XB | XC
#define CHAR_5 XA | XF | XG | XC | XD
#define CHAR_6 XA | XF | XE | XD | XC | XG
#define CHAR_7 XA | XB | XC
#define CHAR_8 XA | XB | XC | XD | XE | XF | XG
#define CHAR_9 XG | XF | XA | XB | XC | XD
#define CHAR_A XE | XF | XA | XB | XC | XG
#define CHAR_b XF | XE | XD | XC | XG
#define CHAR_C XA | XF | XE | XD
#define CHAR_d XB | XC | XD | XE | XG
#define CHAR_E XA | XF | XG | XE | XD
#define CHAR_F XA | XF | XG | XE
#define CHAR_NONE 0

char chars[16] = { CHAR_0, CHAR_1, CHAR_2, CHAR_3, CHAR_4, CHAR_5, CHAR_6, CHAR_7, CHAR_8, CHAR_9, CHAR_A, CHAR_b, CHAR_C, CHAR_d, CHAR_E, CHAR_F };

char currChar;             // Which character we want to display: 0 1 2 3 4 5 6 7 8 9 A b C d E F xa xb xc xd xe xf xg
int idleTime = 60;         // how many milliseconds between character updates
int segCounter = 0;        // Which segment should be illuminated (0-13)
int fooCounter = 0;
unsigned long timer1 = 0;  // Counter that tracks how much time has elapsed since the last segment was written
unsigned long timer1_init = 0;
unsigned long timer2 = 0;  // Counter that tracks how much time has elapsed since the last character was chosen
unsigned long timer2_init = 0;


void setup() {
  DDRA = PORTA_OUTPUT_MASK;   // Set A0, 1, 2, 3, 5, 7 as outputs
  DDRB = PORTB_OUTPUT_MASK;   // Set B0, 1, 2 as outputs

  PORTA = OFF;                // Set all outputs low initially
  PORTB = OFF;                // Set all outputs low initially
  
  delay(idleTime);
  
  // Loop through every segment once
  PORTA = SEG_A_L_PA;
  PORTB = SEG_A_L_PB;
  delay(idleTime);
  PORTA = SEG_B_L_PA;
  PORTB = SEG_B_L_PB;
  delay(idleTime);
  PORTA = SEG_C_L_PA;
  PORTB = SEG_C_L_PB;
  delay(idleTime);
  PORTA = SEG_D_L_PA;
  PORTB = SEG_D_L_PB;
  delay(idleTime);
  PORTA = SEG_E_L_PA;
  PORTB = SEG_E_L_PB;
  delay(idleTime);
  PORTA = SEG_F_L_PA;
  PORTB = SEG_F_L_PB;
  delay(idleTime);
  PORTA = SEG_G_L_PA;
  PORTB = SEG_G_L_PB;
  delay(idleTime);

  timer1_init = millis();
  timer2_init = millis();
}

void show0(int segCounter) {
  if (segCounter == 1) {
    PORTA = SEG_A_L_PA;
    PORTB = SEG_A_L_PB;
  } else if (segCounter == 2) {
    PORTA = SEG_B_L_PA;
    PORTB = SEG_B_L_PB;
  } else if (segCounter == 3) {
    PORTA = SEG_C_L_PA;
    PORTB = SEG_C_L_PB;
  } else if (segCounter == 4) {
    PORTA = SEG_D_L_PA;
    PORTB = SEG_D_L_PB;
  } else if (segCounter == 5) {
    PORTA = SEG_E_L_PA;
    PORTB = SEG_E_L_PB;
  } else if (segCounter == 6) {
    PORTA = SEG_F_L_PA;
    PORTB = SEG_F_L_PB;
  } else {
    PORTA = OFF;
    PORTB = OFF;
  }
}

void show1(int segCounter) {
  if (segCounter == 1) {
    PORTA = SEG_B_L_PA;
    PORTB = SEG_B_L_PB;
  } else if (segCounter == 2) {
    PORTA = SEG_C_L_PA;
    PORTB = SEG_C_L_PB;
  } else {
    PORTA = OFF;
    PORTB = OFF;
  }
}

void show2(int segCounter) {
  if (segCounter == 1) {
    PORTA = SEG_A_L_PA;
    PORTB = SEG_A_L_PB;
  } else if (segCounter == 2) {
    PORTA = SEG_B_L_PA;
    PORTB = SEG_B_L_PB;
  } else if (segCounter == 3) {
    PORTA = SEG_G_L_PA;
    PORTB = SEG_G_L_PB;
  } else if (segCounter == 4) {
    PORTA = SEG_E_L_PA;
    PORTB = SEG_E_L_PB;
  } else if (segCounter == 5) {
    PORTA = SEG_D_L_PA;
    PORTB = SEG_D_L_PB;
  } else {
    PORTA = OFF;
    PORTB = OFF;
  }
}

void show3(int segCounter) {
  if (segCounter == 1) {
    PORTA = SEG_A_L_PA;
    PORTB = SEG_A_L_PB;
  } else if (segCounter == 2) {
    PORTA = SEG_B_L_PA;
    PORTB = SEG_B_L_PB;
  } else if (segCounter == 3) {
    PORTA = SEG_G_L_PA;
    PORTB = SEG_G_L_PB;
  } else if (segCounter == 4) {
    PORTA = SEG_C_L_PA;
    PORTB = SEG_C_L_PB;
  } else if (segCounter == 5) {
    PORTA = SEG_D_L_PA;
    PORTB = SEG_D_L_PB;
  } else {
    PORTA = OFF;
    PORTB = OFF;
  }
}

void show4(int segCounter) {
if (segCounter == 2) {
    PORTA = SEG_B_L_PA;
    PORTB = SEG_B_L_PB;
  } else if (segCounter == 3) {
    PORTA = SEG_C_L_PA;
    PORTB = SEG_C_L_PB;
  } else if (segCounter == 6) {
    PORTA = SEG_F_L_PA;
    PORTB = SEG_F_L_PB;
  } else if (segCounter == 7) {
    PORTA = SEG_G_L_PA;
    PORTB = SEG_G_L_PB;
  } else {
    PORTA = OFF;
    PORTB = OFF;
  }
}

void show5(int segCounter) {
  if (segCounter == 1) {
    PORTA = SEG_A_L_PA;
    PORTB = SEG_A_L_PB;
  } else if (segCounter == 3) {
    PORTA = SEG_C_L_PA;
    PORTB = SEG_C_L_PB;
  } else if (segCounter == 4) {
    PORTA = SEG_D_L_PA;
    PORTB = SEG_D_L_PB;
  } else if (segCounter == 6) {
    PORTA = SEG_F_L_PA;
    PORTB = SEG_F_L_PB;
  } else if (segCounter == 7) {
    PORTA = SEG_G_L_PA;
    PORTB = SEG_G_L_PB;
  } else {
    PORTA = OFF;
    PORTB = OFF;
  }
}

void show6(int segCounter) {
  if (segCounter == 1) {
    PORTA = SEG_A_L_PA;
    PORTB = SEG_A_L_PB;
  } else if (segCounter == 3) {
    PORTA = SEG_C_L_PA;
    PORTB = SEG_C_L_PB;
  } else if (segCounter == 4) {
    PORTA = SEG_D_L_PA;
    PORTB = SEG_D_L_PB;
  } else if (segCounter == 5) {
    PORTA = SEG_E_L_PA;
    PORTB = SEG_E_L_PB;
  } else if (segCounter == 6) {
    PORTA = SEG_F_L_PA;
    PORTB = SEG_F_L_PB;
  } else if (segCounter == 7) {
    PORTA = SEG_G_L_PA;
    PORTB = SEG_G_L_PB;
  } else {
    PORTA = OFF;
    PORTB = OFF;
  }
}

void show7(int segCounter) {
  if (segCounter == 1) {
    PORTA = SEG_A_L_PA;
    PORTB = SEG_A_L_PB;
  } else if (segCounter == 3) {
    PORTA = SEG_B_L_PA;
    PORTB = SEG_B_L_PB;
  } else if (segCounter == 5) {
    PORTA = SEG_C_L_PA;
    PORTB = SEG_C_L_PB;
  } else {
    PORTA = OFF;
    PORTB = OFF;
  }
}

void show8(int segCounter) {
  if (segCounter == 1) {
    PORTA = SEG_A_L_PA;
    PORTB = SEG_A_L_PB;
  } else if (segCounter == 2) {
    PORTA = SEG_B_L_PA;
    PORTB = SEG_B_L_PB;
  } else if (segCounter == 3) {
    PORTA = SEG_C_L_PA;
    PORTB = SEG_C_L_PB;
  } else if (segCounter == 4) {
    PORTA = SEG_D_L_PA;
    PORTB = SEG_D_L_PB;
  } else if (segCounter == 5) {
    PORTA = SEG_E_L_PA;
    PORTB = SEG_E_L_PB;
  } else if (segCounter == 6) {
    PORTA = SEG_F_L_PA;
    PORTB = SEG_F_L_PB;
  } else if (segCounter == 7) {
    PORTA = SEG_G_L_PA;
    PORTB = SEG_G_L_PB;
  } else {
    PORTA = OFF;
    PORTB = OFF;
  }
}

void show9(int segCounter) {
  if (segCounter == 1) {
    PORTA = SEG_A_L_PA;
    PORTB = SEG_A_L_PB;
  } else if (segCounter == 2) {
    PORTA = SEG_B_L_PA;
    PORTB = SEG_B_L_PB;
  } else if (segCounter == 3) {
    PORTA = SEG_C_L_PA;
    PORTB = SEG_C_L_PB;
  } else if (segCounter == 4) {
    PORTA = SEG_D_L_PA;
    PORTB = SEG_D_L_PB;
  } else if (segCounter == 6) {
    PORTA = SEG_F_L_PA;
    PORTB = SEG_F_L_PB;
  } else if (segCounter == 7) {
    PORTA = SEG_G_L_PA;
    PORTB = SEG_G_L_PB;
  } else {
    PORTA = OFF;
    PORTB = OFF;
  }
}


void showA(int segCounter) {
  if (segCounter == 1) {
    PORTA = SEG_A_L_PA;
    PORTB = SEG_A_L_PB;
  } else if (segCounter == 2) {
    PORTA = SEG_B_L_PA;
    PORTB = SEG_B_L_PB;
  } else if (segCounter == 3) {
    PORTA = SEG_C_L_PA;
    PORTB = SEG_C_L_PB;
  } else if (segCounter == 5) {
    PORTA = SEG_E_L_PA;
    PORTB = SEG_E_L_PB;
  } else if (segCounter == 6) {
    PORTA = SEG_F_L_PA;
    PORTB = SEG_F_L_PB;
  } else if (segCounter == 7) {
    PORTA = SEG_G_L_PA;
    PORTB = SEG_G_L_PB;
  } else {
    PORTA = OFF;
    PORTB = OFF;
  }
}

void loop() {
  segCounter++;
  if (segCounter > 8) {
    segCounter = 0;
    fooCounter++;
  }
  if (fooCounter < 1200) {
    show0(segCounter);
  } else if (fooCounter < 2400) {
    show1(segCounter);
  } else if (fooCounter < 3600) {
    show2(segCounter);
  } else if (fooCounter < 4800) {
    show3(segCounter);
  } else if (fooCounter < 6000) {
    show4(segCounter);
  } else if (fooCounter < 7200) {
    show5(segCounter);
  } else if (fooCounter < 8400) {
    show6(segCounter);
  } else if (fooCounter < 9600) {
    show7(segCounter);
  } else if (fooCounter < 10800) {
    show8(segCounter);
  } else if (fooCounter < 12000) {
    show9(segCounter);
  } else if (fooCounter < 13200) {
    showA(segCounter);
  } else {
    fooCounter = 0;
  }
  delayMicroseconds(50);
}
