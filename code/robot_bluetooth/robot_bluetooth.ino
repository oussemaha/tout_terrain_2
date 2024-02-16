#include <SPI.h>  
#include "RF24.h" 
#define correction_right 18
const byte ing1 = 5 ; //in2
const byte ing2 = 7 ; //in1
const byte enabg = 6 ;
const byte ind1 = 4 ; //in4
const byte ind2 = 2 ; //in3
const byte enabd = 3 ;

struct package
{
  int vol = 0;
  int  vitesse =0;
};
typedef struct package Package;
Package data;
RF24 myRadio (9, 10); 
byte addresses[][6] = {"0"}; 

void setup() {
  Serial.begin(9600);
  pin_modes();

}
void loop() {
  String ch="";
     if(Serial.available()){
      c=Serial.readString()
      
      Serial.println("vol:"+String(data.vol)+" speed "+String(data.vitesse));
      data_treatment(data);
    }
}
void initialize_radio(){
  myRadio.begin();
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
}
void data_treatment(Package data){
  int speedy=abs(data.vitesse);
  int vol=map(data.vol,0,1023,-speedy,speedy);
  int speed_right , speed_left;
  
  if(vol>5){
    speed_left=speedy;
    speed_right=speedy-vol;
  }
  else if(vol<0){
    speed_left=speedy+vol;
    speed_right=speedy;
  }
  else{
    speed_right=speedy;
    speed_left=speedy;
  }
  if(speed_right>correction_right){
    speed_right=speed_right-correction_right;
  }
  analogWrite(enabg,speed_left);
  analogWrite(enabd,speed_right);
  Serial.println(String(speed_left)+" "+String(speed_right));
  if(data.vitesse>6){
    digitalWrite(ind1,HIGH);//avance
    digitalWrite(ind2,LOW);
    digitalWrite(ing1,HIGH);
    digitalWrite(ing2,LOW);
  }
  else if(data.vitesse<0){
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
