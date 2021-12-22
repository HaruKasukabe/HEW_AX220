// Õ“Ë”»’è [collision.cpp]

#include "collision.h"

// ‹…“¯m‚ÌÕ“Ë”»’è
bool CollisionSphere(XMFLOAT3 Apos, float Ar, XMFLOAT3 Bpos, float Br)
{
	float dx = Apos.x - Bpos.x;
	float dy = Apos.y - Bpos.y;
	float dz = Apos.z - Bpos.z;
	float dr = Ar + Br;
	return dx * dx + dy * dy + dz * dz <= dr * dr;
}

// AABB“¯m‚ÌÕ“Ë”»’è
bool CollisionAABB(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 Bpos, XMFLOAT3 Bsize)
{
	return (Apos.x - Asize.x <= Bpos.x + Bsize.x) &&
		(Bpos.x - Bsize.x <= Apos.x + Asize.x) &&
		(Apos.y - Asize.y <= Bpos.y + Bsize.y) &&
		(Bpos.y - Bsize.y <= Apos.y + Asize.y) &&
		(Apos.z - Asize.z <= Bpos.z + Bsize.z) &&
		(Bpos.z - Bsize.z <= Apos.z + Asize.z);
}

//‰~“¯m‚Ì“–‚½‚è”»’è
bool CollisionCircle(XMFLOAT2 vA, float fAr, XMFLOAT2 vB, float fBr) {
	float dx = vA.x - vB.x;
	float dy = vA.y - vB.y;
	float dr = fAr + fBr;
	return dx * dx + dy * dy <= dr * dr;
}

//‹éŒ`“¯m‚Ì“–‚½‚è”»’è
bool CollisionRect(XMFLOAT2 vA, XMFLOAT2 vAs, XMFLOAT2 vB, XMFLOAT2 vBs) {
	return	vA.x - vAs.x <= vB.x + vBs.x &&
		vB.x - vBs.x <= vA.x + vAs.x &&
		vA.y - vAs.y <= vB.y + vBs.y &&
		vB.y - vBs.y <= vA.y + vAs.y;
}