
#include "../include/Pantalla.hpp"
#include "../include/MenuInicio.hpp"
#include <SFML/Graphics.hpp>
#include <filesystem>

int main()
{
    // Mostrar menú de inicio en su propia ventana
    const unsigned int WIN_W = 1280;
    const unsigned int WIN_H = 720;
    sf::RenderWindow menuWindow(sf::VideoMode(WIN_W,WIN_H), "Menu Inicio");
    MenuInicio menu(menuWindow);
    sf::Clock clock;
    while (menuWindow.isOpen() && !menu.startRequested()) {
        float dt = clock.restart().asSeconds();
        sf::Event event;
        while (menuWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                menuWindow.close();
                return 0;
            }
            menu.handleEvent(event);
        }
        menu.update(dt);
        menuWindow.clear();
        menu.draw();
        menuWindow.display();
    }
    menuWindow.close();

    // Iniciar la pantalla principal del juego
    Pantalla app(WIN_W, WIN_H, "Semibots Nightmare");
    app.run();
    return 0;
}
