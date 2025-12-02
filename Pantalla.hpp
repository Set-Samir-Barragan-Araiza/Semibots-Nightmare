#pragma once
#include<SFML/Graphics.hpp>

class Pantalla 
{
    public:
    Pantalla(unsigned int w=800, unsigned int h=600, const char *title = "Semibots Nightmare");
    void run();
    private:
    sf::RenderWindow window;

};