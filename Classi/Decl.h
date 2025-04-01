#ifndef DECL_H
#define DECL_H

#include "Block.h"
#include <string>
#include <memory>

class Visitor;
class Block;
class Type;

class Decl{
public:
    // Costruttore della dichiarazione che accetta un tipo e un identificatore
    Decl(Type* type, const std::string& id)
        : type(type), id(id) {}

    void accept(Visitor* visitor);

    // Ritorna l'identificatore della variabile
    const std::string& getId() const { return id; }

    // Ritorna il tipo della variabile
    Type* getType() const { return type; }

private:
    Type* type; // Tipo della variabile (può essere un tipo base o un array)
    std::string id; // Identificatore della variabile
};

#endif // DECL_H
