#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "Visitor.h"
#include "Decl.h"
#include "Program.h"
#include "Token.h"
#include "Expr.h"
#include "Type.h"
#include "Block.h"
#include "Stmt.h"


class IfStmt;
class WhileStmt;
class DoStmt;
class PrintStmt;
class BreakStmt;
class SetStmt;
class SetElemStmt;
class Unary;
class Arith;
class Access;
class Constant;
class And;
class Or;
class Rel;
class Id;
class Decl;

InterpreterVisitor::InterpreterVisitor() {}

// Implementazione del distruttore
InterpreterVisitor::~InterpreterVisitor() {}


void InterpreterVisitor::visitProgram(Program* program) {
    Block* mainBlock = program->getBlock();
    if (mainBlock) {
        mainBlock->accept(this); 
    }

    for (const auto& result : printResults) {
        std::cout << result << std::endl; // Stampa ciascun risultato
    }
}
void InterpreterVisitor::visitBlock(Block* block) {
    if (block->getDecls().empty() && block->getStmts().empty()) {
        std::cout << "Empty Block" << std::endl;
        return;
    }

    // Prima valutiamo tutte le dichiarazioni
    for (const auto& decl : block->getDecls()) {
        if (decl != nullptr) {
            decl->accept(this);
        }
    }

    // Poi eseguiamo tutte le istruzioni
    for (const auto& stmt : block->getStmts()) {
        if (stmt != nullptr) {
            stmt->accept(this);
        }
    }
}

void InterpreterVisitor::visitUnary(Unary* op) {
    op->getExpr()->accept(this);
    if (std::holds_alternative<bool>(result)) {
        bool operandValue = std::get<bool>(result);
        result = !operandValue; 
    }
    else if (std::holds_alternative<int>(result)) {
        int operandValue = std::get<int>(result);
        result = -operandValue;
    }
}

void InterpreterVisitor::visitAccess(Access* op) {
    Id* arrayId = op->getArrayId();
    Expr* indexExpr = op->getIndex();

    std::string arrayName = arrayId->getName();

    // Recupero l'array dalla mappa delle variabili
    auto it = variables.find(arrayName);
    if (it != variables.end()) {
        // Controllo se è un vettore di interi
        if (std::holds_alternative<std::vector<int>>(it->second)) {
            auto& array = std::get<std::vector<int>>(it->second); // Riferimento al vettore

            // Valuta l'indice
            int indexValue = 0;
            if (dynamic_cast<Constant*>(indexExpr)) {
                Constant* constExpr = dynamic_cast<Constant*>(indexExpr);
                if (constExpr->isBoolean()) {
                    indexValue = constExpr->getBoolValue() ? 1 : 0; // Converto booleano in int
                }
                else {
                    indexValue = constExpr->getValue(); // Otteengo il valore intero
                }
            }
            else {
                Id* indexId = dynamic_cast<Id*>(indexExpr);
                if (indexId) {
                    std::string indexIdName = indexId->getName();
                    auto indexIt = variables.find(indexIdName);
                    if (indexIt != variables.end() && std::holds_alternative<int>(indexIt->second)) {
                        indexValue = std::get<int>(indexIt->second);
                    }
                    else {
                        throw std::runtime_error("Error: Index identifier not found");
                    }
                }
                else {
                    throw std::runtime_error("Error: Invalid index expression type");
                }
            }
        }
    }
}


void InterpreterVisitor::visitArith(Arith * op) {
        op->getLeft()->accept(this);
        int leftValue = std::get<int>(result);

        op->getRight()->accept(this);
        int rightValue = std::get<int>(result);

        switch (op->getOpTag()) {
        case Token::ADD:
            result = leftValue + rightValue;
            break;
        case Token::SUB:
            result = leftValue - rightValue;
            break;
        case Token::MUL:
            result = leftValue * rightValue;
            break;
        case Token::DIV:
            if (rightValue == 0) {
                throw std::runtime_error("Error: Division by zero");
            }
            result = leftValue / rightValue;
            break;
        default:
            throw std::runtime_error("Error: Unsupported arithmetic operation");
        }
    }


void InterpreterVisitor::visitConstant(Constant* expr) {
    if (expr->isBoolean()) {
        result = expr->getBoolValue();
    }
    else {
        result = expr->getValue();
    }
}

void InterpreterVisitor::visitAnd(And* op) {

    op->getLeft()->accept(this);
    bool leftValue = std::get<bool>(result);

    if (!leftValue) {
        result = false;
        return;
    }

    op->getRight()->accept(this);
    bool rightValue = std::get<bool>(result);
    result = leftValue && rightValue;

}

void InterpreterVisitor::visitOr(Or* logical) {
        logical->getLeft()->accept(this); 
        bool leftValue = std::get<bool>(result);

        logical->getRight()->accept(this);  
        bool rightValue = std::get<bool>(result);

        result = leftValue || rightValue; 
        return;
    }

void InterpreterVisitor::visitRel(Rel* logical) {
    logical->getLeft()->accept(this); 
    int leftValue = std::get<int>(result);

    logical->getRight()->accept(this);
    int rightValue = std::get<int>(result);

    switch (logical->getOpTag()) {
    case Token::LT:
        result = leftValue < rightValue ? true : false;
        break;
    case Token::GT:
        result = leftValue > rightValue ? true : false;
        break;
    case Token::LE:
        result = leftValue <= rightValue ? true : false;
        break;
    case Token::GE:
        result = leftValue >= rightValue ? true : false;
        break;
    case Token::EQ:
        result = leftValue == rightValue ? true : false;
        break;
    case Token::NE:
        result = leftValue != rightValue ? true : false;
        break;
    default:
        throw std::runtime_error("Error: Unsupported relational operation");
    }
}

void InterpreterVisitor::visitId(Id* id) {
    std::string varName = id->getName();

    auto it = variables.find(varName);
    if (it != variables.end()) {
        if (std::holds_alternative<int>(it->second)) {
            result = std::get<int>(it->second);
        }
        else if (std::holds_alternative<bool>(it->second)) {
            result = std::get<bool>(it->second);
        }
        else if (std::holds_alternative<std::vector<int>>(it->second)) {
            result = std::get<std::vector<int>>(it->second)[0];
        }
        else if (std::holds_alternative<std::vector<bool>>(it->second)) {
            result = std::get<std::vector<bool>>(it->second)[0]; 
        }
        else {
            throw std::runtime_error("Error: Variable type not supported.");
        }
    }
    else {
        throw std::runtime_error("Error: Undeclared variable " + varName);
    }
}


void InterpreterVisitor::visitIfStmt(IfStmt* ifstmt) {
    ifstmt->getCondition()->accept(this);
    bool conditionValue = std::get<bool>(result);

    if (conditionValue) {
        ifstmt->getTrueBranch()->accept(this);
    }
    else if (ifstmt->getFalseBranch() != nullptr) {
        ifstmt->getFalseBranch()->accept(this);
    }
}


void InterpreterVisitor::visitWhileStmt(WhileStmt* stmt) {
    try {
        stmt->getCondition()->accept(this);
        bool conditionValue = std::get<bool>(result);

        while (conditionValue) {
            // Esegui il corpo del ciclo
            stmt->getBody()->accept(this);

            // Valuta nuovamente la condizione
            stmt->getCondition()->accept(this);
            conditionValue = std::get<bool>(result);
        }
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;  // Gestisci l'errore (ad esempio, divisione per zero)
        // Il ciclo termina a causa dell'errore
    }
}


void InterpreterVisitor::visitDoStmt(DoStmt* stmt) {
    bool conditionValue;

    do {
        stmt->getBody()->accept(this);

        stmt->getCondition()->accept(this);

        if (std::holds_alternative<bool>(result)) {
            conditionValue = std::get<bool>(result);
        }
        else {
            throw std::runtime_error("Error: Do-while condition has to be boolean.");
        }
    } while (conditionValue);
}


void InterpreterVisitor::visitSetStmt(SetStmt* stmt) {
    Id* variable = dynamic_cast<Id*>(stmt->getVariable());
    Expr* valueExpr = stmt->getValue();

    if (variable) {
        auto it = variables.find(variable->getName());
        if (it == variables.end()) {
            throw std::runtime_error("Error: Undeclared variable " + variable->getName());
        }

        valueExpr->accept(this);

        // Controllo del tipo
        if ((it->second.index() == 1 && !std::holds_alternative<bool>(result)) || 
            (it->second.index() == 0 && !std::holds_alternative<int>(result))) {
            throw std::runtime_error("Error: Assigned wrong type value to " + variable->getName());
        }

        variables[variable->getName()] = result;
    }
    else {
        throw std::runtime_error("Error: Assigned to a wrong location");
    }
}


void InterpreterVisitor::visitSetElemStmt(SetElemStmt* stmt) {
    std::string arrayName = dynamic_cast<Id*>(stmt->getArray())->getName();

    stmt->getIndex()->accept(this); 
    int index;

    if (std::holds_alternative<int>(result)) {
        index = std::get<int>(result);
    }

    stmt->getValue()->accept(this);
    auto value = result;

    if (std::holds_alternative<std::vector<int>>(variables[arrayName])) {
        auto& array = std::get<std::vector<int>>(variables[arrayName]);
        if (index < 0 || index >= array.size()) {
            throw std::runtime_error("Error: Out of bounds: " + arrayName);
        }
    }
    else if (std::holds_alternative<std::vector<bool>>(variables[arrayName])) {
        auto& array = std::get<std::vector<bool>>(variables[arrayName]);
        if (index < 0 || index >= array.size()) {
            throw std::runtime_error("Error: Out of bounds:  " + arrayName);
        }
    }
}

void InterpreterVisitor::visitBreakStmt(BreakStmt* stmt) {
    throw std::runtime_error("Break statement encountered");
}

void InterpreterVisitor::visitPrintStmt(PrintStmt* stmt) {
    stmt->getExpr()->accept(this);

    std::string output;
    if (std::holds_alternative<bool>(result)) {
        output = std::string() + (std::get<bool>(result) ? "true" : "false");
    }
    else if (std::holds_alternative<int>(result)) {
        output = std::to_string(std::get<int>(result)); 
    }

    printResults.push_back(output);
}

void InterpreterVisitor::visitDecl(Decl* decl) {
    const std::string& id = decl->getId();
    Type* type = decl->getType();

    if (type->isInt()) {
        variables[id] = 0;
    }
    else if (type->isBool()) {
        variables[id] = false;
    }
}









