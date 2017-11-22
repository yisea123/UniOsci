#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"


class Math_
{
public:

    uint8 CalculateFiltr(const uint8 *data, int x, int numPoints);

    void CalculateFiltrArray(const uint8 *dataIn, uint8 *dataOut, int numPoints, bool needSmoothing);
};


extern Math_ _math;


/** @}  @}
 */
