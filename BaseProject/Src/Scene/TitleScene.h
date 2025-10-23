#pragma once
#include "SceneBase.h"
class Grid;
class Stage;
class Player;

class TitleScene : public SceneBase
{

public:

	// �R���X�g���N�^
	TitleScene(void);

	// �f�X�g���N�^
	~TitleScene(void) override;

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// �O���b�h��
	Grid* grid_;
    //�X�e�[�W
	Stage* stage_;
	//�v���C���[
	Player* player_;

};
