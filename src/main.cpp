#include<iostream>
#include "frontend/lexer.hpp"

#define blue "\033[34m"
#define yellow "\033[33m"
#define white "\e[0;37m"

class evaluation_wrapper
{
    private:
    lexer lexer_obj;
    public:
    evaluation_wrapper()
    {
    }

    void handle(std::string input_buffer)
    {
        lexer_obj.initialize(input_buffer);
        lexer_obj.tokenize(input_buffer);
    }
};

int main()
{
    std::string input_buffer;
    evaluation_wrapper main_wrapper;
    while(true)
    {
        std::cout<<yellow<<"privateDb : "<<white;
        std::getline(std::cin,input_buffer);
        if(input_buffer=="exit") exit(0);
        std::cout<<std::endl;
        main_wrapper.handle(input_buffer);
    }
    return 0;
}