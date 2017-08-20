#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

//Pitch and in Tune Libraries-----------------------------------------------------------------
#include "pitch_utils.h"
#include "note_name.h"
#include "tuned_note.h"
//--------------------------------------------------------------------------------------
#define I0(A0);


//Define Variables
AudioAnalyzeNoteFrequency notefreq;
AudioInputAnalog          adc1(A2);
AudioMixer4               mixer;
AudioOutputUSB            usb1;

//---------------------------------------------------------------------------------------
AudioConnection patchCord0(adc1, 0, notefreq, 0);
AudioConnection patchCord1(adc1, 0, usb1, 0 );



int channel = 1; // Defines the MIDI channel to send messages on (values from 1-16)
int velocity = 100; // Defines the velocity that the note plays at (values from 0-127)
int noteValue;  // Defines what note value to send to the midi channel

//---------------------------------------------------------------------------------------

// Global access
std::array<float, 88> pitch_freqs;
std::array<note_name *, 88> pitch_names;
//


void setup() {
  // put your setup code here, to run once:
 AudioMemory(30);
 notefreq.begin(.15);

 pinMode(1, OUTPUT);
 pinMode(2, OUTPUT);
 pinMode(3, OUTPUT);
 pinMode(4, OUTPUT);
 pinMode(6, OUTPUT);
 pinMode(7, OUTPUT);
 pinMode(9, OUTPUT);
 pinMode(10, OUTPUT);
  pinMode(5, OUTPUT);
 
 

 //usbMIDI.begin(1);

 // Pitch detection: gets pitch frequencies and outputs the
 // the range of freqnuencies closes to that of the 88
 // keys on a keyboard
  float reference = 440.0;
  pitch_freqs = get_pitch_freqs(reference);
  pitch_names = get_pitch_names();
 
}

void loop() {
  //Add Delay to not get a million values
  delay(100);

//**********************************************************
  //tested code without violin 
  //move back down to where it says 


  //float displacement = 5;
  //float cents = 10;
  //Serial.printf("cents = %3.2f", cents);
 
  
  //digitalWrite(5, HIGH);

  



//***********************************************************************
  




  
  // put your main code here, to run repeatedly:
   if (notefreq.available()) {
     float note = notefreq.read();
     float prob = notefreq.probability();

     noteValue = 69 + 12*log(note/440);


    
     //noteValue = 5;
 

      tuned_note n = freq_to_note(note, pitch_freqs);

  //get the pitch value   
    int index = n.getPitch();
  //see how far off pitch the value is
   // double distance = n.getDistance();

  // get the closest note value
    note_name closest_note = *pitch_names[index];

  // converted note frequency
  float i = (index-48)/12.0;
  float s = pow(2.0, i);
  float newFreq = s*440.0;


   //Check if note is valid
   
   Serial.printf("Note: %3.2f | Probability: %.2f \n", note, prob);
   Serial.printf("Midi Note: %i\n",noteValue);
   Serial.printf("closest tuned note %c , %c\n", closest_note.getName(), closest_note.getModifier());

     // Send correct note to through the midi channel
  usbMIDI.sendNoteOn(noteValue,velocity,channel);   // Turn the note ON
  usbMIDI.sendNoteOff(noteValue,velocity,channel);  // Turn the note OFF
  Serial.printf("index: %i \n", index);
  Serial.printf("i: %3.2f \n", i);
  Serial.printf("s: %3.2f \n", s);
  Serial.printf("converted: %3.2f \n", newFreq);
  tone(8, newFreq, 300);

  //Display that how off the person is USING LEDS

  Default_LED_Off();

  float displacement = note - newFreq;
  float cents = 1200 * (log(newFreq/note)) / (log(2)); 
  int direction;

  Serial.printf("Displacement: %3.2f \n", displacement);
  Serial.printf("Cents: %3.2f \n", cents);
  //put cents here 

  if (cents <= 10 && cents >= -10){
    // they are hitting the right note or close enough
    //turn green led on
    digitalWrite(5, HIGH);
    Serial.printf("high");
 
  }
  
    else if (-20 <= cents  && cents < -10){
        digitalWrite(4, HIGH);
    }

    else if (-30 <= cents  && cents < -20){
       digitalWrite(4, HIGH);
       digitalWrite(3, HIGH);
    }

    else if (-40 <= cents  && cents < -30){
       digitalWrite(4, HIGH);
       digitalWrite(3, HIGH);
       digitalWrite(2, HIGH);
       
    }

    else if (-50 <= cents  && cents < -40){
       digitalWrite(4, HIGH);
       digitalWrite(3, HIGH);
       digitalWrite(2, HIGH);
       digitalWrite(1, HIGH);       
    }
    

    
    else if (10 <= cents  && cents < 20){
       digitalWrite(6, HIGH);
    }

    else if (20 <= cents  && cents < 30){
       digitalWrite(6, HIGH);
       digitalWrite(7, HIGH);
    }

    else if (30 <= cents  && cents < 40){
       digitalWrite(6, HIGH);
       digitalWrite(7, HIGH);
       digitalWrite(9, HIGH);
    }

    else if (40 <= cents  && cents < 50){
       digitalWrite(6, HIGH);
       digitalWrite(7, HIGH);
       digitalWrite(9, HIGH);
       digitalWrite(10, HIGH);
    }
    


  
 }
}


void Default_LED_Off(){
       digitalWrite(10, LOW);
       digitalWrite(9, LOW);
       digitalWrite(7, LOW);
       digitalWrite(6, LOW); 
       digitalWrite(5, LOW);
       digitalWrite(4, LOW);
       digitalWrite(3, LOW);
       digitalWrite(2, LOW);
       digitalWrite(1, LOW); 
}

