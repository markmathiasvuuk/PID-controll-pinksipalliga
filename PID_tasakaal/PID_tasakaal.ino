// PID palli kontroll
#define trig 7
#define echo 8
int servoPin = 9; 
long duration;

#define kp 50
#define ki 1
#define kd 0.5
double previous=0;
double integral=0;
double dt;
unsigned long last_time = 0;

void setup() {
 pinMode(servoPin,OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);
}

void loop() {
  
  unsigned long now=millis();
  dt=(now-last_time)/1000;
  last_time=now;

  // read distance
  long distance=getDistance();//  read distance

  // run PID
  double PIDvalue=PID(distance);

  // Map PID output to servo angle (0-180)
    int Fvalue = map(PIDvalue, -300, 300, 180, 0);
    if (Fvalue < 0){
     Fvalue = 0; 
    } 
   if (Fvalue > 180){
    Fvalue = 180;
   } 
  Serial.print("PIDvalue = ");
Serial.print(PIDvalue);
Serial.print(" | Servo angle = ");
Serial.println(Fvalue);
  // Move servo manually
  servoWriteManual(Fvalue);

  delay(50); // loop interval
}

  long getDistance(){

  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  long cm=(duration/2)*0.0343;
  

  Serial.print("Distance = ");
  if (cm >= 400 || cm <= 2) {
     Serial.println("Out of range");
  }

  else {
    Serial.print(cm);
    Serial.println(" cm");
    }

  return cm;
 
   } 
  

double PID(long distance) {

  
  int setpoint= 13  ;//mõõda keskpunkt
  double error=setpoint-distance;

  double proportional=error;
  integral +=error*dt;
  double derivative;
  if (dt > 0) {
    derivative = (error - previous) / dt;
  } else {
    derivative = 0;
  }
  previous=error;
  double PIDval=(proportional*kp)+(integral*ki)+(derivative*kd);

  Serial.print("PID output = ");
  Serial.println(PIDval);
  return PIDval;
}
  
  void servoWriteManual(int angle) {

  // Convert angle to pulse width
  int pulseWidth = map(angle, 0, 180, 400, 2600); // microseconds

  digitalWrite(servoPin, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(servoPin, LOW);
  delayMicroseconds(20000 - pulseWidth); // rest of 20ms period

}


