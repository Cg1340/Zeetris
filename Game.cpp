#include"Game.h"

bool Game::startGame(Font font, GameSetting gameSetting) {
	srand(time(0));                     /* 生成一个随机种子 */

	SteadyClock downDelayTimer, lockTimer, textTimer, readyClock, gameTimer;

	bool	isLock = false;

	Text eliminateText;
	eliminateText.setFont(font);
	eliminateText.move(512, 256);

	int line = 0;
	Text lineText;
	lineText.setFont(font);
	lineText.setCharacterSize(55);
	lineText.move(250, 256);

	long long Timer = 0;

	Text readyText;
	readyText.setFont(font);
	readyText.setCharacterSize(55);
	bool readyFinish = false;

	newbag(14);

	/* ----- 添加游戏背景 -----// */
	Texture background, tiles, nextB, holdB;       /* 把图片加载到内存里 */

	background.loadFromFile("media/image/background.png");
	Sprite spriteBackGround(background);  /* 根据图片创建“精灵” */
	spriteBackGround.move(222, 0);

	tiles.loadFromFile("media/image/tiles.png");
	nextB.loadFromFile("media/image/tiles.png");
	holdB.loadFromFile("media/image/tiles.png");
	Sprite	spriteBlock(tiles);
	Sprite	nextBlock1(nextB);
	Sprite	nextBlock2(nextB);
	Sprite	nextBlock3(nextB);
	Sprite	nextBlock4(nextB);
	Sprite	nextBlock5(nextB);
	Sprite	holdBlock(holdB);

	Button pauseBtn;
	pauseBtn.window = window;

	Event event;

	readyClock.restart();

	/* ----- 开始游戏 -----// */
	while ((*window).isOpen()) { /* 如果窗口没有被关闭，继续循环 */
		while (window->pollEvent(event))
		{
			Vector2i mousePos = Mouse::getPosition(*window);
			Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

			if (pauseBtn.click(event, mousePos, mousePosF)) {

				downDelayTimer.pause();
				lockTimer.pause();
				gameTimer.pause();

				Button continueBtn;
				continueBtn.window = window;
				continueBtn.setButton("media/image/button/button.png", 100.0f, 200.0f, 3);
				Button quitBtn;
				quitBtn.window = window;
				quitBtn.setButton("media/image/button/button.png", 100.0f, 100.0f, 3);
				while (true) {
					window->clear();
					continueBtn.draw(font, 40, L"continue");
					quitBtn.draw(font, 40, L"quit");
					window->display();

					window->pollEvent(event);
					Vector2i mousePos = Mouse::getPosition(*window);
					Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

					if (continueBtn.click(event, mousePos, mousePosF)) {
						downDelayTimer.start();
						lockTimer.start();
						gameTimer.start();
						break;
					}

					if (quitBtn.click(event, mousePos, mousePosF)) {
						return -1;
					}
				}
			}

			if (event.type == Event::Closed)
				window->close();

			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == gameSetting.moveLeft) {
					if (move(-1)) {
						lockTimer.restart();
					}
				}
				else if (event.key.code == gameSetting.moveRight) {
					if (move(1)) {
						lockTimer.restart();
					}
				}
				else if (event.key.code == gameSetting.hold) {
					if (isHold == false) {
						if (hold == 0)
						{
							hold = blockIndex;
							newBlock();
						}
						else {
							swapHold();
						}
						isHold = true;
					}
				}
				else if (event.key.code == gameSetting.rotateLeft) {
					if (rotateBlock(-1)) {
						lockTimer.restart();
					}
				}
				else if (event.key.code == gameSetting.rotateRight) {
					if (rotateBlock(1)) {
						lockTimer.restart();
					}
				}
				else if (event.key.code == gameSetting.hardDrop) {
					for (int i = 0; i < 4; i++)
					{
						curBlock[i] = shadow[i];
					}

					/* 要固化处理 */
					for (int i = 0; i < 4; i++)
					{
						table[int(curBlock[i].y)][int(curBlock[i].x)] = blockIndex;
					}

					isHold = false;

					newBlock();
					if (!check())
					{
						return 0;
					}
					break;
				}
			}
		}

		/* 处理软降 */
		if (Keyboard::isKeyPressed(gameSetting.softDrop) && !isLock)
		{
			delay = downDelayFast;
		}
		else {
			delay = downDelay;
		}

		/* ----- 画方块 -----// */
		(*window).clear();

		pauseBtn.setButton("media/image/button/button.png", 900.0f, 10.0f, 2);
		pauseBtn.draw(font, 35, L"pause");

		(*window).draw(spriteBackGround); /* 渲染背景 */;

		//显示消除的行数
		displayText(window, font, to_wstring(line), 100, 512, 140, 1, 1, 1, Color(128, 128, 128));

		//场地上的方块
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
		//下落中的方块、next、hold
		for (int i = 0; i < 4; i++)
		{
			/*下落中的方块 */
			spriteBlock.setTextureRect(IntRect(blockIndex * 30, 0, 30, 30));
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
			holdBlock.setTextureRect(IntRect(hold * 30, 0, 30, 30));
			nextBlock1.setTextureRect(IntRect(next[0] * 30, 0, 30, 30));
			nextBlock2.setTextureRect(IntRect(next[1] * 30, 0, 30, 30));
			nextBlock3.setTextureRect(IntRect(next[2] * 30, 0, 30, 30));
			nextBlock4.setTextureRect(IntRect(next[3] * 30, 0, 30, 30));
			nextBlock5.setTextureRect(IntRect(next[4] * 30, 0, 30, 30));

			if (hold != 0) {
				holdBlock.setPosition((blocks[hold - 1][i][0] + 3) * 30, blocks[hold - 1][i][1] * 30);
			}
			nextBlock1.setPosition((blocks[next[0] - 1][i][0] + 3) * 30, blocks[next[0] - 1][i][1] * 30);
			nextBlock2.setPosition((blocks[next[1] - 1][i][0] + 3) * 30, blocks[next[1] - 1][i][1] * 30);
			nextBlock3.setPosition((blocks[next[2] - 1][i][0] + 3) * 30, blocks[next[2] - 1][i][1] * 30);
			nextBlock4.setPosition((blocks[next[3] - 1][i][0] + 3) * 30, blocks[next[3] - 1][i][1] * 30);
			nextBlock5.setPosition((blocks[next[4] - 1][i][0] + 3) * 30, blocks[next[4] - 1][i][1] * 30);

			if (hold == 1)holdBlock.move(142, 35);
			else if (hold == 4)holdBlock.move(172, 50);
			else
				holdBlock.move(157, 50);


			if (next[0] == 1)nextBlock1.move(582, 35);
			else if (next[0] == 4)nextBlock1.move(612, 50);
			else
				nextBlock1.move(597, 50);


			if (next[1] == 1)nextBlock2.move(582, 110);
			else if (next[1] == 4)nextBlock2.move(612, 125);
			else
				nextBlock2.move(597, 125);


			if (next[2] == 1)nextBlock3.move(582, 185);
			else if (next[2] == 4)nextBlock3.move(612, 200);
			else
				nextBlock3.move(597, 200);


			if (next[3] == 1)nextBlock4.move(582, 260);
			else if (next[3] == 4)nextBlock4.move(612, 275);
			else
				nextBlock4.move(597, 275);


			if (next[4] == 1)nextBlock5.move(582, 335);
			else if (next[4] == 4)nextBlock5.move(612, 350);
			else
				nextBlock5.move(597, 350);

			if (hold != 0) {
				(*window).draw(holdBlock);
			}

			(*window).draw(nextBlock1);
			(*window).draw(nextBlock2);
			(*window).draw(nextBlock3);
			(*window).draw(nextBlock4);
			(*window).draw(nextBlock5);
		}

		//ready...set...go
		if (readyClock.getTime() >= 0 && readyClock.getTime() <= 3000 && !readyFinish) {

			if (readyClock.getTime() < 1000) {
				displayText(window, font, L"ready...", 55, 512, 250, 1, 1, 1);
			}
			else if (readyClock.getTime() < 2000 && readyClock.getTime() >= 1000) {
				displayText(window, font, L"set...", 55, 512, 250, 1, 1, 1);
			}
			else if (readyClock.getTime() >= 2000) {
				displayText(window, font, L"go!", 55, 512, 250, 1, 1, 1);
			}
			window->display();
			continue;
		}
		else if (readyClock.getTime() > 3000 && !readyFinish) {
			Timer = 0;
			downDelayTimer.restart();
			gameTimer.restart();
			readyFinish = true;
			newBlock();                             /* 产生新方块 */
		}

		//显示时间
		{
			Timer = gameTimer.getTime();
			displayText(window, font,
				to_wstring(Timer / 60000) + L"\'" +
				(Timer / 10000 == 0 ? L"0" : L"") +
				to_wstring(Timer / 1000 % 60) + L"\"" +
				(Timer % 1000 / 100 == 0 ? L"0" : L"") +
				(Timer % 1000 / 10 == 0 ? L"0" : L"") +
				(Timer % 1000 == 0 ? L"0" : L"") +
				to_wstring(Timer % 1000), 20, 289, 400, 1, 1, 1);
		}

		(*window).display();
		refShadow();

		for (int i = 0; i < 4; i++)
		{
			if (curBlock[i].y == shadow[i].y)
			{
				downDelayTimer.restart();

				if (!isLock) {
					isLock = true;
					lockTimer.restart();
				}

				if (lockTimer.getTime() >= dropLockTime /*dropLockTime*/)
				{
					/* 要固化处理 */
					for (int j = 0; j < 4; j++)
					{
						table[int(curBlock[j].y)][int(curBlock[j].x)] = blockIndex;
					}

					isHold = false;

					newBlock();
					if (!check())
						return 0;
					downDelayTimer.restart();
					lockTimer.restart();
					isLock = false;
				}
			}
			else {
				isLock = false;
				lockTimer.restart();
			}
		}

		/*
		 * ----- 下降 -----
		 * 获取从clock被重启或被启动之后，到现在的时间
		 */
		if (downDelayTimer.getTime() >= delay && !isLock && check())
		{
			for (int i = 0; i < 4; i++)
			{
				bakBlock[i].x = curBlock[i].x;
				bakBlock[i].y = curBlock[i].y;

				++curBlock[i].y;
			}
			downDelayTimer.restart();
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

bool Game::move(int move)
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
bool Game::rotateBlock(int rota)
{
	Point	p;
	Point	tmpBlock[4];

	/* 开始旋转 */
	for (int i = 0; i < 4; i++)
	{
		tmpBlock[i] = curBlock[i];
	}

	/* 计算旋转中心 */
	if (blockIndex == 1)
	{
		p = curBlock[1];
		switch (rotate)
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
	else if (blockIndex == 6)
	{
		p = curBlock[1];
	}
	else if (blockIndex == 4)
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
		if (blockIndex == 1) {
			for (int j = 0; j < 4; j++) {
				curBlock[j].x += kickTable.I[(rota == 1 ? rotate : rotate + 4)][i][0];
				curBlock[j].y += kickTable.I[(rota == 1 ? rotate : rotate + 4)][i][1];
			}
		}
		else {
			for (int j = 0; j < 4; j++) {
				curBlock[j].x += kickTable.ZSJLT[(rota == 1 ? rotate : rotate + 4)][i][0];
				curBlock[j].y += kickTable.ZSJLT[(rota == 1 ? rotate : rotate + 4)][i][1];
			}
		}

		if (!check()) {
			for (int j = 0; j < 4; j++)
				curBlock[j] = bakBlock[j];
			continue;
		}
		else {
			if (rota == 1) {
				rotate = (rotate == 3 ? 0 : rotate + 1);
			}
			else if (rota == -1) {
				rotate = (rotate == 0 ? 3 : rotate - 1);
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

/* ----- 检查是否合法 ----- */
bool Game::check()
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

void Game::newbag(int count) {
	if (count == 7) {
		for (int i = 7; i < 14; i++) {
			int randomNum = 1 + rand() % (14 - i);
			int counting = 0;
			while (1) {
				if (nextUsed[counting] == true)
					randomNum--;

				if (randomNum == 0) {
					nextUsed[counting] = false;
					next[i] = counting + 1;
					break;
				}

				counting++;
			}
		}
	}
	else if (count == 14) {
		for (int i = 0; i < 7; i++) {
			int randomNum = 1 + rand() % (7 - i); /* 随机取1~7随机数 */
			int counting = 0;
			while (1) {
				if (nextUsed[counting] == true)
					randomNum--;

				if (randomNum == 0) {
					nextUsed[counting] = false;
					next[i] = counting + 1;
					break;
				}

				counting++;
			}
		}

		for (int i = 0; i < 7; i++) {
			nextUsed[i] = true;
		}

		for (int i = 7; i < 14; i++) {
			int randomNum = 1 + rand() % (14 - i); /* 随机取1~7随机数 */
			int counting = 0;
			while (1) {
				if (nextUsed[counting] == true)
					randomNum--;

				if (randomNum == 0) {
					nextUsed[counting] = false;
					next[i] = counting + 1;
					break;
				}

				counting++;
			}
		}
	}
}

/* ----- 生成新方块 ----- */
void Game::newBlock()
{
	/*
	 * 获取一个随机值（1~7）
	 * TODO:7bag出块方式
	 */

	int count = 0;
	for (int i = 0; i < 14; i++) {
		if (next[i]) {
			count++;
		}
	}

	for (int i = 0; i < 7; i++) {
		nextUsed[i] = true;
	}

	if (count == 0) {
		newbag(14);
	}
	else if (count == 7) {
		newbag(7);
	}

	blockIndex = next[0];
	for (int i = 0; i < 13; i++) {
		next[i] = next[i + 1];
	}
	next[13] = 0;

	for (int i = 0; i < 4; i++)
	{
		curBlock[i].x = blocks[blockIndex - 1][i][0] + 3;
		curBlock[i].y = blocks[blockIndex - 1][i][1];
	}

	rotate = 0;
}

void Game::swapHold()
{
	int tmp = blockIndex;
	blockIndex = hold;
	hold = tmp;

	for (int i = 0; i < 4; i++)
	{
		curBlock[i].x = blocks[blockIndex - 1][i][0] + 3;
		curBlock[i].y = blocks[blockIndex - 1][i][1];
	}
}

/* ----- 影子系统 -----// */
void Game::refShadow()
{
	bool	shadowIsLock = false;
	Point	tmp[4];

	for (int i = 0; i < 4; i++)
	{
		shadow[i].x = 0;
		shadow[i].y = 0;

		int	xx = curBlock[i].x - blocks[blockIndex - 1][i][0];
		int	yy = curBlock[i].y - blocks[blockIndex - 1][i][1];

		shadow[i].x = blocks[blockIndex - 1][i][0] + xx;
		shadow[i].y = blocks[blockIndex - 1][i][1] + yy;
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