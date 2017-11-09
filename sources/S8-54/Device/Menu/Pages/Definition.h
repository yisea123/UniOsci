#pragma once
#include "defines.h"
#include "Panel/Controls.h"
#include "Settings/SettingsTypes.h"
#include "Menu/MenuItems.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define COMMON_BEGIN_SB_EXIT  "�����", "Exit", "������ ��� ������ � ���������� ����", "Button to return to the previous menu",


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ���������� �������� ����, ������� ������ ����������� �� ������� ������ button.
const void *PageForButton(PanelButton button);
/// ���������� true, ���� item - ����� ������� �������� ����.
bool IsMainPage(const void *item);
/// ������� ��������� ������ ������ �� ������ ������ ����.
void DrawSB_Exit(int x, int y);
/// ������������� ������� ��������� ������� ������ ������ �� ������ ������ ����.
void OnPressSB_Exit(void);

/*
#ifdef __cplusplus
extern "C" {
#endif
*/

extern const Page mainPage;

/*  
#ifdef __cplusplus
}
#endif
*/
