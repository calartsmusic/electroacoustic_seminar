//   MULTIWAVER
//
// Multi-Waveform Oscillator with AM and FM modulation abilities
//
//


#include <Audio.h>
#include <Wire.h>
#include <SD.h>

float  gain = 0.0,     freq = 200.0,   freqFine = 0.0, FMin = 0.0, AMin = 0.0;
int    range  = 200,   offset = 100,   rangeSW = 0;
int    mixerBit1 = 0,  mixerBit2 = 0,  mixerSW = 0;


// create instance
AudioOutputAnalog audioOutput;
AudioSynthWaveform sine;
AudioSynthWaveform triangle;
AudioSynthWaveform sawtooth;
AudioSynthWaveform square;

AudioMixer4 mixer;
AudioConnection c0(sine,0,mixer, 0);          // connect to mixer
AudioConnection c1(triangle,0,mixer, 1);
AudioConnection c2(sawtooth,0,mixer, 2);
AudioConnection c3(square,0,mixer, 3);
AudioConnection c4(mixer,0,audioOutput, 0);   // connect mixer to DAC


void setup(){
//  Serial.begin(19200);
  AudioMemory(16);
  pinMode(7,INPUT);
  pinMode(8,INPUT);
  pinMode(9,INPUT);
  pinMode(10,INPUT);
  sine.begin( 0.5,777.7,TONE_TYPE_SINE);
  triangle.begin( 0.5,777.7,TONE_TYPE_TRIANGLE);
  sawtooth.begin( 0.5,777.7,TONE_TYPE_SAWTOOTH);
  square.begin( 0.5,777.7,TONE_TYPE_SQUARE);
  
}

void loop(){
  AMin        = analogRead(A4) / 1024.0;
  gain        = (analogRead(A0) / 1024.0) * AMin;
  FMin        = analogRead(A3);
  rangeSW     = digitalRead(7); //0 or 1
  offset      = digitalRead(8);
  mixerBit1   = digitalRead(9);
  mixerBit2   = digitalRead(10);
  mixerSW     = (mixerBit2 <<1) + mixerBit1;
  
  switch(mixerSW){
    case 0: sine.amplitude(gain); 
            triangle.amplitude(0); 
            sawtooth.amplitude(0);
            square.amplitude(0);
            break;
    case 1: sine.amplitude(0); 
            triangle.amplitude(gain); 
            sawtooth.amplitude(0);
            square.amplitude(0);
            break;
    case 2: sine.amplitude(0); 
            triangle.amplitude(0); 
            sawtooth.amplitude(0);
            square.amplitude(gain);
            break;
    case 3: sine.amplitude(0); 
            triangle.amplitude(0); 
            sawtooth.amplitude(gain);
            square.amplitude(0);
            break;
           
/* Switching logic
      (pin9 pin10)
      00 = sine
      10 = triangle
      01 = square
      11 = Sawtooth
*/
  };
  
  if( rangeSW ) { range = 1800;} else {range = 800;};
  if(offset) { offset = 20;} else {offset = 800;};
  
//  Serial.print("freq is "); Serial.println(analogRead(A1),DEC);
//  Serial.print("freqFine is "); Serial.println(analogRead(A2),DEC);
  // originally pots divided at 1024.0
  freqFine  = (analogRead(A2) / 1024.0) * 50;
  freq      = (analogRead(A1) / 1024.0) * range + freqFine + offset + FMin;
  sine.frequency(freq);
  triangle.frequency(freq);
  sawtooth.frequency(freq);
  square.frequency(freq);
}
