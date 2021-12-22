//=============================================================================
//
// マップ [map.h]
// 小楠裕子
//=============================================================================
#pragma once
#include "main.h"
#include "box.h"
//*****構造体定義*****
typedef struct {
	int m_nCategory;	//オブジェクトの種類
	int m_nObject;		//オブジェクト管理番号
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
};		

//*****マクロ定義*****
#define MAP_WIDTH	19
#define MAP_HEIGHT	5

HRESULT InitMap();
void UninitMap();
void UpdateMap();
void DrawOldMap();
void DrawNowMap();

OBJECT_INFO CollisionOldMap(XMFLOAT2 pos, XMFLOAT2 size);
OBJECT_INFO	CollisionNowMap(XMFLOAT2 pos, XMFLOAT2 size);

Box* GetBox();
OBJECT_INFO* GetMap(int);
