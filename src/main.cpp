#include <iostream>
#include "frontend/lexer.hpp"
#include "frontend/parser.cpp"
#include <chrono>
#define blue "\033[34m"
#define yellow "\033[33m"
#define white "\e[0;37m"

class evaluation_wrapper
{
private:
    lexer lexer_obj;
    parser *parser_obj;
    std::unordered_map<int, std::string> status = {
        {0, " SUCCESSFULLY EXECUTED THE COMMAND IN "},
        {1, "   FAILED TO EXECUTE THE COMMAND AT "}};
    int lexer_status, parser_status;

public:
    evaluation_wrapper()
    {
    }
    void handle(std::string input_buffer)
    {
        auto start = std::chrono::high_resolution_clock::now();
        lexer_obj.initialize(input_buffer);
        lexer_status = lexer_obj.tokenize(input_buffer);
        print_execution_result(lexer_status, start);
        if(lexer_status==1) return;
        parser_obj=new parser(lexer_obj);
    }
    void print_execution_result(int execution_status_lexer, auto start)
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double, std::milli>(end - start);
        std::cout << "\n-----------------------------------------------\n";
        std::cout << blue << "$" << (execution_status_lexer == 0 ? green : red) << status[execution_status_lexer] << duration.count() << (execution_status_lexer == 0 ? "ms " : "ms   ") << blue << "$" << white << "\n";
        std::cout << "-----------------------------------------------\n\n";
    }
};

int main()
{
    std::string input_buffer;
    evaluation_wrapper main_wrapper;
    while (true)
    {
        std::cout << yellow << "privateDb : " << white;
        std::getline(std::cin, input_buffer);
        std::cout << std::endl;
        if (input_buffer == "exit")
            exit(0);
        if (input_buffer == "cls" || input_buffer == "clear")
        {
            system("cls");
            continue;
        }
        main_wrapper.handle(input_buffer);
    }
    return 0;
}