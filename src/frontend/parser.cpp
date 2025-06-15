#include <iostream>
#include "lexer.hpp"
#include <vector>

#define blue "\033[34m"
#define red "\e[0;31m"
#define green "\e[0;32m"
#define white "\e[0;37m"

class parser
{
private:
    lexer &lexer_obj;
    std::vector<token> &token_list;

    int parse_insert()
    {
        return 0;
    }

    int parse_delete()
    {
        return 0;
    }

    int parse_search()
    {
        return 0;
    }

    int parse_create()
    {
        return 0;
    }

    int parse_use()
    {
        return 0;
    }

    int parse_exit()
    {
        return 0;
    }

    int parse_update()
    {
        return 0;
    }

public:
    parser(lexer &lexer_obj) : lexer_obj(lexer_obj), token_list(lexer_obj.fetch_vector()) {};
    int parse()
    {
        for (auto &it : token_list)
        {
            switch (it.token_type)
            {
            case token_insert:
                return parse_insert();
            case token_delete:
                return parse_delete();
            case token_search:
                return parse_search();
            case token_create:
                return parse_create();
            case token_use:
                return parse_use();
            case token_exit:
                return parse_exit();
            case token_update:
                return parse_update();
            default:
            {
                std::cout <<red<< "[!] SYNTAX ERROR : UNDEFINED TOKEN -> " << it.value <<white<< std::endl;
                return 1;
            }
            }
        }
    }
};