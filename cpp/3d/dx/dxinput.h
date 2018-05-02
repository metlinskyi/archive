#pragma comment ( lib, "dinput8.lib" )
#pragma comment ( lib, "dxguid.lib" )


#ifndef _DXINPUT_H_
#define _DXINPUT_H_

#include <dinput.h>


// Struct to store the current input state
struct UserInput
{
    BYTE diks[256];   // DirectInput keyboard state buffer 

    // TODO: change as needed
    BOOL bUp;
    BOOL bDown;
    BOOL bLeft;
    BOOL bRight;
    BOOL bFirstWeapon;
    BOOL bSecendWeapon;
	BOOL bCamera;
	BOOL bAxil;

};

bool InitInput( HWND);
void CleanupDirectInput();
void UpdateInput(UserInput*);

#endif

