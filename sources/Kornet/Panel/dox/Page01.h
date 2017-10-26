//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \page page1 ��������������� ������������

    \tableofcontents

    \section section1 ���������� �����

    \code
    +----+-------+--------------+----+-------------+----+-------------------------------------------------------------------------------------------------+
    | �  | ����. |    437ZIT         |   429VIT6        |                                                                                                 |
    |����|       |     FPGA          |   ������         |                                                                                                 |
    +----+-------+--------------+----+-------------+----+-------------------------------------------------------------------------------------------------+
    | 1  | DAC   | 40  PA4 DAC1 |    |             |    |                                                                                                 |
    | 2  |       |              |    |             |    |                                                                                                 |
    | 3  | AD0   | 85  PD14 D0  | IO | 97 PE0  DA0 | IO |                                                                                                 |
    | 4  | AD1   | 86  PD15 D1  | IO | 98 PE1  DA1 | IO |                                                                                                 |
    | 5  | AD2   | 114 PD0  D2  | IO | 1  PE2  DA2 | IO |                                                                                                 |
    | 6  | AD3   | 115 PD1  D3  | IO | 2  PE3  DA3 | IO |                                                                                                 |
    | 7  | AD4   | 58  PE7  D4  | IO | 3  PE4  DA4 | IO |                                                                                                 |
    | 8  | AD5   | 59  PE8  D5  | IO | 4  PE5  DA5 | IO |                                                                                                 |
    | 9  | AD6   | 60  PE9  D6  | IO | 5  PE6  DA6 | IO |                                                                                                 |
    | 10 | AD7   | 63  PE10 D7  | IO | 38 PE7  DA7 | IO |                                                                                                 |
    | 11 |       |              |    |             |    |                                                                                                 |
    | 12 |       | 119 PD5  NWE |    | 86 PD5  NWE |    | �� ������������                                                                                 |
    | 13 |       | 118 PD4  NOE |    | 85 PD4  NOW |    | �� ������������                                                                                 |
    | 14 |       | 127 PG12 NE4 | O  | 7  PC13     | I  | �������� - ������������� ����������� ������ �� ����, ���� - ������������� ������ ������ � ���� |                                                                    |
    | 15 | PAN_0 | 43  PA7      | I  | 8  PC14     | O  | 00 - BUSY - ������ ������                                                                       |
    | 16 | PAN_1 | 44  PC4      | I  | 9  PC15     | O  | 01 - READY_TRANSMIT - � ������ ���� ������ ��� ��������                                         |
    |    |       |              |    |             |    | 10 - READY_RECEIVE - ������ ������ � �����                                                     |
    |    |       |              |    |             |    | 11 - RECEIVE_TRNSMIT_CONFIRM - ������������� �����/�������� �����                              |
    +----+-------+--------------+----+-------------+----+-------------------------------------------------------------------------------------------------+

    \endcode

    \section section2 ����� �������
        
        \subsection subsec1 ������ ������ � ������
        0. ��������� ��������� ������
            NE4 - "1". ���������� � "0" ����� ����������� �� ���� ������ �����.
        1. ���� SVL == BUSY :
            - �����.
        2. ���� PAN == READY_TRANSMIT :
            - ������� � ������������ ����� ������,
            - ������� � �.0
        3. ���� PAN == READY_RECEIVE :
            - ��������� �� ���� ������ ����, ����������� NE4 � "0".
        4. ���� PAN == READY_RECEIVE :
            - �����.
        5. ���� PAN == READY_TRANSMIT :
            - ������� � ������������ ����� ������,
            - ������� � �.0.
        6. ���� PAN == RECEIVE_TRANSMIT_CONFIRM :
            - ����������� NE4 � "1"
            - ������� � ���������� ����� � ������ ��������
        7. ���� ����� ���� - �����.
        8. ������� � �.0

        \subsection subsec2 ������ ������ � ������
        0. ��������� ��������� ������
            NE4 - "1". ���������� � "0" ����� ������ ������ � ����.
        1. ��������� NE4 � "0". ��� ��������, ��� ������ ����� � ����� �����.
        2. ���� PAN == READY_TRANSMIT :
            - �����.
        3. ���� PAN == RECEIVE_TRANSMIT_CONFIRM :
            - ������� ���� ������ � ����.
            - ��������� "1" �� NE4. ��� ��������, ��� ���� ������.
        4. ���� PAN == RECEIVE_TRANSMIT_CONFIRM :
            - �����.
        5. ���� PAN == READY_TRANSMIT :
            - ������� � �.0
        6. �����.


    \section section3 ������ � ������-�����������

    \code
    +----+----------+------+-----+--------------------------------------------------------+
    | �  |          |      |     |                                                        |
    |����|          |      |     |                                                        |
    +----+----------+------+-----+--------------------------------------------------------+
    | 41 | DAC_TEST | PA5  | ��� | ���� ����� ���������� � ����������� � ��������� ����� |
    | 53 | TEST_ON  | PF13 | OUT | ��������� ������-����������. �������� ������� - 0      |
    | 54 | PNP      | PF14 | OUT | ������������� ���������� - 1, ������������� - 0        |
    | 55 | U        | PF15 | OUT | 1 - ���������� �����������, 0 - ���������� �����       |
    | 56 | I        | PG0  | OUT | 1 - ���������� �����, 0 - ���������� �����������       |
    | 99 | TEST_STR | PC9  | IN  | �������� �������                                       |
    +----+----------+------+-----+--------------------------------------------------------+
    \endcode
*/
