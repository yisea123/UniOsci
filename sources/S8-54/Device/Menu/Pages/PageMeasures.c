#include "defines.h"
#include "Menu/MenuItems.h"
#include "Utils/Measures.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/GlobalFunctions.h"
#include "Menu/Pages/Definition.h"
#include "Hardware/Sound.h"
#include "FPGA/fpgaExtensions.h  "


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int8 posActive = 0;                 ///< ������� ��������� ��������� (�� ������� ������)
bool pageChoiceIsActive = false;    ///< ���� true - �������� �������� ������ ���������
int8 posOnPageChoice = 0;           ///< ������� ������� �� �������� ������ ���������

extern const Page pMeasures;
extern const Page ppFreqMeter;
extern const Page ppTune;

//------------------------------------------------------------------------------------------------------------ ��������� - ���������� - ���������� ---
static void OnChanged_FreqMeter_Enable(bool param)
{
    FreqMeter_Init();
}

DEF_CHOICE_2
(
    cFreqMeter_Enable, ppFreqMeter,
    FREQ_METER_ENABLED, FuncActive, OnChanged_FreqMeter_Enable, FuncDraw,
    "����������", "Freq meter",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN
);

//--------------------------------------------------------------------------------------------------------- ��������� - ���������� - ����� ����� F ---
DEF_CHOICE_3
(
    cFreqMeter_TimeF, ppFreqMeter,
    FREQ_METER_TIMECOUNTING, FuncActive, OnChanged_FreqMeter_Enable, FuncDraw,
    "����� ����� F", "Time calc F",
    "��������� ������� �������� ��������� ������� - ��� ������ �����, ��� ������ �������� � ������ ����� ���������",
    "Allows to choose the accuracy of measurement of frequency - the more time, the accuracy more time of measurement is more",
    "100��", "100ms",
    "1�", "1s",
    "10�", "10ms"
);

//--------------------------------------------------------------------------------------------------------- ��������� - ���������� - ����� ������� ---
DEF_CHOICE_4
(
    cFreqMeter_FreqClc, ppFreqMeter,
    FREQ_METER_FREQ_CLC, FuncActive, OnChanged_FreqMeter_Enable, FuncDraw,
    "����� �������", "Timestamps",
    "����� ������� ���������� ������� ���������",
    "Choice of frequency of following of calculating impulses",
    "100���", "10MHz",
    "1���", "200MHz",
    "10���", "10MHz",
    "100���", "100MHz"
);

//------------------------------------------------------------------------------------------------------- ��������� - ���������� - ���-�� �������� ---
DEF_CHOICE_3
(
    cFreqMeter_NumPeriods, ppFreqMeter,
    FREQ_METER_NUM_PERIODS, FuncActive, OnChanged_FreqMeter_Enable, FuncDraw,
    "���-�� ��������", "Num periods",
    "��������� ������� �������� ��������� ������� - ��� ������ �����, ��� ������ �������� � ������ ����� ���������",
    "Allows to choose the accuracy of measurement of period - the more time, the accuracy more time of measurement is more",
    "1", "1",
    "10", "10",
    "100", "100"    
);

//------------------------------------------------------------------------------------------------------------------------- ��������� - ���������� ---
DEF_CHOICE_2
(
    cIsShow, pMeasures,
    SHOW_MEASURES, FuncActive, FuncChangedChoice, FuncDraw,
    "����������", "Show",
    "�������� ��� �� �������� ��������� �� �����",
    "Output or output measurements on screen",
    "���", "No",
    "��", "Yes"
);

//------------------------------------------------------------------------------------------------------------------------- ��������� - ���������� ---
static bool IsActive_Number(void)
{
    return SHOW_MEASURES;
}

DEF_CHOICE_7
(
    cNumber, pMeasures,
    NUM_MEASURES, IsActive_Number, FuncChangedChoice, FuncDraw,
    "����������", "Number"
    ,
    "������������� ������������ ���������� ��������� ���������:\n"
    "\"1\" - ���� ���������\n"
    "\"2\" - ��� ���������\n"
    "\"1�5\" - 1 ������ � ����� �����������\n"
    "\"2�5\" - 2 ������ � ����� ����������� � ������\n"
    "\"3�5\" - 3 ������ � ����� ����������� � ������\n"
    "\"6x1\" - 6 ����� �� ������ ��������� � ������\n"
    "\"6�2\" - 6 ����� �� ��� ��������� � ������"
    ,
    "Sets the maximum number of output measurements:\n"
    "\"1\" - one measurement\n"
    "\"2\" - two measurements\n"
    "\"1x5\" - 1 line with the five dimensions\n"
    "\"2x5\" - two rows with five measurements in each\n"
    "\"3x5\" - 3 lines with five measurements in each"
    "\"6x1\" - 6 lines, one in each dimension\n"
    "\"6x2\" - 6 lines of two dimensions in each\n",
    "1", "1",
    "2", "2",
    "1x5", "1x5",
    "2x5", "2x5",
    "3x5", "3x5",
    "6x1", "6x1",
    "6x2", "6x2"
);

//----------------------------------------------------------------------------------------------------------------------------- ��������� - ������ ---
static bool IsActive_Channels(void)
{
    return SHOW_MEASURES;
}

DEF_CHOICE_3
(
    cChannels, pMeasures,
    SOURCE_MEASURE, IsActive_Channels, FuncChangedChoice, FuncDraw,
    "������", "Channels",
    "�� ����� ������� �������� ���������",
    "Which channels to output measurement",
    "1", "1",
    "2", "2",
    "1 � 2", "1 and 2"
);

//-------------------------------------------------------------------------------------------------------------------------------- ��������� - ��� ---
static bool IsActive_Mode(void)
{
    return SHOW_MEASURES;
}

DEF_CHOICE_2
(
    cMode, pMeasures,
    set.meas_ModeViewSignals, IsActive_Mode, FuncChangedChoice, FuncDraw,
    "���", "View",
    "��������� ��� ��� ���� ������ ������� ��� ���������� ���������� ��� ���������� ���������",
    "Decrease or no zone output signal to avoid overlapping of its measurement results",
    "��� ����",    "As is",
    "���������",   "Reduce"
);

//------------------------------------------------------------------------------------------------------------------ ��������� - ��������� - ����� ---
DEF_SMALL_BUTTON
(
    bTune_Exit,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button to return to the previous menu",
    ppTune, FuncActive, OnPressSB_Exit, DrawSB_Exit
);

//---------------------------------------------------------------------------------------------------------------- ��������� - ��������� - ������� ---
static void OnPress_Tune_Markers(void)
{
    meas.ShortPressOnSmallButonMarker();
}

static void Draw_Tune_Markers(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 2, '\x60');
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON
(
    bTune_Markers,
    "������", "Marker",
    "��������� ���������� ������� ��� ����������� �������� ���������",
    "Allows to establish markers for visual control of measurements",
    ppTune, FuncActive, OnPress_Tune_Markers, Draw_Tune_Markers
);

//-------------------------------------------------------------------------------------------------------------- ��������� - ��������� - ��������� ---
static void OnPress_Tune_Settings(void)
{
    pageChoiceIsActive = !pageChoiceIsActive;
    if (pageChoiceIsActive)
    {
        posOnPageChoice = MEASURE(posActive);
    }
}

static void Draw_Tune_Settings(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, '\x62');
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON
(
    bTune_Settings,
    "���������", "Setup",
    "��������� ������� ����������� ���������",
    "Allows to choose necessary measurements",
    ppTune, FuncActive, OnPress_Tune_Settings, Draw_Tune_Settings
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��������� - ��������� ///
static bool IsActive_Tune(void)
{
    return SHOW_MEASURES;
}

static void OnRegSet_Tune(int angle)
{
    static const int8 step = 3;
    static int8 currentAngle = 0;
    currentAngle += (int8)angle;
    if (currentAngle < step && currentAngle > -step)
    {
        return;
    }
    if (pageChoiceIsActive)
    {
        posOnPageChoice += (int8)math.Sign(currentAngle);
        Sound_RegulatorSwitchRotate();
        if (posOnPageChoice < 0)
        {
            posOnPageChoice = Meas_NumMeasures - 1;
        }
        else if (posOnPageChoice == Meas_NumMeasures)
        {
            posOnPageChoice = 0;
        }
        MEASURE(posActive) = (Meas)posOnPageChoice;
        painter.ResetFlash();
    }
    else
    {
        int row = 0;
        int col = 0;
        meas.GetActive(&row, &col);
        col += math.Sign(currentAngle);
        if (col < 0)
        {
            col = meas.NumCols() - 1;
            row--;
            if (row < 0)
            {
                row = meas.NumRows() - 1;
            }
        }
        else if (col == meas.NumCols())
        {
            col = 0;
            row++;
            if (row >= meas.NumRows())
            {
                row = 0;
            }
        }
        meas.SetActive(row, col);
        Sound_RegulatorSwitchRotate();
    }
    currentAngle = 0;
}

DEF_PAGE_SB(    ppTune, static,
    PageSB_Measures_Tune, &pMeasures, IsActive_Tune,
    EmptyPressPage, EmptyDrawPage, OnRegSet_Tune,
    "���������", "CONFIGURE",
    "������� � ����� ������ ��������� ���������� � ����� ���������",
    "Transition to rezhm of exact control of quantity and types of measurements",
    &bTune_Exit,
    0,
    0,
    0,
    &bTune_Markers,
    &bTune_Settings
);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��������� - ���������� ///
DEF_PAGE_4(     ppFreqMeter, static,
    Page_Service_FreqMeter, &pMeasures, FuncActive, EmptyPressPage,
    "����������", "FREQ METER",
    "",
    "",
    cFreqMeter_Enable,     // ��������� - ���������� - ����������
    cFreqMeter_TimeF,      // ��������� - ���������� - ����� ����� F
    cFreqMeter_FreqClc,    // ��������� - ���������� - ����� �������
    cFreqMeter_NumPeriods  // ��������� - ���������� - ���-�� ��������
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��������� ///
DEF_PAGE_6(     pMeasures, ,
    Page_Measures, &mainPage, FuncActive, EmptyPressPage,
    "���������", "MEASURES",
    "�������������� ���������",
    "Automatic measurements",
    ppFreqMeter,   // ��������� - ����������
    cIsShow,       // ��������� - ����������
    cNumber,       // ��������� - ����������
    cChannels,     // ��������� - ������
    ppTune,        // ��������� - ���������
    cMode          // ��������� - ���
);

const Page * pointerPageMeasures = &pMeasures;