#pragma once
#include <DxLib.h>
class AnimationController;
class Player
{
public:
	// �A�j���[�V�����̍Đ����x
	static constexpr float SPEED_ANIM = 20.0f;
	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		MAX,
	};
	// �R���X�g���N�^
	Player(void);
	// �f�X�g���N�^
	~Player(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	
private:
	// �A�j���[�V��������
	AnimationController* animationController_;
	// �A�j���[�V�������
	ANIM_TYPE animType_;
	// ���f�����
	int modelId_;
	VECTOR pos_;
	VECTOR angles_;
	VECTOR scales_;
	// ���[�J���p�x
	VECTOR localAngles_;

	// �T�C�R�����f�����
	int diceModelId_;
	VECTOR dicePos_;
	VECTOR diceAngles_;
	VECTOR diceScales_;

	VECTOR diceLocalPos_;
	VECTOR diceLocalAngles_;

	// �ړ�����
	VECTOR moveDir_;

	void InitDice(void);
	void SyncDice(void);

	void Move(void);
	// �v���C���[�̒x����]����
	void DelayRotate(void);
};

