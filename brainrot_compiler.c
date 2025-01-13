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

// ================================
// ENUM: Tipi di Token
// ================================
typedef enum {
    TOKEN_GYAT,        // int
    TOKEN_GRIMACE,     // float
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
            else if (strcmp(token.value, "grimace") == 0) token.type = TOKEN_GRIMACE;
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

// Funzione principale del Parser (esempio base)
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
                        // Costruire l'AST per il corpo della funzione
                        ASTNode* program = create_ast_node(NODE_PROGRAM, "main");
                        program->left = NULL; // Corpo della funzione (da implementare)
                        return program;
                    }
                }
            }
        }
    }
    fprintf(stderr, "Errore di sintassi: programma non valido\n");
    exit(1);
}

// ================================
// Funzioni Code Generator
// ================================
void generate_code(ASTNode* node, FILE* output) {
    if (!node) return;

    switch (node->type) {
        case NODE_PROGRAM:
            fprintf(output, "section .text\n");
            fprintf(output, "global _start\n");
            fprintf(output, "_start:\n");
            generate_code(node->left, output);
            fprintf(output, "    mov rax, 60 ; sys_exit\n");
                       fprintf(output, "    xor rdi, rdi ; exit code 0\n");
            fprintf(output, "    syscall\n");
            break;

        case NODE_DECLARATION:
            fprintf(output, "    ; Dichiarazione di variabile: %s\n", node->value);
            fprintf(output, "    mov [rbp-%d], 0 ; Inizializza a 0\n", 8); // Stack offset
            break;

        case NODE_ASSIGNMENT:
            fprintf(output, "    ; Assegna valore a %s\n", node->value);
            fprintf(output, "    mov rax, %s\n", node->right->value);
            fprintf(output, "    mov [rbp-%d], rax\n", 8); // Stack offset
            break;

        case NODE_IF_STATEMENT:
            fprintf(output, "    ; Inizio if\n");
            generate_code(node->left, output); // Condizione
            fprintf(output, "    cmp rax, 0\n");
            fprintf(output, "    je else_%s\n", node->value); // Salto condizionato
            generate_code(node->right, output); // Corpo if
            fprintf(output, "else_%s:\n", node->value);
            break;

        case NODE_FUNCTION_CALL:
            fprintf(output, "    ; Chiamata a funzione: %s\n", node->value);
            if (strcmp(node->value, "yapper") == 0) {
                fprintf(output, "    mov rdi, message\n");
                fprintf(output, "    call printf\n");
            }
            break;

        default:
            fprintf(stderr, "Errore: Nodo non supportato\n");
            exit(1);
    }

    generate_code(node->next, output); // Nodo successivo
}

void generate_program(ASTNode* ast, const char* output_file) {
    FILE* output = fopen(output_file, "w");
    if (!output) {
        perror("Errore nell'apertura del file di output");
        exit(1);
    }

    // Header Assembly
    fprintf(output, "section .data\n");
    fprintf(output, "    message db \"Hello, World!\", 0\n\n");
    fprintf(output, "section .text\n");
    fprintf(output, "    global _start\n\n");
    fprintf(output, "_start:\n");

    // Genera il codice dal programma
    generate_code(ast, output);

    // Footer Assembly
    fprintf(output, "    mov rax, 60 ; sys_exit\n");
    fprintf(output, "    xor rdi, rdi ; exit code 0\n");
    fprintf(output, "    syscall\n");

    fclose(output);
}

// ================================
// Main per Lexer, Parser e Code Generator
// ================================
int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file.ohio> <output_file.asm>\n", argv[0]);
        return 1;
    }

    // Leggere il file sorgente
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        perror("Errore nell'apertura del file sorgente");
        return 1;
    }

    char* source = malloc(MAX_SOURCE_LENGTH);
    size_t length = fread(source, 1, MAX_SOURCE_LENGTH - 1, file);
    fclose(file);

    source[length] = '\0'; // Terminare la stringa

    // Lexer
    Lexer lexer = create_lexer(source);
    printf("Tokenizzazione del sorgente:\n");
    Token token;
    do {
        token = next_token(&lexer);
        printf("Token: Type=%d, Value=%s\n", token.type, token.value);
    } while (token.type != TOKEN_EOF);
    lexer.pos = 0; // Resettare il lexer per il parser

    // Parser
    printf("\nParsing del programma:\n");
    ASTNode* program = parse_program(&lexer);
    if (program) {
        printf("AST Root: %s\n", program->value);
    }

    // Code Generator
    printf("\nGenerazione del codice Assembly:\n");
    generate_program(program, argv[2]);

    free(source);
    return 0;
}
