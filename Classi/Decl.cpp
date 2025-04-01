#include "Decl.h"
#include "Visitor.h"

void Decl::accept(Visitor* visitor) {
    visitor->visitDecl(this);
}