#include "MyPG.h"
#include "MyGameMain.h"

namespace Game
{
				//�֐��̃v���g�^�C�v�錾
	//�X�R�A����
	void  Score_Draw(string t_, ML::Box2D tbx_, DG::Font::SP f_);//ML::Color(a,r,g,b)��a�͓����x
	extern int stage;
}

//�^�C�g�����
namespace Title
{
	using namespace Game;

	//�Q�[�����
	DG::Image::SP       imgSpace,imgPlanet,imgBlueBack,imgScreen,imgMenu,imgStgSelectBorder,imgStageSelector,
						imgTitle,imgSubTitle;
	DG::Font::SP		fontA,fontB;

	//�������摜�Ɏg�p
	struct TitleImage {
		State		state;
		ML::Box2D	posBase;
	};

	TitleImage	planet;				//�f��
	TitleImage  blueBack;			//�������̐��F�w�i
	TitleImage  screen;				//�X�N���[��
	TitleImage  menu;				//���j���[���
	TitleImage  title;				//�^�C�g��
	TitleImage  subTitle;			//�T�u�^�C�g��
	TitleImage  stageSelectBorder;	//�X�e�[�W�𕪂���g
	TitleImage  stageSelector;		//�I��ł���X�e�[�W��h��摜

	int planetPosY;		//�f�����c�ɓ��������߂̒l
	int titleSizeW;		//�^�C�g���̏o���Ɏg�p
	int subTitleSizeW;	//�T�u�^�C�g���̏o���Ɏg�p

	int timeCnt;		//���Ԍv���i�V�[�����ƂɃ��Z�b�g�j
	int Scene;			//�V�[��
	int stageMax = 2;	//���ݍ���Ă���X�e�[�W��
	int movingMenu;		//�X�e�[�W�I�����X�N���[������Ƃ��Ɏg�p

			//�֐��̃v���g�^�C�v�錾
	//�V�[�����Ƃɏ�����ύX
	void Scene_UpDate();
	void Scene1_UpDate();
	void Scene2_UpDate();
	void Scene3_UpDate();
	void Scene4_UpDate();
	void Scene5_UpDate();
	void Scene6_UpDate();
	void Scene7_UpDate();

	//�V�[�����X�L�b�v����
	void SceneSkip1();
	void SceneSkip2();

	//�摜�̏�����
	void TitleImage_Initialize(TitleImage& t_, ML::Box2D img_);

	//�w�i�i�F���j�̕\��
	void Space_Draw();

	//�f���̕\��
	void Planet_Draw(TitleImage& t_);

	//�������̐��F�w�i�̕\��
	void BlueBack_Draw(TitleImage& t_);

	//�X�N���[���̕\��
	void Screen_Draw(TitleImage& t_);

	//���j���[��ʂ̕\��
	void Menu_Draw(TitleImage& t_);

	//�X�e�[�W�����̘g�̕\��
	void StageSelectBorder_Draw(TitleImage& t_);

	//�I�𒆂̃X�e�[�W�̕\��
	void StageSelector_Draw(TitleImage& t_);

	//�^�C�g���̕\��
	void Title_Draw(TitleImage& t_);

	//�T�u�^�C�g���̕\��
	void SubTitle_Draw(TitleImage& t_);

	//-----------------------------------------------------------------------------
	//����������
	//�@�\�T�v�F�v���O�����N�����ɂP����s�����i�f�ނȂǂ̏������s���j
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		imgSpace = DG::Image::Create("./data/image/Title/space.png");
		imgPlanet = DG::Image::Create("./data/image/Title/planet.png");
		imgBlueBack = DG::Image::Create("./data/image/Title/blueBack.png");
		imgScreen = DG::Image::Create("./data/image/Title/screen.png");
		imgMenu = DG::Image::Create("./data/image/Title/menu.png");
		imgTitle = DG::Image::Create("./data/image/Title/title.png");
		imgSubTitle = DG::Image::Create("./data/image/Title/subTitle.png");
		imgStgSelectBorder = DG::Image::Create("./data/image/Title/stageSelectBorder.png");
		imgStageSelector = DG::Image::Create("./data/image/Title/stageSelector.png");

		
		fontA = DG::Font::Create("HGP�n�p�p�߯�ߑ�", 20, 48);
		fontB = DG::Font::Create("HGP�n�p�p�߯�ߑ�", 5, 12);

		TitleImage_Initialize(planet, ML::Box2D(0, 20, 480, 270));
		TitleImage_Initialize(blueBack, ML::Box2D(0, -270, 480, 270));
		TitleImage_Initialize(screen, ML::Box2D(0, -270, 480, 270));
		TitleImage_Initialize(title, ML::Box2D(0, 0, 0, 80));
		TitleImage_Initialize(subTitle, ML::Box2D(0, 60, 0, 50));
		TitleImage_Initialize(menu, ML::Box2D(0, 0, 0, 270));
		TitleImage_Initialize(stageSelectBorder, ML::Box2D(80, 49, 0, 145));
		TitleImage_Initialize(stageSelector, ML::Box2D(82, 51, 178, 45));


		Scene = 1;
		movingMenu = 1;
	}
	//-----------------------------------------------------------------------------
	//�������
	//�@�\�T�v�F�v���O�����I�����ɂP����s�����i�f�ނȂǂ̉�����s���j
	//-----------------------------------------------------------------------------
	void Finalize()
	{
		imgSpace.reset();
		imgPlanet.reset();
		imgBlueBack.reset();
		imgMenu.reset();
		imgTitle.reset();
		imgSubTitle.reset();
		imgStgSelectBorder.reset();
		imgStageSelector.reset();

	}
	//-----------------------------------------------------------------------------
	//�X�V����
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ����鏈��
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		auto inp = ge->in1->GetState();

		//�V�[�����X�L�b�v
		if (true == inp.ST.down) {
			if (Scene < 4) { SceneSkip1(); }
			else if (Scene > 4 && Scene < 7) { SceneSkip2(); }
		}

		//�V�[�����Ƃɏ���
		timeCnt++;
		Scene_UpDate();
		
		TaskFlag rtv = TaskFlag::Title;//��肠�������݂̃^�X�N���w��
		if (Scene == 7 && timeCnt > 10) {
			if (true == inp.ST.down) {
				rtv = TaskFlag::Game;	//���̃^�X�N���Q�[���{�҂�
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
		//�w�i�i�F���j
		Space_Draw();

		//�f��
		Planet_Draw(planet);

		//�������̐��F�w�i
		BlueBack_Draw(blueBack);

		//�X�N���[��
		Screen_Draw(screen);

		//���j���[���
		Menu_Draw(menu);

		//�X�e�[�W�����̘g�̕\��
		StageSelectBorder_Draw(stageSelectBorder);

		//�I�𒆂̃X�e�[�W�̓h��Ԃ�
		if (Scene == 7) { StageSelector_Draw(stageSelector); }

		//�^�C�g��
		Title_Draw(title);

		//�T�u�^�C�g��
		SubTitle_Draw(subTitle);

		//�e�L�X�g���b�Z�[�W�i����̃V�[���ŕ\���j
		if (Scene == 4 && timeCnt % 100 >= 45) {
			Score_Draw("press \"S\"", ML::Box2D(150, 170, 300, 300), fontA);
		}

		if (Scene == 7) {
			for (int sc = movingMenu; sc < movingMenu + 3 && sc <= stageMax; sc++) {
				int stageDraw_y = 50 + 48 * (sc - movingMenu);
				Score_Draw("Stage " + to_string(sc), ML::Box2D(100, stageDraw_y, 300, 100), fontA);
			}
		}

	}

	//�֐���`
	//-----------------------------------------------------------------------------
	//�V�[���̃X�L�b�v
	//�V�[��1�`3�Ȃ�V�[��4��
	void SceneSkip1()
	{
		Scene = 3;
		planet.posBase.y = 0;
		title.posBase.w = 480;
		subTitle.posBase.w = 480;
	}
	//�V�[��5,6�̎��̓V�[��7��
	void SceneSkip2()
	{
		Scene = 6;
		subTitle.posBase.y = -50;
		title.posBase.y = -80;
		blueBack.posBase.y = 0;
		screen.posBase.y = 0;
		menu.posBase.w = 480;
		stageSelectBorder.posBase.w = 182;
	}
	//-----------------------------------------------------------------------------
	//�V�[�����Ƃɏ�����ύX����
	void Scene_UpDate()
	{
		switch (Scene) {
		case 1:Scene1_UpDate();	break;
		case 2:Scene2_UpDate();	break;
		case 3:Scene3_UpDate();	break;
		case 4:Scene4_UpDate(); break;
		case 5:Scene5_UpDate(); break;
		case 6:Scene6_UpDate(); break;
		case 7:Scene7_UpDate(); break;
		}
	}

	//�V�[��1�@�f���̈ړ�
	void Scene1_UpDate()
	{
		//�f���̈ړ�
		if (planet.posBase.y > 0) {
			if (timeCnt % 6 == 1) { planet.posBase.y -= 1; }

		}
		//�I��������V�[���؂�ւ�
		else {
			timeCnt = 0;
			Scene = 2;
		}
	}
	//�V�[��2�@�^�C�g���̏o��
	void Scene2_UpDate()
	{
		//�^�C�g���̏o��
		if (title.posBase.w < 480) {
			if (timeCnt % 2 == 1) { title.posBase.w += 10; }
		}
		//�I��������V�[���؂�ւ�
		else {
			timeCnt = 0;
			Scene = 3;
		}
	}
	//�V�[��3�@�T�u�^�C�g���̕\��
	void Scene3_UpDate()
	{
		//��莞�Ԃ�����T�u�^�C�g���\���E�V�[���؂�ւ�
		if (timeCnt > 30) {
			timeCnt = 0;
			subTitle.posBase.w = 480;
			Scene = 4;
		}
	}
	//�V�[��4�@�L�[���͂�҂�
	void Scene4_UpDate()
	{
		auto inp = ge->in1->GetState();
		//timeCnt��0�`100�̊ԂŌJ��Ԃ�
		if (timeCnt >= 100) { timeCnt = 0; }
		//�L�[���͂��ꂽ��V�[���؂�ւ�
		if (true == inp.ST.down) {
			timeCnt = 0;
			Scene = 5;
		}
	}
	//�V�[��5�@�X�N���[�������낷
	void Scene5_UpDate()
	{
		if (subTitle.posBase.y > -50) {
			title.posBase.y-=5;
			subTitle.posBase.y-=5;
		}
		else if (blueBack.posBase.y < 0) {
			screen.posBase.y += 3;
			blueBack.posBase.y += 3;
		}
		else {
			timeCnt = 0;
			Scene = 6;
		}
	}
	//�V�[��6�@���j���[�\��
	void Scene6_UpDate()
	{
		if (menu.posBase.w < 480) {
			menu.posBase.w += 16;
		}
		else if(stageSelectBorder.posBase.w <182) {
			stageSelectBorder.posBase.w = 182;
		}
		else {
			timeCnt = 0;
			Scene = 7;
		}
	}
	//�V�[��7�@�X�e�[�W�\���E�I��
	void Scene7_UpDate()
	{
		auto inp = ge->in1->GetState();
		if (inp.LStick.BD.down && stage < stageMax) { stage++; }
		if (inp.LStick.BU.down && stage > 1) { stage--; }
		if (timeCnt >= 100) { timeCnt = 20;}
	}
	//-----------------------------------------------------------------------------
	//�摜�̏���������(�ȈՔ�)
	void TitleImage_Initialize(TitleImage& t_, ML::Box2D img_)
	{
		t_.state = State::Non;
		t_.posBase = img_;
	}
	//-----------------------------------------------------------------------------
	//�w�i�i�F���j�̕\��
	void Space_Draw()
	{
		ML::Box2D draw(0, 0, 480, 270);
		ML::Box2D src(0, 0, 480, 270);
		imgSpace->Draw(draw, src);
	}
	//-----------------------------------------------------------------------------
	//�f���̕\��
	void Planet_Draw(TitleImage& t_)
	{
		ML::Box2D draw = t_.posBase;
		ML::Box2D src(0, 0, t_.posBase.w, t_.posBase.h);
		imgPlanet->Draw(draw, src);
	}
	//-----------------------------------------------------------------------------
	//�������̐��F�w�i�̕\��
	void BlueBack_Draw(TitleImage& t_)
	{
		ML::Box2D draw = t_.posBase;
		ML::Box2D src(0, 0, t_.posBase.w, t_.posBase.h);
		imgBlueBack->Draw(draw, src);
	}
	//-----------------------------------------------------------------------------
	//�X�N���[���̕\��
	void Screen_Draw(TitleImage& t_)
	{
		ML::Box2D draw = t_.posBase;
		ML::Box2D src(0, 0, t_.posBase.w, t_.posBase.h);
		imgScreen->Draw(draw, src);
	}
	//-----------------------------------------------------------------------------
	//���j���[��ʂ̕\��
	void Menu_Draw(TitleImage& t_)
	{
		ML::Box2D draw = t_.posBase;
		ML::Box2D src(0, 0, t_.posBase.w, t_.posBase.h);
		imgMenu->Draw(draw, src);
	}
	//-----------------------------------------------------------------------------
	//�X�e�[�W�����̘g�̕\��
	void StageSelectBorder_Draw(TitleImage& t_)
	{
		ML::Box2D draw = t_.posBase;
		ML::Box2D src(0, 0, t_.posBase.w, t_.posBase.h);
		imgStgSelectBorder->Draw(draw, src);
	}
	//-----------------------------------------------------------------------------
	//�I�𒆃X�e�[�W�̓h��Ԃ�
	void StageSelector_Draw(TitleImage& t_)
	{
		ML::Box2D draw = t_.posBase;
		draw.y += (stage - 1) * 48;
		ML::Box2D src(0, 0, t_.posBase.w, t_.posBase.h);
		imgStageSelector->Draw(draw, src);
	}
	//-----------------------------------------------------------------------------
	//�^�C�g���̕\��
	void Title_Draw(TitleImage& t_)
	{
		ML::Box2D draw = t_.posBase;
		ML::Box2D src(0, 0, t_.posBase.w, t_.posBase.h);
		imgTitle->Draw(draw, src);
	}
	//-----------------------------------------------------------------------------
	//�T�u�^�C�g���̕\��
	void SubTitle_Draw(TitleImage& t_)
	{
		ML::Box2D draw = t_.posBase;
		ML::Box2D src(0, 0, t_.posBase.w, t_.posBase.h);
		imgSubTitle->Draw(draw, src);
	}
}