#pragma once
#include "Panel/Controls.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Menu
 *  @{
 *  @defgroup MenuDrawing Menu Drawing
 *  @brief ������� ��������� ����
 *  @{
 */

/// ���������� ����
void Menu_Draw(void);
/// ���������� ����� �������� ����, ���������������� ������ button
void *ItemUnderButton(PanelButton button);
/// ���������� true, ���� ������, ��������������� �������� ���� item, ��������� � ������� ���������
bool IsPressed(void *item                   ///< ��������� �� ������� ����
                );
/// @brief ���������� true, ���� ������� ���� item ������� (��������� �� �� ����� ������� ����. ��� �������, ��� ��������, ��� ������� �������������� ������� 
/// ���� ����� Choice ��� Governor
bool IsShade(void *item                     ///< ��������� �� ������� ����
            );

/** @}  @}
 */
