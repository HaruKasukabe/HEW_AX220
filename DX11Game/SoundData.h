// BGM/SE定義 (SoundData.h)

enum eBGM {
	BGM_000 = 0,	// BGM0
	BGM_001,		// BGM1
	BGM_TITLE,
	BGM_WALK,		 // 歩く音

	MAX_BGM
};

enum eSE {
	SE_DECIDE = 0,	 // 決定音
	SE_CANCEL,		 // キャンセル音
	SE_SELECT,       // 選択音
	SE_SHOT,		 // 弾発射音
	SE_PAUSE_SELECT, // ポーズ選択
	SE_DECISION,     // ポーズ決定
	SE_JUMP,		 // ジャンプ
	SE_UP,			 // 持ち上げる
	SE_DOWN,		 // おろす
	SE_BREAK,		 // 岩を壊す
	SE_CLEAR,		 // クリア音
	SE_TUTORIAL,	 // チュートリアル音

	MAX_SE
};
