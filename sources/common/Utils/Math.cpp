#include "Math.h"
#include "Settings/Settings.h"
#include "FPGA/FPGATypes.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifndef STEP_RSHIFT
#define STEP_RSHIFT 1
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Math math;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const float absStepRShift[] =
{
    2e-3f / 20 / STEP_RSHIFT,
    5e-3f / 20 / STEP_RSHIFT,
    10e-3f / 20 / STEP_RSHIFT,
    20e-3f / 20 / STEP_RSHIFT,
    50e-3f / 20 / STEP_RSHIFT,
    100e-3f / 20 / STEP_RSHIFT,
    200e-3f / 20 / STEP_RSHIFT,
    500e-3f / 20 / STEP_RSHIFT,
    1.0f / 20 / STEP_RSHIFT,
    2.0f / 20 / STEP_RSHIFT,
    5.0f / 20 / STEP_RSHIFT,
    10.0f / 20 / STEP_RSHIFT,
    20.0f / 20 / STEP_RSHIFT
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Math::LowSignedBit(uint value)
{
    int verValue = 1;

    for (int i = 0; i < 32; i++)
    {
        if (verValue & value)
        {
            return i;
        }

        verValue <<= 1;
    }

    return -1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Math::Smoothing(uint8 *data, int numPoints, int numSmooth)
{
    if (numSmooth == 0 || numSmooth == 1)
    {
        return;
    }

    float *buffer = (float *)malloc(numPoints * (int)sizeof(float));
    int  *num = (int *)malloc(numPoints * (int)sizeof(int));

    for (int i = 1; i < numPoints; i++)
    {
        buffer[i] = 0.0f;

        num[i] = 0;
        for (int j = -numSmooth / 2; j < numSmooth / 2; j++)
        {
            int index = i + j;
            if (index >= 1 && index < numPoints)
            {
                buffer[i] += data[index];
                ++num[i];
            }
        }
    }
    
    for (int i = 1; i < numPoints; i++)
    {
        data[i] = (uint8)(buffer[i] / num[i] + 0.5f);
    }

    free(buffer);
    free(num);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Math::IntInRange(int value, int min, int max)
{
    return (value >= min) && (value <= max);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float Math::MaxFloat(float val1, float val2, float val3)
{
    float retValue = val1;
    if (val2 > retValue)
    {
        retValue = val2;
    }
    if (val3 > retValue)
    {
        retValue = val3;
    }
    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Math::SwapInt(int *value0, int *value1)
{
    int temp = *value0;
    *value0 = *value1;
    *value1 = temp;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Math::SortInt(int *value0, int *value1)
{
    if (*value1 < *value0)
    {
        SwapInt(value0, value1);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Math::MinFrom2Int(int val0, int val1)
{
    if (val0 < val1)
    {
        return val0;
    }
    return val1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Pow10(int pow)
{
    int retValue = 1;

    while (pow--)
    {
        retValue *= 10;
    }

    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Math::MaxFromArray(const uint8 *data, int firstPoint, int lastPoint)
{

#define MAX_IF_ABOVE if(d > max) { max = d; }

    uint8 max = 0;
    const uint8 *pointer = &data[firstPoint];

    for (int i = firstPoint; i < lastPoint; i += 2)
    {
        uint8 d = *pointer++;
        MAX_IF_ABOVE;
        d = *pointer++;
        MAX_IF_ABOVE;
    }
    if ((lastPoint - firstPoint + 1) & 1)
    {
        uint8 d = *pointer;
        MAX_IF_ABOVE
    }

    return max;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Math::MinFromArray(const uint8 *data, int firstPoint, int lastPoint)
{

#define MIN_IF_LESS if(d < min) { min = d; }

    uint8 min = 255;
    const uint8 *pointer = &data[firstPoint];

    for (int i = firstPoint; i < lastPoint; i += 2)
    {
        uint8 d = *pointer++;
        MIN_IF_LESS
            d = *pointer++;
        MIN_IF_LESS
    }
    if ((lastPoint - firstPoint + 1) & 1)
    {
        uint8 d = *pointer;
        MIN_IF_LESS
    }

    return min;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Math::FloatsIsEquals(float value0, float value1, float epsilonPart)
{
    float max = fabsf(value0) > fabsf(value1) ? fabsf(value0) : fabsf(value1);

    float epsilonAbs = max * epsilonPart;

    return fabsf(value0 - value1) < epsilonAbs;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float Math::RandFloat(float min, float max)
{
    float delta = max - min;
    return min + ((rand() / (float)RAND_MAX) * delta);
}
