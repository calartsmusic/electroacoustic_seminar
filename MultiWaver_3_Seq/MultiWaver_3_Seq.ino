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


IntervalTimer myTimer;

// create instance
AudioOutputAnalog audioOutput;
AudioSynthWaveform sine;
AudioSynthWaveform seqSine;
AudioSynthWaveform triangle;
AudioSynthWaveform sawtooth;
AudioSynthWaveform square;

AudioMixer4 subMixer;
AudioMixer4 mixer;
AudioConnection c0(sine,0,subMixer, 0);          // connect to mixer
AudioConnection c1(triangle,0,subMixer, 1);
AudioConnection c2(sawtooth,0,subMixer, 2);
AudioConnection c3(square,0,subMixer, 3);
AudioConnection c4(subMixer,0,mixer,0);
AudioConnection c5(seqSine,0,mixer,1);
AudioConnection c6(mixer,0,audioOutput, 0);   // connect mixer to DAC

const unsigned int nFreqs = 8;
volatile float freqs[nFreqs] = { 110.0,220.0,330.0,440.0,550.0,660.0,770.0,880.0};
volatile unsigned int freqCount = 0;
volatile float freqScaler = 1.0;
volatile unsigned long newTime = 150000;

// functions called by IntervalTimer should be short, run as quickly as
// possible, and should avoid calling other functions if possible.
// also, calling analogRead from within this function is sketchy.
// all the analog inputs are on the same ADC, multiplexed. so they
// all use the same interrupt. you could corrupt an ongoing analogRead
// (called from loop()) by calling analogRead here.
void newFreq(void) {
  seqSine.frequency( freqs[freqCount] * freqScaler);
  if( freqCount < (nFreqs-1)) freqCount++;
  else freqCount = 0;
}

void setup(){
//  Serial.begin(19200);
  AudioMemory(16);
  pinMode(7,INPUT);
  pinMode(8,INPUT);
  pinMode(9,INPUT);
  pinMode(10,INPUT);
  sine.begin( 0.5,777.7,TONE_TYPE_SINE);
  seqSine.begin( 0.5,777.7,TONE_TYPE_SINE);
  triangle.begin( 0.5,777.7,TONE_TYPE_TRIANGLE);
  sawtooth.begin( 0.5,777.7,TONE_TYPE_SAWTOOTH);
  square.begin( 0.5,777.7,TONE_TYPE_SQUARE);
  myTimer.begin(newFreq,newTime);
}

void loop(void){
  AMin;
  gain;
  FMin;
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
  
  AMin = analogRead(A4) / 1024.0;
  gain = (analogRead(A0) / 1024.0) * AMin;
  FMin = analogRead(A3);
  freqFine = (analogRead(A2) / 1024.0) * 50;
  freq = (analogRead(A1) / 1024.0) * range + freqFine + offset + FMin;
  freqScaler = analogRead(A5) / 100.0;
//  Serial.println(analogRead(A6),DEC);
//  Serial.println(newTime,DEC);
  newTime = (analogRead(A6) * 1000) + 1000;
//  newTime = map(analogRead(A6),0,1023,300000,3000);
  
  sine.frequency(freq);
  triangle.frequency(freq);
  sawtooth.frequency(freq);
  square.frequency(freq);

  Serial.println(newTime,DEC);
  myTimer.begin(newFreq, newTime ); 

  // it appears that you need to wait newTime microseconds
  // so the IntervalTimer has time enough to trigger an
  // interrupt before it's reset with .begin() again
  delayMicroseconds(newTime);
}
