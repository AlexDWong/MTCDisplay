#PCB Files

Enclosed here are the files needed to have a PCB manufactured, as well as a Bill of Materials.

## Important Note:
The LED displays do not need a current-limiting resistor; instead, the current is set by an external pullup resistor on the MAX7219. If you choose to use a different 7-segment display, then you may need to select a different value for R12 depending on the per-segment forward voltage and foward current. The [MAX7219 datasheet (PDF)](https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf) contains a table with approximate resistance values for a range of voltages and currents; when in doubt, a higher resistance is better.

## Versions
### Version 1.1
Resolved several issues:
* All capacitors have been replaced with 0805 SMD ceramic variants to reduce board space.
* Board reduced in size slightly.
* Rearranged components for easier hand-soldering.
* LCD display package has been corrected; displays now protrude slightly over the edge of the board instead of being inset 1/4" from the edge.
* MIDI OUT/THRU switch removed; no plans to add MTC output at this time.
* Fixed silkscreen part names and text placement.
* Added 0603 LED for power indication.
* Added 0.100", 2x3 ICSP header to program Arduino directly.
* Added diode to RESET signal for ICSP support.
* Added reverse-polarity and over-current protection to input.

### Version 1.0
Initial release. This version has been tested and is proven to work as described. Only Gerber files and a BOM are included.
