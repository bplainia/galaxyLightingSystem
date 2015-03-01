State Machines
===============

\tableofcontents
\section daynight_sm The Day/Night State Machine
There should be a picture here of the day/night state machine.

\section mmode_sm Maintainence Mode State Machine
 ![The Maintainence Mode State Machine](../images/mmode-sm.png)
\subsection standbymode Mode 0: Standby
In this state, the state machine will process several things.
1. Wait for address and data in an interrupt routine
2. Wait for a keypress from the keypad in an interrupt routine
3. Check if the USB is plugged in.
\subsection mastermode Modes 1 and 3: Master Mode
In these states, the machine will check for commands in the command buffer that come from the keypad or usb, depending on the state.
\subsection slavemode Mode 2: Slave Mode
Wait for commands and execute them. Will trade the "token" with the master/controller.
