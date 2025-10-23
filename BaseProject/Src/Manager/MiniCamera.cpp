#include "../Application.h"
#include "Camera.h"
#include "SceneManager.h"
#include "MiniCamera.h"

MiniCamera::MiniCamera(Camera* camera)
{
	camera_ = camera;
	size_ = Vector2(SIZE_X, SIZE_Y);
	miniScreen_ = MakeScreen(size_.x, size_.y);
	modelId_ = -1;
	pos_ = { 0.0f, 0.0f, 0.0f };
}

MiniCamera::~MiniCamera(void)
{
}

void MiniCamera::Init(void)
{

	// �~�j�J�������f���̓ǂݍ���
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "MiniCamera.mv1").c_str());

	// �~�j�J�������f���̑傫����ݒ�
	MV1SetScale(modelId_, { SCALE, SCALE, SCALE });

	// �~�j�J�������f���̈ʒu��ݒ�
	pos_ = { 0.0f, 0.0f, 0.0f };
	MV1SetPosition(modelId_, pos_);

}

void MiniCamera::Update(void)
{
	// �g�p���Ă���J�����̊p�x�ɏ�ɍ��킹��
	MV1SetRotationXYZ(modelId_, camera_->GetAngles());
}

void MiniCamera::DrawScreen(void)
{

	// �w�i�F
	SetBackgroundColor(
		BACKGROUND_COLOR_R,
		BACKGROUND_COLOR_G,
		BACKGROUND_COLOR_B);

	// �`��̈��ύX
	SetDrawScreen(miniScreen_);

	// ��ʂ�������
	ClearDrawScreen();

	// �~�j�J�������ʂ��J�����̐ݒ�
	VECTOR pos = pos_;

	// �~�j�J�������f���̎�O
	pos.z -= 500.0f;

	// �J�����ʒu���Z�b�g
	SetCameraPositionAndAngle(
		pos,
		0.0f,
		0.0f,
		0.0f
	);

	// ���C�g�̐ݒ�
	ChangeLightTypeDir({ 0.0f, -1.0f, 0.0f });

	// �~�j�J�������f����`��
	MV1DrawModel(modelId_);

	// XY��
	const float LEN = 180.0f;
	DrawLine3D(
		{ -LEN, 0.0f, 0.0f },
		{ LEN, 0.0f, 0.0f },
		0xff0000);
	DrawLine3D(
		{ 0.0f, -LEN, 0.0f },
		{ 0.0f, LEN, 0.0f },
		0x00ff00);
	DrawSphere3D({ LEN, 0.0f, 0.0f }, 20.0f, 10, 0xff0000, 0xff0000, true);
	DrawSphere3D({ 0.0f, LEN, 0.0f }, 20.0f, 10, 0x00ff00, 0x00ff00, true);

	// ���̐ݒ�ɖ߂�

	// �w�i�F�ݒ�
	SetBackgroundColor(
		SceneManager::BACKGROUND_COLOR_R,
		SceneManager::BACKGROUND_COLOR_G,
		SceneManager::BACKGROUND_COLOR_B);

	// ���C�g�̐ݒ�
	ChangeLightTypeDir(SceneManager::LIGHT_DIRECTION);

}

void MiniCamera::Draw(void)
{

	// ��ʉE��ɘg�\��
	int width = 10;
	DrawBox(
		Application::SCREEN_SIZE_X - size_.x - (width * 2),
		0,
		Application::SCREEN_SIZE_X,
		size_.y + (width * 2),
		0x000000,
		true
	);

	// �g���Ƀ~�j�J�������ʂ����摜��`��
	DrawGraph(Application::SCREEN_SIZE_X - size_.x - width, width, miniScreen_, false);

}

void MiniCamera::Release(void)
{
	MV1DeleteModel(modelId_);
	DeleteGraph(miniScreen_);
}

int MiniCamera::GetScreen(void) const
{
	return miniScreen_;
}
