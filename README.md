# USING TD 1.62 BETA 3 WITH DEV BOARD 4 OR 5
## ***** WARNING: AT THIS TIME ONLY ARDUINO 1.8.19 IS WORKING *****

1. Install arduino-1.8.19 and then install TD1.62B3.
2. Download this repo and install it the "Arduino" folder. It will then look like "Arduino/hardware".
3. From "arduino-1.8.19/hardware", copy the tools folder to "Arduino/hardware".
4. Change to directory "Arduino/hardware/Files_In_This_Folder_Are_Copied_To_cores_teensy4".
5. Copy the three files in the above directory to "arduino-1.8.19-TD1.62B3/hardware/teensy/avr/cores/teensy4".
6. The three modified files,"core_pins.h", "digital.c" and "pwm.c", will overwrite the existing files.So make sure you back them up.

All that was changed in the three files was the extention of pin numbers defined and processed (pins 55 to 69). 


