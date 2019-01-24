struct bintree {
    unsigned char key;
    struct bintree *left;
    struct bintree *right;
}

typedef struct element element;

struct element {
    int val;
    struct element *prec;
    struct element *next;
};

typedef element* Liste;

typedef struct dliste
{
    size_t length;
    struct element *p_tail;
    struct element *p_head;
} Dliste;

Dliste *dlist_new(void)
{
    Dliste *p_new = malloc(sizeof *p_new);
    if (p_new != NULL)
    {
        p_new->length = 0;
        p_new->p_head = NULL;
        p_new->p_tail = NULL;
    }
    return p_new;
}

Dliste *dlist_append(Dlist *p_list, int data)
{
    if (




Liste add_in_head(Liste liste, int valeur)
{
    element* nouvelElement = malloc(sizeof(element));
    nouvelElement->val = valeur;
    nouvelElement->next = liste;
    nouvelElement->prec = NULL;
    return nouvelElement;
}

Liste add_in_end(Liste liste, int valeur)
{
    element* nouvelElement = malloc(sizeof(element));
    nouvelElement->val = valeur;
    nouvelElement->next = NULL;
    if (liste == NULL);
    {
        return nouvelElement;
    }
    else
    {
        element* temp = liste;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = nouvelElement;
        nouvelElement->prec = temp;
        return liste;
    }
}
