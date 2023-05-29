#include "MyGameMain.h"

//�Q�[�����
TaskFlag			actTask, nextTask;

//-----------------------------------------------------------------------------
//����������
//�@�\�T�v�F�v���O�����N�����ɂP����s�����i�f�ނȂǂ̏������s���j
//-----------------------------------------------------------------------------
void  MyGameMain_Initialize()
{
	//�L�[�{�[�h�̓��͂��󂯎��I�u�W�F�N�g�𐶐�����
	//�A�i���O�X�e�B�b�NL
	XI::AnalogAxisKB	ls = { DIK_LEFT, DIK_RIGHT, DIK_UP, DIK_DOWN };
	XI::AnalogAxisKB	rs = { DIK_NUMPAD4, DIK_NUMPAD6, DIK_NUMPAD8, DIK_NUMPAD2 };
	XI::AnalogTriggerKB  tg = { DIK_E, DIK_R };
	XI::KeyDatas_KB	 key = {
		{ DIK_Z, XI::VGP::B1 },{ DIK_X, XI::VGP::B2 },{ DIK_C, XI::VGP::B3 },
	{ DIK_V, XI::VGP::B4 },{ DIK_A, XI::VGP::SE },{ DIK_S, XI::VGP::ST },
	{ DIK_Q, XI::VGP::L1 },{ DIK_W, XI::VGP::R1 },
	{ DIK_D, XI::VGP::L3 },{ DIK_NUMPAD5, XI::VGP::R3 },
	};
	ge->in1 = XI::GamePad::CreateKB(ls, rs, tg, key);

	//�Q�[���p�b�h�̓��͂��󂯎��I�u�W�F�N�g�𐶐�����
	XI::KeyDatas_GP	but = {
		{ XI::RGP::B01, XI::VGP::B1 },{ XI::RGP::B02, XI::VGP::B2 },
	{ XI::RGP::B03, XI::VGP::B3 },{ XI::RGP::B04, XI::VGP::B4 },
	{ XI::RGP::B05, XI::VGP::L1 },{ XI::RGP::B06, XI::VGP::R1 },
	{ XI::RGP::B07, XI::VGP::SE },{ XI::RGP::B08, XI::VGP::ST },
	{ XI::RGP::B09, XI::VGP::L3 },{ XI::RGP::B10, XI::VGP::R3 },
	};
	auto wgp = XI::GamePad::CreateGP(0, but);
	//�L�[�{�[�h�ƃQ�[���p�b�h����
	XI::GamePad::Link(ge->in1, wgp);

	ge->in2 = XI::GamePad::CreateGP(1, but);
	ge->in3 = XI::GamePad::CreateGP(2, but);
	ge->in4 = XI::GamePad::CreateGP(3, but);
	ge->mouse = XI::Mouse::Create(2, 2);

	actTask = TaskFlag::Non;	//������Ԃ͎��s���̃^�X�N�𖳌��ɂ��Ă���
	nextTask = TaskFlag::Title;	//���̃^�X�N���^�C�g���ɂ��邱�Ƃōŏ��Ƀ^�C�g�������s�����
}
//-----------------------------------------------------------------------------
//�O���t�@�C�����
namespace  Title {//�^�C�g���^�X�N
	extern  void  Initialize();
	extern  TaskFlag  UpDate();
	extern  void  Render();
	extern  void  Finalize();
}
namespace  Game {//�Q�[���{�҃^�X�N
	extern  void  Initialize();
	extern  TaskFlag  UpDate();
	extern  void  Render();
	extern  void  Finalize();
}
namespace  Result {//���U���g�^�X�N
	extern  void  Initialize();
	extern  TaskFlag  UpDate();
	extern  void  Render();
	extern  void  Finalize();
}
namespace  GameOver {//�Q�[���I�[�o�[�^�X�N
	extern  void  Initialize();
	extern  TaskFlag  UpDate();
	extern  void  Render();
	extern  void  Finalize();
}

//�ȉ��K�v�ɉ����Ēǉ�

//-----------------------------------------------------------------------------
//�������
//�@�\�T�v�F�v���O�����I�����ɂP����s�����i�f�ނȂǂ̉�����s���j
//-----------------------------------------------------------------------------
void  MyGameMain_Finalize()
{
	//�r���ŏI���v�����ꂽ�ꍇ�̑΍�
	//�����s���̃^�X�N�̏I���������Ăт���
	switch (actTask) {
	case TaskFlag::Title:		Title::Finalize();	break;
	case TaskFlag::Game:		Game::Finalize();	break;
	case TaskFlag::Result:		Result::Finalize(); break;
	case TaskFlag::GameOver:	GameOver::Finalize(); break;
		//�ȉ��K�v�ɉ����Ēǉ�
	}

	ge->in1.reset();
	ge->in2.reset();
	ge->in3.reset();
	ge->in4.reset();
	ge->mouse.reset();
}
//-----------------------------------------------------------------------------
//�X�V����
//�@�\�T�v�F�Q�[���̂P�t���[���ɓ����鏈��
//-----------------------------------------------------------------------------
void  MyGameMain_UpDate()
{
	//���Ɏ��s����^�X�N���A���̃^�X�N�ƈ�v���Ȃ�
	if (actTask != nextTask) {
		//�����s���̃^�X�N�̏I���������Ăт���
		switch (actTask) {
		case TaskFlag::Title:		Title::Finalize();	break;
		case TaskFlag::Game:		Game::Finalize();	break;
		case  TaskFlag::Result:		Result::Finalize(); break;
		case TaskFlag::GameOver:	GameOver::Finalize(); break;

			//�ȉ��K�v�ɉ����Ēǉ�
		}
		//���̃^�X�N�Ɉڂ�
		actTask = nextTask;
		//���̃^�X�N�̏������������Ăт���
		switch (actTask) {
		case TaskFlag::Title:		Title::Initialize();	break;
		case TaskFlag::Game:		Game::Initialize();		break;
		case TaskFlag::Result:		Result::Initialize();	break;
		case TaskFlag::GameOver:	GameOver::Initialize(); break;

			//�ȉ��K�v�ɉ����Ēǉ�
		}
	}
	//���݂̃^�X�N�̎��s�E�X�V�������Ăт���
	switch (actTask) {
	case TaskFlag::Title:		nextTask = Title::UpDate();		break;
	case TaskFlag::Game:		nextTask = Game::UpDate();		break;
	case TaskFlag::Result:		nextTask = Result::UpDate();	break;
	case TaskFlag::GameOver:	nextTask = GameOver::UpDate();	break;

		//�ȉ��K�v�ɉ����Ēǉ�
	}
}
//-----------------------------------------------------------------------------
//�`�揈��
//�@�\�T�v�F�Q�[���̂P�t���[���ɓ�����\������ �Q�c
//-----------------------------------------------------------------------------
void  MyGameMain_Render2D()
{
	switch (actTask) {
	case TaskFlag::Title:		Title::Render();	break;
	case TaskFlag::Game:		Game::Render();		break;
	case TaskFlag::Result:		Result::Render();	break;
	case TaskFlag::GameOver:	GameOver::Render(); break;
	//�ȉ��K�v�ɉ����Ēǉ�
	}
}
