#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Object/Player/Player.h"
#include "Camera.h"
#include <DxLib.h>

Camera::Camera(void)
{
	// DxLibの初期設定では、
	// カメラの位置が x = 320.0f, y = 240.0f, z = (画面のサイズによって変化)、
	// 注視点の位置は x = 320.0f, y = 240.0f, z = 1.0f
	// カメラの上方向は x = 0.0f, y = 1.0f, z = 0.0f
	// 右上位置からZ軸のプラス方向を見るようなカメラ
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	// カメラの初期位置
	pos_ = DERFAULT_POS;
	// カメラの初期角度
	angles_ = DERFAULT_ANGLES;
}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{
	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(VIEW_NEAR, VIEW_FAR);
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	}
	// カメラの設定(位置と角度による制御)
	SetCameraPositionAndAngle(
		pos_,
		angles_.x,
		angles_.y,
		angles_.z
	);
}
void Camera::SetBeforeDrawFixedPoint(void)
{
	// カメラの設定(位置と角度による制御)
	SetCameraPositionAndAngle(
		pos_,
		angles_.x,
		angles_.y,
		angles_.z
	);
}
void Camera::SetBeforeDrawFree(void)
{
	auto& ins = InputManager::GetInstance();
	// 矢印キーでカメラの角度を変える
	float rotPow = 1.0f * DX_PI_F / 180.0f;

	
	if(GetJoypadNum()==0)
	{
		if (ins.IsNew(KEY_INPUT_DOWN)) { angles_.x += rotPow; }
		if (ins.IsNew(KEY_INPUT_UP)) { angles_.x -= rotPow; }
		if (ins.IsNew(KEY_INPUT_RIGHT)) { angles_.y += rotPow; }
		if (ins.IsNew(KEY_INPUT_LEFT)) { angles_.y -= rotPow; }
	}
	else
	{
		InputManager::JOYPAD_IN_STATE padstate =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		VECTOR dir =
			ins.GetDirectionXZAKey(padstate.AKeyRX, padstate.AKeyRY);

		angles_.x += dir.z * rotPow * 3.0f;
	}
	MoveXYZDirecton();
	// カメラの設定(位置と角度による制御)
	SetCameraPositionAndAngle(
		pos_,
		angles_.x,
		angles_.y,
		angles_.z
	);
}

void Camera::SetBeforeDrawFollow(void)
{
	auto& ins = InputManager::GetInstance();
	if (GetJoypadNum() == 0)
	{
		// 方向回転によるXYZの移動
		MoveXYZDirecton();
	}
	else
	{
		// 方向回転によるXYZの移動(ゲームパッド)
		MoveXYZDirectionPad();
	}

	// カメラの回転行列を作成
	MATRIX matY = MGetIdent();
	//mat = MMult(mat, MGetRotX(angles_.x));
	matY = MMult(matY, MGetRotY(angles_.y));
	//mat = MMult(mat, MGetRotZ(angles_.z));
	// 注視点の移動
	VECTOR followPos = follow_->GetPos();
	targetPos_ = followPos;
	// カメラの移動
	// 
	// カメラの回転行列を作成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotX(angles_.x));
	mat = MMult(mat, MGetRotY(angles_.y));

	// 注視点の移動
	VECTOR followPos = follow_->GetPos();
	VECTOR targetLocalRotPos = VTransform(FOLLOW_TARGET_LOCAL_POS, mat);
	targetPos_ = VAdd(followPos, targetLocalRotPos);
	// カメラの移動
	// 相対座標を回転させて、回転後の相対座標を取得する
	VECTOR cameraLocalRotPos = VTransform(FOLLOW_CAMERA_LOCAL_POS, mat);
	// 相対座標からワールド座標に直して、カメラ座標とする
	pos_ = VAdd(followPos, cameraLocalRotPos);

	//mat = MMult(mat, MGetRotZ(angles_.z));
	// 相対座標を回転させて、回転後の相対座標を取得する
	VECTOR localRotPos = VTransform(FOLLOW_LOCAL_POS, mat);

	// 相対座標からワールド座標に直して、カメラ座標とする
	pos_ = VAdd(followPos, localRotPos);

	VECTOR up = VTransform(AsoUtility::DIR_U, mat);

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		up
	);
}

const VECTOR& Camera::GetTargetPos(void) const
{
	return targetPos_;
}


void Camera::DrawDebug(void)
{
	DrawFormatString(
		0, 10, 0xffffff,
		"カメラ座標 (% .1f, % .1f, % .1f)",
		pos_.x, pos_.y, pos_.z
	);
	DrawFormatString(
		0, 30, 0xffffff,
		 "カメラ角度 (% .1f, % .1f, % .1f)",
		AsoUtility::Rad2DegF(angles_.x),
		AsoUtility::Rad2DegF(angles_.y),
		AsoUtility::Rad2DegF(angles_.z)
	);
	DrawSphere3D(targetPos_, 20.0f, 10, 0xff0000, 0xff0000, true);
}

void Camera::ChangeMode(MODE mode)
{
	// カメラモードの変更
	mode_ = mode;
	// 変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FREE:
		break;
	case Camera::MODE::FOLLOW:
		break;
	}
}
void Camera::SetFollow(Player* follow)
{
	follow_ = follow;
}

void Camera::MoveXYZDirecton(void)
{

}

void Camera::Release(void)
{
}

const VECTOR& Camera::GetPos(void) const
{
	return pos_;
}

const VECTOR& Camera::GetAngles(void) const
{
	return angles_;
}

