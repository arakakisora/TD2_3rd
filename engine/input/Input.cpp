#include "Input.h"
#include <cassert>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "CameraManager.h"
#include "MyMath.h"

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

void Input::Initialize(WinApp *winApp)
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

Vector3 Input::GetMouseWorldPosition(float distance) {
	// アクティブなカメラを取得
	Camera* activeCamera = CameraManager::GetInstans()->GetActiveCamera();
	if (!activeCamera) {
		return Vector3(0, 0, 0); // アクティブカメラが存在しない場合
	}

	// カメラのビュープロジェクション行列を取得
	const Matrix4x4& viewProjectionMatrix = activeCamera->GetViewprojectionMatrix();
	Matrix4x4 invViewProjectionMatrix = viewProjectionMatrix.Inverse();

	// マウス座標を正規化デバイス座標 (NDC) に変換
	float ndcX = (2.0f * mousePos.x) / WinApp::kClientWindth - 1.0f;
	float ndcY = 1.0f - (2.0f * mousePos.y) / WinApp::kClientHeight; // Y座標は反転

	// 視錐台の近点 (Z=0) と遠点 (Z=1) を定義
	Vector3 clipSpaceNear(ndcX, ndcY, 0.0f);
	Vector3 clipSpaceFar(ndcX, ndcY, 1.0f);

	// クリップ空間座標をワールド空間に変換
	Vector3 worldNear = MyMath::Transform(clipSpaceNear, invViewProjectionMatrix);
	Vector3 worldFar = MyMath::Transform(clipSpaceFar, invViewProjectionMatrix);

	// レイの方向を計算
	Vector3 rayDirection = (worldFar - worldNear).Normalize();

	
	
	return worldNear + rayDirection * distance;
}








//Vector3 Input::GetMouseWorldPosition() {
//	const Vector2& mousePos = GetMousePos();
//
//	// アクティブカメラを取得
//	Camera* activeCamera = CameraManager::GetInstans()->GetActiveCamera();
//	if (!activeCamera) {
//		return Vector3(0, 0, 0); // カメラが存在しない場合
//	}
//
//	// カメラ行列を取得
//	const Matrix4x4& viewProjectionMatrix = activeCamera->GetViewprojectionMatrix();
//
//	// ウィンドウサイズを取得
//	float windowWidth = static_cast<float>(WinApp::kClientWindth);
//	float windowHeight = static_cast<float>(WinApp::kClientHeight);
//
//	// マウス位置を正規化デバイス座標 (NDC) に変換
//	float ndcX = (2.0f * mousePos.x / windowWidth) - 1.0f;
//	float ndcY = 1.0f - (2.0f * mousePos.y / windowHeight); // Y座標を反転
//
//	// クリップ空間座標をワールド空間に変換
//	Vector3 clipSpacePos(ndcX, ndcY, 1.0f);
//	Matrix4x4 invViewProjectionMatrix = viewProjectionMatrix.Inverse();
//	return MyMath::Transform(clipSpacePos, invViewProjectionMatrix);
//}



