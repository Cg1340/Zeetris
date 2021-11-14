#include <iostream>
#include <SFML/Graphics.hpp>            /* 处理图像 */
#include <SFML/Audio.hpp>               /* 处理声音 */
#include <time.h>                       /* 处理时间 */
#include <Windows.h>
#include <string>
#include "Button.h"
#include "Display.h"
using namespace sf;
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

bool check();
void newBlock();
void newBlock();
void refShadow();

const int ROW_COUNT = 20;   /* 行数 */
const int COL_COUNT = 10;   /* 列数 */

int table[ROW_COUNT][COL_COUNT] = { 0
};

/*
 * TODO:
 * 1,因为场地是左上角0,0右下角10,20所以偏移表的y轴是反过来的
 */

class Datas {
public:

	class Eliminates {
	public:
		bool tSpin = false;
		bool mini = false;
		int line = 0;
		bool perfectClear = false;
		bool isDisplay = true;

		void resetEliminates() {
			line = 0;
			perfectClear = false;
			isDisplay = true;
			tSpin = false;
			mini = false;
		}

		void storage() {
			gameData.bak.line = gameData.eliminates.line;
			gameData.bak.perfectClear = gameData.eliminates.perfectClear;
			gameData.bak.isDisplay = gameData.eliminates.isDisplay;
			gameData.bak.tSpin = gameData.eliminates.tSpin;
			gameData.bak.mini = gameData.eliminates.mini;
		}
	}eliminates, bak;

	int	hold = 0;
	bool isHold = false;
	int	blockIndex = 0;            /* 表示当前方块的种类 */
	int	rotate = 0;            /* 旋转，0 - 0, 1 - R, 2 - 2, 3 - L */

	int	next[14] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	bool nextUsed[7] = { true, true, true, true, true, true, true };

	float	downDelay = 500;          /*下降延时 */
	float	downDelayFast = 50;           /* 软降时下降延时 */
	float	delay = downDelay;

	float dropLockTime = 1000;

	void reset() {

		hold = 0;
		isHold = false;
		blockIndex = 0;            /* 表示当前方块的种类 */
		rotate = 0;            /* 旋转，0 - 0, 1 - R, 2 - 2, 3 - L */

		for (int i = 0; i < 14; i++) {
			next[i] = 0;
		}

		for (int i = 0; i < 7; i++) {
			nextUsed[i] = true;
		}

		for (int i = 0; i < ROW_COUNT; i++) {
			for (int j = 0; j < COL_COUNT; j++) {
				table[i][j] = 0;
			}
		}

		eliminates.resetEliminates();
		bak.resetEliminates();

		downDelay = 500;          /*下降延时 */
		downDelayFast = 50;           /* 软降时下降延时 */
		delay = downDelay;

		dropLockTime = 1000;
	}
}gameData;

class Settings {
public:
	int ARR = 0;//ms
	int DAS = 0;//ms

	int volume = 100;//0~100

} gameSetting;

int blocks[7] /*28种方块IJLOSTZ各4种形态*/[4][2] /*4个x,y坐标*/ = {
	{                                                                       /* I */
		{ 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 }
	},

	{                                                                       /* J */
		{ 0, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 }
	},

	{                                                                       /* L */
		{ 2, 0 }, { 2, 1 }, { 1, 1 }, { 0, 1 }
	},

	{                                                                       /* O */
		{ 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 }
	},

	{                                                                       /* S */
		{ 2, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 }
	},

	{                                                                       /* T */
		{ 0, 1 }, { 1, 1 }, { 1, 0 }, { 2, 1 }
	},

	{                                                                       /* Z */
		{ 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 }
	}
};

struct KickTables {
	int ZSJLT[8][5][2] = {
		{ { +0, +0 }, { -1, +0 }, { -1, -1 }, { +0, +2 }, { -1, +2 } }, /* 01 */
		{ { +0, +0 }, { +1, +0 }, { +1, +1 }, { +0, -2 }, { +1, -2 } }, /* 12 */
		{ { +0, +0 }, { +1, +0 }, { +1, -1 }, { +0, +2 }, { +1, +2 } }, /* 23 */
		{ { +0, +0 }, { -1, +0 }, { -1, +1 }, { +0, -2 }, { -1, -2 } }, /* 30 */

		{ { +0, +0 }, { +1, +0 }, { +1, -1 }, { +0, +2 }, { +1, +2 } }, /* 03 */
		{ { +0, +0 }, { +1, +0 }, { +1, +1 }, { +0, -2 }, { +1, -2 } }, /* 10 */
		{ { +0, +0 }, { -1, +0 }, { -1, -1 }, { +0, +2 }, { -1, +2 } }, /* 21 */
		{ { +0, +0 }, { -1, +0 }, { -1, +1 }, { +0, -2 }, { -1, -2 } }, /* 32 */
	};                                                                      /* ZSJLT */

	int I[8][5][2] = {
		{ { +0, +0 }, { -2, +0 }, { +1, +0 }, { -2, +1 }, { +1, -2 } }, /* 01 */
		{ { +0, +0 }, { -1, +0 }, { +2, +0 }, { -1, -2 }, { +2, +1 } }, /* 12 */
		{ { +0, +0 }, { +2, +0 }, { -1, +0 }, { +2, -1 }, { -1, +2 } }, /* 23 */
		{ { +0, +0 }, { +1, +0 }, { -2, +0 }, { +1, +2 }, { -2, -1 } }, /* 30 */

		{ { +0, +0 }, { -1, +0 }, { +2, +0 }, { -1, -2 }, { +2, +1 } }, /* 03 */
		{ { +0, +0 }, { +2, +0 }, { -1, +0 }, { +2, -1 }, { -1, +2 } }, /* 10 */
		{ { +0, +0 }, { +1, +0 }, { -2, +0 }, { +1, +2 }, { -2, -1 } }, /* 21 */
		{ { +0, +0 }, { -2, +0 }, { +1, +0 }, { -2, +1 }, { +1, -2 } }, /* 32 */
	};                                                                      /* I */
} kickTable;

/* 单个方块 */
struct Point {
	float	x;
	float	y;
} curBlock[4], bakBlock[4], shadow[4];

class KeyEvents {
public:
	bool move(int move)
	{
		for (int i = 0; i < 4; i++)
		{
			bakBlock[i].x = curBlock[i].x;
			bakBlock[i].y = curBlock[i].y;
			curBlock[i].x += move;
		}
		if (!check())
		{
			for (int i = 0; i < 4; i++)
			{
				curBlock[i] = bakBlock[i];
			}
			return(false);
		}
		return(true);
	}

	/* ----- 旋转（rota == 0不旋转，-1逆时针，1顺时针） ----- */
	bool rotateBlock(int rota)
	{
		Point	p;
		Point	tmpBlock[4];

		/* 开始旋转 */
		for (int i = 0; i < 4; i++)
		{
			tmpBlock[i] = curBlock[i];
		}

		/* 计算旋转中心 */
		if (gameData.blockIndex == 1)
		{
			p = curBlock[1];
			switch (gameData.rotate)
			{
			case 0:
				p.x += 0.5;
				p.y += 0.5;
				break;
			case 1:
				p.x -= 0.5;
				p.y += 0.5;
				break;
			case 2:
				p.x -= 0.5;
				p.y -= 0.5;
				break;
			case 3:
				p.x += 0.5;
				p.y -= 0.5;
				break;
			default:
				break;
			}
		}
		else if (gameData.blockIndex == 6)
		{
			p = curBlock[1];
		}
		else if (gameData.blockIndex == 4)
		{
			return(false);
		}
		else {
			p = curBlock[2];
		}

		for (int i = 0; i < 4; i++)
		{
			float	x = curBlock[i].y - p.y;
			float	y = curBlock[i].x - p.x;
			if (rota == 1)
			{
				curBlock[i].x = p.x - x;
				curBlock[i].y = p.y + y;
			}
			else if (rota == -1)
			{
				curBlock[i].x = p.x + x;
				curBlock[i].y = p.y - y;
			}
		}

		//偏移之前先备份
		for (int i = 0; i < 4; i++) {
			bakBlock[i] = curBlock[i];
		}

		for (int i = 0; i < 5; i++) {
			if (gameData.blockIndex == 1) {
				for (int j = 0; j < 4; j++) {
					curBlock[j].x += kickTable.I[(rota == 1 ? gameData.rotate : gameData.rotate + 4)][i][0];
					curBlock[j].y += kickTable.I[(rota == 1 ? gameData.rotate : gameData.rotate + 4)][i][1];
				}
			}
			else {
				for (int j = 0; j < 4; j++) {
					curBlock[j].x += kickTable.ZSJLT[(rota == 1 ? gameData.rotate : gameData.rotate + 4)][i][0];
					curBlock[j].y += kickTable.ZSJLT[(rota == 1 ? gameData.rotate : gameData.rotate + 4)][i][1];
				}
			}

			if (!check()) {
				for (int j = 0; j < 4; j++)
					curBlock[j] = bakBlock[j];
				continue;
			}
			else {
				if (rota == 1) {
					gameData.rotate = (gameData.rotate == 3 ? 0 : gameData.rotate + 1);
				}
				else if (rota == -1) {
					gameData.rotate = (gameData.rotate == 0 ? 3 : gameData.rotate - 1);
				}
				return(true);
			}
		}

		for (int i = 0; i < 4; i++)
		{
			curBlock[i] = tmpBlock[i];
		}
		return(false);
	}
} KeyEvents;

/* ----- 检查是否合法 ----- */
bool check()
{
	for (int i = 0; i < 4; i++)
	{
		if (curBlock[i].x < 0
			|| curBlock[i].x >= COL_COUNT
			|| curBlock[i].y >= ROW_COUNT
			|| table[int(curBlock[i].y)][int(curBlock[i].x)])
		{
			return(false);
		}
	}
	return(true);
}

void newbag(int count) {
	if (count == 7) {
		for (int i = 7; i < 14; i++) {
			int randomNum = 1 + rand() % (14 - i);
			int counting = 0;
			while (1) {
				if (gameData.nextUsed[counting] == true)
					randomNum--;

				if (randomNum == 0) {
					gameData.nextUsed[counting] = false;
					gameData.next[i] = counting + 1;
					break;
				}

				counting++;
			}
		}
	}
	else if(count == 14) {
		for (int i = 0; i < 7; i++) {
			int randomNum = 1 + rand() % (7 - i); /* 随机取1~7随机数 */
			int counting = 0;
			while (1) {
				if (gameData.nextUsed[counting] == true)
					randomNum--;

				if (randomNum == 0) {
					gameData.nextUsed[counting] = false;
					gameData.next[i] = counting + 1;
					break;
				}

				counting++;
			}
		}

		for (int i = 0; i < 7; i++) {
			gameData.nextUsed[i] = true;
		}

		for (int i = 7; i < 14; i++) {
			int randomNum = 1 + rand() % (14 - i); /* 随机取1~7随机数 */
			int counting = 0;
			while (1) {
				if (gameData.nextUsed[counting] == true)
					randomNum--;

				if (randomNum == 0) {
					gameData.nextUsed[counting] = false;
					gameData.next[i] = counting + 1;
					break;
				}

				counting++;
			}
		}
	}
}

/* ----- 生成新方块 ----- */
void newBlock()
{
	/*
	 * 获取一个随机值（1~7）
	 * TODO:7bag出块方式
	 */

	int count = 0;
	for (int i = 0; i < 14; i++) {
		if (gameData.next[i]) {
			count++;
		}
	}

	for (int i = 0; i < 7; i++) {
		gameData.nextUsed[i] = true;
	}

	if (count == 0) {
		newbag(14);
	}
	else if (count == 7) {
		newbag(7);
	}

	gameData.blockIndex = gameData.next[0];
	for (int i = 0; i < 13; i++) {
		gameData.next[i] = gameData.next[i + 1];
	}
	gameData.next[13] = 0;

	for (int i = 0; i < 4; i++)
	{
		curBlock[i].x = blocks[gameData.blockIndex - 1][i][0] + 3;
		curBlock[i].y = blocks[gameData.blockIndex - 1][i][1];
	}

	gameData.rotate = 0;
}

void swapHold()
{
	int tmp = gameData.blockIndex;
	gameData.blockIndex = gameData.hold;
	gameData.hold = tmp;

	for (int i = 0; i < 4; i++)
	{
		curBlock[i].x = blocks[gameData.blockIndex - 1][i][0] + 3;
		curBlock[i].y = blocks[gameData.blockIndex - 1][i][1];
	}
}

/* ----- 影子系统 -----// */
void refShadow()
{
	bool	shadowIsLock = false;
	Point	tmp[4];

	for (int i = 0; i < 4; i++)
	{
		shadow[i].x = 0;
		shadow[i].y = 0;

		int	xx = curBlock[i].x - blocks[gameData.blockIndex - 1][i][0];
		int	yy = curBlock[i].y - blocks[gameData.blockIndex - 1][i][1];

		shadow[i].x = blocks[gameData.blockIndex - 1][i][0] + xx;
		shadow[i].y = blocks[gameData.blockIndex - 1][i][1] + yy;
	}

	while (1)
	{
		for (int i = 0; i < 4; i++)
		{
			tmp[i] = shadow[i];
			++shadow[i].y;
		}

		for (int i = 0; i < 4; i++)
		{
			if (shadow[i].x < 0
				|| shadow[i].x >= COL_COUNT
				|| shadow[i].y >= ROW_COUNT
				|| table[int(shadow[i].y)][int(shadow[i].x)]
				/*|| (curBlock[i].x == shadow[i].x && curBlock[i].y == shadow[i].y)*/)
			{
				for (int j = 0; j < 4; j++)
				{
					shadow[j] = tmp[j];
				}
				return;
			}
		}
	}
}

void game(Font font, RenderWindow *window) {

	gameData.reset();

	srand(time(0));                     /* 生成一个随机种子 */

	Clock	downDelay;                      /* 生成一个计时器 */
	Clock	lockTimer;
	float	timer = 0;
	bool	isLock = false;

	Text eliminateText;
	eliminateText.setFont(font);
	eliminateText.move(512, 256);
	Clock textTimer;

	int line = 0;
	Text lineText;
	lineText.setFont(font);
	lineText.setCharacterSize(55);
	lineText.move(250,256);

	Clock readyClock;
	readyClock.restart();
	Text readyText;
	readyText.setFont(font);
	readyText.setCharacterSize(55);
	bool readyFinish = false;

	newbag(14);

	/* ----- 添加游戏背景 -----// */
	Texture background, tiles, nextB, holdB;       /* 把图片加载到内存里 */

	background.loadFromFile("image/background.png");
	Sprite spriteBackGround(background);  /* 根据图片创建“精灵” */
	spriteBackGround.move(222, 0);

	tiles.loadFromFile("image/tiles.png");
	nextB.loadFromFile("image/tiles.png");
	holdB.loadFromFile("image/tiles.png");
	Sprite	spriteBlock(tiles);
	Sprite	nextBlock1(nextB);
	Sprite	nextBlock2(nextB);
	Sprite	nextBlock3(nextB);
	Sprite	nextBlock4(nextB);
	Sprite	nextBlock5(nextB);
	Sprite	holdBlock(holdB);

	Button backBtn;
	backBtn.window = window;

	Event event;

	/* ----- 开始游戏 -----// */
	while ((*window).isOpen()) /* 如果窗口没有被关闭，继续循环 */
	{
		while (window->pollEvent(event))
		{
			Vector2i mousePos = Mouse::getPosition(*window);
			Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

			if (backBtn.click(event, mousePos, mousePosF)) {
				return;
			}

			if (event.type == Event::Closed)
				window->close();

			if (event.type == Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case Keyboard::A:
					if (KeyEvents.move(-1)) {
						lockTimer.restart();
					}
					break;

				case Keyboard::D:
					if (KeyEvents.move(1)) {
						lockTimer.restart();
					}
					break;

				case Keyboard::LShift:
					if (gameData.isHold == false) {
						if (gameData.hold == 0)
						{
							gameData.hold = gameData.blockIndex;
							newBlock();
						}
						else {
							swapHold();
						}
						gameData.isHold = true;
					}
					break;

				case Keyboard::O:
					if (KeyEvents.rotateBlock(-1)) {
						lockTimer.restart();
					}
					break;

				case Keyboard::P:
					if (KeyEvents.rotateBlock(1)) {
						lockTimer.restart();
					}
					break;

				case Keyboard::Space:
					for (int i = 0; i < 4; i++)
					{
						curBlock[i] = shadow[i];
					}

					/* 要固化处理 */
					for (int i = 0; i < 4; i++)
					{
						table[int(curBlock[i].y)][int(curBlock[i].x)] = gameData.blockIndex;
					}

					gameData.isHold = false;

					newBlock();
					if (!check())
					{
						return;
					}
					break;

				case Keyboard::Escape:
					return;
				default:
					break;
				}
			}

			/* 处理软降 */
			if (Keyboard::isKeyPressed(Keyboard::S) && !isLock)
			{
				gameData.delay = gameData.downDelayFast;
			}
			else {
				gameData.delay = gameData.downDelay;
			}
		}

		/* ----- 画方块 -----// */
		(*window).clear();

		backBtn.setButton("image/button/back.png", 900.0f, 10.0f, 2);
		backBtn.draw(font, L"");

		(*window).draw(spriteBackGround); /* 渲染背景 */;

		for (int i = 0; i < ROW_COUNT; i++)
		{
			for (int j = 0; j < COL_COUNT; j++)
			{
				if (table[i][j] == 0 /*|| table[i][j] == -1*/)
					continue;
				spriteBlock.setTextureRect(IntRect(table[i][j] * 30, 0, 30, 30));
				spriteBlock.setPosition(j * 30, i * 30);
				spriteBlock.move(362, 5); /* offset */
				(*window).draw(spriteBlock);
			}
		}
		for (int i = 0; i < 4; i++)
		{
			/*下落中的方块 */
			spriteBlock.setTextureRect(IntRect(gameData.blockIndex * 30, 0, 30, 30));
			spriteBlock.setPosition(curBlock[i].x * 30, curBlock[i].y * 30);
			spriteBlock.move(362, 5);       /* offset */
			if (readyFinish) {
				(*window).draw(spriteBlock);
			}

			/* 影子 */
			spriteBlock.setTextureRect(IntRect(0, 0, 30, 30));
			spriteBlock.setPosition(shadow[i].x * 30, shadow[i].y * 30);
			spriteBlock.move(362, 5);       /* offset */
			if (readyFinish) {
				(*window).draw(spriteBlock);
			}

			/*next */
			holdBlock.setTextureRect(IntRect(gameData.hold * 30, 0, 30, 30));
			nextBlock1.setTextureRect(IntRect(gameData.next[0] * 30, 0, 30, 30));
			nextBlock2.setTextureRect(IntRect(gameData.next[1] * 30, 0, 30, 30));
			nextBlock3.setTextureRect(IntRect(gameData.next[2] * 30, 0, 30, 30));
			nextBlock4.setTextureRect(IntRect(gameData.next[3] * 30, 0, 30, 30));
			nextBlock5.setTextureRect(IntRect(gameData.next[4] * 30, 0, 30, 30));

			if (gameData.hold != 0) {
				holdBlock.setPosition((blocks[gameData.hold - 1][i][0] + 3) * 30, blocks[gameData.hold - 1][i][1] * 30);
			}
			nextBlock1.setPosition((blocks[gameData.next[0] - 1][i][0] + 3) * 30, blocks[gameData.next[0] - 1][i][1] * 30);
			nextBlock2.setPosition((blocks[gameData.next[1] - 1][i][0] + 3) * 30, blocks[gameData.next[1] - 1][i][1] * 30);
			nextBlock3.setPosition((blocks[gameData.next[2] - 1][i][0] + 3) * 30, blocks[gameData.next[2] - 1][i][1] * 30);
			nextBlock4.setPosition((blocks[gameData.next[3] - 1][i][0] + 3) * 30, blocks[gameData.next[3] - 1][i][1] * 30);
			nextBlock5.setPosition((blocks[gameData.next[4] - 1][i][0] + 3) * 30, blocks[gameData.next[4] - 1][i][1] * 30);

			if (gameData.hold == 1)holdBlock.move(142, 35);
			else if (gameData.hold == 4)holdBlock.move(172, 50);
			else
				holdBlock.move(157, 50);


			if (gameData.next[0] == 1)nextBlock1.move(582, 35);
			else if (gameData.next[0] == 4)nextBlock1.move(612, 50);
			else
				nextBlock1.move(597, 50);


			if (gameData.next[1] == 1)nextBlock2.move(582, 110);
			else if (gameData.next[1] == 4)nextBlock2.move(612, 125);
			else
				nextBlock2.move(597, 125);


			if (gameData.next[2] == 1)nextBlock3.move(582, 185);
			else if (gameData.next[2] == 4)nextBlock3.move(612, 200);
			else
				nextBlock3.move(597, 200);


			if (gameData.next[3] == 1)nextBlock4.move(582, 260);
			else if (gameData.next[3] == 4)nextBlock4.move(612, 275);
			else
				nextBlock4.move(597, 275);


			if (gameData.next[4] == 1)nextBlock5.move(582, 335);
			else if (gameData.next[4] == 4)nextBlock5.move(612, 350);
			else
				nextBlock5.move(597, 350);

			if (gameData.hold != 0) {
				(*window).draw(holdBlock);
			}

			(*window).draw(nextBlock1);
			(*window).draw(nextBlock2);
			(*window).draw(nextBlock3);
			(*window).draw(nextBlock4);
			(*window).draw(nextBlock5);
		}

		std::string lineString = "";
		lineString += (line / 100 == 0 ? "0" : "");
		lineString += (line / 10 == 0 ? "0" : "");
		lineString += (line / 1 == 0 ? "0" : "");
		lineString += (line == 0 ? "" : std::to_string(line));
		lineText.setString(lineString);
		(*window).draw(lineText);

		//ready...set...go
		if (readyClock.getElapsedTime().asMilliseconds() >= 0 && readyClock.getElapsedTime().asMilliseconds() <= 3000 && !readyFinish) {

			if (readyClock.getElapsedTime().asMilliseconds() < 1000) {
				readyText.setString("ready...");
			}
			else if (readyClock.getElapsedTime().asMilliseconds() < 2000 && readyClock.getElapsedTime().asMilliseconds() >= 1000) {
				readyText.setString("set...");
			}
			else if (readyClock.getElapsedTime().asMilliseconds() >= 2000) {
				readyText.setString("go!");
			}

			readyText.setPosition(512, 250);
			FloatRect lb = readyText.getLocalBounds();
			readyText.setOrigin(lb.left + lb.width / 2, lb.top + lb.height / 2);

			(*window).draw(readyText);
			(*window).display();
			continue;
		}
		else if(readyClock.getElapsedTime().asMilliseconds() > 3000 && !readyFinish) {
			readyFinish = true;
			newBlock();                             /* 产生新方块 */
		}

		(*window).display();

		if (isLock)
		{
			timer = lockTimer.getElapsedTime().asMilliseconds();
		}



		refShadow();

		for (int i = 0; i < 4; i++)
		{
			if (curBlock[i].y == shadow[i].y)
			{
				isLock = true;
				if (timer >= gameData.dropLockTime /*dropLockTime*/)
				{
					/* 要固化处理 */
					for (int i = 0; i < 4; i++)
					{
						table[int(curBlock[i].y)][int(curBlock[i].x)] = gameData.blockIndex;
					}

					gameData.isHold = false;

					newBlock();
					if (!check())
					{
						return;
					}
					downDelay.restart();
					lockTimer.restart();
					timer = 0;
					isLock = false;
				}
			}
			else {
				isLock = false;
				timer = 0;
				lockTimer.restart();
			}
		}

		/*
		 * ----- 下降 -----
		 * 获取从clock被重启或被启动之后，到现在的时间
		 */
		if (downDelay.getElapsedTime().asMilliseconds() >= gameData.delay && !isLock && check())
		{
			for (int i = 0; i < 4; i++)
			{
				bakBlock[i].x = curBlock[i].x;
				bakBlock[i].y = curBlock[i].y;

				++curBlock[i].y;
			}
			downDelay.restart();
		}

		/* ----- 消除 -----// */
		{
			int clearLine = 0;//消除的行数
			for (int i = 0; i < ROW_COUNT; i++) {
				int count = 0;
				for (int j = 0; j < COL_COUNT; j++) {
					if (table[i][j])
						++count;
				}
				if (count == COL_COUNT) {
					++clearLine;
					for (int j = i; j > 0; j--) {
						for (int k = 0; k < COL_COUNT; k++) {
							table[j][k] = table[j - 1][k];
						}
					}
					for (int j = 0; j < COL_COUNT; j++) {
						table[0][j] = 0;
					}
				}
			}

			line += clearLine;
		}
	}
}

int main()
{
	RenderWindow window(
		VideoMode(1024, 768),         /* 窗口分辨率 */
		"Zeetris");                    /* 窗口标题 */

	Font font;
	font.loadFromFile("fonts/proportional.ttf");

	Music backgroundMusic;
	backgroundMusic.openFromFile(
		"music/other side - Lena Raine.wav");            /* 加载背景音乐 */
	backgroundMusic.setLoop(true); /* 设置重复播放音乐 */
	backgroundMusic.setVolume(gameSetting.volume);
	backgroundMusic.play();                 /* 播放音乐 */

	while (window.isOpen()) {
		Event event;

		Button playButton, settingButton, exitButton;
		Texture titleTexture;
		titleTexture.loadFromFile("image/title.png");
		Sprite title(titleTexture);

		while (1) {
			window.clear();

			title.setScale(0.5f, 0.5f);
			title.setPosition(400.0f, 400.0f);
			window.draw(title);

			playButton.window = &window;
			settingButton.window = &window;
			exitButton.window = &window;

			playButton.setButton("image/button/start.png", 0.0f, 100.0f, 3);
			settingButton.setButton("image/button/setting.png", 0.0f, 200.0f, 3);
			exitButton.setButton("image/button/exit.png", 0.0f, 300.0f, 3);

			playButton.draw(font, L"开始");
			settingButton.draw(font, L"设置");
			exitButton.draw(font, L"退出");

			displayText(&window, font, L"操作说明：\nA/D 向左/向右移动\nS 软降\nO/P 逆时针/顺时针旋转\n空格 硬降\n左shift 暂存", 30, 0, 500, 1, 1);

			window.display();

			window.pollEvent(event);
			Vector2i mousePos = Mouse::getPosition(window);
			Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

			if (playButton.click(event, mousePos, mousePosF)) {
				break;
			}

			if (settingButton.click(event, mousePos, mousePosF)) {
				SettingButton volume;
				Button backBtn;
				backBtn.window = &window;
				while (1) {
					window.clear();

					backBtn.setButton("image/button/back.png", 900.0f, 700.0f, 2);
					backBtn.draw(font, L"");

					volume.setButton(0, 0, font, gameSetting.volume);
					FloatRect box = volume.subButton.buttonSprite.getGlobalBounds();
					volume.window = &window;
					volume.draw(&window, font, L"音量：" + to_wstring(gameSetting.volume));

					window.display();

					window.pollEvent(event);
					Vector2i mousePos = Mouse::getPosition(window);
					Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

					gameSetting.volume += volume.click(event, mousePos, mousePosF) * 10;
					if (gameSetting.volume > 100)gameSetting.volume = 100;
					else if (gameSetting.volume < 0)gameSetting.volume = 0;
					backgroundMusic.setVolume(gameSetting.volume);

					if (backBtn.click(event, mousePos, mousePosF)) {
						break;
					}
				}
			}

			if (exitButton.click(event, mousePos, mousePosF)
				|| event.type == Event::Closed
				|| event.key.code == Keyboard::Escape) {
				window.close();
				return 0;
			}
		}

		game(font, &window);
		window.clear();
	}

	return(0);
}