#pragma once
/*
** LCD_4X20.H -- 4x20 LCD Routines
**
** Version 1.00, 05-17-96
**
*/

#ifndef __LCD_H
#define __LCD_H

short _stdcall Inp32(short PortAddress);
void _stdcall Out32(short PortAddress, short data);

void LCD_Init (void);
void LCD_DefineChar (char address, const unsigned char *pattern);
void LCD_DisplayOff (void);
void LCD_DisplayOn (void);
void LCD_Clear (void);
void LCD_Home (void);
void LCD_Cursor (char row, char column);
void LCD_CursorLeft (void);
void LCD_CursorRight (void);
void LCD_CursorOn (void);
void LCD_CursorOff (void);
void LCD_DisplayCharacter (char a_char);
void LCD_DisplayString (char row, char column, char *string);
void LCD_DisplayStringCentered (char row, char *string);
void LCD_DisplayScreen (char *ptr);
void LCD_DisplayRow (char row, char *string);
void LCD_WipeOnLR (char *ptr);
void LCD_WipeOnRL (char *ptr);
void LCD_WipeOffLR (void);
void LCD_WipeOffRL (void);

#endif
