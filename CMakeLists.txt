cmake_minimum_required(VERSION 3.28)
project(INTERPRETER2)

set(CMAKE_CXX_STANDARD 17)

include_directories(Classi)
include_directories(Exceptions)
include_directories(Manager)
include_directories(Parser)
include_directories(Token)
include_directories(Visitor)

add_executable(INTERPRETER2
    Classi/Block.cpp
    Classi/Block.h
    Classi/Decl.cpp
    Classi/Decl.h
    Classi/Expr.cpp
    Classi/Expr.h
    Classi/Program.cpp
    Classi/Program.h
    Classi/Stmt.cpp
    Classi/Stmt.h
    Classi/Type.h
    Exceptions/Exceptions.h
    Manager/Manager.cpp
    Manager/Manager.h
    Parser/Parser.cpp
    Parser/Parser.h
    Token/Token.cpp
    Token/Token.h
    Token/Tokenizer.cpp
    Token/Tokenizer.h
    Visitor/Visitor.cpp
    Visitor/Visitor.h
    INTERPRETER.cpp)
