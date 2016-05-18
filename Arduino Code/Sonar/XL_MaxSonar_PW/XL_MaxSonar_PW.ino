/*
Test code for the Arduino Uno
Written by Tom Bonar for testing
Sensors being used for this code are the MB70X0 from MaxBotix
*/
const int pwPin1 = 2;

long sensor1, cm, distance_cm;
 int value[9];
 
void setup () {
  Serial.begin(9600);
  pinMode(pwPin1, INPUT);
}

int read_sensor(){
  sensor1 = pulseIn(pwPin1, HIGH);
  cm = (sensor1/58);
  return cm;
}

int median(){
  for(int i =0; i<9;i++){
    value[i] = read_sensor()*4;
  }
  for(int i = 0; i < 9; i++){
   for(int j =i; j < 9; j++){
     if (value[j] < value[i]){
       int temp1 = value[j];
       int temp2 = value[i];
       value[i] = temp1;
       value[j] = temp2;
     }
   }
  }
  int distance_cm = value[4];
  return distance_cm;
}

                                                                                                              /*void read_sensorwater(){
                                                                                                                sensor1 = pulseIn(pwPin1, HIGH);
                                                                                                                sensor1 = sensor1*4.1;
                                                                                                                cm = sensor1/58;
                                                                                                                
                                                                                                                if(cm > 30){
                                                                                                                  cm = cm + 110;
                                                                                                                }
                                                                                                                
                                                                                                              }*/

void loop () {
  //read_sensor();
  
  distance_cm = median();
  
  printall();
  //delay(5);
}

void printall(){
  Serial.print("Distance");
  Serial.print(" = ");
  Serial.print(distance_cm);
  Serial.print("cm");
  Serial.println();
}
