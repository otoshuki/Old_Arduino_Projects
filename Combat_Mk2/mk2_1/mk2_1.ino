//Guining Pertin - 10-10-19

#include <SPI.h>
#include <LedMatrix.h>

#define NUMBER_OF_DEVICES 1
#define CS_PIN 15
LedMatrix my_matrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);

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

int search[8]   = {0b01001001,
                   0b00101010,
                   0b00011100,
                   0b01110111,
                   0b00011100,
                   0b00101010,
                   0b01001001,
                   0b00000000
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

int *symbols[8] = {forward, shoot, wait, like, search, move_all, surrender, midfin};

void setup() 
{
  my_matrix.init();
  my_matrix.setIntensity(0);
}

void loop() 
{
  for(int i=0; i<8; i++)
  {
    draw_char(symbols[i]);
    delay(1000); 
  }
  
}

void draw_char(int to_draw[8])
{
  my_matrix.clear();
  for(int i=0; i<8; i++)
  {
    my_matrix.setColumn(i, to_draw[i]);
  }
  my_matrix.commit();
}
