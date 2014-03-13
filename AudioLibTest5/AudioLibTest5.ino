
//#define ORIGINAL_AUDIOSYNTHWAVEFORM
#include <Audio.h>
#include <Wire.h>
#include <SD.h>

float gain = 0;
float freq = 0;

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//

AudioOutputAnalog   audioOutput;        // DAC

#ifdef ORIGINAL_AUDIOSYNTHWAVEFORM
AudioSynthWaveform  triangle(AudioWaveformTriangle);
AudioSynthWaveform  noise(AudioWaveformNoise);
AudioSynthWaveform  sine(AudioWaveformSine);
#else
AudioSynthWaveform  triangle;
AudioSynthWaveform  noise;
AudioSynthWaveform  sine;
AudioSynthWaveform  chaos;
#endif

AudioMixer4         mixer;

// Create Audio connections between the components
//
AudioConnection c1(triangle, 0, mixer, 0); // triangle output 0 to mixer input 0
AudioConnection c2(chaos, 0, mixer, 1);
AudioConnection c3(mixer, 0, audioOutput, 0); // mixer output 0 to DAC input 0


void setup() {
  Serial.begin(115200);
  Serial.println("let's begin...");
  
//  AudioConnection c1(triangle, 0, mixer, 0); // triangle output 0 to mixer input 0
//  AudioConnection c2(chaos, 0, mixer, 1);
//  AudioConnection c3(mixer, 0, audioOutput, 0); // mixer output 0 to DAC input 0
//
//  
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(16);
  
  delay(5000);

#ifdef ORIGINAL_AUDIOSYNTHWAVEFORM
  triangle.amplitude(0.5);
  triangle.frequency(777.7);
  
  sine.amplitude(0.5);
  sine.frequency(555.5);
  
  noise.amplitude(1);
  noise.frequency(777.7);
#else
  triangle.begin( 0.5,777.7,TONE_TYPE_TRIANGLE);
  noise.begin( 0.5,777.7,TONE_TYPE_NOISE);
//  sine.begin( 0.5,777.7,TONE_TYPE_SINE );

   chaos.set_stdmap( 1.13,1000.0/random(1000),1000.0/random(1000) );

  if( !chaos.begin( 0.5,777.7,TONE_TYPE_CHAOTIC)) { 
    Serial.println("FUCK!"); } else {
      Serial.println("happy"); };
  
#endif
  // both mixer channels off, please
  mixer.gain(0,0);
  mixer.gain(1,0);

  
}

void loop() {
    gain = analogRead(A0) / 1024.0; // scales gain to 0...1
    // freq = (analogRead(A1) / 1024.0) * 1000.0 + 60.0;
//    Serial.println( freq,DEC );
//    Serial.println(gain,DEC);
    mixer.gain( 0,gain ); // wants gain to be a float range 0...1
    delay(500);
    mixer.gain( 0,0.0 );
    mixer.gain( 1,gain );
    delay(500);
    mixer.gain( 1,0.0 );
    // noise.frequency(freq);
    // triangle.frequency(freq);
    //chaos.frequency(freq);
    chaos.amplitude(gain);
//    chaos.set_stdmap( 1.13,-0.3,0.5 );
    Serial.println(random(1000)/100.0,DEC);

   chaos.set_stdmap( 1000.0/random(100),1000.0/random(1000),1000.0/random(1000) );
}


