#include "browser.h"

// PUSH operation for the history stack
void push(struct stack *stiva, struct page *pagina) {
    struct page *temp = realloc(stiva->pages, sizeof(struct page) * (stiva->top + 2));
    if (!temp) return;
    stiva->pages = temp;
    stiva->top++;//varf
    stiva->pages[stiva->top].id = pagina->id;
    strcpy(stiva->pages[stiva->top].url, pagina->url);
    stiva->pages[stiva->top].description = strdup(pagina->description);
}

// POP operation for backward/forward navigation
struct page *pop(struct stack *s) {
    if (!s || s->top == -1) return NULL;
    struct page *pagina_extrasa = malloc(sizeof(struct page));
    if (!pagina_extrasa) return NULL;

    
    pagina_extrasa->id = s->pages[s->top].id;
    strcpy(pagina_extrasa->url, s->pages[s->top].url);
    pagina_extrasa->description = s->pages[s->top].description;
    s->pages[s->top].description = NULL;
    s->top--;
    return pagina_extrasa;
}
//eleminate elements after going through stack(dynamic allcation)
void clear_stack(struct stack *s) {
    for (int i = 0; i <= s->top; i++) free(s->pages[i].description);
    free(s->pages);
    s->pages = NULL; s->top = -1;
}
//free the memory allocated for stack
void free_stack(struct stack *s) {
    if (!s) return;
    clear_stack(s);
    free(s);
}
//free the memory allocated for page
void free_page(struct page *p) {
    if (p) { free(p->description); free(p); }
}

// Creates a new tab with the default page
void new_tab(struct browser *b, int *last_id) {
    struct tab *newTab = malloc(sizeof(struct tab));
    if(!newTab) return;
    newTab->id = (*last_id)++;
    newTab->currentPage = malloc(sizeof(struct page));
     
    strcpy(newTab->currentPage->url, "https://homepage.com/");
    newTab->currentPage->description = strdup("Home Page\n");
    newTab->currentPage->id = 0;

    newTab->backwardStack = malloc(sizeof(struct stack));
    newTab->forwardStack = malloc(sizeof(struct stack));
    newTab->backwardStack->pages = NULL; newTab->backwardStack->top = -1;
    newTab->forwardStack->pages = NULL; newTab->forwardStack->top = -1;

    struct lista *nod = malloc(sizeof(struct lista));
    nod->tab = newTab;
    struct lista *santinela = b->list.head;
    struct lista *last = santinela->prev;
    nod->next = santinela;
    nod->prev = last;
    last->next = nod;
    santinela->prev = nod;
    b->current = newTab;
}

// Closes the current tab (except for ID 0)
void close_tab(struct browser *b, FILE *out) {
    if (!b || !b->current) { fprintf(out, "403 Forbidden\n"); return; }
    if (b->current->id == 0) { fprintf(out, "403 Forbidden\n"); return; }
    //legatura santinela si el urmator
    struct lista *el = b->list.head->next;
    while (el != b->list.head && el->tab != b->current) el = el->next;

    struct lista *prev_node = el->prev;
    if (prev_node == b->list.head) prev_node = prev_node->prev;
    b->current = prev_node->tab;

    el->prev->next = el->next;
    el->next->prev = el->prev;
    free_page(el->tab->currentPage);
    free_stack(el->tab->backwardStack);
    free_stack(el->tab->forwardStack);
    free(el->tab); free(el);
}
//search for tab with given id and make it current
void open_tab(struct browser *browser, int tab_id, FILE *out) {
    struct lista *curr = browser->list.head->next;
    while (curr != browser->list.head) {
        if (curr->tab->id == tab_id) {
            browser->current = curr->tab;
            return;
        }
        curr = curr->next;
    }
    fprintf(out, "403 Forbidden\n");
}
//parcurge lista crcular si afiseaza id-urile taburilor
void print_tabs(struct browser *b, FILE *out) {
    if (!b || !b->current) return;
    struct lista *el = b->list.head->next;
    while (el != b->list.head && el->tab != b->current) el = el->next;//last element
    struct lista *start = el;
    do {
        fprintf(out, "%d ", el->tab->id);
        el = el->next;
        if (el == b->list.head) el = el->next;
    } while (el != start);
    fprintf(out, "\n%s", b->current->currentPage->description);
}
//moves the pointer to the next tab in the list
void next_tab(struct browser *b) {
    struct lista *el = b->list.head->next;
    while (el != b->list.head && el->tab != b->current) el = el->next;
    el = el->next;
    if (el == b->list.head) el = el->next;
    b->current = el->tab;
}
//moves the pointer to the previous tab in the list
void prev_tab(struct browser *b) {
    struct lista *el = b->list.head->next;
    while (el != b->list.head && el->tab != b->current) el = el->next;
    el = el->prev;
    if (el == b->list.head) el = el->prev;
    b->current = el->tab;
}
//opens a new page with a specific ID  in the current tab
void page_id(struct browser *b, int id, struct page *pages, int num_pages, FILE *out) {
    struct page *target = NULL;
    for (int i = 0; i < num_pages; i++) {
        if (pages[i].id == id) { target = &pages[i]; break; }
    }
    if (!target) { fprintf(out, "403 Forbidden\n"); return; }

    push(b->current->backwardStack, b->current->currentPage);
    clear_stack(b->current->forwardStack);
    if (b->current->currentPage->id != 0) free_page(b->current->currentPage);

    b->current->currentPage = malloc(sizeof(struct page));
    b->current->currentPage->id = target->id;
    strcpy(b->current->currentPage->url, target->url);
    b->current->currentPage->description = strdup(target->description);
}
//traverse backward/forward stack to navigate through history
void backward(struct browser *b, FILE *out) {
    if (b->current->backwardStack->top == -1) { fprintf(out, "403 Forbidden\n"); return; }
    push(b->current->forwardStack, b->current->currentPage);
    free_page(b->current->currentPage);
    b->current->currentPage = pop(b->current->backwardStack);
}

void forward(struct browser *b, FILE *out) {
    if (b->current->forwardStack->top == -1) { fprintf(out, "403 Forbidden\n"); return; }
    push(b->current->backwardStack, b->current->currentPage);
    free_page(b->current->currentPage);
    b->current->currentPage = pop(b->current->forwardStack);
}

void print_history(struct browser *b, int id, FILE *out) {
    struct lista *el = b->list.head->next;
    while (el != b->list.head) {
        if (el->tab->id == id) {
            for (int i = 0; i <= el->tab->forwardStack->top; i++)
                fprintf(out, "%s\n", el->tab->forwardStack->pages[i].url);
            fprintf(out, "%s\n", el->tab->currentPage->url);
            for (int i = el->tab->backwardStack->top; i >= 0; i--)
                fprintf(out, "%s\n", el->tab->backwardStack->pages[i].url);
            return;
        }
        el = el->next;
    }
    fprintf(out, "403 Forbidden\n");
}