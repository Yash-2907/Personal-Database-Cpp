#include<iostream>
#include "lexer.hpp"
#include<vector>

class parser{
    private:
    lexer& lexer_obj;
    std::vector<token>& token_list;
    public:
        parser(lexer& lexer_obj):lexer_obj(lexer_obj),token_list(lexer_obj.fetch_vector())
        {
        }
        int parse()
        {

        }
};