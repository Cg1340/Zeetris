#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>            /* 处理图像 */
#include <string>
using namespace sf;
using namespace std;

//显示图像
void displaySprite(RenderWindow* window, string filePath, float posX, float posY, float scaleX, float scaleY);

//显示文字(int lo == 0 居左  == 1 居中  == 2 居右)
void displayText(RenderWindow* window, Font font, wstring textString, int textSize, float posX, float posY);
void displayText(RenderWindow* window, Font font, wstring textString, int textSize, float posX, float posY, int lo);
void displayText(RenderWindow* window, Font font, wstring textString, int textSize, float posX, float posY, float scaleX, float scaleY, int lo);
void displayText(RenderWindow* window, Font font, wstring textString, int textSize, float posX, float posY, float scaleX, float scaleY, int lo, Color color);