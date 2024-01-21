#ifndef DisplayText_h
#define DisplayText_h
#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
class DisplayText
{
private:
    char text[16][2];
    int textIndex = 0;
    LiquidCrystal_I2C *lcd;
    Keypad *keypad;
    bool optionSelectMode;
public:
    DisplayText(LiquidCrystal_I2C *lcd, Keypad *keypad);
    ~DisplayText();
    char* getTextData();
    char* formatTextData(char* text);
    void addChar(char c);
    void addChar(char c, int index, int line);
    void removeChar();
    void removeChar(int index, int line);
    void clear();
    void clear(int line);
    void print();
    void print(const char *text);
    void print(const char *text, bool clear);
    void print(const char *text, int line);
    void print(const char *text, int line, bool clear);
    void Error(const char *text, int timeout = 2000);
    void FixCursor();

    char selectOption(const char options[], char stopKey = '#');
};

#endif // DisplayText_h