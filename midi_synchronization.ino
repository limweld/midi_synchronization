#include <Servo.h> 
Servo myServo[2]; 
int servoPin[]={7,8};
int sol1Pin = 9;
byte incomingByte;
byte note;
byte velocity;
int chan = 1;  //specify what MIDI channel we're listing to
int action=2; //0 =note off ; 1=note on ; 2= null
const int lowNote = 36; //what's the first note?  36 is C1 in Ableton
//setup: declaring inputs and outputs and begin serial
void setup() {
  Serial.begin(9600);  
  //setup our outputs
  for(int i= 0; i<2; i++){
  myServo[i].attach(servoPin[i]);
  myServo[i].write(90);
  }
  pinMode(sol1Pin, OUTPUT);
}
//loop: wait for serial data, and interpret the message
void loop () {
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    // wait for as status-byte, channel 1, note on or off
    if (incomingByte== 143+chan){ // note on message starting starting
      action=1;
    }else if (incomingByte== 127+chan){ // note off message starting
      action=0;
    }else if ( (action==0)&&(note==0) ){ // if we received a "note off", we wait for which note (databyte)
      note=incomingByte;
      triggerSolenoid(note, velocity);
      note=0;
      velocity=0;
      action=2;
    }else if ( (action==1)&&(note==0) ){ // if we received a "note on", we wait for the note (databyte)
      note=incomingByte;
    }else if ( (action==1)&&(note!=0) ){ // ...and then the velocity
      velocity=incomingByte;
      triggerSolenoid(note, velocity);
      moveServo(note, velocity);
      note=0;
      velocity=0;
      action=0;
    }else{
      //nada
    }
  }
}
void moveServo(byte note, byte velocity){
//write an angle based on velocity to our servo
switch(note){
   case lowNote:  //if it's note 36
   {int angle1 = int(map(velocity, 0,127, 0,180)); //map velocity to angle
   myServo[0].write(angle1); //write our angle to the servo
   break;
   }
   case lowNote+1:{ //if it's note 37
   int angle2 = int(map(velocity, 0,127, 0,180)); //map velocity to angle
   myServo[1].write(angle2); //write our angle to the servo
   break;
   }
}
 }
void triggerSolenoid(byte note, byte velocity){
 if(note == lowNote+2){ //if it's note 38
      int value=LOW; 
  if (velocity >10){ //if the velocity is over 10 turn the solenoid on
      value=HIGH;
  }else{  //other wise turn it off
   value=LOW;
  }
   digitalWrite(sol1Pin, value);  //write our value to the solenoid
   }
}
