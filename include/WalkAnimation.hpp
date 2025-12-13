#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class WalkAnimation {
public:
    WalkAnimation() = default;

    bool load(const std::string &p1, const std::string &p2) {
        if (!tex0.loadFromFile(p1)) return false;
        if (!tex1.loadFromFile(p2)) return false;
        current = 0;
        sprite.setTexture(tex0);
        loaded = true;
        return true;
    }

    void configure(float targetW, float targetH) {
        if (!loaded) return;
        unsigned int w = tex0.getSize().x;
        unsigned int h = tex0.getSize().y;
        sprite.setOrigin((float)w / 2.f, (float)h / 2.f);
        // usar escala uniforme basada en la altura objetivo para evitar compresión
        cfgTargetW = targetW; cfgTargetH = targetH;
        float s = (h > 0) ? (targetH / (float)h) : 1.f;
        sprite.setScale(s, s);
    }

    // Configure origin at bottom-center so sprite position corresponds to feet on floor
    void configureBottom(float targetW, float targetH) {
        if (!loaded) return;
        unsigned int w = tex0.getSize().x;
        unsigned int h = tex0.getSize().y;
        sprite.setOrigin((float)w / 2.f, (float)h);
        cfgTargetW = targetW; cfgTargetH = targetH;
        float s = (h > 0) ? (targetH / (float)h) : 1.f;
        sprite.setScale(s, s);
    }

    void update(float dt) {
        if (!loaded) return;
        timer += dt;
        if (timer >= frameDuration) {
            timer = 0.f;
            current = (current + 1) % 2;
            // al cambiar la textura, recalcular origen y escala para mantener alineación bottom-center
            sf::Texture &t = (current == 0) ? tex0 : tex1;
            sprite.setTexture(t);
            if (cfgTargetH > 0.f) {
                unsigned int h = t.getSize().y;
                unsigned int w = t.getSize().x;
                sprite.setOrigin((float)w / 2.f, (float)h);
                float s = (h > 0) ? (cfgTargetH / (float)h) : 1.f;
                sprite.setScale(s, s);
                // al cambiar el frame, ya se recalculó origen/escala para mantener alineación
            }
        }
    }

    void setPosition(float x, float y) { sprite.setPosition(x, y); }
    void move(float dx, float dy) { sprite.move(dx, dy); }
    void setOrigin(float ox, float oy) { sprite.setOrigin(ox, oy); }
    void setScale(float sx, float sy) { sprite.setScale(sx, sy); }
    void draw(sf::RenderWindow &w) { if (loaded) w.draw(sprite); }
    bool isLoaded() const { return loaded; }
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    sf::Vector2f getScale() const { return sprite.getScale(); }
    unsigned int texWidth() const { return tex0.getSize().x; }
    unsigned int texHeight() const { return tex0.getSize().y; }
    sf::FloatRect getGlobalBounds() const { if (!loaded) return sf::FloatRect(); return sprite.getGlobalBounds(); }

    // Obtener la textura actualmente usada por la animación
    const sf::Texture &getCurrentTexture() const { return (current == 0) ? tex0 : tex1; }
    int getCurrentFrameIndex() const { return current; }

private:
    sf::Texture tex0;
    sf::Texture tex1;
    sf::Sprite sprite;
    float cfgTargetW = 0.f;
    float cfgTargetH = 0.f;
    bool loaded = false;
    int current = 0;
    float frameDuration = 0.15f;
    float timer = 0.f;
};
