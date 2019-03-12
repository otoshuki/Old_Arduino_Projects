const int num_sensors = 5;
int sensor_data[num_sensors] = {0,0,0,0,0};
const int sensor_pins[num_sensors] = {A0, A1, A2, A3, A4};
int num_samples = 100;
float rate = 0.2;

void setup() {
  Serial.begin(9600);
}

void loop() {
  for(int sen_id=0; sen_id<num_sensors; sen_id++)
  {
    sensor_data[sen_id] = get_conditioned(sensor_pins[sen_id], sensor_data[sen_id]);
  }
  for(int i=0; i<5; i++)
  {
    Serial.print(sensor_data[i]);
    Serial.print('\t');
  }
  Serial.print('\n');
}

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
