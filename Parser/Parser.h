#ifndef PARSER_H
#define PARSER_H

#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "Manager.h"
#include "Exceptions.h"
#include "Token.h"

class ExpressionManager;
class StatementManager;
class BlockManager;
class DeclarationManager;
class TypeManager;
class ProgramManager;

class Parser {
public:
	//Inizializzo il parser con i manager per gestire i nodi di espressioni, istruzioni, blocchi, dichiarazioni, tipi e programmi.
    Parser(ExpressionManager& e,
        StatementManager& s,
        BlockManager& b,
        DeclarationManager& d,
        TypeManager& t,
        ProgramManager& p) : EM{ e }, SM{ s }, BM{ b }, DM{ d }, TM{ t }, PM{ p } {}

	//Operator() per iniziare il parsing dal flusso di token in input.
    Program* operator()(const std::vector<Token>& TokenStream) {
        auto tokenItr = TokenStream.begin();
        streamEnd = TokenStream.end();
        Program* expr = RecursiveProgramParse(tokenItr);
        
        return expr;
    }

private:
    std::unordered_map<std::string, std::variant<int, bool>> variables; // Mappa per le variabili

    std::vector<Token>::const_iterator streamEnd;
   
    ExpressionManager& EM;
    StatementManager& SM;
    BlockManager& BM;
    DeclarationManager& DM;
    TypeManager& TM;
    ProgramManager& PM;


    int ire{};  //Posizione corrente nel flusso di token.

	// Metodi per il parsing ricorsivo dei token
    Program* RecursiveProgramParse(std::vector<Token>::const_iterator& tokenItr);  
    Block* RecursiveBlockParse(std::vector<Token>::const_iterator& tokenItr);     
    Stmt* RecursiveStatementParse(std::vector<Token>::const_iterator& tokenItr);
    Expr* RecursiveExprParse(int precedence, std::vector<Token>::const_iterator& tokenItr); 
    Type* RecursiveTypeParse(std::vector<Token>::const_iterator& tokenItr); 
    Decl* RecursiveDeclParse(std::vector<Token>::const_iterator& tokenItr);


	//Funzione per passare al token successivo in modo sicuro.
    void safe_next(std::vector<Token>::const_iterator& itr) {
        if (itr == streamEnd) {
            throw ParseError("Error: Unexpected end of token stream");
        }

        ++itr;  // Avanza l'iteratore al prossimo token

        // Aggiorna la posizione del token nel contatore 'ire'
        ++ire;
    }


    Token consume(std::vector<Token>::const_iterator& tokenItr, int expectedTag, const std::string& errorMessage) {
        // Verifica se il token corrente ha il tag atteso
        if (tokenItr->tag != expectedTag) {
            std::stringstream tmp{};
            // Messaggio di errore dettagliato, includendo posizione e token atteso
            tmp << errorMessage << "found '" << tokenItr->word << "'";
            throw ParseError(tmp.str());  // Lancia l'errore con il messaggio dettagliato
        }

        // Copia il token corrente prima di avanzare
        Token currentToken = *tokenItr;

        // Avanza al token successivo
        safe_next(tokenItr);  

        // Restituisci il token consumato
        return currentToken;
    }

	//Funzione per ottenere la precedenza di un operatore.
    int getPrecedence(int tokenTag) {
        switch (tokenTag) {
        case Token::OR:     return 1;
        case Token::AND:    return 2;
        case Token::EQ:     return 3;
        case Token::NE:     return 3;
        case Token::LT:     return 4;
        case Token::GT:     return 4;
        case Token::LE:     return 4;
        case Token::GE:     return 4;
        case Token::ADD:    return 5;
        case Token::SUB:    return 5;
        case Token::MUL:    return 6;
        case Token::DIV:    return 6;
        default:            return 0;
        }
    }

   

};

#endif

