#pragma once
#include <SFML/Graphics.hpp>

class MenuInicio {
public:
    explicit MenuInicio(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event);
    void update(float dt);
    void draw();
    bool startRequested() const { return startRequested_; }

private:
    sf::RenderWindow& window_;
    sf::Font font_;
    sf::Text title_;
    sf::Text instruction_;
    sf::RectangleShape backgroundOverlay_;
    sf::Texture bgTexture_;
    sf::Sprite bgSprite_;
    bool bgLoaded_ = false;
    bool fontLoaded_ = false;
    bool startRequested_ = false;
    float blinkTimer_ = 0.f;
    bool showInstruction_ = true;
};
