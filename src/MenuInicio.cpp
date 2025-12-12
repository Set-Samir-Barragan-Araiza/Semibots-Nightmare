#include "../include/MenuInicio.hpp"
#include <filesystem>

MenuInicio::MenuInicio(sf::RenderWindow& window)
    : window_(window)
{
    // Intentar cargar fuente preferida, si no está intentar cualquier .ttf/.otf en assets/fonts
    std::string preferred = "assets/fonts/arial.ttf";
    if (std::filesystem::exists(preferred) && font_.loadFromFile(preferred)) {
        fontLoaded_ = true;
    } else {
        for (const auto& p : std::filesystem::directory_iterator("assets/fonts")) {
            auto ext = p.path().extension().string();
            if (ext == ".ttf" || ext == ".otf") {
                if (font_.loadFromFile(p.path().string())) { fontLoaded_ = true; break; }
            }
        }
    }

    if (fontLoaded_) {
        title_.setFont(font_);
        title_.setString("Semibots Nightmare");
        title_.setCharacterSize(56);
        title_.setFillColor(sf::Color::White);

        instruction_.setFont(font_);
        instruction_.setString("Presiona ENTER para comenzar");
        instruction_.setCharacterSize(24);
        instruction_.setFillColor(sf::Color::White);

        // Centrar textos
        sf::FloatRect tb = title_.getLocalBounds();
        title_.setOrigin(tb.width/2.f, tb.height/2.f);
        title_.setPosition(window_.getSize().x/2.f, window_.getSize().y/3.f);

        sf::FloatRect ib = instruction_.getLocalBounds();
        instruction_.setOrigin(ib.width/2.f, ib.height/2.f);
        instruction_.setPosition(window_.getSize().x/2.f, window_.getSize().y/3.f + 80.f);
    }

    backgroundOverlay_.setSize(sf::Vector2f(window_.getSize()));
    backgroundOverlay_.setFillColor(sf::Color(0,0,0,160));

    // Intentar cargar fondo específico del menú (.png, .jpg, .jpeg)
    {
        const std::vector<std::string> attempts = {
            "assets/images/FondoPantallaInicio.jpg",
            "assets/images/FondoPantallaInicio.jpeg",
            "assets/images/FondoPantallaInicio.png"
        };
        for (const auto &p : attempts) {
            if (bgTexture_.loadFromFile(p)) {
                bgSprite_.setTexture(bgTexture_);
                float bgW = (float)bgTexture_.getSize().x;
                float bgH = (float)bgTexture_.getSize().y;
                float sx = (float)window_.getSize().x / bgW;
                float sy = (float)window_.getSize().y / bgH;
                bgSprite_.setPosition(0.f, 0.f);
                bgSprite_.setScale(sx, sy);
                bgLoaded_ = true;
                break;
            }
        }
    }
}

void MenuInicio::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter) startRequested_ = true;
    }
}

void MenuInicio::update(float dt) {
    if (!fontLoaded_) return;
    blinkTimer_ += dt;
    if (blinkTimer_ >= 0.5f) {
        blinkTimer_ = 0.f;
        showInstruction_ = !showInstruction_;
        instruction_.setFillColor(showInstruction_ ? sf::Color::White : sf::Color(255,255,255,60));
    }
}

void MenuInicio::draw() {
    if (bgLoaded_) window_.draw(bgSprite_);
    window_.draw(backgroundOverlay_);
    if (fontLoaded_) {
        window_.draw(title_);
        window_.draw(instruction_);
    } else {
        // Fallback: dibujar un rectángulo simple si no hay fuente
        sf::RectangleShape box(sf::Vector2f(400.f, 120.f));
        box.setFillColor(sf::Color(50,50,50,220));
        box.setPosition(window_.getSize().x/2.f - 200.f, window_.getSize().y/3.f - 40.f);
        window_.draw(box);
    }
}
