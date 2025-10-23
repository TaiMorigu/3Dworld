#include <cmath>
#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Grid.h"
#include "TitleScene.h"
#include"../Manager/Camera.h"
#include"../Object/Stage.h"
#include"../Object/Player/Player.h"

TitleScene::TitleScene(void) : SceneBase()
{
	grid_ = nullptr;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	// �J�������[�h�ύX
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->ChangeMode(Camera::MODE::FREE);

	// �O���b�h������
	grid_ = new Grid();
	grid_->Init();

	//�X�e�[�W������
	stage_ = new Stage();
	stage_->Init();

	//�v���C���[������
	player_ = new Player();
	player_->Init();

	// �J�������[�h�ύX
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(player_);
	camera->ChangeMode(Camera::MODE::FOLLOW);
	camera->ChangeMode(Camera::MODE::FOLLOW);
}

void TitleScene::Update(void)
{
	// �O���b�h�X�V
	grid_->Update();
	// �X�e�[�W�X�V
	stage_->Update();
	// �v���C���[�X�V
	player_->Update();
}

void TitleScene::Draw(void)
{
	// �O���b�h�`��
	grid_->Draw();
	// �X�e�[�W�`��
	stage_->Draw();
	// �v���C���[�`��
	player_->Draw();
}

void TitleScene::Release(void)
{
	// �O���b�h���
	grid_->Release();
	delete grid_;

	// �X�e�[�W���
	stage_->Release();
	delete stage_;

	// �v���C���[���
	player_->Release();
	delete player_;
}
