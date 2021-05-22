#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

size_t NUM_WALLET_ADDRESS_SYMBOLS = 16;
size_t LEN_WALLET_ADDRESS = 40;
size_t NUM_WALLET_ADDRESS = 2000000;

struct prefix_tree_node
{
    struct prefix_tree_node** children;
    int level;
    int value;
};

struct prefix_tree_node* prefix_tree_root;

int char_to_index_map(char c)
{
    if(c >= 65 && c <= 70)
    {
        return (c - 65) + 10;
    }
    else if(c >= 48 && c <= 57)
    {
        return (c - 48);
    }
    else
    {
        return -1;
    }
}

struct prefix_tree_node* initialize_prefix_tree_node(int level)
{
    struct prefix_tree_node* temp_prefix_tree_node = (struct prefix_tree_node*) malloc(sizeof(struct prefix_tree_node));
    temp_prefix_tree_node -> children = (struct prefix_tree_node**) calloc(sizeof(struct prefix_tree_node*), NUM_WALLET_ADDRESS_SYMBOLS);
    temp_prefix_tree_node -> level = level;
    return temp_prefix_tree_node;
}

void insert_to_prefix_tree(struct prefix_tree_node* prefix_tree_root, char* address, int value)
{
    int length = strlen(address);
    int i;
    int index;
    struct prefix_tree_node* node_pointer = prefix_tree_root;

    for(int i=0; i<length; ++i)
    {
        index = char_to_index_map(address[i]);
        if(index == -1)
        {
            break;
        }

        if(node_pointer -> children[index])
        {
            // printf("Inside if (level_pointer -> children)");
            
            node_pointer = node_pointer -> children[index];
        }
        else
        {
            // printf("Inside else (level_pointer -> children)");
            node_pointer -> children[index] = initialize_prefix_tree_node(i);
            node_pointer = node_pointer -> children[index];
        }

        if(i == length - 1)
        {
            node_pointer -> value = value;
        }
    }
}

int read_from_prefix_tree(struct prefix_tree_node* prefix_tree_root, char* address)
{
    int length = strlen(address);
    int i;
    int index;
    struct prefix_tree_node* node_pointer = prefix_tree_root;

    for(int i=0; i<length; ++i)
    {
        index = char_to_index_map(address[i]);
        if(index == -1)
        {
            break;
        }
        if(node_pointer-> children[index])
        {
            node_pointer = node_pointer -> children[index];
        }
        else
        {
            break;
        }
    }

    if(node_pointer)
    {
        return node_pointer -> value;
    }
}

int random_value_generator(int max)
{
    return rand() % (max + 1);
}

char* random_address_generator()
{
    char symbols[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    int random_symbol_index;
    int i;
    // 0x40_RANDOM_HEX_DIGITS\0 - 43 characters
    char* random_address = calloc(sizeof(char), LEN_WALLET_ADDRESS + 3);

    random_address[0] = '0';
    random_address[1] = 'x';

    for(i=0; i<LEN_WALLET_ADDRESS; ++i)
    {
        random_symbol_index = random_value_generator(NUM_WALLET_ADDRESS_SYMBOLS - 1);
        random_address[2 + i] = symbols[random_symbol_index];
    }
    random_address[42] =  '\0';

    return random_address;
}


char* drop_0x(char* wallet_address)
{
    return wallet_address + 2;
}

int main()
{
    int return_value;
    int insert_value;
    prefix_tree_root = initialize_prefix_tree_node(0);
    char* wallet_address;
    int i;

    srand(time(0));

    for(i=0; i<NUM_WALLET_ADDRESS; ++i)
    {
        wallet_address = random_address_generator();
        printf("Addr : %s\n", wallet_address);
        insert_value = random_value_generator(100);
        insert_to_prefix_tree(prefix_tree_root, drop_0x(wallet_address), insert_value);
        printf("%d\tInsert to prefix_tree: %s. Balance: %d\n", i, drop_0x(wallet_address), insert_value);
        return_value = read_from_prefix_tree(prefix_tree_root, drop_0x(wallet_address));
        printf("Return value from prefix_tree: %d\n\n", return_value);
        assert(insert_value == return_value);
    }
    return 0;
}