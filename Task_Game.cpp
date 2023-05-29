 #include "MyPG.h"
#include "MyGameMain.h"

//ゲーム本編
namespace Game
{
	//ゲーム情報
	DG::Image::SP  imgPlayer, imgShot, imgPlayerHPBar, imgPlayerHPPBar,												//プレイヤー
		imgSlime,imgSlimeShotA,imgSlimeShotB, imgSlimeHPBar, imgSlimeHPPBar, imgSlimeHPBorder,						//スライム
		imgEnemyA, imgEnemyB, imgEnemyC, BombImg, imgEnemyHPBar, imgEnemyHPPBar,									//敵・爆発
		imgBGa, imgBGb, imgBGc, imgBGd, imgscoreBG, imgDangerMessage, imgStar, imgSPGauge,imgPause,imgSelector,		//その他（アイテム、背景など）
		imgEnemyBoss;																								//ボス
	DG::Font::SP   fontA,fontB;



	//--------ステータス設定--------//
	int  player_speed = 3;					//プレイヤーの移動速度
	int  playerShotA_mode = 1;				//プレイヤー弾のモード（0.真っ直ぐ/1.少し照準）

	//プレイヤー弾の出る間隔（初期値・数値が小さいほど間隔が短い）
	int  playerShotA_shotDistance = 20;
	int  playerShotB_shotDistance = 40;

	//レベルごとに必要なSPの量（レベルが上がるごとにSPは0になる）
	int  level_maxSP[5] = { 5,10,20,20,30 };

	//各キャラの最大HP（初期値）
	int  player_maxHP = 50;
	int  slime_maxHP = 100;
	int  enemyA_maxHP = 30;
	int  enemyB_maxHP = 30;
	int  enemyC_maxHP = 1;
	int  enemyBoss_maxHP = 3000;

	//各弾の攻撃力
	int  playerShotA_maxHP = 10;
	int  playerShotB_maxHP = 10;
	int  slimeShotB_maxHP = 10;
	int  enemyShotA_maxHP = 10;
	int  enemyShotB_maxHP = 10;

	//画面内に表示できる弾の最大値の設定
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
		int		  moveMax;		//何フレームごとに動かすかを決める（最小は１）
		int       moveSpeed;	//背景が動く速さ
	};

	Chara  player;
	Chara  slime;
	Chara  slimeShot;					//スライムショット
	Chara  shots[SHOTS_MAX];			//ショット
	Chara  enemyshots[ENEMYSHOTS_MAX];	//敵弾
	Chara  enemys[ENEMYS_MAX];			//敵
	Chara  boss;						//ボス
	Chara  bombs[BOMBS_MAX];			//爆発エフェクト
	Chara  stars[STARS_MAX];			//アイテム
	Chara  pauseSelector;				//ポーズ画面に表示する矢印
	Back   backSky[6];					//空
	Back   backSea[6];					//海
	Back   backCloud[2];				//雲
	Back   backGround[2];				//地形
	Back   scoreback[2];				//スコア背景
	Gauge  PlayerHPGauge;				//プレイヤーのHPゲージ
	Gauge  SlimeHPGauge;				//スライムのHPゲージ
	Gauge  EnemysHPGauge[ENEMYS_MAX];	//敵のHPゲージ
	Gauge  SPGauge;						//SPゲージ（スライムポイントゲージ）
	int	   timeCnt;						//敵出現に使用
	int    shotTimeCnt[2];				//弾の出現に使用(現在2種類分)
	int	   SP;							//スキルポイント
	int    slimeLevel;					//スライムのレベル
	int	   enemysDefeated;				//倒した敵の数
	int	   enemyShotsDefeated;			//弾を壊した数(敵弾Bのみ）
	int    score;						//スコア
	int    stage = 1;					//現在のステージ
	int    gameOverCnt;					//ゲームオーバータスクへ切り替わるまでの間隔
	bool   stageClear;					//trueならリザルトタスクへ切り替わる処理が行われる
	bool   SearchDanger;				//trueなら「!」の絵が出る
	bool   Pause;						//trueならポーズ画面表示

					//関数のプロトタイプ宣言
	//プレイヤー
	void  Player_Initialize(Chara& c_, int  x_, int  y_);
	void  Player_Draw(Chara& c_);
	void  Player_UpDate(Chara& c_);

	//弾
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


	//スライム
	void  Slime_Initialize(Chara& c_, int x_, int y_);
	void  Slime_Draw(Chara& c_);
	void  Slime_UpDate(Chara& c_);

	//スライムショット
	void  SlimeShot_Initialize(Chara& c_, int m_);
	void  SlimeShotA_Initialize(Chara& c_);
	void  SlimeShotB_Initialize(Chara& c_);

	void  SlimeShot_Draw(Chara& c_);
	void  SlimeShotA_Draw(Chara& c_);
	void  SlimeShotB_Draw(Chara& c_);

	void  SlimeShot_UpDate(Chara& c_);
	void  SlimeShotA_UpDate(Chara& c_);
	void  SlimeShotB_UpDate(Chara& c_);

	//敵
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

	//敵弾
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

	//アイテム
	void  Star_Appear(int x_, int y_, int ang_);
	void  Star_Initialize(Chara& c_, int x_, int y_,int ang_);
	void  Star_Draw(Chara& c_);
	void  Star_UpDate(Chara& c_);

	//爆発
	void  Bomb_Appear(int x_, int y_);
	void  Bomb_Initialize(Chara& c_, int x_, int y_);
	void  Bomb_UpDate(Chara& c_);
	void  Bomb_Draw(Chara& c_);

	//背景
	void  Back_Initialize(Back& b_, int tn_, int mv_, int spd_, ML::Box2D box_);
	void  Back_UpDate(Back& b_,int bgCnt_);
	void  Back_Draw(Back& b_);

	//スコア文字
	void  Score_Initialize(string f_, int w_, int h_, DG::Font::SP& font_);
	void  Score_Draw(string t_, ML::Box2D tbx_, DG::Font::SP f_);

	//レベル
	void  SlimeLevel_UpDate();

	//HP・SPゲージ
	void  Gauge_Initialize(Gauge& hp_, int tn_, ML::Box2D box_);
	void  HPGauge_Draw(Chara c_, Gauge& hp_);
	void  SlimeHPGauge_Draw(Chara c_, Gauge& hp_);
	void  EnemyHPGauge_Draw(Chara c_, Gauge& hp_);
	void  PlayerHPGauge_Draw(Chara c_, Gauge& hp_);


	void  SPGauge_Draw(Gauge& sp_);

	//ポーズ画面
	void  Pause_Initialize(Chara& c_);
	void  Pause_Draw(Chara& c_);
	void  Pause_UpDate(Chara& c_, TaskFlag& r_);

	//最小を求める関数
	int  Min(int a, int b);

	//弾の角度を求める関数
	void  PlayerShotAngleSetting(Chara& c1_, Chara c2_);

	//スコアを文字化させる処理
	string scoreToString(int s_);

	//当たり判定の矩形の設定
	ML::Box2D CenterPointHitBox(int w_, int h_);

	//危険メッセージの表示
	void  DangerMessage_Draw();

	//円と矩形の当たり判定
	bool  HitCtoS(int Lx, int Ly, int Rx, int Ry, int x, int y, int radius);
		//Lx,Ly,Rx,Ryは矩形キャラの左上と右下の座標、x,y,radiusは円形キャラの座標と半径


	//キャラ同士の当たり判定
	bool  Characters_HitCheck(Chara& c1_, Chara& c2_);				//矩形と矩形
	bool  Characters_HitCheck_CtoS(Chara& cSquare_, Chara& cCircle_);//矩形と円


	//クリア判定
	void  StageClearDecision();

	//-----------------------------------------------------------------------------
	//初期化処理
	//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
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



		//ポーズ画面の初期化
		Pause_Initialize(pauseSelector);

		//プレイヤの初期化
		Player_Initialize(player, 50, 270 / 2);
		//プレイヤーHPゲージの初期化
		Gauge_Initialize(PlayerHPGauge, 2, ML::Box2D(11, 270 - 17, 148, 4));
		//倒した敵の数を0に
		enemysDefeated = 0;
		//壊した敵弾の数を0に
		enemyShotsDefeated = 0;

		//スライムの初期化
		Slime_Initialize(slime, 50, 250 - 20);
		//スライムHPゲージの初期化
		Gauge_Initialize(SlimeHPGauge, 0, ML::Box2D(10, 270 - 18, 150, 16));
		
		//弾の初期化（無効化）
		for (int s = 0; s < SHOTS_MAX; ++s) { shots[s].state = State::Non; }

		//スライムショットの初期化（無効化）
		slimeShot.state = State::Non;

		//敵の初期化
		for (int e = 0; e < ENEMYS_MAX; ++e) {
			enemys[e].state = State::Non;
			Gauge_Initialize(EnemysHPGauge[e], 1, ML::Box2D(-10,-50,20,2));
		}
		//敵出現カウンタ
		timeCnt = 0;

		//弾の初期化（無効化）
		for (int es = 0; es < ENEMYSHOTS_MAX; ++es) { enemyshots[es].state = State::Non; }

		//アイテムの初期化（無効化）
		for (int str = 0; str < STARS_MAX; str++) { stars[str].state = State::Non; }

		//爆発の初期化
		for (int be = 0; be < BOMBS_MAX; ++be) { bombs[be].state = State::Non; }

		//背景の初期化
		//空
		for (int b = 0; b < 6; b++) {
			Back_Initialize(backSky[b], 0, 10, 1, ML::Box2D(b * 112, 0, 112, 304));
		}
		//海
		for (int b = 0; b < 6; b++) {
			Back_Initialize(backSea[b], 1, 2, 1, ML::Box2D(b * 112, (250 - 96), 112, 96));
		}
		//雲
		for (int b = 0; b < 2; b++) {
			Back_Initialize(backCloud[b], 2, 10, 1, ML::Box2D(b * 544, 0, 544, 236));
		}
		//島
		for (int b = 0; b < 2; b++) {
			Back_Initialize(backGround[b], 3, 3, 1, ML::Box2D(b * 616, (250 - 96 - 110), 616, 110));
		}

		//スコア背景の初期化
		for (int b = 0; b < 2; b++) {
			Back_Initialize(scoreback[b], 10, 1, 1, ML::Box2D(b * 480, (270 - 20), 480, 20));

		}
		//スコアの初期化
		Score_Initialize("HGP創英角ﾎﾟｯﾌﾟ体", 10, 20,fontA);
		Score_Initialize("HGP創英角ﾎﾟｯﾌﾟ体", 20, 40, fontB);
		score = 0;

		//SPゲージの初期化
		for(int sp = 0;sp < 5;sp++)
		Gauge_Initialize(SPGauge, 10, ML::Box2D(200, 270 - 18, 16, 16));//Box2Dは5つあるゲージの内の一番左の座標


		//危険チェックをオフに
		SearchDanger = false;
		//クリア判定をオフに
		stageClear = false;
		//ゲームオーバーまでの間隔をリセット
		gameOverCnt = 0;

	}
	//-----------------------------------------------------------------------------
	//解放処理
	//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
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
	//実行処理
	//機能概要：ゲームの１フレームに当たる処理
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		auto  inp = ge->in1->GetState();

		TaskFlag rtv = TaskFlag::Game;//取りあえず現在のタスクを指定

		//ポーズ判定の更新
		Pause_UpDate(pauseSelector,rtv);

		if (Pause == false)
		{
			SearchDanger = false;

			//プレイヤ行動
			Player_UpDate(player);

			//プレイヤの弾の行動
			for (int s = 0; s < SHOTS_MAX; ++s) {
				Shot_UpDate(shots[s]);
			}

			//スライムの行動
			Slime_UpDate(slime);
			SlimeLevel_UpDate();

			//スライムショットの行動
			SlimeShot_UpDate(slimeShot);

			//敵の行動
			LoadStageData();
			timeCnt++;
			for (int e = 0; e < ENEMYS_MAX; ++e) {
				Enemy_UpDate(enemys[e]);
			}

			//ボスの行動
			EnemyBoss_UpDate(boss);

			//敵弾の行動
			for (int es = 0; es < ENEMYSHOTS_MAX; ++es) {
				EnemyShot_UpDate(enemyshots[es]);
			}

			//アイテムの行動
			for (int str = 0; str < STARS_MAX; str++) {
				Star_UpDate(stars[str]);
			}


			//爆発エフェクトの更新
			for (int be = 0; be < BOMBS_MAX; ++be) {
				Bomb_UpDate(bombs[be]);
			}

			//背景の移動
			//空
			for (int b = 0; b < 6; b++) {
				Back_UpDate(backSky[b], 5);
			}
			//海
			for (int b = 0; b < 6; b++) {
				Back_UpDate(backSea[b], 5);
			}
			//雲
			for (int b = 0; b < 2; b++) {
				Back_UpDate(backCloud[b], 1);
			}
			//島
			for (int b = 0; b < 2; b++) {
				Back_UpDate(backGround[b], 1);
			}
			//スコア背景
			for (int b = 0; b < 2; b++) {
				Back_UpDate(scoreback[b], 1);
			}


			//プレイヤーかスライムのHPが0になったらゲームオーバータスクへ
			if (player.state == State::Non || slime.state == State::Non) {
				gameOverCnt++;
				stageClear = false;
				if (gameOverCnt >= 100)
					rtv = TaskFlag::GameOver;//ゲームオーバーに
			}
			//クリア判定ならリザルト画面へ
			if (stageClear == true) {
				rtv = TaskFlag::Result;	//次のタスクへ
			}
		}
		return rtv;
	}
	//-----------------------------------------------------------------------------
	//描画処理
	//機能概要：ゲームの１フレームに当たる表示処理
	//-----------------------------------------------------------------------------
	void Render()
	{

		//背景の表示処理
		//空
		for (int b = 0; b < 6; b++) {
			Back_Draw(backSky[b]);
		}
		//雲
		for (int b = 0; b < 2; b++) {
			Back_Draw(backCloud[b]);
		}
		for (int b = 0; b < 2; b++) {
			Back_Draw(backGround[b]);
		}
		//海
		for (int b = 0; b < 6; b++) {
			Back_Draw(backSea[b]);
		}

		//危険メッセージの表示
		DangerMessage_Draw();

		//スライムの表示
		Slime_Draw(slime);

		//プレイヤの表示
		Player_Draw(player);

		//スライムショットの表示
		SlimeShot_Draw(slimeShot);

		//プレイヤの弾の表示
		for (int c = 0; c < SHOTS_MAX; ++c) {
			Shot_Draw(shots[c]);
		}

		//ボスの表示
		EnemyBoss_Draw(boss);

		//敵の表示処理
		for (int e = 0; e < ENEMYS_MAX; ++e) {
			Enemy_Draw(enemys[e]);
			HPGauge_Draw(enemys[e],EnemysHPGauge[e]);
		}

		//敵弾の表示処理
		for (int es = 0; es < ENEMYSHOTS_MAX; ++es) {
			EnemyShot_Draw(enemyshots[es]);
		}

		//アイテムの表示処理
		for (int str = 0; str < STARS_MAX; ++str) {
			Star_Draw(stars[str]);
		}

		//爆発の表示処理
		for (int be = 0; be < BOMBS_MAX; ++be) {
			Bomb_Draw(bombs[be]);
		}

		//スコア背景の表示処理
		for (int b = 0; b < 2; b++) {
			Back_Draw(scoreback[b]);
		}
		//スコアの表示処理
		Score_Draw("score:" + scoreToString(score), ML::Box2D(345, 270 - 20, 480, 50),fontA);

		//スライムのHPゲージの表示処理
		HPGauge_Draw(slime, SlimeHPGauge);
		
		//プレイヤーのHPゲージの表示処理
		HPGauge_Draw(player, PlayerHPGauge);

		//SPゲージの表示処理
		SPGauge_Draw(SPGauge);

		//ポーズ画面の表示処理
		Pause_Draw(pauseSelector);

	}
	//-----------------------------------------------------------------------------------------------------------------
	//プレイヤの初期化
	void  Player_Initialize(Chara& c_, int  x_, int  y_)
	{
		c_.state = State::Normal;
		c_.x = x_;
		c_.y = y_;
		c_.HP = player_maxHP;
		c_.maxHP = player_maxHP;
		c_.hitBase = CenterPointHitBox(64 - 18, 25 - 10);//ML::Box2D(-32, -12, 64, 25);	//見た目より少し小さく
		c_.mode = playerShotA_mode;
		c_.typeNum = 1;
		SP = 0;
	}
	//-----------------------------------------------------------------------------
	//プレイヤの表示
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
	//プレイヤの行動
	void  Player_UpDate(Chara& c_)
	{
		auto  inp = ge->in1->GetState();
		//プレイヤ行動
		if (c_.state == State::Normal) {
			//移動
			if (inp.LStick.BL.on && c_.x > 32) { c_.x -= player_speed; }
			if (inp.LStick.BR.on && c_.x < 480 - 32) { c_.x += player_speed; }
			if (inp.LStick.BU.on && c_.y > 13) { c_.y -= player_speed; }
			if (inp.LStick.BD.on && c_.y < 250 - 13) { c_.y += player_speed; }
			//弾
			playerShotA_shotDistance = 20 - slimeLevel * 2;

			if (inp.B2.down) {//弾の種類の切り替え
				if (c_.typeNum == 1) { c_.typeNum = 2; }
				else if (c_.typeNum == 2) { c_.typeNum = 1; }
			}
			switch (c_.typeNum) {
			case 1:
				if (inp.B1.on) {
					if (shotTimeCnt[0] <= 0) {				//弾のクールタイムが0なら
						Shot_Appear(0, c_.x + 20, c_.y);	//弾出現
						shotTimeCnt[0] = playerShotA_shotDistance;				//弾の間隔設定
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

			//スライムショット
			if (inp.B3.down&& slimeShot.state == State::Non && slimeLevel >= 1) {
				SlimeShot_Initialize(slimeShot,1);
			}

			for (int s = 0; s < 2; s++) {				//クールタイムが設定されている場合減らす
				if (shotTimeCnt[s] > 0)
					--shotTimeCnt[s];
			}
		}
	}
	//-----------------------------------------------------------------------------
	//弾の出現処理
	void  Shot_Appear(int tn_, int  x_, int  y_)
	{
		for (int s = 0; s < SHOTS_MAX; ++s) {
			if (shots[s].state == State::Non) {//空き領域を見つけたら
										   //弾を生成する
				Shot_Initialize(shots[s], tn_, x_, y_);
				break;
			}
		}
	}
	//-----------------------------------------------------------------------------
	//弾の初期化
	//タイプを確認する関数
	void  Shot_Initialize(Chara& c_, int tn_, int x_, int y_)
	{
		switch (tn_)
		{
		case 0:ShotA_Initialize(c_, x_, y_);	break;
		case 1:ShotB_Initialize(c_, x_, y_);	break;
		}
	}
	//case 0:弾A
	void  ShotA_Initialize(Chara& c_, int  x_, int  y_)
	{
		c_.state = State::Normal;
		c_.HP = playerShotA_maxHP;		//攻撃力
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
			c_.mx = 1000;		//いったん画面サイズより大きい数値を代入
			c_.my = 1000;
			for (int e = 0; e < ENEMYS_MAX; e++) {
				if (enemys[e].state == State::Normal) {
					if (enemys[e].x >= c_.x + 100) {	 //プレイヤーから右に100以上離れている
						if (enemys[e].y >= c_.y - 40 && enemys[e].y <= c_.y + 40) {//プレイヤーから上下40以内の敵
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
		c_.sx = (float)c_.x;	//動く前の座標を記憶
		c_.sy = (float)c_.y;

	}
	//case 1:弾B
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
	//弾の表示
	//タイプを確認する関数
	void  Shot_Draw(Chara& c_)
	{
		switch (c_.typeNum){
		case 0:ShotA_Draw(c_);	break;
		case 1:ShotB_Draw(c_);	break;
		}
	}
	//case 0:弾A
	void  ShotA_Draw(Chara& c_)
	{
		if (c_.state == State::Normal) {
			ML::Box2D  draw = CenterPointHitBox(32,32);
			draw.Offset(c_.x, c_.y);
			ML::Box2D  src(0, 96, 32, 32);
			imgShot->Draw(draw, src);
		}
	}
	//case 1:弾B
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
	//弾の行動
	//タイプを確認する関数
	void  Shot_UpDate(Chara& c_)
	{
		switch (c_.typeNum)
		{
		case 0:ShotA_UpDate(c_);	break;
		case 1:ShotB_UpDate(c_);	break;
		}
	}
	//case 0:弾A
	void  ShotA_UpDate(Chara& c_)
	{
		if (c_.state == State::Normal) {//有効になっている弾のみ
			//弾の行動処理
			c_.sx += c_.mx;
			c_.sy += c_.my;
			c_.x = (int)(c_.sx + c_.mx);
			c_.y = (int)(c_.sy + c_.my);

			if (c_.x > 480 + 16 || c_.y > 250 + 16 || c_.y < 0) {
				c_.state = State::Non;
			}
			//弾と敵の当たり判定
	//		Shot_HitCheckEnemys_Std(c_);
			for (int e = 0; e < ENEMYS_MAX; e++) {
				if (enemys[e].state == State::Normal && 
					Characters_HitCheck(c_, enemys[e]) == true) {
					//弾と敵が当たっていたら
					c_.state = State::Non;
					enemys[e].HP -= c_.HP;
					if (enemys[e].HP <= 0) {//敵のHPが0になったら
						enemys[e].state = State::Non;
						enemysDefeated++;
						score += 100;
						//敵からアイテム出現
						Star_Appear(enemys[e].x, enemys[e].y, 250);									//敵A,B,C
						if (enemys[e].typeNum <= 1) { Star_Appear(enemys[e].x, enemys[e].y, 60); }	//敵A,B
						if (enemys[e].typeNum == 1) { Star_Appear(enemys[e].x, enemys[e].y, 0); }	//敵B
						//爆発
						Bomb_Appear(enemys[e].x, enemys[e].y);
					}
					break;
				}
				
			}
		}
	}
	//case 1:弾B
	void  ShotB_UpDate(Chara& c_)
	{
		if (c_.state == State::Normal) {//有効になっている弾のみ
										//弾の行動処理
			c_.x += 2;
			c_.y += 2;
			if (c_.x > 480 + 16 || c_.y > 250 + 16) {
				c_.state = State::Non;
			}
			//弾と敵の当たり判定
			for (int e = 0; e < ENEMYS_MAX; e++) {
				if (enemys[e].state == State::Normal &&
					Characters_HitCheck(c_, enemys[e]) == true) {
					//弾と敵が当たっていたら
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
	//スライムの初期化
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
	//スライムの描画処理
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
	//スライムの行動処理
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
	//スライムショットの初期化（設定するmodeごとに別処理）
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
		c_.maxHP = slimeLevel * 1;		//攻撃力
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
		c_.sx = (float)c_.x;	//動く前の座標を記憶
		c_.sy = (float)c_.y;
		slimeLevel = 0;
	}
	//mode == 2
	void  SlimeShotB_Initialize(Chara& c_)
	{
		c_.state = State::Normal;
		c_.HP *= slimeShotB_maxHP ;		//攻撃力
		c_.maxHP *= slimeShotB_maxHP;
		c_.x = player.x;
		c_.y = player.y;
		c_.mode = 2;
		c_.hitBase = CenterPointHitBox(112 * (1 + c_.maxHP / slimeShotB_maxHP) / 6, 56 * (1 + c_.maxHP / slimeShotB_maxHP) / 6);//ML::Box2D(-56,-28,112,56);
	}

	//-----------------------------------------------------------------------------
	//スライムショットの描画（modeごとに別処理）
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
	//スライムショットの行動（modeごとに別処理）
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
		if (c_.state == State::Normal) {//有効になっている弾のみ
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
		if (c_.state == State::Normal) {//有効になっている弾のみ

			c_.x += 8;

			if (c_.x > 480 + 16 || c_.x < 0 || c_.y > 250 + 16 || c_.y < 0) {
				c_.state = State::Non;
			}
		}
	}

	//-----------------------------------------------------------------------------
	//ステージを確認
	void LoadStageData()
	{
		switch (stage) {
		case 1:Enemy_AppearTimer1();	break;
		case 2:Enemy_AppearTimer2();	break;
		case 3:Enemy_AppearTimer3();	break;
		}
	}
	//-----------------------------------------------------------------------------
	//時間経過ごとに敵を出現させる
	//ステージ1
	void  Enemy_AppearTimer1()
	{
		struct  sAppearData {
			int appTime;		//出る時間
			int typeNumber;		//出る敵のタイプ
			int y;				//出る高さ
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
		//ステージクリア判定
		if (timeCnt > 2000) {
			StageClearDecision();
		}
	}
	//ステージ2
	void  Enemy_AppearTimer2()
	{
		struct  sAppearData {
			int appTime;		//出る時間
			int typeNumber;		//出る敵のタイプ
			int y;				//出る高さ
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
		//waveごとに敵出現をいったん止める
		if (DetectEnemy() == true) {
			if (timeCnt > 740 && timeCnt < 800) { timeCnt = 741; }
			if (timeCnt > 1300 && timeCnt < 1400) { timeCnt = 1301; }
		}

		for (int i = 0; i < _countof(ad); i++) {
			if (timeCnt == ad[i].appTime) {
				Enemy_Appear(ad[i].typeNumber, 500, ad[i].y);
			}
		}
		//ステージクリア判定
		if (timeCnt > 2500) {
			StageClearDecision();
		}

	}
	void  Enemy_AppearTimer3()
	{
		struct  sAppearData {
			int appTime;		//出る時間
			int typeNumber;		//出る敵のタイプ
			int y;				//出る高さ
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
	//敵を感知
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
	//敵を出現させる
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
	//敵の初期化

	//タイプを確認する関数
	void  Enemy_Initialize(Chara& c_, int tn_, int x_, int y_)
	{
		switch (tn_) {
		case 0:EnemyA_Initialize(c_, x_, y_); break;
		case 1:EnemyB_Initialize(c_, x_, y_); break;
		case 2:EnemyC_Initialize(c_, x_, y_); break;
		}
	}
	//case 0:敵タイプA
	void  EnemyA_Initialize(Chara& c_, int  x_, int  y_)
	{
		c_.state = State::Normal;
		c_.HP = enemyA_maxHP;
		c_.maxHP = enemyA_maxHP;
		c_.x = x_;
		c_.y = y_;
		c_.moveCnt = 0;
		c_.hitBase = CenterPointHitBox(40, 40);//ML::Box2D(-20, -20, 40, 40);//-20は画像の基準を中心にするため、40,40は画像の大きさ
		c_.animCnt = 0;
		c_.mode = 0;
		c_.typeNum = 0;
	}
	//case 1:敵タイプ1
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
	//case 2:敵タイプ2
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
	//敵の表示

	//タイプを確認する関数
	void  Enemy_Draw(Chara& c_)
	{
		switch (c_.typeNum) {
		case 0:EnemyA_Draw(c_); break;
		case 1:EnemyB_Draw(c_); break;
		case 2:EnemyC_Draw(c_); break;
		}
	}

	//case 0:敵タイプA
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
	//case 1:敵タイプB
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
	//case 2:敵タイプC
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
	//敵の行動

	//タイプを確認する関数
	void  Enemy_UpDate(Chara& c_)
	{
		switch (c_.typeNum) {
		case 0:EnemyA_UpDate(c_); break;
		case 1:EnemyB_UpDate(c_); break;
		case 2:EnemyC_UpDate(c_); break;
		}
	}

	//case 0:敵タイプA
	void  EnemyA_UpDate(Chara& c_)
	{
		if (c_.state == State::Normal) {//生きている敵のみ
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
				//画面外に出たら消滅
				if (c_.x + 32 < 0) {
					c_.state = State::Non;
				}

				break;
			}

			//敵と各オブジェクトとの当たり判定を行う（敵が主体)
			//プレイヤー
			if (Characters_HitCheck(c_, player) == true) {
				c_.state = State::Non;
				player.state = State::Non;
				enemysDefeated++;
				Bomb_Appear(player.x, player.y);
			}
			
			//スライムショットA
			if (slimeShot.mode == 1 && slimeShot.moveCnt % 2 == 0 && Characters_HitCheck_CtoS(c_, slimeShot) == true)
			{
				c_.HP -= slimeShot.HP;
				if (c_.HP <= 0) {//HPが0になったら
					c_.state = State::Non;
					c_.Hitting = false;
					score += 100;
					enemysDefeated++;
					Star_Appear(c_.x, c_.y, 250);
					Star_Appear(c_.x, c_.y, 60);
					Bomb_Appear(c_.x, c_.y);
				}
			}
			//スライムショットB
			else if (c_.Hitting == false && slimeShot.mode == 2 && Characters_HitCheck(c_, slimeShot) == true) {
				c_.HP -= slimeShot.HP;
				c_.Hitting = true;
				if (c_.HP <= 0) {//HPが0になったら
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


			//敵のアニメーション経過時間
			c_.animCnt++;

		}
	}
	//case 1:敵タイプB
	void  EnemyB_UpDate(Chara& c_)
	{
		if (c_.state == State::Normal) {//生きている敵のみ
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
			//画面外に出たら消滅
			if (c_.x + 32 < 0) {
				c_.state = State::Non;
			}

			//敵と各オブジェクトとの当たり判定を行う（敵が主体)
			//プレイヤー
			if (Characters_HitCheck(c_, player) == true) {
				c_.state = State::Non;
				player.state = State::Non;
				Bomb_Appear(player.x, player.y);
			}

			//スライムショットA
			if (slimeShot.mode == 1 && slimeShot.moveCnt % 2 == 0 && Characters_HitCheck_CtoS(c_, slimeShot) == true)
			{
				c_.HP -= slimeShot.HP;
				if (c_.HP <= 0) {//HPが0になったら
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
			//スライムショットB
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

			//敵のアニメーション経過時間
			c_.animCnt++;

		}
	}
	//case 2:敵タイプC
	void  EnemyC_UpDate(Chara& c_)
	{
		if (c_.state == State::Normal) {//生きている敵のみ
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
			//画面外に出たら消滅
			if (c_.x + 32 < 0) {
				c_.state = State::Non;
			}


			//敵と各オブジェクトとの当たり判定を行う（敵が主体)
			//プレイヤ
			if (Characters_HitCheck(c_, player) == true) {
				c_.state = State::Non;
				player.state = State::Non;
				Bomb_Appear(player.x, player.y);
			}

			//スライムショットA
			if (slimeShot.mode == 1 && slimeShot.moveCnt % 2 == 0 && Characters_HitCheck_CtoS(c_, slimeShot) == true)
			{
				c_.HP -= slimeShot.HP;
				if (c_.HP <= 0) {//HPが0になったら
					c_.state = State::Non;
					c_.Hitting = false;
					score += 100;
					enemysDefeated++;
					Star_Appear(c_.x, c_.y, 250);
					Bomb_Appear(c_.x, c_.y);
				}
			}
			//スライムショットB
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



			//敵のアニメーション経過時間
			c_.animCnt++;

		}
	}
	//-----------------------------------------------------------------------------
	//ボスの初期化
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
	//ボスの描画
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
		if (c_.state == State::Normal) {//生きている場合のみ
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
			//画面外に出たら消滅
			if (c_.x + 128 < 0) {
				c_.state = State::Non;
			}
			//敵と各オブジェクトとの当たり判定を行う（敵が主体)
			//プレイヤ
			if (Characters_HitCheck(c_, player) == true) {
				c_.state = State::Non;
				player.state = State::Non;
				Bomb_Appear(player.x, player.y);
			}

			//スライムショットA
			if (slimeShot.mode == 1 && slimeShot.moveCnt % 2 == 0 && Characters_HitCheck_CtoS(c_, slimeShot) == true)
			{
				c_.HP -= slimeShot.HP;
				if (c_.HP <= 0) {//HPが0になったら
					c_.state = State::Non;
					c_.Hitting = false;
					score += 100;
					Star_Appear(c_.x, c_.y, 250);
					Star_Appear(c_.x, c_.y, 60);
					Bomb_Appear(c_.x, c_.y);
				}
			}
			//スライムショットB
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
	//敵弾の出現処理
	void  EnemyShot_Appear(int tn_, int x_, int y_)
	{
		for (int es = 0; es < ENEMYSHOTS_MAX; ++es) {
			if (enemyshots[es].state == State::Non) {//空き領域を見つけたら
										   //弾を生成する
				EnemyShot_Initialize(enemyshots[es], tn_,  x_, y_);
				break;
			}
		}
	}

	//-----------------------------------------------------------------------------
	//敵弾の初期化
	//タイプを確認する関数
	void  EnemyShot_Initialize(Chara& c_, int tn_, int x_, int y_)
	{
		switch (tn_) {
		case 0:	EnemyShotA_Initialize(c_, x_, y_);	break;
		case 1: EnemyShotB_Initialize(c_, x_, y_);	break;
		}
	}
	//case 0:敵弾A
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

		c_.sx = (float)c_.x;//動く前の座標を記憶
		c_.sy = (float)c_.y;
	}
	//case 1:敵弾B
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

		c_.sx = (float)c_.x;//動く前の座標を記憶
		c_.sy = (float)c_.y;
	}

	//-----------------------------------------------------------------------------
	//敵弾の描画
	//タイプを確認する関数
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
	//敵弾の行動
	//タイプを確認する関数
	void  EnemyShot_UpDate(Chara& c_)
	{
		switch (c_.typeNum) {
		case 0:	EnemyShotA_UpDate(c_);	break;
		case 1: EnemyShotB_UpDate(c_);  break;
		}
	}
	void  EnemyShotA_UpDate(Chara& c_)
	{
		if (c_.state == State::Normal) {//有効になっている弾のみ
	//敵弾の行動処理
			c_.sx += c_.mx;
			c_.sy += c_.my;
			c_.x = (int)c_.sx;
			c_.y = (int)c_.sy;

			if (c_.x > 480 + 16 || c_.x < 0 || c_.y > 250 + 16 || c_.y < 0) {
				c_.state = State::Non;
			}
			//弾と各オブジェクトとの当たり判定
			//プレイヤー
			if (Characters_HitCheck(c_, player) == true) {
				player.HP -= c_.HP;
				c_.state = State::Non;
				if (player.HP <= 0) {
					player.state = State::Non;
					Bomb_Appear(player.x, player.y);
				}
			}

			//スライムショットA
			if (slimeShot.mode == 1 && slimeShot.moveCnt % 2 == 0 && Characters_HitCheck_CtoS(c_, slimeShot) == true)
			{
				c_.state = State::Non;
				score += 10;
				enemyShotsDefeated++;
				Bomb_Appear(c_.x, c_.y);

			}
			//スライムショットB
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
		if (c_.state == State::Normal) {//有効になっている弾のみ
	//敵弾の行動処理
			c_.sx += c_.mx;
			c_.sy += c_.my;
			c_.x = (int)c_.sx;
			c_.y = (int)c_.sy;

			if (c_.x > 480 + 16 || c_.x < 0 || c_.y > 250 + 16 || c_.y < 0) {
				c_.state = State::Non;
			}
			//弾と各オブジェクトとの当たり判定
			//スライム
			if (Characters_HitCheck(c_, slime) == true) {
				slime.HP -= c_.HP;
				c_.state = State::Non;
				if (slime.HP <= 0) {
					slime.state = State::Non;
					Bomb_Appear(slime.x, slime.y);
				}
			}

			//プレイヤー弾B
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
					break;//「敵」のループを抜ける
				}
			}
			//スライムショットA
			if (slimeShot.mode == 1 && slimeShot.moveCnt % 2 == 0 && Characters_HitCheck_CtoS(c_, slimeShot) == true)
			{
				c_.state = State::Non;
				score += 10;
				enemyShotsDefeated++;
				Bomb_Appear(c_.x, c_.y);

			}
			//スライムショットB
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


			//危険メッセージを表示
			SearchDanger = true;
		}
	}

	//-----------------------------------------------------------------------------
	//アイテムの出現
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
	//アイテムの初期化
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
	//アイテムの描画
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
	//アイテムの行動
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

		//プレイヤーとの当たり判定
		if (Characters_HitCheck(c_, player) == true) {
			c_.state = State::Non;
			score += 20;
			SP += 1;
		}

		++c_.moveCnt;
	}
	//-----------------------------------------------------------------------------
	//レベルの判定
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
	//爆発エフェクトの出現
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
	//爆発エフェクトの初期化
	void  Bomb_Initialize(Chara& c_, int x_, int y_)
	{
		c_.x = x_;
		c_.y = y_;
		c_.animCnt = 0;
		c_.state = State::Normal;
	}
	//-----------------------------------------------------------------------------
	//爆発エフェクトの更新
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
	//爆発エフェクトの描画
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
	//背景の初期化
	void  Back_Initialize(Back& b_, int tn_, int mv_,int spd_, ML::Box2D box_)
	{
		b_.backImage = box_;
		b_.typeNum = tn_;
		b_.moveCnt = mv_;
		b_.moveMax = mv_;
		b_.moveSpeed = spd_;
	}

	//-----------------------------------------------------------------------------
	//背景の更新
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
	//背景の描画
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
	//スコアの初期化
	void  Score_Initialize(string f_, int w_, int h_,DG::Font::SP&font_)
	{
		font_ = DG::Font::Create(f_,w_,h_);
	}
	//-----------------------------------------------------------------------------
	//スコアの描画
	void  Score_Draw(string t_, ML::Box2D tbx_, DG::Font::SP f_)
	{
		f_->Draw(tbx_, t_, ML::Color(1.0f, 0.0f, 0.0f, 0.0f));
	}
	//-----------------------------------------------------------------------------
	//スコアの文字化
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
	//HP・SPゲージ初期化
	void  Gauge_Initialize(Gauge& hp_,int tn_, ML::Box2D box_)
	{
		hp_.GaugeImage = box_;
		hp_.typeNum = tn_;
	}
	//-----------------------------------------------------------------------------
	//HPゲージ描画
	void  HPGauge_Draw(Chara c_,Gauge& ga_)
	{
		switch (ga_.typeNum) {
		case 0:SlimeHPGauge_Draw(c_, ga_); break;
		case 1:EnemyHPGauge_Draw(c_, ga_); break;
		case 2:PlayerHPGauge_Draw(c_, ga_); break;
		}
	}
	//-----------------------------------------------------------------------------
	//スライムのHPゲージ描画
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
	//敵のHPゲージ描画
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
	//プレイヤーのHPゲージ描画
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
	//SPゲージ描画
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
	//ポーズ判定の更新
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
	//ポーズ画面の表示
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
	//最小を求める関数
	int  Min(int a,int b)
	{
		return a < b ? a : b;
	}
	//-----------------------------------------------------------------------------
	//弾の角度を求める関数
	void  PlayerShotAngleSetting(Chara& c1_, Chara c2_) {
		int ax = c2_.x - c1_.x; //相対座標x
		int ay = c2_.y - c1_.y; //相対座標y
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
	//当たり判定の矩形の設定
	ML::Box2D  CenterPointHitBox(int w_, int h_)
	{
		return ML::Box2D(-w_ / 2, -h_ / 2, w_, h_);
	}
	//-----------------------------------------------------------------------------
	//スライムの危険を知らせる
	void  DangerMessage_Draw()
	{
		if (SearchDanger == true) {
			ML::Box2D draw(20, 180, 32, 32);
			ML::Box2D src(0, 0, 32, 32);
			imgDangerMessage->Draw(draw, src);
		}
	}
	//円と矩形の当たり判定
	bool  HitCtoS(int Lx, int Ly, int Rx, int Ry, int x, int y, int radius) {//L,R,T,Bは矩形キャラの四隅の座標、x,y,radiusは円形キャラの座標と半径
		if (Lx - radius < x && Rx + radius > x && Ly < y && Ry > y) {//矩形の領域判定1
			return true;
		}
		else if (Lx < x && Rx > x && Ly - radius < y && Ry + radius > y) {//矩形の領域判定2
			return true;
		}
		else if ((Lx - x) * (Lx - x) + (Ly - y) * (Ly - y) < radius * radius) {//左上の当たり判定
			return true;
		}
		else if ((Rx - x) * (Rx - x) + (Ly - y) * (Ly - y) < radius * radius) {//右上の当たり判定
			return true;
		}
		else if ((Lx - x) * (Lx - x) + (Ry - y) * (Ry - y) < radius * radius) {//左下の当たり判定
			return true;
		}
		else if ((Rx - x) * (Rx - x) + (Ry - y) * (Ry - y) < radius * radius) {//右下の当たり判定
			return true;
		}
		else {
			return false;
		}
	}

	//-----------------------------------------------------------------------------
	//矩形同士のキャラの当たり判定
	bool  Characters_HitCheck(Chara& c1_, Chara& c2_)
	{
		if (c1_.state == State::Normal) {
			//自分（キャラ1）の矩形を用意（現在位置）
			ML::Box2D  me = c1_.hitBase.OffsetCopy(c1_.x, c1_.y);

			if (c2_.state == State::Normal) {//キャラ2が生存しているときのみ
										   //キャラ2の矩形を用意（現在位置）
				ML::Box2D  you = c2_.hitBase.OffsetCopy(c2_.x, c2_.y);
				//接触判定
				return  you.Hit(me);//自分と接触しているか相手に判断してもらう
			}
			else
				return false;
		}
		else
			return false;
	}
	//-----------------------------------------------------------------------------
	//矩形と円のキャラの当たり判定
	bool Characters_HitCheck_CtoS(Chara& cSquare_, Chara& cCircle_)
	{

		if (cCircle_.state == State::Normal) {
			//円キャラの当たり判定端渓を準備
			ML::Box2D me(cCircle_.x, cCircle_.y, cCircle_.hitBase.w, cCircle_.hitBase.h);
			if (State::Normal == cSquare_.state) {
				//矩形キャラの当たり判定矩形を準備
				ML::Box2D you =
					cSquare_.hitBase.OffsetCopy(cSquare_.x, cSquare_.y);
				//接触判定
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

