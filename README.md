# Morse-Code-Interpreter
This project is an implementation of a Morse Code interpreter using an Arduino, an LCD display, and a button for input. The core idea revolves around translating button presses (long and short) into Morse code, which is then decoded into characters and displayed on the LCD.

Core Ideas Implemented:

1.  Morse Code Input: Button presses are used to simulate Morse code. A short press is interpreted as a dot ('.'), and a long press as a dash #\n ('-'). The code measures the duration of each button press to distinguish between dots and dashes.
   
2.  Binary Tree for Decoding: A binary tree structure is implemented to store Morse code characters. Each node in the tree represents a character, with left branches representing dots and right branches representing dashes. As the user inputs Morse code, the program traverses the tree to find and decode the corresponding character.

3. LCD Display: The decoded characters are displayed on a 16x2 LCD. The project makes use of the Arduino LiquidCrystal library to control the LCD and update the display in real-time as characters are decoded.

4. Button Debouncing: To ensure accuracy, software-based debouncing is applied. This prevents multiple readings from being triggered by mechanical button noise during presses. A simple debounce algorithm is used to verify stable input signals before processing them.

5. Timing Logic: The project uses the `millis()` function to measure the duration of button presses and distinguish between dots, dashes, and pauses (used to determine when a letter has finished and a new one begins).

6. Memory Management: The tree structure is kept in memory for continuous use without needing to recreate it for every input. The design avoids dynamic memory allocation issues and manages the tree without unnecessary allocations, keeping memory usage stable.

This project integrates a variety of embedded systems principles like real-time input processing, tree data structures for decoding, and button input debouncing, all while providing an interactive user interface through the LCD.
