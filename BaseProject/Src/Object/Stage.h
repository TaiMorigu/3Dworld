#pragma once
#include <DxLib.h>
class Stage
{
public:
	Stage(void);
	~Stage(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
private:
	// �X�e�[�W���f���̃n���h��ID
	int modelId_;
	// �X�e�[�W���f���̈ʒu
	VECTOR pos_;
};

