#include "MyPG.h"
#include "MyGameMain.h"

//タイトル画面
namespace GameOver
{
	//ゲーム情報
	DG::Image::SP imgGameOver, imgSelector;	//背景
	int SelectTask;
	int timeCnt;
	//-----------------------------------------------------------------------------
	//初期化処理
	//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		imgGameOver = DG::Image::Create("./data/image/GameOver.png");
		imgSelector = DG::Image::Create("./data/image/Selector.png");
		SelectTask = 1;
		timeCnt = 0;
	}
	//-----------------------------------------------------------------------------
	//解放処理
	//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
	//-----------------------------------------------------------------------------
	void Finalize()
	{
		imgGameOver.reset();
	}
	//-----------------------------------------------------------------------------
	//更新処理
	//機能概要：ゲームの１フレームに当たる処理
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		auto inp = ge->in1->GetState();


		if (true == inp.LStick.BU.down) { SelectTask = 1; }
		if (true == inp.LStick.BD.down) { SelectTask = 2; }


		TaskFlag rtv = TaskFlag::GameOver;//取りあえず現在のタスクを指定
			if (true == inp.ST.down) {
				switch (SelectTask) {
				case 1:rtv = TaskFlag::Game;	break;	//次のタスクをゲーム本編へ
				case 2:rtv = TaskFlag::Title;	break;	//次のタスクをタイトルへ
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
		//背景の表示
		ML::Box2D drawA(0, 0, 480, 270);
		ML::Box2D srcA(0, 0, 480, 270);
		imgGameOver->Draw(drawA, srcA);
		//矢印の表示
			ML::Box2D drawB(190, 0, 64, 64);
			drawB.y = 122 + SelectTask * 35;
			ML::Box2D srcB(0, 0, 64, 64);
			imgSelector->Draw(drawB, srcB);
	}
}