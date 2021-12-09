//===================================================
//		現在[now.h]
//小楠裕子
//=====================================================
#pragma once
#include "main.h"
#include "meshfield.h"
#include "playerGirl.h"

class Now {
public:
	Now();
	~Now();
	void Update();
	void Draw();

	Player_Girl* GetPlayerGirl();
private:
	MeshField* m_pMeshField;		//ﾌｨｰﾙﾄﾞ
	Player_Girl* m_pPlayerGirl;		//女の子
};