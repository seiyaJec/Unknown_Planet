 #include "MyPG.h"
#include "MyGameMain.h"

//�Q�[���{��
namespace Game
{
	//�Q�[�����
	DG::Image::SP  imgPlayer, imgShot, imgPlayerHPBar, imgPlayerHPPBar,												//�v���C���[
		imgSlime,imgSlimeShotA,imgSlimeShotB, imgSlimeHPBar, imgSlimeHPPBar, imgSlimeHPBorder,						//�X���C��
		imgEnemyA, imgEnemyB, imgEnemyC, BombImg, imgEnemyHPBar, imgEnemyHPPBar,									//�G�E����
		imgBGa, imgBGb, imgBGc, imgBGd, imgscoreBG, imgDangerMessage, imgStar, imgSPGauge,imgPause,imgSelector,		//���̑��i�A�C�e���A�w�i�Ȃǁj
		imgEnemyBoss;																								//�{�X
	DG::Font::SP   fontA,fontB;



	//--------�X�e�[�^�X�ݒ�--------//
	int  player_speed = 3;					//�v���C���[�̈ړ����x
	int  playerShotA_mode = 1;				//�v���C���[�e�̃��[�h�i0.�^������/1.�����Ə��j

	//�v���C���[�e�̏o��Ԋu�i�����l�E���l���������قǊԊu���Z���j
	int  playerShotA_shotDistance = 20;
	int  playerShotB_shotDistance = 40;

	//���x�����ƂɕK�v��SP�̗ʁi���x�����オ�邲�Ƃ�SP��0�ɂȂ�j
	int  level_maxSP[5] = { 5,10,20,20,30 };

	//�e�L�����̍ő�HP�i�����l�j
	int  player_maxHP = 50;
	int  slime_maxHP = 100;
	int  enemyA_maxHP = 30;
	int  enemyB_maxHP = 30;
	int  enemyC_maxHP = 1;
	int  enemyBoss_maxHP = 3000;

	//�e�e�̍U����
	int  playerShotA_maxHP = 10;
	int  playerShotB_maxHP = 10;
	int  slimeShotB_maxHP = 10;
	int  enemyShotA_maxHP = 10;
	int  enemyShotB_maxHP = 10;

	//��ʓ��ɕ\���ł���e�̍ő�l�̐ݒ�
	const int SHOTS_MAX		 = 30;
	const int ENEMYSHOTS_MAX = 100;
	const int ENEMYS_MAX	 = 30;
	const int BOMBS_MAX		 = 50;
	const int STARS_MAX		 = 300;
	//------------------------------///
	struct  Back {
		ML::Box2D backImage;
		int		  typeNum;
		int		  moveCnt;
		int		  moveMax;		//���t���[�����Ƃɓ������������߂�i�ŏ��͂P�j
		int       moveSpeed;	//�w�i����������
	};

	Chara  player;
	Chara  slime;
	Chara  slimeShot;					//�X���C���V���b�g
	Chara  shots[SHOTS_MAX];			//�V���b�g
	Chara  enemyshots[ENEMYSHOTS_MAX];	//�G�e
	Chara  enemys[ENEMYS_MAX];			//�G
	Chara  boss;						//�{�X
	Chara  bombs[BOMBS_MAX];			//�����G�t�F�N�g
	Chara  stars[STARS_MAX];			//�A�C�e��
	Chara  pauseSelector;				//�|�[�Y��ʂɕ\��������
	Back   backSky[6];					//��
	Back   backSea[6];					//�C
	Back   backCloud[2];				//�_
	Back   backGround[2];				//�n�`
	Back   scoreback[2];				//�X�R�A�w�i
	Gauge  PlayerHPGauge;				//�v���C���[��HP�Q�[�W
	Gauge  SlimeHPGauge;				//�X���C����HP�Q�[�W
	Gauge  EnemysHPGauge[ENEMYS_MAX];	//�G��HP�Q�[�W
	Gauge  SPGauge;						//SP�Q�[�W�i�X���C���|�C���g�Q�[�W�j
	int	   timeCnt;						//�G�o���Ɏg�p
	int    shotTimeCnt[2];				//�e�̏o���Ɏg�p(����2��ޕ�)
	int	   SP;							//�X�L���|�C���g
	int    slimeLevel;					//�X���C���̃��x��
	int	   enemysDefeated;				//�|�����G�̐�
	int	   enemyShotsDefeated;			//�e���󂵂���(�G�eB�̂݁j
	int    score;						//�X�R�A
	int    stage = 1;					//���݂̃X�e�[�W
	int    gameOverCnt;					//�Q�[���I�[�o�[�^�X�N�֐؂�ւ��܂ł̊Ԋu
	bool   stageClear;					//true�Ȃ烊�U���g�^�X�N�֐؂�ւ�鏈�����s����
	bool   SearchDanger;				//true�Ȃ�u!�v�̊G���o��
	bool   Pause;						//true�Ȃ�|�[�Y��ʕ\��

					//�֐��̃v���g�^�C�v�錾
	//�v���C���[
	void  Player_Initialize(Chara& c_, int  x_, int  y_);
	void  Player_Draw(Chara& c_);
	void  Player_UpDate(Chara& c_);

	//�e
	void  Shot_Appear(int tn_, int  x_, int  y_);

	void  Shot_Initialize(Chara& c_, int tn_, int x_, int y_);
	void  ShotA_Initialize(Chara& c_, int x_, int y_);
	void  ShotB_Initialize(Chara& c_, int x_, int y_);

	void  Shot_Draw(Chara& c_);
	void  ShotA_Draw(Chara& c_);
	void  ShotB_Draw(Chara& c_);

	void  Shot_UpDate(Chara& c_);
	void  ShotA_UpDate(Chara& c_);
	void  ShotB_UpDate(Chara& c_);


	//�X���C��
	void  Slime_Initialize(Chara& c_, int x_, int y_);
	void  Slime_Draw(Chara& c_);
	void  Slime_UpDate(Chara& c_);

	//�X���C���V���b�g
	void  SlimeShot_Initialize(Chara& c_, int m_);
	void  SlimeShotA_Initialize(Chara& c_);
	void  SlimeShotB_Initialize(Chara& c_);

	void  SlimeShot_Draw(Chara& c_);
	void  SlimeShotA_Draw(Chara& c_);
	void  SlimeShotB_Draw(Chara& c_);

	void  SlimeShot_UpDate(Chara& c_);
	void  SlimeShotA_UpDate(Chara& c_);
	void  SlimeShotB_UpDate(Chara& c_);

	//�G
	void  LoadStageData();
	void  Enemy_AppearTimer1();
	void  Enemy_AppearTimer2();
	void  Enemy_AppearTimer3();
	bool  DetectEnemy();
	void  Enemy_Appear(int tn_, int x_, int y_);

	void  Enemy_Initialize(Chara& c_, int tn_, int x_, int y_);
	void  EnemyA_Initialize(Chara& c_, int  x_, int  y_);
	void  EnemyB_Initialize(Chara& c_, int x_, int y_);
	void  EnemyC_Initialize(Chara& c_, int x_, int y_);

	void  Enemy_Draw(Chara& c_);
	void  EnemyA_Draw(Chara& c_);
	void  EnemyB_Draw(Chara& c_);
	void  EnemyC_Draw(Chara& c_);

	void  Enemy_UpDate(Chara& c_);
	void  EnemyA_UpDate(Chara& c_);
	void  EnemyB_UpDate(Chara& c_);
	void  EnemyC_UpDate(Chara& c_);

	void  EnemyBoss_Initialize(Chara& c_, int x_, int y_);
	void  EnemyBoss_Draw(Chara& c_);
	void  EnemyBoss_UpDate(Chara& c_);

	//�G�e
	void  EnemyShot_Appear(int tn_,int x_, int y_);

	void  EnemyShot_Initialize(Chara& c_, int tn_, int x_, int y_);
	void  EnemyShotA_Initialize(Chara& c_, int x_, int y_);
	void  EnemyShotB_Initialize(Chara& c_, int x_, int y_);

	void  EnemyShot_Draw(Chara& c_);
	void  EnemyShotA_Draw(Chara& c_);
	void  EnemyShotB_Draw(Chara& c_);

	void  EnemyShot_UpDate(Chara& c_);
	void  EnemyShotA_UpDate(Chara& c_);
	void  EnemyShotB_UpDate(Chara& c_);

	//�A�C�e��
	void  Star_Appear(int x_, int y_, int ang_);
	void  Star_Initialize(Chara& c_, int x_, int y_,int ang_);
	void  Star_Draw(Chara& c_);
	void  Star_UpDate(Chara& c_);

	//����
	void  Bomb_Appear(int x_, int y_);
	void  Bomb_Initialize(Chara& c_, int x_, int y_);
	void  Bomb_UpDate(Chara& c_);
	void  Bomb_Draw(Chara& c_);

	//�w�i
	void  Back_Initialize(Back& b_, int tn_, int mv_, int spd_, ML::Box2D box_);
	void  Back_UpDate(Back& b_,int bgCnt_);
	void  Back_Draw(Back& b_);

	//�X�R�A����
	void  Score_Initialize(string f_, int w_, int h_, DG::Font::SP& font_);
	void  Score_Draw(string t_, ML::Box2D tbx_, DG::Font::SP f_);

	//���x��
	void  SlimeLevel_UpDate();

	//HP�ESP�Q�[�W
	void  Gauge_Initialize(Gauge& hp_, int tn_, ML::Box2D box_);
	void  HPGauge_Draw(Chara c_, Gauge& hp_);
	void  SlimeHPGauge_Draw(Chara c_, Gauge& hp_);
	void  EnemyHPGauge_Draw(Chara c_, Gauge& hp_);
	void  PlayerHPGauge_Draw(Chara c_, Gauge& hp_);


	void  SPGauge_Draw(Gauge& sp_);

	//�|�[�Y���
	void  Pause_Initialize(Chara& c_);
	void  Pause_Draw(Chara& c_);
	void  Pause_UpDate(Chara& c_, TaskFlag& r_);

	//�ŏ������߂�֐�
	int  Min(int a, int b);

	//�e�̊p�x�����߂�֐�
	void  PlayerShotAngleSetting(Chara& c1_, Chara c2_);

	//�X�R�A�𕶎��������鏈��
	string scoreToString(int s_);

	//�����蔻��̋�`�̐ݒ�
	ML::Box2D CenterPointHitBox(int w_, int h_);

	//�댯���b�Z�[�W�̕\��
	void  DangerMessage_Draw();

	//�~�Ƌ�`�̓����蔻��
	bool  HitCtoS(int Lx, int Ly, int Rx, int Ry, int x, int y, int radius);
		//Lx,Ly,Rx,Ry�͋�`�L�����̍���ƉE���̍��W�Ax,y,radius�͉~�`�L�����̍��W�Ɣ��a


	//�L�������m�̓����蔻��
	bool  Characters_HitCheck(Chara& c1_, Chara& c2_);				//��`�Ƌ�`
	bool  Characters_HitCheck_CtoS(Chara& cSquare_, Chara& cCircle_);//��`�Ɖ~


	//�N���A����
	void  StageClearDecision();

	//-----------------------------------------------------------------------------
	//����������
	//�@�\�T�v�F�v���O�����N�����ɂP����s�����i�f�ނȂǂ̏������s���j
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		imgPlayer = DG::Image::Create("./data/image/player.png");
		imgShot = DG::Image::Create("./data/image/Shot.png");
		imgPlayerHPBar = DG::Image::Create("./data/image/HPBar/PlayerHPBar.png");
		imgPlayerHPPBar = DG::Image::Create("./data/image/HPBar/PlayerHPProgressBar.png");
		imgSlime = DG::Image::Create("./data/image/Slime.png");
		imgSlimeShotA = DG::Image::Create("./data/image/SlimeShotA.png");
		imgSlimeShotB = DG::Image::Create("./data/image/SlimeShotB.png");
		imgSlimeHPBar = DG::Image::Create("./data/image/HPBar/SlimeHPBar.png");
		imgSlimeHPPBar = DG::Image::Create("./data/image/HPBar/SlimeHPProgressBar.png");
		imgSlimeHPBorder = DG::Image::Create("./data/image/HPBar/SlimeHPProgressBarBorder.png");
		imgEnemyA = DG::Image::Create("./data/image/Enemy1.png");
		imgEnemyB = DG::Image::Create("./data/image/Enemy2.png");
		imgEnemyC = DG::Image::Create("./data/image/Enemy3.png");
		BombImg = DG::Image::Create("./data/image/bomb.png");
		imgEnemyHPBar = DG::Image::Create("./data/image/HPBar/EnemyHPBar.png");
		imgEnemyHPPBar = DG::Image::Create("./data/image/HPBar/EnemyHPProgressBar.png");
		imgBGa = DG::Image::Create("./data/image/BackGrounds/sky.png");
		imgBGb = DG::Image::Create("./data/image/BackGrounds/sea.png");
		imgBGc = DG::Image::Create("./data/image/BackGrounds/clouds.png");
		imgBGd = DG::Image::Create("./data/image/BackGrounds/far-grounds.png");
		imgscoreBG = DG::Image::Create("./data/image/BackGrounds/scoreBG.png");
		imgDangerMessage = DG::Image::Create("./data/image/Danger.png");
		imgStar = DG::Image::Create("./data/image/Star.png");
		imgSPGauge = DG::Image::Create("./data/image/SPGauge.png");
		imgPause = DG::Image::Create("./data/image/Pause.png");
		imgSelector = DG::Image::Create("./data/image/selector.png");
		imgEnemyBoss = DG::Image::Create("./data/image/Boss.png");



		//�|�[�Y��ʂ̏�����
		Pause_Initialize(pauseSelector);

		//�v���C���̏�����
		Player_Initialize(player, 50, 270 / 2);
		//�v���C���[HP�Q�[�W�̏�����
		Gauge_Initialize(PlayerHPGauge, 2, ML::Box2D(11, 270 - 17, 148, 4));
		//�|�����G�̐���0��
		enemysDefeated = 0;
		//�󂵂��G�e�̐���0��
		enemyShotsDefeated = 0;

		//�X���C���̏�����
		Slime_Initialize(slime, 50, 250 - 20);
		//�X���C��HP�Q�[�W�̏�����
		Gauge_Initialize(SlimeHPGauge, 0, ML::Box2D(10, 270 - 18, 150, 16));
		
		//�e�̏������i�������j
		for (int s = 0; s < SHOTS_MAX; ++s) { shots[s].state = State::Non; }

		//�X���C���V���b�g�̏������i�������j
		slimeShot.state = State::Non;

		//�G�̏�����
		for (int e = 0; e < ENEMYS_MAX; ++e) {
			enemys[e].state = State::Non;
			Gauge_Initialize(EnemysHPGauge[e], 1, ML::Box2D(-10,-50,20,2));
		}
		//�G�o���J�E���^
		timeCnt = 0;

		//�e�̏������i�������j
		for (int es = 0; es < ENEMYSHOTS_MAX; ++es) { enemyshots[es].state = State::Non; }

		//�A�C�e���̏������i�������j
		for (int str = 0; str < STARS_MAX; str++) { stars[str].state = State::Non; }

		//�����̏�����
		for (int be = 0; be < BOMBS_MAX; ++be) { bombs[be].state = State::Non; }

		//�w�i�̏�����
		//��
		for (int b = 0; b < 6; b++) {
			Back_Initialize(backSky[b], 0, 10, 1, ML::Box2D(b * 112, 0, 112, 304));
		}
		//�C
		for (int b = 0; b < 6; b++) {
			Back_Initialize(backSea[b], 1, 2, 1, ML::Box2D(b * 112, (250 - 96), 112, 96));
		}
		//�_
		for (int b = 0; b < 2; b++) {
			Back_Initialize(backCloud[b], 2, 10, 1, ML::Box2D(b * 544, 0, 544, 236));
		}
		//��
		for (int b = 0; b < 2; b++) {
			Back_Initialize(backGround[b], 3, 3, 1, ML::Box2D(b * 616, (250 - 96 - 110), 616, 110));
		}

		//�X�R�A�w�i�̏�����
		for (int b = 0; b < 2; b++) {
			Back_Initialize(scoreback[b], 10, 1, 1, ML::Box2D(b * 480, (270 - 20), 480, 20));

		}
		//�X�R�A�̏�����
		Score_Initialize("HGP�n�p�p�߯�ߑ�", 10, 20,fontA);
		Score_Initialize("HGP�n�p�p�߯�ߑ�", 20, 40, fontB);
		score = 0;

		//SP�Q�[�W�̏�����
		for(int sp = 0;sp < 5;sp++)
		Gauge_Initialize(SPGauge, 10, ML::Box2D(200, 270 - 18, 16, 16));//Box2D��5����Q�[�W�̓��̈�ԍ��̍��W


		//�댯�`�F�b�N���I�t��
		SearchDanger = false;
		//�N���A������I�t��
		stageClear = false;
		//�Q�[���I�[�o�[�܂ł̊Ԋu�����Z�b�g
		gameOverCnt = 0;

	}
	//-----------------------------------------------------------------------------
	//�������
	//�@�\�T�v�F�v���O�����I�����ɂP����s�����i�f�ނȂǂ̉�����s���j
	//-----------------------------------------------------------------------------
	void Finalize()
	{
		imgPlayer.reset();
		imgShot.reset();
		imgPlayerHPBar.reset();
		imgPlayerHPPBar.reset();
		imgSlime.reset();
		imgSlimeShotA.reset();
		imgSlimeShotB.reset();
		imgEnemyA.reset();
		imgEnemyB.reset();
		imgEnemyC.reset();
		BombImg.reset();
		imgBGa.reset();
		imgBGb.reset();
		imgBGc.reset();
		imgBGd.reset();
		imgscoreBG.reset();
		fontA.reset();
		imgSlimeHPBar.reset();
		imgSlimeHPPBar.reset();
		imgSlimeHPBorder.reset();
		imgEnemyHPBar.reset();
		imgEnemyHPPBar.reset();
		imgDangerMessage.reset();
		imgStar.reset();
		imgSPGauge.reset();
		imgPause.reset();
		imgSelector.reset();
		imgEnemyBoss.reset();
	}
	//-----------------------------------------------------------------------------
	//���s����
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ����鏈��
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		auto  inp = ge->in1->GetState();

		TaskFlag rtv = TaskFlag::Game;//��肠�������݂̃^�X�N���w��

		//�|�[�Y����̍X�V
		Pause_UpDate(pauseSelector,rtv);

		if (Pause == false)
		{
			SearchDanger = false;

			//�v���C���s��
			Player_UpDate(player);

			//�v���C���̒e�̍s��
			for (int s = 0; s < SHOTS_MAX; ++s) {
				Shot_UpDate(shots[s]);
			}

			//�X���C���̍s��
			Slime_UpDate(slime);
			SlimeLevel_UpDate();

			//�X���C���V���b�g�̍s��
			SlimeShot_UpDate(slimeShot);

			//�G�̍s��
			LoadStageData();
			timeCnt++;
			for (int e = 0; e < ENEMYS_MAX; ++e) {
				Enemy_UpDate(enemys[e]);
			}

			//�{�X�̍s��
			EnemyBoss_UpDate(boss);

			//�G�e�̍s��
			for (int es = 0; es < ENEMYSHOTS_MAX; ++es) {
				EnemyShot_UpDate(enemyshots[es]);
			}

			//�A�C�e���̍s��
			for (int str = 0; str < STARS_MAX; str++) {
				Star_UpDate(stars[str]);
			}


			//�����G�t�F�N�g�̍X�V
			for (int be = 0; be < BOMBS_MAX; ++be) {
				Bomb_UpDate(bombs[be]);
			}

			//�w�i�̈ړ�
			//��
			for (int b = 0; b < 6; b++) {
				Back_UpDate(backSky[b], 5);
			}
			//�C
			for (int b = 0; b < 6; b++) {
				Back_UpDate(backSea[b], 5);
			}
			//�_
			for (int b = 0; b < 2; b++) {
				Back_UpDate(backCloud[b], 1);
			}
			//��
			for (int b = 0; b < 2; b++) {
				Back_UpDate(backGround[b], 1);
			}
			//�X�R�A�w�i
			for (int b = 0; b < 2; b++) {
				Back_UpDate(scoreback[b], 1);
			}


			//�v���C���[���X���C����HP��0�ɂȂ�����Q�[���I�[�o�[�^�X�N��
			if (player.state == State::Non || slime.state == State::Non) {
				gameOverCnt++;
				stageClear = false;
				if (gameOverCnt >= 100)
					rtv = TaskFlag::GameOver;//�Q�[���I�[�o�[��
			}
			//�N���A����Ȃ烊�U���g��ʂ�
			if (stageClear == true) {
				rtv = TaskFlag::Result;	//���̃^�X�N��
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

		//�w�i�̕\������
		//��
		for (int b = 0; b < 6; b++) {
			Back_Draw(backSky[b]);
		}
		//�_
		for (int b = 0; b < 2; b++) {
			Back_Draw(backCloud[b]);
		}
		for (int b = 0; b < 2; b++) {
			Back_Draw(backGround[b]);
		}
		//�C
		for (int b = 0; b < 6; b++) {
			Back_Draw(backSea[b]);
		}

		//�댯���b�Z�[�W�̕\��
		DangerMessage_Draw();

		//�X���C���̕\��
		Slime_Draw(slime);

		//�v���C���̕\��
		Player_Draw(player);

		//�X���C���V���b�g�̕\��
		SlimeShot_Draw(slimeShot);

		//�v���C���̒e�̕\��
		for (int c = 0; c < SHOTS_MAX; ++c) {
			Shot_Draw(shots[c]);
		}

		//�{�X�̕\��
		EnemyBoss_Draw(boss);

		//�G�̕\������
		for (int e = 0; e < ENEMYS_MAX; ++e) {
			Enemy_Draw(enemys[e]);
			HPGauge_Draw(enemys[e],EnemysHPGauge[e]);
		}

		//�G�e�̕\������
		for (int es = 0; es < ENEMYSHOTS_MAX; ++es) {
			EnemyShot_Draw(enemyshots[es]);
		}

		//�A�C�e���̕\������
		for (int str = 0; str < STARS_MAX; ++str) {
			Star_Draw(stars[str]);
		}

		//�����̕\������
		for (int be = 0; be < BOMBS_MAX; ++be) {
			Bomb_Draw(bombs[be]);
		}

		//�X�R�A�w�i�̕\������
		for (int b = 0; b < 2; b++) {
			Back_Draw(scoreback[b]);
		}
		//�X�R�A�̕\������
		Score_Draw("score:" + scoreToString(score), ML::Box2D(345, 270 - 20, 480, 50),fontA);

		//�X���C����HP�Q�[�W�̕\������
		HPGauge_Draw(slime, SlimeHPGauge);
		
		//�v���C���[��HP�Q�[�W�̕\������
		HPGauge_Draw(player, PlayerHPGauge);

		//SP�Q�[�W�̕\������
		SPGauge_Draw(SPGauge);

		//�|�[�Y��ʂ̕\������
		Pause_Draw(pauseSelector);

	}
	//-----------------------------------------------------------------------------------------------------------------
	//�v���C���̏�����
	void  Player_Initialize(Chara& c_, int  x_, int  y_)
	{
		c_.state = State::Normal;
		c_.x = x_;
		c_.y = y_;
		c_.HP = player_maxHP;
		c_.maxHP = player_maxHP;
		c_.hitBase = CenterPointHitBox(64 - 18, 25 - 10);//ML::Box2D(-32, -12, 64, 25);	//�����ڂ�菭��������
		c_.mode = playerShotA_mode;
		c_.typeNum = 1;
		SP = 0;
	}
	//-----------------------------------------------------------------------------
	//�v���C���̕\��
	void  Player_Draw(Chara& c_)
	{
		if (c_.state == State::Normal) {
			ML::Box2D  draw = CenterPointHitBox(64, 25);
			draw.Offset(c_.x, c_.y);
			ML::Box2D  src(64 * (c_.typeNum - 1), 0, 64, 25);
			imgPlayer->Draw(draw, src);
		}
	}
	//-----------------------------------------------------------------------------
	//�v���C���̍s��
	void  Player_UpDate(Chara& c_)
	{
		auto  inp = ge->in1->GetState();
		//�v���C���s��
		if (c_.state == State::Normal) {
			//�ړ�
			if (inp.LStick.BL.on && c_.x > 32) { c_.x -= player_speed; }
			if (inp.LStick.BR.on && c_.x < 480 - 32) { c_.x += player_speed; }
			if (inp.LStick.BU.on && c_.y > 13) { c_.y -= player_speed; }
			if (inp.LStick.BD.on && c_.y < 250 - 13) { c_.y += player_speed; }
			//�e
			playerShotA_shotDistance = 20 - slimeLevel * 2;

			if (inp.B2.down) {//�e�̎�ނ̐؂�ւ�
				if (c_.typeNum == 1) { c_.typeNum = 2; }
				else if (c_.typeNum == 2) { c_.typeNum = 1; }
			}
			switch (c_.typeNum) {
			case 1:
				if (inp.B1.on) {
					if (shotTimeCnt[0] <= 0) {				//�e�̃N�[���^�C����0�Ȃ�
						Shot_Appear(0, c_.x + 20, c_.y);	//�e�o��
						shotTimeCnt[0] = playerShotA_shotDistance;				//�e�̊Ԋu�ݒ�
					}
				}
				break;
			case 2:
				if (inp.B1.on) {
					if (shotTimeCnt[1] <= 0) {
						Shot_Appear(1, c_.x + 20, c_.y);
						shotTimeCnt[1] = playerShotB_shotDistance;
					}
				}
				break;
			}

			//�X���C���V���b�g
			if (inp.B3.down&& slimeShot.state == State::Non && slimeLevel >= 1) {
				SlimeShot_Initialize(slimeShot,1);
			}

			for (int s = 0; s < 2; s++) {				//�N�[���^�C�����ݒ肳��Ă���ꍇ���炷
				if (shotTimeCnt[s] > 0)
					--shotTimeCnt[s];
			}
		}
	}
	//-----------------------------------------------------------------------------
	//�e�̏o������
	void  Shot_Appear(int tn_, int  x_, int  y_)
	{
		for (int s = 0; s < SHOTS_MAX; ++s) {
			if (shots[s].state == State::Non) {//�󂫗̈����������
										   //�e�𐶐�����
				Shot_Initialize(shots[s], tn_, x_, y_);
				break;
			}
		}
	}
	//-----------------------------------------------------------------------------
	//�e�̏�����
	//�^�C�v���m�F����֐�
	void  Shot_Initialize(Chara& c_, int tn_, int x_, int y_)
	{
		switch (tn_)
		{
		case 0:ShotA_Initialize(c_, x_, y_);	break;
		case 1:ShotB_Initialize(c_, x_, y_);	break;
		}
	}
	//case 0:�eA
	void  ShotA_Initialize(Chara& c_, int  x_, int  y_)
	{
		c_.state = State::Normal;
		c_.HP = playerShotA_maxHP;		//�U����
		c_.maxHP = playerShotA_maxHP;
		c_.x = x_;
		c_.y = y_;
		c_.hitBase = CenterPointHitBox(8, 8);//ML::Box2D(-4, -4, 8, 8);
		c_.typeNum = 0;
		switch (player.mode) {
		case 0:	c_.mx = 4;
			c_.my = 0;
			break;
		case 1:
			c_.mx = 1000;		//���������ʃT�C�Y���傫�����l����
			c_.my = 1000;
			for (int e = 0; e < ENEMYS_MAX; e++) {
				if (enemys[e].state == State::Normal) {
					if (enemys[e].x >= c_.x + 100) {	 //�v���C���[����E��100�ȏ㗣��Ă���
						if (enemys[e].y >= c_.y - 40 && enemys[e].y <= c_.y + 40) {//�v���C���[����㉺40�ȓ��̓G
							PlayerShotAngleSetting(c_, enemys[e]);
						}
					}
				}
			}
			if (c_.mx == 1000) {
				c_.mx = 4;
				c_.my = 0;
			}
			else {
				c_.angle = atan2(c_.my, c_.mx);
				c_.mx = cos(c_.angle) * 4;
				c_.my = sin(c_.angle) * 4;
			}
		}
		c_.sx = (float)c_.x;	//�����O�̍��W���L��
		c_.sy = (float)c_.y;

	}
	//case 1:�eB
	void  ShotB_Initialize(Chara& c_, int  x_, int  y_)
	{
		c_.state = State::Normal;
		c_.x = x_;
		c_.y = y_; 
		c_.HP = playerShotB_maxHP;
		c_.maxHP = playerShotB_maxHP;
		c_.hitBase = CenterPointHitBox(16, 16); //ML::Box2D(-4, -4, 8, 8);
		c_.typeNum = 1;
	}
	//-----------------------------------------------------------------------------
	//�e�̕\��
	//�^�C�v���m�F����֐�
	void  Shot_Draw(Chara& c_)
	{
		switch (c_.typeNum){
		case 0:ShotA_Draw(c_);	break;
		case 1:ShotB_Draw(c_);	break;
		}
	}
	//case 0:�eA
	void  ShotA_Draw(Chara& c_)
	{
		if (c_.state == State::Normal) {
			ML::Box2D  draw = CenterPointHitBox(32,32);
			draw.Offset(c_.x, c_.y);
			ML::Box2D  src(0, 96, 32, 32);
			imgShot->Draw(draw, src);
		}
	}
	//case 1:�eB
	void  ShotB_Draw(Chara& c_)
	{
		if (c_.state == State::Normal) {
			ML::Box2D  draw=c_.hitBase;
			draw.Offset(c_.x, c_.y);
			ML::Box2D  src(0, 32, 16, 16);
			imgShot->Draw(draw, src);
		}
	}
	//-----------------------------------------------------------------------------
	//�e�̍s��
	//�^�C�v���m�F����֐�
	void  Shot_UpDate(Chara& c_)
	{
		switch (c_.typeNum)
		{
		case 0:ShotA_UpDate(c_);	break;
		case 1:ShotB_UpDate(c_);	break;
		}
	}
	//case 0:�eA
	void  ShotA_UpDate(Chara& c_)
	{
		if (c_.state == State::Normal) {//�L���ɂȂ��Ă���e�̂�
			//�e�̍s������
			c_.sx += c_.mx;
			c_.sy += c_.my;
			c_.x = (int)(c_.sx + c_.mx);
			c_.y = (int)(c_.sy + c_.my);

			if (c_.x > 480 + 16 || c_.y > 250 + 16 || c_.y < 0) {
				c_.state = State::Non;
			}
			//�e�ƓG�̓����蔻��
	//		Shot_HitCheckEnemys_Std(c_);
			for (int e = 0; e < ENEMYS_MAX; e++) {
				if (enemys[e].state == State::Normal && 
					Characters_HitCheck(c_, enemys[e]) == true) {
					//�e�ƓG���������Ă�����
					c_.state = State::Non;
					enemys[e].HP -= c_.HP;
					if (enemys[e].HP <= 0) {//�G��HP��0�ɂȂ�����
						enemys[e].state = State::Non;
						enemysDefeated++;
						score += 100;
						//�G����A�C�e���o��
						Star_Appear(enemys[e].x, enemys[e].y, 250);									//�GA,B,C
						if (enemys[e].typeNum <= 1) { Star_Appear(enemys[e].x, enemys[e].y, 60); }	//�GA,B
						if (enemys[e].typeNum == 1) { Star_Appear(enemys[e].x, enemys[e].y, 0); }	//�GB
						//����
						Bomb_Appear(enemys[e].x, enemys[e].y);
					}
					break;
				}
				
			}
		}
	}
	//case 1:�eB
	void  ShotB_UpDate(Chara& c_)
	{
		if (c_.state == State::Normal) {//�L���ɂȂ��Ă���e�̂�
										//�e�̍s������
			c_.x += 2;
			c_.y += 2;
			if (c_.x > 480 + 16 || c_.y > 250 + 16) {
				c_.state = State::Non;
			}
			//�e�ƓG�̓����蔻��
			for (int e = 0; e < ENEMYS_MAX; e++) {
				if (enemys[e].state == State::Normal &&
					Characters_HitCheck(c_, enemys[e]) == true) {
					//�e�ƓG���������Ă�����
					c_.state = State::Non;
					enemys[e].HP -= c_.HP;
					if (enemys[e].HP <= 0) {
						enemys[e].state = State::Non;
						enemysDefeated++;
						score += 100;
						Star_Appear(enemys[e].x, enemys[e].y, 250);
						if (enemys[e].typeNum <= 1) { Star_Appear(enemys[e].x, enemys[e].y, 60); }
						if (enemys[e].typeNum == 1) { Star_Appear(enemys[e].x, enemys[e].y, 0); }
						Bomb_Appear(enemys[e].x, enemys[e].y);
					}
					break;
				}
			}
		}
	}
	//-----------------------------------------------------------------------------
	//�X���C���̏�����
	void  Slime_Initialize(Chara& c_, int x_, int y_)
	{
		c_.state = State::Normal;
		c_.x = x_;
		c_.y = y_;
		c_.HP = slime_maxHP;
		c_.maxHP = slime_maxHP;
		c_.hitBase = CenterPointHitBox(62, 41); //ML::Box2D(-32, -24, 64, 48);
		c_.animCnt = 0;
		slimeLevel = 0;
	}
	//-----------------------------------------------------------------------------
	//�X���C���̕`�揈��
	void  Slime_Draw(Chara& c_)
	{
		if (c_.state == State::Normal) {
			ML::Box2D  draw = c_.hitBase;
			draw.Offset(c_.x, c_.y + (c_.animCnt / 50 * 2));//(c_.animCnt / 50 * 2));
			draw.w += (c_.animCnt / 50 * 8);
			draw.h -= (c_.animCnt / 50 * 2);
			ML::Box2D  src(1, 6, 62, 41);
			imgSlime->Draw(draw, src);
		}
	}
	//-----------------------------------------------------------------------------
	//�X���C���̍s������
	void  Slime_UpDate(Chara& c_)
	{
		if (c_.state == State::Normal)
		{
			c_.animCnt++;
			if (c_.animCnt >= 100)
				c_.animCnt = 0;
		}
	}
	//-----------------------------------------------------------------------------
	//�X���C���V���b�g�̏������i�ݒ肷��mode���Ƃɕʏ����j
	void  SlimeShot_Initialize(Chara& c_, int m_)
	{
		switch (m_) {
		case 1:SlimeShotA_Initialize(c_);	break;
		case 2:SlimeShotB_Initialize(c_);	break;
		}
	}
	//mode == 1
	void  SlimeShotA_Initialize(Chara& c_)
	{
		SP = 0;
		c_.state = State::Normal;
		c_.maxHP = slimeLevel * 1;		//�U����
		c_.HP = slimeLevel * 1;
		c_.x = slime.x;
		c_.y = slime.y;
		c_.mode = 1;
		c_.moveCnt = 0;
		c_.hitBase = CenterPointHitBox(64 * (1 + slimeLevel) / 6, 64 * (1 + slimeLevel) / 6);//ML::Box2D(-32,-32,64,64);

		c_.mx = (float)(player.x - c_.x);
		c_.my = (float)(player.y - c_.y);

		c_.angle = atan2(c_.my, c_.mx);
		c_.mx = cos(c_.angle) * 4;
		c_.my = sin(c_.angle) * 4;
		c_.sx = (float)c_.x;	//�����O�̍��W���L��
		c_.sy = (float)c_.y;
		slimeLevel = 0;
	}
	//mode == 2
	void  SlimeShotB_Initialize(Chara& c_)
	{
		c_.state = State::Normal;
		c_.HP *= slimeShotB_maxHP ;		//�U����
		c_.maxHP *= slimeShotB_maxHP;
		c_.x = player.x;
		c_.y = player.y;
		c_.mode = 2;
		c_.hitBase = CenterPointHitBox(112 * (1 + c_.maxHP / slimeShotB_maxHP) / 6, 56 * (1 + c_.maxHP / slimeShotB_maxHP) / 6);//ML::Box2D(-56,-28,112,56);
	}

	//-----------------------------------------------------------------------------
	//�X���C���V���b�g�̕`��imode���Ƃɕʏ����j
	void  SlimeShot_Draw(Chara& c_)
	{
		switch (c_.mode) {
		case 1:SlimeShotA_Draw(c_);	break;
		case 2:SlimeShotB_Draw(c_); break;
		}
	}
	//mode == 1
	void  SlimeShotA_Draw(Chara& c_)
	{
		if (c_.state == State::Normal) {
			ML::Box2D  draw = c_.hitBase;
			draw.Offset(c_.x, c_.y);
			ML::Box2D  src(0, 0, 64, 64);
			imgSlimeShotA->Draw(draw, src);
		}
	}
	//mode == 2
	void  SlimeShotB_Draw(Chara& c_)
	{
		if (c_.state == State::Normal) {
			ML::Box2D  draw = c_.hitBase;
			draw.Offset(c_.x, c_.y);
			ML::Box2D  src(0, 0, 112, 56);
			imgSlimeShotB->Draw(draw, src);
		}
	}

	//-----------------------------------------------------------------------------
	//�X���C���V���b�g�̍s���imode���Ƃɕʏ����j
	void  SlimeShot_UpDate(Chara& c_)
	{
		switch (c_.mode) {
		case 1:SlimeShotA_UpDate(c_); break;
		case 2:SlimeShotB_UpDate(c_); break;
		}
	}
	//mode == 1
	void  SlimeShotA_UpDate(Chara& c_)
	{
		if (c_.state == State::Normal) {//�L���ɂȂ��Ă���e�̂�
			c_.sx += c_.mx;
			c_.sy += c_.my;

			c_.x = (int)c_.sx;
			c_.y = (int)c_.sy;

			if (c_.x > 480 + 64 || c_.x+64 < 0 || c_.y > 250 + 64 || c_.y+64 < 0) {
				c_.state = State::Non;
			}

			if (Characters_HitCheck_CtoS(player, c_) == true) {
				SlimeShot_Initialize(slimeShot, 2);
			}
			c_.moveCnt++;
			if (c_.moveCnt > 10) {
				c_.moveCnt = 0;
			}
		}
	}
	//mode == 2
	void  SlimeShotB_UpDate(Chara& c_)
	{
		if (c_.state == State::Normal) {//�L���ɂȂ��Ă���e�̂�

			c_.x += 8;

			if (c_.x > 480 + 16 || c_.x < 0 || c_.y > 250 + 16 || c_.y < 0) {
				c_.state = State::Non;
			}
		}
	}

	//-----------------------------------------------------------------------------
	//�X�e�[�W���m�F
	void LoadStageData()
	{
		switch (stage) {
		case 1:Enemy_AppearTimer1();	break;
		case 2:Enemy_AppearTimer2();	break;
		case 3:Enemy_AppearTimer3();	break;
		}
	}
	//-----------------------------------------------------------------------------
	//���Ԍo�߂��ƂɓG���o��������
	//�X�e�[�W1
	void  Enemy_AppearTimer1()
	{
		struct  sAppearData {
			int appTime;		//�o�鎞��
			int typeNumber;		//�o��G�̃^�C�v
			int y;				//�o�鍂��
		};
		sAppearData ad[] = {
		{ 100, 0, 60 },
		{ 300, 0, 150 },
		{ 400, 2, 30 },
		{ 420, 2, 80 },
		{ 440, 2, 130 },
		{ 540, 2, 200 },
		{ 560, 2, 200 },
		{ 560, 0, 30 },
		{ 580, 2, 200 },
		{ 600, 2, 200 },
		{ 620, 2, 200 },
		{ 720, 2, 30 },
		{ 760, 2, 60 },
		{ 800, 2, 90 },
		{ 840, 2, 200 },
		{ 880, 2, 150 },
		{ 1080, 0, 30 },
		{ 1080, 0, 200 },
		{ 1080, 2, 150 },
		{ 1280, 2, 30 },
		{ 1300, 2, 30 },
		{ 1320, 2, 30 },
		{ 1340, 2, 30 },
		{ 1360, 2, 30 },
		{ 1400, 2, 30 },
		{ 1500, 2, 30 },
		{ 1560, 1, 200 },
		};
		for (int i = 0; i < _countof(ad); i++) {
			if (timeCnt == ad[i].appTime) {
				Enemy_Appear(ad[i].typeNumber, 500, ad[i].y);
			}
		}
		//�X�e�[�W�N���A����
		if (timeCnt > 2000) {
			StageClearDecision();
		}
	}
	//�X�e�[�W2
	void  Enemy_AppearTimer2()
	{
		struct  sAppearData {
			int appTime;		//�o�鎞��
			int typeNumber;		//�o��G�̃^�C�v
			int y;				//�o�鍂��
		};
	
		sAppearData ad[] = {
			//wave1
			{100,0,50},
			{120,0,70},
			{200,0,200},
			{220,0,180},
			{400,2,135},
			{420,2,135},
			{440,2,135},
			{460,2,135},
			{480,2,135},
			{500,1,135},
			{520,0,50},
			{520,0,200},
			{700,1,50},
			{720,1,50},
			{740,1,50},
			//wave2
			{800,1,50},
			{800,0,70},
			{820,1,200},
			{820,0,180},
			{840,1,150},
			{840,0,135},
			{1040,2,30},
			{1060,2,30},
			{1080,2,30},
			{1100,2,30},
			{1120,2,30},
			{1140,2,30},
			{1160,2,30},
			{1180,2,30},
			{1200,2,220},
			{1220,2,220},
			{1240,2,220},
			{1260,2,180},
			{1280,1,180},
			{1300,1,180},
			//wave3
			{1400,1,50},
			{1420,0,70},
			{1440,0,90},
			{1540,1,200},
			{1560,0,180},
			{1580,0,160},
			{1600,2,135},
			{1620,2,135},
			{1640,2,135},
			{1660,2,135},
			{1680,2,135},
			{1700,1,135},
			{1720,1,115},
			{1760,1,90},
			{1780,1,70},
			{1800,1,50},
			{1820,0,50},
			{1840,0,80},
			{1860,1,100},
			{1880,0,130},
			{1900,0,150},
			{1920,1,130},
			{1940,0,180},
			{1960,0,210},
			{1980,1,160},
			{2000,0,195},
			{2020,0,165},
			{2040,1,200},
		};
		//wave���ƂɓG�o������������~�߂�
		if (DetectEnemy() == true) {
			if (timeCnt > 740 && timeCnt < 800) { timeCnt = 741; }
			if (timeCnt > 1300 && timeCnt < 1400) { timeCnt = 1301; }
		}

		for (int i = 0; i < _countof(ad); i++) {
			if (timeCnt == ad[i].appTime) {
				Enemy_Appear(ad[i].typeNumber, 500, ad[i].y);
			}
		}
		//�X�e�[�W�N���A����
		if (timeCnt > 2500) {
			StageClearDecision();
		}

	}
	void  Enemy_AppearTimer3()
	{
		struct  sAppearData {
			int appTime;		//�o�鎞��
			int typeNumber;		//�o��G�̃^�C�v
			int y;				//�o�鍂��
		};

		sAppearData ad[] = {
			{100,5,135},
		};

		for (int i = 0; i < _countof(ad); i++) {
			if (timeCnt == ad[i].appTime) {
				if (ad[i].typeNumber == 5) {
					EnemyBoss_Initialize(boss, 500, ad[i].y);
				}
				else {
					Enemy_Appear(ad[i].typeNumber, 500, ad[i].y);
				}
			}
		}
	}
	//-----------------------------------------------------------------------------
	//�G�����m
	bool  DetectEnemy()
	{
		bool EnemyIsOnScreen;
		for (int e = 0; e < ENEMYS_MAX; e++) {
			if (enemys[e].state == State::Normal) {
				EnemyIsOnScreen = true;
				break;
			}
			EnemyIsOnScreen = false;
		}
		return EnemyIsOnScreen;
	}
	
	//-----------------------------------------------------------------------------
	//�G���o��������
	void Enemy_Appear(int tn_, int x_, int y_)
	{
		for (int c = 0; c < ENEMYS_MAX; ++c) {
			if (enemys[c].state == State::Non) {
				Enemy_Initialize(enemys[c], tn_, x_, y_);
				break;
			}
		}
	}
	//-----------------------------------------------------------------------------
	//�G�̏�����

	//�^�C�v���m�F����֐�
	void  Enemy_Initialize(Chara& c_, int tn_, int x_, int y_)
	{
		switch (tn_) {
		case 0:EnemyA_Initialize(c_, x_, y_); break;
		case 1:EnemyB_Initialize(c_, x_, y_); break;
		case 2:EnemyC_Initialize(c_, x_, y_); break;
		}
	}
	//case 0:�G�^�C�vA
	void  EnemyA_Initialize(Chara& c_, int  x_, int  y_)
	{
		c_.state = State::Normal;
		c_.HP = enemyA_maxHP;
		c_.maxHP = enemyA_maxHP;
		c_.x = x_;
		c_.y = y_;
		c_.moveCnt = 0;
		c_.hitBase = CenterPointHitBox(40, 40);//ML::Box2D(-20, -20, 40, 40);//-20�͉摜�̊�𒆐S�ɂ��邽�߁A40,40�͉摜�̑傫��
		c_.animCnt = 0;
		c_.mode = 0;
		c_.typeNum = 0;
	}
	//case 1:�G�^�C�v1
	void  EnemyB_Initialize(Chara& c_, int  x_, int  y_)
	{
		c_.state = State::Normal;
		c_.HP = enemyB_maxHP;
		c_.maxHP = enemyB_maxHP;
		c_.x = x_;
		c_.y = y_;
		c_.moveCnt = 0;
		c_.hitBase = CenterPointHitBox(32, 32);//ML::Box2D(-16, -16, 32, 32);
		c_.animCnt = 0;
		c_.mode = 0;
		c_.typeNum = 1;
	}
	//case 2:�G�^�C�v2
	void  EnemyC_Initialize(Chara& c_, int x_, int y_)
	{
		c_.state = State::Normal;
		c_.HP = enemyC_maxHP;
		c_.maxHP = enemyC_maxHP;
		c_.x = x_;
		c_.y = y_;
		c_.moveCnt = 0;
		c_.hitBase = CenterPointHitBox(40, 40);//ML::Box2D(-20, -20, 40, 40);
		c_.mode = 0;
		c_.typeNum = 2;
	}

	//-----------------------------------------------------------------------------
	//�G�̕\��

	//�^�C�v���m�F����֐�
	void  Enemy_Draw(Chara& c_)
	{
		switch (c_.typeNum) {
		case 0:EnemyA_Draw(c_); break;
		case 1:EnemyB_Draw(c_); break;
		case 2:EnemyC_Draw(c_); break;
		}
	}

	//case 0:�G�^�C�vA
	void  EnemyA_Draw(Chara& c_)
	{
		if (c_.state == State::Normal) {
			ML::Box2D  draw = c_.hitBase;
			draw.Offset(c_.x, c_.y);
			int animTable[] = { 0,1,2,3,2,1 };
			ML::Box2D  src(animTable[(c_.animCnt / 5) % 6] * 40, 0, 40, 40);
			imgEnemyA->Draw(draw, src);
		}
	}
	//case 1:�G�^�C�vB
	void  EnemyB_Draw(Chara& c_)
	{
		if (c_.state == State::Normal) {
			ML::Box2D draw = c_.hitBase;
			draw.Offset(c_.x, c_.y);
			int animTable[] = { 0,1,2,3,2,1 };
			ML::Box2D src(animTable[(c_.animCnt / 5) % 6] * 32, 0, 32, 32);
			imgEnemyB->Draw(draw, src);
		}
	}
	//case 2:�G�^�C�vC
	void  EnemyC_Draw(Chara& c_)
	{
		if (c_.state == State::Normal) {
			ML::Box2D draw = c_.hitBase;
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 0, 40, 40);
			imgEnemyC->Draw(draw, src);
		}
	}
	//-----------------------------------------------------------------------------
	//�G�̍s��

	//�^�C�v���m�F����֐�
	void  Enemy_UpDate(Chara& c_)
	{
		switch (c_.typeNum) {
		case 0:EnemyA_UpDate(c_); break;
		case 1:EnemyB_UpDate(c_); break;
		case 2:EnemyC_UpDate(c_); break;
		}
	}

	//case 0:�G�^�C�vA
	void  EnemyA_UpDate(Chara& c_)
	{
		if (c_.state == State::Normal) {//�����Ă���G�̂�
			switch (c_.mode) {
			case 0:
				c_.x -= 1;
				c_.moveCnt++;
				if (c_.moveCnt > 100) {
					c_.mode = 1;
					c_.moveCnt = 0;
				}
				break;
			case 1:
				c_.moveCnt++;
				if (c_.moveCnt%100 == 1 && player.state == State::Normal)
					EnemyShot_Appear(0, c_.x, c_.y);
				if (c_.moveCnt > 200) {
					c_.mode = 2;
					c_.moveCnt = 0;
				}
				break;
			case 2:
				c_.x -= 1;
				c_.moveCnt++;
				//��ʊO�ɏo�������
				if (c_.x + 32 < 0) {
					c_.state = State::Non;
				}

				break;
			}

			//�G�Ɗe�I�u�W�F�N�g�Ƃ̓����蔻����s���i�G�����)
			//�v���C���[
			if (Characters_HitCheck(c_, player) == true) {
				c_.state = State::Non;
				player.state = State::Non;
				enemysDefeated++;
				Bomb_Appear(player.x, player.y);
			}
			
			//�X���C���V���b�gA
			if (slimeShot.mode == 1 && slimeShot.moveCnt % 2 == 0 && Characters_HitCheck_CtoS(c_, slimeShot) == true)
			{
				c_.HP -= slimeShot.HP;
				if (c_.HP <= 0) {//HP��0�ɂȂ�����
					c_.state = State::Non;
					c_.Hitting = false;
					score += 100;
					enemysDefeated++;
					Star_Appear(c_.x, c_.y, 250);
					Star_Appear(c_.x, c_.y, 60);
					Bomb_Appear(c_.x, c_.y);
				}
			}
			//�X���C���V���b�gB
			else if (c_.Hitting == false && slimeShot.mode == 2 && Characters_HitCheck(c_, slimeShot) == true) {
				c_.HP -= slimeShot.HP;
				c_.Hitting = true;
				if (c_.HP <= 0) {//HP��0�ɂȂ�����
					c_.state = State::Non;
					c_.Hitting = false;
					score += 100;
					enemysDefeated++;
					Star_Appear(c_.x, c_.y, 250);
					Star_Appear(c_.x, c_.y, 60);
					Bomb_Appear(c_.x, c_.y);
				}
			}
			if (c_.Hitting == true && Characters_HitCheck(c_, slimeShot) == false) {
				c_.Hitting = false;
			}


			//�G�̃A�j���[�V�����o�ߎ���
			c_.animCnt++;

		}
	}
	//case 1:�G�^�C�vB
	void  EnemyB_UpDate(Chara& c_)
	{
		if (c_.state == State::Normal) {//�����Ă���G�̂�
			switch (c_.mode) {
			case 0:
				c_.x -= 2;
				c_.moveCnt++;
				if (c_.moveCnt > 100) {
					c_.mode = 1;
					c_.moveCnt = 0;
				}
				break;
			case 1:
				c_.x += 1;
				c_.y -= 1;
				c_.moveCnt++;
				if (c_.moveCnt > 50) {
					c_.mode = 2;
					c_.moveCnt = 0;
				}
				break;
			case 2:
				if (c_.moveCnt == 0 && slime.state == State::Normal)
					EnemyShot_Appear(1, c_.x, c_.y);
				c_.x -= 1;
				c_.moveCnt++;
				break;
			}
			//��ʊO�ɏo�������
			if (c_.x + 32 < 0) {
				c_.state = State::Non;
			}

			//�G�Ɗe�I�u�W�F�N�g�Ƃ̓����蔻����s���i�G�����)
			//�v���C���[
			if (Characters_HitCheck(c_, player) == true) {
				c_.state = State::Non;
				player.state = State::Non;
				Bomb_Appear(player.x, player.y);
			}

			//�X���C���V���b�gA
			if (slimeShot.mode == 1 && slimeShot.moveCnt % 2 == 0 && Characters_HitCheck_CtoS(c_, slimeShot) == true)
			{
				c_.HP -= slimeShot.HP;
				if (c_.HP <= 0) {//HP��0�ɂȂ�����
					c_.state = State::Non;
					c_.Hitting = false;
					score += 100;
					enemysDefeated++;
					Star_Appear(c_.x, c_.y, 250);
					Star_Appear(c_.x, c_.y, 60);
					Star_Appear(c_.x, c_.y, 0);
					Bomb_Appear(c_.x, c_.y);
				}
			}
			//�X���C���V���b�gB
			else if (c_.Hitting == false && slimeShot.mode == 2 && Characters_HitCheck(c_, slimeShot) == true) {
				c_.HP -= slimeShot.HP;
				c_.Hitting = true;
				if (c_.HP <= 0) {
					c_.state = State::Non;
					c_.Hitting = false;
					score += 100;
					enemysDefeated++;
					Star_Appear(c_.x, c_.y, 250);
					Star_Appear(c_.x, c_.y, 60);
					Star_Appear(c_.x, c_.y, 0);
					Bomb_Appear(c_.x, c_.y);
				}
			}
			if (c_.Hitting == true && Characters_HitCheck(c_, slimeShot) == false) {
				c_.Hitting = false;
			}

			//�G�̃A�j���[�V�����o�ߎ���
			c_.animCnt++;

		}
	}
	//case 2:�G�^�C�vC
	void  EnemyC_UpDate(Chara& c_)
	{
		if (c_.state == State::Normal) {//�����Ă���G�̂�
			switch (c_.mode) {
			case 0:
				c_.x -= 2;
				c_.moveCnt++;
				if (c_.moveCnt > 100) {
					c_.mode = 1;
					c_.moveCnt = 0;
				}
				break;
			case 1:
				c_.x += 1;
				c_.moveCnt++;
				if (c_.moveCnt > 50) {
					c_.mode = 2;
					c_.moveCnt = 0;
				}
				break;
			case 2:
				c_.x -= 2;
				c_.moveCnt++;
				break;
			}
			//��ʊO�ɏo�������
			if (c_.x + 32 < 0) {
				c_.state = State::Non;
			}


			//�G�Ɗe�I�u�W�F�N�g�Ƃ̓����蔻����s���i�G�����)
			//�v���C��
			if (Characters_HitCheck(c_, player) == true) {
				c_.state = State::Non;
				player.state = State::Non;
				Bomb_Appear(player.x, player.y);
			}

			//�X���C���V���b�gA
			if (slimeShot.mode == 1 && slimeShot.moveCnt % 2 == 0 && Characters_HitCheck_CtoS(c_, slimeShot) == true)
			{
				c_.HP -= slimeShot.HP;
				if (c_.HP <= 0) {//HP��0�ɂȂ�����
					c_.state = State::Non;
					c_.Hitting = false;
					score += 100;
					enemysDefeated++;
					Star_Appear(c_.x, c_.y, 250);
					Bomb_Appear(c_.x, c_.y);
				}
			}
			//�X���C���V���b�gB
			else if (c_.Hitting == false && slimeShot.mode == 2 && Characters_HitCheck(c_, slimeShot) == true) {
				c_.HP -= slimeShot.HP;
				c_.Hitting = true;
				if (c_.HP <= 0) {
					c_.state = State::Non;
					c_.Hitting = false;
					score += 100;
					enemysDefeated++;
					Star_Appear(c_.x, c_.y, 250);
					Bomb_Appear(c_.x, c_.y);
				}
			}
			if (c_.Hitting == true && Characters_HitCheck(c_, slimeShot) == false) {
				c_.Hitting = false;
			}



			//�G�̃A�j���[�V�����o�ߎ���
			c_.animCnt++;

		}
	}
	//-----------------------------------------------------------------------------
	//�{�X�̏�����
	void  EnemyBoss_Initialize(Chara& c_, int x_, int y_)
	{
		c_.state = State::Normal;
		c_.HP = enemyBoss_maxHP;
		c_.maxHP = enemyBoss_maxHP;
		c_.x = x_;
		c_.y = y_;
		c_.moveCnt = 0;
		c_.hitBase = CenterPointHitBox(128, 128);//ML::Box2D(-20,-20,40,40);
		c_.mode = 0;
		c_.typeNum = 5;
	}
	//-----------------------------------------------------------------------------
	//�{�X�̕`��
	void  EnemyBoss_Draw(Chara& c_)
	{
		if (c_.state == State::Normal) {
			ML::Box2D draw = c_.hitBase;
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 0, 128, 128);
			imgEnemyBoss->Draw(draw, src);
		}
	}
	//-----------------------------------------------------------------------------
	void  EnemyBoss_UpDate(Chara& c_)
	{
		if (c_.state == State::Normal) {//�����Ă���ꍇ�̂�
			switch (c_.mode) {
			case 0:
				c_.x -= 1;
				c_.moveCnt++;
				if (c_.moveCnt > 100) {
					c_.mode = 1;
					c_.moveCnt = 0;
				}
				break;
			case 1:
				c_.moveCnt++;
				if (c_.moveCnt > 500) {
					c_.mode = 2;
					c_.moveCnt = 0;
				}
				break;
			case 2:
				c_.x -= 1;
				c_.moveCnt++;
				break;
			}
			//��ʊO�ɏo�������
			if (c_.x + 128 < 0) {
				c_.state = State::Non;
			}
			//�G�Ɗe�I�u�W�F�N�g�Ƃ̓����蔻����s���i�G�����)
			//�v���C��
			if (Characters_HitCheck(c_, player) == true) {
				c_.state = State::Non;
				player.state = State::Non;
				Bomb_Appear(player.x, player.y);
			}

			//�X���C���V���b�gA
			if (slimeShot.mode == 1 && slimeShot.moveCnt % 2 == 0 && Characters_HitCheck_CtoS(c_, slimeShot) == true)
			{
				c_.HP -= slimeShot.HP;
				if (c_.HP <= 0) {//HP��0�ɂȂ�����
					c_.state = State::Non;
					c_.Hitting = false;
					score += 100;
					Star_Appear(c_.x, c_.y, 250);
					Star_Appear(c_.x, c_.y, 60);
					Bomb_Appear(c_.x, c_.y);
				}
			}
			//�X���C���V���b�gB
			else if (c_.Hitting == false && slimeShot.mode == 2 && Characters_HitCheck(c_, slimeShot) == true) {
				c_.HP -= slimeShot.HP;
				c_.Hitting = true;
				if (c_.HP <= 0) {
					c_.state = State::Non;
					c_.Hitting = false;
					score += 100;
					Star_Appear(c_.x, c_.y, 250);
					Bomb_Appear(c_.x, c_.y);
				}
			}
			if (c_.Hitting == true && Characters_HitCheck(c_, slimeShot) == false) {
				c_.Hitting = false;
			}
		}
	}
	//-----------------------------------------------------------------------------
	//�G�e�̏o������
	void  EnemyShot_Appear(int tn_, int x_, int y_)
	{
		for (int es = 0; es < ENEMYSHOTS_MAX; ++es) {
			if (enemyshots[es].state == State::Non) {//�󂫗̈����������
										   //�e�𐶐�����
				EnemyShot_Initialize(enemyshots[es], tn_,  x_, y_);
				break;
			}
		}
	}

	//-----------------------------------------------------------------------------
	//�G�e�̏�����
	//�^�C�v���m�F����֐�
	void  EnemyShot_Initialize(Chara& c_, int tn_, int x_, int y_)
	{
		switch (tn_) {
		case 0:	EnemyShotA_Initialize(c_, x_, y_);	break;
		case 1: EnemyShotB_Initialize(c_, x_, y_);	break;
		}
	}
	//case 0:�G�eA
	void  EnemyShotA_Initialize(Chara& c_, int x_, int y_)
	{
		c_.state = State::Normal;
		c_.x = x_;
		c_.y = y_;
		c_.HP = enemyShotA_maxHP;
		c_.maxHP = enemyShotA_maxHP;
		c_.typeNum = 0;
		c_.hitBase = CenterPointHitBox(10, 10);//ML::Box2D(-5, -5, 10, 10);
		c_.mx = -1;
		c_.my = 0;

		if (player.x <= c_.x - 100) {
			c_.mx = (float)(player.x - c_.x);
			c_.my = (float)(player.y - c_.y);

			c_.angle = atan2(c_.my, c_.mx);
			c_.mx = cos(c_.angle)*2;
			c_.my = sin(c_.angle)*2;
		}

		c_.sx = (float)c_.x;//�����O�̍��W���L��
		c_.sy = (float)c_.y;
	}
	//case 1:�G�eB
	void  EnemyShotB_Initialize(Chara& c_, int x_, int y_)
	{
		c_.state = State::Normal;
		c_.x = x_;
		c_.y = y_;
		c_.HP = enemyShotB_maxHP;
		c_.maxHP = enemyShotB_maxHP;
		c_.typeNum = 1;
		c_.hitBase = CenterPointHitBox(26, 26);//ML::Box2D(-13, -13, 26, 26);
		c_.mx = -1;
		c_.my = 0;

		if (slime.x <= c_.x - 100) {
			c_.mx = (float)(slime.x - c_.x);
			c_.my = (float)(slime.y - c_.y);

			c_.angle = atan2(c_.my, c_.mx);
			c_.mx = cos(c_.angle);
			c_.my = sin(c_.angle);
		}

		c_.sx = (float)c_.x;//�����O�̍��W���L��
		c_.sy = (float)c_.y;
	}

	//-----------------------------------------------------------------------------
	//�G�e�̕`��
	//�^�C�v���m�F����֐�
	void  EnemyShot_Draw(Chara& c_)
	{
		switch (c_.typeNum) {
		case 0:	EnemyShotA_Draw(c_);	break;
		case 1: EnemyShotB_Draw(c_);	break;
		}
	}
	void  EnemyShotA_Draw(Chara& c_)
	{
		if (c_.state == State::Normal) {
			ML::Box2D  draw=c_.hitBase;
			draw.Offset(c_.x, c_.y);
			ML::Box2D  src(0, 196, 10, 10);
			imgShot->Draw(draw, src);
		}
	}
	void  EnemyShotB_Draw(Chara& c_)
	{
		if (c_.state == State::Normal) {
			ML::Box2D  draw = c_.hitBase;
			draw.Offset(c_.x, c_.y);
			ML::Box2D  src(3, 3, 26, 26);
			imgShot->Draw(draw, src);
		}
	}

	//-----------------------------------------------------------------------------
	//�G�e�̍s��
	//�^�C�v���m�F����֐�
	void  EnemyShot_UpDate(Chara& c_)
	{
		switch (c_.typeNum) {
		case 0:	EnemyShotA_UpDate(c_);	break;
		case 1: EnemyShotB_UpDate(c_);  break;
		}
	}
	void  EnemyShotA_UpDate(Chara& c_)
	{
		if (c_.state == State::Normal) {//�L���ɂȂ��Ă���e�̂�
	//�G�e�̍s������
			c_.sx += c_.mx;
			c_.sy += c_.my;
			c_.x = (int)c_.sx;
			c_.y = (int)c_.sy;

			if (c_.x > 480 + 16 || c_.x < 0 || c_.y > 250 + 16 || c_.y < 0) {
				c_.state = State::Non;
			}
			//�e�Ɗe�I�u�W�F�N�g�Ƃ̓����蔻��
			//�v���C���[
			if (Characters_HitCheck(c_, player) == true) {
				player.HP -= c_.HP;
				c_.state = State::Non;
				if (player.HP <= 0) {
					player.state = State::Non;
					Bomb_Appear(player.x, player.y);
				}
			}

			//�X���C���V���b�gA
			if (slimeShot.mode == 1 && slimeShot.moveCnt % 2 == 0 && Characters_HitCheck_CtoS(c_, slimeShot) == true)
			{
				c_.state = State::Non;
				score += 10;
				enemyShotsDefeated++;
				Bomb_Appear(c_.x, c_.y);

			}
			//�X���C���V���b�gB
			else if (c_.Hitting == false && slimeShot.mode == 2 && Characters_HitCheck(c_, slimeShot) == true) {
				c_.state = State::Non;
				score += 10;
				enemyShotsDefeated++;
				Bomb_Appear(c_.x, c_.y);
			}
			if (c_.Hitting == true && Characters_HitCheck(c_, slimeShot) == false) {
				c_.Hitting = false;
			}
		}
	}
	void  EnemyShotB_UpDate(Chara& c_)
	{
		if (c_.state == State::Normal) {//�L���ɂȂ��Ă���e�̂�
	//�G�e�̍s������
			c_.sx += c_.mx;
			c_.sy += c_.my;
			c_.x = (int)c_.sx;
			c_.y = (int)c_.sy;

			if (c_.x > 480 + 16 || c_.x < 0 || c_.y > 250 + 16 || c_.y < 0) {
				c_.state = State::Non;
			}
			//�e�Ɗe�I�u�W�F�N�g�Ƃ̓����蔻��
			//�X���C��
			if (Characters_HitCheck(c_, slime) == true) {
				slime.HP -= c_.HP;
				c_.state = State::Non;
				if (slime.HP <= 0) {
					slime.state = State::Non;
					Bomb_Appear(slime.x, slime.y);
				}
			}

			//�v���C���[�eB
			for (int s = 0; s < SHOTS_MAX; s++) {
				if (shots[s].state == State::Normal && shots[s].typeNum == 1 &&
					Characters_HitCheck(c_, shots[s]) == true) {

					shots[s].state = State::Non;
					c_.HP -= shots[s].HP;
					if (c_.HP <= 0) {
						c_.state = State::Non;
						score += 10;
						enemyShotsDefeated++;
						Bomb_Appear(c_.x, c_.y);

					}
					break;//�u�G�v�̃��[�v�𔲂���
				}
			}
			//�X���C���V���b�gA
			if (slimeShot.mode == 1 && slimeShot.moveCnt % 2 == 0 && Characters_HitCheck_CtoS(c_, slimeShot) == true)
			{
				c_.state = State::Non;
				score += 10;
				enemyShotsDefeated++;
				Bomb_Appear(c_.x, c_.y);

			}
			//�X���C���V���b�gB
			else if (c_.Hitting == false && slimeShot.mode == 2 && Characters_HitCheck(c_, slimeShot) == true) {
				c_.HP -= slimeShot.HP;
				c_.Hitting = true;
				if (c_.HP <= 0) {
					c_.state = State::Non;
					c_.Hitting = false;
					score += 10;
					enemyShotsDefeated++;
					Bomb_Appear(c_.x, c_.y);
				}
			}
			if (c_.Hitting == true && Characters_HitCheck(c_, slimeShot) == false) {
				c_.Hitting = false;
			}


			//�댯���b�Z�[�W��\��
			SearchDanger = true;
		}
	}

	//-----------------------------------------------------------------------------
	//�A�C�e���̏o��
	void  Star_Appear(int x_, int y_, int ang_)
	{
		for (int str = 0; str < STARS_MAX; ++str)
		{
			if (stars[str].state == State::Non) {
				Star_Initialize(stars[str], x_, y_, ang_);
				break;
			}
		}
	}
	//-----------------------------------------------------------------------------
	//�A�C�e���̏�����
	void  Star_Initialize(Chara& c_, int x_, int y_, int ang_)
	{
		c_.x = x_;
		c_.y = y_;
		c_.sx = (float)x_;
		c_.sy = (float)y_;
		c_.hitBase = CenterPointHitBox(24, 24);
		c_.moveCnt = 0;
		c_.state = State::Normal;
		c_.mx = cos(ML::ToRadian((float)ang_)) * 3;
		c_.my = sin(ML::ToRadian((float)ang_)) * 3;
	}
	//-----------------------------------------------------------------------------
	//�A�C�e���̕`��
	void  Star_Draw(Chara& c_)
	{
		if (c_.state == State::Normal) {
			ML::Box2D  draw = c_.hitBase;
			draw.Offset(c_.x, c_.y);
			ML::Box2D  src(0, 0, 24, 24);
			imgStar->Draw(draw, src);
		}
	}
	//-----------------------------------------------------------------------------
	//�A�C�e���̍s��
	void  Star_UpDate(Chara& c_)
	{
		if (c_.moveCnt < 5) {
			c_.sx += c_.mx;
			c_.sy += c_.my;
			c_.x = (int)c_.sx;
			c_.y = (int)c_.sy;
		}
		else{
			c_.x -= 1;
		}
		
		if (c_.y < 0) {
			c_.y = 0;
		}
		else if (c_.y > 250) {
			c_.y = 250;
		}

		if (c_.x + 24 < 0)
			c_.state = State::Non;

		//�v���C���[�Ƃ̓����蔻��
		if (Characters_HitCheck(c_, player) == true) {
			c_.state = State::Non;
			score += 20;
			SP += 1;
		}

		++c_.moveCnt;
	}
	//-----------------------------------------------------------------------------
	//���x���̔���
	void  SlimeLevel_UpDate()
	{
		switch (slimeLevel) {
		case 0:
			if (SP >= level_maxSP[0]) {
				SP = 0;
				slimeLevel = 1;
			}
		case 1:
			if (SP >= level_maxSP[1]) {
				SP = 0;
				slimeLevel = 2;
			}
			break;
		case 2:
			if (SP >= level_maxSP[2]) {
				SP = 0;
				slimeLevel = 3;
			}
			break;
		case 3:
			if (SP >= level_maxSP[3]) {
				SP = 0;
				slimeLevel = 4;
			}
			break;
		case 4:
			if (SP >= level_maxSP[4]) {
				SP = 0;
				slimeLevel = 5;
			}
			break;
		case 5:
			SP = 0;
			break;
		}
	}
	//-----------------------------------------------------------------------------
	//�����G�t�F�N�g�̏o��
	void  Bomb_Appear(int x_, int y_)
	{
		for (int c = 0; c < BOMBS_MAX; ++c) {
			if (bombs[c].state == State::Non) {
				Bomb_Initialize(bombs[c], x_, y_);
				break;
			}
		}
	}
	//-----------------------------------------------------------------------------
	//�����G�t�F�N�g�̏�����
	void  Bomb_Initialize(Chara& c_, int x_, int y_)
	{
		c_.x = x_;
		c_.y = y_;
		c_.animCnt = 0;
		c_.state = State::Normal;
	}
	//-----------------------------------------------------------------------------
	//�����G�t�F�N�g�̍X�V
	void  Bomb_UpDate(Chara& c_)
	{
		if (c_.state == State::Normal) {
			c_.animCnt++;
			if (c_.animCnt >= 40)
			{
				c_.state = State::Non;
			}
		}
	}
	//-----------------------------------------------------------------------------
	//�����G�t�F�N�g�̕`��
	void  Bomb_Draw(Chara& c_)
	{
		if (c_.state == State::Normal) {
			ML::Box2D draw = CenterPointHitBox(32,32);
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(32 * (c_.animCnt / 5), 0, 32, 32);
			BombImg->Draw(draw, src);
		}
	}
	//-----------------------------------------------------------------------------
	//�w�i�̏�����
	void  Back_Initialize(Back& b_, int tn_, int mv_,int spd_, ML::Box2D box_)
	{
		b_.backImage = box_;
		b_.typeNum = tn_;
		b_.moveCnt = mv_;
		b_.moveMax = mv_;
		b_.moveSpeed = spd_;
	}

	//-----------------------------------------------------------------------------
	//�w�i�̍X�V
	void  Back_UpDate(Back& b_, int bgCnt_)
	{
		if (b_.backImage.x <= -b_.backImage.w)
			b_.backImage.x = b_.backImage.w * bgCnt_;
		if (--b_.moveCnt <= 0) {
			b_.backImage.x -= b_.moveSpeed ;
			b_.moveCnt = b_.moveMax;
		}
	}

	//-----------------------------------------------------------------------------
	//�w�i�̕`��
	void  Back_Draw(Back& b_)
	{
		ML::Box2D draw = b_.backImage;
		ML::Box2D src(0, 0, b_.backImage.w, b_.backImage.h);
		switch (b_.typeNum) {
		case 0:		imgBGa->Draw(draw, src);		break;
		case 1:		imgBGb->Draw(draw, src);		break;
		case 2:		imgBGc->Draw(draw, src);		break;
		case 3:		imgBGd->Draw(draw, src);		break;
		case 10:	imgscoreBG->Draw(draw, src);	break;
		}
	}

	//-----------------------------------------------------------------------------
	//�X�R�A�̏�����
	void  Score_Initialize(string f_, int w_, int h_,DG::Font::SP&font_)
	{
		font_ = DG::Font::Create(f_,w_,h_);
	}
	//-----------------------------------------------------------------------------
	//�X�R�A�̕`��
	void  Score_Draw(string t_, ML::Box2D tbx_, DG::Font::SP f_)
	{
		f_->Draw(tbx_, t_, ML::Color(1.0f, 0.0f, 0.0f, 0.0f));
	}
	//-----------------------------------------------------------------------------
	//�X�R�A�̕�����
	string scoreToString(int s_)
	{
		return
			to_string(s_ / 100000) +
			to_string(s_ / 10000 % 10) +
			to_string(s_ / 1000 % 10) +
			to_string(s_ / 100 % 10) +
			to_string(s_ / 10 % 10) +
			to_string(s_ % 10);

	}
	//-----------------------------------------------------------------------------
	//HP�ESP�Q�[�W������
	void  Gauge_Initialize(Gauge& hp_,int tn_, ML::Box2D box_)
	{
		hp_.GaugeImage = box_;
		hp_.typeNum = tn_;
	}
	//-----------------------------------------------------------------------------
	//HP�Q�[�W�`��
	void  HPGauge_Draw(Chara c_,Gauge& ga_)
	{
		switch (ga_.typeNum) {
		case 0:SlimeHPGauge_Draw(c_, ga_); break;
		case 1:EnemyHPGauge_Draw(c_, ga_); break;
		case 2:PlayerHPGauge_Draw(c_, ga_); break;
		}
	}
	//-----------------------------------------------------------------------------
	//�X���C����HP�Q�[�W�`��
	void  SlimeHPGauge_Draw(Chara c_, Gauge& hp_)
	{
		ML::Box2D draw = hp_.GaugeImage;
		ML::Box2D src(0, 0, hp_.GaugeImage.w, hp_.GaugeImage.h);
		ML::Box2D draw2 = draw;
		draw2.w = (draw.w * c_.HP) / c_.maxHP;
		ML::Box2D src2 = src;
		src2.w += 2;
		src2.h += 2;
		imgSlimeHPBar->Draw(draw, src);
		imgSlimeHPPBar->Draw(draw2, src);
		imgSlimeHPBorder->Draw(draw, src2);


	}
	//-----------------------------------------------------------------------------
	//�G��HP�Q�[�W�`��
	void  EnemyHPGauge_Draw(Chara c_, Gauge& hp_)
	{
		if (c_.state == State::Normal && c_.maxHP != c_.HP) {
			hp_.GaugeImage.x = c_.x - 10;
			hp_.GaugeImage.y = c_.y - 30;
			ML::Box2D draw = hp_.GaugeImage;
			ML::Box2D draw2 = draw;
			draw2.w = (draw.w * c_.HP) / c_.maxHP;
			ML::Box2D src(0, 0, 40, 2);
			imgEnemyHPBar->Draw(draw, src);
			imgEnemyHPPBar->Draw(draw2, src);
		}
	}
	//-----------------------------------------------------------------------------
	//�v���C���[��HP�Q�[�W�`��
	void  PlayerHPGauge_Draw(Chara c_,Gauge& hp_)
	{
		if (c_.state == State::Normal) {
			ML::Box2D draw = hp_.GaugeImage;
			ML::Box2D draw2 = draw;
			draw2.w = (draw.w * c_.HP) / c_.maxHP;
			ML::Box2D src(0, 0, 64, 4);
			imgPlayerHPBar->Draw(draw, src);
			imgPlayerHPPBar->Draw(draw2, src);
		}
	}
	//-----------------------------------------------------------------------------
	//SP�Q�[�W�`��
	void  SPGauge_Draw(Gauge& sp_)
	{
		for (int sp = 0; sp < 5; sp++) {
			ML::Box2D draw = sp_.GaugeImage;
			draw.x += sp * 17;
			ML::Box2D src(0, 0, 16, 16);
			imgSPGauge->Draw(draw, src);
		}
		for (int sp = 0; sp < slimeLevel; sp++) {
			ML::Box2D draw = sp_.GaugeImage;
			draw.x += sp * 17;
			ML::Box2D src(32, 0, 16, 16);
			imgSPGauge->Draw(draw, src);
		}
		if (slimeLevel < 5) {
			int spWeight = 16 * SP / level_maxSP[slimeLevel];
			ML::Box2D draw = sp_.GaugeImage;
			draw.x += slimeLevel * 17;
			draw.y = 268 - spWeight;
			draw.h = spWeight;
			ML::Box2D src(16, 0, 16, 16);
			src.y = 16 - spWeight;
			src.h = spWeight;
			imgSPGauge->Draw(draw, src);
		}
	}
	//-----------------------------------------------------------------------------
	void Pause_Initialize(Chara& c_)
	{
		Pause = false;
		c_.state = State::Non;
		c_.x = 120;
		c_.y = 100;
		c_.mode = 1;
	}
	//-----------------------------------------------------------------------------
	//�|�[�Y����̍X�V
	void Pause_UpDate(Chara& c_,TaskFlag&r_)
	{
		auto inp = ge->in1->GetState();
		if	(Pause == false )
		{
			if (true == inp.ST.down) { Pause = true; }
		}
		else
		{
			if (true == inp.LStick.BU.down) { c_.mode = 1; }
			if (true == inp.LStick.BD.down) { c_.mode = 2; }
			if (true == inp.ST.down)
			{
				switch (c_.mode)
				{
				case 1:Pause = false;	break;
				case 2:r_ = TaskFlag::Title;
				}
			}
		}
		c_.y = 100 + c_.mode * 32;

	}
	//-----------------------------------------------------------------------------
	//�|�[�Y��ʂ̕\��
	void Pause_Draw(Chara& c_)
	{
		if (Pause == true) {
			ML::Box2D draw(0, 0, 480, 270);
			ML::Box2D src(0, 0, 480, 270);
			imgPause->Draw(draw, src, ML::Color(0.5f, 1.0f, 1.0f, 1.0f));

			ML::Box2D draw2(0, 0, 32, 32);
			draw2.Offset(c_.x, c_.y);
			ML::Box2D src2(0, 0, 32, 32);
			imgSelector->Draw(draw2, src2);

			Score_Draw("Pause", ML::Box2D(190, 30, 200, 100),fontB);
			Score_Draw("Continue", ML::Box2D(160, 135, 200, 100), fontA);
			Score_Draw("Back to title", ML::Box2D(160, 170, 200, 100),fontA);
		}
	}
	//-----------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	//�ŏ������߂�֐�
	int  Min(int a,int b)
	{
		return a < b ? a : b;
	}
	//-----------------------------------------------------------------------------
	//�e�̊p�x�����߂�֐�
	void  PlayerShotAngleSetting(Chara& c1_, Chara c2_) {
		int ax = c2_.x - c1_.x; //���΍��Wx
		int ay = c2_.y - c1_.y; //���΍��Wy
		if (Min((int)c1_.mx, ax) == ax) {
			if (Min((int)c1_.my, ay) == ay) {
				c1_.mx = (float)ax;
				c1_.my = (float)ay;
			}
			else {
				c1_.mx = (float)ax;
				c1_.my = (float)ay;
			}
		}
	}
	//-----------------------------------------------------------------------------
	//�����蔻��̋�`�̐ݒ�
	ML::Box2D  CenterPointHitBox(int w_, int h_)
	{
		return ML::Box2D(-w_ / 2, -h_ / 2, w_, h_);
	}
	//-----------------------------------------------------------------------------
	//�X���C���̊댯��m�点��
	void  DangerMessage_Draw()
	{
		if (SearchDanger == true) {
			ML::Box2D draw(20, 180, 32, 32);
			ML::Box2D src(0, 0, 32, 32);
			imgDangerMessage->Draw(draw, src);
		}
	}
	//�~�Ƌ�`�̓����蔻��
	bool  HitCtoS(int Lx, int Ly, int Rx, int Ry, int x, int y, int radius) {//L,R,T,B�͋�`�L�����̎l���̍��W�Ax,y,radius�͉~�`�L�����̍��W�Ɣ��a
		if (Lx - radius < x && Rx + radius > x && Ly < y && Ry > y) {//��`�̗̈攻��1
			return true;
		}
		else if (Lx < x && Rx > x && Ly - radius < y && Ry + radius > y) {//��`�̗̈攻��2
			return true;
		}
		else if ((Lx - x) * (Lx - x) + (Ly - y) * (Ly - y) < radius * radius) {//����̓����蔻��
			return true;
		}
		else if ((Rx - x) * (Rx - x) + (Ly - y) * (Ly - y) < radius * radius) {//�E��̓����蔻��
			return true;
		}
		else if ((Lx - x) * (Lx - x) + (Ry - y) * (Ry - y) < radius * radius) {//�����̓����蔻��
			return true;
		}
		else if ((Rx - x) * (Rx - x) + (Ry - y) * (Ry - y) < radius * radius) {//�E���̓����蔻��
			return true;
		}
		else {
			return false;
		}
	}

	//-----------------------------------------------------------------------------
	//��`���m�̃L�����̓����蔻��
	bool  Characters_HitCheck(Chara& c1_, Chara& c2_)
	{
		if (c1_.state == State::Normal) {
			//�����i�L����1�j�̋�`��p�Ӂi���݈ʒu�j
			ML::Box2D  me = c1_.hitBase.OffsetCopy(c1_.x, c1_.y);

			if (c2_.state == State::Normal) {//�L����2���������Ă���Ƃ��̂�
										   //�L����2�̋�`��p�Ӂi���݈ʒu�j
				ML::Box2D  you = c2_.hitBase.OffsetCopy(c2_.x, c2_.y);
				//�ڐG����
				return  you.Hit(me);//�����ƐڐG���Ă��邩����ɔ��f���Ă��炤
			}
			else
				return false;
		}
		else
			return false;
	}
	//-----------------------------------------------------------------------------
	//��`�Ɖ~�̃L�����̓����蔻��
	bool Characters_HitCheck_CtoS(Chara& cSquare_, Chara& cCircle_)
	{

		if (cCircle_.state == State::Normal) {
			//�~�L�����̓����蔻��[�k������
			ML::Box2D me(cCircle_.x, cCircle_.y, cCircle_.hitBase.w, cCircle_.hitBase.h);
			if (State::Normal == cSquare_.state) {
				//��`�L�����̓����蔻���`������
				ML::Box2D you =
					cSquare_.hitBase.OffsetCopy(cSquare_.x, cSquare_.y);
				//�ڐG����
				return HitCtoS(you.x, you.y, you.x + you.w, you.y + you.h, me.x, me.y, me.w / 2);
			}
			else
				return false;
		}
		else
			return false;
	}
	void  StageClearDecision()
	{
		stageClear = true;
		for (int e = 0; e < ENEMYS_MAX; e++) {
			if (enemys[e].state == State::Normal) {
				stageClear = false;
				break;
			}
		}
		if (stageClear == true) {
			for (int str = 0; str < STARS_MAX; str++) {
				if (stars[str].state == State::Normal) {
					stageClear = false;
					break;
				}
			}
		}
		if (stageClear == true) {
			for (int es = 0; es < ENEMYSHOTS_MAX; es++) {
				if (enemyshots[es].state == State::Normal) {
					stageClear = false;
					break;
				}
			}
		}

	}

}

