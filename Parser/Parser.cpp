#include "Parser.h"
#include "Exceptions.h"
#include "Token.h"


#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <stack>



// Parsing di un intero programma (<program> → <block>)
Program* Parser::RecursiveProgramParse(std::vector<Token>::const_iterator& tokenItr) {
   
    Block* block = RecursiveBlockParse(tokenItr);

    // Creo e restituisco il programma usando ProgramManager
    Program* program = PM.makeProgram(block);

    return program; 
}



// Parsing di un blocco di codice (<block> → { <decls> <stmts> })
Block* Parser::RecursiveBlockParse(std::vector<Token>::const_iterator& tokenItr) {

        consume(tokenItr, Token::LCB, "Error: Expected '{' at the beginning of the block ");
       

        std::vector<std::shared_ptr<Decl>> decls;  
        std::vector<std::shared_ptr<Stmt>> stmts; 

        while (tokenItr->tag == Token::INT || tokenItr->tag == Token::BOOL) {
            auto decl = RecursiveDeclParse(tokenItr); 
            decls.push_back(std::shared_ptr<Decl>(decl)); 
        }

        // Parsing delle istruzioni <stmts>
        while (tokenItr->tag != Token::RCB && tokenItr != streamEnd) {
            auto stmt = RecursiveStatementParse(tokenItr); 
            stmts.push_back(std::shared_ptr<Stmt>(stmt)); 
        }

        consume(tokenItr, Token::RCB, "Error: Expected '}' at the end of the block ");

       
        return BM.makeBlock(decls, stmts); 

        // Se sia le dichiarazioni che le istruzioni sono vuote, crea un blocco vuoto
        if (decls.empty() && stmts.empty()) {
            Block* emptyBlock = BM.makeEmptyBlock();  
            return emptyBlock; 
        }
    }



/* Parsing di un'istruzione <stmt> → <loc> = <bool> ;
                                    | if (<bool>) < stmt >
                                    | if (<bool>) < stmt > else <stmt>
                                    | while (<bool>) < stmt >
                                    | do <stmt> while (<bool>);
                                    | break;
                                    | print(<bool>);
                                    | <block>
                           <loc> → <loc>[<bool>] | id */
Stmt* Parser::RecursiveStatementParse(std::vector<Token>::const_iterator& tokenItr) {
    

    switch (tokenItr->tag) {

       
    case Token::ID: {
        std::string locName = tokenItr->word; 
        safe_next(tokenItr); 

        if (tokenItr->tag == Token::ASGM) {
		// assegnamento a variabile
            consume(tokenItr, Token::ASGM, "Error: Expected '=' in assignment.");
            Expr* valueExpr = RecursiveExprParse(getPrecedence(tokenItr->tag), tokenItr); 
            consume(tokenItr, Token::SC, "Error: Expected ';' at the end of assignment ");
            return SM.makeSetStmt(new Id(locName), valueExpr);


        }
        else if (tokenItr->tag == Token::LSB) { 
		// assegnamento a vettore
            consume(tokenItr, Token::LSB, "Error: Expected '[' for array access.");
            Expr* indexExpr = RecursiveExprParse(getPrecedence(tokenItr->tag), tokenItr); 
            consume(tokenItr, Token::RSB, "Error: Expected ']' after index.");
            consume(tokenItr, Token::ASGM, "Error: Expected '=' in set element assignment.");
            Expr* valueExpr = RecursiveExprParse(getPrecedence(tokenItr->tag), tokenItr); 
            consume(tokenItr, Token::SC, "Error: Expected ';' at the end of set element assignment.");
            return SM.makeSetElemStmt(new Id(locName), indexExpr, valueExpr); 
        }
    } break;


    case Token::IF: {
        // Gestione della dichiarazione if
		safe_next(tokenItr); 
        consume(tokenItr, Token::LP, "Error: Expected '(' after 'if'.");
        Expr* condition = RecursiveExprParse(0, tokenItr);
        consume(tokenItr, Token::RP, "Error: Expected ')' after condition in 'if' statement.");
        Stmt* trueBranch = RecursiveBlockParse(tokenItr);
        Stmt* falseBranch = nullptr;

        if (tokenItr->tag == Token::ELSE) {
            consume(tokenItr, Token::ELSE, "Error: Expected 'else'.");
            falseBranch = RecursiveBlockParse(tokenItr);
        }

        return new IfStmt(condition, trueBranch, falseBranch);
    } break;


    case Token::WHILE: {
        // Gestione della dichiarazione while
        safe_next(tokenItr);
        consume(tokenItr, Token::LP, "Expected '(' after 'while' ");
        Expr* condition = RecursiveExprParse(getPrecedence(tokenItr->tag), tokenItr);
        consume(tokenItr, Token::RP, "Expected ')' after while condition ");
        Stmt* body = RecursiveBlockParse(tokenItr);
        return new WhileStmt(condition, body);

    } break;

                     
    case Token::DO: {
        // Gestione della dichiarazione do-while
        consume(tokenItr, Token::DO, "Error: Expected 'do'.");
        Stmt* body = RecursiveStatementParse(tokenItr);
        consume(tokenItr, Token::WHILE, "Error: Expected 'while' after 'do' block");
        consume(tokenItr, Token::LP, "Error: Expected '(' after 'while'.");
        Expr* condition = RecursiveExprParse(getPrecedence(tokenItr->tag), tokenItr);
        consume(tokenItr, Token::RP, "Error: Expected ')' after condition in 'do-while' statement.");
        consume(tokenItr, Token::SC, "Error: Expected ';' after 'do-while' statement.");
        return new DoStmt(body, condition);
    } break;

    case Token::BREAK: {
        // Gestione della dichiarazione break
        consume(tokenItr, Token::BREAK, "Error: Expected 'break'.");
        consume(tokenItr, Token::SC, "Error: Expected ';' after 'break' statement.");
        return new BreakStmt();
    } break;

    case Token::PRINT: {
        // Gestione della dichiarazione print
        safe_next(tokenItr); 
        consume(tokenItr, Token::LP, "Error: Expected '(' after 'print' ");
        Expr* expr = RecursiveExprParse(getPrecedence(tokenItr->tag), tokenItr);

        consume(tokenItr, Token::RP, "Error: Expected ')' after expression in 'print' statement.");

        consume(tokenItr, Token::SC, "Error: Expected ';' after print statement.");
        return new PrintStmt(expr);
    } break;
    
    case Token::LCB: { 
        return RecursiveBlockParse(tokenItr); 

    } break;




    default:
        throw ParseError("Error: unexpected token in statement: " + tokenItr->word);
    }

    return nullptr; // Se non viene trovato alcun statement valido
}




// Parsing di una dichiarazione (<decl> → <type> id ;)
    Decl* Parser::RecursiveDeclParse(std::vector<Token>::const_iterator& tokenItr) {

        // Parsing del tipo
        Type* type = RecursiveTypeParse(tokenItr);
        // Parsing dell'identificatore
        Token idToken = consume(tokenItr, Token::ID, "Error: Expected an identifier for the declaration.");
        std::string id = idToken.word;

        // Aggiungo la variabile alla mappa
        if (type->isInt()) {
            variables[id] = 0; // Imposto un valore iniziale per interi
        }
        else if (type->isBool()) {
            variables[id] = false; // Imposto un valore iniziale per booleani
        }

        consume(tokenItr, Token::SC, "Error: Expected ';' at the end of declaration.");

        return new Decl(type, id);
    }





/* Parsing di un tipo <type> → <type>[num] | <basic>
                      <basic> → int | boolean */
 Type* Parser::RecursiveTypeParse(std::vector<Token>::const_iterator & tokenItr) {
       
     if (tokenItr->tag == Token::INT) {
         consume(tokenItr, Token::INT, "Error: Expected 'int'.");

         if (tokenItr->tag == Token::LSB) {
             consume(tokenItr, Token::LSB, "Error: Expected '[' for array declaration.");

             Token sizeToken = consume(tokenItr, Token::NUM, "Error: Expected a number for array size.");
             int arraySize = std::stoi(sizeToken.word);

             consume(tokenItr, Token::RSB, "Error: Expected ']' after array size.");
             return new Type(Type::BasicType::INT, arraySize);
         }

         return new Type(Type::BasicType::INT);
     }
        else if (tokenItr->tag == Token::BOOL) {
            consume(tokenItr, Token::BOOL, "Error: Expected 'boolean'.");

            if (tokenItr->tag == Token::LSB) {
                consume(tokenItr, Token::LSB, "Error: Expected '[' for array declaration."); 

                
                Token sizeToken = consume(tokenItr, Token::NUM, "Error: Expected a number for array size."); 
                int arraySize = std::stoi(sizeToken.word);

                consume(tokenItr, Token::RSB, "Error: Expected ']' after array size."); 
                return new Type(Type::BasicType::BOOL, arraySize); 
            }

            return new Type(Type::BasicType::BOOL); 
        }

        throw ParseError("Error: Expected a valid type (int or boolean).");
    }

 /* Parsing di un'espressione <bool> → <bool> || <join> | <join>
                              <join> → <join> && <equality> | <equality>
                              <equality> → <equality> == <rel> | <equality> != <rel> | <rel>
                              <rel> → <expr> < <expr> | <expr> <= <expr> | <expr> >= <expr> | <expr> > <expr> | <expr>
                              <expr> → <expr> + <term> | <expr> - <term> | <term>
                              <term> → <term> * <unary> | <term> / <unary> | <unary>
                              <unary> → ! <unary> | - <unary> | <factor>
                              <factor> → ( <bool> ) | <loc> | num | true | false */
 Expr* Parser::RecursiveExprParse(int precedence, std::vector<Token>::const_iterator& tokenItr) {

     Expr* leftExpr = nullptr;

     // Gestione delle espressioni tra parentesi
     if (tokenItr->tag == Token::LP) {  
         safe_next(tokenItr);  
         leftExpr = RecursiveExprParse(0, tokenItr);  
         consume(tokenItr, Token::RP, "Error: Expected ')' after expression"); 
     }
     else if (tokenItr->tag == Token::NUM) { 

         leftExpr = EM.makeIntConstant(std::stoi(tokenItr->word));
         safe_next(tokenItr);
     }
     else if (tokenItr->tag == Token::TRUE) { 
         leftExpr = EM.makeBoolConstant(true);
         safe_next(tokenItr);  
     }
     else if (tokenItr->tag == Token::FALSE) { 
         leftExpr = EM.makeBoolConstant(false); 
         safe_next(tokenItr);
     }
    
     else if (tokenItr->tag == Token::ID) {  
         std::string idName = tokenItr->word; 
         safe_next(tokenItr);

         // Controllo se il token successivo è un accesso all'array
         if (tokenItr->tag == Token::LSB) {  
             consume(tokenItr, Token::LSB, "Error: Expected '['"); 
             Expr* indexExpr = RecursiveExprParse(0, tokenItr);
             consume(tokenItr, Token::RSB, "Error: Expected ']'"); 

             leftExpr = EM.makeAccess(EM.makeId(idName), indexExpr); 
         }
         else {
             leftExpr = EM.makeId(idName);
         }
     }
     else if (tokenItr->tag == Token::NOT) {
         safe_next(tokenItr); 
         Expr* operand = RecursiveExprParse(100, tokenItr); 
         leftExpr = EM.makeUnary(operand, Token::NOT);
     }
     else if (tokenItr->tag == Token::SUB) {  
         safe_next(tokenItr);

         if (tokenItr->tag == Token::NUM) {
             leftExpr = EM.makeIntConstant(-std::stoi(tokenItr->word)); 
             safe_next(tokenItr);
         }
         else {
             Expr* operand = RecursiveExprParse(100, tokenItr);
             leftExpr = EM.makeUnary(operand, Token::SUB);
         }
     }
     else {
         throw ParseError("Error: Missing operand in expression");
     }
 

        while (precedence < getPrecedence(tokenItr->tag)) {

             Token binOp = *tokenItr;  
             safe_next(tokenItr); 

             
             Expr* rightExpr = RecursiveExprParse(getPrecedence(binOp.tag), tokenItr);

             if (rightExpr == nullptr) {
                 throw ParseError("Error: Invalid right expression.");
             }

             switch (binOp.tag) {
             case Token::ADD:
             case Token::SUB:
             case Token::MUL:
             case Token::DIV:
                 leftExpr = EM.makeArith(leftExpr, rightExpr, binOp.tag);  
                 break;
             case Token::AND:
                 leftExpr = EM.makeAnd(leftExpr, rightExpr); 
                 break;
             case Token::OR:
                 leftExpr = EM.makeOr(leftExpr, rightExpr);
                 break;
             case Token::EQ:
                 leftExpr = EM.makeRel(leftExpr, rightExpr, binOp.tag); 
                 break;
             case Token::NE:
                 leftExpr = EM.makeRel(leftExpr, rightExpr, binOp.tag);  
                 break;
             case Token::LT:
                 leftExpr = EM.makeRel(leftExpr, rightExpr, binOp.tag); 
                 break;
             case Token::LE:
                 leftExpr = EM.makeRel(leftExpr, rightExpr, binOp.tag); 
                 break;
             case Token::GT:
                 leftExpr = EM.makeRel(leftExpr, rightExpr, binOp.tag); 
                 break;
             case Token::GE:
                 leftExpr = EM.makeRel(leftExpr, rightExpr, binOp.tag);
                 break;
             default:
                 throw ParseError("Error: Unexpected operator in expression");
             }
         }

         return leftExpr;
     }
