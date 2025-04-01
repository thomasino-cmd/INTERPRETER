#ifndef TYPE_H
#define TYPE_H

#include <string>
#include <memory>




class Type {
public:
    enum BasicType {
        INT,
        BOOL,
        
    };

    // Costruttore per tipi di base
    Type(BasicType basicType) : type(basicType), arraySize(0) {}

    // Costruttore per array
    Type(BasicType basicType, int size) : type(basicType), arraySize(size) {}


    bool isInt() const {
        return type == INT;
    }

    bool isBool() const {
        return type == BOOL;
    }

    bool isArrayType() const {
        return arraySize > 0;  // Se la dimensione dell'array è maggiore di 0, è un array
    }

    int getArraySize() const {
        return arraySize; // Restituisce la dimensione dell'array
    }

    BasicType getBasicType() const {
        return type; // Restituisce il tipo di base
    }

private:
    BasicType type; // Tipo base o array
    int arraySize;  // Dimensione dell'array
};

#endif


