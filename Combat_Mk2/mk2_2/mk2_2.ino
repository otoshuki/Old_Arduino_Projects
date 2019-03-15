//Guining Pertin - 13-3-19
//Assumes Gaussian distribution of features
//Assumes equiprobable symbols
//Assumes independent features
//Uses Naive Bayes with MAP rule for decision

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
//Gaussian fit - 5D means and variances
unsigned int calib_means[8][num_sensors];
unsigned int calib_stds[8][num_sensors];
//Calibration
bool calib_state = false;

//Function declarations
int get_conditioned(int pin_num, int prev_val);
void draw_char(int to_draw[8]);
void fit_gaussian(int y_j);
void perform_calib();
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
  //Calibration
  if(calib_state) perform_calib();
  //Get data from 5 sensors
  for(int sen_id=0; sen_id<num_sensors; sen_id++)
  {
    sensor_data[sen_id] = get_conditioned(sensor_pins[sen_id], sensor_data[sen_id]);
  }
  //Predict current symbol using Naive Bayes with MAP rule
  int curr_symbol = naive_predict();
  draw_char(symbols[curr_symbol]);
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

//Gaussian fitting for each symbol/class
void fit_gaussian(int y_j)
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
  draw_char(symbols[y_j]);
  delay(1000);

  //Variables to store sampled data
  //We don't need very high precision, int will suffice
  //We are using mean and std
  //Calculating std*std for variance is easier than root(std) for every prediction
  unsigned int char_data[num_sensors][100] = {0};
  unsigned int char_mean[num_sensors] = {0};
  unsigned int char_std[num_sensors] = {0};
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
    }
  }
  //We have the samples x_i and the means u_x_i for each sensor i
  //Calculate variances for each feature
  for(int sen_id=0; sen_id<num_sensors; sen_id++)
  {
     //Empirical variance over all samples
     for(int i=0; i<100; i++)
     {
        //Get sum(x_i - u_x_i)^2
        char_std[sen_id] += pow(char_data[sen_id][i] - char_mean[sen_id]/100, 2);
     }
     //Std_dev = sqrt(variance), variance by n-1
     char_std[sen_id] = pow(char_std[sen_id]/99, 0.5);
  }
  //Store the data for given character
  for(int sen_id=0; sen_id<num_sensors; sen_id++)
  {
    calib_means[y_j][sen_id] = char_mean[sen_id]/100;
    calib_stds[y_j][sen_id] = char_std[sen_id];
  }
}

//Perform gaussian fit for each symbol
void perform_calib()
{
  //For each symbol gaussian fit
  for(int sym_id=0; sym_id<8; sym_id++)
  {
    fit_gaussian(sym_id);
  }
  //Set back the state
  draw_char(done);
  calib_state = false;
  delay(1000);
}

//PDF given mu and std
float gaussian_func(int x, int mu, int std)
{
  //No need to recalculate this over and over
  static const float inv_sqrt_2pi = 0.3989422804014327;
  float a = (x - mu)/std;
  return inv_sqrt_2pi/std * exp(-0.5f*a*a);
}

//Naive Bayes classification
int naive_predict()
{
  //Consider equiprobable classes, we don't need prior
  //We only need argmax over the conditionals
  int argmax = 0;
  float max_p = 0;
  //For each class
  for(int y_j=0; y_j<8; y_j++)
  {
    float prob_for_y_j = 1;
    //Get product over conditionals
    for (int x_i=0; x_i<num_sensors; x_i++)
    {
      //Get sample
      int x = map(sensor_data[x_i],0,300,0,300);
      prob_for_y_j *= gaussian_func(x, calib_means[y_j][x_i], calib_stds[y_j][x_i]);
    }
    //Get max and argmax
    if (prob_for_y_j > max_p)
    {
      //If curr is max, change max and argmax
      max_p = prob_for_y_j;
      argmax = y_j;
    }
  }
  return argmax;
}
