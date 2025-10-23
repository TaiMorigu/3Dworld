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
	// カメラモード変更
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->ChangeMode(Camera::MODE::FREE);

	// グリッド初期化
	grid_ = new Grid();
	grid_->Init();

	//ステージ初期化
	stage_ = new Stage();
	stage_->Init();

	//プレイヤー初期化
	player_ = new Player();
	player_->Init();

	// カメラモード変更
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(player_);
	camera->ChangeMode(Camera::MODE::FOLLOW);
	camera->ChangeMode(Camera::MODE::FOLLOW);
}

void TitleScene::Update(void)
{
	// グリッド更新
	grid_->Update();
	// ステージ更新
	stage_->Update();
	// プレイヤー更新
	player_->Update();
}

void TitleScene::Draw(void)
{
	// グリッド描画
	grid_->Draw();
	// ステージ描画
	stage_->Draw();
	// プレイヤー描画
	player_->Draw();
}

void TitleScene::Release(void)
{
	// グリッド解放
	grid_->Release();
	delete grid_;

	// ステージ解放
	stage_->Release();
	delete stage_;

	// プレイヤー解放
	player_->Release();
	delete player_;
}
