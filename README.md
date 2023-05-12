# MIDI Looper

This is a simple MIDI looper program that uses the Arduino R3-compatible SparkFun MIDI Shield. It allows you to record and playback MIDI while adjusting the playback speed using a potentiometer.

## Functionality

- Pipes MIDI input to MIDI output as it occurs
- Begins recording MIDI input when the button is pressed
- Stops recording when the button is pressed again
- After stopping recording, the recorded MIDI is repeatedly played back through the MIDI output 
- MIDI input continues to be sent to MIDI output, even during recording
- Potentiometer can be used to adjust playback speed

## Hardware

- One pushbutton
- One potentiometer
- MIDI input
- MIDI output