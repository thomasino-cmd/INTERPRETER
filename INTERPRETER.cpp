#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <variant>

#include "Exceptions/Exceptions.h"
#include "Token.h"
#include "Tokenizer.h"
#include "Manager.h"
#include "Parser.h"
#include "Visitor.h"

int main(int argc, char* argv[]) {

    // Verifico se è stato specificato un file come argomento
    if (argc < 2) {
        std::cerr << "Error: Unspecified file!";
        std::cerr << "I use: " << argv[0] << " <nome_file>" << std::endl;
        return EXIT_FAILURE;
    }

    // Apertura del file specificato negli argomenti
    std::ifstream inputFile;
    try {
        inputFile.open(argv[1]);
        if (!inputFile.is_open()) {
            std::cerr << "Error: unexpected error opening file: " << argv[1] << std::endl;
            return EXIT_FAILURE;
        }

        /* Mostra il contenuto del file(per debugging)
        std::string line;
        while (std::getline(inputFile, line)) {
            std::cout << "Contenuto del file: " << line << std::endl;
        }*/

        // Torna all'inizio del file per tokenizzare
        inputFile.clear();  // Resetta gli eventuali flag di fine file
        inputFile.seekg(0, std::ios::beg);  // Torna all'inizio del file
    }
    catch (std::exception& exc) {
        std::cerr << "Error: Impossible to open file " << argv[1] << std::endl;
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }


    // Istanzia il tokenizer e popola il vettore di token
    Tokenizer tokenize;
    std::vector<Token> inputTokens;

    try {
        inputTokens = std::move(tokenize(inputFile));  // Tokenizzazione dell'input
        inputFile.close(); // Chiude il file
    }
    catch (LexicalError& le) {
        std::cerr << "Error: ";
        std::cerr << le.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception& exc) {
        std::cerr << "Error: Impossible reading from " << argv[1] << std::endl;
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Verifica se il tokenizer ha trovato token validi
    if (inputTokens.empty()) {
        std::cerr << "Error: Empty program" << std::endl;
        return EXIT_FAILURE;
    }

    // Gestione dei manager per l'allocazione dei nodi dell'AST
    ExpressionManager EM;
    StatementManager SM;
    BlockManager BM;
    DeclarationManager DM;
    TypeManager TM;
    ProgramManager PM;

    // Inizializza il parser con i manager per la costruzione dell'AST
    Parser parse{ EM, SM, BM, DM, TM, PM };

    try {
        // Parsing dei token e costruzione dell'AST
        Program* p = parse(inputTokens);
        std::unique_ptr<InterpreterVisitor> visitor = std::make_unique<InterpreterVisitor>();
        p->accept(visitor.get());  // Usa il visitor per interpretare il programma
    }
    catch (ParseError& pe) {
        std::cerr << pe.what() << std::endl;
        return EXIT_FAILURE;

    }
    catch (SemanticError& se) {
        std::cerr << se.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception& exc) {
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
