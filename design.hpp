#include "Interface.hpp"
#include "Structures.hpp"

enum ActiveStructure {
    ARRAY,
    LIST,
    TREE,
    NONE
} active_structure;

void design(sf::Vector2i window_size, ui::Container &lpanel, ui::Container &rpanel) {
    // Left side panel for selecting the structure

    lpanel.setSize(sf::Vector2f(450, window_size.y));
    lpanel.setFillColor(sf::Color(10, 10, 10, 255));
    lpanel.setOutlineColor(sf::Color::White);
    lpanel.setOutlineThickness(-2);
    lpanel.setPosition(-450 + 70, 0);  // panel peaking to show button (50) + 10 padding each side
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

    lpanel.getButtonById("open").onHover(2);
    lpanel.getButtonById("array").onHover(2);
    lpanel.getButtonById("list").onHover(2);
    lpanel.getButtonById("tree").onHover(2);

    // Right side panel for selecting the operation

    rpanel.setSize(sf::Vector2f(450, window_size.y));
    rpanel.setFillColor(sf::Color(10, 10, 10, 255));
    rpanel.setOutlineColor(sf::Color::White);
    rpanel.setOutlineThickness(-2);
    rpanel.setPosition(window_size.x - 70, 0);  // panel peaking to show button (50) + 10 padding each side
    rpanel.setState(0);

    rpanel.addComponent(ui::Container::ComponentType::BUTTON, "open", 10, 10, 50, 50);

    rpanel.getButtonById("open").setOutlineThickness(-2);
    rpanel.getButtonById("open").setOutlineColor(sf::Color::White);
    rpanel.getButtonById("open").setFillColor(sf::Color::Transparent);
    rpanel.getButtonById("open").setText("≡", 18, sf::Color::White);
    rpanel.getButtonById("open").onHover(2);

    rpanel.addComponent(ui::Container::ComponentType::LABEL, "rpanel");
    rpanel.setLabel("rpanel", "Algorithms", 50, 400, 100, 150);

    // Buttons for when array is active.
    rpanel.addComponent(ui::Container::ComponentType::BUTTON, "selSort", 50, 200, rpanel.getSize().x - 100, 50);
    rpanel.getButtonById("selSort").setOutlineThickness(-2);
    rpanel.getButtonById("selSort").setOutlineColor(sf::Color::White);
    rpanel.getButtonById("selSort").setFillColor(sf::Color::Transparent);
    rpanel.getButtonById("selSort").setText("Selection Sort", 18, sf::Color::White);
    rpanel.getButtonById("selSort").onHover(2);
    rpanel.getButtonById("selSort").hidden = true;

    rpanel.addComponent(ui::Container::ComponentType::BUTTON, "bubSort", 50, 300, rpanel.getSize().x - 100, 50);
    rpanel.getButtonById("bubSort").setOutlineThickness(-2);
    rpanel.getButtonById("bubSort").setOutlineColor(sf::Color::White);
    rpanel.getButtonById("bubSort").setFillColor(sf::Color::Transparent);
    rpanel.getButtonById("bubSort").setText("Bubble Sort", 18, sf::Color::White);
    rpanel.getButtonById("bubSort").onHover(2);
    rpanel.getButtonById("bubSort").hidden = true;

    rpanel.addComponent(ui::Container::ComponentType::BUTTON, "insSort", 50, 400, rpanel.getSize().x - 100, 50);
    rpanel.getButtonById("insSort").setOutlineThickness(-2);
    rpanel.getButtonById("insSort").setOutlineColor(sf::Color::White);
    rpanel.getButtonById("insSort").setFillColor(sf::Color::Transparent);
    rpanel.getButtonById("insSort").setText("Insertion Sort", 18, sf::Color::White);
    rpanel.getButtonById("insSort").onHover(2);
    rpanel.getButtonById("insSort").hidden = true;

    rpanel.addComponent(ui::Container::ComponentType::BUTTON, "merSort", 50, 500, rpanel.getSize().x - 100, 50);
    rpanel.getButtonById("merSort").setOutlineThickness(-2);
    rpanel.getButtonById("merSort").setOutlineColor(sf::Color::White);
    rpanel.getButtonById("merSort").setFillColor(sf::Color::Transparent);
    rpanel.getButtonById("merSort").setText("Merge Sort", 18, sf::Color::White);
    rpanel.getButtonById("merSort").onHover(2);
    rpanel.getButtonById("merSort").hidden = true;

    rpanel.addComponent(ui::Container::ComponentType::BUTTON, "arrInsert", 50, 600, rpanel.getSize().x - 100, 50);
    rpanel.getButtonById("arrInsert").setOutlineThickness(-2);
    rpanel.getButtonById("arrInsert").setOutlineColor(sf::Color::White);
    rpanel.getButtonById("arrInsert").setFillColor(sf::Color::Transparent);
    rpanel.getButtonById("arrInsert").setText("Insert at Index", 18, sf::Color::White);
    rpanel.getButtonById("arrInsert").onHover(2);
    rpanel.getButtonById("arrInsert").hidden = true;
    // #################
    auto ins_bt = rpanel.getButtonById("arrInsert").getGlobalBounds();
    rpanel.addComponent(ui::Container::ComponentType::INPUT, "arrInsertIdx", 50 + ins_bt.width - 50, 600, 50, 50);
    rpanel.getInputById("arrInsertIdx").setFillColor(sf::Color(20, 20, 20));
    rpanel.getInputById("arrInsertIdx").setTextColor(sf::Color::White);
    rpanel.getInputById("arrInsertIdx").setPlaceholder("0", sf::Color(127, 127, 127));
    rpanel.getInputById("arrInsertIdx").setOutlineThickness(-2);
    rpanel.getInputById("arrInsertIdx").setOutlineColor(sf::Color::White);
    rpanel.getInputById("arrInsertIdx").numeric = true;
    rpanel.getInputById("arrInsertIdx").hidden = true;

    rpanel.addComponent(ui::Container::ComponentType::BUTTON, "arrDelete", 50, 700, rpanel.getSize().x - 100, 50);
    rpanel.getButtonById("arrDelete").setOutlineThickness(-2);
    rpanel.getButtonById("arrDelete").setOutlineColor(sf::Color::White);
    rpanel.getButtonById("arrDelete").setFillColor(sf::Color::Transparent);
    rpanel.getButtonById("arrDelete").setText("Delete at Index", 18, sf::Color::White);
    rpanel.getButtonById("arrDelete").onHover(2);
    rpanel.getButtonById("arrDelete").hidden = true;

    // Buttons for when linked list is active
    rpanel.addComponent(ui::Container::ComponentType::BUTTON, "listInsert", 50, 200, rpanel.getSize().x - 100, 50);
    rpanel.getButtonById("listInsert").setOutlineThickness(-2);
    rpanel.getButtonById("listInsert").setOutlineColor(sf::Color::White);
    rpanel.getButtonById("listInsert").setFillColor(sf::Color::Transparent);
    rpanel.getButtonById("listInsert").setText("Insert after Index", 18, sf::Color::White);
    rpanel.getButtonById("listInsert").onHover(2);
    rpanel.getButtonById("listInsert").hidden = true;

    rpanel.addComponent(ui::Container::ComponentType::BUTTON, "listDelete", 50, 300, rpanel.getSize().x - 100, 50);
    rpanel.getButtonById("listDelete").setOutlineThickness(-2);
    rpanel.getButtonById("listDelete").setOutlineColor(sf::Color::White);
    rpanel.getButtonById("listDelete").setFillColor(sf::Color::Transparent);
    rpanel.getButtonById("listDelete").setText("Delete at Index", 18, sf::Color::White);
    rpanel.getButtonById("listDelete").onHover(2);
    rpanel.getButtonById("listDelete").hidden = true;
}

void toggleButtons(ui::Container &rpanel) {
    rpanel.getButtonById("merSort").hidden = (active_structure != ARRAY);
    rpanel.getButtonById("insSort").hidden = (active_structure != ARRAY);
    rpanel.getButtonById("selSort").hidden = (active_structure != ARRAY);
    rpanel.getButtonById("bubSort").hidden = (active_structure != ARRAY);
    rpanel.getButtonById("arrDelete").hidden = (active_structure != ARRAY);
    rpanel.getButtonById("arrInsert").hidden = (active_structure != ARRAY);
    rpanel.getInputById("arrInsertIdx").hidden = (active_structure != ARRAY);

    rpanel.getButtonById("listDelete").hidden = (active_structure != LIST);
    rpanel.getButtonById("listInsert").hidden = (active_structure != LIST);
}