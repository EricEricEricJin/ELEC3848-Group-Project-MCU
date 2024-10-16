#include <Arduino.h>

extern "C"
{
// Fuck CPP
#include "mecanum.h"
#include "log.h"
}

void setup()
{
    // put your setup code here, to run once:
    log_i("THIS IS A LOG");
}

void loop()
{
    // put your main code here, to run repeatedly:
}
