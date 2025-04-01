#include "Manager.h"

#include<vector>


template <typename T>
void Manager::clearMemory(std::vector<T>& allocated) {
    // Itera attraverso ciascun elemento del vettore passato. 
    // Dealloca l'elemento e, infine, imposta la lunghezza del vettore a zero.
    auto i = allocated.begin();
    for (; i != allocated.end(); ++i) {
        delete(*i);
    }
    allocated.resize(0);
}

ExpressionManager::~ExpressionManager() {
    clearMemory(Eallocated);
}


StatementManager::~StatementManager() {
	//Chiama la funzione clearMemory per deallocare la memoria per le istruzioni.
    clearMemory(Sallocated);
}

BlockManager::~BlockManager() {
    clearMemory(Ballocated);
}

DeclarationManager::~DeclarationManager() {
    clearMemory(Dallocated);
}

TypeManager::~TypeManager() {
    clearMemory(Tallocated);
}

ProgramManager::~ProgramManager() {
    clearMemory(Pallocated);
}

