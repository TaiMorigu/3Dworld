#include <DxLib.h>
#include "Stage.h"
#include "../Application.h"
Stage::Stage(void)
{
    modelId_ = -1;
    pos_ = { 0.0f,0.0f,0.0f };

}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
	modelId_ = MV1LoadModel(
        (Application::PATH_MODEL + "Stage.mv1").c_str());

    pos_ = VGet(0.0f, 80.0f, 0.0f);

    // ‰ŠúˆÊ’u‚ğ”½‰f
    if (modelId_ != -1)
    {
        MV1SetPosition(modelId_, pos_);
    }
}

void Stage::Update(void)
{
    // ‰ŠúˆÊ’u‚ğ”½‰f
    if (modelId_ != -1)
    {
        MV1SetPosition(modelId_, pos_);
    }
}

void Stage::Draw(void)
{
    if (modelId_ != -1)
    {
        // ƒ‚ƒfƒ‹‚ğ•`‰æ
        MV1DrawModel(modelId_);
    }
}

void Stage::Release(void)
{
    if (modelId_ != -1)
    {
        MV1DeleteModel(modelId_);
        modelId_ = -1;
    }
}
