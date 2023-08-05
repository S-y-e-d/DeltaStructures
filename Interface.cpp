#include "Interface.hpp"

ui::Button::Button(sf::RenderWindow &window, sf::Font &font) : window(window), font(font) {
    text.setFont(font);
    text.setFillColor(sf::Color::Black);
    text.setString("Press");
}

void ui::Button::setPosition(int x, int y) {
    rect.setPosition(x, y);
}
void ui::Button::setOutlineThickness(int thickness) {
    rect.setOutlineThickness(thickness);
}
void ui::Button::setSize(int width, int height) {
    rect.setSize(sf::Vector2f(width, height));
}
void ui::Button::setOutlineColor(sf::Color color) {
    rect.setOutlineColor(color);
}
void ui::Button::setFillColor(sf::Color color) {
    rect.setFillColor(color);
}

bool ui::Button::clicked() {
    if (pressed == true) {
        pressed = not pressed;
        return true;
    }
    return false;
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
    if (mouse_over == false) {
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
    }
    window.draw(rect);
    window.draw(text);
}

// ********************************************************************************************************
// ********************************************************************************************************

ui::Input::Input(sf::RenderWindow &window, sf::Font &font) : window(window), font(font) {
    text.setFont(font);
}

void ui::Input::setPosition(int x, int y) {
    
    rect.setPosition(x, y);
    text.setString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");       // Centering the text based on all possible inputs
    int y_offset = text.getPosition().y - text.getGlobalBounds().top;
    int y_pos = rect.getPosition().y + (rect.getGlobalBounds().height - text.getGlobalBounds().height) / 2 + y_offset;
    text.setPosition(x+padding, y_pos);
    text.setString("");
}

void ui::Input::setSize(int width, int height) {
    rect.setSize(sf::Vector2f(width, height));
    text.setString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");
    while (text.getGlobalBounds().height < height - 2 * padding) {
        text.setCharacterSize(text.getCharacterSize() + 1);
    }
    text.setString("");
}
void ui::Input::setOutlineThickness(int thickness) {
    rect.setOutlineThickness(thickness);
}
void ui::Input::setOutlineColor(sf::Color color) {
    rect.setOutlineColor(color);
}
void ui::Input::setFillColor(sf::Color color) {
    fill_color = color;
    rect.setFillColor(color);
}
void ui::Input::setTextColor(sf::Color color){
    text_color = color;
    text.setFillColor(color);
}


void ui::Input::iCursor() {
    i_cursor.setSize(sf::Vector2f(2, rect.getGlobalBounds().height-padding));
    i_cursor.setPosition(text.getPosition().x + text.getGlobalBounds().width + 2, rect.getPosition().y+padding/2);
    i_cursor.setFillColor(sf::Color::Transparent);
    while (in_focus) {
        i_cursor.setFillColor(sf::Color::Black);
        std::this_thread::sleep_for(0.5s);
        i_cursor.setFillColor(sf::Color::Transparent);
        std::this_thread::sleep_for(0.5s);
    }
}

std::string ui::Input::getString(){
    return text_string;
}

void ui::Input::setPlaceholder(std::string txt, sf::Color color){
    placeholder_color = color;
    placeholder_text = txt;
    text.setString(txt);
    text.setFillColor(color);
}

void ui::Input::handleClick(float mouseX, float mouseY) {
    if (mouseX == 0 and mouseY == 0) {
        return;
    }
    if (rect.getGlobalBounds().contains(mouseX, mouseY)) {
        if (in_focus == true) {
            return;
        }
        in_focus = true;
        rect.setFillColor(sf::Color::White);
        text.setFillColor(sf::Color::Black);
        if(text_string.empty()){
            text.setString("");
        }
        void (Input::*func)() = &Input::iCursor;
        std::thread(func, this).detach();
    } else {
        in_focus = false;
        i_cursor.setFillColor(sf::Color::Transparent);
        rect.setFillColor(fill_color);
        text.setFillColor(text_color);
        if(text_string.empty()){
            text.setString(placeholder_text);
            text.setFillColor(placeholder_color);
        }
    }
}

void ui::Input::handleKeyPress(char key) {
    if (not in_focus) {
        return;
    }
    if (key == '\0') {
        return;
    }
    if (key == '\b') {
        if (not text_string.empty()) {
            text_string.pop_back();
        }
    } else {
        text_string += key;
    }
    text.setString(text_string);
    i_cursor.setPosition(text.getPosition().x + text.getGlobalBounds().width + 2, rect.getPosition().y+padding/2);
}

void ui::Input::update(float mouseX, float mouseY, char key) {
    handleKeyPress(key);
    handleClick(mouseX, mouseY);

    window.draw(rect);
    window.draw(text);
    window.draw(i_cursor);
}