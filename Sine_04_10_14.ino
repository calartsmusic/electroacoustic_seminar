
//#define ORIGINAL_AUDIOSYNTHWAVEFORM
#include <Audio.h>
#include <Wire.h>
#include <SD.h>


float gain = 0.0, freq= 200.0;
int range = 200, offset = 100, sw=0; 

AudioOutputAnalog audioOutput; 
AudioSynthWaveform sine; 
AudioConnection c0(sine,0,audioOutput, 0);

void setup() { 
 
  AudioMemory(16);
pinMode(7,INPUT);
 pinMode(8,INPUT);
  pinMode(9,INPUT);
   pinMode(10,INPUT);
  sine.begin(0.5, 7777.7,TONE_TYPE_SINE);
  
}


void loop () { 
  
  gain = analogRead(A0) / 1024.0;
  sw = digitalRead(7); 
 offset = digitalRead(8);
// 
 if(sw) { range = 500; }  else { range = 200;};
   if ( offset) { offset = 100; } else { offset = 1000; }; 
    freq = (analogRead(A1) / 1024.0) * range + offset; 
    sine.amplitude(gain); 
    sine.frequency(freq); 
    
  }
  
  
  
