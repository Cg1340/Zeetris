#include "Button.h"

void Button::setTexture(string str) {
	buttonTexture.loadFromFile(str);
	buttonSprite.setTexture(buttonTexture);
}

void Button::draw() {
	isActive = true;//若要显示，则为活跃状态
	(*window).draw(buttonSprite);
}

void Button::setPosition(float x, float y) {
	buttonSprite.setPosition(x, y);
}

void Button::setScale(int scale) {
	buttonSprite.setScale(scale, scale);
}

bool Button::onClick(Event event) {

	if (!isActive) //如果按钮不是活跃状态（不显示），不响应
		return false;
	bool flag = false;

	FloatRect box = buttonSprite.getGlobalBounds();//获取按钮的有效点击范围
	if (Mouse::getPosition(*window).x >= box.left && Mouse::getPosition(*window).x <= (box.left + box.width) && Mouse::getPosition(*window).y >= box.top && Mouse::getPosition(*window).y <= (box.top + box.height)) {
		if (event.type == Event::MouseButtonReleased || event.type == Mouse::Left && PreBehRel)
		{
			PreBehRel = false;//要先按下再松开才返回true
			flag = true;
		}
		else
			flag = false;

		if (event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left)
		{
			this->buttonSprite.setColor(Color(125, 125, 0, 255));//按钮按下时的颜色和不透明度
			this->PreBehRel = true;
		}
		else
			this->buttonSprite.setColor(Color(125, 125, 0, 100));//鼠标指着按钮但未按下时的颜色和不透明度
	}
	else
		this->buttonSprite.setColor(Color(255, 255, 255, 255));//默认状态的按钮颜色
	return flag;
}

void Button::setFont(Font font) {
	buttonText.setFont(font);
}

void Button::setText(string text) {

}