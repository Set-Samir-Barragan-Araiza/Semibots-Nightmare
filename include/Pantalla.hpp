#pragma once
#include<SFML/Graphics.hpp>
#include "WalkAnimation.hpp"

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
    sf::Texture texture;
    sf::Sprite sprite;
    bool textureLoaded = false;
    WalkAnimation walkAnim;
    // Animation (sprite sheet) support: when using a sprite sheet
    // SemibotParado.png contiene 2 frames horizontales; estos campos permiten alternarlos
    int frameCount = 1;          // cuántos frames hay en la fila (por defecto 1)
    int currentFrame = 0;        // frame actual
    sf::Vector2i frameSize;      // tamaño de cada frame (px)
    float frameDuration = 0.5f;  // segundos entre frames cuando está parado
    float frameTimer = 0.f;      // acumulador de tiempo

};