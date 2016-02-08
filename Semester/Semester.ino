#include "FastLED.h"

#define NUM_LEDS 64
#define DATA_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];
unsigned int ledsEnabled[NUM_LEDS][2];
int currCol = 0;
int currRow = 0;
int timerCol = 0;

/*    Algo for calculating LED#:
 *    currCol + 8*currRow = LED#
 */

void setup() { 
  pinMode(2, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  FastLED.addLeds<PL9823, DATA_PIN, RGB>(leds, NUM_LEDS);
  Serial.begin(9600);
  //Turns them all off
  allOff();
  for(int i=0;i<NUM_LEDS;i++){
    ledsEnabled[i][0] = false;
    ledsEnabled[i][1] = CRGB::Black;
  }
  FastLED.setBrightness(50);
}

void loop() {
  double timeOut = 0;
  if(isSwitchOn()){ //If the switch is on, music should be playing
    timeOut = calcNum();
  }else{ //If music is playing, dont show the cursor
    calcJoy();
    timerCol = 0;
  }

  
  applyLEDs();

  
  if(isSwitchOn()){
    enableCol(timerCol);
    if(timerCol==7){
      timerCol=0;
    }else{
      timerCol++;
    }
    delay(((timeOut/9)*400)+100);
  }else{
    delay(300);
  }
}

void calcJoy(){
  if(analogRead(5) < 50 && currRow<=6){
    currRow++;
  }
  if(analogRead(5) > 1000 && currRow>=1){
    currRow--;
  }
  if(analogRead(4) > 1000 && currCol<=6){
    currCol++;
  }
  if(analogRead(4) < 50 && currCol>=1){
    currCol--;
  }
}

void enableCol(int col){
  for(int i=0;i<=56;i+=8){
    leds[col+i] = CRGB::Yellow;
  }
  playSounds(col);
  FastLED.show();
}

void playSounds(int col){
  int sound;
  for(int i=0;i<=56;i+=8){
    if(ledsEnabled[col+i][0]){
      sound = getRow(col+i);
      Serial.println(getRow(col+i));
      switch(sound){
        case 0:
          tone(4, 500, 50);
          break;
        case 1:
          tone(4, 700, 50);
          break;
        case 2:
          tone(4, 900, 50);
          break;
        case 3:
          tone(4, 1100, 50);
          break;
        case 4:
          tone(5, 1300, 50);
          break;
        case 5:
          tone(5, 1500, 50);
          break;
        case 6:
          tone(5, 1700, 50);
          break;
        case 7:
          tone(5, 1900, 50);
          break;
      }
      //return;
    }
  }
}

int getCol(int led){
  for(int c=0;c<8;c++){
    for(int r=0;r<8;r++){
      if(c+8*r == led){
        return c;
      }
    }
  }
  return false;
}

int getRow(int led){
  for(int c=0;c<8;c++){
    for(int r=0;r<8;r++){
      if(c+8*r == led){
        return r;
      }
    }
  }
  return false;
}

int calcNum(){
  int num = 0;
  //bitWrite(x, n, b)
  bitWrite(num, 3, !digitalRead(11));
  bitWrite(num, 2, !digitalRead(10));
  bitWrite(num, 1, !digitalRead(9));
  bitWrite(num, 0, !digitalRead(8));
  return num;
}

void applyLEDs(){
  isButtonPressed();
  for(int i=0;i<NUM_LEDS;i++){
    if(ledsEnabled[i][0]){
      leds[i] = ledsEnabled[i][1];
    }else{
      leds[i] = CRGB::Black;
    }
  }

  if(!isSwitchOn()){
    leds[currCol + 8*currRow] = CRGB::Green;
  }
  
  FastLED.show();
}

boolean isSwitchOn(){
  return digitalRead(6);
}

void isButtonPressed(){
  int status = digitalRead(2);
  if(!status){
    ledsEnabled[currCol + 8*currRow][0] = !ledsEnabled[currCol + 8*currRow][0];
    ledsEnabled[currCol + 8*currRow][1] = ledsEnabled[currCol + 8*currRow][1]==CRGB::Black ? CRGB::Blue : CRGB::Black;
    flashLED(currCol + 8*currRow, CRGB::Blue);
  }
}

void flashLED(int led, CRGB color){
  leds[led] = color;
  FastLED.show();
  delay(250);
  leds[led] = CRGB::Black;
}

void setLEDOff(int led){
  leds[led] = CRGB::Black;
  FastLED.show();
}

int calcLED(int col, int row){
  return col + 8*row;
}

void allOff(){
  for(int i=0;i<NUM_LEDS;i++){
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

