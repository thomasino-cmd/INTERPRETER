#include "Expr.h"
#include "Visitor.h"



void Arith::accept(Visitor* visitor) {
	visitor->visitArith(this);
}

void Unary::accept(Visitor* visitor) {
	visitor->visitUnary(this);
}

void Access::accept(Visitor* visitor) {
	visitor->visitAccess(this);
}

void Constant::accept(Visitor* visitor) {
	visitor->visitConstant(this);
}

void And::accept(Visitor* visitor) {
	visitor->visitAnd(this);
}


void Or::accept(Visitor* visitor) {
	visitor->visitOr(this);
}

void Rel::accept(Visitor* visitor) {
	visitor->visitRel(this);
}

void Id::accept(Visitor* visitor) {
	visitor->visitId(this);
}