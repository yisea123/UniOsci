#include "main.h"
#include "Display/Symbols.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGAMath.h"
#include "Hardware/Hardware.h"
#include "Hardware/RTC.h"
#include "Hardware/Sound.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Panel/Panel.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern void Func_Start(int key);          // 1 - �������, 1 - ����������

extern const         Page pService;
extern const       Button bResetSettings;                           ///< ������ - ����� ��������
        void       OnPress_ResetSettings(void);
static void           Draw_ResetSettings(void);
extern const       Button bAutoSearch;                              ///< ������ - ����� �������
static void        OnPress_AutoSearch(void);
extern const        Page ppCalibrator;                              ///< ������ - ����������
extern const       Choice cCalibrator_Calibrator;                   ///< ������ - ���������� - ����������
static void      OnChanged_Calibrator_Calibrator(bool active);
extern const       Button bCalibrator_Calibrate;                    ///< ������ - ���������� - �����������
static bool       IsActive_Calibrator_Calibrate(void);
static void        OnPress_Calibrator_Calibrate(void);
#ifdef OLD_RECORDER
extern const       Choice cRecorder;
static void      OnChanged_Recorder(bool active);
#else
static const       Page ppRecorder;                                 ///< ������ - �����������
static const     SButton bRecorder_Exit;                            ///< ������ - ����������� - �����
static const     SButton bRecorder_SaveTo;                          ///< ������ - ����������� - ��������� �...
static bool      IsActive_Recorder_SaveTo(void);
static void       OnPress_Recorder_SaveTo(void);
static void          Draw_Recorder_SaveTo(int, int);
static void          Draw_Recorder_SaveTo_RAM(int, int);
static void          Draw_Recorder_SaveTo_EXT(int, int);
static const     SButton bRecorder_Choice;                          ///< ������ - ����������� - �����
static void       OnPress_Recorder_Choice(void);
static void          Draw_Recorder_Choice(int x, int y);
static const     SButton bRecorder_Cursor;                          ///< ������ - ����������� - ������
static void       OnPress_Recorder_Cursor(void);
static void          Draw_Recorder_Cursor(int x, int y);
#endif
extern const        Page ppFFT;                                     ///< ������ - ������
static bool       IsActive_FFT(void);
static void        OnPress_FFT(void);
extern const       Choice cFFT_View;                                ///< ������ - ������ - �����������
extern const       Choice cFFT_Scale;                               ///< ������ - ������ - �����
extern const       Choice cFFT_Source;                              ///< ������ - ������ - ��������
extern const       Choice cFFT_Window;                              ///< ������ - ������ - ����
extern const       Page pppFFT_Cursors;                             ///< ������ - ������ - �������
static bool       IsActive_FFT_Cursors(void);
static void       OnRegSet_FFT_Cursors(int angle);
extern const      SButton bFFT_Cursors_Exit;                        ///< ������ - ������ - ������� - �����
extern const      SButton bFFT_Cursors_Source;                      ///< ������ - ������ - ������� - ��������
static void        OnPress_FFT_Cursors_Source(void);
static void           Draw_FFT_Cursors_Source(int x, int y);
extern const       Choice cFFT_Range;                               ///< ������ - ������ - ��������
static bool       IsActive_FFT_Range(void);
extern const        Page ppFunction;                                ///< ������ - �������
static bool       IsActive_Function(void);
static void        OnPress_Function(void);
static void       OnRegSet_Function(int delta);
extern const      SButton bFunction_Exit;                           ///< ������ - ������� - �����
extern const      SButton bFunction_Screen;                         ///< ������ - ������� - �����
static void        OnPress_Function_Screen(void);
static void           Draw_Function_Screen(int x, int y);
static void           Draw_Function_Screen_Disable(int x, int y);
static void           Draw_Function_Screen_Separate(int x, int y);
static void           Draw_Function_Screen_Together(int x, int y);
extern const      SButton bFunction_Type;                           ///< ������ - ������� - ���
static void        OnPress_Function_Type(void);
static void           Draw_Function_Type(int x, int y);
static void           Draw_Function_Type_Sum(int x, int y);
static void           Draw_Function_Type_Mul(int x, int y);
extern const      SButton bFunction_ModeRegSet;                     ///< ������ - ������� - ����� ����� ���������
static void        OnPress_Function_ModeRegSet(void);
static void           Draw_Function_ModeRegSet(int x, int y);
static void           Draw_Function_ModeRegSet_Range(int x, int y);
static void           Draw_Function_ModeRegSet_RShift(int x, int y);
extern const      SButton bFunction_RangeA;                         ///< ������ - ������� - ������� 1-�� ������
static void        OnPress_Function_RangeA(void);
static void           Draw_Function_RangeA(int x, int y);
extern const      SButton bFunction_RangeB;                         ///< ������ - ������� - ������� 2-�� ������
static void        OnPress_Function_RangeB(void);
static void           Draw_Function_RangeB(int x, int y);
extern const        Page ppEthernet;                                ///< ������ - ETHERNET
extern const      Choice cEthernet_Ethernet;                        ///< ������ - ETHERNET - Ethernet
static void     OnChanged_Ethernet_Settings(bool active);
extern const  IPaddress ipEthernet_IP;                              ///< ������ - ETHERNET - IP �����
extern const  IPaddress ipEthernet_NetMask;                         ///< ������ - ETHERNET - ����� �������
extern const  IPaddress ipEthernet_Gateway;                         ///< ������ - ETHERNET - ����
extern const MACaddress ipEthernet_MAC;                             ///< ������ - ETHERNET - MAC �����
extern const       Page ppSound;                                    ///< ������ - ����
extern const      Choice cSound_Enable;                             ///< ������ - ���� - ����
extern const    Governor gSound_Volume;                             ///< ������ - ���� - ���������
extern const       Page ppRTC;                                     ///< ������ - �����
extern const        Time tRTC_Time;                                ///< ������ - ����� - �����
extern const    Governor tRTC_Correction;                          ///< ������ - ����� - ���������
static void     OnChanged_Time_Correction(void);
extern const      Choice cLanguage;                                 ///< ������ - ����
extern const       Page ppInformation;                              ///< ������ - ����������
static void       OnPress_Information(void);
static void   Information_Draw(void);
extern const     SButton bInformation_Exit;                         ///< ������ - ���������� - �����
static void       OnPress_Information_Exit(void);

// ������ ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const void * const itemspService[] =
{
        (void *)&bResetSettings,    // ������ - ����� ��������
        (void *)&bAutoSearch,       // ������ - ����� �������
        (void *)&ppCalibrator,      // ������ - ����������
#ifdef OLD_RECORDER
        (void *)&cRecorder,         // ������ - �����������
#else
        (void *)&ppRecorder,        // ������ - �����������
#endif
        (void *)&ppFFT,             // ������ - ������
        (void *)&ppFunction,        // ������ - �������
        (void *)&ppEthernet,        // ������ - ETHERNET
        (void *)&ppSound,           // ������ - ����
        (void *)&ppRTC,             // ������ - �����
        (void *)&cLanguage,         // ������ - ����
        (void *)&ppInformation      // ������ - ����������
};

const Page pService =
{
    Item_Page, false, Page_Service, 11, &mainPage, 0,
    {
        "������", "SERVICE",
        "�������������� ���������, ����������, ����� �������, �������������� �������",
        "Additional settings, calibration, signal search, mathematical functions"
    },
    itemspService,
    EmptyPressPage
};

// ������ - ����� �������� ---------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON(
    bResetSettings,
    "����� ��������", "Reset settings",
    "����� �������� �� ��������� �� ���������",
    "Reset to default settings",
    pService, EmptyFuncBV, OnPress_ResetSettings, EmptyFuncVII);

void OnPress_ResetSettings(void)
{
    panel.Disable();
    display.SetDrawMode(DrawMode_Hand, Draw_ResetSettings);

    if (panel.WaitPressingButton() == B_Start)
    {
        Settings_Load(true);
    }

    display.SetDrawMode(DrawMode_Auto, 0);
    panel.Enable();
    //Func_Start(1);
}

static void Draw_ResetSettings(void)
{
    painter.BeginScene(gColorBack);

    painter.DrawTextInRectWithTransfersC(30, 110, 300, 200, "����������� ����� �������� �������� ������ ����/����.\n"
                                                         "������� ����� ������ ������, ���� ����� �� �����.", gColorFill);

    painter.EndScene();
}

// ������ - ����� ������� ----------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON(
    bAutoSearch,
    "����� �������", "Find signal",
    "������������� ����������� ��������� ������������ ��� ������� � ������ 1",
    "Sets optimal settings for the oscilloscope signal on channel 1",
    pService, FuncActive, OnPress_AutoSearch, FuncDraw);

static void OnPress_AutoSearch(void)
{
    FPGA_NEED_AUTO_FIND = 1;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(         ppCalibrator,                                                                                           // ������ - ���������� ///
    Page_Service_Calibrator, &pService, FuncActive, EmptyPressPage,
    "����������", "CALIBRATOR",
    "����������� ������������ � ���������� ������������",
    "Control of the calibrator and calibration of an oscillograph",
    cCalibrator_Calibrator, // ������ - ���������� - ����������
    bCalibrator_Calibrate   // ������ - ���������� - �����������
);

//--------------------------------------------------------------------------------------------------------------- ������ - ���������� - ���������� ---
static void OnChanged_Calibrator_Calibrator(bool)
{
    FPGA_SetCalibratorMode(CALIBRATOR_MODE);
}

DEF_CHOICE_3
(
    cCalibrator_Calibrator, ppCalibrator,
    CALIBRATOR_MODE, FuncActive, OnChanged_Calibrator_Calibrator, FuncDraw,
    "����������",   "Calibrator",
    "����� ������ �����������",
    "Mode of operation of the calibrator",
    "�����",    "DC",
    "+4V",      "+4V",
    "0V",       "0V"
);

//-------------------------------------------------------------------------------------------------------------- ������ - ���������� - ����������� ---
static bool IsActive_Calibrator_Calibrate(void)
{
    return !(SET_CALIBR_MODE_A == CalibrationMode_Disable && CALIBR_MODE_B == CalibrationMode_Disable);
}

static void OnPress_Calibrator_Calibrate(void)
{
    gStateFPGA.needCalibration = true;
}

DEF_BUTTON
(
    bCalibrator_Calibrate,
    "�����������", "Calibrate",
    "������ ��������� ����������",
    "Running the calibration procedure",
    ppCalibrator, IsActive_Calibrator_Calibrate, OnPress_Calibrator_Calibrate, FuncDraw
);

#ifdef OLD_RECORDER

//--------------------------------------------------------------------------------------------------------------------------- ������ - ����������� ---
static void OnChanged_Recorder(bool)
{
    FPGA_EnableRecorderMode(RECORDER_MODE);
}

DEF_CHOICE_2
(
    cRecorder, pService,
    RECORDER_MODE, FuncActive, OnChanged_Recorder, FuncDraw,
    "�����������", "Recorder",
    "��������/��������� ����� ������������. ���� ����� �������� �� ��������� 50 ��/��� � ����� ���������.",
    "Turn on/off recorder mode. This mode is available for scanning 20ms/div and slower.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

#else

// C����� - ����������� //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Page ppRecorder =
{
    Item_Page, &pService, 0,
    {
        "�����������", "RECORDER",
        "������ � ��������������� ������� ��������",
        "Recording and playback of input signals"
    },
    PageSB_Service_Recorder,
    {
        (void *)&bRecorder_Exit,
        (void *)&bRecorder_SaveTo,
        (void *)&bRecorder_Choice,
        (void *)&bRecorder_Cursor
    },
    true
};

// ������ - ����������� - ����� ----------------------------------------------------------------------------------------------------------------------
static const SButton bRecorder_Exit =
{
    Item_SmallButton, &ppRecorder, 0,
    {
        "�����", "Exit",
        "����� �� ������ ������������",
        "Exit registrator mode"
    },
    OnPressSB_Exit,
    DrawSB_Exit
};

// ������ - ����������� - ��������� � ... ------------------------------------------------------------------------------------------------------------
static const SButton bRecorder_SaveTo =
{
    Item_SmallButton, &ppRecorder, IsActive_Recorder_SaveTo,
    {
        "��������� � ...", "Save in ...",
        "����� ����� �������� ������",
        "Selecting a storage location"
    },
    OnPress_Recorder_SaveTo,
    Draw_Recorder_SaveTo,
    {
        {Draw_Recorder_SaveTo_RAM, "������ ����������� �� ���������� ������", "data is stored in the internal memory"},
        {Draw_Recorder_SaveTo_EXT, "������ ����������� �� ������� ��", "data is stored in the data storage"}
    }
};

static bool IsActive_Recorder_SaveTo(void)
{
    return FDRIVE_IS_CONNECTED;
}

static void OnPress_Recorder_SaveTo(void)
{
    CircleIncreaseInt8((int8*)(&REC_PLACE_OF_SAVING), 0, PlaceOfSaving_Number - 1);
}

static void Draw_Recorder_SaveTo(int x, int y)
{
    static const pFuncVII funcs[PlaceOfSaving_Number] =
    {
        Draw_Recorder_SaveTo_RAM, Draw_Recorder_SaveTo_EXT
    };

    funcs[REC_PLACE_OF_SAVING](x, y);
}

static void Draw_Recorder_SaveTo_RAM(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_ROM);
    painter.SetFont(TypeFont_8);
}

static void Draw_Recorder_SaveTo_EXT(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_FLASH_DRIVE_BIG);
    painter.SetFont(TypeFont_8);
}

// ������ - ����������� - ����� ----------------------------------------------------------------------------------------------------------------------
static const SButton bRecorder_Choice =
{
    Item_SmallButton, &ppRecorder, 0,
    {
        "�����", "Choice",
        "����� ������� ��� ���������",
        "Select signal to view"
    },
    OnPress_Recorder_Choice,
    Draw_Recorder_Choice
};

static void OnPress_Recorder_Choice(void)
{

}

static void Draw_Recorder_Choice(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 1, y, SYMBOL_FOLDER);
    painter.SetFont(TypeFont_8);
}

// ������ - ����������� - ������ ---------------------------------------------------------------------------------------------------------------------
static const SButton bRecorder_Cursor =
{
    Item_SmallButton, &ppRecorder, 0,
    {
        "������", "Cursors",
        "����� �������",
        "Cursor selection"
    },
    OnPress_Recorder_Cursor,
    Draw_Recorder_Cursor
};

static void OnPress_Recorder_Cursor(void)
{
    CircleIncreaseInt8(&REC_NUM_CURSOR, 0, 1);
}

static void Draw_Recorder_Cursor(int x, int y)
{
    painter.DrawText(x + 8, y + 5, REC_NUM_CURSOR ? "2" : "1");
}

//----------------------------------------------------------------------------------------------------------------------------------------------------



#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������ - ������ ///
static bool IsActive_FFT(void)
{
    return !FUNC_MODE_DRAW_IS_ENABLED;
}

static void OnPress_FFT(void)
{
    if (!IsActive_FFT())
    {
        display.ShowWarning(ImpossibleEnableFFT);
    }
}

DEF_PAGE_6(         ppFFT, 
    Page_Service_FFT, &pService, IsActive_FFT, OnPress_FFT,
    "������", "SPECTRUM",
    "����������� ������� �������� �������",
    "Mapping the input signal spectrum",
    cFFT_View,         // ������ - ������ - �����������
    cFFT_Scale,        // ������ - ������ - �����
    cFFT_Source,       // ������ - ������ - ��������
    cFFT_Window,       // ������ - ������ - ���� 
    pppFFT_Cursors,    // ������ - ������ - �������
    cFFT_Range         // ������ - ������ - ��������
);

//------------------------------------------------------------------------------------------------------------------ ������ - ������ - ����������� ---
DEF_CHOICE_2
(
    cFFT_View, ppFFT,
    FFT_ENABLED, FuncActive, FuncChangedChoice, FuncDraw,
    "�����������", "Display",
    "�������� � ��������� ����������� �������",
    "Enables or disables the display of the spectrum",
     DISABLE_RU,    DISABLE_EN,
     ENABLE_RU,     ENABLE_EN
);

//------------------------------------------------------------------------------------------------------------------------ ������ - ������ - ����� ---
DEF_CHOICE_2
(
    cFFT_Scale, ppFFT,
    SCALE_FFT, FuncActive, FuncChangedChoice, FuncDraw,
    "�����",        "Scale",
    "����� ������� ������ ������� - �������� ��� ���������������",
    "Sets the scale of the output spectrum - linear or logarithmic",
    "��������", "Log",
    "��������", "Linear"
);

//--------------------------------------------------------------------------------------------------------------------- ������ - ������ - �������� ---
DEF_CHOICE_3
(
    cFFT_Source, ppFFT,
    SOURCE_FFT, FuncActive, FuncChangedChoice, FuncDraw,
    "��������",     "Source",
    "����� ��������� ��� ������� �������",
    "Selecting the source for the calculation of the spectrum",
    "����� 1",      "Channel 1",
    "����� 2",      "Channel 2",
    "����� 1 + 2",  "Channel 1 + 2"    
);

//------------------------------------------------------------------------------------------------------------------------- ������ - ������ - ���� ---
DEF_CHOICE_4
(
    cFFT_Window, ppFFT,
    WINDOW_FFT, FuncActive, FuncChangedChoice, FuncDraw,
    "����",         "Window",
    "����� ���� ��� ������� �������",
    "Sets the window to calculate the spectrum",
    "�����������",  "Rectangle",
    "��������",     "Hamming",
    "��������",     "Blackman",
    "�����",        "Hann"        
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_FFT_Cursors(void)
{
    return FFT_ENABLED;
}

static void OnRegSet_FFT_Cursors(int angle)
{
    FFT_POS_CURSOR(MATH_CURRENT_CUR) += (uint8)angle;
    sound.RegulatorShiftRotate();
}

DEF_PAGE_SB(        pppFFT_Cursors,                                                                                   // ������ - ������ - ������� ///
    PageSB_Service_FFT_Cursors, &ppFFT, IsActive_FFT_Cursors,
    EmptyPressPage, FuncDrawPage, OnRegSet_FFT_Cursors,
    "�������", "CURSORS",
    "�������� ������� ��� ��������� ���������� �������",
    "Includes cursors to measure the parameters of the spectrum",
    &bFFT_Cursors_Exit,     // ������ - ������ - ������� - �����
    &bFFT_Cursors_Source,   // ������ - ������ - ������� - ��������
    0,
    0,
    0,
    0
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON_EXIT(  bFFT_Cursors_Exit,                                                                 //--- ������ - ������ - ������� - ����� ---
    pppFFT_Cursors, FuncActive, OnPressSB_Exit, DrawSB_Exit
);

//----------------------------------------------------------------------------------------------------------- ������ - ������ - ������� - �������� ---
static void OnPress_FFT_Cursors_Source(void)
{
    MATH_CURRENT_CUR = (uint8)((MATH_CURRENT_CUR + 1) % 2);
}

static void Draw_FFT_Cursors_Source(int x, int y)
{
    painter.DrawText(x + 7, y + 5, MATH_CURRENT_CUR_IS_0 ? "1" : "2");
}

DEF_SMALL_BUTTON
(
    bFFT_Cursors_Source,
     "��������", "Source",
     "����� ��������� ��� ������� �������",
     "Source choice for calculation of a range",
    pppFFT_Cursors, FuncActive, OnPress_FFT_Cursors_Source, Draw_FFT_Cursors_Source
);

//--------------------------------------------------------------------------------------------------------------------- ������ - ������ - �������� ---
static bool IsActive_FFT_Range(void)
{
    return SCALE_FFT_IS_LOG;
}

DEF_CHOICE_3
(
    cFFT_Range, ppFFT,
    MAX_DB_FFT, IsActive_FFT_Range, FuncChangedChoice, FuncDraw,
    "��������", "Range",
    "����� ����� ������ ������ ���������� �� ��������� �������",
    "Here you can set the limit of monitoring the power spectrum",
    "-40��", "-40dB",
    "-60��", "-60dB",
    "-80��", "-80dB"
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Function(void)
{
    return !FFT_ENABLED;
}

static void OnPress_Function(void)
{
    if (FFT_ENABLED)
    {
        display.ShowWarning(ImpossibleEnableMathFunction);
    }
}

static void OnRegSet_Function(int delta)
{
    if (!FUNC_MODE_DRAW_IS_ENABLED)
    {
        return;
    }

    if (MATH_MODE_REG_SET_IS_RSHIFT)
    {
        uint16 prevRShift = SET_RSHIFT_MATH;
        uint16 rShift = prevRShift;
        if (delta > 0)
        {
            if (rShift < RShiftMax)
            {
                rShift += 4 * STEP_RSHIFT;
                LIMIT_ABOVE(rShift, RShiftMax);
                if (prevRShift < RShiftZero && rShift > RShiftZero)
                {
                    rShift = RShiftZero;
                }
                sound.RegulatorShiftRotate();
                SET_RSHIFT_MATH = rShift;
            }
        }
        else if (delta < 0)
        {
            if (rShift > RShiftMin)
            {
                rShift -= 4 * STEP_RSHIFT;
                LIMIT_BELOW(rShift, RShiftMin);
                if (prevRShift > RShiftZero && rShift < RShiftZero)
                {
                    rShift = RShiftZero;
                }
                sound.RegulatorShiftRotate();
                SET_RSHIFT_MATH = rShift;
            }
        }
    }
    if (MATH_MODE_REG_SET_IS_RANGE)
    {
        static int sum = 0;
        sum -= delta;

        float rShiftAbs = RSHIFT_2_ABS(SET_RSHIFT_MATH, SET_RANGE_MATH);

        if (sum > 2)
        {
            if (SET_RANGE_MATH < RangeSize - 1)
            {
                SET_RANGE_MATH = (Range)((int)SET_RANGE_MATH + 1);  // SET_RANGE_MATH++;
                SET_RSHIFT_MATH = (uint16)RSHIFT_2_REL(rShiftAbs, SET_RANGE_MATH);
                sound.RegulatorSwitchRotate();
            }
            sum = 0;
        }
        else if (sum < -2)
        {
            if (SET_RANGE_MATH > 0)
            {
                SET_RANGE_MATH = (Range)((int)SET_RANGE_MATH - 1);  // SET_RANGE_MATH--;
                SET_RSHIFT_MATH = (uint16)RSHIFT_2_REL(rShiftAbs, SET_RANGE_MATH);
                sound.RegulatorSwitchRotate();
            }
            sum = 0;
        }
    }
}

DEF_PAGE_SB(        ppFunction,                                                                                                // ������ - ������� ///
    PageSB_Service_Function, &pService, IsActive_Function,
    OnPress_Function, FuncDrawPage, OnRegSet_Function,
    "�������", "FUNCTION",
    "��������� � ����� �������������� ������� - �������� ��� ���������",
    "Installation and selection of mathematical functions - addition or multiplication",
    &bFunction_Exit,        // ������ - ������� - �����
    &bFunction_Screen,      // ������ - ������� - �����
    &bFunction_Type,        // ������ - ������� - ���
    &bFunction_ModeRegSet,  // ������ - ������� - ����� ����� ���������
    &bFunction_RangeA,      // ������ - ������� - ������� 1-�� ������
    &bFunction_RangeB
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON_EXIT(  bFunction_Exit,                                                                             //--- ������ - ������� - ����� ---
    ppFunction, FuncActive, FuncPress, DrawSB_Exit
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_Screen(void)
{
    if (FFT_ENABLED)
    {
        display.ShowWarning(ImpossibleEnableMathFunction);
    }
    else
    {
        CircleIncrease<int8>((int8 *)&FUNC_MODE_DRAW, 0, 2);
    }
}

static void Draw_Function_Screen(int x, int y)
{
    const pFuncVII funcs[3] =
    {
        Draw_Function_Screen_Disable,
        Draw_Function_Screen_Separate,
        Draw_Function_Screen_Together
    };
    funcs[FUNC_MODE_DRAW](x, y);
}

static void Draw_Function_Screen_Disable(int x, int y)
{
    painter.DrawText(x + 2 + (LANG_EN ? 2 : 0), y + 5, DICT(DDis));
}

static void Draw_Function_Screen_Separate(int x, int y)
{
    painter.DrawRectangle(x + 3, y + 5, 13, 9);
    painter.DrawHLine(y + 9, x + 3, x + 16);
    painter.DrawHLine(y + 10, x + 3, x + 16);
}

static void Draw_Function_Screen_Together(int x, int y)
{
    painter.DrawRectangle(x + 3, y + 5, 13, 9);
}

DEF_SMALL_BUTTON_HINTS_3(   bFunction_Screen,                                                                       //--- ������ - ������� - ����� ---
    "�����", "Display",
    "�������� ����� ����������� ��������������� �������",
    "Chooses the mode of display of a mathematical signal",
    ppFunction, FuncActive, OnPress_Function_Screen, Draw_Function_Screen,
    Draw_Function_Screen_Disable,   "����� �������������� ������� ��������",     
                                    "The conclusion of mathematical function is disconnected",
    Draw_Function_Screen_Separate,  "������� � �������������� ������� ��������� � ������ �����",
                                    "Signals and mathematical function are removed in different windows",
    Draw_Function_Screen_Together,  "������� � �������������� ������� ��������� � ����� ����",
                                    "Signals and mathematical function are removed in one window"
);

//------------------------------------------------------------------------------------------------------------------------- ������ - ������� - ��� ---
static void OnPress_Function_Type(void)
{
    CircleIncrease<int8>((int8 *)&MATH_FUNC, 0, 1);
}

static void Draw_Function_Type(int x, int y)
{
    const pFuncVII funcs[2] = {Draw_Function_Type_Sum, Draw_Function_Type_Mul};
    funcs[MATH_FUNC](x, y);
}

static void Draw_Function_Type_Sum(int x, int y)
{
    painter.DrawHLine(y + 9, x + 4, x + 14);
    painter.DrawVLine(x + 9, y + 4, y + 14);
}

static void Draw_Function_Type_Mul(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 4, y + 3, SYMBOL_MATH_FUNC_MUL);
    painter.SetFont(TypeFont_8);
}
DEF_SMALL_BUTTON_HINTS_2
(
    bFunction_Type,
    "���", "Type",
    "����� �������������� �������",
    "Choice of mathematical function",
    ppFunction, FuncActive, OnPress_Function_Type, Draw_Function_Type,
    Draw_Function_Type_Sum, "��������",     "Addition",
    Draw_Function_Type_Mul, "���������",    "Multiplication"
);

//------------------------------------------------------------------------------------------------------- ������ - ������� - ����� ����� ��������� ---
static void OnPress_Function_ModeRegSet(void)
{
    CircleIncrease<int8>((int8 *)&MATH_MODE_REG_SET, 0, 1);
}

static void Draw_Function_ModeRegSet(int x, int y)
{
    static const pFuncVII funcs[2] = {Draw_Function_ModeRegSet_Range, Draw_Function_ModeRegSet_RShift};
    funcs[MATH_MODE_REG_SET](x, y);
}

static void Draw_Function_ModeRegSet_Range(int x, int y)
{
    painter.DrawChar(x + 7, y + 5, LANG_RU ? 'M' : 'S');
}

static void Draw_Function_ModeRegSet_RShift(int x, int y)
{
    painter.DrawText(x + 5 - (LANG_EN ? 3 : 0), y + 5, DICT(DShift));
}

DEF_SMALL_BUTTON_HINTS_2
(
    bFunction_ModeRegSet,
    "����� ����� ���������", "Mode regulator SET",
    "����� ������ ����� ��������� - ���������� ��������� ��� ���������",
    "Choice mode regulcator ��������� - management of scale or shift",
    ppFunction, FuncActive, OnPress_Function_ModeRegSet, Draw_Function_ModeRegSet,
    Draw_Function_ModeRegSet_Range,  "���������� ���������", "Management of scale",
    Draw_Function_ModeRegSet_RShift, "���������� ���������", "Management of shift"
);

//--------------------------------------------------------------------------------------------------------- ������ - ������� - ������� 1-�� ������ ---
static void OnPress_Function_RangeA(void)
{
    SET_RANGE_MATH = SET_RANGE_A;
    MATH_DIVIDER = SET_DIVIDER_A;
}

static void Draw_Function_RangeA(int x, int y)
{
    painter.DrawChar(x + 8, y + 5, '1');
}

DEF_SMALL_BUTTON
(
    bFunction_RangeA,
    "������� 1-�� ������", "Scale of the 1st channel",
    "���������� ������� ������� ������ ��� ����������� ����������",
    "Takes scale for a mathematical signal from the first channel",
    ppFunction, FuncActive, OnPress_Function_RangeA, Draw_Function_RangeA
);

//--------------------------------------------------------------------------------------------------------- ������ - ������� - ������� 2-�� ������ ---
static void OnPress_Function_RangeB(void)
{
    SET_RANGE_MATH = SET_RANGE_B;
    MATH_DIVIDER = SET_DIVIDER_B;
}

static void Draw_Function_RangeB(int x, int y)
{
    painter.DrawChar(x + 8, y + 5, '2');
}

DEF_SMALL_BUTTON
(
    bFunction_RangeB,
    "������� 2-�� ������", "Scale of the 2nd channel",
    "���������� ������� ������� ������ ��� ����������� ����������",
    "Takes scale for a mathematical signal from the second channel",
    ppFunction, FuncActive, OnPress_Function_RangeB, Draw_Function_RangeB
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������ - ETHERNET ///
DEF_PAGE_5(         ppEthernet,
    Page_Service_Ethernet, &pService, FuncActive, EmptyPressPage,
    "ETHERNET", "ETHERNET",
    "��������� ethernet",
    "Settings of ethernet",
    cEthernet_Ethernet, // ������ - ETHERNET - Ethernet
    ipEthernet_IP,      // ������ - ETHERNET - IP �����
    ipEthernet_NetMask, // ������ - ETHERNET - ����� �������
    ipEthernet_Gateway, // ������ - ETHERNET - ����
    ipEthernet_MAC      // ������ - ETHERNET - MAC �����
);

//------------------------------------------------------------------------------------------------------------------- ������ - ETHERNET - Ethernet ---
static void OnChanged_Ethernet_Settings(bool)
{
    display.ShowWarning(NeedRebootDevice);
}

DEF_CHOICE_2
(
    cEthernet_Ethernet, ppEthernet,
    ETH_ENABLED, FuncActive, OnChanged_Ethernet_Settings, FuncDraw,
    "Ethernet",    "Ethernet",
    "����� ������������� ethernet, �������� \"��������\" � ��������� ������.\n"
    "����� ��������� ethernet, �������� \"���������\" � ��������� ������.",
    "To involve ethernet, choose \"Included\" and switch off the device.\n"
    "To disconnect ethernet, choose \"Disconnected\" and switch off the device.",
    "��������",     "Included",
    "���������",    "Disconnected"
);

// ������ - ETHERNET - IP ����� ----------------------------------------------------------------------------------------------------------------------
static const IPaddress ipEthernet_IP =
{
    Item_IP, &ppEthernet, 0,
    {
        "IP �����", "IP-address",
        "��������� IP ������",
        "Set of IP-address"
    },
    &IP_ADDR0, &IP_ADDR1, &IP_ADDR2, &IP_ADDR3,
    OnChanged_Ethernet_Settings,
    &ETH_PORT
};

// ������ - ETHERNET - ����� ������� -----------------------------------------------------------------------------------------------------------------
static const IPaddress ipEthernet_NetMask =
{
    Item_IP, &ppEthernet, 0,
    {
        "����� �������", "Network mask",
        "��������� ����� �������",
        "Set of network mask"
    },
    &NETMASK_ADDR0, &NETMASK_ADDR1, &NETMASK_ADDR2, &NETMASK_ADDR3,
    OnChanged_Ethernet_Settings
};

// ������ - ETHERNET - ���� --------------------------------------------------------------------------------------------------------------------------
static const IPaddress ipEthernet_Gateway =
{
    Item_IP, &ppEthernet, 0,
    {
        "����", "Gateway",
        "��������� ������ ��������� �����",
        "Set of gateway address"
    },
    &GW_ADDR0, &GW_ADDR1, &GW_ADDR2, &GW_ADDR3,
    OnChanged_Ethernet_Settings
};

// ������ - ETHERNET - MAC ����� ---------------------------------------------------------------------------------------------------------------------
static const MACaddress ipEthernet_MAC =
{
    Item_MAC, &ppEthernet, 0,
    {
        "MAC �����", "MAC-address",
        "��������� ����������� ������",
        "Set of MAC-address"
    },
    &set.eth_mac0, &set.eth_mac1, &set.eth_mac2, &set.eth_mac3, &set.eth_mac4, &set.eth_mac5,
    OnChanged_Ethernet_Settings
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(         ppSound,                                                                                                      // ������ - ���� ///
    Page_Service_Sound, &pService, FuncActive, EmptyPressPage,
    "����", "SOUND",
    "� ���� ���� ����� ��������� ��������� �����",
    "In this menu, you can adjust the volume",
    cSound_Enable,  // ������ - ���� - ����
    gSound_Volume   // ������ - ���� - ���������
);

//--------------------------------------------------------------------------------------------------------------------------- ������ - ���� - ���� ---
DEF_CHOICE_2
(
    cSound_Enable, ppSound,
    SOUND_ENABLED, FuncActive, FuncChangedChoice, FuncDraw,
    "����", "Sound",
    "���������/���������� �����",
    "Inclusion/switching off of a sound",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN
);

//---------------------------------------------------------------------------------------------------------------------- ������ - ���� - ��������� ---
DEF_GOVERNOR
(
    gSound_Volume,
    "���������", "Volume",
    "��������� ��������� �����",
    "Set the volume",
    ppSound, SOUND_VOLUME, 0, 100, FuncActive, FuncChanged, FuncBeforeDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(         ppRTC,                                                                                                       // ������ - ����� ///
    Page_Service_RTC, &pService, FuncActive, EmptyPressPage,
    "�����", "TIME",
    "��������� � ��������� �������",
    "Set and setup time",
    tRTC_Time,          // ������ - ����� - �����
    tRTC_Correction     // C����� - ����� - ���������
);

//------------------------------------------------------------------------------------------------------------------------- ������ - ����� - ����� ---
static int8 dServicetime = 0;
static int8 hours = 0, minutes = 0, secondes = 0, year = 0, month = 0, day = 0;
DEF_TIME(
    tRTC_Time,
    "�����", "Time",
    "��������� �������� �������.\n������� ������:\n"
    "������ �� ������� ���� \"�����\". ��������� ���� ��������� �������� �������. ��������� ��������� ������ �� �������� ����������, �������������� "
    "�������� ���������� \"�����\", �������� ����, ������, �������, ���, �����, ��� �����. ���������� ������� ������������ �������� ��������. "
    "��������� ����� ��������� ���������� ����������� ��������. ����� �������� ����� \"���������\", ������ � ���������� ����� 0.5 ��� ������ �� ������ "
    "����������. ���� ��������� �������� ������� ��������� � ����������� ������ �������� �������. ������� ���������� ��������� ������ �� ����� ������ �������� "
    "������� � �������� ���� ��������� �������� ���    ���� ��� ���������� ������ �������� �������"
    ,
    "Setting the current time. \nPoryadok work:\n"
    "Click on the menu item \"Time\".The menu set the current time.By briefly pressing the button on the numeric keypad of conformity "
    "Control \"Time\", highlight the hours, minutes, seconds, year, month, or a number.The selected item is indicated by a flashing area. "
    "Turn the setting knob to set the desired value. Then highlight \"Save\", press and udrezhivat more than 0.5 seconds, the button on the panel "
    "Control. Menu Setting the current time will be closed to the conservation of the new current time. Pressing a button on the prolonged retention of any other element "
    "will lead to the closure of the current time setting menu without saving the new current time",
    ppRTC, FuncActive, dServicetime, hours, minutes, secondes, month, day, year);

//--------------------------------------------------------------------------------------------------------------------- ������ - ����� - ��������� ---
static void OnChanged_Time_Correction(void)
{
    RTC_SetCorrection((int8)NRST_CORRECTION_TIME);
}

DEF_GOVERNOR
(
    tRTC_Correction,
    "���������", "Correction",
    "��������� ��������������� ������������ ��� ����������� ���� �������",
    "Setting correction factor to compensate for time travel",
    ppRTC, NRST_CORRECTION_TIME, -63, 63, FuncActive, OnChanged_Time_Correction, FuncBeforeDraw
);

//---------------------------------------------------------------------------------------------------------------------------------- ������ - ���� ---
DEF_CHOICE_2
(
    cLanguage, pService,
    LANG, FuncActive, FuncChangedChoice, FuncDraw,
    "Language",     "����",
    "��������� ������� ���� ����",
    "Allows you to select the menu language",
    "�������",      "Russian",
    "����������",   "English"
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Information(void)
{
    display.SetDrawMode(DrawMode_Auto, Information_Draw);
}

static void Information_Draw(void)
{
    painter.BeginScene(gColorBack);
    int x = 100;
    int dY = 20;
    int y = 20;
    painter.DrawRectangle(0, 0, 319, 239, gColorFill);
    y += dY;
    painter.DrawText(x, y, DICT(DInformation));
    y += dY;
    
    char buffer[100];

    sprintf(buffer, "%s : %s", DICT(DModel), LANG_RU ? MODEL_RU : MODEL_EN);
    painter.DrawText(x, y, buffer);

    y += 2 * dY;

    painter.DrawText(x, y, DICT(DSoftware));
    y += dY;
    sprintf(buffer, (const char *)DICT(DVersion), NUM_VER);
    painter.DrawText(x, y, buffer);
    y += dY;

    painter.DrawFormText(x, y, gColorFill, "CRC32 : %X", Hardware_CalculateCRC32());

    dY = -10;
    painter.DrawStringInCenterRect(0, 190 + dY, 320, 20, "��� ��������� ������ ������� � ����������� ������ ������");
#ifdef S8_54
    painter.DrawStringInCenterRect(0, 205 + dY, 320, 20, "����� ����������: ���./����. 8-017-262-57-50");
    painter.DrawStringInCenterRect(0, 220 + dY, 320, 20, "������������: e-mail: mnipi-24(@)tut.by, ���. 8-017-262-57-51");
#endif

    menu.Draw();
    painter.EndScene();
}

DEF_PAGE_SB(        ppInformation,                                                                                          // ������ - ���������� ///
    PageSB_Service_Information, &pService, FuncActive,
    OnPress_Information, FuncDrawPage, FuncRegSetPage,
    "����������", "INFORMATION",
    "���������� ���������� � �������",
    "Displays information about the device",
    &bInformation_Exit, // ������ - ���������� - �����
    0,
    0,
    0,
    0,
    0
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Information_Exit(void)
{
    display.SetDrawMode(DrawMode_Auto, 0);
}

DEF_SMALL_BUTTON_EXIT(  bInformation_Exit,                                                                       //--- ������ - ���������� - ����� ---
    ppInformation, FuncActive, OnPress_Information_Exit, DrawSB_Exit
);








































/*
//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnPressPrevSettings(void)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnChangedColorMenuTitle(void)
{
    uint16 newColor1 = painter.ReduceBrightness(COLOR(COLOR_MENU_TITLE), 0.50f);
    uint16 newColor2 = painter.ReduceBrightness(COLOR(COLOR_MENU_TITLE), 1.50f);
    COLOR(Color::MENU_TITLE_DARK) = newColor1;
    COLOR(COLOR_MENU_TITLE_BRIGHT) = newColor2;
    Color_Log(COLOR_MENU_TITLE);
    Color_Log(Color::MENU_TITLE_DARK);
    Color_Log(COLOR_MENU_TITLE_BRIGHT);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnChangedColorMenu2Item(void)
{
    uint16 newColor1 = painter.ReduceBrightness(COLOR(COLOR_MENU_ITEM), 0.50f);
    uint16 newColor2 = painter.ReduceBrightness(COLOR(COLOR_MENU_ITEM), 1.50f);
    COLOR(COLOR_MENU_ITEM_DARK) = newColor1;
    COLOR(COLOR_MENU_ITEM_BRIGHT) = newColor2;
    Color_Log(COLOR_MENU_ITEM);
    Color_Log(COLOR_MENU_ITEM_DARK);
    Color_Log(COLOR_MENU_ITEM_BRIGHT);
}


// ������ ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const Page pService;

// ������ - ����� ��������
const Button mbServicePreviousSettings =
{
    Item_Button, &pService, 0,
    {
        "����� ��������", "Return settings",
        "���������� ��������� ������������, ������� ���� � ������ ������� \"����� �������\"",
        "Returns the oscilloscope settings that were in when you press \"Searching for signal\""
    },
    OnPressPrevSettings
};

static bool ActiveF_MathFormula(void)
{
    return MATH_FUNC_IS_MUL || MATH_FUNC_IS_SUM;
}

static void ChangeF_MathFormula(void)
{

}

static int8 curDigit = 0;
const Formula mfMathFormula =
{
    Item_Formula, &ppFunction, ActiveF_MathFormula,
    {
        "�������", "Formulf",
        "����� �������� ������������ � ����� � �������������� �������",
        "Here you can set the coefficients and signs in a mathematical formula"
    },
    (int8 *)&MATH_FUNC, (int8 *)&set.math_koeff1add, (int8 *)&set.math_koeff2add, (int8 *)&set.math_koeff1mul, (int8 *)&set.math_koeff2mul, &curDigit, ChangeF_MathFormula
};
*/
