#ifndef __LEVELS_H__
#define __LEVELS_H__
/**
 * @file
 * Schnittstelle des Levelmoduls.
 * Das Modul kapselt die Level des Programms.
 *
 * @author Joshua-Scott Schoettke, Ilana Schmara (Gruppe 3).
 */

/** Spielfeldgroesze */
#define FIELDSIZE           10

/*
 * Art von Levelfeldern
 * FT_FREE: leeres Feld
 * FT_WALL: Mauer
 * FT_EXPLODED : Explosion
 */
typedef enum { FT_FREE, FT_WALL, FT_EXPLODED } esFieldType;

/* Spielfeld */
typedef esFieldType esLevel[FIELDSIZE][FIELDSIZE];

/* Zeiger auf ein Spielfeld */
typedef esFieldType (*esLevelPointer)[FIELDSIZE];

/**
 * Liefert das aktuelle Spielfeld
 * @param level das Level welches gewaehlt wurde
 * @return liefert das Level zurueck
 */
esLevel *
getLevel (int level);

#endif