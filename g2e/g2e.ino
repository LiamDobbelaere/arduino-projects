#include "pitches.h"

#define PIEZO_PIN 9
#define LED_PIN_START 2
#define LED_PIN_END 8

float bpm = 118.0;
float measureLength = 60.0 / bpm * 4.0;
float stepLength = measureLength / 16.0;
unsigned short song[] = {
  NOTE_D6, 6,
  NOTE_F6, 6,
  NOTE_A6, 6,
  NOTE_D7, 20,
  NOTE_D7, 6,
  NOTE_A6, 6,
  NOTE_F6, 6,
  NOTE_D6, 80,
  NOTE_WAIT, 20,

  NOTE_D6, 6,
  NOTE_F6, 6,
  NOTE_A6, 6,
  NOTE_D7, 20,
  NOTE_D7, 6,
  NOTE_A6, 6,
  NOTE_F6, 6,
  NOTE_D6, 10,
  NOTE_E6, 10,
  NOTE_F6, 10,
  NOTE_G6, 10,
  NOTE_A6, 60,

  NOTE_F6, 6, 
  NOTE_A6, 6,
  NOTE_C7, 6,  
  NOTE_F7, 20,
  NOTE_F7, 6,
  NOTE_E7, 6,
  NOTE_C7, 6,
  NOTE_A6, 80,
  NOTE_WAIT, 20,

  NOTE_E6, 6,
  NOTE_A6, 6,
  NOTE_C7, 6,
  NOTE_E7, 20,
  NOTE_D7, 6,
  NOTE_A6, 6,
  NOTE_F6, 6,
  NOTE_D6, 80,
  NOTE_WAIT, 20,

  NOTE_D6, 6,
  NOTE_F6, 6,
  NOTE_A6, 6,
  NOTE_D7, 20,
  NOTE_D7, 6,
  NOTE_A6, 6,
  NOTE_F6, 6,
  NOTE_D6, 80,
  NOTE_WAIT, 20,

  NOTE_D6, 6,
  NOTE_F6, 6,
  NOTE_A6, 6,
  NOTE_D7, 20,
  NOTE_D7, 6,
  NOTE_A6, 6,
  NOTE_F6, 6,
  NOTE_D6, 10,
  NOTE_E6, 10,
  NOTE_F6, 10,
  NOTE_G6, 10,
  NOTE_A6, 60,

  NOTE_F6, 6, 
  NOTE_A6, 6,
  NOTE_C7, 6,  
  NOTE_F7, 20,
  NOTE_F7, 6,
  NOTE_E7, 6,
  NOTE_C7, 6,
  NOTE_A6, 80,
  NOTE_WAIT, 20,

  NOTE_E6, 6,
  NOTE_A6, 6,
  NOTE_C7, 6,
  NOTE_D7, 20,
  NOTE_D7, 6,
  NOTE_A6, 6,
  NOTE_F6, 6,
  NOTE_D6, 140+160,
  NOTE_WAIT, 20,  
  NOTE_WAIT, 160,

  // singing
  NOTE_D6, 40,
  NOTE_C6, 40,
  NOTE_A5, 40,
  NOTE_G5, 20,
  NOTE_F5, 20,

  NOTE_G5, 40,
  NOTE_WAIT, 1,
  NOTE_G5, 20,
  NOTE_F5, 10,
  NOTE_G5, 30,
  NOTE_A5, 20,
  NOTE_D5, 20,
  NOTE_F5, 20,
  
  NOTE_G5, 40,
  NOTE_WAIT, 1,
  NOTE_G5, 20,
  NOTE_F5, 10,
  NOTE_G5, 50,
  NOTE_D5, 20,
  NOTE_F5, 20,

  NOTE_G5, 20,
  NOTE_WAIT, 1,
  NOTE_G5, 20,
  NOTE_WAIT, 1,
  NOTE_G5, 20,
  NOTE_A5, 10,
  NOTE_G5, 30,
  NOTE_F5, 20,
  NOTE_D5, 20,
  NOTE_C5, 20,
  NOTE_D5, 120,
  NOTE_WAIT, 40,

  // more singing
  NOTE_D6, 40,
  NOTE_C6, 40,
  NOTE_A5, 40,
  NOTE_G5, 20,
  NOTE_F5, 20,

  NOTE_G5, 40,
  NOTE_WAIT, 1,
  NOTE_G5, 20,
  NOTE_F5, 10,
  NOTE_G5, 10,
  NOTE_WAIT, 1,  
  NOTE_G5, 20,
  NOTE_A5, 20,
  NOTE_D5, 20,
  NOTE_F5, 20,
  
  NOTE_G5, 20,
  NOTE_WAIT, 1,
  NOTE_G5, 20,
  NOTE_WAIT, 1,
  NOTE_G5, 20,
  NOTE_F5, 10,
  NOTE_G5, 50,
  NOTE_D5, 20,
  NOTE_F5, 20,

  NOTE_G5, 20,
  NOTE_WAIT, 1,
  NOTE_G5, 20,
  NOTE_WAIT, 1,
  NOTE_G5, 20,
  NOTE_A5, 10,
  NOTE_G5, 30,
  NOTE_F5, 20,
  NOTE_D5, 20,
  NOTE_C5, 20,
  NOTE_D5, 120,
  NOTE_WAIT, 60,

  // not fair
  NOTE_E5, 20,
  NOTE_WAIT, 1,
  NOTE_E5, 20,
  NOTE_WAIT, 1,
  NOTE_E5, 20,
  NOTE_WAIT, 1,
  NOTE_E5, 25,
  NOTE_F5, 25,
  NOTE_G5, 100,
  NOTE_WAIT, 100,

  NOTE_E5, 20,
  NOTE_WAIT, 1,
  NOTE_E5, 20,
  NOTE_WAIT, 1,
  NOTE_E5, 20,
  NOTE_WAIT, 1,
  NOTE_E5, 25,
  NOTE_F5, 25,
  NOTE_G5, 185,

  // arpeggio
  NOTE_F6, 10,
  NOTE_E6, 10,
  NOTE_D6, 10,
  NOTE_A5, 10,
  NOTE_F6, 10,
  NOTE_E6, 10,
  NOTE_D6, 10,
  NOTE_A5, 10,
  NOTE_F6, 10,
  NOTE_E6, 10,
  NOTE_D6, 10,
  NOTE_A5, 10,
  NOTE_F6, 10,
  NOTE_E6, 10,
  NOTE_D6, 10,
  NOTE_A5, 10,

  NOTE_E6, 10,
  NOTE_D6, 10,
  NOTE_CS6, 10,
  NOTE_A5, 10,
  NOTE_E6, 10,
  NOTE_D6, 10,
  NOTE_CS6, 10,
  NOTE_A5, 10,
  NOTE_E6, 10,
  NOTE_D6, 10,
  NOTE_CS6, 10,
  NOTE_A5, 10,
  NOTE_E6, 10,
  NOTE_D6, 10,
  NOTE_CS6, 10,
  NOTE_A5, 10,

  // gimme
  NOTE_D5, 20,
  NOTE_F5, 20,
  NOTE_D5, 20,
  NOTE_F5, 20,
  NOTE_D5, 20,
  NOTE_F5, 20,
  NOTE_A5, 20,
  NOTE_G5, 40,
  NOTE_F5, 20,
  NOTE_D5, 20,
  NOTE_F5, 40,
  NOTE_D5, 40,

  NOTE_G5, 40,
  NOTE_WAIT, 1,
  NOTE_G5, 20,
  NOTE_F5, 20,
  NOTE_D5, 20,
  NOTE_F5, 20,
  NOTE_D5, 20,
  NOTE_WAIT, 1,
  NOTE_D5, 20,
  NOTE_WAIT, 1,
  NOTE_D5, 20,
  NOTE_C5, 20,
  NOTE_A4, 20,
  NOTE_F5, 20,
  NOTE_D5, 60,

  NOTE_D5, 20,
  NOTE_F5, 20,
  NOTE_D5, 20,
  NOTE_F5, 20,
  NOTE_D5, 20,
  NOTE_F5, 20,
  NOTE_A5, 20,
  NOTE_G5, 40,
  NOTE_F5, 20,
  NOTE_D5, 20,
  NOTE_F5, 40,
  NOTE_D5, 40,

  NOTE_G5, 40,
  NOTE_WAIT, 1,
  NOTE_G5, 20,
  NOTE_F5, 20,
  NOTE_D5, 20,
  NOTE_F5, 20,
  NOTE_D5, 20,
  NOTE_WAIT, 1,
  NOTE_D5, 20,
  NOTE_WAIT, 1,
  NOTE_D5, 20,
  NOTE_C5, 20,
  NOTE_A4, 20,
  NOTE_F5, 20,
  NOTE_D5, 60,
};

// TODO: make LEDs dance along with the song

int songLength = sizeof(song) / sizeof(song[0]);

void setup() {
  randomSeed(analogRead(A0));
  /*delay(1000000);*/
  /*
  for (int ledPin = LED_PIN_START; ledPin <= LED_PIN_END; ledPin++) {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
  }

  for (int ledPin = LED_PIN_START; ledPin <= LED_PIN_END; ledPin++) {
    int time = random(500 * (ledPin + 1));
    delay(time);

    digitalWrite(ledPin, HIGH);
  }*/

  pinMode(PIEZO_PIN, OUTPUT); 

  for (int i = 0; i < songLength; i+=2) {
    unsigned short noteToPlay = song[i];
    unsigned short noteDelay = song[i+1];
    float actualNoteDelay = stepLength * ((float) noteDelay / 10.0) * 1000.0;
    if (noteToPlay == 0) {
      noTone(PIEZO_PIN);   
      break;
    }

    if (noteToPlay != -1) {
      tone(PIEZO_PIN, noteToPlay);
    } else {
      noTone(PIEZO_PIN);  
    }

    delay(actualNoteDelay);
  }

  noTone(PIEZO_PIN);
}

void loop() {
}
