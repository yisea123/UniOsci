#pragma once
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PI 3.141592653589793f

#define LIMIT_BELOW(x, min)         if((x) < (min)) { x = (min); }

#define LIMIT_ABOVE(x, max)         if((x) > (max)) { x = (max); }

#define SET_MIN_IF_LESS(x, min)     if((x) < (min)) { (min) = (x); }

#define SET_MAX_IF_LARGER(x, max)   if((x) > (max)) { (max) = (x); }

#define LIMITATION_BOUND(var, min, max)     if((var) < (min)) { (min) = (var); }; if((var) > (max)) { (max) = (var); };

#define TSHIFT_2_REL(tShiftAbs, tBase) ((int)((tShiftAbs) / absStepTShift[(tBase)] / 2.0f))

#define ROUND(x) ((x) += 0.5f, (x))

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const float absStepRShift[];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Math
{
public:
    /// ���������� true, ���� value ������ � �������� [min; max]
    bool IntInRange(int value, int min, int max );

    int LowSignedBit(uint value);

    void Smoothing(uint8 *data, int numPoints, int numSmooth);
    /// ���������� ������������ �������� �� ���
    float MaxFloat(float val1, float val2, float val3);
    /// ���������� ������� ���������� ������ �� ������� value0 � value1
    void SwapInt(int *value0, int *value1);
    /// ��������� ���������� value0 � value1 � ������� �����������
    void SortInt(int *value0, int *value1);
    
    int MinFrom2Int(int val0, int val1);

    uint8 MaxFromArray(const uint8 *data, int firstPoint, int lastPoint);

    uint8 MinFromArray(const uint8 *data, int firstPoint, int lastPoint);

    uint8 MaxFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint);

    uint8 MinFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint);
    /// \brief ���������� ��� �����. ���������� true, ���� ����� ���������� ���� �� ����� �� �����, ��� �� epsilonPart. ��� ���� ��� ������� epsilonPart 
    /// ������������ ������� � ������ ������ �����.
    bool FloatsIsEquals(float value0, float value1, float epsilonPart);

    float RandFloat(float min, float max);

    template<class T>
    void CircleIncrease(T *value, int min, int max)
    {
        if (*value < max)   { ++(*value); }
        else                { *value = (T)min; }
    }

    template<class T>
    void CircleDecrease(T *value, int min, int max)
    {
        if (*value > min)   { --(*value); }
        else                { *value = (T)max; }
    }

    template<class T>
    static void LimitationIncrease(T *value, T max)
    {
        if ((*value) < max) { ++(*value); }
    }

    /// ���������������� *value, �� �� ������, ��� max
    template<class T>
    static void LimitationDecrease(T *value, T min)
    {
        if (*value > min)  { --(*value); }
    }

    /// ��������� � *value term � ����������, ���� ��������� ������� �� ������� [min, max]
    template<class T>
    static void AddtionThisLimitation(T *value, int term, T min, T max)
    {
        if (term > 0)
        {
            if (max - term >= *value)   { *value += (T)term; }
            else                        { *value = max;      }
        }
        else
        {
            if (min - term <= *value)   { *value += (T)term; }
            else                        { *value = min;      }
        }
    }

    template<class T>
    static void Limitation(T *value, T min, T max)
    {
        if (*value < min)       { *value = min; }
        else if (*value > max)  { *value = max; }
    }

    template<class T>
    static T LimitationRet(T value, T min, T max)
    {
        if(value < min) { return min; }
        if(value > max) { return max; }
        return value;
    }
};


extern class Math math;

/// ���������� 10**pow.
int Pow10(int pow);

template<class T> int Sign(T x)
{
    if (x > (T)(0)) { return 1; }
    if (x < (T)(0)) { return -1; }
    return 0;
}

template<class T> T Abs(T x)
{
    if (x < (T)0)   { return -x; }
    return x;
}

typedef void (Math::*pFuncMathPI8II)(int8 *, int, int);
typedef void (Math::*pFuncMathPIII)(int *, int, int);
