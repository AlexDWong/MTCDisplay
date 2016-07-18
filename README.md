# MTCDisplay
A simple MIDI Time Code (MTC) visual display using an Arduino Uno (ATMega328p) and a MAX7219.

MTCDisplay accepts input from a MIDI Timecode stream and displays it on a large, 7-segment display. The displays are driven and controlled by a MAX7219 driver, enabling the Arduino to focus on decoding the MIDI Timecode stream. MTCDisplay supports all four MTC framerates (24/25/29.97 drop-frame/30 frames/s), and the design can be scaled up to support almost any 7-segment, common-cathode LED display. Future work will attempt to integrate:

* MIDI Thru capability, enabling MTCDisplay to daisy-chain the timecode stream to other devices
* Visual indication of framerate
* A custom PCB and enclosure
* Linear Timecode (LTC) capability

##"What's (MIDI) Timecode?"

Timecode is a way to synchronize equipment with each other.; for instance, an audio track could output a timecode stream to a lighting console so that lighting cues are triggered at exactly the right moment. [MIDI Timecode (MTC)](https://en.wikipedia.org/wiki/MIDI_timecode) is timecode transmitted between devices using the MIDI protocol and interface, and is based off [SMPTE Timecode](https://en.wikipedia.org/wiki/SMPTE_timecode). 

You can read more about the MIDI protocol on [Wikipedia](https://en.wikipedia.org/wiki/MIDI) or on the o[fficial MIDI Association website](https://www.midi.org/). 

##MIDI Timecode

MIDI Timecode follows the SMPTE standard for timecode. Each timecode is represented as four 8-bit unsigned integers:
* Hours (hh)
* Minutes (mm)
* Seconds (ss)
* Frames (ff)

Hours, minutes and seconds are self-explanatory. Frames are sub-divisions of seconds, originally referring to a frame of film as SMPTE timecode was originally developed for film use. There are several different framerates, and as a result MIDI Timecode must also transmit the framerate to ensure all devices are kept in sync.

MIDI Timecode messages can take one of two forms:

###Quarter-frame Timecode

Quarter-frame Timecode messages are used when the timecode stream is running. Each hour, minute, second or frame byte is broken down into two 4-bit values and attached to a 3-bit identifier. Before each piece is sent, it is preceded by a single status byte (0xF1).

| Piece |  ID #  |  Data  |  Full Byte  | Description         |
|:-----:|:------:|:------:|:-----------:|---------------------|
| 0     | `0000` | `ffff` | `0000 ffff` | Frame number lsbits |
| 1     | `0001` | `000f` | `0001 000f` | Frame number msbit  |
| 2     | `0010` | `ssss` | `0010 ssss` | Seconds lsbits      |
| 3     | `0011` | `00ss` | `0011 00ss` | Seconds msbits      |
| 4     | `0100` | `mmmm` | `0100 mmmm` | Minutes lsbits      |
| 5     | `0101` | `00mm` | `0101 00mm` | Minutes msbits      |
| 6     | `0110` | `hhhh` | `0110 hhhh` | Hour lsbits         |
| 7     | `0111` | `0rrh` | `0111 0rrh` | Rate and hour msbit |

**EX:** Timecode = 01:02:03:04, framerate = 30 frames/s

Quarter-frame Timecode = `F1 04 F1 10 F1 23 F1 30 F1 42 F1 50 F1 61 F1 76`

*Source: [Wikipedia](https://en.wikipedia.org/wiki/MIDI_timecode)*

###Full Timecode

If the timecode stream suddenly jumps (i.e. jumping ahead in an audio track) a full timecode is sent so that all equipment can resync. Full timecode messages are 10 bytes long, and once transmitted the MTC stream reverts back to quarter-frame messages; full timecode messages are structured as follows:
	`F0 7F 7F 01 01 hh mm ss ff F7`

