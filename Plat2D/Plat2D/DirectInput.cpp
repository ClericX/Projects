#include "DirectInput.h"
#include "Main.h"

DirectInput::DirectInput(void)
{
}

DirectInput::~DirectInput(void)
{
}

void DirectInput::Initialize()
{
	DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&InputObject, NULL);

	InputObject->CreateDevice(GUID_SysKeyboard, &KeyboardDevice, NULL);

	KeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	KeyboardDevice->SetCooperativeLevel(App.hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	KeyboardDevice->Acquire();

	InputObject->CreateDevice(GUID_SysMouse, &MouseDevice, NULL);

	MouseDevice->SetDataFormat(&c_dfDIMouse);
	MouseDevice->SetCooperativeLevel(App.hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	MouseDevice->Acquire();
}

void DirectInput::UpdateInputState()
{
	//memset(&dims, 0, sizeof(DIMOUSESTATE));
	DI.KeyboardDevice->GetDeviceState(sizeof(DI.KeyboardState),(void*)&DI.KeyboardState);
	MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (void*)&dims);

	// squirrel away newest rel position data
	RelPos.x = dims.lX;
	RelPos.y = dims.lY;
	//m_relposition.z = dims.lZ;

	// get button data
	for (int q=0; q < NUMMOUSEBUTTONS; q++)
	{
		MouseButtonState[q] = (dims.rgbButtons[q] & 0x80);
	}
}

bool DirectInput::KeyPressed(int Key)
{
	if (KeyboardState[Key]/128)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void DirectInput::UpdateKey(int Key, bool ConstantFunctionality, void KeyPressedFunction(), void KeyHeldDownFunction())
{
	if (KeyPressed(Key))
	{
		if (ConstantFunctionality)
		{
			KeyPressedFunction();
		}
		if (KeyHeldBuffer[Key] <= 30)
		{
			KeyHeldBuffer[Key]++;
		}
		if (KeyHeldBuffer[Key] <= 30 && !KeyHeldUsed[Key])
		{
			if (!ConstantFunctionality)
			{
				if (&KeyPressedFunction != NULL)
				{
					KeyPressedFunction();
				}
			}
			KeyHeldUsed[Key] = true;
		}
		else if (KeyHeldBuffer[Key] > 30 && !KeyHeldDown[Key])
		{
			KeyHeldDown[Key] = true;
		}
		else if (KeyHeldDown[Key] && !ConstantFunctionality)
		{
			if (&KeyHeldDownFunction != NULL)
			{
				KeyHeldDownFunction();
			}
		}
	}

	else if (!KeyPressed(Key))
	{
		if (KeyHeldBuffer[Key] != 0)
		{
			KeyHeldBuffer[Key] = 0;
		}
		if (KeyHeldUsed[Key] == true)
		{
			KeyHeldUsed[Key] = false;
		}
		if (KeyHeldDown[Key] == true)
		{
			KeyHeldDown[Key] = false;
		}
	}
}

void DirectInput::ProcessInput()
{
	UpdateInputState();

	//UpdateKey(DIK_B, false, NULL, lolzfunc);

	UpdateKey(DIK_LEFT, true, KeyActions::CharMovement::MoveLeft);
	UpdateKey(DIK_RIGHT, true, KeyActions::CharMovement::MoveRight);
	UpdateKey(DIK_UP, false, KeyActions::CharMovement::Jump);

	//UpdateKey(DIK_W, true, KeyActions::CameraMovement::CamUp);
	//UpdateKey(DIK_S, true, KeyActions::CameraMovement::CamDown);
	//UpdateKey(DIK_A, true, KeyActions::CameraMovement::CamLeft);
	//UpdateKey(DIK_D, true, KeyActions::CameraMovement::CamRight);

	UpdateKey(DIK_A, false, KeyActions::Testing::Looting);

	UpdateKey(DIK_T, true, KeyActions::Testing::HealHP);
	UpdateKey(DIK_Y, true, KeyActions::Testing::HealMP);
	UpdateKey(DIK_U, false, KeyActions::Testing::Haste);
	UpdateKey(DIK_Q, false, KeyActions::Testing::Attack);
	UpdateKey(DIK_E, false, KeyActions::Testing::ReviveSnail);
	UpdateKey(DIK_V, false, KeyActions::Testing::SwitchMap);
}

int DirectInput::IsMouseButtonDown(int index)
{
	if (index < 0 || index > NUMMOUSEBUTTONS) return(0);
	return(MouseButtonState[index]);
}