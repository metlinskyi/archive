#define STRICT
#define DIRECTINPUT_VERSION 0x0800

#include "dxinput.h"

LPDIRECTINPUT8          m_pDI;						// DirectInput object
LPDIRECTINPUTDEVICE8    m_pKeyboard;				// DirectInput keyboard device


bool InitInput( HWND hWnd )
{
    // Create a IDirectInput8*
    if( FAILED(DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (VOID**)&m_pDI, NULL ) ) )
	{
        return false;
	}

    // Create a IDirectInputDevice8* for the keyboard
    if( FAILED(m_pDI->CreateDevice( GUID_SysKeyboard, &m_pKeyboard, NULL ) ) )
	{
        return false;
	}
    
    // Set the keyboard data format
    if( FAILED(m_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
	{
        return false;
	}
    
    // Set the cooperative level on the keyboard
    if( FAILED( m_pKeyboard->SetCooperativeLevel( hWnd, 
                                            DISCL_NONEXCLUSIVE | 
                                            DISCL_FOREGROUND | 
                                            DISCL_NOWINKEY ) ) )
	{
        return false;
	}

    // Acquire the keyboard
    m_pKeyboard->Acquire();

    return true;
}


void CleanupDirectInput()
{
    // Cleanup DirectX input objects
	if( m_pKeyboard != NULL) 
        m_pKeyboard->Release();

	if( m_pDI!= NULL) 
        m_pDI->Release();

 }


void UpdateInput( UserInput* pUserInput )
{
    // Get the input's device state, and put the state in dims
    ZeroMemory( &pUserInput->diks, sizeof(pUserInput->diks) );
    if( FAILED(m_pKeyboard->GetDeviceState( sizeof(pUserInput->diks), &pUserInput->diks )) ) 
    {
        m_pKeyboard->Acquire();
        return; 
    }

    // TODO: Process user input as needed
    pUserInput->bLeft  = ( (pUserInput->diks[DIK_LEFT] & 0x80)  == 0x80 );
    pUserInput->bRight = ( (pUserInput->diks[DIK_RIGHT] & 0x80) == 0x80 );
    pUserInput->bUp    = ( (pUserInput->diks[DIK_UP] & 0x80)    == 0x80 );
    pUserInput->bDown  = ( (pUserInput->diks[DIK_DOWN] & 0x80)  == 0x80 );
    pUserInput->bFirstWeapon   = ( (pUserInput->diks[DIK_Z] & 0x80)     == 0x80 );
    pUserInput->bSecendWeapon   = ( (pUserInput->diks[DIK_X] & 0x80)     == 0x80 );
    pUserInput->bCamera   = ( (pUserInput->diks[DIK_C] & 0x80)     == 0x80 );
    pUserInput->bAxil   = ( (pUserInput->diks[DIK_LSHIFT] & 0x80)     == 0x80 );
}
