#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DisplayText.h>

DisplayText::DisplayText(LiquidCrystal_I2C *lcd, Keypad *keypad)
{
    this->lcd = lcd;
    this->keypad = keypad;
}

DisplayText::~DisplayText()
{
    this->lcd->clear();
    this->lcd->backlight();
    this->lcd->off();
}

char *DisplayText::getTextData()
{
    // Serial print whole text array
    // for (int i = 0; i < 16; i++)
    // {
    //     Serial.print(this->text[i][0]);
    // }
    // for (int i = 0; i < 16; i++)
    // {
    //     Serial.print(this->text[i][1]);
    // }
    return (char *)this->text;
}

char *DisplayText::formatTextData(char* text)
{
    char *result = (char *)malloc(32 * sizeof(char));
    for (int i = 0; i < 16; i++)
    {
        result[i] = this->text[i][0];
    }
    for (int i = 0; i < 16; i++)
    {
        result[i + 16] = this->text[i][1];
    }
    return result;
}

void DisplayText::addChar(char c)
{
    if (this->textIndex < 16)
    {
        this->text[this->textIndex][0] = c;
        this->textIndex++;
    }
    else if (this->textIndex < 32)
    {
        this->text[this->textIndex - 16][1] = c;
        this->textIndex++;
    }
    DisplayText::print();
}

void DisplayText::addChar(char c, int index, int line)
{
    if (line == 1)
    {
        this->text[index][0] = c;
    }
    else if (line == 2)
    {
        this->text[index][1] = c;
    }
    DisplayText::print();
}

void DisplayText::removeChar()
{
    if (this->textIndex > 0)
    {
        this->textIndex--;
        if (this->textIndex < 16)
        {
            this->text[this->textIndex][0] = '\0';
        }
        else if (this->textIndex < 32)
        {
            this->text[this->textIndex - 16][1] = '\0';
        }
    }
    DisplayText::print();
}

void DisplayText::removeChar(int index, int line)
{
    if (line == 1)
    {
        this->text[index][0] = '\0';
    }
    else if (line == 2)
    {
        this->text[index][1] = '\0';
    }
    DisplayText::print();
}

void DisplayText::clear()
{
    this->textIndex = 0;
    for (int i = 0; i < 16; i++)
    {
        this->text[i][0] = '\0';
        this->text[i][1] = '\0';
    }
    DisplayText::print();
    this->lcd->setCursor(0, 0);
}
void DisplayText::clear(int line)
{
    if (line == 1)
    {
        for (int i = 0; i < 16; i++)
        {
            this->text[i][0] = '\0';
        }
        this->lcd->setCursor(0, line - 1);
    }
    else if (line == 2)
    {
        for (int i = 0; i < 16; i++)
        {
            this->text[i][1] = '\0';
        }
        this->lcd->setCursor(0, line - 1);
    }
    DisplayText::print();
}

void DisplayText::print()
{
    this->lcd->clear();
    this->lcd->setCursor(0, 0);
    for (int i = 0; i < 16; i++)
    {
        if (this->text[i][0] != '\0')
        {
            this->lcd->print(this->text[i][0]);
        }
        else
        {
            break;
        }
    }
    this->lcd->setCursor(0, 1);
    for (int i = 0; i < 16; i++)
    {
        if (this->text[i][1] != '\0')
        {
            this->lcd->print(this->text[i][1]);
        }
        else
        {
            break;
        }
    }
    DisplayText::FixCursor();
}

void DisplayText::FixCursor()
{
    if (this->textIndex < 16)
    {
        this->lcd->setCursor(this->textIndex, 0);
    }
    else if (this->textIndex < 32)
    {
        this->lcd->setCursor(this->textIndex - 16, 1);
    }
}

void DisplayText::print(const char *text)
{
    // Checking length of text
    // if(strlen(text) > 16) {DisplayText::Error("Text is too big"); return;}

    this->lcd->clear();
    this->lcd->setCursor(0, 0);
    this->lcd->print(text);
}

void DisplayText::print(const char *text, bool clear)
{
    // if(strlen(text) > 16) {DisplayText::Error("Text is too big"); return;}

    if (clear)
        this->lcd->clear();

    this->lcd->setCursor(0, 0);
    this->lcd->print(text);
}

void DisplayText::print(const char *text, int line)
{
    // if(strlen(text) > 16) {DisplayText::Error("Text is too big"); return;}

    if (line == 1)
    {
        this->lcd->setCursor(0, 0);
    }
    else if (line == 2)
    {
        this->lcd->setCursor(0, 1);
    }
    this->lcd->print(text);
}

void DisplayText::print(const char *text, int line, bool clear)
{
    // if(strlen(text) > 16) {DisplayText::Error("Text is too big"); return;}
    if (clear)
        this->lcd->clear();

    if (line == 1)
    {
        this->lcd->setCursor(0, 0);
    }
    else if (line == 2)
    {
        this->lcd->setCursor(0, 1);
    }
    this->lcd->print(text);
}

void DisplayText::Error(const char *text, int timeout)
{
    this->lcd->clear();
    this->lcd->setCursor(0, 0);
    this->lcd->print("Error:");
    this->lcd->setCursor(0, 1);
    this->lcd->print(text);
    delay(timeout);
    this->lcd->clear();
    this->lcd->setCursor(0, 0);
    DisplayText::print();
}

/* ------------------ */

char DisplayText::selectOption(const char options[], char stopKey)
{
    this->optionSelectMode = true;
    this->lcd->cursor();
    short unsigned int optionsSize = strlen(options);
    DisplayText::print("Select options:", true);
    char *optionText = (char *)malloc(16 * sizeof(char));
    for (unsigned int i = 0; i < optionsSize; i++)
    {
        sprintf(optionText, "%s%u%c%c ", optionText, i + 1, 126, options[i]);
        // Serial.println(optionText);
    }
    DisplayText::print(optionText, 2);
    free(optionText);
    // this->lcd->noBlink();
    while (this->optionSelectMode)
    {
        const char key = this->keypad->getKey();
        if (key)
        {
            // Serial.println(key);
            // Serial.println(!isnan(key));
            // Serial.println((unsigned short)(key - '0'));
            if (!isnan(key) && (unsigned short)(key - '0') <= optionsSize)
            {
                this->optionSelectMode = false;
                return options[key - '0' - 1];
                break;
            }
            else if (key == stopKey)
            {
                this->optionSelectMode = false;
                DisplayText::print();
                break;
            }
        }
    }
}