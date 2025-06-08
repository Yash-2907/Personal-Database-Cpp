#include<iostream>
#include "frontend/lexer.hpp"

class evaluation_wrapper
{
    private:
    lexer lexer_obj;
    public:
    evaluation_wrapper()
    {
        std::cout<<"wrapper intialized !\n";
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
        std::cout<<"privateDb : ";
        std::getline(std::cin,input_buffer);
    }
    return 0;
}