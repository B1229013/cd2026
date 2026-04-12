#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
 * HW1: Crap-C Scanner (Lexer) — DFA-based
 * Based on hw0 (cd2026-main/main.c) character-reading structure.
 * Constraints: only <stdio.h>, <stdlib.h>, <string.h>. Pure C.
 * ============================================================ */

/* --- Token type names --- */
#define MAX_ID_LEN 256

/* DFA States */
typedef enum {
    S_START,
    S_ID,
    S_NUM,
    S_EQ,     /* saw '=', might be '==' */
    S_GT,     /* saw '>', might be '>=' */
    S_LT,     /* saw '<', might be '<=' */
    S_DONE
} State;

/* -------------------------------------------------------
 * keyword_token: returns the keyword token string if buf
 * matches a keyword, otherwise returns NULL.
 * ------------------------------------------------------- */
const char* keyword_token(const char *buf)
{
    if (strcmp(buf, "int")   == 0) return "TYPE_TOKEN";
    if (strcmp(buf, "main")  == 0) return "MAIN_TOKEN";
    if (strcmp(buf, "if")    == 0) return "IF_TOKEN";
    if (strcmp(buf, "else")  == 0) return "ELSE_TOKEN";
    if (strcmp(buf, "while") == 0) return "WHILE_TOKEN";
    return NULL;
}

/* -------------------------------------------------------
 * is_letter: returns 1 if c is a-z, A-Z, or underscore
 * ------------------------------------------------------- */
int is_letter(int c)
{
    return ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            c == '_');
}

/* -------------------------------------------------------
 * is_digit: returns 1 if c is 0-9
 * ------------------------------------------------------- */
int is_digit(int c)
{
    return (c >= '0' && c <= '9');
}

/* -------------------------------------------------------
 * is_alnum_or_under: digit OR letter OR underscore
 * ------------------------------------------------------- */
int is_alnum_or_under(int c)
{
    return (is_letter(c) || is_digit(c));
}

/* -------------------------------------------------------
 * is_space: space, tab, newline, carriage return
 * ------------------------------------------------------- */
int is_space(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

/* -------------------------------------------------------
 * lexical_error: prints an error and exits
 * ------------------------------------------------------- */
void lexical_error(int c)
{
    fprintf(stderr, "Lexical error: unexpected character '%c' (0x%02X)\n", c, (unsigned char)c);
    exit(1);
}

/* ============================================================
 * MAIN — the DFA loop
 * Replaces hw0's while((c = fgetc(fp)) != EOF) loop and
 * the linked-list accumulator with a state-machine dispatcher.
 * ============================================================ */
int main(void)
{
    int      c;                   /* current character (int for EOF) */
    int      next;                /* lookahead character */
    char     buf[MAX_ID_LEN];     /* accumulator for id / number */
    int      buf_len;
    const char *kw;

    /* --------------------------------------------------------
     * Main DFA loop — mirrors hw0's while(fgetc) structure.
     * -------------------------------------------------------- */
    while ((c = fgetc(stdin)) != EOF)
    {

        /* ---- START state dispatch ---- */
        if (is_space(c))
        {
            /* Whitespace: skip (slide says ignore ' ', '\n', '\t') */
            continue;
        }
        else if (is_letter(c))
        {
            /* ---- ID / Keyword path ---- */
            /*
             * ID ::= LETTER { LETTER | DIGIT | UNDERSCORE }
             */
            buf[0] = (char)c;
            buf_len = 1;

            while ((c = fgetc(stdin)) != EOF && is_alnum_or_under(c))
            {
                if (buf_len < MAX_ID_LEN - 1)
                    buf[buf_len++] = (char)c;
            }
            buf[buf_len] = '\0';

            /* Put back the char that ended the identifier */
            if (c != EOF)
                ungetc(c, stdin);

            /* Check if it's a keyword */
            kw = keyword_token(buf);
            if (kw != NULL)
                printf("%s: %s\n", buf, kw);
            else
                printf("%s: ID_TOKEN\n", buf);
        }
        else if (is_digit(c))
        {
            /* ---- Number path ---- */
            /*
             * INTLITERAL ::= DIGIT { DIGIT }
             */
            buf[0] = (char)c;
            buf_len = 1;

            while ((c = fgetc(stdin)) != EOF && is_digit(c))
            {
                if (buf_len < MAX_ID_LEN - 1)
                    buf[buf_len++] = (char)c;
            }
            buf[buf_len] = '\0';

            if (c != EOF)
                ungetc(c, stdin);

            printf("%s: LITERAL_TOKEN\n", buf);
        }
        else if (c == '=')
        {
            /* ---- '=' or '==' ---- */
            next = fgetc(stdin);
            if (next == '=')
                printf("==: EQUAL_TOKEN\n");
            else
            {
                /* single '=' is assignment */
                if (next != EOF)
                    ungetc(next, stdin);
                printf("=: ASSIGN_TOKEN\n");
            }
        }
        else if (c == '>')
        {
            /* ---- '>' or '>=' ---- */
            next = fgetc(stdin);
            if (next == '=')
                printf(">=: GREATEREQUAL_TOKEN\n");
            else
            {
                if (next != EOF)
                    ungetc(next, stdin);
                printf(">: GREATER_TOKEN\n");
            }
        }
        else if (c == '<')
        {
            /* ---- '<' or '<=' ---- */
            next = fgetc(stdin);
            if (next == '=')
                printf("<=: LESSEQUAL_TOKEN\n");
            else
            {
                if (next != EOF)
                    ungetc(next, stdin);
                printf("<: LESS_TOKEN\n");
            }
        }
        else if (c == '+')  { printf("+: PLUS_TOKEN\n");          }
        else if (c == '-')  { printf("-: MINUS_TOKEN\n");         }
        else if (c == '(')  { printf("(: LEFTPAREN_TOKEN\n");     }
        else if (c == ')')  { printf("): RIGHTPAREN_TOKEN\n");    }
        else if (c == '{')  { printf("{: LEFTBRACE_TOKEN\n");     }
        else if (c == '}')  { printf("}: RIGHTBRACE_TOKEN\n");    }
        else if (c == ',')  { printf(",: COMMA_TOKEN\n");         }
        else if (c == ';')  { printf(";: SEMICOLON_TOKEN\n");     }
        else
        {
            /* Unrecognized character */
            lexical_error(c);
        }
    }

    return 0;
}