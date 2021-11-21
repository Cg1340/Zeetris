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

//ÏÔÊ¾ÎÄ×Ö(int lo == 0 ¾Ó×ó  == 1 ¾ÓÖÐ  == 2 ¾ÓÓÒ)
void displayText(RenderWindow* window, Font font, wstring textString, int textSize, float posX, float posY) {
	Text text;
	text.setFont(font);
	text.setString(textString);
	text.setCharacterSize(textSize);
	FloatRect box = text.getGlobalBounds();
	text.setPosition(posX, posY);
	text.setFillColor(Color::White);
	window->draw(text);
}

void displayText(RenderWindow* window, Font font, wstring textString, int textSize, float posX, float posY, int lo) {
	Text text;
	text.setFont(font);
	text.setString(textString);
	text.setCharacterSize(textSize);
	FloatRect box = text.getGlobalBounds();
	text.setPosition(posX - (box.width / 2 * lo), posY);
	text.setFillColor(Color::White);
	window->draw(text);
}

void displayText(RenderWindow* window, Font font, wstring textString, int textSize, float posX, float posY, float scaleX, float scaleY, int lo) {
	Text text;
	text.setFont(font);
	text.setString(textString);
	text.setCharacterSize(textSize);
	FloatRect box = text.getGlobalBounds();
	text.setPosition(posX - (box.width / 2 * lo), posY);
	text.setScale(scaleX, scaleY);
	text.setFillColor(Color::White);
	window->draw(text);
}

void displayText(RenderWindow* window, Font font, wstring textString, int textSize, float posX, float posY, float scaleX, float scaleY, int lo, Color color) {
	Text text;
	text.setFont(font);
	text.setString(textString);
	text.setCharacterSize(textSize);
	FloatRect box = text.getGlobalBounds();
	text.setPosition(posX - (box.width / 2 * lo), posY);
	text.setScale(scaleX, scaleY);
	text.setFillColor(color);
	window->draw(text);
}