#include "DxLib.h"

#include "resource.h"

#define GAME_WIDTH  960

#define GAME_HEIGHT 550

#define GAME_COLOR  32 

#define GAME_WINDOW_BAR   0

#define GAME_WINDOW_NAME  "脱獄"

#define GAME_FPS    60

#define FONT_PATH_MAX   255
#define PATH_MAX   255

#define FONT_MS_PATH           TEXT("chogokubosogothic.ttf")
#define FONT_MS_NAME           TEXT("超極細ゴシック")

#define FONT_INSTALL_ERR_TITLE TEXT("フォントインストールエラー")
#define FONT_CREATE_ERR_TITLE  TEXT("フォント作成エラー")

#define IMAGE_LOAD_ERR_TITLE   TEXT("画像読み込みエラー")

#define IMAGE_BACK_PATH        TEXT(".\\IMAGE\\toshi.png")
#define IMAGE_PLAYER_PATH      TEXT(".\\IMAGE\\syujin.png")

#define IMAGE_TITLE_BK_PATH       TEXT(".\\IMAGE\\kangoku.jpg")
#define IMAGE_TITLE_ROGO_PATH     TEXT(".\\IMAGE\\title.png")
#define IMAGE_TITLE_ROGO_ROTA     0.005  
#define IMAGE_TITLE_ROGO_ROTA_MAX 1.0
#define IMAGE_TITLE_ROGO_X_SPEED  1

#define IMAGE_END_COMP_PATH       TEXT(".\\IMAGE\\end1.png")
#define IMAGE_END_COMP_CNT        1
#define IMAGE_END_COMP_CNT_MAX    30

#define IMAGE_END_FAIL_PATH       TEXT(".\\IMAGE\\end2.png")
#define IMAGE_END_FAIL_CNT        1
#define IMAGE_END_FAIL_CNT_MAX    30

#define IMAGE_BACK_REV_PATH    TEXT(".\\IMAGE\\toshi.png")
#define IMAGE_BACK_NUM         4

#define ITEM_ERR_TITLE TEXT("アイテム位置エラー")
#define ITEM_ERR_CAPTION TEXT("アイテム位置が決まってません")
#define ITEM_GET_TITLE TEXT("仲間を発見")
#define ITEM_GET_CAPTION TEXT("仲間を見つけました")
#define ITEM_MISS_TITLE TEXT("仲間がいません")
#define ITEM_MISS_CAPTION TEXT("仲間を見つけて来てください。")


#define MUSIC_LOAD_ERR_TITLE   TEXT("音楽読み込みエラー")

#define MUSIC_BGM_PATH         TEXT(".\\MUSIC\\逃走.mp3")

#define MUSIC_BGM2_PATH        TEXT(".\\MUSIC\\牢獄.mp3")

#define MUSIC_BGM_TITLE_PATH        TEXT(".\\MUSIC\\Gliese_(Prod._by_Mr_Kimy).mp3")
#define MUSIC_BGM_COMP_PATH         TEXT(".\\MUSIC\\famipop3.mp3")
#define MUSIC_BGM_FAIL_PATH         TEXT(".\\MUSIC\\牢獄.mp3")

#define GOAL_MAX               5

#define ITEM_MAX               5

#define GAME_MAP_TATE_MAX      24
#define GAME_MAP_YOKO_MAX      45
#define GAME_MAP_KIND_MAX      2

#define GAME_MAP_PATH          TEXT(".\\IMAGE\\MAP\\map.png")

#define MAP_DIV_WIDTH          64
#define MAP_DIV_HEIGHT         64
#define MAP_DIV_TATE           10
#define MAP_DIV_YOKO           4
#define MAP_DIV_NUM            MAP_DIV_TATE * MAP_DIV_YOKO

#define START_ERR_TITLE        TEXT("スタート位置エラー")
#define START_ERR_CAPTION      TEXT("スタート位置が決まっていません")

#define GOAL_ERR_TITLE        TEXT("ゴール位置エラー")
#define GOAL_ERR_CAPTION      TEXT("ゴール位置が決まっていません")

#define ESCAPE_CLICK_TITLE    TEXT("ゲーム中断")
#define ESCAPE_CLICK_CAPTION  TEXT("ゲームを中断し、タイトル画面に戻りますか？")

//制限時間
int TimeCou = 0;
#define GAME_TIME_LIMT  120

enum GAME_MAP_KIND
{
	n = -1,
	b = 10,
	t = 9,
	s = 16,
	g = 15,
	k = 13,
	e = 14,
	i = 17
};

enum GAME_SCENE {
	GAME_SCENE_START,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
};

enum GAME_END {
	GAME_END_COMP,
	GAME_END_FAIL
};

enum GAME_SPEED {
	CHARA_SPEED_LOW = 1,
	CHARA_SPEED_MIDI = 2,
	CHARA_SPEED_HIGH = 3
};


typedef struct STRUCT_1_POINT
{
	int x = -1;
	int y = -1;

}iPOINT;


typedef struct STRUCT_FONT
{
	char path[FONT_PATH_MAX];
	char name[FONT_PATH_MAX];
	int handle;
	int size;
	int bold;
	int type;
}FONT;

typedef struct STRUCT_IMAGE
{
	char path[PATH_MAX];
	int handle;
	int x;
	int y;
	int width;
	int height;
}IMAGE;

typedef struct STRUCT_MUSIC
{
	char path[PATH_MAX];
	int handle;
	int handle2;
}MUSIC;


typedef struct STRUCT_CHARA
{
	IMAGE image;
	int speed;
	int CenterX;
	int CenterY;

	RECT coll;
	iPOINT collBeforePt;

	int mapDisX;
	int mapDisY;

}CHARA;

typedef struct STRUCT_IMAGE_BACK
{
	IMAGE image;
	BOOL IsDraw;

}IMAGE_BACK;

typedef struct STRUCT_IMAGE_ROTA
{
	IMAGE image;
	double angle;
	double angleMAX;
	double rate;
	double rateMAX;
}IMAGE_ROTA;

typedef struct STRUCT_IMAGE_BLINK
{
	IMAGE image;
	int Cnt;
	int CntMAX;
	BOOL IsDraw;
}IMAGE_BLINK;

typedef struct STRUCT_MAP_IMAGE
{
	char path[PATH_MAX];
	int handle[MAP_DIV_NUM];
	int kind[MAP_DIV_NUM];
	int width;
	int height;
}MAPCHIP;

typedef struct STRUCT_MAP
{
	GAME_MAP_KIND kind;
	int x;
	int y;
	int width;
	int height;
	RECT coll;
}MAP;

int StartTimeFps;
int CountFps;
float CalcFps;
int SampleNumFps = GAME_FPS;

int AllKeyState[256] = { '\0' };
int OldAllKeyState[256] = { '\0' };


FONT FontTanu32;
FONT MS;

int GameScene;

int GameEndkind;

RECT GoalRect[GOAL_MAX] = { -1,-1,-1,-1 };
int GoalCnt = 0;

RECT ItemRect[ITEM_MAX] = { -1,-1,-1,-1 };
int ItemCnt = 0;

IMAGE_BACK ImageBack[IMAGE_BACK_NUM];

IMAGE ImageTitleBk;
IMAGE_ROTA ImageTitleROGO;
IMAGE_BLINK ImageTitleSTART;

IMAGE_BLINK ImageEndCOMP;
IMAGE_BLINK ImageEndFAIL;

CHARA player;

MUSIC BGM;

MUSIC BGM_TITLE;
MUSIC BGM_COMP;
MUSIC BGM_FAIL;


GAME_MAP_KIND mapData[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX]{
//  0                   1                 2                   3                   4
//  0 1 2 3 4 5 6 7 8 9 0 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 
	k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,    // 0
	k,t,t,t,t,t,k,t,t,t,t,t,t,t,k,k,t,t,t,t,t,k,t,t,t,t,t,t,t,k,k,t,t,t,t,t,k,t,t,t,t,t,t,t,k,    // 1
	k,t,t,t,t,t,k,t,t,t,t,t,t,t,k,k,t,t,t,t,t,k,t,t,t,k,t,t,t,k,k,t,t,t,t,t,k,t,t,t,k,t,t,t,k,    // 2
	k,t,t,t,t,t,k,k,k,t,t,t,t,t,t,t,t,t,t,t,t,k,k,k,t,t,t,t,t,t,t,t,t,t,t,t,k,k,k,t,t,t,t,t,k,    // 3
	k,t,k,t,k,k,s,t,t,t,k,k,t,t,k,k,t,k,k,k,k,k,t,t,t,k,k,t,t,k,k,t,k,k,k,k,k,t,t,t,k,k,t,t,k,    // 4
	k,k,t,t,t,t,k,t,k,t,t,t,t,t,k,k,t,t,t,t,t,k,t,k,t,t,t,t,t,k,k,t,t,t,t,t,k,t,k,t,t,t,t,t,k,    // 5
	k,t,t,t,t,t,t,t,t,t,t,t,t,t,k,k,t,t,t,t,t,t,t,t,t,t,t,t,t,k,k,t,t,t,t,t,t,t,t,t,t,t,t,t,k,    // 6 
	k,k,k,k,k,k,t,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,t,k,k,k,k,k,k,k,k,k,t,k,k,k,k,k,k,k,k,k,k,    // 7
	k,k,t,k,k,k,t,t,k,t,k,k,k,k,k,k,k,k,k,k,k,k,k,k,t,k,k,k,k,k,k,k,k,k,t,k,k,k,k,k,k,k,k,k,k,    // 8
	k,t,t,t,t,t,t,t,t,t,t,t,t,t,k,k,t,t,t,t,t,k,t,t,t,t,t,t,t,k,k,t,t,t,t,t,k,t,t,t,t,t,t,t,k,    // 9
	k,t,t,t,t,t,k,t,t,t,k,t,t,t,k,k,t,t,t,t,t,k,t,t,t,k,t,t,t,k,k,t,t,t,t,t,k,t,t,t,t,t,t,t,k,    // 10
	k,t,t,t,t,t,k,k,k,t,t,t,t,t,t,t,t,t,k,t,t,k,k,k,t,t,t,t,t,t,t,t,t,t,t,t,k,k,k,t,t,t,t,t,k,    // 1
	k,t,k,k,k,k,t,t,t,t,k,k,t,t,k,k,t,k,k,t,k,k,t,t,t,k,k,t,t,k,k,t,k,k,k,k,k,t,t,t,k,k,t,t,k,    // 2
	k,k,t,t,t,t,k,t,k,t,t,t,t,t,k,k,k,t,t,t,t,k,t,k,t,t,t,t,t,k,k,t,t,t,t,t,k,t,k,t,t,t,t,t,k,    // 3
	k,t,t,t,t,t,t,t,t,t,t,t,t,t,k,k,t,t,t,t,t,t,t,t,t,t,t,t,t,k,k,t,t,t,t,t,t,t,t,t,t,t,t,t,k,    // 4 
	k,k,k,t,k,k,t,k,k,t,k,k,k,k,k,k,k,k,k,k,k,k,k,k,t,k,k,k,k,k,k,k,k,k,t,k,k,k,k,k,k,k,k,k,k,    // 5
	k,k,k,k,k,k,t,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,t,k,k,k,k,k,k,k,k,k,t,k,k,k,k,k,k,k,k,k,k,    // 6
	k,t,t,t,t,t,t,t,t,t,t,t,t,t,k,k,t,t,t,t,t,k,t,t,t,t,t,t,t,k,k,t,t,t,t,t,k,t,t,t,t,t,t,t,k,    // 7
	k,t,t,t,t,t,k,t,t,t,k,t,t,t,k,k,t,t,t,t,t,k,t,t,t,k,t,t,t,k,k,t,t,t,t,t,k,t,t,t,k,t,t,t,k,    // 8
	k,t,t,t,t,t,k,k,k,t,t,t,t,t,k,k,t,t,t,t,t,k,k,k,t,t,t,t,t,k,k,k,t,t,t,t,k,k,k,t,t,t,t,t,k,    // 9
	k,t,k,k,k,k,t,t,t,t,k,k,t,t,t,t,t,k,t,k,k,k,t,t,t,k,k,t,t,t,t,t,t,k,k,k,k,t,t,t,k,k,t,t,k,    // 20
	k,k,t,t,t,t,k,t,k,t,t,t,t,t,k,k,k,t,t,t,t,k,t,k,t,t,t,t,t,k,k,t,t,t,t,t,k,t,k,t,t,t,t,t,k,    // 1
	k,t,t,t,t,t,t,t,t,t,t,t,t,t,k,k,t,t,t,t,t,t,t,t,t,t,t,t,t,t,k,t,t,t,k,t,t,t,t,t,t,t,t,t,k,    // 2 
	k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k     // 3
};

GAME_MAP_KIND mapDataInit[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];

MAPCHIP mapChip;

MAP map[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];

iPOINT startPt{ -1,-1 };

iPOINT randmap[5][2][2] =
{
	//{{{6,7},{6,8}},{{15,3},{16,3}}},             //左上
	{{{6,15},{6,16}},{{15,11},{16,11}}},         //左中
	//{{{6,15},{6,16}},{{15,20},{16,20}}},         //左下
	{{{25,7},{25,8}},{{30,3},{31,3}}},         //中上
	{{{25,15},{25,16}},{{30,11},{31,11}}},     //中中
	{{{25,15},{25,16}},{{30,20},{31,20}}},     //中下
	//{{{35,7},{35,8}},{{30,3},{31,33}}},         //右上
	{{{35,15},{35,16}},{{30,11},{31,11}}},       //右中
	//{{{35,15},{35,16}},{{30,20},{31,20}}}      //右下
};

iPOINT randgoal[5] =
{
	{42,0},
	{45,11},
	{1,23},
	{27,23},
	{44,23}
};

iPOINT randitem[5] =
{
	{1,4},
	{1,22},
	{21,11},
	{32,18},
	{39,4}
};


RECT mapColl[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];

VOID MY_FPS_UPDATE(VOID);
VOID MY_FPS_DRAW(VOID);
VOID MY_FPS_WAIT(VOID);

VOID MY_ALL_KEYDOWN_UPDATE(VOID);
BOOL MY_KEY_DOWN(int);
BOOL MY_KEY_UP(int);
BOOL MY_KEYDOWN_KEEP(int, int);

BOOL MY_FONT_INSTALL_ONCE(VOID);
VOID MY_FONT_UNINSTALL_ONCE(VOID);
BOOL MY_FONT_CREATE(VOID);
VOID MY_FONT_DELETE(VOID);

VOID MY_START(VOID);
VOID MY_START_PROC(VOID);
VOID MY_START_DRAW(VOID);

VOID MY_PLAY(VOID);
VOID MY_PLAY_PROC(VOID);
VOID MY_PLAY_DRAW(VOID);

VOID MY_END(VOID);
VOID MY_END_PROC(VOID);
VOID MY_END_DRAW(VOID);

BOOL MY_LOAD_IMAGE(VOID);
VOID MY_DELETE_IMAGE(VOID);

BOOL MY_LOAD_MUSIC(VOID);
VOID MY_DELETE_MUSIC(VOID);

BOOL MY_CHECK_MAP1_PLAYER_COLL(RECT player);
BOOL MY_CHECK_RECT_COLL(RECT, RECT);

extern BOOL IsMapMove = TRUE;

int spHandle;
int spX;
int spY;

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);

	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);

	SetWindowStyleMode(GAME_WINDOW_BAR);

	SetMainWindowText(TEXT(GAME_WINDOW_NAME));

	SetAlwaysRunFlag(TRUE);

	SetWindowIconID(IDI_ICON1);

	if (DxLib_Init() == -1) { return -1; }

	if (MY_LOAD_IMAGE() == FALSE) { return -1; }

	if (MY_LOAD_MUSIC() == FALSE) { return -1; }

	if (MY_FONT_INSTALL_ONCE() == FALSE) { return -1; }

	if (MY_FONT_CREATE() == FALSE) { return -1; }

	SetMouseDispFlag(TRUE);

	GameScene = GAME_SCENE_START;

	SetDrawScreen(DX_SCREEN_BACK);

	//通路を乱数生成
	for (int room = 0; room < 5; room++)  //部屋数
	{
		for (int way = 0; way < 2; way++)  //方向数
		{
			int randway = GetRand(1);  //乱数０か１か
			if (randway == 1)  //１ならば壁を作る
			{
				iPOINT work1 = randmap[room][way][0];
				iPOINT work2 = randmap[room][way][1];
				mapData[work1.y][work1.x] = k;         //通路１個目
				mapData[work2.y][work2.x] = k;         //通路２個目
				map[work1.y][work1.x].kind = k;
				map[work2.y][work2.x].kind = k;
				break;  //全て閉じれれて積むことを防ぐため（どちらか一方はあくようにする）
			}
		}
	}

	//ゴールのランダム生成
	int goalnum = GetRand(4);  //ゴールの数

	iPOINT goal = randgoal[goalnum];
	mapData[goal.y][goal.x] = g;
	map[goal.y][goal.x].kind = g;

	//アイテムのランダム生成
	int Itemnum = GetRand(4);  //ゴールの数

	iPOINT item = randitem[Itemnum];
	mapData[item.y][item.x] = i;
	map[item.y][item.x].kind = i;

	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			//スタート
			if (mapData[tate][yoko] == s)
			{
				startPt.x = mapChip.width * yoko + mapChip.width / 2;
				startPt.y = mapChip.height * tate + mapChip.height / 2;
				break;
			}

			//ゴールの当たり判定
			if (mapData[tate][yoko] == g && GoalCnt < GOAL_MAX)
			{
				GoalRect[GoalCnt].left = mapChip.width * yoko;
				GoalRect[GoalCnt].top = mapChip.height * tate;
				GoalRect[GoalCnt].right = mapChip.width * (yoko + 1);
				GoalRect[GoalCnt].bottom = mapChip.height * (tate + 1);
				GoalCnt++;
			}

			//アイテムの当たり判定
			if (mapData[tate][yoko] == i && ItemCnt < ITEM_MAX)
			{
				ItemRect[ItemCnt].left = mapChip.width * yoko;
				ItemRect[ItemCnt].top = mapChip.height * tate;
				ItemRect[ItemCnt].right = mapChip.width * (yoko + 1);
				ItemRect[ItemCnt].bottom = mapChip.height * (tate + 1);
				ItemCnt++;
			}
		}
	}

	if (startPt.x == -1 && startPt.y == -1)
	{
		MessageBox(GetMainWindowHandle(), START_ERR_CAPTION, START_ERR_TITLE, MB_OK);
		return -1;
	}

	if (GoalRect[GoalCnt].left == -1)
	{
		MessageBox(GetMainWindowHandle(), GOAL_ERR_CAPTION, GOAL_ERR_TITLE, MB_OK);
		return -1;
	}

	while (TRUE)
	{

		if (ProcessMessage() != 0) { break; }
		if (ClearDrawScreen() != 0) { break; }

		MY_ALL_KEYDOWN_UPDATE();

		MY_FPS_UPDATE();


		switch (GameScene)
		{
		case GAME_SCENE_START:
			MY_START();
			break;
		case GAME_SCENE_PLAY:
			MY_PLAY();
			break;
		case GAME_SCENE_END:
			MY_END();
			break;
		}
		 
		MY_FPS_DRAW();

		ScreenFlip();

		MY_FPS_WAIT();
	}

	MY_FONT_DELETE();

	MY_FONT_UNINSTALL_ONCE();

	MY_DELETE_IMAGE();

	MY_DELETE_MUSIC();

	DxLib_End();

	return 0;
}

VOID MY_FPS_UPDATE(VOID)
{
	if (CountFps == 0)
	{
		StartTimeFps = GetNowCount();
	}

	if (CountFps == SampleNumFps)
	{
		int now = GetNowCount();

		CalcFps = 1000.f / ((now - StartTimeFps) / (float)SampleNumFps);
		CountFps = 0;
		StartTimeFps = now;
	}

	//制限時間のカウント進める
	TimeCou++;

	CountFps++;
	return;
}

VOID MY_FPS_DRAW(VOID)
{
	DrawFormatString(GAME_WIDTH - 70, 0, GetColor(255, 255, 255), "FPS:%.1f", CalcFps);
	return;
}

VOID MY_FPS_WAIT(VOID)
{
	int resultTime = GetNowCount() - StartTimeFps;
	int waitTime = CountFps * 1000 / GAME_FPS - resultTime;

	if (waitTime > 0)
	{
		WaitTimer(waitTime);
	}
	return;
}

VOID MY_ALL_KEYDOWN_UPDATE(VOID)
{
	char TempKey[256];

	for (int i = 0; i < 256; i++)
	{
		OldAllKeyState[i] = AllKeyState[i];
	}

	GetHitKeyStateAll(TempKey);

	for (int i = 0; i < 256; i++)
	{
		if (TempKey[i] != 0)
		{
			AllKeyState[i]++;
		}
		else
		{
			AllKeyState[i] = 0;
		}
	}
	return;
}
BOOL MY_KEY_DOWN(int KEY_INPUT_)
{
	if (AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL MY_KEY_UP(int KEY_INPUT_)
{
	if (OldAllKeyState[KEY_INPUT_] >= 1
		&& AllKeyState[KEY_INPUT_] == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL MY_KEYDOWN_KEEP(int KEY_INPUT_, int DownTime)
{
	int UpdateTime = DownTime * GAME_FPS;

	if (AllKeyState[KEY_INPUT_] > UpdateTime)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL MY_FONT_INSTALL_ONCE(VOID)
{
	if (AddFontResourceEx(FONT_MS_PATH, FR_PRIVATE, NULL) == 0)
	{
		MessageBox(GetMainWindowHandle(), FONT_MS_NAME, FONT_INSTALL_ERR_TITLE, MB_OK);
		return FALSE;
	}
	return TRUE;
}

VOID MY_FONT_UNINSTALL_ONCE(VOID)
{
	RemoveFontResourceEx(FONT_MS_PATH, FR_PRIVATE, NULL);

	return;
}

BOOL MY_FONT_CREATE(VOID)
{
	strcpy_s(MS.path, sizeof(MS.path), FONT_MS_PATH);
	strcpy_s(MS.name, sizeof(MS.name), FONT_MS_NAME);
	MS.handle = -1;
	MS.size = 40;
	MS.bold = 1;
	MS.type = DX_FONTTYPE_EDGE;

	MS.handle = CreateFontToHandle(MS.name, MS.size, MS.bold, MS.type);
	if (MS.handle == -1) { MessageBox(GetMainWindowHandle(), FONT_MS_NAME, FONT_CREATE_ERR_TITLE, MB_OK); return FALSE; }

	return TRUE;
}

VOID MY_FONT_DELETE(VOID)
{
	DeleteFontToHandle(MS.handle);

	return;
}

VOID MY_START(VOID)
{
	MY_START_PROC();
	MY_START_DRAW();
}

VOID MY_START_PROC(VOID)
{
	if (CheckSoundMem(BGM.handle2) == 0)
	{
		ChangeVolumeSoundMem(255 * 20 / 100, BGM.handle2);

		//DX_PLAYTYPE_NORMAL;
		//DX_PLAYTYPE_BACK;
		//DX_PLAYTYPE_LOOP;
		PlaySoundMem(BGM.handle2, DX_PLAYTYPE_LOOP);
	}

	if (MY_KEY_DOWN(KEY_INPUT_RETURN) == TRUE)
	{

		if (CheckSoundMem(BGM.handle2) != 0)
		{
			StopSoundMem(BGM.handle2);
		}

		SetMouseDispFlag(FALSE);

		player.mapDisX = 0;
		player.mapDisY = 0;

		player.CenterX = startPt.x;
		player.CenterY = startPt.y;

		player.image.x = player.CenterX;
		player.image.y = player.CenterY;

		player.collBeforePt.x = player.CenterX;
		player.collBeforePt.y = player.CenterY;

		//制限時間の初期化
		TimeCou = 0;


		GameEndkind = GAME_END_FAIL;

		GameScene = GAME_SCENE_PLAY;

	}


	if (ImageTitleROGO.rate < ImageTitleROGO.rateMAX)
	{
		ImageTitleROGO.rate += IMAGE_TITLE_ROGO_ROTA;
	}

	if (ImageTitleROGO.image.x < GAME_WIDTH / 2)
	{
		ImageTitleROGO.image.x += IMAGE_TITLE_ROGO_X_SPEED;
	}

	return;
}

VOID MY_START_DRAW(VOID)
{
	DrawGraph(ImageTitleBk.x, ImageTitleBk.y, ImageTitleBk.handle, TRUE);

	DrawRotaGraph(
		ImageTitleROGO.image.x, ImageTitleROGO.image.y,
		ImageTitleROGO.rate,
		ImageTitleROGO.angle,
		ImageTitleROGO.image.handle, TRUE);

	if (ImageTitleSTART.IsDraw == TRUE)
	{
		DrawGraph(ImageTitleSTART.image.x, ImageTitleSTART.image.y, ImageTitleSTART.image.handle, TRUE);
	}

	DrawString(0, 0, "スタート画面(エンターキーを押してください)", GetColor(255, 255, 255));

	return;
}

VOID MY_PLAY(VOID)
{
	MY_PLAY_PROC();
	MY_PLAY_DRAW();
}

static bool Get = true;		//ITEM獲得判定
static bool Miss = true;		//ITEM獲得MISS（いらなかったかも）
static bool IsKey = false;		//ITEM保持判定

VOID MY_PLAY_PROC(VOID)
{
	if (CheckSoundMem(BGM.handle) == 0)
	{
		ChangeVolumeSoundMem(255 * 30 / 100, BGM.handle);

		//DX_PLAYTYPE_NORMAL;
		//DX_PLAYTYPE_BACK;
		//DX_PLAYTYPE_LOOP;
		PlaySoundMem(BGM.handle, DX_PLAYTYPE_LOOP);
	}

	if (MY_KEY_DOWN(KEY_INPUT_ESCAPE) == TRUE)
	{

		SetMouseDispFlag(TRUE);

		int Ret = MessageBox(GetMainWindowHandle(), ESCAPE_CLICK_CAPTION, ESCAPE_CLICK_TITLE, MB_YESNO);

		if (Ret == IDYES)
		{
			GameScene = GAME_SCENE_START;

			if (CheckSoundMem(BGM.handle) != 0)
			{
				StopSoundMem(BGM.handle);
			}
			return;
		}
		else if (Ret == IDNO)
		{
			SetMouseDispFlag(FALSE);
		}
	}


	//マップを動かすとき
	if (IsMapMove == TRUE)
	{
		//プレイヤーの上移動
		if (MY_KEY_DOWN(KEY_INPUT_W))
		{

			//player.CenterY -= CHARA_SPEED_MIDI;

			player.mapDisY -= player.speed;

			for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
			{
				for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
				{
					mapColl[tate][yoko].top += player.speed; //マップを動かす
					mapColl[tate][yoko].bottom += player.speed; //マップを動かす
				}
			}

			for (int i = 0; i < GoalCnt; i++)
			{
			GoalRect[i].top += player.speed;  //ゴールを動かす
			GoalRect[i].bottom += player.speed;  //ゴールを動かす
			}

			for (int i = 0; i < ItemCnt; i++)
			{
				ItemRect[i].top += player.speed;  //アイテムを動かす
				ItemRect[i].bottom += player.speed;  //アイテムを動かす
			}

			player.coll.left = player.CenterX - mapChip.width / 2 + 5;
			player.coll.top = player.CenterY - mapChip.height / 2 + 5;
			player.coll.right = player.CenterX + mapChip.width / 2 - 5;
			player.coll.bottom = player.CenterY + mapChip.height / 2 - 5;

			if (MY_CHECK_MAP1_PLAYER_COLL(player.coll) == TRUE)
			{
				//player.CenterY += CHARA_SPEED_MIDI;

				player.mapDisY += player.speed;

				for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
				{
					for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
					{
						mapColl[tate][yoko].top -= player.speed; //マップを動かす
						mapColl[tate][yoko].bottom -= player.speed; //マップを動かす

					}
				}

				for (int i = 0; i < GoalCnt; i++)
				{
					GoalRect[i].top -= player.speed;  //ゴールを動かす
					GoalRect[i].bottom -= player.speed;  //ゴールを動かす
				}

				for (int i = 0; i < ItemCnt; i++)
				{
					ItemRect[i].top -= player.speed;  //アイテムを動かす
					ItemRect[i].bottom -= player.speed;  //アイテムを動かす
				}
			}
		}

		//プレイヤーの下移動
		if (MY_KEY_DOWN(KEY_INPUT_S))
		{
			//player.CenterY += CHARA_SPEED_MIDI;

			player.mapDisY += player.speed;

			for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
			{
				for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
				{
					mapColl[tate][yoko].top -= player.speed; //マップを動かす
					mapColl[tate][yoko].bottom -= player.speed; //マップを動かす
				}
			}

			for (int i = 0; i < GoalCnt; i++)
			{
				GoalRect[i].top -= player.speed;  //ゴールを動かす
				GoalRect[i].bottom -= player.speed;  //ゴールを動かす
			}

			for (int i = 0; i < ItemCnt; i++)
			{
				ItemRect[i].top -= player.speed;  //アイテムを動かす
				ItemRect[i].bottom -= player.speed;  //アイテムを動かす
			}

			player.coll.left = player.CenterX - mapChip.width / 2 + 5;
			player.coll.top = player.CenterY - mapChip.height / 2 + 5;
			player.coll.right = player.CenterX + mapChip.width / 2 - 5;
			player.coll.bottom = player.CenterY + mapChip.height / 2 - 5;

			if (MY_CHECK_MAP1_PLAYER_COLL(player.coll) == TRUE)
			{
				//player.CenterY -= CHARA_SPEED_MIDI;

				player.mapDisY -= player.speed;

				for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
				{
					for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
					{
						mapColl[tate][yoko].top += player.speed; //マップを動かす
						mapColl[tate][yoko].bottom += player.speed; //マップを動かす
					}
				}

				for (int i = 0; i < GoalCnt; i++)
				{
					GoalRect[i].top += player.speed;  //ゴールを動かす
					GoalRect[i].bottom += player.speed;  //ゴールを動かす
				}

				for (int i = 0; i < ItemCnt; i++)
				{
					ItemRect[i].top += player.speed;  //アイテムを動かす
					ItemRect[i].bottom += player.speed;  //アイテムを動かす
				}

			}
			//player.CenterY = player.CenterY + player.speed;
		}

		//プレイヤーの右移動
		if (MY_KEY_DOWN(KEY_INPUT_D))
		{
			//player.CenterX += CHARA_SPEED_MIDI;

			player.mapDisX += player.speed;

			for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
			{
				for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
				{
					mapColl[tate][yoko].left -= player.speed; //マップを動かす
					mapColl[tate][yoko].right -= player.speed; //マップを動かす
				}
			}

			for (int i = 0; i < GoalCnt; i++)
			{
				GoalRect[i].left -= player.speed;  //ゴールを動かす
				GoalRect[i].right -= player.speed;  //ゴールを動かす
			}

			for (int i = 0; i < ItemCnt; i++)
			{
				ItemRect[i].left -= player.speed;  //アイテムを動かす
				ItemRect[i].right -= player.speed;  //アイテムを動かす
			}

			player.coll.left = player.CenterX - mapChip.width / 2 + 5;
			player.coll.top = player.CenterY - mapChip.height / 2 + 5;
			player.coll.right = player.CenterX + mapChip.width / 2 - 5;
			player.coll.bottom = player.CenterY + mapChip.height / 2 - 5;
			if (MY_CHECK_MAP1_PLAYER_COLL(player.coll) == TRUE)
			{
				//player.CenterX -= CHARA_SPEED_MIDI;

				player.mapDisX -= player.speed;

				for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
				{
					for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
					{
						mapColl[tate][yoko].left += player.speed; //マップを動かす
						mapColl[tate][yoko].right += player.speed; //マップを動かす

					}
				}

				for (int i = 0; i < GoalCnt; i++)
				{
					GoalRect[i].left += player.speed;  //ゴールを動かす
					GoalRect[i].right += player.speed;  //ゴールを動かす
				}

				for (int i = 0; i < ItemCnt; i++)
				{
					ItemRect[i].left += player.speed;  //アイテムを動かす
					ItemRect[i].right += player.speed;  //アイテムを動かす
				}
			}
			//player.CenterX = player.CenterX + player.speed;
		}
		//プレイヤーの左移動
		if (MY_KEY_DOWN(KEY_INPUT_A))
		{
			//player.CenterX -= CHARA_SPEED_MIDI;

			player.mapDisX -= player.speed;

			for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
			{
				for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
				{
					mapColl[tate][yoko].left += player.speed; //マップを動かす
					mapColl[tate][yoko].right += player.speed; //マップを動かす

				}
			}

			for (int i = 0; i < GoalCnt; i++)
			{
				GoalRect[i].left += player.speed;  //ゴールを動かす
				GoalRect[i].right += player.speed;  //ゴールを動かす
			}

			for (int i = 0; i < ItemCnt; i++)
			{
				ItemRect[i].left += player.speed;  //アイテムを動かす
				ItemRect[i].right += player.speed;  //アイテムを動かす
			}

			player.coll.left = player.CenterX - mapChip.width / 2 + 5;
			player.coll.top = player.CenterY - mapChip.height / 2 + 5;
			player.coll.right = player.CenterX + mapChip.width / 2 - 5;
			player.coll.bottom = player.CenterY + mapChip.height / 2 - 5;
			if (MY_CHECK_MAP1_PLAYER_COLL(player.coll) == TRUE)
			{
				//player.CenterX += CHARA_SPEED_MIDI;

				player.mapDisX += player.speed;

				for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
				{
					for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
					{
						mapColl[tate][yoko].left -= player.speed; //マップを動かす
						mapColl[tate][yoko].right -= player.speed; //マップを動かす
					}
				}

				for (int i = 0; i < GoalCnt; i++)
				{
					GoalRect[i].left -= player.speed;  //ゴールを動かす
					GoalRect[i].right -= player.speed;  //ゴールを動かす
				}

				for (int i = 0; i < ItemCnt; i++)
				{
					ItemRect[i].left -= player.speed;  //アイテムを動かす
					ItemRect[i].right -= player.speed;  //アイテムを動かす
				}
			}
		}

		player.coll.left = player.CenterX - mapChip.width / 2 + 5;
		player.coll.top = player.CenterY - mapChip.height / 2 + 5;
		player.coll.right = player.CenterX + mapChip.width / 2 - 5;
		player.coll.bottom = player.CenterY + mapChip.height / 2 - 5;

		BOOL IsMove = TRUE;

		if (MY_CHECK_MAP1_PLAYER_COLL(player.coll) == TRUE)
		{
			SetMousePoint(player.collBeforePt.x, player.collBeforePt.y);
			IsMove = FALSE;
		}

		player.image.x = player.CenterX - player.image.width / 2;
		player.image.y = player.CenterY - player.image.height / 2;

		RECT PlayerRect;
		int CollRange = 5;
		PlayerRect.left = player.image.x + player.image.width / 2 - CollRange;
		PlayerRect.top = player.image.y + player.image.height / 2 - CollRange;
		PlayerRect.right = player.image.x + player.image.width / 2 + CollRange;
		PlayerRect.bottom = player.image.y + player.image.height / 2 + CollRange;

		for (int i = 0; i < ItemCnt; i++)
		{
			if (Get) {
				if (MY_CHECK_RECT_COLL(PlayerRect, ItemRect[i]) == TRUE) {
					MessageBox(GetMainWindowHandle(), ITEM_GET_CAPTION, ITEM_GET_TITLE, MB_OK);
					IsKey = true;		//アイテム獲得状態
					Get = false;		//アイテムの判定を消す

				}
			}
		}

		for (int i = 0; i < GoalCnt; i++)
		{
			if (MY_CHECK_RECT_COLL(PlayerRect, GoalRect[i]) == TRUE)
			{
				if (IsKey) {
					IsKey = false;	//アイテム獲得をしていない状態に
					Get = true;		//アイテムの判定を戻す
					Miss = true;		//アイテムを持っていない状態の時の処理判定

					if (CheckSoundMem(BGM.handle) != 0) {
						StopSoundMem(BGM.handle);
					}

					GameEndkind = GAME_END_COMP;
					GameScene = GAME_SCENE_END;

					return;
				}

				else {
					if (Miss) {	//メッセージが永遠と出力されるバグ修正
						MessageBox(GetMainWindowHandle(), ITEM_MISS_CAPTION, ITEM_MISS_TITLE, MB_OK);
						Miss = false;
					}
				}
			}
		}

		//制限時間エンド画面へ遷移
		if(TimeCou / GAME_FPS >= GAME_TIME_LIMT)
		{
			if (CheckSoundMem(BGM.handle) != 0)
			{
				StopSoundMem(BGM.handle);
			}

			SetMouseDispFlag(TRUE);

			GameEndkind = GAME_END_FAIL;

			GameScene = GAME_SCENE_END;

			return;
		}

		if (player.image.x > GAME_WIDTH || player.image.y > GAME_HEIGHT
			|| player.image.x + player.image.width < 0 || player.image.y + player.image.height < 0)
		{
			if (CheckSoundMem(BGM.handle) != 0)
			{
				StopSoundMem(BGM.handle);
			}

			SetMouseDispFlag(TRUE);

			GameEndkind = GAME_END_FAIL;

			GameScene = GAME_SCENE_END;

			return;
		}

		for (int num = 0; num < IMAGE_BACK_NUM; num++)
		{
			ImageBack[num].image.x++;

			if (ImageBack[num].IsDraw == FALSE)
			{
				if (ImageBack[num].image.x + ImageBack[num].image.width > 0)
				{
					ImageBack[num].IsDraw = TRUE;
				}
			}

			if (ImageBack[num].image.x > GAME_WIDTH)
			{
				ImageBack[num].image.x = 0 - ImageBack[0].image.width * 3;
				ImageBack[num].IsDraw = FALSE;
			}
		}
		return;
	}
}

VOID MY_PLAY_DRAW(VOID)
{

	for (int num = 0; num < IMAGE_BACK_NUM; num++)
	{
		if (ImageBack[num].IsDraw == TRUE)
		{
			DrawGraph(ImageBack[num].image.x, ImageBack[num].image.y, ImageBack[num].image.handle, TRUE);
		}
	}

	//マップの当たり判定とマップの描画
	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			DrawGraph(
				map[tate][yoko].x -player.mapDisX,
				map[tate][yoko].y -player.mapDisY,
				mapChip.handle[map[tate][yoko].kind],
				TRUE);

			////当たり判定の描画
			//switch (map[tate][yoko].kind)
			//{
			//case k:
			//	DrawBox(
			//		mapColl[tate][yoko].left,
			//		mapColl[tate][yoko].top,
			//		mapColl[tate][yoko].right,
			//		mapColl[tate][yoko].bottom,
			//		GetColor(255, 0, 0),
			//		false
			//	);
			//	break;

			//case t:
			//	DrawBox(
			//		mapColl[tate][yoko].left,
			//		mapColl[tate][yoko].top,
			//		mapColl[tate][yoko].right,
			//		mapColl[tate][yoko].bottom,
			//		GetColor(255, 0, 255),
			//		false
			//	);
			//	break;

			//case g:
			//	DrawBox(
			//		mapColl[tate][yoko].left,
			//		mapColl[tate][yoko].top,
			//		mapColl[tate][yoko].right,
			//		mapColl[tate][yoko].bottom,
			//		GetColor(0, 255, 255),
			//		false
			//	);
			//	break;

			//case i:
			//	DrawBox(
			//		mapColl[tate][yoko].left,
			//		mapColl[tate][yoko].top,
			//		mapColl[tate][yoko].right,
			//		mapColl[tate][yoko].bottom,
			//		GetColor(255, 255, 255),
			//		false
			//	);
			//	break;
			//}

		}

	}

	DrawGraph(player.image.x, player.image.y, player.image.handle, TRUE);

	//プレイヤーの当たり判定の描画
	//DrawBox(
	//	player.coll.left,
	//	player.coll.top,
	//	player.coll.right,
	//	player.coll.bottom,
	//	GetColor(0, 255, 0),
	//	false
	//);

	//制限時間のカウントダウンの描画
	DrawFormatString( 0, 0, GetColor(255, 255, 255), "TIME:%d", GAME_TIME_LIMT - TimeCou / GAME_FPS);

	return;
}

VOID MY_END(VOID)
{
	MY_END_PROC();
	MY_END_DRAW();
}

VOID MY_END_PROC(VOID)
{
	if (MY_KEY_DOWN(KEY_INPUT_ESCAPE) == TRUE)
	{
		if (CheckSoundMem(BGM_COMP.handle) != 0)
		{
			StopSoundMem(BGM_COMP.handle);
		}

		if (CheckSoundMem(BGM_FAIL.handle) != 0)
		{
			StopSoundMem(BGM_FAIL.handle);
		}

		SetMouseDispFlag(TRUE);

		GameScene = GAME_SCENE_START;

		return; /*returnがないと関数が終われない*/
	}

	switch (GameEndkind)
	{
	case GAME_END_COMP:

		if (CheckSoundMem(BGM_COMP.handle) == 0)
		{
			ChangeVolumeSoundMem(255 * 30 / 100, BGM_COMP.handle);

			//DX_PLAYTYPE_NORMAL;
			//DX_PLAYTYPE_BACK;
			//DX_PLAYTYPE_LOOP;
			PlaySoundMem(BGM_COMP.handle, DX_PLAYTYPE_LOOP);
		}

		if (ImageEndCOMP.Cnt < ImageEndCOMP.CntMAX)
		{
			ImageEndCOMP.Cnt += IMAGE_END_COMP_CNT;
		}
		else
		{
			if (ImageEndCOMP.IsDraw == FALSE)
			{
				ImageEndCOMP.IsDraw = TRUE;
			}
			else if (ImageEndCOMP.IsDraw == TRUE)
			{
				ImageEndCOMP.IsDraw = FALSE;
			}
			ImageEndCOMP.Cnt = 0;
		}
		break;

	case GAME_END_FAIL:

		if (CheckSoundMem(BGM_FAIL.handle) == 0)
		{
			ChangeVolumeSoundMem(255 * 20 / 100, BGM_FAIL.handle);

			//DX_PLAYTYPE_NORMAL;
			//DX_PLAYTYPE_BACK;
			//DX_PLAYTYPE_LOOP;
			PlaySoundMem(BGM_FAIL.handle, DX_PLAYTYPE_LOOP);
		}

		if (ImageEndFAIL.Cnt < ImageEndFAIL.CntMAX)
		{
			ImageEndFAIL.Cnt += IMAGE_END_FAIL_CNT;
		}
		else
		{
			if (ImageEndFAIL.IsDraw == FALSE)
			{
				ImageEndFAIL.IsDraw = TRUE;
			}
			else if (ImageEndFAIL.IsDraw == TRUE)
			{
				ImageEndFAIL.IsDraw = FALSE;
			}
			ImageEndFAIL.Cnt = 0;
		}
		break;
	}
	return;
}

VOID MY_END_DRAW(VOID)
{
	//DrawBox(10, 10, GAME_WIDTH - 10, GAME_HEIGHT - 10, GetColor(0, 0, 255), TRUE);

	MY_PLAY_DRAW();

	switch (GameEndkind)
	{
	case GAME_END_COMP:

		DrawGraph(ImageEndCOMP.image.x, ImageEndCOMP.image.y, ImageEndCOMP.image.handle, TRUE);

		//if (ImageEndCOMP.IsDraw == TRUE)
		//{
		//	DrawGraph(ImageEndCOMP.image.x, ImageEndCOMP.image.y, ImageEndCOMP.image.handle, TRUE);
		//}
		break;

	case GAME_END_FAIL:

		DrawGraph(ImageEndFAIL.image.x, ImageEndFAIL.image.y, ImageEndFAIL.image.handle, TRUE);

		//if (ImageEndFAIL.IsDraw == TRUE)
		//{
		//	DrawGraph(ImageEndFAIL.image.x, ImageEndFAIL.image.y, ImageEndFAIL.image.handle, TRUE);
		//}
		break;

	}

	DrawString(0, 0, "スタート画面(エスケープキーを押してください)", GetColor(255, 255, 255));
	return;
}

BOOL MY_LOAD_IMAGE(VOID)
{
	strcpy_s(ImageTitleBk.path, IMAGE_TITLE_BK_PATH);
	ImageTitleBk.handle = LoadGraph(ImageTitleBk.path);
	if (ImageTitleBk.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_TITLE_BK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageTitleBk.handle, &ImageTitleBk.width, &ImageTitleBk.height);
	ImageTitleBk.x = GAME_WIDTH / 2 - ImageTitleBk.width / 2;
	ImageTitleBk.y = GAME_HEIGHT / 2 - ImageTitleBk.height / 2;

	strcpy_s(ImageTitleROGO.image.path, IMAGE_TITLE_ROGO_PATH);
	ImageTitleROGO.image.handle = LoadGraph(ImageTitleROGO.image.path);
	if (ImageTitleROGO.image.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_TITLE_ROGO_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageTitleROGO.image.handle, &ImageTitleROGO.image.width, &ImageTitleROGO.image.height);
	ImageTitleROGO.image.x = 230;
	ImageTitleROGO.image.y = GAME_HEIGHT / 2;
	ImageTitleROGO.angle = DX_PI * 2;
	ImageTitleROGO.angleMAX = DX_PI * 2;
	ImageTitleROGO.rate = 0.0;
	ImageTitleROGO.rateMAX = IMAGE_TITLE_ROGO_ROTA_MAX;

	strcpy_s(ImageEndCOMP.image.path, IMAGE_END_COMP_PATH);
	ImageEndCOMP.image.handle = LoadGraph(ImageEndCOMP.image.path);
	if (ImageEndCOMP.image.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_END_COMP_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageEndCOMP.image.handle, &ImageEndCOMP.image.width, &ImageEndCOMP.image.height);
	ImageEndCOMP.image.x = GAME_WIDTH / 2 - ImageEndCOMP.image.width / 2;
	ImageEndCOMP.image.y = GAME_HEIGHT / 2 - ImageEndCOMP.image.height / 2;
	ImageEndCOMP.Cnt = 0.0;
	ImageEndCOMP.CntMAX = IMAGE_END_COMP_CNT_MAX;
	ImageEndCOMP.IsDraw = FALSE;

	strcpy_s(ImageEndFAIL.image.path, IMAGE_END_FAIL_PATH);
	ImageEndFAIL.image.handle = LoadGraph(ImageEndFAIL.image.path);
	if (ImageEndFAIL.image.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_END_FAIL_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageEndFAIL.image.handle, &ImageEndFAIL.image.width, &ImageEndFAIL.image.height);
	ImageEndFAIL.image.x = GAME_WIDTH / 2 - ImageEndFAIL.image.width / 2;
	ImageEndFAIL.image.y = GAME_HEIGHT / 2 - ImageEndFAIL.image.height / 2;
	ImageEndFAIL.Cnt = 0.0;
	ImageEndFAIL.CntMAX = IMAGE_END_COMP_CNT_MAX;
	ImageEndFAIL.IsDraw = FALSE;



	strcpy_s(ImageBack[0].image.path, IMAGE_BACK_PATH);
	strcpy_s(ImageBack[1].image.path, IMAGE_BACK_REV_PATH);
	strcpy_s(ImageBack[2].image.path, IMAGE_BACK_PATH);
	strcpy_s(ImageBack[3].image.path, IMAGE_BACK_REV_PATH);

	for (int num = 0; num < IMAGE_BACK_NUM; num++)
	{
		ImageBack[num].image.handle = LoadGraph(ImageBack[num].image.path);
		if (ImageBack[num].image.handle == -1)
		{
			MessageBox(GetMainWindowHandle(), IMAGE_BACK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}
		GetGraphSize(ImageBack[num].image.handle, &ImageBack[num].image.width, &ImageBack[num].image.height);
	}

	ImageBack[0].image.x = 0 - ImageBack[0].image.width * 0;
	ImageBack[0].image.y = GAME_HEIGHT / 2 - ImageBack[0].image.height / 2;
	ImageBack[0].IsDraw = FALSE;

	ImageBack[1].image.x = 0 - ImageBack[0].image.width * 1;
	ImageBack[1].image.y = GAME_HEIGHT / 2 - ImageBack[1].image.height / 2;
	ImageBack[1].IsDraw = FALSE;

	ImageBack[2].image.x = 0 - ImageBack[0].image.width * 2;
	ImageBack[2].image.y = GAME_HEIGHT / 2 - ImageBack[0].image.height / 2;
	ImageBack[2].IsDraw = FALSE;

	ImageBack[3].image.x = 0 - ImageBack[0].image.width * 3;
	ImageBack[3].image.y = GAME_HEIGHT / 2 - ImageBack[0].image.height / 2;
	ImageBack[3].IsDraw = FALSE;

	strcpy_s(player.image.path, IMAGE_PLAYER_PATH);
	player.image.handle = LoadGraph(player.image.path);

	if (player.image.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_PLAYER_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(player.image.handle, &player.image.width, &player.image.height);
	player.image.x = GAME_WIDTH / 2 - player.image.width / 2;
	player.image.y = GAME_HEIGHT / 2 - player.image.height / 2;
	player.CenterX = player.image.x + player.image.width / 2;
	player.CenterY = player.image.y + player.image.height / 2;
	player.speed = CHARA_SPEED_HIGH;

	int mapRes = LoadDivGraph(
		GAME_MAP_PATH,
		MAP_DIV_NUM, MAP_DIV_TATE, MAP_DIV_YOKO,
		MAP_DIV_WIDTH, MAP_DIV_HEIGHT,
		&mapChip.handle[0]);

	if (mapRes == -1)
	{
		MessageBox(GetMainWindowHandle(), GAME_MAP_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	GetGraphSize(mapChip.handle[0], &mapChip.width, &mapChip.height);

	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			mapDataInit[tate][yoko] = mapData[tate][yoko];

			map[tate][yoko].kind = mapData[tate][yoko];

			map[tate][yoko].width = mapChip.width;
			map[tate][yoko].height = mapChip.height;

			map[tate][yoko].x = yoko * map[tate][yoko].width;
			map[tate][yoko].y = tate * map[tate][yoko].height;

		}

	}

	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			mapColl[tate][yoko].left = (yoko + 0) * mapChip.width + 1;
			mapColl[tate][yoko].top = (tate + 0) * mapChip.height + 1;
			mapColl[tate][yoko].right = (yoko + 1) * mapChip.width - 1;
			mapColl[tate][yoko].bottom = (tate + 1) * mapChip.height - 1;
		}

	}

	return TRUE;
}

VOID MY_DELETE_IMAGE(VOID)
{
	//DeleteGraph(ImageBack.handle);

	for (int num = 0; num < IMAGE_BACK_NUM; num++)
	{
		DeleteGraph(ImageBack[0].image.handle);
	}
	DeleteGraph(player.image.handle);

	//for (int i = 0; i < ENEMY_MAX; i++)DeleteGraph(enemy[i].image.handle);

	DeleteGraph(ImageTitleBk.handle);
	DeleteGraph(ImageTitleROGO.image.handle);
	DeleteGraph(ImageTitleSTART.image.handle);

	DeleteGraph(ImageEndCOMP.image.handle);
	DeleteGraph(ImageEndFAIL.image.handle);


	for (int i_num = 0; i_num < MAP_DIV_NUM; i_num++) { DeleteGraph(mapChip.handle[i_num]); }

	return;
}

BOOL MY_LOAD_MUSIC(VOID)
{
	strcpy_s(BGM.path, MUSIC_BGM_PATH);
	BGM.handle = LoadSoundMem(BGM.path);
	if (BGM.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), MUSIC_BGM_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	strcpy_s(BGM.path, MUSIC_BGM2_PATH);
	BGM.handle2 = LoadSoundMem(BGM.path);
	if (BGM.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), MUSIC_BGM2_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	strcpy_s(BGM_TITLE.path, MUSIC_BGM_TITLE_PATH);
	BGM_TITLE.handle = LoadSoundMem(BGM_TITLE.path);
	if (BGM_TITLE.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), MUSIC_BGM_TITLE_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	strcpy_s(BGM_COMP.path, MUSIC_BGM_COMP_PATH);
	BGM_COMP.handle = LoadSoundMem(BGM_COMP.path);
	if (BGM_COMP.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), MUSIC_BGM_COMP_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	strcpy_s(BGM_FAIL.path, MUSIC_BGM_FAIL_PATH);
	BGM_FAIL.handle = LoadSoundMem(BGM_FAIL.path);
	if (BGM_FAIL.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), MUSIC_BGM_FAIL_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	return TRUE;
}

VOID MY_DELETE_MUSIC(VOID)
{
	DeleteSoundMem(BGM.handle);
	DeleteSoundMem(BGM.handle2);

	DeleteSoundMem(BGM_TITLE.handle);
	DeleteSoundMem(BGM_COMP.handle);
	DeleteSoundMem(BGM_FAIL.handle);

	return;
}

BOOL MY_CHECK_MAP1_PLAYER_COLL(RECT player)
{
	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			if (MY_CHECK_RECT_COLL(player, mapColl[tate][yoko]) == TRUE)
			{
				if (map[tate][yoko].kind == k) { return TRUE; }
			}
		}
	}
	return FALSE;
}

BOOL MY_CHECK_RECT_COLL(RECT a, RECT b)
{
	if (a.left < b.right &&
		a.top < b.bottom &&
		a.right > b.left &&
		a.bottom > b.top
		)
	{
		return TRUE;
	}
	return FALSE;
}
