#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>            /* ´¦ÀíÍ¼Ïñ */
using namespace sf;

class GameSetting {
public:
	int ARR = 0;//ms
	int DAS = 0;//ms

	Keyboard::Key moveLeft = Keyboard::A;
	Keyboard::Key moveRight = Keyboard::D;
	Keyboard::Key rotateLeft = Keyboard::O;
	Keyboard::Key rotateRight = Keyboard::P;
	Keyboard::Key softDrop = Keyboard::S;
	Keyboard::Key hardDrop = Keyboard::Space;
	Keyboard::Key hold = Keyboard::LShift;
	Keyboard::Key restart = Keyboard::Tab;

	int volume = 100;//0~100
};