#include "Input.h"

HWND Input::_HWnd;
LPDIRECTINPUT8 Input::_DirectInput;		// The DirectInput object     

												//Keyboard
LPDIRECTINPUTDEVICE8 Input:: _Keyboard;	// The keyboard device 
BYTE  Input::_KeyStates[256];			// DirectInput keyboard state buffer 
DIDEVICEOBJECTDATA Input::_KeyEvents[KEYBOARD_BUFFER_SIZE]; // Buffered keyboard data
std::vector<int> Input::_KeyCodesVector; //a vector of keycodes contained in buffered input
std::vector<int> Input::_KeyStatesVector; //a vector of keystates contained in buffered input


										  //Mouse
LPDIRECTINPUTDEVICE8 Input::_Mouse; // The mouse device
DIMOUSESTATE  Input::_MouseState; // DirectInput mouse state buffer 
DIDEVICEOBJECTDATA Input::_MouseEvents[16]; // Buffered keyboard data
std::vector<int> Input::_MouseCodesVector; //a vector of keycodes contained in buffered input
std::vector<int> Input::_MouseStatesVector; //a vector of keystates contained in buffered input
Vector2 Input::_MousePosition;


Input::Input()
{	
	
}

Input::~Input()
{

}

void Input::Release()
{
	if (_DirectInput != NULL)
	{
		_DirectInput->Release();
		_DirectInput = NULL;
	}
	if (_Keyboard != NULL)
	{
		_Keyboard->Unacquire();
		_Keyboard->Release();
		_Keyboard = NULL;
	}

	if (_Mouse != NULL)
	{
		_Mouse->Unacquire();
		_Mouse->Release();
		_Mouse = NULL;
	}
}

void Input::Init(HINSTANCE hInstance, HWND hWnd)
{
	//Release first for sure since we want to have just one class instance
	Release();
	ClearBuffedInput();

	_HWnd = hWnd;
	_MousePosition.Set(0, 0);

	HRESULT
		result = DirectInput8Create
		(
			hInstance, //handle for a window instance
			DIRECTINPUT_VERSION, //direct input version
			IID_IDirectInput8, //
			(VOID**)&_DirectInput,  //receive the interface pointer
			NULL //NULL if the interface is not aggregated
		);

	// TO-DO: put in exception handling
	if (result != DI_OK) return;
	//trace(L"DirectInput has been created");

	InitKeyBoard();

	InitMouse();
	
}


bool Input::InitKeyBoard()
{
	HRESULT result = _DirectInput->CreateDevice(
		GUID_SysKeyboard, //The default system keyboard
		&_Keyboard,  //receive the IDirectInputDevice8 interface pointer 
		NULL //NULL if the interface is not aggregated.
	);

	// TO-DO: put in exception handling
	if (result != DI_OK) return false;

	//trace(L"DirectInput keyboard has been created");


	// Set the data format to "keyboard format" - a predefined data format 
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing an array
	// of 256 bytes to IDirectInputDevice::GetDeviceState.
	result = _Keyboard->SetDataFormat(&c_dfDIKeyboard);

	//trace(L"SetDataFormat for keyboard successfully");

	result = _Keyboard->SetCooperativeLevel(_HWnd,
		DISCL_FOREGROUND |		//The application requires foreground access. 
								//If foreground access is granted, 
								//the device is automatically unacquired when the associated window moves to the background.
		DISCL_NONEXCLUSIVE		//The application requires nonexclusive access. 
								//Access to the device does not interfere with other applications that are accessing the same device.
	);
	//trace(L"SetCooperativeLevel for keyboard successfully");

	// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
	//
	// DirectInput uses unbuffered I/O (buffer size = 0) by default.
	// If you want to read buffered data, you need to set a nonzero
	// buffer size.
	//
	// Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
	//
	// The buffer size is a DWORD property associated with the device.
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE; // Arbitary buffer size

	result = _Keyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	if (result != DI_OK) return false;
	//trace(L"SetProperty for keyboard successfully");

	result = _Keyboard->Acquire();
	if (result != DI_OK) return false;
	//trace(L"Keyboard has been acquired successfully");

	return true;
}
bool Input::InitMouse()
{
	HRESULT result = _DirectInput->CreateDevice(
		GUID_SysMouse, //The default system keyboard
		&_Mouse,  //receive the IDirectInputDevice8 interface pointer 
		NULL //NULL if the interface is not aggregated.
	);

	// TO-DO: put in exception handling
	if (result != DI_OK) return false;
	//trace(L"DirectInput mouse has been created");


	// Set the data format to "mouse format" - a predefined data format 
	result = _Mouse->SetDataFormat(&c_dfDIMouse);
	//trace(L"SetDataFormat for mouse successfully");

	result = _Mouse->SetCooperativeLevel(_HWnd,
		DISCL_FOREGROUND |		//The application requires foreground access. 
								//If foreground access is granted, 
								//the device is automatically unacquired when the associated window moves to the background.
		DISCL_NONEXCLUSIVE		//The application requires nonexclusive access. 
								//Access to the device does not interfere with other applications that are accessing the same device.
	);
	//trace(L"SetCooperativeLevel for mouse successfully");



	// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = 16; // Arbitary buffer size
	
	result = _Mouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	if (result != DI_OK) return false;
	//trace(L"SetProperty for keyboard successfully");




	result = _Mouse->Acquire();
	if (result != DI_OK) return false;
	//trace(L"Mouse has been acquired successfully");

	return true;
}


void Input::ProcessKeyBoardInformation()
{
	if (_Keyboard == NULL) return;

	HRESULT result;

	// Collect all key states first
	result = _Keyboard->GetDeviceState(sizeof(_KeyStates), _KeyStates);
	
	//If the keyboard lost focus or was not acquired then try to get control back.
	if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
	{
		_Keyboard->Acquire();
		return;
	}

	// Handle key press event
	// NOTES: Buffered input is like an Event
	// Collect all buffered events (also clear them from DirectInput buffer)
	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	_Keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), _KeyEvents, &dwElements, 0);

	//insert keycodes and keystates into vectors
	_KeyCodesVector.insert(_KeyCodesVector.end(), &(_KeyEvents[0].dwOfs), &(_KeyEvents[0].dwOfs) + dwElements);
	_KeyStatesVector.insert(_KeyStatesVector.end(), &(_KeyEvents[0].dwData), &(_KeyEvents[0].dwData) + dwElements);

}

void Input::ProcessMouseInformation()
{
	HRESULT result;

	// Read the mouse device.
	result = _Mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&_MouseState);

	// If the mouse lost focus or was not acquired then try to get control back.
	if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
	{
		_Mouse->Acquire();
		return;
	}

	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	_MousePosition.x += _MouseState.lX;
	_MousePosition.y += _MouseState.lY;



	// Handle key press event
	// NOTES: Buffered input is like an Event
	// Collect all buffered events (also clear them from DirectInput buffer)
	DWORD dwElements = 16;
	_Mouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), _MouseEvents, &dwElements, 0);

	//insert keycodes and keystates into vectors
	_MouseCodesVector.insert(_MouseCodesVector.end(), &(_MouseEvents[0].dwOfs), &(_MouseEvents[0].dwOfs) + dwElements);
	_MouseStatesVector.insert(_MouseStatesVector.end(), &(_MouseEvents[0].dwData), &(_MouseEvents[0].dwData) + dwElements);

}

bool Input::GetKey(int keyCode)
{
	return (_KeyStates[keyCode] & 0x80) > 0;
}

bool Input::GetKeyDown(int keyCode) 
{
	//check if this keycode is pressed
	std::vector<int>::iterator it = std::find(_KeyCodesVector.begin(), _KeyCodesVector.end(), keyCode);
	bool keyCodePressed = (it != _KeyCodesVector.end());
	
	if (keyCodePressed)
	{
		//check if it is key down
		return (_KeyStatesVector.at(it - _KeyCodesVector.begin()) & 0x80) > 0;
	}
	else
	{
		return false;
	}

}

bool Input::GetKeyUp(int keyCode) 
{ 
	//check if this keycode is pressed
	std::vector<int>::iterator it = std::find(_KeyCodesVector.begin(), _KeyCodesVector.end(), keyCode);
	bool keyCodePressed = (it != _KeyCodesVector.end());

	if (keyCodePressed)
	{
		//check if it is key up
		return (_KeyStatesVector.at(it - _KeyCodesVector.begin()) & 0x80) <= 0;
	}
	else
	{
		return false;
	}

}

bool Input::GetMouse(int mouse)
{
	if (mouse > 3) return false;
	return _MouseState.rgbButtons[mouse] & 0x80;
}

bool Input::GetMouseDown(int mouse)
{
	//check if this keycode is pressed
	std::vector<int>::iterator it = std::find(_MouseCodesVector.begin(), _MouseCodesVector.end(), mouse);
	bool mouseCodePressed = (it != _MouseCodesVector.end());

	if (mouseCodePressed)
	{
		//check if it is key down
		return (_MouseStatesVector.at(it - _MouseCodesVector.begin()) & 0x80) > 0;
	}
	else
	{
		return false;
	}
}

bool Input::GetMouseUp(int mouse)
{
	//check if this keycode is pressed
	std::vector<int>::iterator it = std::find(_MouseCodesVector.begin(), _MouseCodesVector.end(), mouse);
	bool mouseCodePressed = (it != _MouseCodesVector.end());

	if (mouseCodePressed)
	{
		//check if it is key down
		return (_MouseStatesVector.at(it - _MouseCodesVector.begin()) & 0x80) <= 0;
	}
	else
	{
		return false;
	}
}

Vector2 Input::GetMousePosition()
{

	// Ensure the mouse location doesn't exceed the screen width or height.
	if (_MousePosition.x < 0) { _MousePosition.x = 0; }
	if (_MousePosition.y < 0) { _MousePosition.y = 0; }

	//if (_MousePosition.X > 640) { _MousePosition.X = 640; }
	//if (_MousePosition.Y > 480) { _MousePosition.Y = 480; }

	return _MousePosition;

}

void Input::ClearBuffedInput()
{
	_KeyCodesVector.clear();
	_KeyStatesVector.clear();
	_MouseCodesVector.clear();
	_MouseStatesVector.clear();
}