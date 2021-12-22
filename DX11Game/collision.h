// Õ“Ë”»’è [collision.h]

#pragma once
#include "main.h"

bool CollisionSphere(XMFLOAT3 Apos, float Ar, XMFLOAT3 Bpos, float Br);
bool CollisionAABB(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 Bpos, XMFLOAT3 Bsize);

//‰~“¯m‚Ì“–‚½‚è”»’è
bool CollisionCircle(XMFLOAT2 vA, float fAr, XMFLOAT2 vB, float fBr);

//‹éŒ`“¯m‚Ì“–‚½‚è”»’è
bool CollisionRect(XMFLOAT2 vA, XMFLOAT2 vAs, XMFLOAT2 vB, XMFLOAT2 vBs);