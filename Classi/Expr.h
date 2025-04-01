#ifndef EXPR_H
#define EXPR_H


#include "Visitor.h"
#include "Token.h"

class Visitor;

// Classe base per tutte le espressioni
class Expr {
public:
    virtual ~Expr() = default;
    virtual void accept(Visitor* visitor) = 0; // Metodo per il visitor
    
  
};

class Constant : public Expr {
public:
    Constant(int value) : value(value) {}
    Constant(bool value) : boolValue(value), isBool(true) {}

    void accept(Visitor* visitor) override;

    

    int getValue() const { return value; }
    bool isBoolean() const { return isBool; }
    bool getBoolValue() const { return boolValue; }

private:
    int value = 0;
    bool boolValue = false;
    bool isBool = false;
};

// Classe per gli identificatori (variabili)
class Id : public Expr {
public:
    // Costruttore per creare un identificatore con un nome specifico
    Id(const std::string& name) : name(name) {}

    // Metodo per accettare il visitor
    void accept(Visitor* visitor) override;

    // Ritorna il nome dell'identificatore
    const std::string& getName() const { return name; }


private:
    std::string name;
   
};

class Op : public Expr {
public:
	Op() = default;
    Op(char opTag) : opTag(opTag) {} 
    virtual ~Op() = default;
    virtual void accept(Visitor* visitor) = 0; 

protected:
    char opTag;
};

// Classe per le operazioni aritmetiche
class Arith : public Op {
public:
    Arith(Expr* left, Expr* right, char opTag)
        : Op(opTag), left(left), right(right) {}

    void accept(Visitor* visitor) override;


    Expr* getLeft() const { return left; }
    Expr* getRight() const { return right; }
    char getOpTag() const { return opTag; }
private:
    Expr* left;
    Expr* right;
    
};

// Classe per le operazioni unarie (come NOT)
class Unary : public Op {
public:
    Unary(Expr* expr, char opTag)
        : Op(opTag), expr(expr) {}

    void accept(Visitor* visitor) override;

    Expr* getExpr() const { return expr; }
    char getOpTag() const { return opTag; }

private:
    Expr* expr;
	
};


class Access : public Op {
public:
    Access(Id* arrayId, Expr* index)
        : arrayId(arrayId), index(index) {}

    void accept(Visitor* visitor) override;

    Id* getArrayId() const { return arrayId; } // Metodo per ottenere l'ID dell'array
    Expr* getIndex() const { return index; }     // Metodo per ottenere l'indice

private:
    Id* arrayId; // Espressione che rappresenta l'array
    Expr* index; // Espressione che rappresenta l'indice
};





//Logical sottoclasse di Expr
class Logical : public Expr {
public:
    virtual void accept(Visitor* visitor) = 0;

protected:
    Logical(char opTag) : opTag(opTag) {}

   char opTag;
};


// Sottoclasse per OR (operazione binaria)
class Or : public Logical {
public:
    Or(Expr* left, Expr* right) : Logical(Token::OR), left(left), right(right) {}

    void accept(Visitor* visitor) override;

    Expr* getLeft() const { return left; }
    Expr* getRight() const { return right; }

private:
    Expr* left;  // Operando sinistro
    Expr* right; // Operando destro
};

// Sottoclasse per AND (operazione binaria)
class And : public Logical {
public:
    And(Expr* left, Expr* right) : Logical(Token::AND), left(left), right(right) {}

    void accept(Visitor* visitor) override;

    Expr* getLeft() const { return left; }
    Expr* getRight() const { return right; }

private:
    Expr* left;
    Expr* right;
};

// Sottoclasse per operazioni relazionali (Rel)
class Rel : public Logical {
public:
    Rel(Expr* left, Expr* right, char opTag) : Logical(opTag), left(left), right(right), opTag(opTag) {}

    void accept(Visitor* visitor) override;

    Expr* getLeft() const { return left; }
    Expr* getRight() const { return right; }
    int getOpTag() const { return opTag; }

private:
    Expr* left;
    Expr* right;
    char opTag;  // Tag dell'operatore relazionale
};



#endif