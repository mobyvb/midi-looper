#include <MIDI.h>
#include <Bounce.h>

#define BUTTON_PIN 7
#define POT_PIN A0

MIDI_CREATE_DEFAULT_INSTANCE();
Bounce buttonBouncer = Bounce(BUTTON_PIN, 5);
bool isRecording = false;
unsigned long lastPotRead = 0;

const int bufferSize = 1024;
uint8_t buffer[bufferSize][3];
int bufferIndex = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
}

void loop() {
  MIDI.read();
  buttonBouncer.update();
  if (buttonBouncer.fell() && !isRecording) {
    bufferIndex = 0;
    isRecording = true;
  } else if (buttonBouncer.fell() && isRecording) {
    isRecording = false;
  }

  if (isRecording) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  
  if (millis() - lastPotRead > 10) {
    int potValue = analogRead(POT_PIN);
    float playbackSpeed = potValue / 1023.0;
    waitAndSendMIDI(playbackSpeed);
    lastPotRead = millis();
  }
}

void handleNoteOn(byte channel, byte pitch, byte velocity) {
  MIDI.sendNoteOn(pitch, velocity, channel);
  if (isRecording) {
    buffer[bufferIndex][0] = 0x90 | channel;
    buffer[bufferIndex][1] = pitch;
    buffer[bufferIndex][2] = velocity;
    bufferIndex = (bufferIndex + 1) % bufferSize;
  }
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {
  MIDI.sendNoteOff(pitch, velocity, channel);
  if (isRecording) {
    buffer[bufferIndex][0] = 0x80 | channel;
    buffer[bufferIndex][1] = pitch;
    buffer[bufferIndex][2] = velocity;
    bufferIndex = (bufferIndex + 1) % bufferSize;
  }
}

void waitAndSendMIDI(float playbackSpeed) {
  for (int i = 0; i < bufferSize; i++) {
    delay((int)(playbackSpeed * 5));
    MIDI.send(buffer[i][0], buffer[i][1], buffer[i][2]);
  }
}