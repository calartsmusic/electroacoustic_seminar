#include <Audio.h>
#include <Wire.h>
#include <SD.h>

IntervalTimer myTimer;

AudioOutputAnalog audioOutput; 
AudioSynthWaveform sine; 
AudioConnection c0(sine,0,audioOutput, 0);

void setup(void) {
  AudioMemory(16);
  Serial.begin(9600); // for the serial monitor
  myTimer.begin(newFreq,150000);
  sine.begin(0.5, 110.0,TONE_TYPE_SINE);
}

const unsigned int nFreqs = 8;
volatile float freqs[nFreqs] = { 110.0,220.0,330.0,440.0,550.0,660.0,770.0,880.0};
volatile unsigned int freqCount = 0;
volatile float freqScaler = 1.0;

// functions called by IntervalTimer should be short, run as quickly as
// possible, and should avoid calling other functions if possible.

void newFreq(void) {
  sine.frequency( freqs[freqCount] * freqScaler);
  if( freqCount < (nFreqs-1)) freqCount++;
  else freqCount = 0;
}

// The main program will print freqCount
// to the Arduino Serial Monitor
void loop(void) {
  unsigned long freqCountCopy;  // holds a copy of the blinkCount

  // to read a variable which the interrupt code writes, we
  // must temporarily disable interrupts, to be sure it will
  // not change while we are reading.  To minimize the time
  // with interrupts off, just quickly make a copy, and then
  // use the copy while allowing the interrupt to keep working.
  noInterrupts(); // turn off interrupts
  freqCountCopy = freqCount;
  freqScaler = analogRead(A1) / 10.0;
  interrupts(); // turn interrupts back on
  
  // reset interval timer
  myTimer.begin(newFreq, analogRead(A0) * 100); 

  Serial.print("freqCount = ");
  Serial.println(freqCountCopy);
  delay(100);
}

