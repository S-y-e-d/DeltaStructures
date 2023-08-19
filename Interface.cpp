#include "Interface.hpp"

ui::Button::Button(sf::RenderWindow& window, sf::Font& font) : window(window), font(font) {
    text.setFont(font);
    text.setFillColor(sf::Color::Black);
    text.setString("Press");
}

void ui::Button::setPosition(int x, int y) {
    sf::RectangleShape::setPosition(x, y);
    text.move(sf::Vector2f(x, y) - getPosition());
}

void ui::Button::move(float x, float y) {
    sf::RectangleShape::move(x, y);
    text.move(x, y);
}

bool ui::Button::clicked() {
    if (pressed == true) {
        pressed = not pressed;
        return true;
    }
    return false;
}

void ui::Button::setText(std::string txt, int font_size, sf::Color text_color) {
    // convert the string to wstring in order to use non ascii characters.
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
    std::wstring wtxt = converter.from_bytes(txt);

    text.setString(wtxt);
    text.setCharacterSize(font_size);
    text.setFillColor(text_color);
    int x_pos = getPosition().x + (getGlobalBounds().width - text.getGlobalBounds().width) / 2;

    int y_offset = text.getPosition().y - text.getGlobalBounds().top;
    int y_pos = getPosition().y + (getGlobalBounds().height - text.getGlobalBounds().height) / 2 + y_offset;

    text.setPosition(x_pos, y_pos);
}

bool ui::Button::mouseOver() {
    if (mouse_over == true) {
        return false;
    }
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

    if (getGlobalBounds().contains(worldPos)) {
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

    if (not getGlobalBounds().contains(worldPos)) {
        mouse_over = false;
    }
    return not mouse_over;
}

void ui::Button::onHover(int change) {
    static int ch = 0;
    if(change != 0){
        ch = change;
    }
    if (mouseOver()) {
        setOutlineThickness(ch);
    }
    if (mouseOut()) {
        setOutlineThickness(-ch);
    }
}

void ui::Button::update(float mouseX, float mouseY, char key) {
    if(hidden){
        return;
    }
    onHover();
    if (getGlobalBounds().contains(mouseX, mouseY)) {
        pressed = true;
    }
    window.draw(*this);
    window.draw(text);
}

// ********************************************************************************************************
// ********************************************************************************************************

ui::Input::Input(sf::RenderWindow& window, sf::Font& font) : window(window), font(font) {
    text.setFont(font);
}

void ui::Input::setPosition(int x, int y) {
    sf::RectangleShape::setPosition(x, y);
    text.setString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");  // Centering the text based on all possible inputs
    int y_offset = text.getPosition().y - text.getGlobalBounds().top;
    int y_pos = getPosition().y + (getGlobalBounds().height - text.getGlobalBounds().height) / 2 + y_offset;
    text.setPosition(x + padding, y_pos);
    text.setString("");
}
void ui::Input::move(int x, int y) {
    sf::RectangleShape::move(x, y);
    text.move(x, y);
    i_cursor.move(x, y);
}
void ui::Input::setSize(int width, int height) {
    sf::RectangleShape::setSize(sf::Vector2f(width, height));
    text.setString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");
    while (text.getGlobalBounds().height < height - 2 * padding) {
        text.setCharacterSize(text.getCharacterSize() + 1);
    }
    text.setString("");
}
void ui::Input::setFillColor(sf::Color color) {
    sf::RectangleShape::setFillColor(color);
    fill_color = color;
}
void ui::Input::setTextColor(sf::Color color) {
    text_color = color;
    text.setFillColor(color);
}

void ui::Input::iCursor() {
    i_cursor.setSize(sf::Vector2f(2, getGlobalBounds().height - padding));
    i_cursor.setPosition(text.getPosition().x + text.getGlobalBounds().width + 2, getPosition().y + padding / 2);
    i_cursor.setFillColor(sf::Color::Transparent);
    while (in_focus) {
        i_cursor.setFillColor(sf::Color::Black);
        std::this_thread::sleep_for(0.5s);
        i_cursor.setFillColor(sf::Color::Transparent);
        std::this_thread::sleep_for(0.5s);
    }
}

std::string ui::Input::getString() {
    return text_string;
}

void ui::Input::setPlaceholder(std::string txt, sf::Color color) {
    placeholder_color = color;
    placeholder_text = txt;
    text.setString(txt);
    text.setFillColor(color);
}

void ui::Input::handleClick(float mouseX, float mouseY) {
    if (mouseX == 0 and mouseY == 0) {
        return;
    }
    if (getGlobalBounds().contains(mouseX, mouseY)) {
        if (in_focus == true) {
            return;
        }
        in_focus = true;
        setFillColor(sf::Color::White);
        text.setFillColor(sf::Color::Black);
        if (text_string.empty()) {
            text.setString("");
        }
        void (Input::*func)() = &Input::iCursor;
        std::thread(func, this).detach();
    } else {
        in_focus = false;
        i_cursor.setFillColor(sf::Color::Transparent);
        setFillColor(fill_color);
        text.setFillColor(text_color);
        if (text_string.empty()) {
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
    i_cursor.setPosition(text.getPosition().x + text.getGlobalBounds().width + 2, getPosition().y + padding / 2);
}

void ui::Input::update(float mouseX, float mouseY, char key) {
    handleKeyPress(key);
    handleClick(mouseX, mouseY);

    window.draw(*this);
    window.draw(text);
    window.draw(i_cursor);
}

// *****************************************************************************************************************
// *****************************************************************************************************************

ui::Container::Container(sf::RenderWindow& window, sf::Font& font) : window(window), font(font) {
}

void ui::Container::addComponent(ComponentType type, std::string id, int xr, int yr, int width, int height) {
    int x = getPosition().x + xr;  // xr and yr are relative positions to the container itself
    int y = getPosition().y + yr;
    switch (type) {
    case BUTTON:
        buttons[id] = new Button(window, font);
        buttons[id]->setPosition(x, y);
        buttons[id]->setSize(sf::Vector2f(width, height));
        break;
    case INPUT:
        inputs[id] = new Input(window, font);
        inputs[id]->setPosition(x, y);
        inputs[id]->setSize(width, height);
        break;
    case LABEL:
        labels[id] = new sf::Text();
        labels[id]->setFont(font);
        break;
    }
}

void ui::Container::setLabel(std::string id, std::string str, float left, float right, float top, float bottom) {
    if (labels.find(id) == labels.end()) {
        throw std::range_error("Error: " + id + " does not exist.");
    }
    sf::Text& txt = *labels[id];
    txt.setString(str);
    sf::Rect bounds = txt.getGlobalBounds();
    
    float x1 = getPosition().x + left;
    float x2 = getPosition().x + right;
    
    float x_pos = left + (right - left - bounds.width) / 2;
    float y_pos = top + (bottom - top - bounds.height) / 2;

    txt.setPosition(getPosition() + sf::Vector2f(x_pos, y_pos));
}

ui::Button& ui::Container::getButtonById(std::string id) {
    if (buttons.find(id) == buttons.end()) {
        throw std::range_error("Error: " + id + " does not exist.");
    }
    return *buttons[id];
}
ui::Input& ui::Container::getInputById(std::string id) {
    if (inputs.find(id) == inputs.end()) {
        throw std::range_error("Error: " + id + " does not exist.");
    }
    return *inputs[id];
}
sf::Text& ui::Container::getLabelById(std::string id) {
    if (labels.find(id) == labels.end()) {
        throw std::range_error("Error: " + id + " does not exist.");
    }
    return *labels[id];
}
void ui::Container::setPosition(int x, int y) {
    int xr = x - getPosition().x;  // xr and yr are the relative displacement (destination - start)
    int yr = y - getPosition().y;
    sf::RectangleShape::setPosition(x, y);
    for (auto& [id, button] : buttons) {
        button->move(xr, yr);
    }
    for (auto& [id, input] : inputs) {
        input->move(xr, yr);
    }
    for (auto& [id, label] : labels) {
        label->move(xr, yr);
    }
}

void ui::Container::move(float x, float y, float speed, bool thread) {
    moving = true;

    sf::Vector2f velocity = {x / 100 * speed, y / 100 * speed};
    for (int i = 0; i < 100 / speed; i++) {
        sf::RectangleShape::move(velocity.x, velocity.y);
        for (auto& [id, button] : buttons) {
            button->move(velocity.x, velocity.y);
        }
        for (auto& [id, input] : inputs) {
            input->move(velocity.x, velocity.y);
        }
        for (auto& [id, label] : labels) {
            label->move(velocity.x, velocity.y);
        }
        std::this_thread::sleep_for(10ms);
    }

    moving = false;
}

void ui::Container::move(float x, float y, float speed) {  // default speed 0 = instant
    if (speed > 0) {
        if (moving) {
            return;
        }
        void (ui::Container::*func)(float, float, float, bool) = &ui::Container::move;
        std::thread(func, this, x, y, speed, true).detach();
        return;
    }
    sf::RectangleShape::move(x, y);
    for (auto& [id, button] : buttons) {
        button->move(x, y);
    }
    for (auto& [id, input] : inputs) {
        input->move(x, y);
    }
    for (auto& [id, label] : labels) {
        label->move(x, y);
    }
}
void ui::Container::setState(int st) {
    if (not moving) {
        state = st;
    }
}
int ui::Container::getState() {
    return state;
}

void ui::Container::update(float mouseX, float mouseY, char key) {
    window.draw(*this);
    for (auto& [id, button] : buttons) {
        button->update(mouseX, mouseY, key);
    }
    for (auto& [id, input] : inputs) {
        input->update(mouseX, mouseY, key);
    }
    for (auto& [id, label] : labels) {
        window.draw(*label);
    }
}

ui::Container::~Container() {
    for (auto& [id, button] : buttons) {
        delete button;
    }
    for (auto& [id, input] : inputs) {
        delete input;
    }
    for (auto& [id, label] : labels) {
        delete label;
    }
}