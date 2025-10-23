#pragma once
#include <chrono>
class SceneBase;
class Fader;
class Camera;
class MiniCamera;
class Stage;

class SceneManager
{

public:

	// �w�i�F
	static constexpr int BACKGROUND_COLOR_R = 0;
	static constexpr int BACKGROUND_COLOR_G = 139;
	static constexpr int BACKGROUND_COLOR_B = 139;

	// �f�B���N�V���i�����C�g�̕���
	static constexpr VECTOR LIGHT_DIRECTION = { 0.3f, -0.7f, 0.8f };

	// �V�[���Ǘ��p
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME
	};
	
	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static SceneManager& GetInstance(void);

	// ������
	void Init(void);
	
	// 3D�̏�����
	void Init3D(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

	// ���\�[�X�̔j��
	void Destroy(void);

	// ��ԑJ��
	void ChangeScene(SCENE_ID nextId);

	// �V�[��ID�̎擾
	SCENE_ID GetSceneID(void);

	// �f���^�^�C���̎擾
	float GetDeltaTime(void) const;

	// �J�����̎擾
	Camera* GetCamera(void) const;

private:

	// �ÓI�C���X�^���X
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	// �t�F�[�h
	Fader* fader_;

	// �e��V�[��
	SceneBase* scene_;

	// �J����
	Camera* camera_;

	// �~�j�J����
	MiniCamera* miniCamera_;

	Stage* stage_;

	// �V�[���J�ڒ�����
	bool isSceneChanging_;

	// �f���^�^�C��
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	
	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	SceneManager(void);

	// �R�s�[�R���X�g���N�^�����l
	SceneManager(const SceneManager& instance) = default;

	// �f�X�g���N�^�����l
	~SceneManager(void) = default;

	// �f���^�^�C�������Z�b�g����
	void ResetDeltaTime(void);

	// �V�[���J��
	void DoChangeScene(SCENE_ID sceneId);

	// �t�F�[�h
	void Fade(void);

};