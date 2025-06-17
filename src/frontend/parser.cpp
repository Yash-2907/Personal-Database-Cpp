#include "headers.hpp"
#include "lexer.hpp"

class parser
{
private:
    lexer &lexer_obj;
    std::vector<token> &local_token_list;
    token current_token;
    int index=0,length=0;
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
        std::string id_payload;
        std::vector<ast_node> children;
    };

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
    parser(lexer &lexer_obj) : lexer_obj(lexer_obj), local_token_list(lexer_obj.fetch_vector()){
        length=local_token_list.size();
        index=0;
        current_token=local_token_list[0];
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
        for (auto &it : local_token_list)
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
                std::cout << red << "[!] SYNTAX ERROR : UNDEFINED TOKEN -> " << it.value << white << std::endl;
                return 1;
            }
            }
        }
    }
};