#include "../include/Semibot.hpp"
#include <SFML/System.hpp>

Semibot::Semibot() {}

bool Semibot::initWithBackground(const Direccion &d, const sf::Sprite &bgSprite, const sf::Texture &bgTexture, unsigned int winW, unsigned int winH, float fOffset) {
    // almacenar ventana y área del fondo
    direccion_ = d;
    windowW = winW; windowH = winH;
    areaLeft = bgSprite.getPosition().x;
    areaTop = bgSprite.getPosition().y;
    areaW = (float)bgTexture.getSize().x * bgSprite.getScale().x;
    areaH = (float)bgTexture.getSize().y * bgSprite.getScale().y;
    floorOffset = fOffset;

    // Cargar asset base (el loader anterior ya lo hace)
    if (!load(d)) return false;

    // permitir escalado proporcional y permitir swaps de textura/animación
    baseOnly = false;
    ignoreScale = false;

    // establecer tamaño objetivo en píxeles: 156x208 (ancho x alto)
    setIdleSizePixels(156.f, 208.f);

    // establecer textura/parado inicial y pre-cargar animación de caminar si existe
    setDirection(d);
    tryLoadWalkAnim();

    // posición inicial: centro horizontal, en el suelo (o centro si debug)
    float px = (float)windowW / 2.f;
    float py = debugCenter ? (float)windowH / 2.f : (areaTop + areaH - floorOffset);
    setPosition(px, py);

    // intentar cargar sonido de pasos localmente
    const std::vector<std::string> attempts = {"assets/sound/footsteps.wav","assets/sound/footsteps.ogg"};
    for (const auto &p : attempts) {
        if (std::filesystem::exists(p) && footBuffer.loadFromFile(p)) {
            footSound.setBuffer(footBuffer);
            footSound.setLoop(true);
            footLoaded = true;
            break;
        }
    }

    return true;
}

bool Semibot::load(const Direccion &dir) {
    direccion_ = dir;
    // Cargar el asset base completo (SemibotBase.png) sin tratarlo como spritesheet
    std::string p = direccion_.archivoBase(".png");
    if (!std::filesystem::exists(p) || !texture.loadFromFile(p)) {
        p = "assets/images/SemibotBase.png";
        if (!std::filesystem::exists(p) || !texture.loadFromFile(p)) {
            return false;
        }
    }
    sprite.setTexture(texture);
    // usar la imagen completa (no spritesheet)
    unsigned int texW = texture.getSize().x;
    unsigned int texH = texture.getSize().y;
    (void)texW; (void)texH;
    (void)texW; (void)texH;
    (void)texW; (void)texH;
    sprite.setTextureRect(sf::IntRect(0, 0, texW, texH));
    // origen en centro inferior para alinear pies
    sprite.setOrigin((float)texW / 2.f, (float)texH);
    // dejar el control de escalado al inicializador (no forzar aquí)
    // sprite.setScale(1.f, 1.f); // comentado para depuración: evitar cambios de resolución
    loaded = true;
    return true;
}

void Semibot::setPosition(float x, float y) {
    sprite.setPosition(x,y);
    if (walkAnim.isLoaded()) {
        // asegurar que la animación está configurada y alineada bottom-center
        walkAnim.configureBottom(idlePixW, idlePixH);
        walkAnim.setOrigin((float)walkAnim.texWidth() / 2.f, (float)walkAnim.texHeight());
        walkAnim.setPosition(x,y);
    }
}
sf::Vector2f Semibot::getPosition() const { return sprite.getPosition(); }
void Semibot::move(float dx, float dy) { sprite.move(dx, dy); if (walkAnim.isLoaded()) walkAnim.move(dx, dy); }

float Semibot::getHalfWidth() const {
    if (walkAnim.isLoaded()) {
        return (float)walkAnim.texWidth() * walkAnim.getScale().x / 2.f;
    }
    sf::FloatRect b = sprite.getGlobalBounds();
    return b.width / 2.f;
}

void Semibot::update(float dt) {
    if (moving && walkAnim.isLoaded()) {
        walkAnim.update(dt);
    }
    // actualizar sonido según estado de movimiento
    if (footLoaded) {
        if (moving) {
            if (footSound.getStatus() != sf::Sound::Playing) footSound.play();
        } else {
            if (footSound.getStatus() == sf::Sound::Playing) footSound.stop();
        }
    }
}

void Semibot::processInput(float dt) {
    if (!loaded) return;
    
    // solo cambiar la dirección si realmente cambia para evitar recargas
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        if (!direccion_.esIzquierda()) {
            Direccion nd; nd.setIzquierda();
            setDirection(nd);
        }
        move(-speed * dt, 0.f);
        setMoving(true);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        if (!direccion_.esDerecha()) {
            Direccion nd; nd.setDerecha();
            setDirection(nd);
        }
        move(speed * dt, 0.f);
        setMoving(true);
    } else {
        setMoving(false);
    }

    bool crouchingNow = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
    setCrouching(crouchingNow);

    // limitar posición horizontal dentro del área
    float halfw = getHalfWidth();
    sf::Vector2f pos = getPosition();
    if (pos.x < areaLeft + halfw) pos.x = areaLeft + halfw;
    if (pos.x > areaLeft + areaW - halfw) pos.x = areaLeft + areaW - halfw;
    // mantener Y según debugCenter o suelo
    if (debugCenter) pos.y = (float)windowH / 2.f;
    else pos.y = areaTop + areaH - floorOffset;
    setPosition(pos.x, pos.y);
}

sf::FloatRect Semibot::getGlobalBounds() const {
    if (walkAnim.isLoaded() && moving) {
        return walkAnim.getGlobalBounds();
    }
    return sprite.getGlobalBounds();
}

void Semibot::draw(sf::RenderWindow &win) {
    if (moving && walkAnim.isLoaded()) {
        walkAnim.draw(win);
    } else {
        win.draw(sprite);
    }
}

void Semibot::setDirection(const Direccion &d) {
    direccion_ = d;
    if (baseOnly) return; // no cambiar textura si estamos en modo base-only
    // recargar parado según nueva dirección
    std::string p = direccion_.archivoParado(".png");
    if (std::filesystem::exists(p) && texture.loadFromFile(p)) {
        sprite.setTexture(texture);
        unsigned int texW = texture.getSize().x;
        unsigned int texH = texture.getSize().y;
        // usar la imagen completa para el estado parado (no asumir spritesheet de 4 frames)
        sprite.setTextureRect(sf::IntRect(0,0,texW,texH));
        sprite.setOrigin((float)texW/2.f, (float)texH);
        applyIdleScale();
    }
    tryLoadWalkAnim();
}

void Semibot::setCrouching(bool c) {
    if (c == crouching) return;
    crouching = c;
    if (baseOnly) return; // no cambiar textura cuando usamos solo el asset base
    if (crouching) {
        // intentar cargar agachado
        // preferir SemibotAgachadoBase con sufijo de dirección, luego archivoAgachado, luego fallbacks
        std::string a = direccion_.archivoAgachadoBase(".png");
        if (!std::filesystem::exists(a) || !texture.loadFromFile(a)) {
            a = direccion_.archivoAgachadoBase(".jpg");
            if (!std::filesystem::exists(a) || !texture.loadFromFile(a)) {
                a = direccion_.archivoAgachado(".png");
                if (!std::filesystem::exists(a) || !texture.loadFromFile(a)) {
                    a = direccion_.archivoAgachado(".jpg");
                    if (!std::filesystem::exists(a) || !texture.loadFromFile(a)) {
                        // mantener textura previa
                    }
                }
            }
        }
        sprite.setTexture(texture);
        unsigned int tw = texture.getSize().x;
        unsigned int th = texture.getSize().y;
        sprite.setTextureRect(sf::IntRect(0,0,tw,th));
        sprite.setOrigin((float)tw/2.f, (float)th);
        applyCrouchScale();
    } else {
        // restaurar parado
        // Restaurar usando únicamente SemibotParadoDerecha/Izquierda
        std::string p = direccion_.archivoParado(".png");
        if (!std::filesystem::exists(p) || !texture.loadFromFile(p)) {
            // si no existe, mantener la textura actual y no intentar otros nombres
        }
        sprite.setTexture(texture);
            unsigned int texW = texture.getSize().x;
            unsigned int texH = texture.getSize().y;
            // usar la imagen completa para el estado parado
            sprite.setTextureRect(sf::IntRect(0, 0, texW, texH));
            sprite.setOrigin((float)texW/2.f, (float)texH);
        applyIdleScale();
    }
}

void Semibot::setMoving(bool m) {
    if (moving == m) return;
    moving = m;
    if (baseOnly) return; // no cambiar animaciones si estamos en modo base-only
    if (moving) {
        // intentar cargar animación de caminar
        tryLoadWalkAnim();
        if (walkAnim.isLoaded()) {
            // forzar alineación y escala iguales al sprite parado
            float sx = sprite.getScale().x;
            float sy = sprite.getScale().y;
            walkAnim.setScale(sx, sy);
            walkAnim.setOrigin((float)walkAnim.texWidth() / 2.f, (float)walkAnim.texHeight());
            walkAnim.setPosition(sprite.getPosition().x, sprite.getPosition().y);
            // reset animation timer/frame to start from first frame
            // (no direct API, update handles alternation)
        }
    } else {
        // al dejar de moverse, si no está agachado, volver al SemibotParado
        if (!crouching) {
            // Alternativa: usar el frame actual de la animación de caminar como 'idle'
            if (walkAnim.isLoaded()) {
                const sf::Texture &t = walkAnim.getCurrentTexture();
                sprite.setTexture(t);
                unsigned int texW = t.getSize().x;
                unsigned int texH = t.getSize().y;
                sprite.setTextureRect(sf::IntRect(0, 0, texW, texH));
                sprite.setOrigin((float)texW/2.f, (float)texH);
                // aplicar la misma escala que tenía la animación para evitar saltos
                sf::Vector2f s = walkAnim.getScale();
                sprite.setScale(s.x, s.y);
            } else {
                std::string p = direccion_.archivoParado(".png");
                if (!std::filesystem::exists(p) || !texture.loadFromFile(p)) {
                    // no intentar otros nombres
                }
                sprite.setTexture(texture);
                unsigned int texW = texture.getSize().x;
                unsigned int texH = texture.getSize().y;
                unsigned int frameH = texH / 4;
                sprite.setTextureRect(sf::IntRect(0, 0, texW, frameH));
                sprite.setOrigin((float)texW/2.f, (float)frameH);
                applyIdleScale();
            }
        }
    }
}

void Semibot::setIdleSizePixels(float w, float h) { idlePixW = w; idlePixH = h; applyIdleScale(); }
void Semibot::setCrouchSizePixels(float w, float h) { crouchUseAbsolute = true; crouchPixW = w; crouchPixH = h; if (crouching) applyCrouchScale(); }
void Semibot::setCrouchProportional(float fx, float fy) { crouchUseAbsolute = false; crouchFactorX = fx; crouchFactorY = fy; if (crouching) applyCrouchScale(); }

void Semibot::applyIdleScale() {
    if (texture.getSize().x == 0) return;
    unsigned int texW = texture.getSize().x;
    unsigned int texH = texture.getSize().y;
    (void)texW; (void)texH;
    // si es spritesheet vertical, usar ancho completo y frame height
    // unsigned int frameH = texH;
    // if (texH > texW) frameH = texH / 4; // heurística
    if (ignoreScale) {
        sprite.setScale(1.f, 1.f);
        if (walkAnim.isLoaded()) {
            walkAnim.setScale(1.f, 1.f);
            walkAnim.setOrigin((float)walkAnim.texWidth() / 2.f, (float)walkAnim.texHeight());
        }
        return;
    }
    // escala uniforme basada en la altura objetivo para evitar compresión
    float frameH = (float)texH;
    float scale = (frameH > 0.f) ? (idlePixH / frameH) : 1.f;
    sprite.setScale(scale, scale);
    if (walkAnim.isLoaded()) walkAnim.configureBottom(idlePixW, idlePixH);
}

void Semibot::applyCrouchScale() {
    if (texture.getSize().x == 0) return;
    unsigned int tw = texture.getSize().x;
    unsigned int th = texture.getSize().y;
    if (ignoreScale) {
        sprite.setScale(1.f, 1.f);
        (void)tw; (void)th;
        if (walkAnim.isLoaded()) {
            walkAnim.setScale(1.f, 1.f);
            walkAnim.setOrigin((float)walkAnim.texWidth() / 2.f, (float)walkAnim.texHeight());
        }
        return;
    }
    if (crouchUseAbsolute) {
        float s = (th > 0.f) ? (crouchPixH / (float)th) : 1.f;
        sprite.setScale(s, s);
        if (walkAnim.isLoaded()) walkAnim.configureBottom(crouchPixW, crouchPixH);
    } else {
        // calcular escala idle y aplicar factor vertical para mantener proporción
        float idleS = (th > 0.f) ? (idlePixH / (float)th) : 1.f;
        float s = idleS * crouchFactorY;
        sprite.setScale(s, s);
        if (walkAnim.isLoaded()) walkAnim.configureBottom(idlePixW * crouchFactorX, idlePixH * crouchFactorY);
    }
}

void Semibot::tryLoadWalkAnim() {
    std::string p1 = direccion_.archivoPaso1(".png");
    std::string p2 = direccion_.archivoPaso2(".png");
    if (std::filesystem::exists(p1) && std::filesystem::exists(p2)) {
            if (walkAnim.load(p1, p2)) {
            if (ignoreScale) {
                walkAnim.setScale(1.f, 1.f);
                walkAnim.setOrigin((float)walkAnim.texWidth() / 2.f, (float)walkAnim.texHeight());
            } else {
                walkAnim.configureBottom(idlePixW, idlePixH);
            }
            walkAnim.setPosition(sprite.getPosition().x, sprite.getPosition().y);
        }
    }
}
