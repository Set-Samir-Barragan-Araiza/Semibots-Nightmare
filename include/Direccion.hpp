#pragma once
#include <string>

// Clase simple para manejar la orientación del semibot (derecha/izquierda)
class Direccion {
public:
    enum class Dir { Derecha, Izquierda };

    Direccion(Dir d = Dir::Derecha) : dir_(d) {}

    void setDerecha() { dir_ = Dir::Derecha; }
    void setIzquierda() { dir_ = Dir::Izquierda; }
    void toggle() { dir_ = (dir_ == Dir::Derecha) ? Dir::Izquierda : Dir::Derecha; }

    bool esDerecha() const { return dir_ == Dir::Derecha; }
    bool esIzquierda() const { return dir_ == Dir::Izquierda; }

    // Devuelven rutas de archivo completas asumiendo carpeta assets/images y extensión (.png por defecto)
    std::string archivoParado(const std::string &ext = ".png") const {
        return prefijo() + "SemibotParado" + sufijo() + ext;
    }
    // Archivo base (referencia para estado parado sin detalles de animación)
    std::string archivoBase(const std::string &ext = ".png") const {
        return prefijo() + "SemibotBase" + sufijo() + ext;
    }
    std::string archivoPaso1(const std::string &ext = ".png") const {
        return prefijo() + "SemibotPaso1" + sufijo() + ext;
    }
    std::string archivoPaso2(const std::string &ext = ".png") const {
        return prefijo() + "SemibotPaso2" + sufijo() + ext;
    }
    std::string archivoAgachado(const std::string &ext = ".png") const {
        return prefijo() + "SemibotAgachado" + sufijo() + ext;
    }
    // Archivo base de agachado (referencia para agachado simplificado)
    std::string archivoAgachadoBase(const std::string &ext = ".png") const {
        return prefijo() + "SemibotAgachadoBase" + sufijo() + ext;
    }

private:
    Dir dir_;
    inline std::string sufijo() const { return (dir_ == Dir::Derecha) ? "Derecha" : "Izquierda"; }
    inline std::string prefijo() const { return std::string("assets/images/"); }
};
