#ifndef __lexer_hpp
#define __lexer_hpp

#include <iostream>
#include <vector>

typedef enum
{
    token_insert,
    token_into,
    token_string,
    token_value,
    token_left_paren,
    token_right_paren,
    token_int
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
            current=input_buffer[cursor];
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
        while(isalpha(current))
        {
            local_buffer.push_back(current);
            advance();
        }
        local_token.token_type=token_string;
        local_token.value=local_buffer;
        token_list.push_back(local_token);
    }

    void reset()
    {
        input_buffer.clear();
        token_list.clear();
        cursor=0;
        length=0;
        input_buffer='\0';
    }

public:
    lexer()
    {
    }

    void displayToken()
    {
        for (auto &it : token_list)
            std::cout << it.token_type << ") " << it.value << std::endl;
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
            if(isalpha(current))
            {
                tokenize_alpha();
            }
        }
        displayToken();
        reset();
    }
};

#endif