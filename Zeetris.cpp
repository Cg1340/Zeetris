#include <iostream>
#include <SFML/Graphics.hpp>            /* 处理图像 */
#include <SFML/Audio.hpp>               /* 处理声音 */
#include <time.h>                       /* 处理时间 */
#include <Windows.h>
#include <string>
#include "Button.h"
#include "Display.h"
#include "GameSetting.h"
#include "Game.h"
using namespace sf;
using namespace std::chrono;
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

int main()
{
	RenderWindow window(
		VideoMode(1024, 768),         /* 窗口分辨率 */
		"Zeetris");                    /* 窗口标题 */

	GameSetting gameSetting;

	Font font;
	font.loadFromFile("media/fonts/proportional.ttf");

	Music backgroundMusic;
	backgroundMusic.openFromFile(
		"media/music/other side - Lena Raine.wav");            /* 加载背景音乐 */
	backgroundMusic.setLoop(true); /* 设置重复播放音乐 */
	backgroundMusic.setVolume(gameSetting.volume);
	backgroundMusic.play();                 /* 播放音乐 */

	while (window.isOpen()) {
		Event event;

		Button playButton, settingButton, exitButton;
		Texture titleTexture;
		titleTexture.loadFromFile("media/image/title.png");
		Sprite title(titleTexture);

		while (1) {
			window.clear();

			title.setScale(0.5f, 0.5f);
			title.setPosition(400.0f, 400.0f);
			window.draw(title);

			playButton.window = &window;
			settingButton.window = &window;
			exitButton.window = &window;

			playButton.setButton("media/image/button/start.png", 0.0f, 100.0f, 3);
			settingButton.setButton("media/image/button/setting.png", 0.0f, 200.0f, 3);
			exitButton.setButton("media/image/button/exit.png", 0.0f, 300.0f, 3);

			playButton.draw(font, 80, L"开始");
			settingButton.draw(font, 80, L"设置");
			exitButton.draw(font, 80, L"退出");

			displayText(&window, font, L"操作说明：\nA/D 向左/向右移动\nS 软降\nO/P 逆时针/顺时针旋转\n空格 硬降\n左shift 暂存", 30, 0, 500, 1, 1, false);

			window.display();

			window.pollEvent(event);
			Vector2i mousePos = Mouse::getPosition(window);
			Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

			if (playButton.click(event, mousePos, mousePosF)) {
				break;
			}

			//设置
			if (settingButton.click(event, mousePos, mousePosF)) {

				SettingButton volume;
				Button backBtn;
				backBtn.window = &window;
				while (1) {
					window.clear();

					backBtn.setButton("media/image/button/button.png", 900.0f, 700.0f, 2);
					backBtn.draw(font, 40, L"back");

					volume.setButton(0, 0, font, gameSetting.volume);
					FloatRect box = volume.subButton.buttonSprite.getGlobalBounds();
					volume.window = &window;
					volume.draw(&window, font, 48, L"音量：" + to_wstring(gameSetting.volume));

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

		Game game;
		game.window = &window;
		game.startGame(font, gameSetting);
		window.clear();
	}

	return(0);
}