#include <SPI.h>  
#include "RF24.h" 
#include <LiquidCrystal_I2C.h>
#define joy2 A4 //direction
#define joy1 A5 //vitesse
#define btnr 6
#define btnl 5
#define nitro_button 10 
#define max_speed 200
LiquidCrystal_I2C lcd(0x27,16,2);
RF24 myRadio (7, 8); // CE, CSN
byte addresses[][6] = {"3110"}; 
unsigned int times;
struct package
{
  int vol = 0;
  int vitesse =0;
  int nitro=0;
  int btn_r=0;
  int btn_l=0;
};

typedef struct package Package;

void setup() {
  pinMode(nitro_button,INPUT_PULLUP);
  pinMode(btnr,INPUT_PULLUP);
  pinMode(btnl,INPUT_PULLUP);
  digitalWrite(nitro_button,1);
  //lcd.init();
  //lcd.backlight();
  Serial.begin(9600);
  Serial.println("Start");
  initialize_radio();
  myRadio.openWritingPipe( addresses[0]);
  //lcd.clear();
  //lcd.setCursor(5,0);
  //lcd.print("OBTIMUS");
  delay(1500);
  times=millis();
}
void loop() {
  Package data;
  fill_table(&data);
  Serial.println(digitalRead(btnr));
  Serial.println(digitalRead(btnl));
  data.nitro=digitalRead(nitro_button);
  myRadio.write(&data, sizeof(data));
  Serial.println(data.vitesse);
  delay(5);
  

}

void initialize_radio(){
  if (!myRadio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
   while (1) {} // hold in infinite loop
  }
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ;
}
void fill_table(Package *data){
  data->btn_l=digitalRead(btnl);
  data->btn_r=digitalRead(btnr);
  data->vol = analogRead(joy2);
  data->vitesse=map(analogRead(joy1), 0, 1023, max_speed, -max_speed+75);
  

}  
