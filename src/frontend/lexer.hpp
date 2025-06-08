#ifndef __lexer_hpp
#define __lexer_hpp

#include <iostream>

class lexer
{
private:
int cursor;
int length;
public:
    lexer()
    {
        std::cout << "this is from lexer";
    }

    void initialize(std::string input_buffer)
    {
        cursor=0;
        length=input_buffer.size();
        std::cout << "initialized : " << input_buffer << std::endl;
    }

    void tokenize(std::string input_buffer){
        std::cout << "tokenized into curosor : "<<cursor<<" with length : "<<length<<std::endl;
    }
};

#endif