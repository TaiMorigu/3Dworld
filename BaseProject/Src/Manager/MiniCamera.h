#pragma once
#include <DxLib.h>
#include "../Common/Vector2.h"
class Camera;

class MiniCamera
{

public:

	// �~�j��ʂ̃T�C�Y
	static constexpr int SIZE_X = 200;
	static constexpr int SIZE_Y = 200;

	// �w�i�F
	static constexpr int BACKGROUND_COLOR_R = 200;
	static constexpr int BACKGROUND_COLOR_G = 200;
	static constexpr int BACKGROUND_COLOR_B = 200;

	// �~�j�J�������f���̑傫��
	static constexpr float SCALE = 3.0f;

	// �R���X�g���N�^
	MiniCamera(Camera* camera);

	// �f�X�g���N�^
	~MiniCamera(void);

	// ������
	void Init(void);

	// �X�V
	void Update(void);

	// �~�j�J�����p������̕`��
	void DrawScreen(void);

	// �`��
	void Draw(void);

	// ���
	void Release(void);

	// �X�N���[���̃n���h��ID�擾
	int GetScreen(void) const;

private:

	// �J����
	Camera* camera_;

	// �X�N���[���̃n���h��ID
	int miniScreen_;

	// �~�j�J�������f���̃n���h��ID
	int modelId_;
	
	// �~�j�J�����p�̕`��̈�T�C�Y
	Vector2 size_;

	// �~�j�J�������f���̈ʒu
	VECTOR pos_;

};
