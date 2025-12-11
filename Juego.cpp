#include "Pantalla.hpp"
#include <SFML/Window/Event.hpp>

Pantalla::Pantalla(unsigned int w, unsigned int h, const char *title)
: window(sf::VideoMode(w, h), title)
{
    // Intentar cargar la imagen SemibitParado desde la carpeta Imagenes
    textureLoaded = false;
    if (texture.loadFromFile("Imagenes/SemibotParado.png")) {
        sprite.setTexture(texture);
        // Configurar animación: SemibotParado.png contiene 4 frames verticales
        frameCount = 4;
        // frames apilados verticalmente: ancho = textura completa, alto = textura / frameCount
        frameSize.x = texture.getSize().x;
        frameSize.y = texture.getSize().y / frameCount;
        currentFrame = 0;
        // Ajustar rect al primer frame (primer cuadro en Y)
        sprite.setTextureRect(sf::IntRect(0, 0, frameSize.x, frameSize.y));
        // centrar usando el centro del frame
        sprite.setOrigin((float)frameSize.x / 2.f, (float)frameSize.y / 2.f);
        // Escalar el sprite para que mida 128x172 en pantalla (basado en tamaño de frame)
        const float targetW = 128.f;
        const float targetH = 172.f;
        float scaleX = targetW / (float)frameSize.x;
        float scaleY = targetH / (float)frameSize.y;
        sprite.setScale(scaleX, scaleY);
        sprite.setPosition((float)w / 2.f, (float)h / 2.f);
        textureLoaded = true;
        // Intentar cargar animación de caminar (dos imágenes)
        if (walkAnim.load("Imagenes/SemibotPaso1.png", "Imagenes/SemibotPaso2.png")) {
            walkAnim.configure(targetW, targetH);
            walkAnim.setPosition((float)w / 2.f, (float)h / 2.f);
        }
    } else {
        // si falla la carga, al menos dejamos textureLoaded = false y usaremos un fallback en run()
        // Puedes comprobar en tiempo de ejecución que la ruta es correcta y que el archivo existe.
    }
}

void Pantalla::run()
{
    // Fallback shape en caso de que la textura no cargue (tamaño 128x172)
    const float fallbackW = 128.f;
    const float fallbackH = 172.f;
    sf::RectangleShape rect(sf::Vector2f(fallbackW, fallbackH));
    rect.setFillColor(sf::Color::Green);
    // centrar rect si no hay textura
    if (!textureLoaded) {
        rect.setOrigin(fallbackW / 2.f, fallbackH / 2.f);
        rect.setPosition((float)window.getSize().x / 2.f, (float)window.getSize().y / 2.f);
    }

    // Movimiento dependiente del tiempo
    sf::Clock clock;
    const float speed = 200.f; // pixeles por segundo
    const float targetW = 128.f;
    const float targetH = 172.f;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Input: A/D para mover
        bool moving = false;
        if (textureLoaded) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    if (walkAnim.isLoaded()) walkAnim.move(-speed * dt, 0.f);
                    else sprite.move(-speed * dt, 0.f);
                    moving = true;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    if (walkAnim.isLoaded()) walkAnim.move(speed * dt, 0.f);
                    else sprite.move(speed * dt, 0.f);
                    moving = true;
                }

            // Limitar sprite dentro de la ventana (usar tamaño mostrado = frameSize * escala)
            // calcular límites dependiendo si usamos walkAnim o sprite
            if (walkAnim.isLoaded()) {
                // usar tamaño del frame y escala de walkAnim
                float halfw = (float)frameSize.x * walkAnim.getScale().x / 2.f;
                sf::Vector2f pos = walkAnim.getPosition();
                if (pos.x < halfw) walkAnim.setPosition(halfw, pos.y);
                if (pos.x > window.getSize().x - halfw) walkAnim.setPosition(window.getSize().x - halfw, pos.y);
            } else {
                sf::Vector2f pos = sprite.getPosition();
                sf::Vector2f scale = sprite.getScale();
                float halfw = (float)frameSize.x * scale.x / 2.f;
                if (pos.x < halfw) sprite.setPosition(halfw, pos.y);
                if (pos.x > window.getSize().x - halfw) sprite.setPosition(window.getSize().x - halfw, pos.y);
            }

            // Cuando está parado: usar la imagen completa de SemibotParado.png (sin alternar frames)
            if (!moving) {
                unsigned int texW = texture.getSize().x;
                unsigned int texH = texture.getSize().y;
                // si existía walkAnim en movimiento, copiar su posición al sprite
                if (walkAnim.isLoaded()) {
                    sf::Vector2f wp = walkAnim.getPosition();
                    sprite.setPosition(wp.x, wp.y);
                }
                // mostrar la textura completa
                sprite.setTextureRect(sf::IntRect(0, 0, texW, texH));
                sprite.setOrigin((float)texW / 2.f, (float)texH / 2.f);
                sprite.setScale(targetW / (float)texW, targetH / (float)texH);
            } else {
                // Al moverse, usar la animación de caminar si está disponible
                if (walkAnim.isLoaded()) {
                    walkAnim.update(dt);
                } else {
                    // mantener el primer frame (frame 0) del sprite sheet
                    currentFrame = 0;
                    sprite.setTextureRect(sf::IntRect(0, 0, frameSize.x, frameSize.y));
                    sprite.setOrigin((float)frameSize.x / 2.f, (float)frameSize.y / 2.f);
                    sprite.setScale(targetW / (float)frameSize.x, targetH / (float)frameSize.y);
                    frameTimer = 0.f;
                }
            }
        } else {
            // Mover el rect como fallback
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                rect.move(-speed * dt, 0.f);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                rect.move(speed * dt, 0.f);
            }
            // Limitar rect dentro de la ventana
            sf::Vector2f spos = rect.getPosition();
            float halfw = rect.getSize().x / 2.f;
            if (spos.x < halfw) rect.setPosition(halfw, spos.y);
            if (spos.x > window.getSize().x - halfw) rect.setPosition(window.getSize().x - halfw, spos.y);
        }

        window.clear();
        if (textureLoaded) {
            if (moving && walkAnim.isLoaded()) {
                walkAnim.draw(window);
            } else {
                window.draw(sprite);
            }
        } else {
            window.draw(rect);
        }
        window.display();
    }
}