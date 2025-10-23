#include <DxLib.h>
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

const std::string Application::PATH_MODEL = "Data/Model/";

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{

	// �A�v���P�[�V�����̏����ݒ�
	SetWindowText("3DWorld");

	// �E�B���h�E�T�C�Y
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// DxLib�̏�����
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	// �����̃V�[�h�l��ݒ肷��
	DATEDATA date;

	// ���ݎ������擾����
	GetDateTime(&date);

	// �����̏����l��ݒ肷��
	// �ݒ肷�鐔�l�ɂ���āA�����_���̏o�����ς��
	SRand(date.Year + date.Mon + date.Day + date.Hour + date.Min + date.Sec);

	// ���͐��䏉����
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// �V�[���Ǘ�������
	SceneManager::CreateInstance();

}

void Application::Run(void)
{

	InputManager& inputManager = InputManager::GetInstance();
	SceneManager& sceneManager = SceneManager::GetInstance();

	// �Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		inputManager.Update();
		sceneManager.Update();

		sceneManager.Draw();

		ScreenFlip();

	}

}

void Application::Destroy(void)
{

	// �V�[���Ǘ����
	SceneManager::GetInstance().Destroy();

	// ���͐�����
	InputManager::GetInstance().Destroy();

	// DxLib�I��
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	// �C���X�^���X�̃��������
	delete instance_;

}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
}
