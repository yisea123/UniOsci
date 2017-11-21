#include "_Math.h"
#include "Settings/Settings.h"
#include "Log.h"
#include "FPGA/FPGAtypes.h"
#include "FPGA/FPGAMath.h"
#include "Display/Grid.h"
#include "Hardware/Timer.h"
#include "Utils/Math.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Math_ _math;

const float voltsInPoint[RangeSize] =
{
    2e-3f / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE),   // 2mV
    5e-3f / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE),   // 5mV
    10e-3f / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE),  // 10mV
    20e-3f / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE),  // 20mV
    50e-3f / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE),  // 50mV
    100e-3f / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE), // 100mV
    200e-3f / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE), // 200mV
    500e-3f / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE), // 500mV
    1.0f / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE),    // 1V
    2.0f / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE),    // 2V
    5.0f / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE)     // 5V
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Math_::RShift2Pixels(uint16 rShift, int heightGrid)
{
    float scale = (float)heightGrid / (STEP_RSHIFT * 200);
    return (int)(scale * (rShift - RShiftZero));
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float Math_::VoltageCursor(float shiftCurU, Range range, uint16 rShift)
{
    return MAX_VOLTAGE_ON_SCREEN(range) - shiftCurU * voltsInPixel[range] - RSHIFT_2_ABS(rShift, range);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Math_::DataExtrapolation(uint8 *data, uint8 *there, int size)
{
    int pointer = 0;
    while(there[pointer] == 0)
    {
        pointer++;
    }
    for(int i = 0; i < pointer; i++)
    {
        data[i] = data[pointer];
    }
    
    while(pointer < size)
    {
        while(there[pointer] != 0)
        {
            pointer++;
            if(pointer >= size)
            {
                return;
            }
        }
        pointer--;
        int pointer2 = pointer + 1;
        while(there[pointer2] == 0)
        {
            pointer2++;
            if(pointer2 >= size)
            {
                return;
            }
        }
        int deltaY = pointer2 - pointer;
        if(deltaY >= 2)
        {
            float deltaX = (float)(data[pointer2] - data[pointer]) / deltaY;
            for(int i = 1; i < deltaY; i++)
            {
                data[pointer + i] = (uint8)(data[pointer] + (uint8)(i * deltaX));
            }
        }
        pointer = pointer2 + 1;
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Math_::PointsRelToVoltage(const uint8 *points, int numPoints, Range range, uint16 rShift, float *voltage)
{
    const int voltsInPointInt[RangeSize] =   // ����������� 20000
    {
        2,      // 2
        5,      // 5
        10,     // 10
        20,     // 20
        50,     // 50
        100,    // 100
        200,    // 200
        500,    // 500
        1000,   // 1
        2000,   // 2
        5000    // 5
    };

    float voltInPoint = voltsInPointInt[range] / ((MAX_VALUE - MIN_VALUE) / 200.0f);
    float maxVoltsOnScreen = MAX_VOLTAGE_ON_SCREEN(range);
    float rShiftAbs = RSHIFT_2_ABS(rShift, range);
    int diff = (int)((MIN_VALUE * voltInPoint) + (int)((maxVoltsOnScreen + rShiftAbs) * 20e3f));
    float koeff = 1.0f / 20e3f;
    for (int i = 0; i < numPoints; i++)
    {
        voltage[i] = (points[i] * voltInPoint - diff) * koeff;
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Math_::PointsVoltageToRel(const float *voltage, int numPoints, Range range, uint16 rShift, uint8 *points)
{
    float maxVoltOnScreen = MAX_VOLTAGE_ON_SCREEN(range);
    float rShiftAbs = RSHIFT_2_ABS(rShift, range);
    float voltInPixel = 1.0f / (voltsInPixel[range] / ((MAX_VALUE - MIN_VALUE) / 200.0f));

    float add = maxVoltOnScreen + rShiftAbs;

    float delta = add * voltInPixel + MIN_VALUE;

    for (int i = 0; i < numPoints; i++)
    {
        int value = (int)(voltage[i] * voltInPixel + delta);
        if (value < 0)
        {
            points[i] = 0;
            continue;
        }
        else if (value > 255)
        {
            points[i] = 255;
            continue;
        }
        points[i] = (uint8)value;
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Math_::VoltageToPoint(float voltage, Range range, uint16 rShift)
{
    int relValue = (int)((voltage + MAX_VOLTAGE_ON_SCREEN(range) + RSHIFT_2_ABS(rShift, range)) / voltsInPoint[range]) + MIN_VALUE;
    LIMITATION(relValue, 0, 255);
    return (uint8)relValue;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float Math_::GetIntersectionWithHorizontalLine(int x0, int y0, int x1, int y1, int yHorLine)
{
    if(y0 == y1)
    {
        return (float)x1;
    }

    return (yHorLine - y0) / ((float)(y1 - y0) / (float)(x1 - x0)) + x0;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Math_::FloatsIsEquals(float value0, float value1, float epsilonPart)
{
    float max = fabsf(value0) > fabsf(value1) ? fabsf(value0) : fabsf(value1);

    float epsilonAbs = max * epsilonPart;

    return fabsf(value0 - value1) < epsilonAbs;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float Math_::MinFrom3float(float value1, float value2, float value3)
{
    float retValue = value1;
    if(value2 < retValue)
    {
        retValue = value2;
    }
    if(value3 < retValue)
    {
        retValue = value3;
    }
    return retValue;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
int Math_::MaxInt(int val1, int val2)
{
    return val1 > val2 ? val1 : val2;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int Math_::MinInt(int val1, int val2)
{
    return val1 < val2 ? val1 : val2;
}

/*
    ������� �������������� �����. ��������� ������ ������� ��� ��������������� �������.
    ���������� �������� ������ ���� 2**N
*/

#ifndef DEBUG
#include "TablesWindow.h"
#include "TablesLog.h"


//----------------------------------------------------------------------------------------------------------------------------------------------------
static float const *Koeff(int numPoints)
{
    float const *tables[3][4] = {
        {koeffsNorm256, koeffsHamming256, koeffsBlack256, koeffsHann256},
        {koeffsNorm512, koeffsHamming512, koeffsBlack512, koeffsHann512},
        {koeffsNorm1024, koeffsHamming1024, koeffsBlack1024, koeffsHann1024},
    };

    int row = 0;
    if (numPoints == 512)
    {
        row = 1;
    }
    else if (numPoints == 1024)
    {
        row = 2;
    }

    return tables[row][PageServiceMath_GetWindowFFT()];
}

#endif


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Normalize(float *data, int)
{
    float max = 0.0;
    for (int i = 0; i < 256; i++)
    {
        if (data[i] > max)
        {
            max = data[i];
        }
    }

    for (int i = 0; i < 256; i++)
    {
        data[i] /= max;
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void MultiplyToWindow(float *data, int numPoints)
{
#ifndef DEBUG
    float const *koeff = Koeff(numPoints);

    for (int i = 0; i < numPoints; i++)
    {
        data[i] *= koeff[i];
    }
#else
    if (WINDOW_FFT == WindowFFT_Hamming)
    {
        for (int i = 0; i < numPoints; i++)
        {
            data[i] *= 0.53836f - 0.46164f * cosf(2.0f * 3.1415926f * i / (numPoints - 1));
        }
    }
    else if (WINDOW_FFT == WindowFFT_Blackman)
    {
        float alpha = 0.16f;
        float a0 = (1.0f - alpha) / 2.0f;
        float a1 = 0.5f;
        float a2 = alpha / 2.0f;
        for (int i = 0; i < numPoints; i++)
        {
            data[i] *= a0 - a1 * cosf(2.0f * 3.1415926f * i / (numPoints - 1.0f)) + a2 * cosf(4.0f * 3.1415926f * i / (numPoints - 1.0f));
        }
    }
    else if (WINDOW_FFT == WindowFFT_Hann)
    {
        for (int i = 0; i < numPoints; i++)
        {
            data[i] *= 0.5f * (1.0f - cosf(2.0f * 3.1415926f * i / (numPoints - 1.0f)));
        }
    }
#endif
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
#define SET_MIN_MAX(x)		\
	if (x < min) min = x;	\
	if (x > max) max = x;


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Math_::CalculateFFT(float *dataR, int numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1)
{
    float scale = 1.0f / absStepTShift[SET_TBASE] / 1024.0f;

    float k = 1024.0f / numPoints;

    *freq0 = scale * POS_MATH_CUR_0 * k;
    *freq1 = scale * POS_MATH_CUR_1 * k;
    if (SET_PEAKDET)
    {
        *freq0 *= 2;
        *freq1 *= 2;
    }

    for (int i = 0; i < numPoints; i++)
    {
        result[i] = 0.0f;
    }

    MultiplyToWindow(dataR, numPoints);

    const int logs[FPGA_ENUM_POINTS_SIZE] = {9, 10, 11, 12, 13, 14, 15};

    int logN = logs[NumPoints_2_ENumPoints(numPoints)];

    static const float Rcoef[14] =
    {
        -1.0000000000000000F, 0.0000000000000000F, 0.7071067811865475F,
        0.9238795325112867F, 0.9807852804032304F, 0.9951847266721969F,
        0.9987954562051724F, 0.9996988186962042F, 0.9999247018391445F,
        0.9999811752826011F, 0.9999952938095761F, 0.9999988234517018F,
        0.9999997058628822F, 0.9999999264657178F
    };

    static const float Icoef[14] =
    {
        0.0000000000000000F, -1.0000000000000000F, -0.7071067811865474F,
        -0.3826834323650897F, -0.1950903220161282F, -0.0980171403295606F,
        -0.0490676743274180F, -0.0245412285229122F, -0.0122715382857199F,
        -0.0061358846491544F, -0.0030679567629659F, -0.0015339801862847F,
        -0.0007669903187427F, -0.0003834951875714F
    };

    int nn = numPoints >> 1;
    int ie = numPoints;

    for (int n = 1; n <= logN; n++) 
    {
        float rw = Rcoef[logN - n];
        float iw = Icoef[logN - n];
        int in = ie >> 1;
        float ru = 1.0f;
        float iu = 0.0f;
        for (int j = 0; j < in; j++) 
        {
            for (int i = j; i < numPoints; i += ie) 
            {
                int io = i + in;
                float dRi = dataR[i];
                float dRio = dataR[io];
                float ri = result[i];
                float rio = result[io];
                dataR[i] = dRi + dRio;
                result[i] = ri + rio;
                float rtq = dRi - dRio;
                float itq = ri - rio;
                dataR[io] = rtq * ru - itq * iu;
                result[io] = itq * ru + rtq * iu;
            }
            float sr = ru;
            ru = ru * rw - iu * iw;
            iu = iu * rw + sr * iw;
        }
        ie >>= 1;
    }

    for (int j = 1, i = 1; i < numPoints; i++) 
    {
        if (i < j)
        {
            int io = i - 1;
            int in = j - 1;
            float rtp = dataR[in];
            float itp = result[in];
            dataR[in] = dataR[io];
            result[in] = result[io];
            dataR[io] = rtp;
            result[io] = itp;
        }

        int kk = nn;

        while (kk < j)
        {
            j = j - kk;
            kk >>= 1;
        }

        j = j + kk;
    }

    for (int i = 0; i < 256; i++)
    {
        result[i] = sqrtf(dataR[i] * dataR[i] + result[i] * result[i]);
    }

    result[0] = 0.0f;       /// \todo ������� ������������ ������ ���������. ���� � ������

    Normalize(result, 256);

    if (SCALE_FFT_LOG)
    {
        float minDB = sService_MathGetFFTmaxDBabs();

        for (int i = 0; i < 256; i++)
        {
#ifdef DEBUG
            result[i] = 20 * log10f(result[i]);
#else
            result[i] = Log10[(int)(result[i] * 10000)];
#endif
            if (i == POS_MATH_CUR_0)
            {
                *density0 = result[i];
            }
            else if (i == POS_MATH_CUR_1)
            {
                *density1 = result[i];
            }
            if (result[i] < minDB)
            {
                result[i] = minDB;
            }
            result[i] = 1.0f - result[i] / minDB;
        }
    }
    else
    {
        *density0 = result[POS_MATH_CUR_0];
        *density1 = result[POS_MATH_CUR_1];
    }
    *y0 = grid.MathBottom() - (int)(result[POS_MATH_CUR_0] * grid.MathHeight());
    *y1 = grid.MathBottom() - (int)(result[POS_MATH_CUR_1] * grid.MathHeight());
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Math_::CalculateMathFunction(float *data0andResult, float *dataB, int numPoints)
{
    if (MATH_FUNC_SUM)
    {
        int delta = dataB - data0andResult;
        float *end = &data0andResult[numPoints];
        while (data0andResult < end)
        {
            *data0andResult += *(data0andResult + delta);
            data0andResult++;
        }
    }
    else if (MATH_FUNC_MUL)
    {
        int delta = dataB - data0andResult;
        float *end = &data0andResult[numPoints];
        while (data0andResult < end)
        {
            *data0andResult *= *(data0andResult + delta);
            data0andResult++;
        }
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float Math_::RandFloat(float min, float max)
{
    float delta = max - min;
    return min + ((rand() / (float)RAND_MAX) * delta);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int8 Math_::AddInt8WithLimitation(int8 value, int8 delta, int8, int8 max)
{
    int8 retValue = value + delta;
    if (retValue < 0)
    {
        return 0;
    }
    if (retValue > max)
    {
        return max;
    }
    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Math_::NumDigitsInNumber(int value)
{
    value = FabsInt(value);
    int num = 1;
    while ((value /= 10) > 0)
    {
        num++;
    }
    return num;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int Math_::FabsInt(int value)
{
    return value >= 0 ? value : -value;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Math_::GetMaxFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint)
{
    uint8 max = GetMaxFromArray(data, firstPoint, lastPoint);
    if (max >= MAX_VALUE)
    {
        max = ERROR_VALUE_UINT8;
    }
    return max;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Math_::GetMinFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint)
{
    uint8 min = GetMinFromArray(data, firstPoint, lastPoint);
    if (min < MIN_VALUE || min >= MAX_VALUE)
    {
        min = ERROR_VALUE_UINT8;
    }
    return min;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Math_::GetMinFromArray(const uint8 *data, int firstPoint, int lastPoint)
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
uint8 Math_::GetMinFromArray_RAM(const uint16 *data, int firstPoint, int lastPoint)
{
    uint8 min = 255;

    const uint16 *pointer = &data[firstPoint];

    const int endPoint = lastPoint / 2;

    for (int i = firstPoint; i < endPoint; i++)
    {
        uint16 d16 = *pointer++;

        uint8 d8 = (uint8)d16;
        if (d8 < min)
        {
            min = d8;
        }
        d8 = (uint8)(d16 >> 8);
        if (d8 < min)
        {
            min = d8;
        }
    }
    
    return min;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Math_::GetMaxFromArray(const uint8 *data, int firstPoint, int lastPoint)
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
uint8 Math_::GetMaxFromArray_RAM(const uint16 *data, int firstPoint, int lastPoint)
{
    uint8 max = 0;

    const uint16 *pointer = &data[firstPoint];

    const int endPoint = lastPoint / 2;

    for (int i = firstPoint; i < endPoint; i++)
    {
        uint16 d16 = *pointer++;

        uint8 d8 = (uint8)d16;
        if (d8 > max)
        {
            max = d8;
        }

        d8 = (uint8)(d16 >> 8);
        if (d8 > max)
        {
            max = d8;
        }
    }

    return max;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
#define LIMIT                           \
    if (value <= min) { return min; }   \
    if (value >= max) { return max; }   \
    return value;


//----------------------------------------------------------------------------------------------------------------------------------------------------
float Math_::LimitationFloat(float value, float min, float max)
{
    LIMIT
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Math_::CalculateFiltr(const uint8 *data, int x, int numPoints)
{
    if (NUM_SMOOTHING < 2)
    {
        return data[x];
    }

    int count = 1;
    int sum = data[x];
    int startDelta = 1;

    int endDelta = NUM_SMOOTHING / 2;

    for (int delta = startDelta; delta <= endDelta; delta++)
    {
        if (((x - delta) >= 0) && ((x + delta) < (numPoints)))
        {
            sum += data[x - delta];
            sum += data[x + delta];
            count += 2;
        }
    }

    if ((NUM_SMOOTHING % 2) == 1)
    {
        int delta = NUM_SMOOTHING / 2;
        if ((x + delta) < numPoints)
        {
            sum += data[x + delta];
            count++;
        }
    }

    return (uint8)(sum / count);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Math_::CalculateFiltrArray(const uint8 *dataIn, uint8 *dataOut, int numPoints, bool needSmoothing)
{
    if (NUM_SMOOTHING < 2 || !needSmoothing)
    {
        memcpy(dataOut, dataIn, numPoints);
    }
    else
    {
        bool addCalculation = (NUM_SMOOTHING % 2) == 1;
        int endDelta = NUM_SMOOTHING / 2;
        int startDelta = 1;

        for (int i = 0; i < numPoints; i++)
        {
            int count = 1;
            int sum = dataIn[i];

            for (int delta = startDelta; delta <= endDelta; delta++)
            {
                if (((i - delta) >= 0) && ((i + delta) < (numPoints)))
                {
                    sum += dataIn[i - delta];
                    sum += dataIn[i + delta];
                    count += 2;
                }
            }

            if (addCalculation)
            {
                int delta = NUM_SMOOTHING / 2;
                
                if ((i + delta) < numPoints)
                {
                    sum += dataIn[i + delta];
                    count++;
                }
            }

            dataOut[i] = (uint8)(sum / count);
        }
    }
}
