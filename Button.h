#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>            /* 处理图像 */
#include <string>
using namespace sf;
using namespace std;

class Button {
public:
	void setTexture(string str);
	void draw();
	void setPosition(float x, float y);
	void setScale(int scale);
	bool onClick(Event event);

	void setFont(Font font);
	void setText(string text);

	bool isActive;//按钮是否活跃状态
	bool PreBehRel;//松开之前是否按下
	RenderWindow* window;
	Sprite buttonSprite;
	Text buttonText;

private:
	Texture buttonTexture;
};