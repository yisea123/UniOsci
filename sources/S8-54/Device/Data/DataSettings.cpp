#include "DataSettings.h"
#include "Globals.h"
#include "Hardware/FLASH.h"
#include "Settings/SettingsMemory.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int BytesInChannel(DataSettings *ds)
{
    static const int numPoints[FPGA_ENUM_POINTS_SIZE][3] =
    {
        {512,   1024,  1024},
        {1024,  2048,  2048},
        {2048,  4096,  4096},
        {4096,  8192,  8192},
        {8192,  16384, 16384},
        {16384, 32768, 16384},
        {32768, 32768, 32768}
    };

    return numPoints[ENUM_POINTS(ds)][PEAKDET(ds)];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DataSettings_Fill(DataSettings *ds)
{
    Lval_ENABLED_A(ds) = SET_ENABLED_A ? 1U : 0U;
    Lval_ENABLED_B(ds) = SET_ENABLED_B ? 1U : 0U;
    INVERSE_A(ds) = SET_INVERSE_A ? 1U : 0U;
    INVERSE_B(ds) = SET_INVERSE_B ? 1U : 0U;
    Lval_RANGE_A(ds) = (uint8)SET_RANGE_A;
    Lval_RANGE_B(ds) = (uint8)SET_RANGE_B;
    RSHIFT_A(ds) = SET_RSHIFT_A;
    RSHIFT_B(ds) = SET_RSHIFT_B;
    Lval_TBASE(ds) = SET_TBASE;
    TSHIFT(ds) = SET_TSHIFT;
    Lval_COUPLE_A(ds) = SET_COUPLE_A;
    Lval_COUPLE_B(ds) = SET_COUPLE_B;
    TRIGLEV_A(ds) = SET_TRIGLEV_A;
    TRIGLEV_B(ds) = SET_TRIGLEV_A;
    Lval_PEAKDET(ds) = SET_PEAKDET;
    Lval_DIVIDER_A(ds) = SET_DIVIDER_A;
    Lval_DIVIDER_B(ds) = SET_DIVIDER_B;
    TIME_MS(ds) = 0;                        // ��� ����� ��� ������ ���������� ������. ��������, ��� ������ ������ ��� �� ������
    ENUM_POINTS(ds) = FPGA_ENUM_POINTS;
}