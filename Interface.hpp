#ifndef Interface_hpp
#define Interface_hpp

#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <variant>
#include <vector>
#include <codecvt>
#include <locale>

namespace ui {
    using namespace std::chrono_literals;

    class Button : public sf::RectangleShape {
        sf::Text text;
        sf::RenderWindow &window;
        sf::Font &font;
        bool pressed = false;
        bool mouse_over = false;

    public:
        Button(sf::RenderWindow &window, sf::Font &font);

        void setPosition(int x, int y);
        void move(float x, float y);
        void setText(std::string txt, int font_size, sf::Color text_color = sf::Color::Black);
        void onHover(int change);

        bool mouseOver();  // return true if the mouse has been brought over, not when it's already been over.
        bool mouseOut();
        bool clicked();

        void update(float mouseX, float mouseY, char key);
    };

    class Input : public sf::RectangleShape{
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
        void move(int x, int y);
        void setSize(int width, int height);
        void setTextColor(sf::Color color);
        void setFillColor(sf::Color color);
        void setPlaceholder(std::string text, sf::Color color);

        std::string getString();

        void update(float mouseX, float mouseY, char key);
    };

    class Container : public sf::RectangleShape {
        sf::RenderWindow &window;
        sf::Font &font;

        std::map<std::string, Button*> buttons;         // Had to use pointers because SFML does not allow assignment of RenderWindow
        std::map<std::string, Input*> inputs;
        std::map<std::string, sf::Text*> labels;

        bool moving = false;
        int state = 0;      // have this to store information for the main function to use.

    public:
        enum ComponentType{
            BUTTON,
            INPUT,
            LABEL
        } type;

        
        Container(sf::RenderWindow &window, sf::Font &font);
        ~Container();
        
        void addComponent(ComponentType type, std::string id, int xr = 0, int yr = 0, int width = 0, int height = 0);
        Button& getButtonById(std::string id);
        Input& getInputById(std::string id);
        sf::Text& getLabelById(std::string id);

        int getState();
        void setState(int st);

        void setPosition(int x, int y);
        void setLabel(std::string id, std::string str, float left, float right, float top, float bottom);

        void move(float x, float y, float speed, bool thread);
        void move(float x, float y, float speed = 0);

        void update(float mouseX, float mouseY, char key);
    };
};

#endif