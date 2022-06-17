#define SOUND_VELOCITY 0.034

const int usTrig = 32;
const int  usEcho = 33;
int hall_val = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(usTrig, OUTPUT);
  pinMode(usEcho, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  hall();
  Serial.println("OK!"); delay(1000);
  uSonic();
  Serial.println("OK!"); delay(1000);
}

void hall(){
  hall_val = hallRead();
  Serial.print("Hall sensor value = ");
  Serial.println(hall_val);
  delay(500);
}

void uSonic(){
  int timeInt;  //time for the wave to travel
  float dist; 
  // clear trigger
  digitalWrite(usTrig, LOW);
  delayMicroseconds(2);
  // sets the trigger HIGH for 10us
  digitalWrite(usTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(usTrig, LOW);
  
  // Reads the usEcho, returns the sound wave travel time in microseconds
  timeInt = pulseIn(usEcho, HIGH);
  
  // Calculate the distance
  dist = timeInt * SOUND_VELOCITY/2;
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(dist);
  
  delay(1000);
}
