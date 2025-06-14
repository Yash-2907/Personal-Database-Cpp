#ifndef __lexer_hpp
#define __lexer_hpp

#include <iostream>
#include <vector>
#include <unordered_map>
#define blue "\033[34m"
#define red "\e[0;31m"
#define green "\e[0;32m"
#define white "\e[0;37m"

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
    token_greater_than,
    token_exit,
    token_id
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
    std::unordered_map<std::string, token_set> keyword_map = {
        {"insert", token_insert},
        {"into", token_into},
        {"value", token_value},
        {"delete", token_delete},
        {"from", token_from},
        {"search", token_search},
        {"in", token_in},
        {"create", token_create},
        {"new", token_new},
        {"database", token_database},
        {"table", token_table},
        {"use", token_use},
        {"update", token_update},
        {"where", token_where},
        {"with", token_with},
        {"exit", token_exit},
    };

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
        while (current && current == ' ')
            advance();
    }

    int tokenize_string()
    {
        advance();
        std::string local_buffer;
        token local_token;
        while (current && current != '"')
        {
            local_buffer.push_back(current);
            advance();
        }
        if (!current)
        {
            std::cout << red << "[!] LEXER ERROR : NO ENDING QUOTES ADDED FOR THE STRING" << white << std::endl;
            return 1;
        }
        local_token.token_type = token_string;
        local_token.value = local_buffer;
        token_list.push_back(local_token);
        advance();
        return 0;
    }

    int tokenize_id()
    {
        std::string local_buffer;
        token local_token;
        while (isalpha(current) || isalnum(current) || current == '_')
        {
            local_buffer.push_back(current);
            advance();
        }
        local_token.token_type = !keyword_map.count(local_buffer) ? token_id : keyword_map[local_buffer];
        local_token.value = local_buffer;
        token_list.push_back(local_token);
        return 0;
    }

    int tokenize_integer()
    {
        std::string local_buffer;
        token local_token;
        while (isdigit(current) || isalpha(current))
        {
            if (isdigit(current))
            {
                local_buffer.push_back(current);
                advance();
            }
            else if (isalpha(current))
            {
                while (current && current != ' ')
                {
                    local_buffer.push_back(current);
                    advance();
                }
                std::cout << red << "[!] LEXER ERROR : A VARIABLE NAME CAN ONLY START WITH A CHARACTER -> " << local_buffer << white << std::endl;
                return 1;
            }
        }
        local_token.token_type = token_integer;
        local_token.value = local_buffer;
        token_list.push_back(local_token);
        return 0;
    }

    int tokenize_special_char(token_set type)
    {
        token local_token;
        local_token.token_type = type;
        local_token.value = current;
        advance();
        token_list.push_back(local_token);
        return 0;
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
        case 23:
            return "token_exit";
        case 24:
            return "token_id";
        default:
            return "[!] LEXER ERROR IDENTIFYING TOKEN -> " + (local_token_set);
        }
    }

    void displayToken()
    {
        for (auto &it : token_list)
            std::cout << it.value << " [" << token_type_to_string(it.token_type) << "]" << std::endl;
    }

    void initialize(std::string input_buffer)
    {
        reset();
        cursor = 0;
        length = input_buffer.size();
        this->input_buffer = input_buffer;
        current = input_buffer[0];
    }

    std::vector<token>& fetch_vector()
    {
        return token_list;
    }

    int tokenize(std::string input_buffer)
    {
        int execution_status = 0;
        while (current && execution_status == 0)
        {
            skip_whitespace();
            if (!current)
                break;
            else if (isalpha(current) || current == '_')
            {
                execution_status = tokenize_id();
            }
            else if (isdigit(current))
            {
                execution_status = tokenize_integer();
            }
            else
            {
                switch (current)
                {
                case '(':
                {
                    execution_status = tokenize_special_char(token_left_paren);
                    break;
                }
                case ')':
                {
                    execution_status = tokenize_special_char(token_right_paren);
                    break;
                }
                case ',':
                {
                    execution_status = tokenize_special_char(token_comma);
                    break;
                }
                case '=':
                {
                    execution_status = tokenize_special_char(token_equals);
                    break;
                }
                case '<':
                {
                    execution_status = tokenize_special_char(token_less_than);
                    break;
                }
                case '>':
                {
                    execution_status = tokenize_special_char(token_greater_than);
                    break;
                }
                case '"':
                {
                    execution_status = tokenize_string();
                    break;
                }
                default:
                {
                    std::cout << red << "\n[!] LEXER ERROR : UNINDENTIFIED CHARACTER -> " << current << white << std::endl;
                    execution_status = 1;
                }
                }
            }
        }
        if (execution_status == 0)
            displayToken();
        return execution_status;
    }
};

#endif