#PCB Files

Enclosed here are the files needed to have a PCB manufactured, as well as a Bill of Materials. There are currently **two** revisions; **both revisions use identical parts and share the same BOM**:

##Version 1.0

This is the original MTCDisplay, and the design has been proven (i.e. assembled, tested and confirmed working). The folder contains Gerber files for the top/bottom silkscreens, copper layers, soldermasks, and drill file. There is also a file listing all non-plated through holes for specific fab houses which request it.

##Version 1.1

This is the current MTCDisplay, but the design **has not been proven**. The layout is identical to Version 1.0; however, the package for the 7-segment displays has been updated and the board slightly reduced in depth to allow the displays to be flush against the edge. The folder contains an Eagle schematic and board file.

#Optional Parts

* MIDI-THRU: MTCDisplay can read MIDI Timecode without requiring the MIDI-THRU ports; as a result, the total cost of components can be reduced by not populating the PCB with the following components:
  * THRU-1: U3, R2, R3
  * THRU-2: U4, R4, R5
  * THRU-3/OUT: U5, R6, R7 *(Note: In addition, if U5 is used only as a THRU and not an OUT then part S1 can be left off and replaced with a wire in the appropriate through-holes.)*
  * If no MIDI-THRU connections are needed, then the hex inverter (IC1) and capacitor C9 can be left unpopulated. However, if one or more MIDI-THRU connections are needed then IC1 and C9 **must** be included.
* 7-segment displays: MTCDisplay makes use of Avago HDSP-5523 7-segment displays. However, these displays can be substituted for other displays provided they meet several requirements:
  * Common-cathode design
  * Identical pinout (see [here](http://www.avagotech.com/products/leds-and-displays/7-segment/through-hole/hdsp-5523) for the HDSP-5523 datasheet)
  * The value of resistor R10 must be chosen to match the per-segment forward voltage and current for the new displays. See [Table 11 on page 11 of the MAX7219 datasheet](https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf) for more information. 
* DIP sockets may be excluded at yoru discression, but have been included in the BOM for prototyping purposes.
