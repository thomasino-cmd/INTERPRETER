#ifndef VISITOR_H
#define VISITOR_H

#include <unordered_map>
#include <variant>
#include <vector>
#include <string>
#include <iostream>


#include "Program.h"
#include "Block.h"
#include "Stmt.h"
#include "Decl.h"
#include "Expr.h"


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



class Visitor {
public:
	virtual ~Visitor() = default;

	virtual void visitProgram(Program* program) = 0;
	virtual void visitBlock(Block* block) = 0;
	virtual void visitIfStmt(IfStmt* stmt) = 0;
	virtual void visitWhileStmt(WhileStmt* stmt) = 0;
	virtual void visitDoStmt(DoStmt* stmt) = 0;
	virtual void visitPrintStmt(PrintStmt* stmt) = 0;
	virtual void visitBreakStmt(BreakStmt* stmt) = 0;
	virtual void visitSetStmt(SetStmt* stmt) = 0;
	virtual void visitSetElemStmt(SetElemStmt* stmt) = 0;
	virtual void visitDecl(Decl* decl) = 0;
	virtual void visitUnary(Unary* op) = 0;
	virtual void visitArith(Arith* op) = 0;
	virtual void visitAccess(Access* op) = 0;
	virtual void visitConstant(Constant* expr) = 0;
	virtual void visitAnd(And* logical) = 0;
	virtual void visitOr(Or* logical) = 0;
	virtual void visitRel(Rel* logical) = 0;
	virtual void visitId(Id* id) = 0;

};

class Constant;

class InterpreterVisitor : public Visitor {
public:
	
	InterpreterVisitor();  // Costruttore
	~InterpreterVisitor() override;  // Distruttore

	void visitProgram(Program* program) override;
	void visitBlock(Block* block) override;
	void visitIfStmt(IfStmt* ifstmt) override;
	void visitWhileStmt(WhileStmt* stmt) override;
	void visitDoStmt(DoStmt* stmt) override;
	void visitPrintStmt(PrintStmt* stmt) override;
	void visitBreakStmt(BreakStmt* stmt) override;
	void visitSetStmt(SetStmt* stmt) override;
	void visitSetElemStmt(SetElemStmt* stmt) override;
	void visitDecl(Decl* decl) override;
	void visitUnary(Unary* op) override;
	void visitArith(Arith* op) override;
	void visitAccess(Access* op) override;
	void visitConstant(Constant* expr) override;
	void visitAnd(And* logical) override;
	void visitOr(Or* logical) override;
	void visitRel(Rel* logical) override;
	void visitId(Id* id) override;

private:

	std::variant<int, bool, std::vector<int>, std::vector<bool>> result;

	// Mappa per le variabili e i loro valori
	std::unordered_map<std::string, std::variant<int, bool, std::vector<int>, std::vector<bool>>> variables;
	
	std::vector<std::string> printResults;

};

#endif // VISITOR_H
