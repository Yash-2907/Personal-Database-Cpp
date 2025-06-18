#include "headers.hpp"
#include "lexer.hpp"

class parser
{
private:
    typedef enum
    {
        node_create_database,
        node_create_table,
        node_use,
        node_search,
        node_delete,
        node_insert,
        node_update,
        node_sub_values,
        node_exit
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
                      << red << "[!] ERROR : EXPECTED " << lexer_obj.token_type_to_string(*required_token.begin()) << " BUT RECIEVED " << lexer_obj.token_type_to_string(current_token.token_type) << " ==> { " << current_token.value << " } " << white << std::endl;
            return 1;
        }
        previous_token = current_token;
        index++;
        current_token = local_token_list[index];
        return 0;
    }

    bool check(std::set<token_set> required)
    {
        return required.find(current_token.token_type) != required.end();
    }

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
        // syntax to be followed : create new database/table name_here
        if (advance({token_create}) == 1)
            return 1;
        if (advance({token_new}) == 1)
            return 1;
        if (advance({token_database, token_table}) == 1)
            return 1;
        else
            evaluated_node.node_type = previous_token.token_type == token_database ? node_create_database : node_create_table;
        if (advance({token_id}) == 1)
            return 1;
        else
            evaluated_node.payload = previous_token.value;
        if(advance({token_end_of_input})==1)
            return 1;
        std::cout<<"so this was parsed and the operation to be perfomed is : "<<node_type_to_string(evaluated_node.node_type)<<" where the name given in payload will be : "<<evaluated_node.payload<<std::endl;
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
        case node_exit:
            return "node_exit";
        default:
        {
            return "[!] SYTNAX ERROR : UNIDENTIFIED NODE -> " + local_node;
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
            std::cout << red << "[!] SYNTAX ERROR : UNDEFINED TOKEN -> " << current_token.value << white << std::endl;
            return 1;
        }
        }
    }
};