#ifndef __lexer_hpp
#define __lexer_hpp

#include <iostream>
#include <vector>
#include<unordered_map>

typedef enum
{
    token_insert,
    token_into,
    token_value,
    token_string,
    token_integer,
    token_left_paren,
    token_right_paren,
    token_comma,
    token_delete,
    token_from,
    token_search,
    token_in,
    token_create,
    token_new,
    token_database,
    token_table,
    token_use,
    token_update,
    token_where,
    token_with,
    token_equals,
    token_less_than,
    token_greater_than

} token_set;

struct token
{
    token_set token_type;
    std::string value;
};

class lexer
{
private:
    int cursor;
    int length;
    std::string input_buffer;
    char current;
    std::vector<token> token_list;
    std::unordered_map<std::string,token_set> keyword_map={
        {"insert",token_insert},
        {"into",token_into},
        {"value",token_value},
        {"delete",token_delete},
        {"from",token_from},
        {"search",token_search},
        {"in",token_in},
        {"create",token_create},
        {"new",token_new},
        {"database",token_database},
        {"table",token_table},
        {"use",token_use},
        {"update",token_update},
        {"where",token_where},
        {"with",token_with},
    };

    std::string token_type_to_string(token_set local_token_set)
    {
        switch (local_token_set)
        {
        case 0:
            return "token_insert";
        case 1:
            return "token_into";
        case 2:
            return "token_value";
        case 3:
            return "token_string";
        case 4:
            return "token_integer";
        case 5:
            return "token_left_parenthesis";
        case 6:
            return "token_right_paranthesis";
        case 7:
            return "token_comma";
        case 8:
            return "token_delete";
        case 9:
            return "token_from";
        case 10:
            return "token_search";
        case 11:
            return "token_in";
        case 12:
            return "token_create";
        case 13:
            return "token_new";
        case 14:
            return "token_database";
        case 15:
            return "token_table";
        case 16:
            return "token_use";
        case 17:
            return "token_update";
        case 18:
            return "token_where";
        case 19:
            return "token_with";
        case 20:
            return "token_equals";
        case 21:
            return "token_less_than";
        case 22:
            return "token_greater_than";
        default:
            return "[!] ERROR IDENTIFYING TOKEN -> " + (local_token_set);
        }
    }

    void advance()
    {
        if (cursor == length - 1)
        {
            current = '\0';
            return;
        }
        else
        {
            cursor++;
            current = input_buffer[cursor];
        }
    }

    void skip_whitespace()
    {
        while (cursor < length && input_buffer[cursor] == ' ')
            advance();
    }

    void tokenize_alpha()
    {
        std::string local_buffer;
        token local_token;
        while (isalpha(current) || current == '_')
        {
            local_buffer.push_back(current);
            advance();
        }
        local_token.token_type = !keyword_map.count(local_buffer) ? token_string : keyword_map[local_buffer];
        local_token.value = local_buffer;
        token_list.push_back(local_token);
    }

    void tokenize_integer()
    {
        std::string local_buffer;
        token local_token;
        while (isdigit(current))
        {
            local_buffer.push_back(current);
            advance();
        }
        local_token.token_type = token_integer;
        local_token.value = local_buffer;
        token_list.push_back(local_token);
    }

    void tokenize_special_char(token_set type)
    {
        token local_token;
        local_token.token_type = type;
        local_token.value = current;
        advance();
        token_list.push_back(local_token);
    }

    void reset()
    {
        input_buffer.clear();
        token_list.clear();
        cursor = 0;
        length = 0;
        input_buffer = '\0';
    }

public:
    lexer()
    {
    }

    void displayToken()
    {
        for (auto &it : token_list)
            std::cout << it.value << " [" << token_type_to_string(it.token_type) << "]" << std::endl;
    }

    void initialize(std::string input_buffer)
    {
        cursor = 0;
        length = input_buffer.size();
        this->input_buffer = input_buffer;
        current = input_buffer[0];
    }

    void tokenize(std::string input_buffer)
    {
        while (current)
        {
            skip_whitespace();
            if (isalpha(current) || current=='_')
            {
                tokenize_alpha();
            }
            else if (isdigit(current))
            {
                tokenize_integer();
            }
            else
            {
                switch (current)
                {
                case '(':
                {
                    tokenize_special_char(token_left_paren);
                    break;
                }
                case ')':
                {
                    tokenize_special_char(token_right_paren);
                    break;
                }
                case ',':
                {
                    tokenize_special_char(token_comma);
                    break;
                }
                case '=':
                {
                    tokenize_special_char(token_equals);
                    break;
                }
                case '<':
                {
                    tokenize_special_char(token_less_than);
                    break;
                }
                case '>':
                {
                    tokenize_special_char(token_greater_than);
                    break;
                }
                default:
                {
                    std::cout << "[!] ERROR : UNINDENTIFIED CHARACTER -> " << current<<std::endl;
                    advance();
                }
                }
            }
        }
        displayToken();
        reset();
    }
};

#endif