#include "../include/Pantalla.hpp"
#include "../include/Semibot.hpp"
#include <SFML/Window/Event.hpp>
#include <filesystem>
#include <algorithm>

Pantalla::Pantalla(unsigned int w, unsigned int h, const char *title)
: window(sf::VideoMode(w, h), title)
{
    // Intentar cargar el fondo
    bgLoaded = false;
    if (bgTexture.loadFromFile("assets/images/FondoChimenea.png")) {
        bgSprite.setTexture(bgTexture);
        // escalar para llenar la ventana (ajustar según proporción)
        float bgW = (float)bgTexture.getSize().x;
        float bgH = (float)bgTexture.getSize().y;
        float sx = (float)w / bgW;
        float sy = (float)h / bgH;
        bgSprite.setOrigin(0.f, 0.f);
        bgSprite.setPosition(0.f, 0.f);
        bgSprite.setScale(sx, sy);
        bgLoaded = true;
    }

    // Inicializar Semibot (gestiona texturas y animaciones internamente)
    {
        Direccion d; // por defecto Derecha
        // Calcular tamaño objetivo proporcional usando la escala aplicada al fondo
        (void)bgLoaded; // el Semibot calculará escalado si es necesario
        semibotLoaded = semibot.initWithBackground(d, bgSprite, bgTexture, w, h, floorOffset);
        if (semibotLoaded) {
            // tamaño del semibot configurado internamente en Semibot::initWithBackground
        }
    }

    // Cargar sonido de footsteps (wav/ogg)
    {
        const std::vector<std::string> attempts = {
            "assets/sound/footsteps.wav",
            "assets/sound/footsteps.ogg"
        };
        for (const auto &p : attempts) {
            if (std::filesystem::exists(p) && footBuffer.loadFromFile(p)) {
                footSound.setBuffer(footBuffer);
                footSound.setLoop(true);
                footLoaded = true;
                break;
            }
        }
    }

    // Cargar ayuda visual (GamesButtons.png) y fuente para el texto
    {
        const std::string helpPath = "assets/images/GamesButtons.png";
        if (std::filesystem::exists(helpPath) && helpTexture.loadFromFile(helpPath)) {
            helpSprite.setTexture(helpTexture);
            helpLoaded = true;
        }
        // intentar cargar fuente (similar a MenuInicio)
        std::string preferred = "assets/fonts/arial.ttf";
        if (std::filesystem::exists(preferred) && helpFont.loadFromFile(preferred)) {
            helpText.setFont(helpFont);
        } else {
            for (const auto& p : std::filesystem::directory_iterator("assets/fonts")) {
                auto ext = p.path().extension().string();
                if (ext == ".ttf" || ext == ".otf") {
                    if (helpFont.loadFromFile(p.path().string())) { helpText.setFont(helpFont); break; }
                }
            }
        }
        helpText.setString("Usa las teclas A / D para moverte");
        helpText.setCharacterSize(20);
        helpText.setFillColor(sf::Color::White);
    }
}

void Pantalla::run()
{
    // Fallback shape en caso de que la textura no cargue (tamaño 128x172)
    const float fallbackW = 128.f;
    const float fallbackH = 172.f;
    sf::RectangleShape rect(sf::Vector2f(fallbackW, fallbackH));
    rect.setFillColor(sf::Color::Transparent);
    // centrar rect si no hay semibot cargado
    if (!semibotLoaded) {
        rect.setOrigin(fallbackW / 2.f, fallbackH / 2.f);
        rect.setPosition((float)window.getSize().x / 2.f, (float)window.getSize().y / 2.f);
    }

    // Movimiento dependiente del tiempo
    sf::Clock clock;
    const float speed = 200.f; // pixeles por segundo

    // Debug se gestiona desde Semibot ahora

    // Zona central para depuración (desactivada)

    // (no se necesita variable previa de estado de crouch)
    Direccion dir; // seguir dirección actual (derecha por defecto)
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        // Input y movimiento: delegar al Semibot (incluye límites y sonido)
        if (semibotLoaded) {
            semibot.processInput(dt);
            semibot.update(dt);
        } else {
            // fallback: mover rect
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) rect.move(-speed * dt, 0.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) rect.move(speed * dt, 0.f);
        }

        // Render
        window.clear();
        if (bgLoaded) window.draw(bgSprite);
        // zona central desactivada
        // si hay semibot, dibujar borde alrededor del asset (sprite o anim)
        if (semibotLoaded) {
            // dibujar semibot
            semibot.draw(window);
            // dibujar ayuda sobre el semibot durante los primeros segundos o hasta que el usuario presione tecla
            if (showHelp) {
                // calcular escala y posición relativa al semibot
                sf::Vector2f pos = semibot.getPosition();
                sf::FloatRect sb = semibot.getGlobalBounds();
                float sbW = sb.width;
                float sbH = sb.height;
                if (helpLoaded) {
                    // queremos que la imagen sea pequeña: ~70% del ancho del semibot
                    float targetW = sbW * 0.7f;
                    float texW = (float)helpTexture.getSize().x;
                    float texH = (float)helpTexture.getSize().y;
                    float s = (texW > 0.f) ? (targetW / texW) : 1.f;
                    helpSprite.setScale(s, s);
                    // posicionar centrada horizontalmente sobre el semibot, con un offset vertical
                    float hx = pos.x - (texW * s) / 2.f;
                    float hy = pos.y - sbH - 10.f - (texH * s); // 10 px de margen
                    helpSprite.setPosition(hx, hy);
                    window.draw(helpSprite);
                }
                // dibujar texto debajo de la imagen (o sobre si falta imagen)
                sf::FloatRect tb = helpText.getLocalBounds();
                helpText.setOrigin(tb.width/2.f, tb.height/2.f);
                helpText.setPosition(pos.x, pos.y - sbH - 12.f);
                window.draw(helpText);
                // actualizar temporizador
                helpTimer += dt;
                if (helpTimer >= helpDuration) showHelp = false;
            }
        } else {
            window.draw(rect);
        }
        window.display();
    }
}