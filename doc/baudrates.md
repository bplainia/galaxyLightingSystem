Baud Rate for UARTs
====================

\section selection Baud Rate selection

In the following table, certain values of the baud rate generator and it's corresponding value for the 1/4 divider and 16 bit SPBRG enabled.

| SPBRG | Baud(k) | Comments                                           |
| ----- | ------- | ---------------------------------------------------|
| 31    | 500     | The maximum frequency                              |
| 39    | 400     |                                                    |
| 49    | 320     |                                                    |
| 63    | 250     | The current setting for the comm system            |
| 79    | 200     |                                                    |
| 124   | 128     |                                                    |
| 138   | 115.107 | Close to 115200 with .08% error.                   |
| 1535  | 10.416  | Close to 10417 with .003% error. Not common.       |
| 1666  | 9.598   | Close to 9600 baud. Common value. Needed for LCD.  |

For the communications items, it will have to come down to testing to figure out what baud rate will be chosen. It is assumed that the lower the baud, the futher it will be able to go.
Note that the maximum speed of the transmission chip is 500kbps.
