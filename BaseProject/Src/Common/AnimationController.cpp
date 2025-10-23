#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "AnimationController.h"

AnimationController::AnimationController(void)
{
}

AnimationController::AnimationController(int modelId)
{
	modelId_ = modelId;
	playType_ = -1;
	isLoop_ = true;
}

AnimationController::~AnimationController(void)
{
	
}

void AnimationController::AddInFbx(int type, float speed, int animIndex)
{
	Animation animation;
	animation.model = -1;
	animation.animIndex = animIndex;

	animation.speed = speed;
	

	animation.speed = speed;

	if (animations_.count(type) == 0)
	{
		// 動的配列に追加
		animations_.emplace(type, animation);
	}
}

void AnimationController::Add(int type, float speed, const std::string path)
{
	Animation animation;
	animation.model = MV1LoadModel(path.c_str());
	animation.animIndex = -1;
	

	animation.speed = speed;

	if (animations_.count(type) == 0)
	{
		// 動的配列に追加
		animations_.emplace(type, animation);
	}
}

void AnimationController::Play(int type,bool isLoop)
{
	if (playType_ == type)
	{
		return;
	}

	if (playType_ != -1)
	{
		// 前のアニメーションを停止
		MV1DetachAnim(modelId_, playAnim_.attachNo);
	}



	// アニメーション種別を変更
	playType_ = type;
	playAnim_ = animations_[type];

	playAnim_.step = 0.0f;

	if (playAnim_.model == -1)
	{
		playAnim_.attachNo = MV1AttachAnim(modelId_, playAnim_.animIndex);
	}
	else
	{
		int animIdx = 0;
		playAnim_.attachNo = MV1AttachAnim(modelId_, playAnim_.animIndex, playAnim_.model);
	}
	

	playAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, playAnim_.attachNo);

	isLoop_ = isLoop;
}

void AnimationController::Update(void)
{
	// 経過時間の取得
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// 再生
	playAnim_.step += (deltaTime * playAnim_.speed);

	if (playAnim_.step >= playAnim_.totalTime)
	{
		if (isLoop_)
		{
			playAnim_.step = 0.0f;
		}
		else
		{
			playAnim_.step = playAnim_.totalTime;
		}
	}

	// アニメーション設定
	MV1SetAttachAnimTime(modelId_, playAnim_.attachNo, playAnim_.step);
}

void AnimationController::Release(void)
{
	for (const std::pair<int, Animation>&pair : animations_) {
		if (pair.second.model != -1)
		{
			MV1DeleteModel(pair.second.model);
		}
	}
		//
		animations_.clear();
}

const bool AnimationController::IsEnd(void) const
{
	bool ret = false;

	if (isLoop_)
	{
		return ret;
	}

	if (playAnim_.step >= playAnim_.totalTime)
	{
		return true;
	}

	return ret;
}

int AnimationController::GetPlayType(void) const
{
	return playType_;
}

void AnimationController::Add(int type, float speed, Animation& animation)
{
	animation.speed = speed;

	if (animations_.count(type) == 0)
	{
		// 動的配列に追加
		animations_.emplace(type, animation);
	}
}
