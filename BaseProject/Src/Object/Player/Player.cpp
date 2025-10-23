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
	//モデルの読み込み
	modelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Player/Player.mv1").c_str());

	//モデルの角度
	angles_ = { 0.0f,0.0f,0.0f};
	localAngles_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };

	// 行列の合成(子, 親と指定すると親⇒子の順に適用される)
	MATRIX mat = MatrixUtility::Multiplication(localAngles_, angles_);

	// 回転行列をモデルに反映
	MV1SetRotationMatrix(modelId_, mat);

	//モデルの位置設定
	pos_ = AsoUtility::VECTOR_ZERO;
	MV1SetPosition(modelId_, pos_);

	//モデルアニメーション制御の初期化
	animationController_ = new AnimationController(modelId_);

	//アニメーションの追加
	animationController_->Add(
		static_cast<int>(ANIM_TYPE::IDLE), 30.0f, Application::PATH_MODEL + "Player/Idle.mv1"
	);
	animationController_->Add(
		static_cast<int>(ANIM_TYPE::WALK), 30.0f, Application::PATH_MODEL + "Player/Walk.mv1"
	);

	animationController_->Play(static_cast<int>(ANIM_TYPE::WALK));

	//初期アニメーションの再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));

	InitDice();

	// 角度から方向に変換する
	moveDir_ = { sinf(angles_.y), 0.0f, cosf(angles_.y) };


}

void Player::Update(void)
{
	//モデルのY軸回転
	//angles_.y += AsoUtility::Deg2RadF(0.1f);

	// プレイヤーの遅延回転処理
	DelayRotate();

	// 行列の合成(子, 親と指定すると親⇒子の順に適用される)
	MATRIX mat = MatrixUtility::Multiplication(localAngles_, angles_);

	// 回転行列をモデルに反映
	MV1SetRotationMatrix(modelId_, mat);

	Move();

	//アニメーションの更新
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
		"キャラ角度　 ：(% .1f, % .1f, % .1f)",
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
	//行列の合成
	MATRIX selfMat = MatrixUtility::Multiplication(diceLocalAngles_, diceAngles_);

	MATRIX parentMat = MatrixUtility::GetMatrixRotateXYZ(angles_);

	MATRIX mat = MatrixUtility::Multiplication(selfMat, parentMat);

	diceLocalPos_ = VTransform(diceLocalPos_, parentMat);
	
	//回転行列をモデルに反映
	MV1SetRotationMatrix(diceModelId_, selfMat);

	//サイコロのワールド座標
	dicePos_ = VAdd(diceLocalPos_, pos_);

	//サイコロの位置設定
	MV1SetPosition(diceModelId_, dicePos_);
}

void Player::Move(void)
{
	auto& ins = InputManager::GetInstance();

	VECTOR cameraAngles =
		SceneManager::GetInstance().GetCamera()->GetAngles();


		// WASDでカメラを移動させる
	const float movePow = 3.0f;
	VECTOR dir = AsoUtility::VECTOR_ZERO;
	// ゲームパッドが接続数で処理を分ける
	if (GetJoypadNum() == 0)
	{
		// WASDで移動する
		if (ins.IsNew(KEY_INPUT_W)) { dir = { 0.0f, 0.0f, 1.0f }; }
		if (ins.IsNew(KEY_INPUT_A)) { dir = { -1.0f, 0.0f, 0.0f }; }
		if (ins.IsNew(KEY_INPUT_S)) { dir = { 0.0f, 0.0f, -1.0f }; }
		if (ins.IsNew(KEY_INPUT_D)) { dir = { 1.0f, 0.0f, 0.0f }; }
	}
	else
	{
		// 接続されているゲームパッド１の情報を取得
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
		// アナログキーの入力値から方向を取得
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

		// 回転行列を使用して、ベクトルを回転させる
		moveDir_ = VTransform(dir, mat);
		// 移動方向から角度に変換する
		angles_.y = atan2f(moveDir_.x, moveDir_.z);
		// 方向×スピードで移動量を作って、座標に足して移動
		pos_ = VAdd(pos_, VScale(moveDir_, movePow));


	}
}

void Player::DelayRotate(void)
{
	// 移動方向から角度に変換する
	float goal = atan2f(moveDir_.x, moveDir_.z);
	// 常に最短経路で補間
	angles_.y = AsoUtility::LerpAngle(angles_.y, goal, 0.2f);
}
