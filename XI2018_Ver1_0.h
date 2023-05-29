#pragma warning(disable:4996)
#pragma once
#include <windows.h>
#pragma comment(lib, "xinput.lib")
#include <XInput.h> 
#define DIRECTINPUT_VERSION	0x0800
#pragma comment(lib,"dxguid")
#pragma comment(lib,"dInput8")
#include <dInput.h>
#include "myLib.h"
#include <string>
#include <vector>

//����������������������������������������������������������������������������������������������
//�����͐���n�@�\�Q							�@�@��
//����������������������������������������������������������������������������������������������
namespace XI
{
	struct  ButtonInfo {
		bool  on = false;
		bool  off = true;
		bool  down = false;
		bool  up = false;
		//�{�^����ON�ɏ���������
		void  SetOn() {   on = 1; off = 0; down = 0; up = 0; }
		//�{�^����OFF�ɏ���������
		void  SetOff() {  on = 0; off = 1; down = 0; up = 0; }
		//�{�^����DOWN�ɏ���������
		void  SetDown() { on = 1; off = 0; down = 1; up = 0; }
		//�{�^����UP�ɏ���������
		void  SetUp() {   on = 0; off = 1; down = 0; up = 1; }
	};
	struct  StickInfo {
		float                angleDYP;	//�A�i���O�X�e�B�b�N�̊p�x�i�P�ʂ̓��W�A���j�i����90�x�Ƃ���j
		float                angleUYP;	//�A�i���O�X�e�B�b�N�̊p�x�i�P�ʂ̓��W�A���j�i�オ90�x�Ƃ���j
		float                volume;	//�A�i���O�X�e�B�b�N�̋���0.0f�`1.0f
		ML::Vec2             axis;		//�����̗�-1.0f�`+1.0f
		ButtonInfo			 BL;
		ButtonInfo			 BR;
		ButtonInfo			 BU;
		ButtonInfo			 BD;
		StickInfo() {
			this->angleDYP = 0;
			this->angleUYP = 0;
			this->volume = 0;
			this->axis = ML::Vec2(0,0);
			this->BL.SetOff();
			this->BR.SetOff();
			this->BU.SetOff();
			this->BD.SetOff();
		}
	};
	struct  TriggerInfo {
		float				 volumeL;//���g���K�[�̉����ꂽ��
		float				 volumeR;//�E�g���K�[�̉����ꂽ��
		ButtonInfo			 L2;//�{�^���Ƃ��Ă݂�ꍇ
		ButtonInfo			 R2;//�{�^���Ƃ��Ă݂�ꍇ
		TriggerInfo() {
			this->volumeL = 0;
			this->volumeR = 0;
			this->L2.SetOff();
			this->R2.SetOff();
		}
	};
	//PG��Ŏg�p���鎞�̃{�^���̖��O
	enum class VGP : BYTE{
		//xbox=�` ps2=�~
		B1,
		//xbox=�a ps2=��
		B2,
		//xbox=�w ps2=��
		B3,
		//xbox=�x ps2=��
		B4,
		//xbox=LB  ps2=L1
		L1,
		//xbox=RB  ps2=R1
		R1,
		//xbox=Menue�@ps2=SELECT
		SE,
		//xbox=View�@ps2=START
		ST,
		//ps2=L3
		L3,
		//ps2=R3
		R3,
		//ps2=��
		HL,
		//ps2=��
		HR,
		//ps2=��
		HU,
		//ps2=��
		HD,
		//�g��
		S0, S1, S2, S3, S4, S5, S6, S7, S8, S9,
		KMax		//�@�{�^������
	};

	//�u�t�r�a�Q�[���R���g���[���[�̃Z�b�g�A�b�v�v�ŕ\�������{�^���ɑΉ�
	//�����A�����I�ȃQ�[���p�b�g�̃{�^���ɑΉ�����t���O
	enum class RGP : BYTE {
		B01, B02, B03, B04, B05, B06, B07, B08,
		B09, B10, B11, B12, B13, B14, B15, B16,
		HSL, HSR, HSU, HSD,		//�n�b�g�X�C�b�` 
		KMax					//�@�{�^������
	};


	struct  AnalogAxisKB {
		int           vKeyL;	//�L�[�{�[�h�̃o�[�`�����L�[
		int           vKeyR;	//�L�[�{�[�h�̃o�[�`�����L�[
		int           vKeyU;	//�L�[�{�[�h�̃o�[�`�����L�[
		int           vKeyD;	//�L�[�{�[�h�̃o�[�`�����L�[
	};
	struct  AnalogTriggerKB {
		int           vKeyL;	//�L�[�{�[�h�̃o�[�`�����L�[
		int           vKeyR;	//�L�[�{�[�h�̃o�[�`�����L�[
	};
	//�L�[�ݒ�L�[�{�[�h�p
	struct  KD_KB {
		int			vKey;	//�L�[�{�[�h�̃o�[�`�����L�[
		VGP			pKey;	//�{�^���̎�ށi�o�f��ł̖��́j
	};
	using KeyDatas_KB = vector<KD_KB>;

	//�L�[�ݒ�Q�[���p�b�h�p
	struct  KD_GP {
		RGP			gKey;	//�Q�[���p�b�h�̃{�^��
		VGP			pKey;	//�{�^���̎�ށi�o�f��ł̖��́j
	};
	using KeyDatas_GP = vector<KD_GP>;

	//��O�Ƃ��ē�������G���[���
	enum class Err : BYTE {
		NOTHING,		//�G���[�Ȃ�
		CREATE,			//�������s
		NOTDEVICE,		//�f�o�C�X���擾�I�Ȃ�
		UNNONTYPE,		//�s���ȃf�o�C�X���
		ENUMDEVICE,		//�f�o�C�X�̗񋓂Ɏ��s
		CREATEDEVICE,	//�f�o�C�X�̐������s
		DATAFORMAT,		//�f�[�^�t�H�[�}�b�g�̐ݒ莸�s
		COOPLEVEL,		//�������x���ݒ莸�s
		GPAXIS,			//
		KBPROPERTY,
	};
	//���z�R���g���[���\����
	struct  VGamePad {
		StickInfo  LStick;
		StickInfo  RStick;
		TriggerInfo Trigger;
		ButtonInfo  B1;
		ButtonInfo  B2;
		ButtonInfo  B3;
		ButtonInfo  B4;
		ButtonInfo  L1;
		ButtonInfo  R1;
		ButtonInfo  L3;
		ButtonInfo  R3;
		ButtonInfo  SE;
		ButtonInfo  ST;
		ButtonInfo  HL;
		ButtonInfo  HR;
		ButtonInfo  HU;
		ButtonInfo  HD;
		ButtonInfo  S0;
		ButtonInfo  S1;
		ButtonInfo  S2;
		ButtonInfo  S3;
		ButtonInfo  S4;
		ButtonInfo  S5;
		ButtonInfo  S6;
		ButtonInfo  S7;
		ButtonInfo  S8;
		ButtonInfo  S9;
	};
	//GamePad----------------------------------------------------------------
	class  GamePad {
	protected:
		enum  class  ButtonState : BYTE {
			Err,
			On,		//�����Ă���Ԓ�
			Off,	//�����Ă���Ԓ�
			Down,	//����������i�P�t���[���̂ݔ����j
			Up,		//����������i�P�t���[���̂ݔ����j
		};
		struct  Stick{
			ML::Vec2             axis;		//�����̗�-1.0f�`+1.0f
			ButtonState			 BL;
			ButtonState			 BR;
			ButtonState			 BU;
			ButtonState			 BD;
			Stick() {
				this->axis = ML::Vec2(0, 0);
				this->BL = ButtonState::Off;
				this->BR = ButtonState::Off;
				this->BU = ButtonState::Off;
				this->BD = ButtonState::Off;
			}
		};
		struct  Trigger{
			float				 volumeL;//���g���K�[�̉����ꂽ��
			float				 volumeR;//�E�g���K�[�̉����ꂽ��
			ButtonState			 L2;//�{�^���Ƃ��Ă݂�ꍇ
			ButtonState			 R2;//�{�^���Ƃ��Ă݂�ꍇ
			Trigger() {
				this->volumeL = 0;
				this->volumeR = 0;
				this->L2 = ButtonState::Off;
				this->R2 = ButtonState::Off;
			}
		};
	public:
		using SP = shared_ptr<GamePad>;
		using WP = weak_ptr<GamePad>;
	protected:
		SP				link;
		Stick			ls, rs;
		Trigger			tg;
	public:
		GamePad(void);
		static  SP  CreateKB(
			const AnalogAxisKB&			axisL_,
			const AnalogAxisKB&			axisR_,
			const AnalogTriggerKB&		trigger_,
			const KeyDatas_KB&			buttonKB_);
		static  SP  CreateGP(
			const DWORD&				id_,//0�`3�܂�
			const KeyDatas_GP&			buttonGP_);
		//	�X�V
		void  UpDate();
		virtual  void  B_UpDate() = 0;
		//�Q�̃f�o�C�X�𓝍�����
		//�Е��̃f�o�C�X�������ł��Ă��Ȃ��ꍇ�̑Ή�����
		static  GamePad::SP  Link(SP  a, SP  b);
		//�Q�[���p�b�h�̃{�^���̏�Ԃ��擾����
		VGamePad  GetState();
		virtual  void  SetVibrationL(float  pow_, int  frame_);
		virtual  void  SetVibrationR(float  pow_, int  frame_);
		virtual ButtonState  CheckBT(VGP  kk_) = 0;
	private:
	protected:
		SP  Link(SP  l) {
			if (this == l.get()) { return 0; }
			else { SP w = link; link = l; return link; }
		}
		//�{�^��
		ButtonState  UpdateButtonState(bool  nbs_, ButtonState  old_);
		ButtonState  IntegrationKey(ButtonState  a_, ButtonState  b_);
		ButtonInfo  ConvertKS(ButtonState  ks_);

		//�A�i���O�X�e�B�b�N
		void  SIUpdate(Stick&  si_, float  ax_, float  ay_);
		StickInfo  StickL();
		StickInfo  StickR();
		void  ConvertSI(StickInfo&  si_, Stick&  s_);

		void  TGUpdate(Trigger&  tg_, float  lt_, float  rt_);
		TriggerInfo  TriggerLR();
		void  ConvertTG(TriggerInfo&  ti_, Trigger&  t_);
	};
	//--------------------------------
	//���z�}�E�X�\����
	struct  VMouse {
		ML::Point         pos;
		ML::Point         pos_Screen;
		int			  wheel;
		ButtonInfo	  LB;
		ButtonInfo	  RB;
		ButtonInfo	  CB;
		ButtonInfo	  B1;
		ButtonInfo	  B2;
		ButtonInfo	  B3;
		ButtonInfo	  B4;
		ButtonInfo	  B5;
	};
	class Mouse {
	protected:
		enum  class  ButtonState : BYTE {
			Err,
			On,		//�����Ă���Ԓ�
			Off,	//�����Ă���Ԓ�
			Down,	//����������i�P�t���[���̂ݔ����j
			Up,		//����������i�P�t���[���̂ݔ����j
		};
		enum  class  MB : BYTE {		//�}�E�X�̃{�^����\��
			//�}�E�X�{�^���̎�� 8�܂ŔF���\
			LB, RB, CB, RESERVED1,
			RESERVED2, RESERVED3, RESERVED4, RESERVED5,
			kMax
		};
	private:
		LPDIRECTINPUTDEVICE8 mouse;
		DIMOUSESTATE2 mouseState;
		ML::Point		  pos_Screen;	//�X�N���[�����W
		ML::Point		  pos;			//�N���C�A���g���W
		long          mouseWheel;
		ButtonState      mouseButton[(int)MB::kMax];
		Err			     init(float, float);
		float			 scaleX, scaleY;
	public:
		using SP = shared_ptr<Mouse>;
		using WP = weak_ptr<Mouse>;

	private:
		ButtonState     CheckBT(MB  mb_);
		Mouse();
		ButtonInfo  ConvertKS(ButtonState  ks_);
	public:
		~Mouse();
		//����
		static SP   Create(float scaleX_ = 1.f, float scaleY_ = 1.f);
		//	�X�V
		void UpDate();
		//
		VMouse  GetState();
	};
	//---------------------------------------------------------------------------
	//XI�I�u�W�F�N�g
	class  Obj
	{
	public:
		using WP = weak_ptr<Obj>;
		using SP = shared_ptr<Obj>;
	private:
		static  Obj::WP  winst;		//	�P�������݂ł��Ȃ��C���X�^���X�̃E�B�[�N�|�C���^
		IDirectInput8*  di;	//�}�E�X���p�̈׎c����
		HWND  wnd;

		//�e��R���X�g���N�^����
		Obj(const Obj&);
		Obj(const Obj&&);
		Obj();

		bool  Initialize(HWND wnd_);

		static  vector<GamePad::WP>  gpads;
		static  vector<Mouse::WP>    mouses;

	public:
		~Obj();
		static  Obj::SP  GetInst();
		//	XI XI���C�u�����𐶐�����
		static  Obj::SP  Create(HWND  wnd_);
		HWND  Wnd() { return  wnd; }
		IDirectInput8&  DInput() { return  *di; }
		//���t���[���Ăяo��
		void UpDate();
		
		void Set(GamePad::SP s_);
		void Set(Mouse::SP s_);
	};

	//---------------------------------------------------------------------------
	//�Q�[���p�b�h�}�l�[�W���[
	class  GamePadManager {
	private:
		static  map<string, XI::GamePad::SP>	arr;
		static  bool notObject;
		GamePadManager() { notObject = false; }
	public:
		//�o�^
		static  void  Set(const string& name_, XI::GamePad::SP sp_);
		//���O�Ō���
		static  XI::GamePad::SP  Get(const string& name_);
		//�P�̔j��
		static  void  Clear(const string& name_);
		//���ׂĔj��
		static  void  AllClear();
	};
	//---------------------------------------------------------------------------
	//�}�E�X�}�l�[�W���[
	class  MouseManager {
	private:
		static  map<string, XI::Mouse::SP>	arr;
		static  bool notObject;
		MouseManager() { notObject = false; }
	public:
		//�o�^
		static  void  Set(const string& name_, XI::Mouse::SP sp_);
		//���O�Ō���
		static  XI::Mouse::SP  Get(const string& name_);
		//�P�̔j��
		static  void  Clear(const string& name_);
		//���ׂĔj��
		static  void  AllClear();
	};
}