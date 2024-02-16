#include <SPI.h>  
#include "RF24.h" 
#define correction_right 35 
int max_speed=190;  
const byte ing1 = 5 ; //in2
const byte ing2 = 7 ; //in1
const byte enabg = 6 ;
const byte ind1 = 2 ; //in4
const byte ind2 = 4 ; //in3
const byte enabd = 3 ;
int vol;
struct package
{
  int vol = 0;
  int vitesse =0;
  int nitro=0;
  int btn_r=0;
  int btn_l=0;
};
typedef struct package Package;
Package data;
RF24 myRadio (9, 10); 
byte addresses[][6] = {"3110"}; 

void setup() {
  delay(1000);
  Serial.begin(9600);
  initialize_radio();
  pin_modes();
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
  delay(2000);
  Serial.println("Start");
}
void loop() {
     while (myRadio.available())
    {
      myRadio.read(&data,sizeof(data));
      Serial.println("vol:"+String(data.vol)+" speed "+String(data.vitesse));
      data_treatment(data);
    }
}
void initialize_radio(){
   while (!myRadio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    //while (1) {} // hold in infinite loop
   }   
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
}
void data_treatment(Package data){
  int speed_left,speed_right;
   if(data.nitro==0){
    max_speed=255;
  }
  else{
    max_speed=190;
  }
  if(data.btn_r==0){
    digitalWrite(ind1,LOW);
    digitalWrite(ind2,HIGH);
    digitalWrite(ing1,HIGH);
    digitalWrite(ing2,LOW);
    speed_left=max_speed;
    speed_right=max_speed;
  }
  else if(data.btn_l==0){
    digitalWrite(ind1,HIGH);
    digitalWrite(ind2,LOW);
    digitalWrite(ing1,LOW);
    digitalWrite(ing2,HIGH);
    speed_left=max_speed;
    speed_right=max_speed;
  }
  else if(data.vol<300){
    speed_left=0;
    speed_right=max_speed;
  }
  else if(data.vol>700){
    speed_right=0;
    speed_left=max_speed;
  }
  else{
    speed_left=max_speed;
    speed_right=max_speed;
  }
  /*if(speed_right>correction_right){
    speed_right=speed_right-correction_right;
  }*/
   analogWrite(enabg,speed_left);
   analogWrite(enabd,speed_right);
  
  Serial.println(String(speed_left)+" "+String(speed_right));
  if(data.vitesse>100){
    digitalWrite(ind1,HIGH);//avance
    digitalWrite(ind2,LOW);
    digitalWrite(ing1,HIGH);
    digitalWrite(ing2,LOW);
  }
  else if(data.vitesse<-50){
    digitalWrite(ind1,LOW);//arrière
    digitalWrite(ind2,HIGH);
    digitalWrite(ing1,LOW);
    digitalWrite(ing2,HIGH);
  }
  else{
    digitalWrite(ind1,LOW);//stop
    digitalWrite(ind2,LOW);
    digitalWrite(ing1,LOW);
    digitalWrite(ing2,LOW);
  }
}
void pin_modes(){
  pinMode(ing1,OUTPUT);
  pinMode(ing2,OUTPUT);
  pinMode(enabg,OUTPUT);
  pinMode(ind1,OUTPUT);
  pinMode(ind2,OUTPUT);
  pinMode(enabd,OUTPUT);
}
