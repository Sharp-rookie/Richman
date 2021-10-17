#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "Game.hpp"
#include <fstream>

static int n_10(uint8_t n)
{
    int i = 1;
    for (int j = 0; j < n; j++)
    {
        i *= 10;
    }
    return i;
}