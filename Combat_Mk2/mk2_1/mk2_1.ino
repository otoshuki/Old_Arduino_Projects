//Guining Pertin - 10-10-19

#include <LedControl.h>

#define DIN 4
#define CS 3
#define CLK 2

LedControl my_matrix = LedControl(DIN, CLK, CS, 1);

//Set of different hand signs
int forward[8]  = {0b00001000,
                   0b00001100,
                   0b11111110,
                   0b11111111,
                   0b11111110,
                   0b00001100,
                   0b00001000,
                   0b00000000
                  };
                  
int shoot[8]    = {0b00001000,
                   0b00011100,
                   0b00101010,
                   0b01111111,
                   0b00101010,
                   0b00011100,
                   0b00001000,
                   0b00000000
                  };
                  
int wait[8]     = {0b00001000,
                   0b00011100,
                   0b00011110,
                   0b00011110,
                   0b01011110,
                   0b01011110,
                   0b00111110,
                   0b00011110
                  };
                  
int like[8]     = {0b00000000,
                   0b01100110,
                   0b11111111,
                   0b11111111,
                   0b01111110,
                   0b00111100,
                   0b00011000,
                   0b00000000
                  };

int search[8]   = {0b11100111,
                   0b11000011,
                   0b10100101,
                   0b00011000,
                   0b00011000,
                   0b10100101,
                   0b11000011,
                   0b11100111
                  };

int move_all[8] = {0b11001000,
                   0b11001100,
                   0b11111110,
                   0b11111111,
                   0b11111110,
                   0b11001100,
                   0b11001000,
                   0b00000000
                  };

int surrender[8]= {0b00111000,
                   0b00111110,
                   0b00111110,
                   0b00111110,
                   0b00100110,
                   0b00100000,
                   0b00100000,
                   0b01110000
                  };

//Explicit content!
int midfin[8]   = {0b00001000,
                   0b00001000,
                   0b00001000,
                   0b00011110,
                   0b01011110,
                   0b01011110,
                   0b00111110,
                   0b00011110
                  };

//Collection of all symbols
int *symbols[8] = {forward, shoot, wait, like, search, move_all, surrender, midfin};

//Variables
int a = 0;
int curr = 0;
float rate = 0.2;
int state = 0;

void setup() 
{
  //Initialize
  my_matrix.shutdown(0, false);
  my_matrix.setIntensity(0, 0);
  my_matrix.clearDisplay(0);
  Serial.begin(9600);
}

void loop() 
{
//  for(int i=0; i<8; i++)
//  {
//    draw_char(symbols[i]);
//    delay(1000); 
//  }

  //Moving average
  a = 0;
  for (int i=0; i<100; i++)
  {
    a += analogRead(A0);
  }
  //Exponential moving average
  curr = rate*a/100 + (1-rate)*curr;
  
  Serial.println(curr);
  if (curr < 100) draw_char(search);
  else my_matrix.clearDisplay(0);
//  Serial.print('\t');
}

//Helper function to draw the symbol
void draw_char(int to_draw[8])
{
  for(int i=0; i<8; i++)
  {
    my_matrix.setRow(0, i, to_draw[i]);
  }
}
