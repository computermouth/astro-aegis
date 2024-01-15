
#ifndef AA_KEYS_H
#define AA_KEYS_H

#include "raylib.h"

char * keys_keylist[349] = { 
    [KEY_NULL            ] = "NULL",        // Key: NULL, used for no key pressed
    // Alphanumeric keys ] 
    [KEY_APOSTROPHE      ] = "'",       // Key: '
    [KEY_COMMA           ] = ",",       // Key: ,
    [KEY_MINUS           ] = "-",       // Key: -
    [KEY_PERIOD          ] = ".",       // Key: .
    [KEY_SLASH           ] = "/",       // Key: /
    [KEY_ZERO            ] = "0",       // Key: 0
    [KEY_ONE             ] = "1",       // Key: 1
    [KEY_TWO             ] = "2",       // Key: 2
    [KEY_THREE           ] = "3",       // Key: 3
    [KEY_FOUR            ] = "4",       // Key: 4
    [KEY_FIVE            ] = "5",       // Key: 5
    [KEY_SIX             ] = "6",       // Key: 6
    [KEY_SEVEN           ] = "7",       // Key: 7
    [KEY_EIGHT           ] = "8",       // Key: 8
    [KEY_NINE            ] = "9",       // Key: 9
    [KEY_SEMICOLON       ] = ";",       // Key: ;
    [KEY_EQUAL           ] = "=",       // Key: =
    [KEY_A               ] = "A",       // Key: A | a
    [KEY_B               ] = "B",       // Key: B | b
    [KEY_C               ] = "C",       // Key: C | c
    [KEY_D               ] = "D",       // Key: D | d
    [KEY_E               ] = "E",       // Key: E | e
    [KEY_F               ] = "F",       // Key: F | f
    [KEY_G               ] = "G",       // Key: G | g
    [KEY_H               ] = "H",       // Key: H | h
    [KEY_I               ] = "I",       // Key: I | i
    [KEY_J               ] = "J",       // Key: J | j
    [KEY_K               ] = "K",       // Key: K | k
    [KEY_L               ] = "L",       // Key: L | l
    [KEY_M               ] = "M",       // Key: M | m
    [KEY_N               ] = "N",       // Key: N | n
    [KEY_O               ] = "O",       // Key: O | o
    [KEY_P               ] = "P",       // Key: P | p
    [KEY_Q               ] = "Q",       // Key: Q | q
    [KEY_R               ] = "R",       // Key: R | r
    [KEY_S               ] = "S",       // Key: S | s
    [KEY_T               ] = "T",       // Key: T | t
    [KEY_U               ] = "U",       // Key: U | u
    [KEY_V               ] = "V",       // Key: V | v
    [KEY_W               ] = "W",       // Key: W | w
    [KEY_X               ] = "X",       // Key: X | x
    [KEY_Y               ] = "Y",       // Key: Y | y
    [KEY_Z               ] = "Z",       // Key: Z | z
    [KEY_LEFT_BRACKET    ] = "[",       // Key: [
    [KEY_BACKSLASH       ] = "\\",       // Key: '\'
    [KEY_RIGHT_BRACKET   ] = "]",       // Key: ]
    [KEY_GRAVE           ] = "`",       // Key: `
    // Function keys
    [KEY_SPACE           ] = "SPACE" ,       // Key: Space
    [KEY_ESCAPE          ] = "ESCAPE",      // Key: Esc
    [KEY_ENTER           ] = "ENTER",      // Key: Enter
    [KEY_TAB             ] = "TAB",      // Key: Tab
    [KEY_BACKSPACE       ] = "BCKSPC",      // Key: Backspace
    [KEY_INSERT          ] = "INSERT",      // Key: Ins
    [KEY_DELETE          ] = "DELETE",      // Key: Del
    [KEY_RIGHT           ] = "RIGHT",      // Key: Cursor right
    [KEY_LEFT            ] = "LEFT",      // Key: Cursor left
    [KEY_DOWN            ] = "DOWN",      // Key: Cursor down
    [KEY_UP              ] = "UP",      // Key: Cursor up
    [KEY_PAGE_UP         ] = "PG_UP",      // Key: Page up
    [KEY_PAGE_DOWN       ] = "PG_DN",      // Key: Page down
    [KEY_HOME            ] = "HOME",      // Key: Home
    [KEY_END             ] = "END",      // Key: End
    [KEY_CAPS_LOCK       ] = "CAPS_LCK",      // Key: Caps lock
    [KEY_SCROLL_LOCK     ] = "SCRL_LCK",      // Key: Scroll down
    [KEY_NUM_LOCK        ] = "NUM_LCK",      // Key: Num lock
    [KEY_PRINT_SCREEN    ] = "PRNT_SCR",      // Key: Print screen
    [KEY_PAUSE           ] = "PAUSE",      // Key: Pause
    [KEY_F1              ] = "F1",      // Key: F1
    [KEY_F2              ] = "F2",      // Key: F2
    [KEY_F3              ] = "F3",      // Key: F3
    [KEY_F4              ] = "F4",      // Key: F4
    [KEY_F5              ] = "F5",      // Key: F5
    [KEY_F6              ] = "F6",      // Key: F6
    [KEY_F7              ] = "F7",      // Key: F7
    [KEY_F8              ] = "F8",      // Key: F8
    [KEY_F9              ] = "F9",      // Key: F9
    [KEY_F10             ] = "F10",      // Key: F10
    [KEY_F11             ] = "F11",      // Key: F11
    [KEY_F12             ] = "F12",      // Key: F12
    [KEY_LEFT_SHIFT      ] = "L_SHIFT",      // Key: Shift left
    [KEY_LEFT_CONTROL    ] = "L_CTRL",      // Key: Control left
    [KEY_LEFT_ALT        ] = "L_ALT",      // Key: Alt left
    [KEY_LEFT_SUPER      ] = "L_SUPER",      // Key: Super left
    [KEY_RIGHT_SHIFT     ] = "R_SHIFT",      // Key: Shift right
    [KEY_RIGHT_CONTROL   ] = "R_CTRL",       // Key: Control right
    [KEY_RIGHT_ALT       ] = "R_ALT",        // Key: Alt right
    [KEY_RIGHT_SUPER     ] = "R_SUPER",      // Key: Super right
    [KEY_KB_MENU         ] = "KB_MENU",      // Key: KB menu
    // Keypad keys
    [KEY_KP_0            ] = "KP_0",      // Key: Keypad 0
    [KEY_KP_1            ] = "KP_1",      // Key: Keypad 1
    [KEY_KP_2            ] = "KP_2",      // Key: Keypad 2
    [KEY_KP_3            ] = "KP_3",      // Key: Keypad 3
    [KEY_KP_4            ] = "KP_4",      // Key: Keypad 4
    [KEY_KP_5            ] = "KP_5",      // Key: Keypad 5
    [KEY_KP_6            ] = "KP_6",      // Key: Keypad 6
    [KEY_KP_7            ] = "KP_7",      // Key: Keypad 7
    [KEY_KP_8            ] = "KP_8",      // Key: Keypad 8
    [KEY_KP_9            ] = "KP_9",      // Key: Keypad 9
    [KEY_KP_DECIMAL      ] = "KP_.",      // Key: Keypad .
    [KEY_KP_DIVIDE       ] = "KP_/",      // Key: Keypad /
    [KEY_KP_MULTIPLY     ] = "KP_*",      // Key: Keypad *
    [KEY_KP_SUBTRACT     ] = "KP_-",      // Key: Keypad -
    [KEY_KP_ADD          ] = "KP_+",      // Key: Keypad +
    [KEY_KP_ENTER        ] = "KP_ENTER",      // Key: Keypad Enter
    [KEY_KP_EQUAL        ] = "KP_=",      // Key: Keypad =
    // Android key buttons
    // [KEY_BACK            ] = "BACK",        // Key: Android back button
    // [KEY_MENU            ] = "MENU",       // Key: Android menu button
    // [KEY_VOLUME_UP       ] = "VOL+",       // Key: Android volume up button
    // [KEY_VOLUME_DOWN     ] = "VOL-"        // Key: Android volume down button
};

#endif
