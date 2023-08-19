#include "Interface.hpp"
#include "Structures.hpp"
#include "design.hpp"

// refactor panels to use threads and use move instead of set positions.

int main() {
    srand(time(0));

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8.0;
    sf::Vector2i window_size(1920, 1080);

    sf::RenderWindow window(sf::VideoMode(window_size.x, window_size.y), "Delta-Structures");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("./assets/ARIAL.TTF")) {
        std::cout << "Error: Couldn't load font\n";
        return 0;
    }

    structures::Array array(window, font);
    array.addBlock(1);
    array.addBlock(2);
    array.addBlock(3);
    structures::LinkedList list(window, font);
    structures::Tree tree(window, font);
    active_structure = NONE;

    ui::Container lpanel(window, font);
    /*
        Buttons:
            open, array, list, tree
        Labels:
            lpanel
    */
   ui::Container rpanel(window, font);
    design(window_size, lpanel, rpanel);
  
    // delete this and references
    // bool temp;
    // ui::Button temp(window, font);
    // temp.setSize(sf::Vector2f(100, 100));
    // temp.setPosition(70, 70);
    // temp.setOutlineColor(sf::Color::White);
    // temp.setOutlineThickness(-2);
    // temp.onHover(2);
    // temp.setText("Test", 32, sf::Color::White);
    // temp.setFillColor(sf::Color::Transparent);
    // temp.hidden = true;

    while (window.isOpen()) {
        sf::Event event;
        int mouse_click_x = 0, mouse_click_y = 0;
        char key_pressed = '\0';
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::TextEntered) {
                key_pressed = event.text.unicode;
            }
            // Detecting keyboard input
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                window.close();
            }
            // Triggering test events

            if (event.type == sf::Event::KeyReleased) {
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Convert the position of the pixel to the relative coordinate of the original window
                    mouse_click_x = worldPos.x;
                    mouse_click_y = worldPos.y;
                } else {
                    // Using negative to show right click. Surely nothing can go wrong.
                    mouse_click_x = -worldPos.x;
                    mouse_click_y = -worldPos.y;
                }
            }
        }
        window.clear();

        switch (active_structure) {
        case ARRAY:
            array.update(mouse_click_x, mouse_click_y, key_pressed);
            break;
        case LIST:
            list.update(mouse_click_x, mouse_click_y, key_pressed);
            break;
        case TREE:
            tree.update(mouse_click_x, mouse_click_y, key_pressed);
            break;
        default:
            break;
        }


        // Left panel config
        if (lpanel.getButtonById("open").clicked()) {
            lpanel.getButtonById("open").setOutlineThickness(-2);

            if (lpanel.getState() == 0) {
                lpanel.setState(1);
                lpanel.move(lpanel.getSize().x - 70, 0, 5);  // 70 being the button size
            } else if (lpanel.getState() == 1) {
                lpanel.setState(0);
                lpanel.move(-lpanel.getSize().x + 70, 0, 5);
            }
        }
        if (lpanel.getButtonById("array").clicked()) {
            lpanel.getButtonById("array").setOutlineThickness(-2);
            active_structure = ARRAY;
            toggleButtons(rpanel);
        }
        if (lpanel.getButtonById("list").clicked()) {
            lpanel.getButtonById("list").setOutlineThickness(-2);
            active_structure = LIST;
            toggleButtons(rpanel);
        }
        if (lpanel.getButtonById("tree").clicked()) {
            lpanel.getButtonById("tree").setOutlineThickness(-2);
            active_structure = TREE;
            toggleButtons(rpanel);
        }

        // Right panel config
        if (rpanel.getButtonById("open").clicked()) {
            rpanel.getButtonById("open").setOutlineThickness(-2);

            if (rpanel.getState() == 0) {
                rpanel.setState(1);
                rpanel.move(-rpanel.getSize().x + 70, 0, 5);
            } else if (rpanel.getState() == 1) {
                rpanel.setState(0);
                rpanel.move(rpanel.getSize().x - 70, 0, 5);
            }
        }

        lpanel.update(mouse_click_x, mouse_click_y, key_pressed);
        rpanel.update(mouse_click_x, mouse_click_y, key_pressed);

        window.display();
    }

    return 0;
}