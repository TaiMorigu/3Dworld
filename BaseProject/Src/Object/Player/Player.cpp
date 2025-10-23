#include<DxLib.h>
#include "Player.h"
#include"../../Application.h"
#include"../../Utility/AsoUtility.h"
#include"../../Utility/MatrixUtility.h"
#include"../../Common/AnimationController.h"
#include"../../Manager/InputManager.h"
#include"../../Manager/Camera.h"
#include"../../Manager/SceneManager.h"


Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Init(void)
{
	//���f���̓ǂݍ���
	modelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Player/Player.mv1").c_str());

	//���f���̊p�x
	angles_ = { 0.0f,0.0f,0.0f};
	localAngles_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };

	// �s��̍���(�q, �e�Ǝw�肷��Ɛe�ˎq�̏��ɓK�p�����)
	MATRIX mat = MatrixUtility::Multiplication(localAngles_, angles_);

	// ��]�s������f���ɔ��f
	MV1SetRotationMatrix(modelId_, mat);

	//���f���̈ʒu�ݒ�
	pos_ = AsoUtility::VECTOR_ZERO;
	MV1SetPosition(modelId_, pos_);

	//���f���A�j���[�V��������̏�����
	animationController_ = new AnimationController(modelId_);

	//�A�j���[�V�����̒ǉ�
	animationController_->Add(
		static_cast<int>(ANIM_TYPE::IDLE), 30.0f, Application::PATH_MODEL + "Player/Idle.mv1"
	);
	animationController_->Add(
		static_cast<int>(ANIM_TYPE::WALK), 30.0f, Application::PATH_MODEL + "Player/Walk.mv1"
	);

	animationController_->Play(static_cast<int>(ANIM_TYPE::WALK));

	//�����A�j���[�V�����̍Đ�
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));

	InitDice();

	// �p�x��������ɕϊ�����
	moveDir_ = { sinf(angles_.y), 0.0f, cosf(angles_.y) };


}

void Player::Update(void)
{
	//���f����Y����]
	//angles_.y += AsoUtility::Deg2RadF(0.1f);

	// �v���C���[�̒x����]����
	DelayRotate();

	// �s��̍���(�q, �e�Ǝw�肷��Ɛe�ˎq�̏��ɓK�p�����)
	MATRIX mat = MatrixUtility::Multiplication(localAngles_, angles_);

	// ��]�s������f���ɔ��f
	MV1SetRotationMatrix(modelId_, mat);

	Move();

	//�A�j���[�V�����̍X�V
	animationController_->Update();

	//
	diceAngles_.z += 0.01f;
	SyncDice();
}

void Player::Draw(void)
{
	MV1DrawModel(modelId_);
	MV1DrawModel(diceModelId_);

	DrawFormatString(
		0, 50, 0xffffff,
		"�L�����p�x�@ �F(% .1f, % .1f, % .1f)",
		AsoUtility::Rad2DegF(angles_.x),
		AsoUtility::Rad2DegF(angles_.y),
		AsoUtility::Rad2DegF(angles_.z)
	);
}

void Player::Release(void)
{
	MV1DeleteModel(modelId_);
	delete animationController_;
}

void Player::InitDice(void)
{
	diceModelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Dice/Dice.mv1").c_str());

	diceScales_ = { 0.1f, 0.1f, 0.1f };
	MV1SetScale(diceModelId_, diceScales_);

	diceAngles_ = AsoUtility::VECTOR_ZERO;
	diceLocalAngles_ = AsoUtility::VECTOR_ZERO;

	diceLocalAngles_ = { 200.f,100.0f,0.0f };

	SyncDice();

}

void Player::SyncDice(void)
{
	//�s��̍���
	MATRIX selfMat = MatrixUtility::Multiplication(diceLocalAngles_, diceAngles_);

	MATRIX parentMat = MatrixUtility::GetMatrixRotateXYZ(angles_);

	MATRIX mat = MatrixUtility::Multiplication(selfMat, parentMat);

	diceLocalPos_ = VTransform(diceLocalPos_, parentMat);
	
	//��]�s������f���ɔ��f
	MV1SetRotationMatrix(diceModelId_, selfMat);

	//�T�C�R���̃��[���h���W
	dicePos_ = VAdd(diceLocalPos_, pos_);

	//�T�C�R���̈ʒu�ݒ�
	MV1SetPosition(diceModelId_, dicePos_);
}

void Player::Move(void)
{
	auto& ins = InputManager::GetInstance();

	VECTOR cameraAngles =
		SceneManager::GetInstance().GetCamera()->GetAngles();


		// WASD�ŃJ�������ړ�������
	const float movePow = 3.0f;
	VECTOR dir = AsoUtility::VECTOR_ZERO;
	// �Q�[���p�b�h���ڑ����ŏ����𕪂���
	if (GetJoypadNum() == 0)
	{
		// WASD�ňړ�����
		if (ins.IsNew(KEY_INPUT_W)) { dir = { 0.0f, 0.0f, 1.0f }; }
		if (ins.IsNew(KEY_INPUT_A)) { dir = { -1.0f, 0.0f, 0.0f }; }
		if (ins.IsNew(KEY_INPUT_S)) { dir = { 0.0f, 0.0f, -1.0f }; }
		if (ins.IsNew(KEY_INPUT_D)) { dir = { 1.0f, 0.0f, 0.0f }; }
	}
	else
	{
		// �ڑ�����Ă���Q�[���p�b�h�P�̏����擾
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
		// �A�i���O�L�[�̓��͒l����������擾
		dir = ins.GetDirectionXZAKey(padState.AKeyLX, padState.AKeyLY);
	}

	if (!AsoUtility::EqualsVZero(dir))
	{
		MATRIX mat = MGetIdent();
		//mat = MMult(mat, MGetRotX(cameraAngles.x));
		mat = MMult(mat, MGetRotY(cameraAngles.y));
		//mat = MMult(mat, MGetRotZ(cameraAngles.z));

		VECTOR moveDir = VTransform(dir, mat);

		pos_ = VAdd(pos_, VScale(moveDir, movePow));

		// ��]�s����g�p���āA�x�N�g������]������
		moveDir_ = VTransform(dir, mat);
		// �ړ���������p�x�ɕϊ�����
		angles_.y = atan2f(moveDir_.x, moveDir_.z);
		// �����~�X�s�[�h�ňړ��ʂ�����āA���W�ɑ����Ĉړ�
		pos_ = VAdd(pos_, VScale(moveDir_, movePow));


	}
}

void Player::DelayRotate(void)
{
	// �ړ���������p�x�ɕϊ�����
	float goal = atan2f(moveDir_.x, moveDir_.z);
	// ��ɍŒZ�o�H�ŕ��
	angles_.y = AsoUtility::LerpAngle(angles_.y, goal, 0.2f);
}
