#ifndef PROGRAM_H
#define PROGRAM_H




class Visitor;
class Block;

class Program
{
public:
    Program(Block* MB) : MainBlock{ MB } {}

    Block* getBlock() const {
        return MainBlock;
    }

    void accept(Visitor* visitor);

private:
    
    Block* MainBlock;
};
#endif


