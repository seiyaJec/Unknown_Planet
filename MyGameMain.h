#pragma once
#include "MyPG.h"

extern  void  MyGameMain_Finalize( );
extern  void  MyGameMain_Initialize( );
extern  void  MyGameMain_UpDate( );
extern  void  MyGameMain_Render2D( );

enum class TaskFlag
{
	Non,		//�^�X�N�����I�����
	Title,		//�^�C�g��
	Game,		//�Q�[���{��
	Result,		//���U���g
	GameOver,	//�Q�[���I�[�o�[

	//�ȉ��K�v�ɉ����Ēǉ�
};
