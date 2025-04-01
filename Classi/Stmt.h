#ifndef STMT_H
#define STMT_H




class Visitor;
class Expr;


// Classe base per tutte le istruzioni
class Stmt {
public:
    virtual ~Stmt() = default;
    virtual void accept(Visitor* visitor) = 0;
};

// Istruzione If
class IfStmt : public Stmt {
public:
    IfStmt(Expr* condition, Stmt* trueBranch, Stmt* falseBranch = nullptr)
        : condition(condition), trueBranch(trueBranch), falseBranch(falseBranch) {}

    void accept(Visitor* visitor) override;

    Expr* getCondition() const { return condition; }
    Stmt* getTrueBranch() const { return trueBranch; }
    Stmt* getFalseBranch() const { return falseBranch; }

private:
    Expr* condition;
    Stmt* trueBranch;
    Stmt* falseBranch;
};


// Istruzione While
class WhileStmt : public Stmt {
public:
    WhileStmt(Expr* condition, Stmt* body)
        : condition(condition), body(body) {}

    void accept(Visitor* visitor) override;

    Expr* getCondition() const { return condition; }
    Stmt* getBody() const { return body; }

private:
    Expr* condition;
    Stmt* body;
};

// Do
class DoStmt : public Stmt {
public:
    DoStmt(Stmt* body, Expr* condition)
        : body(body), condition(condition) {}

    void accept(Visitor* visitor) override;

    Stmt* getBody() const { return body; }
    Expr* getCondition() const { return condition; }

private:
    Stmt* body;
    Expr* condition;
};

// Istruzione Set (assegnamento)
class SetStmt : public Stmt {
public:
    SetStmt(Expr* variable, Expr* value)
        : variable(variable), value(value) {}

    void accept(Visitor* visitor) override;

    Expr* getVariable() const { return variable; }
    Expr* getValue() const { return value; }

private:
    Expr* variable;
    Expr* value;
};

// assegnamento a vettore
class SetElemStmt : public Stmt {
public:
    SetElemStmt(Expr* array, Expr* index, Expr* value)
        : array(array), index(index), value(value) {}


    void accept(Visitor* visitor) override;

    Expr* getArray() const { return array; }
    Expr* getIndex() const { return index; }
    Expr* getValue() const { return value; }

private:
    Expr* array;
    Expr* index;
    Expr* value;
};

// Istruzione Break
class BreakStmt : public Stmt {
public:
    BreakStmt() = default;

    void accept(Visitor* visitor) override;
};



// Istruzione Print
class PrintStmt : public Stmt {
public:
    PrintStmt(Expr* expr)
        : expr(expr) {}

    void accept(Visitor* visitor) override;

    Expr* getExpr() const { return expr; }

private:
    Expr* expr;  
};




#endif // STMT_H


