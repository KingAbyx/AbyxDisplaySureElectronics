# AbyxDisplaySureElectronics
Library for using an Arduino Uno or similar to send data to a Sure Electronics LED Matrix Display

# Using this Library:
 * Initializing:
  * #include <AbyxMatrix.h>
  * Then somewhere in your code you need create an object of this Library. Each port is given as a set of two Values, a char and an int.
    * The char Value is the port (Example: PORTB) given as a single Letter like 'B'
    * The int Value is the pin of that port given as a number, usually from 0 to 7
      * Example: If you want to have the port be PB4 then you would parse 'B' and PB4 "AbyxMatrix('B', PB4,"
 * Sending to Screen
   * This can be accomlished in two ways, by either using sendScreen or sendScreenSmart
    * sendScreen takes an array that has 48 uint16_t values and sends them all at once to the screen
    * sendScreenSmart takes two array with 48 uint16_t values each
      * Arrays
        * The first array (current) will be what is on Screen currently. Basically the last thing you sent to screen
        * The second array (newA) is what you want the screen to display next
      * It then compares both array and only sends the vertical lines that changed using sendPacket
      * This is faster for small changes on screen, but slower if most of the screen changes from frame to frame
 * initComands
  * initComands sends all the initializing
   * This will turn the display on, turn it to Master Mode, turn the LEDs on, sets it to full brightness and turns blink off
