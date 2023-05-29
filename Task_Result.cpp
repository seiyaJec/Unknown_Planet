#include "MyPG.h"
#include "MyGameMain.h"

namespace Game {
	extern DG::Image::SP imgPlayer, imgShot, imgEnemyA, imgEnemyB, imgEnemyC, BombImg, imgSlimeShotB,
		imgSlimeHPBar, imgSlimeHPPBar, imgSlimeHPBorder, imgPlayerHPBar, imgPlayerHPPBar;

	extern const int BOMBS_MAX = 50;
	extern Chara player;
	extern Chara slime;
	extern Chara slimeShot;
	extern Chara bombs[BOMBS_MAX];
	extern Gauge SlimeHPGauge;
	extern Gauge PlayerHPGauge;
	extern int SP;
	extern int score;
	extern int enemysDefeated;
	extern int enemyShotsDefeated;
	extern int stage;

				//�֐��̃v���g�^�C�v�錾
	//HP�Q�[�W
	void  HPGauge_Draw(Chara c_, Gauge& hp_);

	//�X�R�A����
	void  Score_Draw(string t_, ML::Box2D tbx_, DG::Font::SP f_);
	//�X�R�A�𕶎��������鏈��
	string scoreToString(int s_);


	//����
	void  Bomb_Appear(int x_, int y_);
	void  Bomb_UpDate(Chara& c_);
	void  Bomb_Draw(Chara& c_);

	//�L�������m�̓����蔻��
	bool Characters_HitCheck(Chara& c1_, Chara& c2_);


	//�����蔻��̋�`�̐ݒ�
	ML::Box2D CenterPointHitBox(int w_, int h_);

}

//�G���f�B���O
namespace Result
{
	//�Q�[�����
	using namespace Game;

	DG::Image::SP imgScoreboard;	//�w�i
	DG::Font::SP fontA,fontB,fontC; 

	int timeCnt;
	int score_enemysDefeated;		//�G��|���������v�Z
	int score_enemyShotsDefeated;	//�G�e���󂵂������v�Z�i�G�eB�̂݁j	
	Chara EnemyA;
	Chara EnemyB;
	Chara EnemyC;
	Chara ShotA;
	Chara ShotB;

	//�֐�
	void Chara_Initialize(Chara& c_, int x_, int y_, int w_, int h_);
	void Chara_Draw(Chara& c_, ML::Box2D src_, DG::Image::SP img_);
	void Scoreboard_Draw();
	void Enemy_disappear(Chara& c_);

	//-----------------------------------------------------------------------------
	//����������
	//�@�\�T�v�F�v���O�����N�����ɂP����s�����i�f�ނȂǂ̏������s���j
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		imgSlimeHPBar = DG::Image::Create("./data/image/HPBar/SlimeHPToItemBar.png");
		imgSlimeHPPBar = DG::Image::Create("./data/image/HPBar/SlimeHPProgressBar.png");
		imgSlimeHPBorder = DG::Image::Create("./data/image/HPBar/SlimeHPProgressBarBorder.png");
		imgPlayerHPBar = DG::Image::Create("./data/image/HPBar/PlayerHPToItemBar.png");
		imgPlayerHPPBar = DG::Image::Create("./data/image/HPBar/PlayerHPProgressBar.png");
		imgScoreboard = DG::Image::Create("./data/image/Scoreboard.png");
		imgPlayer = DG::Image::Create("./data/image/player.png");
		imgShot = DG::Image::Create("./data/image/Shot.png");
		imgEnemyA = DG::Image::Create("./data/image/Enemy1.png");
		imgEnemyB = DG::Image::Create("./data/image/Enemy2.png");
		imgEnemyC = DG::Image::Create("./data/image/Enemy3.png");
		BombImg = DG::Image::Create("./data/image/bomb.png");
		imgSlimeShotB = DG::Image::Create("./data/image/SlimeShotB.png");

		fontA = DG::Font::Create("HGP�n�p�p�߯�ߑ�", 20, 48);
		fontB = DG::Font::Create("HGP�n�p�p�߯�ߑ�", 10, 24);
		fontC = DG::Font::Create("HGP�n�p�p�߯�ߑ�", 5, 12);

		timeCnt = 0;

		score_enemysDefeated = 0;
		score_enemyShotsDefeated = 0;


		//�e�L�����̏�����
		Chara_Initialize(player, 0, 30, 64, 25);		//�v���C���[
		Chara_Initialize(slimeShot, 0, 120, 40, 20);	//�X���C���V���b�g
		Chara_Initialize(EnemyA, 200, 120, 40, 40);		//�GA
		Chara_Initialize(EnemyB, 220, 120, 32, 32);		//�GB
		Chara_Initialize(EnemyC, 240, 120, 40, 40);		//�GC
		Chara_Initialize(ShotA, 30, 0, 16, 16);			//�v���C���[�eB
		Chara_Initialize(ShotB, 230, 180, 26, 26);		//�G�eB
		for (int b = 0; b < BOMBS_MAX; b++) { bombs[b].state = State::Non; }	//����


		//HP�Q�[�W���Q�[���^�X�N�Ƃ͈Ⴄ�ʒu�Ɉړ�
		SlimeHPGauge.GaugeImage.x = 240 - SlimeHPGauge.GaugeImage.w / 2;
		SlimeHPGauge.GaugeImage.y = 200 - SlimeHPGauge.GaugeImage.h / 2;
		
		PlayerHPGauge.GaugeImage.x = 241 - SlimeHPGauge.GaugeImage.w / 2;
		PlayerHPGauge.GaugeImage.y = 201 - SlimeHPGauge.GaugeImage.h / 2;


	}
	//-----------------------------------------------------------------------------
	//�������
	//�@�\�T�v�F�v���O�����I�����ɂP����s�����i�f�ނȂǂ̉�����s���j
	//-----------------------------------------------------------------------------
	void Finalize()
	{
		fontA.reset();
		imgSlimeHPBar.reset();
		imgSlimeHPBorder.reset();
		imgSlimeHPPBar.reset();
		imgPlayerHPBar.reset();
		imgPlayerHPPBar.reset();
		imgEnemyA.reset();
		imgEnemyB.reset();
		imgEnemyC.reset();
		imgPlayer.reset();
		imgShot.reset();
		BombImg.reset();
		imgSlimeShotB.reset();
		imgScoreboard.reset();

	}
	//-----------------------------------------------------------------------------
	//�X�V����
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ����鏈��
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		auto inp = ge->in1->GetState();

		timeCnt++;		//���Ԍv��
		player.x++;		//�w�i�Ńv���C���[�𓮂���

		//�X�R�A�\���A�j���[�V�����iS�L�[�ŃX�L�b�v)
		if (true == inp.ST.down && timeCnt<370) 
		{//S�L�[�������ꂽ��X�L�b�v����
			timeCnt = 370;
			//�A�j���[�V�����p�̓G�Ȃǂ�����
			slimeShot.state = State::Non;
			EnemyA.state = State::Non;
			EnemyB.state = State::Non;
			EnemyC.state = State::Non;
			ShotA.state = State::Non;
			ShotB.state = State::Non;

			//�G��|������
			score_enemysDefeated = enemysDefeated;
			enemysDefeated = 0;
			//�G�e��j�󂵂���
			score_enemyShotsDefeated = enemyShotsDefeated;
			enemyShotsDefeated = 0;
			//�c��HP���X�R�A��
			score += slime.HP * 20;
			slime.HP = 0;
			score += player.HP * 10;
			player.HP = 0;
		}
		else if(timeCnt < 370)
		{//�ʏ�̏���
			//�G�ƃX���C���V���b�g�̃A�j���[�V����
			if (timeCnt >= 100 && slimeShot.x < 480 + 100) {
				slimeShot.x += 16;
				if (Characters_HitCheck(slimeShot, EnemyA)) { Enemy_disappear(EnemyA); }
				if (Characters_HitCheck(slimeShot, EnemyB)) { Enemy_disappear(EnemyB); }
				if (Characters_HitCheck(slimeShot, EnemyC)) { Enemy_disappear(EnemyC); }
				timeCnt = 101;
			}
			//�G��|�����������Z
			if (timeCnt >= 110 && enemysDefeated > 0) {
				score_enemysDefeated++;
				enemysDefeated--;
				timeCnt = 111;
			}
			//�G�e�ƃv���C���[�e�̃A�j���[�V����
			if (timeCnt >= 150 && ShotA.state == State::Normal)
			{
				ShotA.x += 3;
				ShotA.y += 3;
				if (Characters_HitCheck(ShotA, ShotB)) { Enemy_disappear(ShotB), ShotA.state = State::Non; }
			}
			//�G�e���󂵂��������Z
			if (timeCnt >= 170 && enemyShotsDefeated > 0) {
				score_enemyShotsDefeated++;
				enemyShotsDefeated--;
				timeCnt = 171;
			}
			//�X���C����HP���X�R�A�ɉ��Z
			if (timeCnt > 300 && slime.HP > 0) {
				score += 20;
				slime.HP--;
				timeCnt = 301;
			}
			//�v���C���[��HP���X�R�A�ɉ��Z
			if (timeCnt > 350 && player.HP > 0) {
				score += 10;
				player.HP--;
				timeCnt = 351;
			}
		}

		//�����̍X�V����
		for (int b = 0; b < BOMBS_MAX; b++) { Bomb_UpDate(bombs[b]); }

		TaskFlag rtv = TaskFlag::Result;//��肠�������݂̃^�X�N���w��
		if (true == inp.ST.down && timeCnt >= 371) {
			rtv = TaskFlag::Title;	//���̃^�X�N��
		}

		return rtv;
	}
	//-----------------------------------------------------------------------------
	//�`�揈��
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ�����\������
	//-----------------------------------------------------------------------------
	void Render()
	{
		Scoreboard_Draw();

		Score_Draw("   STAGE " + to_string(stage), ML::Box2D(90, 50, 480, 100), fontA);

		//���ꂼ�ꎞ�Ԃ��Ƃɕ\��
		//�GA
		if (timeCnt >= 20) { Chara_Draw(EnemyA, ML::Box2D(0, 0, 40, 40), imgEnemyA); }
		//�GB
		if (timeCnt >= 40) { Chara_Draw(EnemyB, ML::Box2D(0, 0, 32, 32), imgEnemyB); }
		//�GC
		if (timeCnt >= 60) { Chara_Draw(EnemyC, ML::Box2D(0, 0, 40, 40), imgEnemyC); }
		//�G��|������
		if (timeCnt >= 100) 
		{
			Chara_Draw(slimeShot, ML::Box2D(0, 0, 112, 56), imgSlimeShotB);
			Score_Draw(to_string(score_enemysDefeated), ML::Box2D(270, 100, 200, 200), fontA);
			Score_Draw("ENEMYS", ML::Box2D(100, 100, 200, 200), fontB);
		}
		//�G�e���󂵂���
		if (timeCnt >= 150) 
		{
			Chara_Draw(ShotA, ML::Box2D(0, 32, 16, 16), imgShot);	//�v���C���[�eB
			Chara_Draw(ShotB, ML::Box2D(3, 3, 26, 26), imgShot);	//�G�eB
			Score_Draw(to_string(score_enemyShotsDefeated), ML::Box2D(270, 145, 200, 200), fontA);
			Score_Draw("ENEMY SHOTS", ML::Box2D(100, 146, 200, 200), fontB);
		}
		//�X�R�A
		if (timeCnt >= 250) { 
			Score_Draw(scoreToString(score), ML::Box2D(170, 270 - 60, 480, 100), fontA);
			Score_Draw("SCORE", ML::Box2D(50, 270 - 60, 480, 100), fontB);
		}
		//�X���C��HP�Q�[�W
		if (timeCnt >= 300) { HPGauge_Draw(slime, SlimeHPGauge); }		//�X���C��HP�Q�[�W
		//�v���C���[HP�Q�[�W
		if (timeCnt >= 350) { HPGauge_Draw(player, PlayerHPGauge); }	//�v���C���[HP�Q�[�W
		//���b�Z�[�W
		if (timeCnt >= 400 && timeCnt % 100 >= 50)
		{
			Score_Draw("Press \"S\"", ML::Box2D(350, 230, 200, 200), fontC);
		}

		//��ɕ\��
		for (int b = 0; b < BOMBS_MAX; b++) { Bomb_Draw(bombs[b]); }
		Chara_Draw(player, ML::Box2D(0, 0, 64, 25), imgPlayer);
	}
	//-----------------------------------------------------------------------------
	//�L�����̏�����(�ȈՔ�)
	void Chara_Initialize(Chara& c_, int x_, int y_, int w_, int h_)
	{
		c_.state = State::Normal;
		c_.x = x_;
		c_.y = y_;
		c_.hitBase = CenterPointHitBox(w_, h_);
	}
	//-----------------------------------------------------------------------------
	//�L�����̕`��(�ȈՔŁj
	void Chara_Draw(Chara& c_,ML::Box2D src_, DG::Image::SP img_)
	{
		if (c_.state == State::Normal) {
			ML::Box2D draw = c_.hitBase;
			draw.Offset(c_.x, c_.y);
			ML::Box2D src = src_;
			img_->Draw(draw, src);
		}
	}
	//-----------------------------------------------------------------------------
	//�G�̏���
	void Enemy_disappear(Chara&c_)
	{
		Bomb_Appear(c_.x, c_.y);
		c_.state = State::Non;
	}
	//-----------------------------------------------------------------------------
	//�w�i�̕`��
	void Scoreboard_Draw()
	{
		ML::Box2D  draw(0, 0, 480, 270);
		ML::Box2D  src(0, 0, 480, 270);
		imgScoreboard->Draw(draw, src);
	}
}