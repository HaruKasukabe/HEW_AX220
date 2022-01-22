//=============================================================================
//
// マップ [map.h]
// 小楠裕子
//=============================================================================
#pragma once
#include "main.h"
#include "box.h"
#include "DWBox.h"
#include "HalfBox.h"
#include "Monument.h"
#include <vector>

//*****構造体定義*****
typedef struct {
	int m_nCategory;	//オブジェクトの種類
	int	m_nObject;		//オブジェクト管理番号
	int m_nGravity;		//箱に重力がかかってるかどうか(1:有, 0:無し)
	bool m_bOnBox;		//上の判定用
}OBJECT_INFO;

//*****列挙型*****
// オブジェクトの種類
enum MAP_OBJECT 
{
	NONE,		// なにもなし
	NORMAL,		// 箱
	BREAK,		// 壊せる箱
	CARRY,		// 運べる箱
	PUSH,		// 押せる箱
	GOAL,		// ゴールの石碑
};		

//*****マクロ定義*****
#define MAP_WIDTH	35
#define MAP_HEIGHT	5

#define MAP_END	(270.0f)

HRESULT InitMap();
void UninitMap();
void UpdateMap();
void DrawOldMap();
void DrawNowMap();

// 今までの当たり判定
OBJECT_INFO CollisionOldMap(XMFLOAT2 pos, XMFLOAT2 size);
OBJECT_INFO	CollisionNowMap(XMFLOAT2 pos, XMFLOAT2 size);

// 移動用の当たり判定
std::vector<OBJECT_INFO> WalkCollisionOldMap(XMFLOAT2 pos, XMFLOAT2 size);
std::vector<OBJECT_INFO> WalkCollisionNowMap(XMFLOAT2 pos, XMFLOAT2 size);

// 箱用の当たり判定
std::vector<OBJECT_INFO> BoxCollisionOldMap(XMFLOAT2 pos, XMFLOAT2 size, int nObject);
std::vector<OBJECT_INFO> BoxCollisionNowMap(XMFLOAT2 pos, XMFLOAT2 size, int nObject);

Box* GetBox();
DWBox* GetDWBox();
HalfBox* GetHalfBox();
Monument* GetMonument();
OBJECT_INFO* GetMap(int);
