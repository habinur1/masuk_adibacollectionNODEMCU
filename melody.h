
#define DELAY_TIMEOUT 1200
#include "pitch.h"
#include "melody.h"
////////////////////////////////////////////
////////////// variable ldr ////////////////
byte ldr = A0;
int nilai_ldr;
////////////////////////////////////////////
////////////// variable led ////////////////
int biru_kanan = 0;
int hijau_kiri = 2;
int led_terang = 12;
int led_redup = 13;
int ledState = LOW; 
bool untuk_led = false;
unsigned long waktu_led_terang;
unsigned long waktu_led_redup;
unsigned long waktu_led_sekarang;
unsigned long waktu_led_berjalan;            
long previousMillis = 0;        
long interval = 1000;
////////////////////////////////////////////
//////////// variable buzzer ///////////////
int buzzer = 14;
int thisNote = 0;
bool untuk_buzzer = false;
unsigned long waktu_buzzer;
unsigned long waktu_buzzer_berjalan;
////////////////////////////////////////////
//////// variable sensor proximity /////////
int ir_right_pin = 5;
int ir_left_pin = 4;
int ir_right_state = 0;
int ir_left_state  = 0;
int ir_right_state_last = -1;
int ir_left_state_last  = -1;
int in_counter = 0;
int out_counter = 0;
bool bWalkIn = false;
bool cekMasuk = false;
bool bWalkOut = false;
unsigned long tm;
unsigned long cm;
unsigned long ck;
unsigned long nah;
unsigned long waktu = 8000;
unsigned long waktu_lama = 60000;
unsigned long waktu_depan_lagi;
////////////////////////////////////////////
void setup(void) {
  Serial.begin(9600);
  pinMode( ir_right_pin, INPUT);
  pinMode( ir_left_pin , INPUT);
  pinMode(ldr, INPUT);
  pinMode(biru_kanan, OUTPUT);
  pinMode(hijau_kiri, OUTPUT);
  pinMode(led_terang, OUTPUT);
  pinMode(led_redup, OUTPUT);
  pinMode(buzzer, OUTPUT);
}
void loop(void) {
  nilai_ldr = analogRead(ldr);
  ir_right_state = digitalRead( ir_right_pin );  
  ir_left_state =  digitalRead( ir_left_pin );
  led_indikator();
  masuk();
  di_depan();
  keluar();
  led_kedip();
  buzzer_music();
}
//////////////////////////// sensor masuk /////////////////////////////
void masuk(){
  if( ir_right_state != ir_right_state_last ){
    ir_right_state_last = ir_right_state;
      if( (bWalkIn == false) && ( ir_right_state == LOW ) ){
        bWalkIn = true;
         tm = millis();
      }
  }

  if((cekMasuk == false)&&( bWalkIn == true) && (ir_left_state == LOW) && (ir_right_state == HIGH) ){
    bWalkIn = false;
    cekMasuk = true;
    cm = millis();
    in_counter++;
    untuk_led = true;
    untuk_buzzer = true;
  }

  if((millis()-cm) > 300000){
    cekMasuk = false;
  }
  if((millis()-tm) > 10000){
    untuk_led = false;
  }
}
/////////////////////////////// sensor didepan ///////////////////////////////
void di_depan(){
if( ir_right_state != ir_right_state_last ){
    ir_right_state_last = ir_right_state;
      if( (bWalkIn == false) && ( ir_right_state == LOW ) ){
         waktu_depan_lagi = millis();
      }
}

ck = millis() - waktu_depan_lagi;
  if ( (ir_right_state == LOW)&&(ir_left_state == HIGH)&&(bWalkOut ==  false)){
    if(ck == waktu ){
        Serial.println("ada TAMU di Depan TOKO");
          untuk_led = true;
          untuk_buzzer = true;
      }
      else{}

        if(ck == waktu_lama ){
        Serial.println("ada Orang di Depan TOKO Sudah LAMA");
          untuk_led = true;
      }
      else {}
  }
}
/////////////////////////// sensor keluar //////////////////////
void keluar(){
 if( ir_left_state != ir_left_state_last ){
    ir_left_state_last = ir_left_state;
      if( (bWalkOut == false) && ( ir_left_state == LOW ) ){
        bWalkOut = true;
        tm = millis();
      }
  }
 if( (millis() - tm) > DELAY_TIMEOUT ){
    bWalkOut = false;
  }
 if( bWalkOut && (ir_right_state == LOW) && (ir_left_state == HIGH) ){
    bWalkOut = false;
    out_counter++;
    Serial.println(" * Keluar * ");
  }
}
/////////////////////// led kedip //////////////////////////
void led_kedip(){
 if(untuk_led == true) { 
  if(nilai_ldr >= 180){
    waktu_led_terang = millis();
      if((ledState == HIGH) && (waktu_led_terang - waktu_led_sekarang >= 150))
      {
        ledState = LOW;  // Turn it off
        waktu_led_sekarang = waktu_led_terang;  // Remember the time
        digitalWrite(led_terang , ledState);
      }
      else if ((ledState == LOW) && (waktu_led_terang - waktu_led_sekarang >= 650))
      {
        ledState = HIGH;
        waktu_led_sekarang = waktu_led_terang;
        digitalWrite(led_terang , ledState);
      }
  } 
  if(nilai_ldr <= 180){
    waktu_led_redup = millis();
      if((ledState == HIGH) && (waktu_led_redup - waktu_led_sekarang >= 150))
      {
        ledState = LOW;
        waktu_led_sekarang = waktu_led_redup;
        digitalWrite(led_redup , ledState);
      }
      else if ((ledState == LOW) && (waktu_led_redup - waktu_led_sekarang >= 650))
      {
        ledState = HIGH;
        waktu_led_sekarang = waktu_led_redup;
        digitalWrite(led_redup , ledState);
      }
  } 
 }
}
/////////////////// led indikator ///////////////////////
void led_indikator(){
  if(ir_right_state == LOW){
    digitalWrite(biru_kanan, HIGH);
  }else{
    digitalWrite(biru_kanan, LOW);
  }
  
  if(ir_left_state == LOW){
    digitalWrite(hijau_kiri, HIGH);
  }else{
    digitalWrite(hijau_kiri, LOW);
  }
}
///////////////////// buzzer music //////////////////////
void buzzer_music(){
  if(untuk_buzzer == true){
    waktu_buzzer= millis();
    int noteDuration = 1000 / noteDurations[thisNote];
    int pauseBetweenNotes = noteDuration * 1.30;
    int size = sizeof(melody) / sizeof(int);
  
      if ((waktu_buzzer - waktu_buzzer_berjalan >= pauseBetweenNotes)){
        tone(buzzer , melody[thisNote], noteDuration);
        thisNote++;
        if (thisNote > size){
          untuk_buzzer = false;
          noTone(buzzer);
          thisNote = 0;
          }
        
        waktu_buzzer_berjalan = waktu_buzzer;
      }
  }

}
