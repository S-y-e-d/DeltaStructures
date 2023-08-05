#ifndef Interface_hpp
#define Interface_hpp

#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <variant>
#include <vector>

namespace ui {
    using namespace std::chrono_literals;

    class Button {
        sf::RectangleShape rect;
        sf::Text text;
        sf::RenderWindow &window;
        sf::Font &font;
        bool pressed = false;
        bool mouse_over = false;

    public:
        Button(sf::RenderWindow &window, sf::Font &font);

        void setPosition(int x, int y);
        void setText(std::string txt, int font_size, sf::Color text_color = sf::Color::Black);

        bool mouseOver();  // return true if the mouse has been brought over, not when it's already been over.
        bool mouseOut();
        bool clicked();
        void setOutlineThickness(int thickness);
        void setSize(int width, int height);
        void setOutlineColor(sf::Color color);
        void setFillColor(sf::Color color);

        void update(float mouseX, float mouseY, char key);
    };

    class Input{
        sf::RectangleShape rect;
        sf::Text text;
        std::string text_string;
        sf::RenderWindow &window;
        sf::Font &font;

        sf::Color fill_color;
        sf::Color text_color;
        
        sf::Color placeholder_color;
        std::string placeholder_text;

        sf::RectangleShape i_cursor;

        int padding = 10;
        bool in_focus = false;

        void handleKeyPress(char key);
        void handleClick(float mouseX, float mouseY);
        void iCursor();
    
    public:
        Input(sf::RenderWindow &window, sf::Font &font);
        
        void setPosition(int x, int y);
        void setSize(int width, int height);
        void setOutlineThickness(int thickness);
        void setOutlineColor(sf::Color color);
        void setFillColor(sf::Color color);
        void setTextColor(sf::Color color);
        void setPlaceholder(std::string text, sf::Color color);

        std::string getString();

        void update(float mouseX, float mouseY, char key);
    };

    class Container {
        sf::RectangleShape rect;
        sf::RenderWindow &window;
        sf::Font &font;

    public:
        Container(sf::RenderWindow &window, sf::Font &font);
        
    };
};

#endif