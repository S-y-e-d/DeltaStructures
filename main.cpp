#include "Panels.hpp"
#include "Interface.hpp"
#include "Structures.hpp"

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
    array.addBlock(1); array.addBlock(2); array.addBlock(3);
    structures::LinkedList list(window, font);
    structures::Tree tree(window, font);

    enum ActiveStructure {
        ARRAY,
        LIST,
        TREE,
        NONE
    } active_structure = NONE;

    ui::Container lpanel(window, font);
    lpanel.setSize(sf::Vector2f(450, window_size.y));
    lpanel.setFillColor(sf::Color(10, 10, 10, 255));
    lpanel.setOutlineColor(sf::Color::White);
    lpanel.setOutlineThickness(-2);
    lpanel.setPosition(-450 + 70, 0);       // panel peaking to show button (50) + 10 padding each side
    lpanel.setState(0);
    
    lpanel.addComponent(ui::Container::ComponentType::BUTTON, "open", 450 - 60, 10, 50, 50);

    lpanel.getButtonById("open").setOutlineThickness(-2);
    lpanel.getButtonById("open").setOutlineColor(sf::Color::White);
    lpanel.getButtonById("open").setFillColor(sf::Color::Transparent);
    lpanel.getButtonById("open").setText("≡", 18, sf::Color::White);

    lpanel.addComponent(ui::Container::ComponentType::LABEL, "lpanel");
    lpanel.setLabel("lpanel", "Structures", 50, 400, 100, 150);

    lpanel.addComponent(ui::Container::ComponentType::BUTTON, "array", 50, 200, lpanel.getSize().x - 100, 50);
    lpanel.getButtonById("array").setOutlineThickness(-2);
    lpanel.getButtonById("array").setOutlineColor(sf::Color::White);
    lpanel.getButtonById("array").setFillColor(sf::Color::Transparent);
    lpanel.getButtonById("array").setText("Array", 18, sf::Color::White);

    lpanel.addComponent(ui::Container::ComponentType::BUTTON, "list", 50, 300, lpanel.getSize().x - 100, 50);
    lpanel.getButtonById("list").setOutlineThickness(-2);
    lpanel.getButtonById("list").setOutlineColor(sf::Color::White);
    lpanel.getButtonById("list").setFillColor(sf::Color::Transparent);
    lpanel.getButtonById("list").setText("Linked List", 18, sf::Color::White);

    lpanel.addComponent(ui::Container::ComponentType::BUTTON, "tree", 50, 400, lpanel.getSize().x - 100, 50);
    lpanel.getButtonById("tree").setOutlineThickness(-2);
    lpanel.getButtonById("tree").setOutlineColor(sf::Color::White);
    lpanel.getButtonById("tree").setFillColor(sf::Color::Transparent);
    lpanel.getButtonById("tree").setText("Tree", 18, sf::Color::White);

    // delete this and references
    // bool temp;    

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

        lpanel.getButtonById("open").onHover(2);
        lpanel.getButtonById("array").onHover(2);
        lpanel.getButtonById("list").onHover(2);
        lpanel.getButtonById("tree").onHover(2);
        
        
        if(lpanel.getButtonById("open").clicked()){
            lpanel.getButtonById("open").setOutlineThickness(-2);
            
            if(lpanel.getState()== 0){
                lpanel.setState(1);
                lpanel.move(lpanel.getSize().x - 70, 0, 5);     // 70 being the button size
            }else if(lpanel.getState() == 1){
                lpanel.setState(0);
                lpanel.move(-lpanel.getSize().x + 70, 0, 5);
            }
        }

        if(lpanel.getButtonById("array").clicked()){
            lpanel.getButtonById("array").setOutlineThickness(-2);
            active_structure = ARRAY;
        }
        if(lpanel.getButtonById("list").clicked()){
            lpanel.getButtonById("list").setOutlineThickness(-2);
            active_structure = LIST;
        }
        if(lpanel.getButtonById("tree").clicked()){
            lpanel.getButtonById("tree").setOutlineThickness(-2);
            active_structure = TREE;
        }

        lpanel.update(mouse_click_x, mouse_click_y, key_pressed);
        // inp.update(mouse_click_x, mouse_click_y, key_pressed);
        window.display();
    }

    return 0;
}