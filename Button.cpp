#include "Button.h"

void Button::setButton(string file, float posX, float posY, int scale) {
	buttonTexture.loadFromFile(file);
	buttonSprite.setTexture(buttonTexture);
	buttonSprite.setPosition(posX, posY);
	buttonSprite.setScale(scale, scale);
}

bool Button::click(Event event, Vector2i mousePos, Vector2f mousePosF) {

    if (event.type == Event::MouseMoved) {
        if (buttonSprite.getGlobalBounds().contains(mousePosF))
        {
            buttonSprite.setColor(Color(255, 255, 255, 255));
        }
        else
        {
            buttonSprite.setColor(Color(255, 255, 255));
        }
    }

    if (event.type == Event::MouseButtonPressed && !rel) {
        if (buttonSprite.getGlobalBounds().contains(mousePosF))
        {
            rel = true;
            return true;
        }
    }
    else if (!(event.type == Event::MouseButtonPressed) && rel) {
        if (buttonSprite.getGlobalBounds().contains(mousePosF)) {
            rel = false;
        }
    }
    return false;
}

void Button::draw(Font font, wstring textString) {
    window->draw(buttonSprite);
    FloatRect box =buttonSprite.getGlobalBounds();//获取按钮的范围
    displayText(window, font, textString, 80, box.left, box.top, 1, 1);
}

void SettingButton::setButton(float posX, float posY, Font font, int value) {
    subButton.setButton("image/button/sub_button.png", posX, posY, 3);
    FloatRect box = subButton.buttonSprite.getGlobalBounds();
    addButton.setButton("image/button/add_button.png", posX + box.width, posY, 3);
}

void SettingButton::draw(RenderWindow* window, Font font, wstring textString) {
    window->draw(this->addButton.buttonSprite);
    window->draw(this->subButton.buttonSprite);
    FloatRect box = subButton.buttonSprite.getGlobalBounds();//获取按钮的范围
    displayText(window, font, textString, 48, box.left + box.width * 2, box.top, 1, 1);
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