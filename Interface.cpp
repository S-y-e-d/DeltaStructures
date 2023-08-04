#include "Interface.hpp"

ui::Button::Button(sf::RenderWindow &window, sf::Font &font) : window(window), font(font) {
    text.setFont(font);
    text.setFillColor(sf::Color::Black);
    text.setString("Press");
}

void ui::Button::setPosition(int x, int y) {
    rect.setPosition(x, y);
}

bool ui::Button::clicked() {
    pressed = not pressed;  // If the button has been pressed, reset it when checked because handling will be in main function
    return !pressed;
}

sf::RectangleShape& ui::Button::getRect(){
    return rect;
}

void ui::Button::setText(std::string txt, int font_size, sf::Color text_color) {
    text.setString(txt);
    text.setCharacterSize(font_size);
    text.setFillColor(text_color);
    int x_pos = rect.getPosition().x + (rect.getGlobalBounds().width - text.getGlobalBounds().width) / 2;

    int y_offset = text.getPosition().y - text.getGlobalBounds().top;
    int y_pos = rect.getPosition().y + (rect.getGlobalBounds().height - text.getGlobalBounds().height) / 2 + y_offset;

    text.setPosition(x_pos, y_pos);
}

bool ui::Button::mouseOver() {
    if (mouse_over == true) {
        return false;
    }
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

    if (rect.getGlobalBounds().contains(worldPos)) {
        mouse_over = true;
    }
    return mouse_over;
}

bool ui::Button::mouseOut() {
    if(mouse_over == false) {
        return false;
    }
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

    if (not rect.getGlobalBounds().contains(worldPos)) {
        mouse_over = false;
    }
    return not mouse_over;
}

void ui::Button::update(float mouseX, float mouseY, char key) {
    if (rect.getGlobalBounds().contains(mouseX, mouseY)) {
        pressed = true;
        std::cout << "Clicked\n";
    }
    window.draw(rect);
    window.draw(text);
}