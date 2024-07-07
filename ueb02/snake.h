#ifndef __SNAKE_H__
#define __SNAKE_H__

/**
 * @file
 * Schlangenkoerper-Modul.
 * Das Modul kapselt die Listenlogik. 
 * Dieses Modul beinhaltet die Operationen zur Verwaltung der Schlangenliste.
 *
 * @author Joshua-Scott Schoettke, Ilana Schmara (Gruppe 3).
 */

/* ---- Header einbinden ---- */
#include "types.h"


/** Listenstruktur fuer die Schlange */
typedef struct ListNode *List;

struct ListNode {
    CGPoint3i snakePart;
    List next;
};

/**
 * liefert leere Liste
 * @return leere liste
 */
List 
emptyList(void);

/**
 * vergleicht auf leere Liste
 * @return leer oder nicht
 */
int 
isEmpty(List l);

/**
 * Konstruiert den Listenanfang
 * @param l die Liste
 * @param sp Element der Liste
 * @return die neue Liste
*/
List 
cons (List l, CGPoint3i sp);

/**
 * liefert das Element aus dem kopf der Liste
 * @param l die Liste
 * @return das Element
*/
CGPoint3i 
*head (List l);

/**
 * liefert das Element an einer bestimmten Stelle der liste zurueck
 * @param l die Liste
 * @param idx der Index
 * @return das Element
*/
CGPoint3i *
getAt (List l, int idx);

/**
 * liefert die Restliste ohne den Kopf
 * @param l die Liste
 * @return die Liste
*/
List 
tail (List l);

/**
 * liefert die Laenge der Liste
 * @param l die liste
 * @return die Laenge der Liste
*/
unsigned int 
length(List l);

/**
 * fuegt an der Stelle des idx das uebergebene Element in die Liste ein
 * @param l die liste
 * @param idx der Index
 * @param sp das Element
 * @return die erweiterte Liste
*/
List 
insertAt(List l, unsigned int idx, CGPoint3i sp);

/**
 * loescht das erste Element aus der Liste
 * @param l die liste
 * @return die reduzierte Liste
*/
List
deleteFirst(List l);

/**
 * loescht das Element am Index aus der Liste
 * @param l die liste
 * @param idx der index
 * @return die reduzierte Liste
*/
List
removeAt(List l, unsigned int idx);

/**
 * loescht die Liste
 * @param l die liste
 * @return die leere Liste
*/
List
deleteAll(List l);
#endif