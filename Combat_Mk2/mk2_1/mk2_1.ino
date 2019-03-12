//Guining Pertin - 10-10-19

#include <LedControl.h>

//Important pins
#define TR_PIN 2
#define DIN 5
#define CS 6
#define CLK 7

//Create matrix
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

//Constants
//Simple moving average
const int num_samples = 50;
//Exp moving average
const float rate = 0.2;
//Sensors
const int num_sensors = 5;
const int sensor_pins[num_sensors] = {A0, A1, A2, A3, A4};

//Variables
int char_index = 0;
int sensor_data[num_sensors] = {0,0,0,0,0};
char to_print[8];

//Function declarations
int get_conditioned(int pin_num, int prev_val);
void draw_char(int to_draw[8]);
void isr();

void setup() 
{
  //Attach interrupt
  attachInterrupt(digitalPinToInterrupt(TR_PIN), isr, FALLING);
  //Initialize matrix
  my_matrix.shutdown(0, false);
  my_matrix.setIntensity(0, 0);
  my_matrix.clearDisplay(0);
  //Start serial
  Serial.begin(9600);
}

void loop() 
{
  //Get data from 5 sensors
  for(int sen_id=0; sen_id<num_sensors; sen_id++)
  {
    sensor_data[sen_id] = get_conditioned(sensor_pins[sen_id], sensor_data[sen_id]);
  }
  //Print all the data
  for(int sen_id=0; sen_id<num_sensors; sen_id++)
  {
    //Normalizing data between 0 and 100 for use
    Serial.print(map(sensor_data[sen_id], 0, 300, 0, 100));
    Serial.print("\t");
  }
  Serial.print("\n");
}

//Helper function to draw the symbol
void draw_char(int to_draw[8])
{
  for(int i=0; i<8; i++)
  {
    my_matrix.setRow(0, i, to_draw[i]);
  }
}

//Interrupt Service Routine
void isr()
{
  if (char_index>=7) char_index = 0;
  else char_index++;
  Serial.println(char_index);
}

//Signal conditioning function
int get_conditioned(int pin_num, int prev_val)
{
  int samples = 0;
  //Perform simple moving average
  for (int i=0; i<num_samples; i++)
  {
    samples += analogRead(pin_num);
  }
  //Return the exponential moving average
  return rate*samples/num_samples + (1-rate)*prev_val;   
}
