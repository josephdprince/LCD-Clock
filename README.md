# LCD-Clock
This project outputs the date (MM / DD / YYYY) and time (HH:MM:SS) onto an [LCD screen](https://www.digikey.com/en/products/detail/lumex-opto-components-inc/LCM-S01602DSR-A/469799). <br>
The date and time can be set using input from a [16-key keypad](https://www.digikey.com/en/products/detail/grayhill-inc/96BB2-006-R/180932)

# Keypad Key
A - Set the date <br>
B - Set the time <br>
D - toggle between 24 hour clock and 12 hour clock <br>

Number keys are used for inputting date or time.

# Schematic
This circuit is centered around the [Atmega32 microcontroller](https://www.digikey.com/en/products/detail/microchip-technology/ATMEGA32-16PU/739771) and is controlled using an [ATATMEL-ICE programmer](https://www.digikey.com/en/products/detail/microchip-technology/ATATMEL-ICE-BASIC/4753381). A 9V input is regulated into 5V using a [voltage regulator](https://www.digikey.com/en/products/detail/onsemi/MC7805CTG/919333), and an external 8 MHZ crystal is used instead of the on-board clock. Exact details of the schematic can be found below. 
![alt text](https://github.com/josephdprince/LCD-Clock/blob/main/P2%20Schematic-1.png)

# Usage
To upload C code to the programmer, follow these instructions: <br>
1. Download the [Microchip Studio](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio) IDE <br>
2. Create a new GCC C Executable Project <br>
3. Select the Atmega 32 <br>
4. After uploading code into Microchip Studio, press F7 to build <br>
5. Click Tools > Device Programmer and under device find Atmega 32 <br>
6. Read device signature (voltage source must be active and circuit assembled) <br>
7. Click Fuses and deselect HIGH.JTAGEN <br>
8. Scroll down and change item in LOW.SUT_CKSEL to external crystal/resonator with 16K ck + 64 ms <br>
9. Click Memories and then Program
