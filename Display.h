#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>            /* 处理图像 */
#include <string>
using namespace sf;
using namespace std;

//显示图像
void displaySprite(RenderWindow* window, string filePath, float posX = 0, float posY = 0, float scaleX = 1, float scaleY = 1);

//显示文字
void displayText(RenderWindow* window, Font font, wstring textString, int textSize = 30, float posX = 0, float posY = 0, float scaleX = 1, float scaleY = 1);