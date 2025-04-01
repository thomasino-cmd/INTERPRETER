#ifndef MANAGER_H
#define MANAGER_H

#include <memory>
#include<string>
#include<vector>

#include "Block.h"
#include "Decl.h"
#include "Type.h"
#include "Expr.h"
#include "Program.h"
#include "Stmt.h"
#include "Parser.h"


//I Manager vengono utilizzati per tenere traccia di tutti gli oggetti allocati.
class Manager
{
public:

	//Il costruttore di default genera vettori vuoti inizialmente.
    Manager() = default;

	//Ognuna delle classi derivate dovrà creare il proprio metodo di deallocazione della memoria.
    virtual ~Manager() {};

	//Devo eliminare i metodi di copia e assegnazione per evitare problemi di proprietà con i puntatori.
    Manager(const Manager& other) = delete;
    Manager& operator=(const Manager& other) = delete;

	//Funzione template per liberare la memoria di un vettore di oggetti.
    template <typename T>
    void clearMemory(std::vector<T>& vec);
};


class ExpressionManager : public Manager {
public:
    // Funzioni per creare diversi tipi di espressioni
    Constant* makeIntConstant(int value) {
        Constant* created = new Constant(value);
        Eallocated.push_back(created);
        return created;
    }

    Constant* makeBoolConstant(bool value) {
        Constant* created = new Constant(value);
        Eallocated.push_back(created);
        return created;
    }

    Id* makeId(const std::string& name) {
        Id* created = new Id(name);
        Eallocated.push_back(created);
        return created;
    }

    Arith* makeArith(Expr* left, Expr* right, char opTag) {
        Arith* created = new Arith(left, right, opTag);
        Eallocated.push_back(created);
        return created;
    }

    Access* makeAccess(Id* id, Expr* index) {
        Access* created = new Access(id, index);
        Eallocated.push_back(created);
        return created;
    }

    Unary* makeUnary(Expr* expr, char opTag) {
        Unary* created = new Unary(expr, opTag);
        Eallocated.push_back(created);
        return created;
    }


    Logical* makeOr(Expr* left, Expr* right) {
        Logical* created = new Or(left, right);
        Eallocated.push_back(created);
        return created;
    }

    Logical* makeAnd(Expr* left, Expr* right) {
        Logical* created = new And(left, right);
        Eallocated.push_back(created);
        return created;
    }

    Rel* makeRel(Expr* left, Expr* right, char opTag) {
        Rel* created = new Rel(left, right, opTag);
        Eallocated.push_back(created);
        return created;
    }

    ~ExpressionManager() override;

private:
    std::vector<Expr*> Eallocated; // Vector per gestire tutte le espressioni allocate
};

class StatementManager : public Manager {
public:
    
    Stmt* makeIfStmt(Expr* condition, Stmt* truebranch, Stmt* falsebranch) {
        Stmt* created = new IfStmt(condition, truebranch, falsebranch);
        Sallocated.push_back(created);
        return created;
    }

    Stmt* makeWhileStmt(Expr* condition, Stmt* body) {
        Stmt* created = new WhileStmt(condition, body);
        Sallocated.push_back(created);
        return created;
    }

    Stmt* makeDoStmt(Stmt* body, Expr* condition) {
        Stmt* created = new DoStmt(body, condition);
        Sallocated.push_back(created);
        return created;
    }

    Stmt* makeSetStmt(Expr* variable, Expr* value) {
        Stmt* created = new SetStmt(variable, value);
        Sallocated.push_back(created);
        return created;
    }

    Stmt* makeSetElemStmt(Expr* array, Expr* index, Expr* value) {
        Stmt* created = new SetElemStmt(array, index, value);
        Sallocated.push_back(created);
        return created;
    }

    Stmt* makeBreakStmt() {
        Stmt* created = new BreakStmt();
        Sallocated.push_back(created);
        return created;
    }

    Stmt* makePrintStmt(Expr* expr) {
        Stmt* created = new PrintStmt(expr);
        Sallocated.push_back(created);
        return created;
    }

    ~StatementManager() override;

private:
    std::vector<Stmt*> Sallocated;
};

class BlockManager : public Manager {
public:
    // Metodo per creare e memorizzare un nuovo blocco con dichiarazioni e istruzioni
    Block* makeBlock(const std::vector<std::shared_ptr<Decl>>& decls, const std::vector<std::shared_ptr<Stmt>>& stmts) {
        Block* created = new Block(decls, stmts);  
        Ballocated.push_back(created); 
        return created;
    }

    // Metodo per creare e memorizzare un blocco vuoto
    Block* makeEmptyBlock() {
        Block* created = new Block();  
        Ballocated.push_back(created);  
        return created;
    }

    ~BlockManager() override;  

private:
    std::vector<Block*> Ballocated;  
};

class DeclarationManager : public Manager {
public:
    // Metodo per creare una dichiarazione 
    Decl* makeDecl(Type* type, const std::string& id) {
        Decl* created = new Decl(type, id);  
        Dallocated.push_back(created);  
        return created;
    }

    ~DeclarationManager() override;

private:
    std::vector<Decl*> Dallocated;  
};


class TypeManager : public Manager {
public:
    // Metodo per creare un tipo base (int o boolean)
    Type* makeBasicType(Type::BasicType basicType) {
        Type* created = new Type(basicType);
        Tallocated.push_back(created);  
        return created;
    }

    // Metodo per creare un tipo array
    Type* makeArrayType(Type::BasicType basicType, int arraySize) {
        if (arraySize <= 0) {
            throw std::invalid_argument("La dimensione dell'array deve essere maggiore di zero.");
        }
        Type* created = new Type(basicType, arraySize); 
        Tallocated.push_back(created);  
        return created;
    }

    ~TypeManager() override;

private:
    std::vector<Type*> Tallocated; 
};


class ProgramManager : public Manager {
public:
   
    Program* makeProgram(Block* MB) {
        Program* created = new Program(MB);
        Pallocated.push_back(created);
        return created;
    }
    
    ~ProgramManager() override;


private:
    std::vector<Program*> Pallocated; 
};

#endif
