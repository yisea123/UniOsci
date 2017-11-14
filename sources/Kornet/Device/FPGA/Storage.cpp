#include "Storage.h"
#include "Settings/Settings.h"
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Storage storage;

uint8 data[2][4096];

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Storage::AddData(uint8 *dataA, uint8 *dataB)
{
    memcpy(&data[0][0], dataA, NUM_POINTS);
    memcpy(&data[1][0], dataB, NUM_POINTS);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Storage::GetData(uint8 **dataA, uint8 **dataB)
{
    *dataA = &data[0][0];
    *dataB = &data[1][0];
}