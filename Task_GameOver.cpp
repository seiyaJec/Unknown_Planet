#include "MyPG.h"
#include "MyGameMain.h"

//�^�C�g�����
namespace GameOver
{
	//�Q�[�����
	DG::Image::SP imgGameOver, imgSelector;	//�w�i
	int SelectTask;
	int timeCnt;
	//-----------------------------------------------------------------------------
	//����������
	//�@�\�T�v�F�v���O�����N�����ɂP����s�����i�f�ނȂǂ̏������s���j
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		imgGameOver = DG::Image::Create("./data/image/GameOver.png");
		imgSelector = DG::Image::Create("./data/image/Selector.png");
		SelectTask = 1;
		timeCnt = 0;
	}
	//-----------------------------------------------------------------------------
	//�������
	//�@�\�T�v�F�v���O�����I�����ɂP����s�����i�f�ނȂǂ̉�����s���j
	//-----------------------------------------------------------------------------
	void Finalize()
	{
		imgGameOver.reset();
	}
	//-----------------------------------------------------------------------------
	//�X�V����
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ����鏈��
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		auto inp = ge->in1->GetState();


		if (true == inp.LStick.BU.down) { SelectTask = 1; }
		if (true == inp.LStick.BD.down) { SelectTask = 2; }


		TaskFlag rtv = TaskFlag::GameOver;//��肠�������݂̃^�X�N���w��
			if (true == inp.ST.down) {
				switch (SelectTask) {
				case 1:rtv = TaskFlag::Game;	break;	//���̃^�X�N���Q�[���{�҂�
				case 2:rtv = TaskFlag::Title;	break;	//���̃^�X�N���^�C�g����
				}
			}
		return rtv;
	}
	//-----------------------------------------------------------------------------
	//�`�揈��
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ�����\������
	//-----------------------------------------------------------------------------
	void Render()
	{
		//�w�i�̕\��
		ML::Box2D drawA(0, 0, 480, 270);
		ML::Box2D srcA(0, 0, 480, 270);
		imgGameOver->Draw(drawA, srcA);
		//���̕\��
			ML::Box2D drawB(190, 0, 64, 64);
			drawB.y = 122 + SelectTask * 35;
			ML::Box2D srcB(0, 0, 64, 64);
			imgSelector->Draw(drawB, srcB);
	}
}