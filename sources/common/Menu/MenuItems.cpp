#include "MenuItems.h"
#include "Menu.h"
#include "Hardware/Sound.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern int8 gCurDigit;

#define NAME_FROM_INDEX(index) (names[index * 2 + LANG])


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *Choice::NameCurrentSubItem()
{
    return ((int8 *)cell == 0) ? "" : NAME_FROM_INDEX(*cell); // names[*((int8 *)cell)][LANG];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *Choice::NameNextSubItem()
{
    if (cell == 0)
    {
        return "";
    }

    int index = *((int8 *)cell) + 1;

    if (index == NumSubItems())
    {
        index = 0;
    }
    return NAME_FROM_INDEX(index);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *Choice::NamePrevSubItem()
{
    if (cell == 0)
    {
        return "";
    }

    int index = *((int8 *)cell) - 1;

    if (index < 0)
    {
        index = NumSubItems() - 1;
    }
    return NAME_FROM_INDEX(index);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Control *Page::Item(int numElement) const
{
    return (Control *)items[numElement + (isPageSB ? 1 : 0)];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
SButton* Page::SmallButonFromPage(int numButton)
{
    return (SButton *)items[numButton];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Button::CallFuncOnDraw(int x, int y)
{
    if (funcForDraw)
    {
        funcForDraw(x, y);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Page::PosItemOnTop()
{
    return NumCurrentSubPage() * MENU_ITEMS_ON_DISPLAY;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *Choice::NameSubItem(int i)
{
    return NAME_FROM_INDEX(i);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Page::ShortPressOnItem(int numItem)
{
    ((Button *)items[numItem])->funcOnPress();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int8 Page::PosCurrentItem() const
{
    return MENU_POS_ACT_ITEM(name) & 0x7f;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Page::ChangeSubPage(int delta) const
{
    if (delta > 0 && MenuCurrentSubPage(name) < NumSubPages() - 1)
    {
        Sound::RegulatorSwitchRotate();
        SetMenuCurrentSubPage(name, MenuCurrentSubPage(name) + 1);
    }
    else if (delta < 0 && MenuCurrentSubPage(name) > 0)
    {
        Sound::RegulatorSwitchRotate();
        SetMenuCurrentSubPage(name, MenuCurrentSubPage(name) - 1);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsActive() const
{
    return funcOfActive ? funcOfActive() : true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
PageBase *Control::Keeper() const
{
    return (PageBase *)keeper;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsPageSB() const
{
    return isPageSB;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Control::HeightOpened() const
{
    if (type == Item_Page)
    {
        int numItems = ((const Page *)this)->NumItems() - ((Page *)this)->NumCurrentSubPage() * MENU_ITEMS_ON_DISPLAY;
        LIMITATION(numItems, 0, MENU_ITEMS_ON_DISPLAY);
        return MP_TITLE_HEIGHT + MI_HEIGHT * numItems;
    }
    else if (type == Item_Choice || type == Item_ChoiceReg)
    {
        return MOI_HEIGHT_TITLE + ((Choice *)this)->NumSubItems() * MOSI_HEIGHT - 1;
    }
    return MI_HEIGHT;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsShade() const
{
    return keeper->CurrentItemIsOpened() && (this != Menu::OpenedItem());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsPressed() const
{
    return this == Menu::ItemUnderKey();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Control::SetCurrent(bool active)
{
    Page *page = (Page *)keeper;
    if (!active)
    {
        SetMenuPosActItem(page->name, 0x7f);
    }
    else
    {
        for (int i = 0; i < page->NumItems(); i++)
        {
            if (page->Item(i) == this)
            {
                SetMenuPosActItem(page->name, (int8)i);
                return;
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsOpened() const
{
    if (type == Item_Page)
    {
        return keeper->CurrentItemIsOpened();
    }
    return (MENU_POS_ACT_ITEM(keeper->name) & 0x80) != 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Control::Open(bool open)
{
    Page *parent = (Page *)keeper;
    SetMenuPosActItem(parent->GetNamePage(), open ? (parent->PosCurrentItem() | 0x80) : (parent->PosCurrentItem() & 0x7f));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *Control::Title() const
{
    return titleHint[LANG];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::ChangeOpened(int delta)
{
    if (delta < 2 && delta > -2)
    {
        return false;
    }

    if (type == Item_Page)
    {
        ((const Page *)this)->ChangeSubPage(delta);
    }
    else if (type == Item_IP)
    {
        ((IPaddress *)this)->ChangeValue(delta);
    }
    else if (type == Item_MAC)
    {
        ((MACaddress *)this)->ChangeValue(delta);
    }
    else if (type == Item_ChoiceReg || type == Item_Choice)
    {
        ((Choice *)this)->ChangeIndex(MENU_IS_SHOWN ? delta : -delta);
    }
    else if (type == Item_Governor)
    {
        ((Governor *)this)->ChangeValue(delta);
    }

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
#define CURRENT_ITEM_IS_OPENED return _GET_BIT(MENU_POS_ACT_ITEM(name), 7) == 1

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool PageBase::CurrentItemIsOpened() const
{
    CURRENT_ITEM_IS_OPENED;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Page::CurrentItemIsOpened() const
{
    CURRENT_ITEM_IS_OPENED;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Control::ShortPress()
{
    if(type == Item_Choice)
    {
        Choice *choice = (Choice *)this;
        if(!IsActive())
        {
            choice->funcOnChanged(false);
        }
        else if(!IsOpened())
        {
            choice->SetCurrent(Menu::CurrentItem() != this);
            choice->StartChange(1);
        }
        else
        {
            choice->ChangeIndex(-1);
        }
    }
    else if(type == Item_Button)
    {
        if(IsActive())
        {
            SetCurrent(true);
            ((Button *)this)->funcOnPress();
        }
    }
    else if(type == Item_Page)
    {
        Page *page = (Page *)this;
        page->funcOnPress();
        page->SetCurrentSB();
    }
    else if(type == Item_Governor)
    {
        if(IsActive())
        {
            Governor *governor = (Governor *)this;
            if(Menu::OpenedItem() == this)
            {
                governor->NextPosition();
            }
            else
            {
                governor->SetCurrent(Menu::CurrentItem() != this);
            }
        }
    }
    else if(type == Item_Time)
    {
        Time *time = (Time *)this;
        if(!IsOpened())
        {
            SetCurrent(true);
            time->SetOpened();
            Open(true);
        }
        else
        {
            time->SelectNextPosition();
        }
    }
    else if(type == Item_IP)
    {
        if(Menu::OpenedItem() == this)
        {
            ((IPaddress *)this)->NextPosition();
        }
    }
    else if(type == Item_GovernorColor)
    {
        if(IsActive())
        {
            GovernorColor *governor = (GovernorColor *)this;
            if(Menu::OpenedItem() == this)
            {
                CircleIncrease<int8>(&governor->ct->currentField, 0, 3);
            }
            else
            {
                if(Menu::CurrentItem() != this)
                {
                    SetCurrent(true);
                }
                Open(!IsOpened());
            }
        }
    }
    else if(type == Item_MAC)
    {
        if(Menu::OpenedItem() == this)
        {
            CircleIncrease<int8>(&gCurDigit, 0, 5);
        }
    }
    else if(type == Item_ChoiceReg)
    {
        Choice *choice = (Choice *)this;
        if(IsActive())
        {
            choice->SetCurrent(Menu::CurrentItem() != this);
        }
        else
        {
            choice->funcOnChanged(false);
        }
    }
    else if(type == Item_SmallButton)
    {
        SButton *button = (SButton *)this;
        button->funcOnPress();
        Menu::itemUnderKey = this;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Control::LongPress()
{
    if(type == Item_Button)
    {
        ((Button *)this)->ShortPress();
    }
    else if(type == Item_Time)
    {
        if(Menu::CurrentItem() != this)
        {
            SetCurrent(true);
        }
        Time *time = (Time *)this;
        if(IsOpened() && (*time->curField == iSET))
        {
            time->SetNewTime();
        }
        Open(!IsOpened());
        time->SetOpened();
    }
    else if(type == Item_Formula)
    {
    }
    else if(type == Item_SmallButton)
    {
        SButton *button = (SButton *)this;
        button->funcOnPress();
        Menu::itemUnderKey = this;
    }
    else
    {
        if(Menu::CurrentItem() != this)
        {
            SetCurrent(true);
        }
        Open(!IsOpened());
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsChoiceReg() const
{
    return type == Item_ChoiceReg;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsGovernor() const
{
    return type == Item_Governor;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsIP() const
{
    return type == Item_IP;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsMAC() const
{
    return type == Item_MAC;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsPage() const
{
    return type == Item_Page;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsChoice() const
{
    return type == Item_Choice;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsGovernorColor() const
{
    return type == Item_GovernorColor;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsTime() const
{
    return type == Item_Time;
}
