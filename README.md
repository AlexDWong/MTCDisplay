# MTCDisplay

![Front](/PCB/V1.1/Front.jpg)

## Description
MTCDisplay is an Arduino-based MIDI Timecode visual display that allows users to view a MIDI Timecode stream on an external LED display. The goal of this project was to create a simple display to enable lighting, audio, and video equipment operators to visually monitor a timecode stream without spending $400+ on a fully-integrated solution.

## Updates
**9/28/2016:** Board version 1.1; see directory for changes.

**8/8/2016:** Board version 1.0 proven.

## Features
* MIDI IN/THRU (DIN-5 connector)
  * 1x MIDI IN
  * Up to 3x MIDI THRU
* 12VDC input with polarity and over-current protection
* Supports all MTC framerates (24, 25, 29.97, 30)
* Mounting holes for custom enclosures and edge clearance for extruded aluminum enclosures

## Physical
* Board Dimensions: 5.00" x 3.35" (127mm x 85.09mm)
* Board Thickness: 1.6mm
* Input Power: +12VDC, 1A (max)
* Input Connections: 2.1mm DC barrel jack, MIDI (DIN-5)
* Output Connections: Up to 3x MIDI (DIN-5)
* Mounting: 4x 0.125" holes at edges of board, 0.1" clearance on sides for insertion into extruded aluminum case


## How it works
MTCDisplay accepts input from a MIDI Timecode stream and displays it on a large, 7-segment display. The displays are driven and controlled by a MAX7219 driver, enabling the Arduino to focus on decoding the MIDI Timecode stream. MTCDisplay supports all four MTC framerates (24/25/29.97 drop-frame/30 frames/s), and the design can be scaled up to support almost any 7-segment, common-cathode LED display.


### "What's (MIDI) Timecode?"

Timecode is a way to synchronize equipment with each other; for instance, an audio track could output a timecode stream to a lighting console so that lighting cues are triggered at exactly the right moment. [MIDI Timecode (MTC)](https://en.wikipedia.org/wiki/MIDI_timecode) is timecode transmitted between devices using the MIDI protocol and interface, and is based off [SMPTE Timecode](https://en.wikipedia.org/wiki/SMPTE_timecode). 

You can read more about the MIDI protocol on [Wikipedia](https://en.wikipedia.org/wiki/MIDI) or on the [official MIDI Association website](https://www.midi.org/). 

### MIDI Timecode

MIDI Timecode follows the SMPTE standard for timecode. Each timecode is represented as four 8-bit unsigned integers:
* Hours (hh)
* Minutes (mm)
* Seconds (ss)
* Frames (ff)

Hours, minutes and seconds are self-explanatory. Frames are sub-divisions of seconds, originally referring to a frame of film as SMPTE timecode was originally developed for film use. There are several different framerates, and as a result MIDI Timecode must also transmit the framerate to ensure all devices are kept in sync.

MIDI Timecode messages can take one of two forms:

#### Quarter-frame Timecode

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
| 7     | `0111` | `0rrh` | `0111 0rrh` | Hour msbit and rate |

**EX:** Timecode = 01:02:03:04, framerate = 30 frames/s

Quarter-frame Timecode = `F1 04 F1 10 F1 23 F1 30 F1 42 F1 50 F1 61 F1 76`

*Source: [Wikipedia](https://en.wikipedia.org/wiki/MIDI_timecode)*

#### Full Timecode

If the timecode stream suddenly jumps (i.e. jumping ahead in an audio track) a full timecode is sent so that all equipment can resync. Full timecode messages are 10 bytes long, and once transmitted the MTC stream reverts back to quarter-frame messages; full timecode messages are structured as follows:
	`F0 7F 7F 01 01 hh mm ss ff F7`
In it's current state, MTCDisplay only supports quarter-frame messages. If the timecode input jumps, the change will not be reflected on MTCDisplay until the next quarter-frame message is received.
