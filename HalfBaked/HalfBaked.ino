
//#define ORIGINAL_AUDIOSYNTHWAVEFORM
#include <Audio.h>
#include <Wire.h>
#include <SD.h>

float gain = 0, freq = 100.0;
int range = 200, offset = 500, sw = 0;

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
#endif

AudioMixer4         mixer;

// Create Audio connections between the components
//
AudioConnection c1(triangle, 0, mixer, 0); // triangle output 0 to mixer input 0
AudioConnection c2(noise, 0, mixer, 1);
AudioConnection c3(mixer, 0, audioOutput, 0); // mixer output 0 to DAC input 0


void setup() {
  Serial.begin(115200);
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(12);
  
  delay(5000);
  
  pinMode( 7,INPUT );
  pinMode( 8,INPUT );

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
  sine.begin( 0.5,777.7,TONE_TYPE_SINE );
#endif
  // both mixer channels off, please
  mixer.gain(0,0);
  mixer.gain(1,0);

  
}

void loop() {
    gain = analogRead(A0) / 1024.0; // scales gain to 0...1
    sine.amplitude(gain);
    // frequency scaling with pots
    // freq = (analogRead(A1) / 1024.0) * analogRead(A2) + analogRead(A3);
    // frequency scaling with a switch
    sw = digitalRead(7); // 0 or 1
    offset = digitalRead(8);
    if( sw ) { range = 500; } else { range = 200; };
    if( offset ) { offset = 1000; } else { offset = 500; };
    freq = (analogRead(A1) / 1024.0) * range + offset;
    sine.frequency(freq);
}


