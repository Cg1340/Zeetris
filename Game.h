#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>            /* 处理图像 */
#include <string>
#include "Display.h"
#include "SteadyClock.h"
#include "Button.h"
#include "GameSetting.h"
using namespace sf;
using namespace std;

class Game {
public:
	RenderWindow* window;
	bool startGame(Font font, GameSetting gameSetting);

private:
	/*
	 * TODO:
	 * 1,因为场地是左上角0,0右下角10,20所以偏移表的y轴是反过来的
	 */

	int hold = 0;
	bool isHold = false;
	int blockIndex = 0;            /* 表示当前方块的种类 */
	int rotate = 0;            /* 旋转，0 - 0, 1 - R, 2 - 2, 3 - L */
	int next[14] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	bool nextUsed[7] = { true, true, true, true, true, true, true };
	float downDelay = 500;          /*下降延时 */
	float downDelayFast = 50;           /* 软降时下降延时 */
	float delay = downDelay;
	float dropLockTime = 1000;

	static const int ROW_COUNT = 20;   /* 行数 */
	static const int COL_COUNT = 10;   /* 列数 */

	int table[ROW_COUNT][COL_COUNT] = { 0
	};

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

	struct {
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

	bool move(int move);

	/* ----- 旋转（rota == 0不旋转，-1逆时针，1顺时针） ----- */
	bool rotateBlock(int rota);

	/* ----- 检查是否合法 ----- */
	bool check();

	void newbag(int count);

	/* ----- 生成新方块 ----- */
	void newBlock();

	void swapHold();

	/* ----- 影子系统 -----// */
	void refShadow();
};