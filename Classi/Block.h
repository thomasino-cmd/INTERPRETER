#ifndef BLOCK_H
#define BLOCK_H

#include "Decl.h"
#include "Stmt.h"
#include "Visitor.h"
#include <vector>
#include <memory>

class Visitor;
class Stmt;
class Decl;

// Classe che rappresenta un blocco di codice
class Block : public Stmt {
public:
    // Costruttore vuoto per blocco vuoto
    Block() = default;

    void accept(Visitor* v);

    // Costruttore che prende un vettore di dichiarazioni e un vettore di istruzioni
    Block(const std::vector<std::shared_ptr<Decl>>& decls, const std::vector<std::shared_ptr<Stmt>>& stmts)
        : decls(decls), stmts(stmts) {}


    // Restituisce il vettore delle dichiarazioni
    const std::vector<std::shared_ptr<Decl>>& getDecls() const { return decls; }

    // Restituisce il vettore delle istruzioni
    const std::vector<std::shared_ptr<Stmt>>& getStmts() const { return stmts; }

private:
    std::vector<std::shared_ptr<Decl>> decls;  // Dichiarazioni
    std::vector<std::shared_ptr<Stmt>> stmts;  // Istruzioni
};

#endif // BLOCK_H
