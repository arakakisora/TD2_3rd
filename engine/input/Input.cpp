#include "Input.h"
#include <cassert>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

Input* Input::instance = nullptr;

Input* Input::GetInstans()
{
	if (instance == nullptr) {
		instance = new Input;
	}
	return instance;
}

void Input::Finalize()
{
	delete instance;
	instance = nullptr;

}

void Input::Initialize(WinApp* winApp)
{
	winApp_ = winApp;
	HRESULT hr;
	//DirectInputのインスタンスを生成
	hr = DirectInput8Create(winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(hr));
	//キーボードデバイス生成

	hr = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(hr));
	//入力データ形式のセット
	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));
	//排他制御レベルのセット
	hr = keyboard->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));

	//マウスデバイス生成
	hr = directInput->CreateDevice(GUID_SysMouse, &devMouse_, NULL);
	assert(SUCCEEDED(hr));
	//入力データ形式のセット
	hr = devMouse_->SetDataFormat(&c_dfDIMouse2);
	assert(SUCCEEDED(hr));
	//排他制御レベルのセット
	hr = devMouse_->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(hr));


}

void Input::Update()
{
	//前回のキー入力を保存
	memcpy(preKey, key, sizeof(key));
	//キーボード情報の取得
	keyboard->Acquire();
	//全キーボード入力情報を取得
	keyboard->GetDeviceState(sizeof(key), key);


	//前回のマウス入力を保存
	memcpy(&preMouse, &mouse, sizeof(mouse));
	//マウス情報の取得
	devMouse_->Acquire();
	//全マウス入力情報を取得
	devMouse_->GetDeviceState(sizeof(mouse), &mouse);
	//マウス座標の取得
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(winApp_->GetHwnd(), &point);
	mousePos.x = (float)point.x;
	mousePos.y = (float)point.y;
}

bool Input::PushKey(BYTE keyNumber)
{
	if (key[keyNumber]) {

		return true;
	}
	return false;

}

bool Input::TriggerKey(BYTE keyNumber)
{
	if (key[keyNumber] && !preKey[keyNumber]) {

		return true;
	}
	return false;


}



bool Input::PushMouse(int buttonNumber)
{
	if (mouse.rgbButtons[buttonNumber]) {
		return true;
	}
	return false;
}

bool Input::TriggerMouse(int buttonNumber)
{
	if (mouse.rgbButtons[buttonNumber] && !preMouse.rgbButtons[buttonNumber]) {
		return true;
	}
	return false;
}
