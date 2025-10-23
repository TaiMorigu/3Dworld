#pragma once
#include <DxLib.h>
class Player;

class Camera
{

public:

	// �J�����̏������W
	static constexpr VECTOR DERFAULT_POS = { 0.0f, 200.0f, -500.0f };
	// �J�����̏����p�x
	static constexpr VECTOR DERFAULT_ANGLES = {
	0.0f, 0.0f, 0.0f
	};

	// �Ǐ]�Ώۂ���̑��΍��W
	static constexpr VECTOR FOLLOW_LOCAL_POS = { 0.0f, 200.0f, -500.0f };

	// �Ǐ]�Ώۂ���J�����ւ̑��΍��W
	static constexpr VECTOR FOLLOW_CAMERA_LOCAL_POS = { 0.0f, 200.0f, -350.0f };

	// �Ǐ]�Ώۂ��璍���_�ւ̑��΍��W
	static constexpr VECTOR FOLLOW_TARGET_LOCAL_POS = { 0.0f, 0.0f, 150.0f };
	// �J�����̃N���b�v�͈�
	static constexpr float VIEW_NEAR = 1.0f;
	static constexpr float VIEW_FAR = 30000.0f;
	// �J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT, // ��_�J����
		FREE, // �t���[���[�h
		FOLLOW,// �Ǐ]���[�h
	};
	// �R���X�g���N�^
	Camera(void);

	// �f�X�g���N�^
	~Camera(void);

	// ������
	void Init(void);

	// �X�V
	void Update(void);

	// �`��O�̃J�����ݒ�
	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);
	void SetBeforeDrawFollow(void);

	// �f�o�b�O�p�`��
	void DrawDebug(void);

	// ���
	void Release(void);

	// ���W�̎擾
	const VECTOR& GetPos(void) const;

	// �p�x�̎擾
	const VECTOR& GetAngles(void) const;

	// �����_�̎擾
	const VECTOR& GetTargetPos(void) const;

	// �J�������[�h�̕ύX
	void ChangeMode(MODE mode);

	// �Ǐ]����̐ݒ�
	void SetFollow(Player* follow);
private:

	// �Ǐ]����
	Player* follow_;

	// �J�������[�h
	MODE mode_;

	// �J�����̈ʒu
	VECTOR pos_;

	// �J�����̊p�x
	VECTOR angles_;

	// �����_
	VECTOR targetPos_;

	void MoveXYZDirecton(void);

	void MoveXYZDirectionPad(void);

};
