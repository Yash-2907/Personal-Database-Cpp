#include "headers.hpp"
#include "lexer.hpp"

class parser
{
private:
    typedef enum
    {
        node_create_database,
        node_create_table,
        node_condition_less,
        node_condition_greater,
        node_condition_equals,
        node_use,
        node_search,
        node_delete,
        node_insert,
        node_update,
        node_sub_values,
        node_integer,
        node_string,
        node_exit,
    } node_set;

    struct ast_node
    {
        node_set node_type;
        std::string payload;
        std::vector<ast_node> children;
    };

    lexer &lexer_obj;
    std::vector<token> &local_token_list;
    token current_token, previous_token;
    ast_node evaluated_node;
    int index = 0, length = 0;

    int advance(std::set<token_set> required_token)
    {
        if (!check(required_token))
        {
            std::cout << std::endl
                      << red << "[!] PARSER ERROR : EXPECTED " << lexer_obj.token_type_to_string(*required_token.begin()) << " BUT RECIEVED " << lexer_obj.token_type_to_string(current_token.token_type) << " ==> { " << current_token.value << " } " << white << std::endl;
            return 1;
        }
        previous_token = current_token;
        index++;
        if (index <= length - 1)
            current_token = local_token_list[index];
        return 0;
    }

    bool check(std::set<token_set> required)
    {
        return required.find(current_token.token_type) != required.end();
    }

    void print_job(ast_node &node, int depth = 0)
    {
        std::string indent(depth * 2, ' ');
        if (depth == 0)
            std::cout << green << "\nParsing Successful :\n"
                      << white;

        std::cout << indent << "{\n";
        std::cout << indent << "  Type    : " << cyan << node_type_to_string(node.node_type) << white << "\n";
        std::cout << indent << "  Payload : " << yellow << node.payload << white << "\n";

        if (!node.children.empty())
        {
            std::cout << indent << "  Children:\n";
            for (auto &child : node.children)
            {
                print_job(child, depth + 1);
            }
        }

        std::cout << indent << "}\n";
    }

    ast_node parse_children()
    {
        ast_node child_node;
        child_node.node_type = previous_token.token_type == token_integer ? node_integer : node_string;
        child_node.payload = previous_token.value;
        return child_node;
    }

    int parse_insert()
    {
        // syntax to be followed : insert into table_name values (a,b,c...)
        if (advance({token_insert}))
            return 1;
        else
            evaluated_node.node_type = node_insert;
        if (advance({token_into}))
            return 1;
        if (advance({token_id}))
            return 1;
        else
            evaluated_node.payload = previous_token.value;
        if (advance({token_values}))
            return 1;
        if (advance({token_left_paren}))
            return 1;
        while (current_token.token_type == token_string || current_token.token_type == token_integer)
        {
            advance({current_token.token_type});
            evaluated_node.children.push_back(parse_children());
            if (current_token.token_type == token_comma)
                advance({token_comma});
        }
        if (current_token.token_type != token_right_paren)
        {
            std::cout << red << "\n[!] PARSER ERROR : EXPECTED INTEGER OR STRING INSIDE PARENTHESIS BUT RECIEVED -> {" << current_token.value << "}\n"
                      << white << std::endl;
            return 1;
        }
        else
        {
            if (advance({token_right_paren}))
                return 1;
        }
        if (advance({token_end_of_input}))
            return 1;
        print_job(evaluated_node);
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
        // syntax to be followed : create new database/table name_here
        if (advance({token_create}))
            return 1;
        if (advance({token_new}))
            return 1;
        if (advance({token_database, token_table}))
            return 1;
        else
            evaluated_node.node_type = previous_token.token_type == token_database ? node_create_database : node_create_table;
        if (advance({token_id}))
            return 1;
        else
            evaluated_node.payload = previous_token.value;
        if (advance({token_end_of_input}))
            return 1;
        print_job(evaluated_node);
        return 0;
    }

    int parse_use()
    {
        // syntax to be followed : use database_name
        if (advance({token_use}))
            return 1;
        else
            evaluated_node.node_type = node_use;
        if (advance({token_id}))
            return 1;
        else
            evaluated_node.payload = previous_token.value;
        print_job(evaluated_node);
        return 0;
    }

    int parse_exit()
    {
        // syntax to be followed : exit
        if (advance({token_exit}) == 1)
            return 1;
        else
            evaluated_node.node_type = node_exit;
        if (advance({token_end_of_input}) == 1)
            return 1;
        print_job(evaluated_node);
        return 0;
    }

    int parse_update()
    {
        // syntax to be followed : update table_name to (name=yash,surname=gupta) where (name=yash / result<100 / result>100)
        if (advance({token_update}))
            return 1;
        if (advance({token_id}))
            return 1;
        else
            evaluated_node.payload = previous_token.value;
        if (advance({token_to}))
            return 1;
        if (advance({token_left_paren}))
            return 1;
        if (advance({token_id}))
            return 1;
        else
            evaluated_node.children.push_back(parse_children());
        if (advance({token_equals}))
            return 1;
        if (advance({token_string, token_integer}))
            return 1;
        else
            evaluated_node.children.back().children.push_back(parse_children());
        if (advance({token_right_paren}))
            return 1;
        if (advance({token_where}))
            return 1;
        if (advance({token_left_paren}))
            return 1;
        if (advance({token_id}))
            return 1;
        else
            evaluated_node.children.push_back(parse_children());
        if (advance({token_less_than, token_greater_than, token_equals}))
            return 1;
        else
        {
            if (previous_token.token_type == token_less_than)
                evaluated_node.node_type = node_condition_less;
            else if (previous_token.token_type == token_greater_than)
                evaluated_node.node_type = node_condition_greater;
            else
                evaluated_node.node_type = node_condition_equals;
        }
        if (advance({token_string, token_integer}))
            return 1;
        else
            evaluated_node.children.back().children.push_back(parse_children());
        if (advance({token_right_paren}))
            return 1;
        if (advance({token_end_of_input}))
            return 1;
        print_job(evaluated_node);
        return 0;
    }

public:
    parser(lexer &lexer_obj) : lexer_obj(lexer_obj), local_token_list(lexer_obj.fetch_vector())
    {
        length = local_token_list.size();
        index = 0;
        current_token = local_token_list[0];
    };

    std::string node_type_to_string(node_set local_node)
    {
        switch (local_node)
        {
        case node_create_database:
            return "node_create_database";
        case node_create_table:
            return "node_create_table";
        case node_use:
            return "node_use";
        case node_search:
            return "node_search";
        case node_delete:
            return "node_delete";
        case node_insert:
            return "node_insert";
        case node_update:
            return "node_update";
        case node_sub_values:
            return "node_subvalue";
        case node_integer:
            return "node_integer";
        case node_string:
            return "node_string";
        case node_condition_less:
            return "node_condition_less";
        case node_condition_equals:
            return "node_condition_equals";
        case node_condition_greater:
            return "node_condition_greater";
        case node_exit:
            return "node_exit";
        default:
        {
            return "[!] PARSER SYNTAX ERROR : UNIDENTIFIED NODE -> " + local_node;
        }
        }
    }

    int parse()
    {
        switch (current_token.token_type)
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
            std::cout << red << "[!] PARSER SYNTAX ERROR : UNDEFINED TOKEN -> " << current_token.value << white << std::endl;
            return 1;
        }
        }
    }
};