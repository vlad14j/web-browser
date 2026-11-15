#ifndef BROWSER_H
#define BROWSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for a web page
struct page {
    int id;
    char url[50];
    char *description;
};

// Structure for the history stack
struct stack {
    struct page *pages;
    int top;
};

// Structure for a tab
struct tab {
    int id;
    struct page *currentPage;
    struct stack *backwardStack;
    struct stack *forwardStack;
};

// Structure for a doubly linked list node
struct lista {
    struct tab *tab;
    struct lista *next;
    struct lista *prev;
};

//Santinela
struct tabsList {
    struct lista *head;
};

// Main browser structure
struct browser {
    struct tab *current;
    struct tabsList list;
};

// Prototypes for stack and page management functions
void push(struct stack *stiva, struct page *pagina);
struct page *pop(struct stack *s);
void clear_stack(struct stack *s);
void free_stack(struct stack *s);
void free_page(struct page *p);

// Prototypes for browser operation functions
void new_tab(struct browser *b, int *last_id); //homepage
void close_tab(struct browser *b, FILE *out);
void open_tab(struct browser *browser, int tab_id, FILE *out);
void print_tabs(struct browser *b, FILE *out);
void next_tab(struct browser *b);
void prev_tab(struct browser *b);
void page_id(struct browser *b, int id, struct page *pages, int num_pages, FILE *out);
void backward(struct browser *b, FILE *out);
void forward(struct browser *b, FILE *out);
void print_history(struct browser *b, int id, FILE *out);

#endif