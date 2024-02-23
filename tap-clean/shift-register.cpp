#include "shift-register.h"

ShiftRegister::ShiftRegister(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin)
{
    this->dataPin = dataPin;
    this->clockPin = clockPin;
    this->latchPin = latchPin;
}

void ShiftRegister::init(uint8_t initial_value = 0x00)
{
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    this->set(initial_value);
}

void ShiftRegister::write(uint8_t value)
{
    // Pull the latch pin low so the OUTPUT don't change while you're sending in bits
    digitalWrite(this->latchPin, LOW);

    // Send out the 8 bits
    shiftOut(this->dataPin, this->clockPin, MSBFIRST, value);

    // Pull the latch pin high to put the 8 bits on the output pins
    digitalWrite(this->latchPin, HIGH);
}

void ShiftRegister::set(byte value)
{
    this->value = value;
    this->write(value);
}

void ShiftRegister::setBit(uint8_t index, bool value)
{
    // If value is true, the code uses the bitwise OR operator (|=) to set the bit at the specified index to 1. It does this by creating a value where only the bit at the specified index is 1 (using 1 << index) and all other bits are 0, and then performing a bitwise OR with the current value of the shift register. This operation leaves all bits in this->value unchanged, except for the bit at the specified index, which is set to 1.
    if (value)
    {
        // bitwise OR
        this->value |= (1 << index);
    }

    // If value is false, the code uses the bitwise AND operator (&=) to set the bit at the specified index to 0. It does this by creating a value where only the bit at the specified index is 0 (using ~(1 << index)) and all other bits are 1, and then performing a bitwise AND with the current value of the shift register. This operation leaves all bits in this->value unchanged, except for the bit at the specified index, which is set to 0.
    else
    {
        // bitwise AND with the complement
        this->value &= ~(1 << index);
    }
    this->write(this->value);
}

void ShiftRegister::clear(void)
{
    this->value = 0x00;
    this->write(this->value);
}

void ShiftRegister::rise(void)
{
    this->value = 0xFF;
    this->write(this->value);
}

byte ShiftRegister::getValue(void)
{
    return this->value;
}

uint8_t ShiftRegister::join(uint8_t left, uint8_t right)
{
    this->value = (left << 4) | right;
    this->write(this->value);
    return this->value;
}