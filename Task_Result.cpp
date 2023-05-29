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

				//関数のプロトタイプ宣言
	//HPゲージ
	void  HPGauge_Draw(Chara c_, Gauge& hp_);

	//スコア文字
	void  Score_Draw(string t_, ML::Box2D tbx_, DG::Font::SP f_);
	//スコアを文字化させる処理
	string scoreToString(int s_);


	//爆発
	void  Bomb_Appear(int x_, int y_);
	void  Bomb_UpDate(Chara& c_);
	void  Bomb_Draw(Chara& c_);

	//キャラ同士の当たり判定
	bool Characters_HitCheck(Chara& c1_, Chara& c2_);


	//当たり判定の矩形の設定
	ML::Box2D CenterPointHitBox(int w_, int h_);

}

//エンディング
namespace Result
{
	//ゲーム情報
	using namespace Game;

	DG::Image::SP imgScoreboard;	//背景
	DG::Font::SP fontA,fontB,fontC; 

	int timeCnt;
	int score_enemysDefeated;		//敵を倒した数を計算
	int score_enemyShotsDefeated;	//敵弾を壊した数を計算（敵弾Bのみ）	
	Chara EnemyA;
	Chara EnemyB;
	Chara EnemyC;
	Chara ShotA;
	Chara ShotB;

	//関数
	void Chara_Initialize(Chara& c_, int x_, int y_, int w_, int h_);
	void Chara_Draw(Chara& c_, ML::Box2D src_, DG::Image::SP img_);
	void Scoreboard_Draw();
	void Enemy_disappear(Chara& c_);

	//-----------------------------------------------------------------------------
	//初期化処理
	//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
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

		fontA = DG::Font::Create("HGP創英角ﾎﾟｯﾌﾟ体", 20, 48);
		fontB = DG::Font::Create("HGP創英角ﾎﾟｯﾌﾟ体", 10, 24);
		fontC = DG::Font::Create("HGP創英角ﾎﾟｯﾌﾟ体", 5, 12);

		timeCnt = 0;

		score_enemysDefeated = 0;
		score_enemyShotsDefeated = 0;


		//各キャラの初期化
		Chara_Initialize(player, 0, 30, 64, 25);		//プレイヤー
		Chara_Initialize(slimeShot, 0, 120, 40, 20);	//スライムショット
		Chara_Initialize(EnemyA, 200, 120, 40, 40);		//敵A
		Chara_Initialize(EnemyB, 220, 120, 32, 32);		//敵B
		Chara_Initialize(EnemyC, 240, 120, 40, 40);		//敵C
		Chara_Initialize(ShotA, 30, 0, 16, 16);			//プレイヤー弾B
		Chara_Initialize(ShotB, 230, 180, 26, 26);		//敵弾B
		for (int b = 0; b < BOMBS_MAX; b++) { bombs[b].state = State::Non; }	//爆発


		//HPゲージをゲームタスクとは違う位置に移動
		SlimeHPGauge.GaugeImage.x = 240 - SlimeHPGauge.GaugeImage.w / 2;
		SlimeHPGauge.GaugeImage.y = 200 - SlimeHPGauge.GaugeImage.h / 2;
		
		PlayerHPGauge.GaugeImage.x = 241 - SlimeHPGauge.GaugeImage.w / 2;
		PlayerHPGauge.GaugeImage.y = 201 - SlimeHPGauge.GaugeImage.h / 2;


	}
	//-----------------------------------------------------------------------------
	//解放処理
	//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
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
	//更新処理
	//機能概要：ゲームの１フレームに当たる処理
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		auto inp = ge->in1->GetState();

		timeCnt++;		//時間計測
		player.x++;		//背景でプレイヤーを動かす

		//スコア表示アニメーション（Sキーでスキップ)
		if (true == inp.ST.down && timeCnt<370) 
		{//Sキーが押されたらスキップ処理
			timeCnt = 370;
			//アニメーション用の敵などを消す
			slimeShot.state = State::Non;
			EnemyA.state = State::Non;
			EnemyB.state = State::Non;
			EnemyC.state = State::Non;
			ShotA.state = State::Non;
			ShotB.state = State::Non;

			//敵を倒した数
			score_enemysDefeated = enemysDefeated;
			enemysDefeated = 0;
			//敵弾を破壊した数
			score_enemyShotsDefeated = enemyShotsDefeated;
			enemyShotsDefeated = 0;
			//残りHPをスコア化
			score += slime.HP * 20;
			slime.HP = 0;
			score += player.HP * 10;
			player.HP = 0;
		}
		else if(timeCnt < 370)
		{//通常の処理
			//敵とスライムショットのアニメーション
			if (timeCnt >= 100 && slimeShot.x < 480 + 100) {
				slimeShot.x += 16;
				if (Characters_HitCheck(slimeShot, EnemyA)) { Enemy_disappear(EnemyA); }
				if (Characters_HitCheck(slimeShot, EnemyB)) { Enemy_disappear(EnemyB); }
				if (Characters_HitCheck(slimeShot, EnemyC)) { Enemy_disappear(EnemyC); }
				timeCnt = 101;
			}
			//敵を倒した数を加算
			if (timeCnt >= 110 && enemysDefeated > 0) {
				score_enemysDefeated++;
				enemysDefeated--;
				timeCnt = 111;
			}
			//敵弾とプレイヤー弾のアニメーション
			if (timeCnt >= 150 && ShotA.state == State::Normal)
			{
				ShotA.x += 3;
				ShotA.y += 3;
				if (Characters_HitCheck(ShotA, ShotB)) { Enemy_disappear(ShotB), ShotA.state = State::Non; }
			}
			//敵弾を壊した数を加算
			if (timeCnt >= 170 && enemyShotsDefeated > 0) {
				score_enemyShotsDefeated++;
				enemyShotsDefeated--;
				timeCnt = 171;
			}
			//スライムのHPをスコアに加算
			if (timeCnt > 300 && slime.HP > 0) {
				score += 20;
				slime.HP--;
				timeCnt = 301;
			}
			//プレイヤーのHPをスコアに加算
			if (timeCnt > 350 && player.HP > 0) {
				score += 10;
				player.HP--;
				timeCnt = 351;
			}
		}

		//爆発の更新処理
		for (int b = 0; b < BOMBS_MAX; b++) { Bomb_UpDate(bombs[b]); }

		TaskFlag rtv = TaskFlag::Result;//取りあえず現在のタスクを指定
		if (true == inp.ST.down && timeCnt >= 371) {
			rtv = TaskFlag::Title;	//次のタスクへ
		}

		return rtv;
	}
	//-----------------------------------------------------------------------------
	//描画処理
	//機能概要：ゲームの１フレームに当たる表示処理
	//-----------------------------------------------------------------------------
	void Render()
	{
		Scoreboard_Draw();

		Score_Draw("   STAGE " + to_string(stage), ML::Box2D(90, 50, 480, 100), fontA);

		//それぞれ時間ごとに表示
		//敵A
		if (timeCnt >= 20) { Chara_Draw(EnemyA, ML::Box2D(0, 0, 40, 40), imgEnemyA); }
		//敵B
		if (timeCnt >= 40) { Chara_Draw(EnemyB, ML::Box2D(0, 0, 32, 32), imgEnemyB); }
		//敵C
		if (timeCnt >= 60) { Chara_Draw(EnemyC, ML::Box2D(0, 0, 40, 40), imgEnemyC); }
		//敵を倒した数
		if (timeCnt >= 100) 
		{
			Chara_Draw(slimeShot, ML::Box2D(0, 0, 112, 56), imgSlimeShotB);
			Score_Draw(to_string(score_enemysDefeated), ML::Box2D(270, 100, 200, 200), fontA);
			Score_Draw("ENEMYS", ML::Box2D(100, 100, 200, 200), fontB);
		}
		//敵弾を壊した数
		if (timeCnt >= 150) 
		{
			Chara_Draw(ShotA, ML::Box2D(0, 32, 16, 16), imgShot);	//プレイヤー弾B
			Chara_Draw(ShotB, ML::Box2D(3, 3, 26, 26), imgShot);	//敵弾B
			Score_Draw(to_string(score_enemyShotsDefeated), ML::Box2D(270, 145, 200, 200), fontA);
			Score_Draw("ENEMY SHOTS", ML::Box2D(100, 146, 200, 200), fontB);
		}
		//スコア
		if (timeCnt >= 250) { 
			Score_Draw(scoreToString(score), ML::Box2D(170, 270 - 60, 480, 100), fontA);
			Score_Draw("SCORE", ML::Box2D(50, 270 - 60, 480, 100), fontB);
		}
		//スライムHPゲージ
		if (timeCnt >= 300) { HPGauge_Draw(slime, SlimeHPGauge); }		//スライムHPゲージ
		//プレイヤーHPゲージ
		if (timeCnt >= 350) { HPGauge_Draw(player, PlayerHPGauge); }	//プレイヤーHPゲージ
		//メッセージ
		if (timeCnt >= 400 && timeCnt % 100 >= 50)
		{
			Score_Draw("Press \"S\"", ML::Box2D(350, 230, 200, 200), fontC);
		}

		//常に表示
		for (int b = 0; b < BOMBS_MAX; b++) { Bomb_Draw(bombs[b]); }
		Chara_Draw(player, ML::Box2D(0, 0, 64, 25), imgPlayer);
	}
	//-----------------------------------------------------------------------------
	//キャラの初期化(簡易版)
	void Chara_Initialize(Chara& c_, int x_, int y_, int w_, int h_)
	{
		c_.state = State::Normal;
		c_.x = x_;
		c_.y = y_;
		c_.hitBase = CenterPointHitBox(w_, h_);
	}
	//-----------------------------------------------------------------------------
	//キャラの描画(簡易版）
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
	//敵の消滅
	void Enemy_disappear(Chara&c_)
	{
		Bomb_Appear(c_.x, c_.y);
		c_.state = State::Non;
	}
	//-----------------------------------------------------------------------------
	//背景の描画
	void Scoreboard_Draw()
	{
		ML::Box2D  draw(0, 0, 480, 270);
		ML::Box2D  src(0, 0, 480, 270);
		imgScoreboard->Draw(draw, src);
	}
}