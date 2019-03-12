//Guining Pertin - 10-10-19

#include <LedControl.h>

//Important pins
#define TR_PIN 3
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
                   0b00001000,
                   0b00011100,
                   0b01111111,
                   0b00011100,
                   0b00001000,
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

int load[8]     = {0b00000000,
                   0b00000000,
                   0b00000000,
                   0b00000000,
                   0b00000000,
                   0b00000000,
                   0b00000000,
                   0b00000000
                  };

int done[8]     = {0b00000001,
                   0b00000010,
                   0b00000100,
                   0b10001000,
                   0b01010000,
                   0b00100000,
                   0b00000000,
                   0b00000000
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
const int num_samples = 20;
//Exp moving average
const float rate = 0.2;
//Sensors
const int num_sensors = 5;
const int sensor_pins[num_sensors] = {A0, A1, A2, A3, A4};

//Variables
int char_index = 0;
int sensor_data[num_sensors] = {0,0,0,0,0};
char to_print[8];
//Gaussian fit
unsigned int calib_means[8][num_sensors];
//Calibration
bool calib_state = false;

//Function declarations
int get_conditioned(int pin_num, int prev_val);
void draw_char(int to_draw[8]);
int perform_calib();
void isr();

void setup() 
{
  //Attach interrupt
  attachInterrupt(digitalPinToInterrupt(TR_PIN), isr, FALLING);
//  pinMode(TR_PIN, INPUT);
  //Initialize matrix
  my_matrix.shutdown(0, false);
  my_matrix.setIntensity(0, 0);
  my_matrix.clearDisplay(0);
  //Start serial
  Serial.begin(9600);
  //Perform calibration at the beginning
  perform_calib();
}

void loop() 
{
  //Calibration
  if(calib_state) perform_calib();
  //Get data from 5 sensors
  for(int sen_id=0; sen_id<num_sensors; sen_id++)
  {
    sensor_data[sen_id] = get_conditioned(sensor_pins[sen_id], sensor_data[sen_id]);
    Serial.print(sensor_data[sen_id]);
    Serial.print('\t');
  }
  Serial.print('\n');
  //Predict current symbol
  int curr_symbol = predict_symbol();
  draw_char(symbols[curr_symbol]);
//  Serial.println(curr_symbol);
//  delay(500);
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
  calib_state = true;
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

//Calibration function for users
int perform_calib()
{
  Serial.println("Performing calibration");
  //For each symbol
  for (int sym_id=0; sym_id<8; sym_id++)
  {
    //First, show calibration logo
    for(int i=0; i<9; i++)
      {
        draw_char(load);
        load[3] += 0b10000000>>i;
        load[4] += 0b10000000>>i;
        delay(200);
      }
    load[3] = 0b00000000;
    load[4] = 0b00000000;
    draw_char(symbols[sym_id]);
    delay(1000);
    unsigned int char_data[num_sensors][100] = {0};
    unsigned int char_mean[num_sensors] = {0};
    //Get 100 samples
    for(int i=0; i<100; i++)
    {
      //For each sensor
      for(int sen_id=0; sen_id<num_sensors; sen_id++)
      {
        char_data[sen_id][i] = map(analogRead(sensor_pins[sen_id]),0,300,0,300);
        //Fitting normalized mean onto the sample
        char_mean[sen_id] += char_data[sen_id][i];
        delay(5);
//        Serial.print(char_data[sen_id][i]);
//        Serial.print("\t");
      }
//      Serial.print("\n");
    }
    //Fit Gaussian onto the samples
    //Calculate empirical 5D mean and save it for the character
    for(int sen_id=0; sen_id<num_sensors; sen_id++)
    {
      calib_means[sym_id][sen_id] = char_mean[sen_id]/100;
      Serial.print(calib_means[sym_id][sen_id]);
      Serial.print('\t');
    }
    Serial.println('\n');
  }
  //Set back the state
  draw_char(done);
  calib_state = false;
  delay(1000);
}

//Prediction
int predict_symbol()
{
  float min_dist = 10000;
  int predicted = 0;
  //For all symbols
  for(int sym_id=0; sym_id<8; sym_id++)
  {
    float dist = 0;
    //Get L2 distance
    for(int sen_id=0; sen_id<num_sensors; sen_id++)
    {
      dist += pow(calib_means[sym_id][sen_id] - map(sensor_data[sen_id],0,300,0,300), 2);
    }
    dist = sqrt(dist);
    //If this is the minimum distance
    if(dist<min_dist)
    {
      min_dist = dist;
      predicted = sym_id;
    }
  }
  return predicted;
}
