
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
int printahLed = LOW; 
int kedip = 0;
bool untuk_led = false;
unsigned long waktu_led;
unsigned long waktu_led_terang;
unsigned long waktu_led_redup;
unsigned long waktu_led_sekarang;
unsigned long waktu_led_berjalan;
////////////////////////////////////////////
//////////// variable buzzer ///////////////
int buzzer = 14;
int note = 0;
bool untuk_buzzer = false;
unsigned long waktu_buzzer;
unsigned long waktu_buzzer_berjalan;
////////////////////////////////////////////
//////// variable sensor proximity /////////
int kanan_pin = 5;
int kiri_pin = 4;
int kanan_state = 0;
int kiri_state  = 0;
int kanan_stateTerakhir = -1;
int kiri_stateTerakhir  = -1;
int hitung_masuk = 0;
int hitung_keluar = 0;
bool jalan_masuk = false;
bool cekMasuk = false;
bool jalan_keluar = false;
unsigned long waktu_masuk;
unsigned long waktu_stop_masuk;
unsigned long ck;
unsigned long nah;
unsigned long waktu = 8000;
unsigned long waktu_lama = 60000;
unsigned long waktu_depan_lagi;
unsigned long waktuLepas;
////////////////////////////////////////////
void setup(void) {
  Serial.begin(9600);
  pinMode( kanan_pin, INPUT);
  pinMode( kiri_pin , INPUT);
  pinMode(ldr, INPUT);
  pinMode(biru_kanan, OUTPUT);
  pinMode(hijau_kiri, OUTPUT);
  pinMode(led_terang, OUTPUT);
  pinMode(led_redup, OUTPUT);
  pinMode(buzzer, OUTPUT);
}
void loop(void) {
  nilai_ldr = analogRead(ldr);
  kanan_state = digitalRead( kanan_pin );  
  kiri_state =  digitalRead( kiri_pin );
  led_indikator();
  masuk();
  di_depan();
//  keluar();
  led_kedip();
  buzzer_music();
}
//////////////////////////// sensor masuk /////////////////////////////
void masuk(){
  if( kanan_state != kanan_stateTerakhir ){
    kanan_stateTerakhir = kanan_state;
      if( (jalan_masuk == false) && ( kanan_state == LOW ) ){
        jalan_masuk = true;
        waktu_masuk = millis();
      }
  }
    
  if((millis() - waktu_masuk) > 4000) {
        jalan_masuk = false;
    }

  if((cekMasuk == false)&&( jalan_masuk == true) && (kiri_state == LOW) && (kanan_state == HIGH) ){
    jalan_masuk = false;
    cekMasuk = true;
    waktu_stop_masuk = millis();
    hitung_masuk++;
    untuk_led = true;
    untuk_buzzer = true;
  }

  if((millis()-waktu_stop_masuk) > 300000){
    cekMasuk = false;
  }

}
/////////////////////////////// sensor didepan ///////////////////////////////
void di_depan(){
if( kanan_state != kanan_stateTerakhir ){
    kanan_stateTerakhir = kanan_state;
      if( (jalan_masuk == false) && ( kanan_state == LOW ) ){
         waktu_depan_lagi = millis();
      }
}

ck = millis() - waktu_depan_lagi;
  if ( (kanan_state == LOW)&&(kiri_state == HIGH)&&(jalan_keluar ==  false)){
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
//void keluar(){
// if( kiri_state != kiri_stateTerakhir ){
//    kiri_stateTerakhir = kiri_state;
//      if( (jalan_keluar == false) && ( kiri_state == LOW ) ){
//        jalan_keluar = true;
//        waktu_masuk = millis();
//      }
//  }
// if( (millis() - waktu_masuk) > 1200 ){
//    jalan_keluar = false;
//  }
// if( jalan_keluar && (kanan_state == LOW) && (kiri_state == HIGH) ){
//    jalan_keluar = false;
//    hitung_keluar++;
//    Serial.println(" * Keluar * ");
//  }
//}
/////////////////////// led kedip //////////////////////////
void led_kedip(){
 if(untuk_led == true) {
  waktu_led = millis(); 
  if(nilai_ldr >= 180){
    waktu_led_terang = millis();
      if((printahLed == HIGH) && (waktu_led_terang - waktu_led_sekarang >= 200))
      {
        printahLed = LOW;  // Turn it off
        waktu_led_sekarang = waktu_led_terang;  // Remember the time
        digitalWrite(led_terang , printahLed);
      }
      else if ((printahLed == LOW) && (waktu_led_terang - waktu_led_sekarang >= 600))
      {
        printahLed = HIGH;
        waktu_led_sekarang = waktu_led_terang;
        digitalWrite(led_terang , printahLed);
        kedip++;
      }
            if (kedip > 20){
                untuk_led = false;
                printahLed = LOW;
                digitalWrite(led_terang,printahLed);
                kedip = 0;
                }
  } 
  if(nilai_ldr <= 180){
    waktu_led_redup = millis();
      if((printahLed == HIGH) && (waktu_led_redup - waktu_led_sekarang >= 200))
      {
        printahLed = LOW;
        waktu_led_sekarang = waktu_led_redup;
        digitalWrite(led_redup , printahLed);
      }
      else if ((printahLed == LOW) && (waktu_led_redup - waktu_led_sekarang >= 600))
      {
        printahLed = HIGH;
        waktu_led_sekarang = waktu_led_redup;
        digitalWrite(led_redup , printahLed);
        kedip++;
      }
            if (kedip > 12){
                untuk_led = false;
                printahLed = LOW;
                digitalWrite(led_redup,printahLed);
                kedip = 0;
        }
  }
 }

}
/////////////////// led indikator ///////////////////////
void led_indikator(){
  if(kanan_state == LOW){
    digitalWrite(biru_kanan, HIGH);
  }else{
    digitalWrite(biru_kanan, LOW);
  }
  
  if(kiri_state == LOW){
    digitalWrite(hijau_kiri, HIGH);
  }else{
    digitalWrite(hijau_kiri, LOW);
  }
}
///////////////////// buzzer music //////////////////////
void buzzer_music(){
  if(untuk_buzzer == true){
    waktu_buzzer= millis();
    int durasi = 1000 / tempo[note];
    int pause = durasi * 1.30;
    int size = sizeof(melody) / sizeof(int);
  
      if ((waktu_buzzer - waktu_buzzer_berjalan >= pause)){
        tone(buzzer , melody[note], durasi);
        note++;
        if (note > size){
          untuk_buzzer = false;
          noTone(buzzer);
          note = 0;
          }
        
        waktu_buzzer_berjalan = waktu_buzzer;
      }
  }

}
