#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Object/Player/Player.h"
#include "Camera.h"
#include <DxLib.h>

Camera::Camera(void)
{
	// DxLib�̏����ݒ�ł́A
	// �J�����̈ʒu�� x = 320.0f, y = 240.0f, z = (��ʂ̃T�C�Y�ɂ���ĕω�)�A
	// �����_�̈ʒu�� x = 320.0f, y = 240.0f, z = 1.0f
	// �J�����̏������ x = 0.0f, y = 1.0f, z = 0.0f
	// �E��ʒu����Z���̃v���X����������悤�ȃJ����
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	// �J�����̏����ʒu
	pos_ = DERFAULT_POS;
	// �J�����̏����p�x
	angles_ = DERFAULT_ANGLES;
}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{
	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
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
	// �J�����̐ݒ�(�ʒu�Ɗp�x�ɂ�鐧��)
	SetCameraPositionAndAngle(
		pos_,
		angles_.x,
		angles_.y,
		angles_.z
	);
}
void Camera::SetBeforeDrawFixedPoint(void)
{
	// �J�����̐ݒ�(�ʒu�Ɗp�x�ɂ�鐧��)
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
	// ���L�[�ŃJ�����̊p�x��ς���
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
	// �J�����̐ݒ�(�ʒu�Ɗp�x�ɂ�鐧��)
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
		// ������]�ɂ��XYZ�̈ړ�
		MoveXYZDirecton();
	}
	else
	{
		// ������]�ɂ��XYZ�̈ړ�(�Q�[���p�b�h)
		MoveXYZDirectionPad();
	}

	// �J�����̉�]�s����쐬
	MATRIX matY = MGetIdent();
	//mat = MMult(mat, MGetRotX(angles_.x));
	matY = MMult(matY, MGetRotY(angles_.y));
	//mat = MMult(mat, MGetRotZ(angles_.z));
	// �����_�̈ړ�
	VECTOR followPos = follow_->GetPos();
	targetPos_ = followPos;
	// �J�����̈ړ�
	// 
	// �J�����̉�]�s����쐬
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotX(angles_.x));
	mat = MMult(mat, MGetRotY(angles_.y));

	// �����_�̈ړ�
	VECTOR followPos = follow_->GetPos();
	VECTOR targetLocalRotPos = VTransform(FOLLOW_TARGET_LOCAL_POS, mat);
	targetPos_ = VAdd(followPos, targetLocalRotPos);
	// �J�����̈ړ�
	// ���΍��W����]�����āA��]��̑��΍��W���擾����
	VECTOR cameraLocalRotPos = VTransform(FOLLOW_CAMERA_LOCAL_POS, mat);
	// ���΍��W���烏�[���h���W�ɒ����āA�J�������W�Ƃ���
	pos_ = VAdd(followPos, cameraLocalRotPos);

	//mat = MMult(mat, MGetRotZ(angles_.z));
	// ���΍��W����]�����āA��]��̑��΍��W���擾����
	VECTOR localRotPos = VTransform(FOLLOW_LOCAL_POS, mat);

	// ���΍��W���烏�[���h���W�ɒ����āA�J�������W�Ƃ���
	pos_ = VAdd(followPos, localRotPos);

	VECTOR up = VTransform(AsoUtility::DIR_U, mat);

	// �J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
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
		"�J�������W (% .1f, % .1f, % .1f)",
		pos_.x, pos_.y, pos_.z
	);
	DrawFormatString(
		0, 30, 0xffffff,
		 "�J�����p�x (% .1f, % .1f, % .1f)",
		AsoUtility::Rad2DegF(angles_.x),
		AsoUtility::Rad2DegF(angles_.y),
		AsoUtility::Rad2DegF(angles_.z)
	);
	DrawSphere3D(targetPos_, 20.0f, 10, 0xff0000, 0xff0000, true);
}

void Camera::ChangeMode(MODE mode)
{
	// �J�������[�h�̕ύX
	mode_ = mode;
	// �ύX���̏���������
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

