#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include "WalkAnimation.hpp"
#include "Direccion.hpp"
#include "Semibot.hpp"

class Pantalla 
{
    public:
    Pantalla(unsigned int w=1280, unsigned int h=720, const char *title = "Semibots Nightmare");
    void run();
    private:
    sf::RenderWindow window;
    // background
    sf::Texture bgTexture;
    sf::Sprite bgSprite;
    bool bgLoaded = false;
    // offset en píxeles desde la parte inferior del fondo que se considera "suelo"
    float floorOffset = 0.f;
    public:
    void setFloorOffset(float o) { floorOffset = o; }
    Semibot semibot;
    bool semibotLoaded = false;
    // audio
    sf::SoundBuffer footBuffer;
    sf::Sound footSound;
    bool footLoaded = false;
    // Ayuda visual de controles
    sf::Texture helpTexture;
    sf::Sprite helpSprite;
    bool helpLoaded = false;
    sf::Font helpFont;
    sf::Text helpText;
    bool showHelp = true;
    float helpDuration = 4.f; // segundos por defecto
    float helpTimer = 0.f;
    // (La animación y texturas ahora se manejan desde `Semibot`)

};