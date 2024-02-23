#include "dag-potentiometer.h"

DagPot::DagPot(uint8_t pin) : DagPot(pin, 0, 1023, 0) {}
// {
//     this->pin = pin;
//     pinMode(this->pin, INPUT);

//     set_minmax(0, 1023);
//     set_delta(0);

//     this->STORED = false;
//     this->CHANGED = false;
//     this->val = 0;
// }

DagPot::DagPot(uint8_t pin, float map_min, float map_max) : DagPot(pin, map_min, map_max, 0) {} 
// {
//     this->pin = pin;
//     pinMode(this->pin, INPUT);

//     set_minmax(map_min, map_max);
//     set_delta(0);

//     this->STORED = false;
//     this->CHANGED = false;
//     this->val = 0;
// }

DagPot::DagPot(uint8_t pin, float map_min, float map_max, float delta)
{
    this->pin = pin;
    pinMode(this->pin, INPUT);

    set_minmax(map_min, map_max);
    set_delta(delta);

    this->STORED = false;
    this->CHANGED = false;
    this->val = 0;
}

void DagPot::set_minmax(float map_min, float map_max)
{
    if (map_max < map_min)
    {
        this->min = map_max;
        this->max = map_min;
    }
    else
    {
        this->min = map_min;
        this->max = map_max;
    }
}

void DagPot::set_delta(float delta)
{
    this->dx = delta;
}

void DagPot::init(float map_min, float map_max)
{
    set_minmax(map_min, map_max);
}

void DagPot::init(float map_min, float map_max, float delta)
{
    set_minmax(map_min, map_max);
    set_delta(delta);
}

float DagPot::read()
{
    int raw = analogRead(this->pin);
    return map(raw, 0, 1023, this->min, this->max);
}

int DagPot::read_raw()
{
    return analogRead(this->pin);
}

float DagPot::value()
{
    return this->val;
}

void DagPot::save()
{
    this->val = read();
    this->STORED = true;
    this->CHANGED = false;
}

void DagPot::cancel()
{
    this->STORED = false;
    this->val = 0;
}

bool DagPot::changed()
{
    // se il valore è stato memorizzato e la differenza tra il valore attuale e il valore memorizzato è maggiore del delta
    this->CHANGED = (this->STORED && abs(read() - this->val) > this->dx)
                        ? true
                        : false;

    return this->CHANGED;
}

void DagPot::onChange(void (*fun)(void))
{
    onChange(fun, false);
}

void DagPot::onChange(void (*fun)(void), bool repeat)
{
    if (changed() && !FIRED)
    {
        FIRED = true;
        fun();

        if (LOOP)
        {
            FIRED = false;
        }
    }
    else
        return;
}