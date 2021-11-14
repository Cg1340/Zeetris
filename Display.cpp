#include "Display.h"

//ÏÔÊ¾Í¼Ïñ
void displaySprite(RenderWindow* window, string filePath, float posX, float posY, float scaleX, float scaleY) {
	Texture texture;
	texture.loadFromFile(filePath);
	Sprite sprite(texture);
	sprite.setPosition(posX, posY);
	sprite.setScale(scaleX, scaleY);
	window->draw(sprite);
}

//ÏÔÊ¾ÎÄ×Ö
void displayText(RenderWindow* window, Font font, wstring textString,int textSize ,float posX, float posY, float scaleX, float scaleY) {
	Text text;
	text.setFont(font);
	text.setString(textString);
	text.setCharacterSize(textSize);
	text.setPosition(posX, posY);
	text.setScale(scaleX, scaleY);
	text.setFillColor(Color::White);
	window->draw(text);
}