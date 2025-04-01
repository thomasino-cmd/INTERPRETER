#include "Stmt.h"
#include "Visitor.h"



void IfStmt::accept(Visitor* visitor) {
    visitor->visitIfStmt(this);
}

// Implementazione del metodo accept per WhileStmt
void WhileStmt::accept(Visitor* visitor) {
    visitor->visitWhileStmt(this);
}

// Implementazione del metodo accept per DoStmt
void DoStmt::accept(Visitor* visitor) {
    visitor->visitDoStmt(this);
}

// Implementazione del metodo accept per PrintStmt
void PrintStmt::accept(Visitor* visitor) {
    visitor->visitPrintStmt(this);
}

// Implementazione del metodo accept per BreakStmt
void BreakStmt::accept(Visitor* visitor) {
    visitor->visitBreakStmt(this);
}

// Implementazione del metodo accept per SetStmt
void SetStmt::accept(Visitor* visitor) {
    visitor->visitSetStmt(this);
}

// Implementazione del metodo accept per SetElemStmt
void SetElemStmt::accept(Visitor* visitor) {
    visitor->visitSetElemStmt(this);
}
