# include "stdafx.h"
# include "display.h"

/*
** LCD.C -- 4x20 LCD Control Routines
**
** Copyright 1996 by Randy Rasa
**                   rrasa@sky.net
**                   http://www.sky.net/~rrasa
**
** Version 1.00, 05-17-96
**
** These routines may be used in any way you see fit ... no strings attached.
**
*/

#include <string.h>
#include <dos.h>


/*
** Define user-defined character dot patterns
*/
const unsigned char LCD_CHAR_BAR1[] = {
  0x10,   // ---10000
  0x10,   // ---10000
  0x10,   // ---10000
  0x10,   // ---10000
  0x10,   // ---10000
  0x10,   // ---10000
  0x10,   // ---10000
  0x10    // ---10000
};
const unsigned char LCD_CHAR_BAR2[] = {
  0x18,   // ---11000
  0x18,   // ---11000
  0x18,   // ---11000
  0x18,   // ---11000
  0x18,   // ---11000
  0x18,   // ---11000
  0x18,   // ---11000
  0x18    // ---11000
};
const unsigned char LCD_CHAR_BAR3[] = {
  0x1c,   // ---11100
  0x1c,   // ---11100
  0x1c,   // ---11100
  0x1c,   // ---11100
  0x1c,   // ---11100
  0x1c,   // ---11100
  0x1c,   // ---11100
  0x1c    // ---11100
};
const unsigned char LCD_CHAR_BAR4[] = {
  0x1e,   // ---11110
  0x1e,   // ---11110
  0x1e,   // ---11110
  0x1e,   // ---11110
  0x1e,   // ---11110
  0x1e,   // ---11110
  0x1e,   // ---11110
  0x1e    // ---11110
};
const unsigned char LCD_CHAR_BAR5[] = {
  0x1f,   // ---11111
  0x1f,   // ---11111
  0x1f,   // ---11111
  0x1f,   // ---11111
  0x1f,   // ---11111
  0x1f,   // ---11111
  0x1f,   // ---11111
  0x1f    // ---11111
};
const unsigned char LCD_CHAR_UP_ARROW[] = {
  0x1f,   // ---11111
  0x1b,   // ---11011
  0x11,   // ---10001
  0x0a,   // ---01010
  0x1b,   // ---11011
  0x1b,   // ---11011
  0x1b,   // ---11011
  0x1f    // ---11111
};
const unsigned char LCD_CHAR_DOWN_ARROW[] = {
  0x1f,   // ---11111
  0x1b,   // ---11011
  0x1b,   // ---11011
  0x1b,   // ---11011
  0x0a,   // ---01010
  0x11,   // ---10001
  0x1b,   // ---11011
  0x1f    // ---11111
};
const unsigned char LCD_CHAR_TRADEMARK_T[] = {
  0x1f,   // ---11111
  0x04,   // ---00100
  0x04,   // ---00100
  0x04,   // ---00100
  0x00,   // ---00000
  0x00,   // ---00000
  0x00,   // ---00000
  0x00    // ---00000
};
const unsigned char LCD_CHAR_TRADEMARK_M[] = {
  0x11,   // ---10001
  0x1b,   // ---11011
  0x15,   // ---10101
  0x11,   // ---10001
  0x00,   // ---00000
  0x00,   // ---00000
  0x00,   // ---00000
  0x00    // ---00000
};


/*
** Local (Module-Level) Function Prototypes
*/
static void LCD_InitDriver (void);
static void LCD_WriteControl (unsigned char data);
static void LCD_WriteData (unsigned char data);



/* -------------------- High-Level LCD Routines ------------------------ */

/*
** LCD_Init: Initialize the LCD.
*/
void LCD_Init (void)
{
  LCD_InitDriver();
  LCD_Clear();
  LCD_CursorOff();
  
  //
  // load user-defined characters into LCD
  //
  LCD_DefineChar (0, LCD_CHAR_UP_ARROW);
  LCD_DefineChar (1, LCD_CHAR_DOWN_ARROW);
  LCD_DefineChar (2, LCD_CHAR_TRADEMARK_T);
  LCD_DefineChar (3, LCD_CHAR_TRADEMARK_M);
  LCD_DefineChar (4, LCD_CHAR_BAR1);
  LCD_DefineChar (5, LCD_CHAR_BAR2);
  LCD_DefineChar (6, LCD_CHAR_BAR3);
  LCD_DefineChar (7, LCD_CHAR_BAR4);
}


/*
** LCD_Clear: Clear the LCD screen (also homes cursor).
*/
void LCD_Clear (void)
{
  LCD_WriteControl(0x01);
}


/*
** LCD_Home: Position the LCD cursor at row 1, col 1.
*/
void LCD_Home (void)
{
  LCD_WriteControl(0x02);
}


/*
** LCD_DisplayCharacter: Display a single character,
**                       at the current cursor location.
*/
void LCD_DisplayCharacter (char a_char)
{
  LCD_WriteData (a_char);
}


/*
** LCD_DisplayString: Display a string at the specified row and column.
*/
void LCD_DisplayString (char row, char column, char *string)
{
  LCD_Cursor (row, column);
  while (*string)
    LCD_DisplayCharacter (*string++);
}


/*
** LCD_DisplayStringCentered: Display a string centered on the specified row.
*/
void LCD_DisplayStringCentered (char row, char *string)
{
  char len = strlen (string);

  if (len <= 20) {
    // if the string is less than one line, center it ...
    char i;
    LCD_Cursor (row, 1);
    for (i=0; i<20; i++)
      LCD_DisplayCharacter (' ');
    LCD_DisplayString(row,((20 - len) / 2)+1,string);
  }
  else {
    // if the string is more than one line, display first 20 characters
    char temp[21];
    strncpy(temp, string, 20);
    temp[20] = 0;
    LCD_DisplayString(row,1,temp);
  }
}


/*
** LCD_Cursor: Position the LCD cursor at "row", "column".
*/
void LCD_Cursor (char row, char column)
{
  switch (row) {
    case 1: LCD_WriteControl (0x80 + column - 1); break;
    case 2: LCD_WriteControl (0xc0 + column - 1); break;
    case 3: LCD_WriteControl (0x94 + column - 1); break;
    case 4: LCD_WriteControl (0xd4 + column - 1); break;
    default: break;
  }
}


/*
** LCD_DisplayScreen: Display an entire screen (80 characters).
**
**  inputs: ptr = pointer to a string containing the entire screen
**
**  example:
**    char screen[] = "01234567890123456789"
**                    " This is a test of  "
**                    "LCD_DisplayScreen()."
**                    "   How's it look?   ";
**    DisplayScreen(screen);
**
*/
void LCD_DisplayScreen (char *ptr)
{
  LCD_DisplayRow(1,ptr+ 0);
  LCD_DisplayRow(2,ptr+20);
  LCD_DisplayRow(3,ptr+40);
  LCD_DisplayRow(4,ptr+60);
}


/*
** LCD_WipeOnLR: Display an entire screen (80 characters) by
**               "wiping" it on (left to right).
**
**  inputs: ptr = pointer to a string containing the entire screen
**
*/
void LCD_WipeOnLR (char *ptr)
{
  // "wipe" on new screen
  char i;
  for (i=0; i<20; i++) {
    LCD_Cursor(1,i+1);
    LCD_DisplayCharacter(*(ptr+ 0+i));
    LCD_Cursor(2,i+1);
    LCD_DisplayCharacter(*(ptr+20+i));
    LCD_Cursor(3,i+1);
    LCD_DisplayCharacter(*(ptr+40+i));
    LCD_Cursor(4,i+1);
    LCD_DisplayCharacter(*(ptr+60+i));
  }
}


/*
** LCD_WipeOnLR: Display an entire screen (80 characters) by
**               "wiping" it on (right to left).
**
**  inputs: ptr = pointer to a string containing the entire screen
**
*/
void LCD_WipeOnRL (char *ptr)
{
  // "wipe" on new screen
  char i;
  for (i=20; i>0; i--) {
    LCD_Cursor(1,i);
    LCD_DisplayCharacter(*(ptr+ 0+i-1));
    LCD_Cursor(2,i);
    LCD_DisplayCharacter(*(ptr+20+i-1));
    LCD_Cursor(3,i);
    LCD_DisplayCharacter(*(ptr+40+i-1));
    LCD_Cursor(4,i);
    LCD_DisplayCharacter(*(ptr+60+i-1));
  }
}


/*
** LCD_WipeOffLR: "Wipe" screen left-to-right.
*/
void LCD_WipeOffLR (void)
{
  // "wipe" off old screen (left to right)
  char i;
  for (i=1; i<21; i++) {
    #define BLOCK 0xff
    LCD_Cursor(1,i);
    LCD_DisplayCharacter(BLOCK);
    LCD_Cursor(2,i);
    LCD_DisplayCharacter(BLOCK);
    LCD_Cursor(3,i);
    LCD_DisplayCharacter(BLOCK);
    LCD_Cursor(4,i);
    LCD_DisplayCharacter(BLOCK);
  }
}


/*
** LCD_WipeOffRL: "Wipe" screen right-to-left.
*/
void LCD_WipeOffRL (void)
{
  // "wipe" off old screen (right to left)
  char i;
  for (i=20; i>0; i--) {
    #define BLOCK 0xff
    LCD_Cursor(1,i);
    LCD_DisplayCharacter(BLOCK);
    LCD_Cursor(2,i);
    LCD_DisplayCharacter(BLOCK);
    LCD_Cursor(3,i);
    LCD_DisplayCharacter(BLOCK);
    LCD_Cursor(4,i);
    LCD_DisplayCharacter(BLOCK);
  }
}



/*
** LCD_DisplayRow: Display a string at the specified row.
*/
void LCD_DisplayRow (char row, char *string)
{
  char i;
  LCD_Cursor (row, 1);
  for (i=0; i<20; i++)
    LCD_DisplayCharacter (*string++);
}


/*
** LCD_CursorLeft: Move the cursor left by one character.
*/
void LCD_CursorLeft (void)
{
  LCD_WriteControl (0x10);
}


/*
** LCD_CursorRight: Move the cursor right by one character.
*/
void LCD_CursorRight (void)
{
  LCD_WriteControl (0x14);
}


/*
** LCD_CursorOn: Turn the cursor on.
*/
void LCD_CursorOn (void)
{
  LCD_WriteControl (0x0d);
}


/*
** LCD_CursorOff: Turn the cursor off.
*/
void LCD_CursorOff (void)
{
  LCD_WriteControl (0x0c);
}


/*
** LCD_DisplayOff: Turn Off LCD.
*/
void LCD_DisplayOff (void)
{
  LCD_WriteControl(0x08);
}


/*
** LCD_DisplayOn: Turn On LCD.
*/
void LCD_DisplayOn (void)
{
  LCD_WriteControl(0x0c);
}


/*
** LCD_DefineCharacter: Define dot pattern for user-defined character.
**
**  inputs: address = address of character (0x00-0x07)
**          pattern = pointer to 8-byte array containing the dot pattern
*/
void LCD_DefineChar (char address, const unsigned char *pattern)
{
  char i;

  LCD_WriteControl(0x40 + (address << 3));
  for (i=0; i<8; i++) {
    LCD_WriteData(*pattern++);
  }
}

/* -------------------- Low-Level LCD Routines ------------------------ */


/*
** Define addresses of LCD hardware.
**
**  LPT1 = 0x0378
**  LPT2 = 0x0278
*/
#define LPT                   0x0378
#define LCD_DATA_ADDRESS      LPT+0
#define LCD_CONTROL_ADDRESS   LPT+2


/*
** LCD_InitDriver: Initialize the LCD driver.
*/
static void LCD_InitDriver (void)
{
  LCD_WriteControl(0x38);
  LCD_WriteControl(0x38);
  LCD_WriteControl(0x38);
  LCD_WriteControl(0x06);
  LCD_WriteControl(0x0c);
}


/*
** LCD_WriteControl: Write a control instruction to the LCD
*/
static void LCD_WriteControl (unsigned char data)
{
  Out32(LCD_CONTROL_ADDRESS,0x03);  // RS=0, R/W=0, E=0
  
  Out32(LCD_DATA_ADDRESS,data);
  
  Out32(LCD_CONTROL_ADDRESS,0x02);  // RS=0, R/W=0, E=1
  Out32(LCD_CONTROL_ADDRESS,0x03);  // RS=0, R/W=0, E=0
  Out32(LCD_CONTROL_ADDRESS,0x01);  // RS=0, R/W=1, E=0

  Sleep(10);// 10ms delay
}


/*
** LCD_WriteData: Write one byte of data to the LCD
*/
static void LCD_WriteData (unsigned char data)
{
  Out32(LCD_CONTROL_ADDRESS,0x07);  // RS=1, R/W=0, E=0
  
  Out32(LCD_DATA_ADDRESS,data);
  
  Out32(LCD_CONTROL_ADDRESS,0x06);  // RS=1, R/W=0, E=1
  Out32(LCD_CONTROL_ADDRESS,0x07);  // RS=1, R/W=0, E=0
  Out32(LCD_CONTROL_ADDRESS,0x05);  // RS=1, R/W=1, E=0
  
  Sleep(1);                           // 1ms delay
}
