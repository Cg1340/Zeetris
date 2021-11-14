#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>            /* ´¦ÀíÍ¼Ïñ */
#include <string>
#include "Display.h"
using namespace sf;
using namespace std;

class Button {
public:
	void setButton(string file, float posX, float posY, int scale);
	bool click(Event event, Vector2i mousePos, Vector2f mousePosF);
	void draw(Font font, wstring textString);
	bool rel = false;
	RenderWindow* window;
	Sprite buttonSprite;

private:
	Texture buttonTexture;
};

class SettingButton {
public:


	void setButton(float posX, float posY, Font font, int value);
	void draw(RenderWindow* window, Font font, wstring textString);
	int click(Event event, Vector2i mousePos, Vector2f mousePosF);

	RenderWindow *window;

	Button addButton;
	Button subButton;
};