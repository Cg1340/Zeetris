#include "Button.h"

void Button::setButton(string file, float posX, float posY, float scale) {
	buttonTexture.loadFromFile(file);
	buttonSprite.setTexture(buttonTexture);
	buttonSprite.setPosition(posX, posY);
	buttonSprite.setScale(scale, scale);
    this->scale = scale;
}

bool Button::click(Event event, Vector2i mousePos, Vector2f mousePosF) {

    bool flag = false;
    FloatRect box = buttonSprite.getGlobalBounds();//获取按钮的有效点击范围
    if (buttonSprite.getGlobalBounds().contains(mousePosF))
    {
        if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && PreBehRel)
        {
            this->PreBehRel = false;//要先按下再松开才返回true，置于为什么要判断先按下，后面会讲
            flag = true;
        }
        else
            flag = false;
        if (event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left)
        {
            this->buttonSprite.setScale(scale + 0.3, scale + 0.3);
            this->PreBehRel = true;
        }
        else
            this->buttonSprite.setScale(scale + 0.2, scale + 0.2);
    }
    else
        this->buttonSprite.setScale(scale, scale);
    return flag;
}

void Button::draw(Font font, int textSize, wstring textString) {
    window->draw(buttonSprite);
    FloatRect box =buttonSprite.getGlobalBounds();//获取按钮的范围
    displayText(window, font, textString, textSize, box.left, box.top, 1, 1, false);
}

void SettingButton::setButton(float posX, float posY, Font font, int value) {
    subButton.setButton("media/image/button/sub_button.png", posX, posY, 3);
    FloatRect box = subButton.buttonSprite.getGlobalBounds();
    addButton.setButton("media/image/button/add_button.png", posX + box.width, posY, 3);
}

void SettingButton::draw(RenderWindow* window, Font font,int textSize ,wstring textString) {
    window->draw(this->addButton.buttonSprite);
    window->draw(this->subButton.buttonSprite);
    FloatRect box = subButton.buttonSprite.getGlobalBounds();//获取按钮的范围
    displayText(window, font, textString, textSize, box.left + box.width * 2, box.top, 1, 1, false);
}

int SettingButton::click(Event event, Vector2i mousePos, Vector2f mousePosF) {
    if (subButton.click(event, mousePos, mousePosF)) {
        return -1;
    }
    if (addButton.click(event, mousePos, mousePosF)) {
        return 1;
    }
    return 0;
}