#include "browser.h"

int main() {
    struct browser Browser;
    // Initialize the sentinel node for the doubly linked list
    Browser.list.head = malloc(sizeof(struct lista));
    Browser.list.head->tab = NULL;
    Browser.list.head->next = Browser.list.head->prev = Browser.list.head;
    Browser.current = NULL;

    struct page pages[50];
    char sir[100], comanda[100];
    int nrpag, last_id = 0;

    FILE *in = fopen("input.in", "r");
    FILE *out = fopen("output.out", "w");
    if(!in || !out) return 1;

    // Reading page resources
    if(fscanf(in, "%d", &nrpag) != 1) return 1;
    for (int i = 0; i < nrpag; i++) {
        fscanf(in, "%d %s", &pages[i].id, pages[i].url);
        fgetc(in); fgets(sir, sizeof(sir), in);
        pages[i].description = strdup(sir);
    }

    // Open initial tab
    new_tab(&Browser, &last_id);

    // Main command processing loop
    while (fscanf(in, "%s", comanda) == 1) {
        if (strcmp(comanda, "NEW_TAB") == 0) new_tab(&Browser, &last_id);
        else if (strcmp(comanda, "CLOSE") == 0) close_tab(&Browser, out);
        else if (strcmp(comanda, "OPEN") == 0) { int id; fscanf(in, "%d", &id); open_tab(&Browser, id, out); }
        else if (strcmp(comanda, "PRINT") == 0) print_tabs(&Browser, out);
        else if (strcmp(comanda, "NEXT") == 0) next_tab(&Browser);
        else if (strcmp(comanda, "PREV") == 0) prev_tab(&Browser);
        else if (strcmp(comanda, "PAGE") == 0) { int pid; fscanf(in, "%d", &pid); page_id(&Browser, pid, pages, nrpag, out); }
        else if (strcmp(comanda, "BACKWARD") == 0) backward(&Browser, out);
        else if (strcmp(comanda, "FORWARD") == 0) forward(&Browser, out);
        else if (strcmp(comanda, "PRINT_HISTORY") == 0) { int id; fscanf(in, "%d", &id); print_history(&Browser, id, out); }
    }

    // Free memory resources
    struct lista *curr = Browser.list.head->next;
    while (curr != Browser.list.head) {
        struct lista *next = curr->next;
        free_page(curr->tab->currentPage);
        free_stack(curr->tab->backwardStack);
        free_stack(curr->tab->forwardStack);
        free(curr->tab); free(curr);
        curr = next;
    }
    free(Browser.list.head);
    for (int i = 0; i < nrpag; i++) free(pages[i].description);

    fclose(in); fclose(out);
    return 0;
}