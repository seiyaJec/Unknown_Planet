#include "MyPG.h"
#include "MyGameMain.h"

namespace Game
{
				//関数のプロトタイプ宣言
	//スコア文字
	void  Score_Draw(string t_, ML::Box2D tbx_, DG::Font::SP f_);//ML::Color(a,r,g,b)のaは透明度
	extern int stage;
}

//タイトル画面
namespace Title
{
	using namespace Game;

	//ゲーム情報
	DG::Image::SP       imgSpace,imgPlanet,imgBlueBack,imgScreen,imgMenu,imgStgSelectBorder,imgStageSelector,
						imgTitle,imgSubTitle;
	DG::Font::SP		fontA,fontB;

	//動かす画像に使用
	struct TitleImage {
		State		state;
		ML::Box2D	posBase;
	};

	TitleImage	planet;				//惑星
	TitleImage  blueBack;			//半透明の水色背景
	TitleImage  screen;				//スクリーン
	TitleImage  menu;				//メニュー画面
	TitleImage  title;				//タイトル
	TitleImage  subTitle;			//サブタイトル
	TitleImage  stageSelectBorder;	//ステージを分ける枠
	TitleImage  stageSelector;		//選んでいるステージを塗る画像

	int planetPosY;		//惑星を縦に動かすための値
	int titleSizeW;		//タイトルの出現に使用
	int subTitleSizeW;	//サブタイトルの出現に使用

	int timeCnt;		//時間計測（シーンごとにリセット）
	int Scene;			//シーン
	int stageMax = 2;	//現在作られているステージ数
	int movingMenu;		//ステージ選択をスクロールするときに使用

			//関数のプロトタイプ宣言
	//シーンごとに処理を変更
	void Scene_UpDate();
	void Scene1_UpDate();
	void Scene2_UpDate();
	void Scene3_UpDate();
	void Scene4_UpDate();
	void Scene5_UpDate();
	void Scene6_UpDate();
	void Scene7_UpDate();

	//シーンをスキップする
	void SceneSkip1();
	void SceneSkip2();

	//画像の初期化
	void TitleImage_Initialize(TitleImage& t_, ML::Box2D img_);

	//背景（宇宙）の表示
	void Space_Draw();

	//惑星の表示
	void Planet_Draw(TitleImage& t_);

	//半透明の水色背景の表示
	void BlueBack_Draw(TitleImage& t_);

	//スクリーンの表示
	void Screen_Draw(TitleImage& t_);

	//メニュー画面の表示
	void Menu_Draw(TitleImage& t_);

	//ステージ分けの枠の表示
	void StageSelectBorder_Draw(TitleImage& t_);

	//選択中のステージの表示
	void StageSelector_Draw(TitleImage& t_);

	//タイトルの表示
	void Title_Draw(TitleImage& t_);

	//サブタイトルの表示
	void SubTitle_Draw(TitleImage& t_);

	//-----------------------------------------------------------------------------
	//初期化処理
	//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
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

		
		fontA = DG::Font::Create("HGP創英角ﾎﾟｯﾌﾟ体", 20, 48);
		fontB = DG::Font::Create("HGP創英角ﾎﾟｯﾌﾟ体", 5, 12);

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
	//解放処理
	//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
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
	//更新処理
	//機能概要：ゲームの１フレームに当たる処理
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		auto inp = ge->in1->GetState();

		//シーンをスキップ
		if (true == inp.ST.down) {
			if (Scene < 4) { SceneSkip1(); }
			else if (Scene > 4 && Scene < 7) { SceneSkip2(); }
		}

		//シーンごとに処理
		timeCnt++;
		Scene_UpDate();
		
		TaskFlag rtv = TaskFlag::Title;//取りあえず現在のタスクを指定
		if (Scene == 7 && timeCnt > 10) {
			if (true == inp.ST.down) {
				rtv = TaskFlag::Game;	//次のタスクをゲーム本編へ
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
		//背景（宇宙）
		Space_Draw();

		//惑星
		Planet_Draw(planet);

		//半透明の水色背景
		BlueBack_Draw(blueBack);

		//スクリーン
		Screen_Draw(screen);

		//メニュー画面
		Menu_Draw(menu);

		//ステージ分けの枠の表示
		StageSelectBorder_Draw(stageSelectBorder);

		//選択中のステージの塗りつぶし
		if (Scene == 7) { StageSelector_Draw(stageSelector); }

		//タイトル
		Title_Draw(title);

		//サブタイトル
		SubTitle_Draw(subTitle);

		//テキストメッセージ（特定のシーンで表示）
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

	//関数定義
	//-----------------------------------------------------------------------------
	//シーンのスキップ
	//シーン1～3ならシーン4へ
	void SceneSkip1()
	{
		Scene = 3;
		planet.posBase.y = 0;
		title.posBase.w = 480;
		subTitle.posBase.w = 480;
	}
	//シーン5,6の時はシーン7へ
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
	//シーンごとに処理を変更する
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

	//シーン1　惑星の移動
	void Scene1_UpDate()
	{
		//惑星の移動
		if (planet.posBase.y > 0) {
			if (timeCnt % 6 == 1) { planet.posBase.y -= 1; }

		}
		//終了したらシーン切り替え
		else {
			timeCnt = 0;
			Scene = 2;
		}
	}
	//シーン2　タイトルの出現
	void Scene2_UpDate()
	{
		//タイトルの出現
		if (title.posBase.w < 480) {
			if (timeCnt % 2 == 1) { title.posBase.w += 10; }
		}
		//終了したらシーン切り替え
		else {
			timeCnt = 0;
			Scene = 3;
		}
	}
	//シーン3　サブタイトルの表示
	void Scene3_UpDate()
	{
		//一定時間したらサブタイトル表示・シーン切り替え
		if (timeCnt > 30) {
			timeCnt = 0;
			subTitle.posBase.w = 480;
			Scene = 4;
		}
	}
	//シーン4　キー入力を待つ
	void Scene4_UpDate()
	{
		auto inp = ge->in1->GetState();
		//timeCntを0～100の間で繰り返す
		if (timeCnt >= 100) { timeCnt = 0; }
		//キー入力されたらシーン切り替え
		if (true == inp.ST.down) {
			timeCnt = 0;
			Scene = 5;
		}
	}
	//シーン5　スクリーンをおろす
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
	//シーン6　メニュー表示
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
	//シーン7　ステージ表示・選択
	void Scene7_UpDate()
	{
		auto inp = ge->in1->GetState();
		if (inp.LStick.BD.down && stage < stageMax) { stage++; }
		if (inp.LStick.BU.down && stage > 1) { stage--; }
		if (timeCnt >= 100) { timeCnt = 20;}
	}
	//-----------------------------------------------------------------------------
	//画像の初期化処理(簡易版)
	void TitleImage_Initialize(TitleImage& t_, ML::Box2D img_)
	{
		t_.state = State::Non;
		t_.posBase = img_;
	}
	//-----------------------------------------------------------------------------
	//背景（宇宙）の表示
	void Space_Draw()
	{
		ML::Box2D draw(0, 0, 480, 270);
		ML::Box2D src(0, 0, 480, 270);
		imgSpace->Draw(draw, src);
	}
	//-----------------------------------------------------------------------------
	//惑星の表示
	void Planet_Draw(TitleImage& t_)
	{
		ML::Box2D draw = t_.posBase;
		ML::Box2D src(0, 0, t_.posBase.w, t_.posBase.h);
		imgPlanet->Draw(draw, src);
	}
	//-----------------------------------------------------------------------------
	//半透明の水色背景の表示
	void BlueBack_Draw(TitleImage& t_)
	{
		ML::Box2D draw = t_.posBase;
		ML::Box2D src(0, 0, t_.posBase.w, t_.posBase.h);
		imgBlueBack->Draw(draw, src);
	}
	//-----------------------------------------------------------------------------
	//スクリーンの表示
	void Screen_Draw(TitleImage& t_)
	{
		ML::Box2D draw = t_.posBase;
		ML::Box2D src(0, 0, t_.posBase.w, t_.posBase.h);
		imgScreen->Draw(draw, src);
	}
	//-----------------------------------------------------------------------------
	//メニュー画面の表示
	void Menu_Draw(TitleImage& t_)
	{
		ML::Box2D draw = t_.posBase;
		ML::Box2D src(0, 0, t_.posBase.w, t_.posBase.h);
		imgMenu->Draw(draw, src);
	}
	//-----------------------------------------------------------------------------
	//ステージ分けの枠の表示
	void StageSelectBorder_Draw(TitleImage& t_)
	{
		ML::Box2D draw = t_.posBase;
		ML::Box2D src(0, 0, t_.posBase.w, t_.posBase.h);
		imgStgSelectBorder->Draw(draw, src);
	}
	//-----------------------------------------------------------------------------
	//選択中ステージの塗りつぶし
	void StageSelector_Draw(TitleImage& t_)
	{
		ML::Box2D draw = t_.posBase;
		draw.y += (stage - 1) * 48;
		ML::Box2D src(0, 0, t_.posBase.w, t_.posBase.h);
		imgStageSelector->Draw(draw, src);
	}
	//-----------------------------------------------------------------------------
	//タイトルの表示
	void Title_Draw(TitleImage& t_)
	{
		ML::Box2D draw = t_.posBase;
		ML::Box2D src(0, 0, t_.posBase.w, t_.posBase.h);
		imgTitle->Draw(draw, src);
	}
	//-----------------------------------------------------------------------------
	//サブタイトルの表示
	void SubTitle_Draw(TitleImage& t_)
	{
		ML::Box2D draw = t_.posBase;
		ML::Box2D src(0, 0, t_.posBase.w, t_.posBase.h);
		imgSubTitle->Draw(draw, src);
	}
}