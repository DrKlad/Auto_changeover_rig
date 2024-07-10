#include <SPI.h>
#include <RTClib.h>
#include <Adafruit_ILI9341.h>
#include <SD.h>
#include <Wire.h>
#include <HoneywellZephyrI2C.h>
#include <HoneywellTruStabilitySPI.h>
#define TFT_DC 3 // D/C PIN
#define TFT_CS 2// CS PIN

float PS;
float PSR;
float PS1;
float PS2 ;
float FS1;
const int SS_R= Q0_0;
const int SS_1= Q0_1;
const int SS_2= Q0_2;
int enter_button = I0_5;
uint8_t Z = 0;
uint8_t W = 0;
float X;
float Y;
int R;
uint8_t i = 0;
float min_press = 4.2;
float max_press = 4.3;
int SV3 = R0_5;
int SV2 = R0_6;
int SV1 = R0_7;
int SV4 = R0_4;
float pressure_sensor;
float reg_press;
float pressure_1;
float pressure_2;
float flow_val;
int Low_Changeover_time = 15000; //change to 15000
int High_Changeover_time = 5000; //change to 5000
const int wait = 3000;
float l_level;
float h_level;
String valve;
String SV1_;
String SV2_;
int read_delay = 100;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS,TFT_DC);

TruStabilityPressureSensor PSRsensor( Q0_0, 0, 150 ); //(Slaveselect pin, min pressure, max pressure)
TruStabilityPressureSensor PS1sensor( Q0_1, 0, 150 ); //(Slaveselect pin, min pressure, max pressure)
TruStabilityPressureSensor PS2sensor( Q0_2, 0, 150 ); //(Slaveselect pin, min pressure, max pressure)

ZephyrFlowRateSensor FS1sensor(0x49, 15, ZephyrFlowRateSensor::SLPM); // (I2C address, Flow range, Flow units)

float readPSR()
{
  float PSR;
  PSRsensor.readSensor();
  PSR = ((PSRsensor.pressure()*1)+0)/14.504; // ---------------------------------------------  PSR CALIBRATION
  return PSR;
}

float readPS1()
{
  float PS1;
  PS1sensor.readSensor();
  PS1 = ((PS1sensor.pressure()*1)+0)/14.504; // ---------------------------------------------  PS1 CALIBRATION
  return PS1;
}

float readPS2()
{
  float PS2;
  PS2sensor.readSensor();
  PS2 = ((PS2sensor.pressure()*1)+0)/14.504; // ---------------------------------------------  PS2 CALIBRATION
  return PS2;
}

float readFS1(){

  float FS1;
  FS1sensor.readSensor();
  FS1 = ((FS1sensor.flow())*1)+0;    // ---------------------------------------------- FS1 CALIBRATION
  return FS1; 
}

void repeat_change_over(int changeTime){
  i=1;
  digitalWrite(SV1,HIGH);
  digitalWrite(SV2,HIGH);

  if(changeTime == Low_Changeover_time){

    Serial.println("0.3LPM Test");
  }

  if(changeTime == High_Changeover_time){

    Serial.println("3LPM Test");
  }

  Serial.print("PS1");
  Serial.print(",");
  Serial.println("PS2");

  while(i<10)  {

    digitalWrite(SV1,LOW);  //Port 1 Change
    digitalWrite(SV2,HIGH);

    pressure_1 = change_over_check(1);
    pressure_2 = readPS2();    

    Serial.print(pressure_1);
    Serial.print(",");

    tft.fillScreen(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(5, 5);
    tft.print("SV1 = ");
    tft.print(pressure_1);
    tft.setCursor(5, 25);
    tft.print("SV2 = ");
    tft.print("----");
    tft.setCursor(5, 100);
    tft.print(i);

    digitalWrite(SV1,HIGH);
    digitalWrite(SV2,LOW); //Port 2 change

    pressure_1 = readPS1();
    pressure_2 = change_over_check(2);

    Serial.println(pressure_2);

    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(5, 5);
    tft.print("SV1 = ");
    tft.print(pressure_1);
    tft.setCursor(5, 25);
    tft.print("SV2 = ");
    tft.print(pressure_2);   
    tft.setCursor(5, 100);
    tft.print(i);

    i++;

// After 5th reading 3 min begins

    if(i == 6){

      tft.setCursor(0,150);
      tft.setTextSize(3);
      tft.println("3 min wait");
      tft.setCursor(0,180);

      delay(wait);
      tft.fillRect(0,150, 200, 200, ILI9341_BLACK);
      tft.setTextSize(2);

    }
  }

  Serial.println("");
  Serial.println("");
}

float change_over_check(int valve){

  tft.setTextColor(ILI9341_WHITE,ILI9341_BLACK);

  float VAL_0;
  float VAL_1;
  float VAL_2;
  float VAL_3;
  float VAL_4;
  float VAL_00;

  while (1){

    if(valve == 1){

      VAL_0 = readPS1();
      tft.setCursor(0,250);
      tft.print("PS1= ");
      tft.println(VAL_0);
      VAL_00 = readPS2();
      tft.setCursor(0,280);
      tft.print("PS2= ");
      tft.println(VAL_00);
      delay(read_delay);
      
      VAL_1 = readPS1();
      tft.setCursor(0,250);
      tft.print("PS1= ");
      tft.println(VAL_1);
      VAL_00 = readPS2();
      tft.setCursor(0,280);
      tft.print("PS2= ");
      tft.println(VAL_00);
      delay(read_delay);
      
      VAL_2 = readPS1();
      tft.setCursor(0,250);
      tft.print("PS1= ");
      tft.println(VAL_2);
      VAL_00 = readPS2();
      tft.setCursor(0,280);
      tft.print("PS2= ");
      tft.println(VAL_00);
      delay(read_delay);
      
      VAL_3 = readPS1();
      tft.setCursor(0,250);
      tft.print("PS1= ");
      tft.println(VAL_3);
      VAL_00 = readPS2();
      tft.setCursor(0,280);
      tft.print("PS2= ");
      tft.println(VAL_00);
      delay(read_delay);

      VAL_4 = readPS1();
      tft.setCursor(0,250);
      tft.print("PS1= ");
      tft.println(VAL_4);
      VAL_00 = readPS2();
      tft.setCursor(0,280);
      tft.print("PS2= ");
      tft.println(VAL_00);
    }

    else if(valve == 2){

      VAL_0 = readPS2();
      tft.setCursor(0,280);
      tft.print("PS2= ");
      tft.println(VAL_0);
      VAL_00 = readPS1();
      tft.setCursor(0,250);
      tft.print("PS1= ");
      tft.println(VAL_00);
      delay(read_delay);
      
      VAL_1 = readPS2();
      tft.setCursor(0,280);
      tft.print("PS2= ");
      tft.println(VAL_1);
      VAL_00 = readPS1();
      tft.setCursor(0,250);
      tft.print("PS1= ");
      tft.println(VAL_00);
      delay(read_delay);
      
      VAL_2 = readPS2();
      tft.setCursor(0,280);
      tft.print("PS2= ");
      tft.println(VAL_2);
      VAL_00 = readPS1();
      tft.setCursor(0,250);
      tft.print("PS1= ");
      tft.println(VAL_00);
      delay(read_delay);

      VAL_3 = readPS2();
      tft.setCursor(0,280);
      tft.print("PS2= ");
      tft.println(VAL_3);
      VAL_00 = readPS1();
      tft.setCursor(0,250);
      tft.print("PS1= ");
      tft.println(VAL_00);
      delay(read_delay);

      VAL_4 = readPS2();
      tft.setCursor(0,280);
      tft.print("PS2= ");
      tft.println(VAL_4);
      VAL_00 = readPS1();
      tft.setCursor(0,250);
      tft.print("PS1= ");
      tft.println(VAL_00);
    }

    if ( VAL_0 - VAL_1 <= abs(.01) &&
      VAL_0 - VAL_2 <= abs(.01) &&
      VAL_0 - VAL_3 <= abs(.01) &&
      VAL_0 - VAL_4 <= abs(.01)){

        break;
      }

    else {};
  }

  return VAL_0;
}

void flow_check(float f_level){

  if(f_level == 0.3){

    l_level = 0.25;
    h_level = 0.35;
  }

  if(f_level == 3){

    l_level = 2.5;
    h_level = 3.5;
  }

  while(1){

    float FC1 = readFS1();
    delay(500);
    float FC2 = readFS1();
    delay(500);
    float FC3 = readFS1();
    delay(500);
    float FC4 = readFS1();
    delay(500);
    float FC5 = readFS1();

    if (FC1>= l_level && FC1<= h_level
      && FC2>= l_level && FC1<= h_level 
      && FC3>= l_level && FC1<= h_level
      && FC4>= l_level && FC1<= h_level 
      && FC5>= l_level && FC1<= h_level){

      tft.fillScreen(ILI9341_BLACK);
      tft.setCursor(5,20);
      tft.println("Flow Check:");
      tft.setCursor(70,50);
      tft.println("In Range");

      break;
    }

    else{

      tft.fillScreen(ILI9341_BLACK);
      tft.setCursor(5,20);
      tft.println("Flow Check [lpm]:");
      tft.setCursor(70,50);
      tft.println("Out of Range");

      flow_val = readFS1();

      tft.fillRect(0,90, 90, 150, ILI9341_BLACK);
      tft.setCursor(0,90);
      tft.println(flow_val);
      
      tft.fillRect(0,90, 90, 150, ILI9341_BLACK);
      tft.setCursor(0,120);
      tft.println(FC1);
      tft.setCursor(0,150);
      tft.println(FC2);
      tft.setCursor(0,180);
      tft.println(FC3);
      tft.setCursor(0,210);
      tft.println(FC4);
      tft.setCursor(0,240);
      tft.println(FC5);
      delay(100);

    }
  }
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  Wire.begin();
  FS1sensor.begin();
  PSRsensor.begin();
  PS1sensor.begin();
  PS2sensor.begin();
  tft.begin();

  pinMode(3,OUTPUT); // NEEDED?
  pinMode(2,OUTPUT); // NEEDED?
  pinMode(53,OUTPUT); // NEEDED?
  //pinMode(50,INPUT_PULLUP); // NEEDED?
  
 
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(5, 5);
  tft.println("NOxBOx Change Over");
  tft.setCursor(5,55 );
  tft.println("TEST RIG");
  tft.setCursor(5, 105);
  tft.println("THE WEST GROUP");
  tft.setCursor(5, 155);
  tft.println("DTV 2023");
  delay(5000);

  }
  
void loop() {

  W = 0;
  i = 0;
  digitalWrite(SV1,LOW);
  digitalWrite(SV2,LOW);
  digitalWrite(SV3,LOW);
  digitalWrite(SV4,LOW);

  tft.fillScreen(ILI9341_BLACK);

  while(digitalRead(enter_button) == LOW){
    tft.setCursor(5,10);
    tft.println("PRESS BUTTON ");
    tft.setCursor(5,60);
    tft.println("TO START TEST");
  }

//PRESSING ENTER BEGINS TEST

  if (digitalRead(enter_button) == HIGH) {

    Z = 1;

    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(5,10);
    tft.println("BEGINNING TEST ");

    delay(500);

    reg_press = readPSR();

    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0,0);
    tft.println("Regulator Pressure");
    tft.setCursor(90,70);
    tft.println(reg_press);
    tft.setCursor(160,70);
    tft.println("[bar]");

    delay(1000);

    //PRESSURE MUST BE BETWEEN 4.2 AND 4.3 BAR

    while(W==0 && Z==1){
      
      if (reg_press < min_press){
       
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(5,20);
        tft.println("Pressure Too Low!");
        tft.setCursor(70,50);
        tft.println("Please Adjust");
        delay(500);
        W = 1;
      }

      else if (reg_press > max_press){
        
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(5,20);
        tft.println("Pressure Too High!");
        tft.setCursor(70,50);
        tft.println("Please Adjust");
        delay(500);
        W = 2;
      }

      else {
        
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(0,20);
        tft.println("Pressure in Range");
        tft.setCursor(90,70);
        tft.println(reg_press);
        tft.setCursor(160,70);
        tft.println("[bar]");
        delay(2000);
        W = 3;
      }

    tft.fillScreen(ILI9341_BLACK);

    while (W == 1 || W == 2){

      reg_press = readPSR();
      tft.setCursor(0,20);
      tft.println("Flow out of range");
      tft.fillRect(0,130, 150, 150, ILI9341_BLACK);
      tft.setCursor(90,150);
      tft.println(reg_press);
      delay(100);

      if (digitalRead(enter_button) == HIGH){
        W=0;
      }

      else{}
    }

    while (W == 3){

      digitalWrite(SV1,HIGH); // TURN SV1 ON
      digitalWrite(SV3,HIGH); // TURN SV3 ON
      delay(100);
      flow_val = readFS1();

      if (flow_val < 0.250 || flow_val > 0.350){

        flow_check(0.3);
      }
      
      else {

        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(0,20);
        tft.println("Flow In Range");
        tft.setCursor(0,60);
        tft.println("Begining Changeover");
        tft.setCursor(0,90);
        tft.println(flow_val);
        delay(wait);
        W = 4;
      }
      

    while (W == 4){

    
      tft.fillScreen(ILI9341_BLACK);
      tft.setCursor(0,20);
      tft.println("Low Changeover");
      tft.setCursor(0,40);
      tft.println("Beginning");

      repeat_change_over(Low_Changeover_time);

      tft.fillScreen(ILI9341_BLACK);
      tft.setCursor(0,20);
      tft.println("Low Changeover");
      tft.setCursor(0,40);
      tft.println("Complete");

      digitalWrite(SV3,LOW); // TURN SV2 ON
      digitalWrite(SV4,HIGH); // TURN SV2 ON
      delay(5000);
      flow_val = readFS1();

      if (flow_val < 2.90 || flow_val > 3.10){

        flow_check(3);
      }
      
      else {

        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(0,20);
        tft.println("Flow In Range");
        tft.setCursor(0,60);
        tft.println("Begining Changeover");
        tft.setCursor(0,90);
        tft.println(flow_val);
        delay(wait);
        W = 5;
        }
    }

    while(W == 5){

      tft.fillScreen(ILI9341_BLACK);
      tft.setCursor(0,20);
      tft.println("High Changeover");
      tft.setCursor(0,40);
      tft.println("Beginning");

      repeat_change_over(High_Changeover_time);

      tft.fillScreen(ILI9341_BLACK);
      tft.setCursor(0,20);
      tft.println("High Changeover");
      tft.setCursor(0,40);
      tft.println("Complete");
      delay(wait);

      W = 6;
    }

    while(W == 6){
      Serial.println("");
      Serial.println("");
      Serial.println("");
      Serial.println("");
      tft.fillScreen(ILI9341_BLACK);
      tft.setCursor(0,20);
      tft.println("Completed");
      delay(wait);
      digitalWrite(SV1,LOW);
      digitalWrite(SV2,LOW);
      digitalWrite(SV3,LOW);
      digitalWrite(SV4,LOW);
      W = 0;
      Z = 0;
    }
  }

    }
  }
}

      