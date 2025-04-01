#include "tokenizer.h"
#include "Exceptions.h"
#include "Parser.h"

#include <cctype>   // Per isdigit, isalpha
#include <iostream> 

void Tokenizer::tokenizeInputFile(std::ifstream& inputFile, std::vector<Token>& inputTokens) {
    char currentChar;
    std::string currentWord;

    while (inputFile.get(currentChar)) {
		// Salta gli spazi
        if (isspace(currentChar)) {
            continue;
        }

        if (isdigit(currentChar)) {
            currentWord.clear();
            currentWord += currentChar;
            while (inputFile.get(currentChar) && isdigit(currentChar)) {
                currentWord += currentChar;
            }
            inputFile.unget();
            inputTokens.emplace_back(Token::NUM, currentWord);
            continue;
        }

        else if (isalpha(currentChar)) {
            currentWord.clear();
            currentWord += currentChar;
            while (inputFile.get(currentChar) && (isalnum(currentChar) || currentChar == '_')) {
                currentWord += currentChar;
            }
            inputFile.unget();

            if (currentWord == "if") {
                inputTokens.emplace_back(Token::IF, currentWord);
            }
            else if (currentWord == "else") {
                inputTokens.emplace_back(Token::ELSE, currentWord);
            }
            else if (currentWord == "while") {
                inputTokens.emplace_back(Token::WHILE, currentWord);
            }
            else if (currentWord == "do") {
                inputTokens.emplace_back(Token::DO, currentWord);
            }
            else if (currentWord == "break") {
                inputTokens.emplace_back(Token::BREAK, currentWord);
            }
            else if (currentWord == "print") {
                inputTokens.emplace_back(Token::PRINT, currentWord);
            }
            else if (currentWord == "true") {
                inputTokens.emplace_back(Token::TRUE, currentWord);
            }
            else if (currentWord == "false") {
                inputTokens.emplace_back(Token::FALSE, currentWord);
            }
            else if (currentWord == "int") {
                inputTokens.emplace_back(Token::INT, currentWord);

                // Controllo se il prossimo carattere è '['
                if (inputFile.peek() == '[') {
                    inputFile.get(currentChar); 
                    inputTokens.emplace_back(Token::LSB, "[");
                    currentWord.clear();

                    // Leggo il numero per la dimensione dell'array
                    while (inputFile.get(currentChar) && isdigit(currentChar)) {
                        currentWord += currentChar;
                    }

                    // Aggiungo il numero come token
                    if (!currentWord.empty()) {
                        inputTokens.emplace_back(Token::NUM, currentWord);
                    }

                    // Controll se il prossimo carattere è ']'
                    if (currentChar == ']') {
                        inputTokens.emplace_back(Token::RSB, "]");
                    }
                    else {
                        throw LexicalError("Error: Expected ']' after array size.");
                    }
                }
            }
            else if (currentWord == "boolean") {
                inputTokens.emplace_back(Token::BOOL, currentWord);
            }
            else {
                inputTokens.emplace_back(Token::ID, currentWord);
            }
            continue;
        }
            

        switch (currentChar) {
        case '=':
            if (inputFile.peek() == '=') {
                inputFile.get(currentChar);
                inputTokens.emplace_back(Token::EQ, "==");
            }
            else {
                inputTokens.emplace_back(Token::ASGM, "=");
            }
            break;
        case '!':
            if (inputFile.peek() == '=') {
                inputFile.get(currentChar);
                inputTokens.emplace_back(Token::NE, "!=");
            }
            else {
                inputTokens.emplace_back(Token::NOT, "!");
            }
            break;
        case '>':
            if (inputFile.peek() == '=') {
                inputFile.get(currentChar);
                inputTokens.emplace_back(Token::GE, ">=");
            }
            else {
                inputTokens.emplace_back(Token::GT, ">");
            }
            break;
        case '<':
            if (inputFile.peek() == '=') {
                inputFile.get(currentChar);
                inputTokens.emplace_back(Token::LE, "<=");
            }
            else {
                inputTokens.emplace_back(Token::LT, "<");
            }
            break;
        case '&':
            if (inputFile.peek() == '&') {
                inputFile.get(currentChar);
                inputTokens.emplace_back(Token::AND, "&&");
            }
            else {
                throw LexicalError("Error: Unexpected character: &");
            }
            break;
        case '|':
            if (inputFile.peek() == '|') {
                inputFile.get(currentChar);
                inputTokens.emplace_back(Token::OR, "||");
            }
            else {
                throw LexicalError("Error: Unexpected character: |");
            }
            break;
        case '+':
            inputTokens.emplace_back(Token::ADD, "+");
            break;
        case '-':
            inputTokens.emplace_back(Token::SUB, "-");
            break;
        case '*':
            inputTokens.emplace_back(Token::MUL, "*");
            break;
        case '/':
            inputTokens.emplace_back(Token::DIV, "/");
            break;
        case '{':
            inputTokens.emplace_back(Token::LCB, "{");
            break;
        case '}':
            inputTokens.emplace_back(Token::RCB, "}");
            break;
        case '(':
            inputTokens.emplace_back(Token::LP, "(");
            break;
        case ')':
            inputTokens.emplace_back(Token::RP, ")");
            break;
        case '[':
            inputTokens.emplace_back(Token::LSB, "[");
            break;
        case ']':
            inputTokens.emplace_back(Token::RSB, "]");
            break;
        case ';':
            inputTokens.emplace_back(Token::SC, ";");
            break;

        default:
            throw LexicalError("Unexpected character " + std::string(1, currentChar)); // Lancia LexicalError
        }
    }
}



