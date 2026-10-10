================================================================================
                    			GRAND MANEUVER
================================================================================

DEVELOPERS / TEAM MEMBERS
-------------------------
- Muhammad Shahwaiz Abbasi
- Samat Chowdhury

--------------------------------------------------------------------------------
REQUIREMENTS
--------------------------------------------------------------------------------
 Hardware:
   - DE10-Lite FPGA Board
   - VGA Cable and compatible monitor

--------------------------------------------------------------------------------
HOW TO EXECUTE AND RUN
--------------------------------------------------------------------------------
1. Open your terminal application.
2. Navigate to your project directory:
     cd Documents/project

3. Compile the program source code:
     make

4. Verify board connection via JTAG:
     jtagconfig

5. Program and upload the compiled file to the board:
     dtekv-run main.bin


--------------------------------------------------------------------------------
GAME CONTROLS 
--------------------------------------------------------------------------------
- SW1         : Accelerate / Drive Forward
- SW2         : Rotate Car 90° Clockwise
- SW0	         : Reset current level
- SW8-SW9     : Select starting level (set mode and press BTN1 while SW0 is down):
                * SW8 ON  SW9 OFF   01 (Mode 1) -> Level 1
                * SW8 OFF SW9 ON    10 (Mode 2) -> Level 2
                * SW8 ON  SW9 ON    11 (Mode 3) -> Level 3

--------------------------------------------------------------------------------
HOW TO PLAY
--------------------------------------------------------------------------------
1. Select a level or start on Level 1.
2. Drive the car across the screen and manoeuvre through obstacles such as trees and walls.
3. Park inside the designated parking spot before the timer on the 7 segment display runs down to 0.
4. Complete all 3 levels to view your total elapsed time output on the terminal.
================================================================================