#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
public:
	//Tipi di token con valori univoci
    static constexpr int LCB = 0;  // {
    static constexpr int RCB = 1;  // }
    static constexpr int RSB = 2;  // ]
    static constexpr int LSB = 3;  // [
    static constexpr int LP = 4;   // (
    static constexpr int RP = 5;   // )
    static constexpr int IF = 6;
    static constexpr int ELSE = 7;
    static constexpr int WHILE = 8;
    static constexpr int DO = 9;
    static constexpr int BREAK = 10;  
    static constexpr int PRINT = 11;
    static constexpr int ID = 12;
    static constexpr int EQ = 13;  // ==
    static constexpr int GT = 14;  // >
    static constexpr int GE = 15;  // >=
    static constexpr int LE = 16;  // <=
    static constexpr int LT = 17;  // <
    static constexpr int AND = 18;  // &&
    static constexpr int OR = 19;   // ||
    static constexpr int NOT = 20;  // !
    static constexpr int ADD = 21;
    static constexpr int SUB = 22;
    static constexpr int MUL = 23;
    static constexpr int DIV = 24;
    static constexpr int TRUE = 25;
    static constexpr int FALSE = 26;
    static constexpr int NUM = 27;
    static constexpr int BOOL = 28;
    static constexpr int SC = 28;   // ;
    static constexpr int ASGM = 30; // =
    static constexpr int NE = 31;   // !=
    static constexpr int INT = 32;


    // Array to map token IDs to their string representation
    static constexpr const char* id2word[] = {
        "{", "}", "[", "]", "(", ")", "if", "else", "while", "do",
        "break", "print", "ID", "==", ">", ">=", "<=", "<", "&&", "||", "!",
        "+", "-", "*", "/", "TRUE", "FALSE", "NUM", "BOOL", ";", "=", "!=", "INT"
    };

    // Constructor for Token
    Token(int t, const char* w) : tag{ t }, word{ w } { }

    Token(int t, std::string w) : tag{ t }, word{ w } { }
    ~Token() = default;
    Token(Token const&) = default;
    Token& operator=(Token const&) = default;

    int tag;  // Memorizza il tipo del token
   
	std::string word; //Memorizza il testo effettivo (lexema) per il token
};

#endif // TOKEN_H





