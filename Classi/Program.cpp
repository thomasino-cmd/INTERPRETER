#include "Program.h"
#include "Visitor.h"



void Program::accept(Visitor* visitor) {
    visitor->visitProgram(this);
}