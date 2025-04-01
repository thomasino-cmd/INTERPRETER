#include "Block.h"
#include "Visitor.h"

// Metodo per accettare un visitor
void Block::accept(Visitor* visitor) {
    visitor->visitBlock(this);
}
