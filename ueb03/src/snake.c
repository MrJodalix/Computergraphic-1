/**
 * @file
 * Schlangenkoerper-Modul.
 * Das Modul kapselt die Listenlogik. 
 * Dieses Modul beinhaltet die Operationen zur Verwaltung der Schlangenliste.
 *
 * @author Joshua-Scott Schoettke, Ilana Schmara (Gruppe 3).
 */

/* ---- System Header einbinden ---- */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* ---- Eigene Header einbinden ---- */
#include "snake.h"

/**
 * liefert leere Liste
 * @return leere liste
 */
List emptyList(void)
{
    return NULL;
}

/**
 * vergleicht auf leere Liste
 * @return leer oder nicht
 */
int isEmpty(List l)
{
    return l == emptyList();
}

/**
 * Konstruiert den Listenanfang
 * @param l die Liste
 * @param sp Element der Liste
 * @return die neue Liste
*/
List cons(List l, CGPoint3i sp)
{
    List res = malloc(sizeof(struct ListNode));
    if (res == NULL)
    {
        exit(1);
    }
    res->snakePart[0] = sp[0];
    res->snakePart[1] = sp[1];
    res->snakePart[2] = sp[2];
    res->next = l;
    return res;
}

/**
 * liefert das Element aus dem kopf der Liste
 * @param l die Liste
 * @return das Element
*/
CGPoint3i *
head(List l)
{
    assert(!isEmpty(l));

    return &l->snakePart;
}

/**
 * liefert das Element an einer bestimmten Stelle der liste zurueck
 * @param l die Liste
 * @param idx der Index
 * @return das Element
*/
CGPoint3i *
getAt(List l, int idx)
{
    for (; idx > 0; idx--)
    {
        assert(!isEmpty(l));
        l = l->next;
    }
    return &l->snakePart;
}

/**
 * liefert die Restliste ohne den Kopf
 * @param l die Liste
 * @return die Liste
*/
List 
tail(List l)
{
    assert(!isEmpty(l));
    return l->next;
}

/**
 * liefert die Laenge der Liste
 * @param l die liste
 * @return die Laenge der Liste
*/
unsigned int
length(List l)
{
    unsigned int res = 0;
    for (res = 0; !isEmpty(l); l = l->next)
        res++;
    return res;
}

/**
 * fuegt an der Stelle des idx das uebergebene Element in die Liste ein
 * @param l die liste
 * @param idx der Index
 * @param sp das Element
 * @return die erweiterte Liste
*/
List 
insertAt(List l, unsigned int idx, CGPoint3i sp)
{
    List run = l;
    if (idx == 0)
    {
        return cons(l, sp);
    }
    for (; idx > 1; idx--)
    {
        assert(!isEmpty(run));
        run = run->next;
    }
    run->next = cons(run->next, sp);
    return l;
}

/**
 * loescht das erste Element aus der Liste
 * @param l die liste
 * @return die reduzierte Liste
*/
List 
deleteFirst(List l)
{
    assert(!isEmpty(l));
    {
        List res = tail(l);
        free(l);
        return res;
    }
}

/**
 * loescht das Element am Index aus der Liste
 * @param l die liste
 * @param idx der index
 * @return die reduzierte Liste
*/
List 
removeAt(List l, unsigned int idx)
{
    List run = l;
    if (idx == 0)
    {
        return deleteFirst(l);
    }
    for (; idx > 1; idx--)
    {
        assert(!isEmpty(run));
        run = run->next;
    }
    run->next = deleteFirst(run->next);
    return l;
}

/**
 * loescht die Liste
 * @param l die liste
 * @return die leere Liste
*/
List deleteAll(List l)
{
    while (!isEmpty(l))
    {
        l = deleteFirst(l);
    }
    return emptyList();
}