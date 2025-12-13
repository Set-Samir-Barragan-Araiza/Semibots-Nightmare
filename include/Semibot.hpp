#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "WalkAnimation.hpp"
#include "Direccion.hpp"
#include <filesystem>

class Semibot {
public:
    Semibot();
    // carga inicial usando la dirección actual (intenta archivos con sufijos)
    bool load(const Direccion &dir);

    // estado
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    float getHalfWidth() const;
    void move(float dx, float dy);
    void update(float dt);
    void draw(sf::RenderWindow &win);

    void setDirection(const Direccion &d);
    void setCrouching(bool c); // entra/sale de agachado
    bool isCrouching() const { return crouching; }
    void setMoving(bool m);

    // escalado: establecer tamaño en píxeles para estado parado
    void setIdleSizePixels(float w, float h);
    // escalado absoluto para agachado en píxeles (ej: 19x13)
    void setCrouchSizePixels(float w, float h);
    // escalado proporcional sobre la escala de parado (factorX, factorY)
    void setCrouchProportional(float factorX, float factorY);
    // debug: ignorar cualquier escalado y usar tamaño natural de las texturas
    void setIgnoreScale(bool v) { ignoreScale = v; }
    // forzar o desactivar modo base-only desde fuera
    void setBaseOnly(bool v) { baseOnly = v; }
    // inicializar el semibot con información del fondo y ventana
    bool initWithBackground(const Direccion &d, const sf::Sprite &bgSprite, const sf::Texture &bgTexture, unsigned int windowW, unsigned int windowH, float floorOffset = 0.f);
    // procesar input y movimiento internamente (A/D, LCtrl)
    void processInput(float dt);
    void setDebugCenter(bool v) { debugCenter = v; }
    // establecer altura deseada del personaje en píxeles (sin importar ventana)
    void setCharacterHeightPixels(float h) { characterHeight = h; }
    // obtener bounds globales del semibot (sprite o animación)
    sf::FloatRect getGlobalBounds() const;

private:
    Direccion direccion_;
    sf::Texture texture; // textura actualmente usada (parado o agachado)
    sf::Sprite sprite;
    WalkAnimation walkAnim;

    bool loaded = false;
    bool moving = false;
    bool crouching = false;

    // target pixels para idle (por defecto 128x172)
    float idlePixW = 128.f;
    float idlePixH = 172.f;
    // crouch override
    bool crouchUseAbsolute = true;
    float crouchPixW = 19.f;
    float crouchPixH = 13.f;
    float crouchFactorX = 0.75f;
    float crouchFactorY = 0.6f;

    void applyIdleScale();
    void applyCrouchScale();
    void tryLoadWalkAnim();
    bool ignoreScale = false;
    // si true, usar solo el asset SemibotBase (sin cambiar texturas ni animaciones)
    bool baseOnly = false;
    // bounds/área derivados del fondo
    float areaLeft = 0.f;
    float areaTop = 0.f;
    float areaW = 0.f;
    float areaH = 0.f;
    float floorOffset = 0.f;
    unsigned int windowW = 1280;
    unsigned int windowH = 720;
    float speed = 200.f;
    bool debugCenter = false;
    // altura objetivo del sprite en píxeles (ej: 64)
    float characterHeight = 64.f;
    // audio interno de pasos
    sf::SoundBuffer footBuffer;
    sf::Sound footSound;
    bool footLoaded = false;
};
