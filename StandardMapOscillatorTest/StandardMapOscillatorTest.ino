
//#define ORIGINAL_AUDIOSYNTHWAVEFORM
#include <Audio.h>
#include <Wire.h>
#include <SD.h>

float gain = 0;
float freq = 0;
float k = 5.13;  // an arbitrary value for now
float x;
float p;
int i = 0;

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//

AudioOutputAnalog   audioOutput;        // DAC

AudioSynthWaveform  chaos;

AudioMixer4         mixer;

// Create Audio connections between the components
//
AudioConnection c1(chaos, 0, mixer, 0); // chaotic oscillator output 0 to mixer input 0
AudioConnection c2(mixer, 0, audioOutput, 0); // mixer output 0 to DAC input 0


void setup() {
  Serial.begin(115200);
 
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(16);
  
  delay(3000);

  if( !chaos.begin( 0.5,777.7,TONE_TYPE_CHAOTIC)) { 
    Serial.println("chaos.begin returned false"); 
  } else {
    chaos.set_stdmap( random(1000)/200.0,random(1000)/200.0,random(1000)/200.0 );
  };
  
  // start with no sound
  mixer.gain(0,0);

}

void loop() {
    gain = analogRead(A0) / 1024.0; // scales gain to 0...1
    k = (analogRead(A1) / 200.0) ; // scales k 0...5.0
    p = analogRead(A2) / 200.0;
    x = analogRead(A3) / 200.0;
    freq = analogRead(A4) + 21.0; 
//    Serial.println( k,DEC );
//    Serial.println(gain,DEC);
    mixer.gain( 0,gain ); // wants gain to be a float, range 0...1
    chaos.amplitude(gain);
//    chaos.set_k(k);
    chaos.set_stdmap( k,x,p );
//    chaos.frequency(freq);
    delay(random(1000)+10);
}


