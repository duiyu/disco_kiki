#include <FastLED.h>
#include <Adafruit_Si7021.h>
#include <stdio.h>
#include <string.h>
#include <math.h>  
#include <time.h>
#include <stdlib.h> 


#define DATA_PIN    6
#define NUM_LEDS    24
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];


Adafruit_Si7021 sensor = Adafruit_Si7021();

float temp;
float humidity;
int pResistor = 2;
int fsrAnalogPin = 0; // FSR is connected to analog 0
int fsrReading;      // the analog reading from the FSR resistor divider

int uvRed,uvGreen,uvBlue;
int tempRed,tempGreen,tempBlue;
int humiRed,humiGreen,humiBlue;

float humiArr[5]; 
float tempArr[5]; 
float uvArr[5]; 
float humiSum, tempSum, uvSum;
float humiAve, tempAve, uvAve;
int times = 0;

int value;

void setup() {
  pinMode(pResistor, INPUT);
  Serial.begin(9600);
  sensor.begin();
  // put your setup code here, to run once:
 FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
}

void printStat() {
  //temp & humid sensor
  temp = sensor.readTemperature();
  humidity= sensor.readHumidity();
  Serial.print("temp  = ");
  Serial.println(temp);
  Serial.print("humidity = ");
  Serial.println(humidity);
  
  //uv sensor
  fsrReading = analogRead(fsrAnalogPin);
  Serial.print("Analog reading = ");
  Serial.println(fsrReading);

  //delay(1000);
}






void tempColor (float tempIndex) {
  //red(255, 0, 0) to green (0,255, 0)
  //20 celsius is the most comfortable temperature, light is green
  //towards 10 celsius or 30 celsius, light turn red; 
    tempBlue = 0;

  
  if (tempIndex < 15) {
    tempRed = 255;
    tempGreen = 0;
  } else if ((15<tempIndex) && (tempIndex < 30))  {
    int numRed = (rand() % 100 );
    Serial.println("red : ");
    Serial.println(numRed);

    int numGreen = ( rand() % (255 - 100 + 1) + 100);
    Serial.println("green : ");
    Serial.println(numGreen);
    
    tempRed = numRed;
    tempGreen = numGreen;
  } else if ( tempIndex >30)  {
    tempRed = 150;
    tempGreen = 50;
  }
    
   for( int i = 0; i < NUM_LEDS; i = i + 3) {
       //Serial.println("hiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii ");
      leds[i] = CRGB(tempRed,tempGreen,tempBlue);
      FastLED.show();        
   }
  
}

void humiColor (float humiIndex) {
  humiBlue = 255;
  if (humiIndex < 30) {
      humiRed = 255;
      humiGreen = 0;
  } else if ((30<humiIndex) && (humiIndex < 60))  {
    int numRed = (rand() % (50 - 0 + 1) + 0 );
    int numGreen = ( rand() % (255 - 100 + 1) + 100);
      humiRed = numRed;
      humiGreen = numGreen;
  } else if ( humiIndex >60)  {
      humiRed = 0;
      humiGreen = 50;
  }
    
  for( int i = 1; i < NUM_LEDS; i = i + 3) {
     leds[i] = CRGB(humiRed,humiGreen,humiBlue);
     FastLED.show();
  }
}

void uvColor (int uvIndex) {
  //red(255, 0-100, 0) to orange (255,100-200,0) yellow (255,200-255, 0)
  //the more UV plant get, the yellower light is
  uvRed = 220;
  uvBlue = 0;

  
  double uvGreenTemp = uvIndex;
  uvGreenTemp = uvGreenTemp/180*255;  //if uv index is 20, it get enough light
  uvGreen = uvGreenTemp;

  for( int i = 2; i < NUM_LEDS; i = i + 3) {
     leds[i] = CRGB(uvRed,uvGreen,uvBlue);
      FastLED.show();
  }
}
  




void loop() {
 



  value = analogRead(pResistor);
  Serial.print("photo resistor : ");
  Serial.println(value);
   printStat();
  //delay(12000);
  //delay(1000);

  if (value < 800) {
    uvColor(fsrReading);
    humiColor(humidity);
    tempColor(temp);
  } else { 
      for( int i = 0; i < NUM_LEDS; i ++) {
        leds[i] = CRGB::Black;
        FastLED.show();        
      }
  }


  
  if (times < 5) {
    humiArr[times] = humidity; 
    tempArr[times] = temp;
    uvArr[times] = fsrReading;
    
    humiSum = humiSum + humiArr[times];
    tempSum = tempSum + tempArr[times];
    uvSum = uvSum + uvArr[times];
    
    times = times + 1;
  } else {
      time_t now;
      time(&now);
      Serial.print(ctime(&now));
      humiAve = humiSum/5;
      tempAve = tempSum/5;
      uvAve = uvSum/5;
      Serial.print("Ave humidity : ");
      Serial.println(humiAve);
      Serial.print("Ave temperature : ");
      Serial.println(tempAve);
      Serial.print("Ave uv : ");
      Serial.println(uvAve);
      Serial.println("      ***********         ");
      times = 0;
  }


}
