/*
 * OhioSkibidiC Compiler - Versione Completa
 * Include:
 *  - Lexer: analizza e converte il codice sorgente in token.
 *  - Parser: costruisce l'AST (Abstract Syntax Tree) dai token.
 *  - Code Generator: genera Assembly dal programma rappresentato come AST.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 64
#define MAX_SOURCE_LENGTH 1024
#define MAX_MESSAGE_COUNT 100

// ================================
// ENUM: Tipi di Token
// ================================
typedef enum {
    TOKEN_GYAT,        // int
    TOKEN_RIZZ,        // float
    TOKEN_YAP,         // char
    TOKEN_CASEOH,      // double
    TOKEN_TOILETSKIBIDI, // void

    TOKEN_BETA,        // if
    TOKEN_SIGMA,       // else
    TOKEN_CHAD,        // switch
    TOKEN_SLAY,        // case
    TOKEN_BASED,       // default

    TOKEN_EDGING,      // while
    TOKEN_MANGOPHONK,  // for
    TOKEN_GOONING,     // do

    TOKEN_NOMILK,      // return
    TOKEN_WOKE,        // break
    TOKEN_AUTOBLU,     // continue
    TOKEN_GETOUT,      // goto

    TOKEN_GRIND,       // sizeof
    TOKEN_YAPPING,     // typedef
    TOKEN_CUCK,        // include
    TOKEN_YAPPER,      // printf
    TOKEN_CASEOH_FUN,  // scanf

    TOKEN_IDENTIFIER,  // Identificatori
    TOKEN_NUMBER,      // Numeri
    TOKEN_STRING,      // Stringhe
    TOKEN_LPAREN,      // (
    TOKEN_RPAREN,      // )
    TOKEN_LBRACE,      // {
    TOKEN_RBRACE,      // }
    TOKEN_SEMICOLON,   // ;
    TOKEN_ASSIGN,      // =
    TOKEN_ASTERISK,    // *
    TOKEN_AMPERSAND,   // &
    TOKEN_PLUS,        // +
    TOKEN_MINUS,       // -
    TOKEN_MULTIPLY,    // *
    TOKEN_DIVIDE,      // /
    TOKEN_LT,          // <
    TOKEN_GT,          // >
    TOKEN_EQ,          // ==
    TOKEN_COMMA,       // ,
    TOKEN_EOF          // Fine del file
} TokenType;

// ================================
// STRUTTURA: Token
// ================================
typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
} Token;

// ================================
// STRUTTURA: Lexer
// ================================
typedef struct {
    const char* source;
    size_t pos;
    size_t length;
} Lexer;

// ================================
// STRUTTURA: Nodo AST
// ================================
typedef enum {
    NODE_PROGRAM,
    NODE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_IF_STATEMENT,
    NODE_WHILE_LOOP,
    NODE_FOR_LOOP,
    NODE_FUNCTION_CALL,
    NODE_LITERAL,
    NODE_IDENTIFIER
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char value[MAX_TOKEN_LENGTH]; // Per valori come nomi o numeri
    struct ASTNode* left;
    struct ASTNode* right;
    struct ASTNode* next;
} ASTNode;

// ================================
// Funzioni Lexer
// ================================
Lexer create_lexer(const char* source) {
    Lexer lexer = {source, 0, strlen(source)};
    return lexer;
}

Token next_token(Lexer* lexer) {
    while (lexer->pos < lexer->length) {
        char current = lexer->source[lexer->pos];

        if (isspace(current)) {
            lexer->pos++;
            continue;
        }

        if (isdigit(current)) {
            Token token = {TOKEN_NUMBER, {0}};
            size_t start = lexer->pos;
            while (isdigit(lexer->source[lexer->pos])) lexer->pos++;
            strncpy(token.value, lexer->source + start, lexer->pos - start);
            return token;
        }

        if (isalpha(current)) {
            Token token = {TOKEN_IDENTIFIER, {0}};
            size_t start = lexer->pos;
            while (isalnum(lexer->source[lexer->pos])) lexer->pos++;
            strncpy(token.value, lexer->source + start, lexer->pos - start);

            if (strcmp(token.value, "gyat") == 0) token.type = TOKEN_GYAT;
            else if (strcmp(token.value, "grimace") == 0) token.type = TOKEN_RIZZ;
            else if (strcmp(token.value, "yap") == 0) token.type = TOKEN_YAP;
            else if (strcmp(token.value, "caseoh") == 0) token.type = TOKEN_CASEOH;
            else if (strcmp(token.value, "toiletskibidi") == 0) token.type = TOKEN_TOILETSKIBIDI;
            else if (strcmp(token.value, "beta") == 0) token.type = TOKEN_BETA;
            else if (strcmp(token.value, "sigma") == 0) token.type = TOKEN_SIGMA;
            else if (strcmp(token.value, "yapper") == 0) token.type = TOKEN_YAPPER;
            return token;
        }

        if (current == '"') { // Stringhe
            lexer->pos++; // Salta il carattere iniziale "
            Token token = {TOKEN_STRING, {0}};
            size_t start = lexer->pos;
            while (lexer->source[lexer->pos] != '"' && lexer->pos < lexer->length) {
                lexer->pos++;
            }
            if (lexer->source[lexer->pos] == '"') {
                strncpy(token.value, lexer->source + start, lexer->pos - start);
                lexer->pos++; // Salta il carattere finale "
                return token;
            } else {
                fprintf(stderr, "Errore: stringa non terminata\n");
                exit(1);
            }
        }

        lexer->pos++;
        switch (current) {
            case '(' : return (Token){TOKEN_LPAREN, "("};
            case ')' : return (Token){TOKEN_RPAREN, ")"};
            case '{' : return (Token){TOKEN_LBRACE, "{"};
            case '}' : return (Token){TOKEN_RBRACE, "}"};
            case ';' : return (Token){TOKEN_SEMICOLON, ";"};
            case '=' : {
                if (lexer->source[lexer->pos] == '=') {
                    lexer->pos++;
                    return (Token){TOKEN_EQ, "=="};
                }
                return (Token){TOKEN_ASSIGN, "="};
            }
            case '<' : return (Token){TOKEN_LT, "<"};
            case '>' : return (Token){TOKEN_GT, ">"};
            case ',' : return (Token){TOKEN_COMMA, ","};
            default:
                fprintf(stderr, "Unrecognized character: %c\n", current);
                exit(1);
        }
    }

    return (Token){TOKEN_EOF, ""};
}

// ================================
// Funzioni Parser
// ================================
ASTNode* create_ast_node(NodeType type, const char* value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    strncpy(node->value, value, MAX_TOKEN_LENGTH);
    node->left = node->right = node->next = NULL;
    return node;
}

// Parsing di una funzione con più argomenti
ASTNode* parse_function_call(Lexer* lexer) {
    Token token = next_token(lexer); // Nome della funzione
    ASTNode* node = create_ast_node(NODE_FUNCTION_CALL, token.value);

    token = next_token(lexer); // (
    if (token.type != TOKEN_LPAREN) {
        fprintf(stderr, "Errore di sintassi: '(' atteso dopo %s\n", node->value);
        exit(1);
    }

    ASTNode* arg_head = NULL;
    ASTNode* arg_current = NULL;

    while (1) {
        token = next_token(lexer);
        if (token.type == TOKEN_RPAREN) break;

        ASTNode* arg = create_ast_node(NODE_LITERAL, token.value);
        if (!arg_head) {
            arg_head = arg;
        } else {
            arg_current->next = arg;
        }
        arg_current = arg;

        token = next_token(lexer);
        if (token.type == TOKEN_RPAREN) break;

        if (token.type != TOKEN_COMMA) {
            fprintf(stderr, "Errore di sintassi: ',' o ')' atteso\n");
            exit(1);
        }
    }

    node->left = arg_head;

    token = next_token(lexer); // ;
    if (token.type != TOKEN_SEMICOLON) {
        fprintf(stderr, "Errore di sintassi: ';' atteso dopo chiamata a funzione\n");
        exit(1);
    }

    return node;
}

// Parsing del corpo del programma
ASTNode* parse_statements(Lexer* lexer) {
    ASTNode* head = NULL;
    ASTNode* current = NULL;

    while (1) {
        Token token = next_token(lexer);
        if (token.type == TOKEN_RBRACE || token.type == TOKEN_EOF) break;

        ASTNode* statement = NULL;

        if (token.type == TOKEN_GYAT || token.type == TOKEN_RIZZ || token.type == TOKEN_YAP || token.type == TOKEN_CASEOH) {
            // Dichiarazione e assegnazione
            Token identifier = next_token(lexer);
            if (identifier.type != TOKEN_IDENTIFIER) {
                fprintf(stderr, "Errore di sintassi: identificatore atteso\n");
                exit(1);
            }

            Token assign = next_token(lexer);
            if (assign.type == TOKEN_ASSIGN) {
                Token value = next_token(lexer);
                if (value.type != TOKEN_NUMBER) {
                    fprintf(stderr, "Errore di sintassi: valore numerico atteso\n");
                    exit(1);
                }

                Token semicolon = next_token(lexer);
                if (semicolon.type != TOKEN_SEMICOLON) {
                    fprintf(stderr, "Errore di sintassi: ';' atteso\n");
                    exit(1);
                }

                statement = create_ast_node(NODE_ASSIGNMENT, identifier.value);
                statement->left = create_ast_node(NODE_LITERAL, value.value);
            }
        } else if (token.type == TOKEN_YAPPER) {
            statement = parse_function_call(lexer);
        } else {
            fprintf(stderr, "Errore di sintassi: token non riconosciuto '%s'\n", token.value);
            exit(1);
        }

        if (!head) {
            head = statement;
        } else {
            current->next = statement;
        }
        current = statement;
    }

    return head;
}

// Parsing del programma principale
ASTNode* parse_program(Lexer* lexer) {
    Token token = next_token(lexer);
    if (token.type == TOKEN_TOILETSKIBIDI) {
        token = next_token(lexer);
        if (token.type == TOKEN_IDENTIFIER && strcmp(token.value, "main") == 0) {
            token = next_token(lexer);
            if (token.type == TOKEN_LPAREN) {
                token = next_token(lexer);
                if (token.type == TOKEN_RPAREN) {
                    token = next_token(lexer);
                    if (token.type == TOKEN_LBRACE) {
                        ASTNode* program = create_ast_node(NODE_PROGRAM, "main");
                        program->left = parse_statements(lexer);
                        return program;
                    }
                }
            }
        }
    }

    fprintf(stderr, "Errore di sintassi: programma non valido\n");
    exit(1);
}

// Generazione del codice per puntatori e array
void generate_code(ASTNode* node, FILE* output, char messages[MAX_MESSAGE_COUNT][MAX_TOKEN_LENGTH], int* message_count) {
    if (!node) return;

    switch (node->type) {
        case NODE_PROGRAM:
            fprintf(output, "section .data\n");
            for (int i = 0; i < *message_count; i++) {
                fprintf(output, "    message%d db \"%s\", 0\n", i, messages[i]);
            }
            fprintf(output, "\nsection .text\n");
            fprintf(output, "global _start\n");
            fprintf(output, "_start:\n");
            generate_code(node->left, output, messages, message_count);
            fprintf(output, "    mov rax, 60 ; sys_exit\n");
            fprintf(output, "    xor rdi, rdi ; exit code 0\n");
            fprintf(output, "    syscall\n");
            break;

        case NODE_FUNCTION_CALL:
            if (strcmp(node->value, "yapper") == 0) {
                int message_index = *message_count;
                strncpy(messages[message_index], node->left->value, MAX_TOKEN_LENGTH);
                (*message_count)++;
                fprintf(output, "    mov rax, 1 ; syscall write\n");
                fprintf(output, "    mov rdi, 1 ; stdout\n");
                fprintf(output, "    mov rsi, message%d\n", message_index);
                fprintf(output, "    mov rdx, %lu\n", strlen(messages[message_index - 1]));
                fprintf(output, "    syscall\n");
            }
            break;

        default:
            fprintf(stderr, "Errore: Nodo non supportato\n");
            exit(1);
    }

    generate_code(node->next, output, messages, message_count);
}

void generate_program(ASTNode* ast, const char* output_file) {
    FILE* output = fopen(output_file, "w");
    if (!output) {
        perror("Errore nell'apertura del file di output");
        exit(1);
    }

    char messages[MAX_MESSAGE_COUNT][MAX_TOKEN_LENGTH] = {"Hello, World!"};
    int message_count = 1;

    generate_code(ast, output, messages, &message_count);
    fclose(output);
}

// Main
int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file.ohio> <output_file.asm>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        perror("Errore nell'apertura del file sorgente");
        return 1;
    }

    char* source = malloc(MAX_SOURCE_LENGTH);
    size_t length = fread(source, 1, MAX_SOURCE_LENGTH - 1, file);
    fclose(file);
    source[length] = '\0';

    Lexer lexer = create_lexer(source);
    ASTNode* program = parse_program(&lexer);
    generate_program(program, argv[2]);

    free(source);
    return 0;
}
