#ifndef __lexer_hpp
#define __lexer_hpp

#include "headers.hpp"

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
    token_id,
    token_end_of_input,
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
        case token_insert:
            return "token_insert";
        case token_into:
            return "token_into";
        case token_value:
            return "token_value";
        case token_string:
            return "token_string";
        case token_integer:
            return "token_integer";
        case token_left_paren:
            return "token_left_parenthesis";
        case token_right_paren:
            return "token_right_paranthesis";
        case token_comma:
            return "token_comma";
        case token_delete:
            return "token_delete";
        case token_from:
            return "token_from";
        case token_search:
            return "token_search";
        case token_in:
            return "token_in";
        case token_create:
            return "token_create";
        case token_new:
            return "token_new";
        case token_database:
            return "token_database";
        case token_table:
            return "token_table";
        case token_use:
            return "token_use";
        case token_update:
            return "token_update";
        case token_where:
            return "token_where";
        case token_with:
            return "token_with";
        case token_equals:
            return "token_equals";
        case token_less_than:
            return "token_less_than";
        case token_greater_than:
            return "token_greater_than";
        case token_exit:
            return "token_exit";
        case token_id:
            return "token_id";
        case token_end_of_input:
            return "token_end_of_input";
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

    std::vector<token> &fetch_vector()
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
        token_list.push_back({token_end_of_input,"\0"});
        if (execution_status == 0)
            displayToken();
        return execution_status;
    }
};

#endif