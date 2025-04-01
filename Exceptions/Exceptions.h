#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

// Estensione della classe "std::runtime_error" in contesti differenti per aiutare a identificare la fase del programma in cui si è verificato il problema.

struct LexicalError : std::runtime_error {
    LexicalError(const char* msg) : std::runtime_error(msg) { }
    LexicalError(std::string msg) : std::runtime_error(msg.c_str()) { }
};

struct ParseError : std::runtime_error {
    ParseError(const char* msg) : std::runtime_error(msg) { }
    ParseError(std::string msg) : std::runtime_error(msg.c_str()) { }
};

struct SemanticError : std::runtime_error {
    SemanticError(const char* msg) : std::runtime_error(msg) { }
    SemanticError(std::string msg) : std::runtime_error(msg.c_str()) { }
};

#endif

