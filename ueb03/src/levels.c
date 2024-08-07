/**
 * @file
 * Levelmodul.
 * Das Modul kapselt die Level des Programms.
 *
 * @author Joshua-Scott Schoettke, Ilana Schmara (Gruppe 3).
 */

/* ---- Eigene Header einbinden ---- */
#include "levels.h"

/* Erstes Level */
esLevel level1 =
{ 
  {FT_WALL, FT_WALL, FT_WALL, FT_WALL, FT_FREE, FT_FREE, FT_WALL, FT_WALL, FT_WALL, FT_WALL},
  {FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL},
  {FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL},
  {FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL},
  {FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL, FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE},
  {FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL, FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE},
  {FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL},
  {FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL},
  {FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL},
  {FT_WALL, FT_WALL, FT_WALL, FT_WALL, FT_FREE, FT_FREE, FT_WALL, FT_WALL, FT_WALL, FT_WALL},
};

/* Zweites Level */
esLevel level2 =
{ 
  {FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL, FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE},
  {FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE},
  {FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL, FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE},
  {FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL, FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE},
  {FT_WALL, FT_FREE, FT_WALL, FT_WALL, FT_WALL, FT_WALL, FT_WALL, FT_WALL, FT_WALL, FT_WALL},
  {FT_WALL, FT_FREE, FT_WALL, FT_WALL, FT_WALL, FT_WALL, FT_WALL, FT_WALL, FT_WALL, FT_WALL},
  {FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL, FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE},
  {FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL, FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE},
  {FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL, FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE},
  {FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL, FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE},
};

/* Drittes Level */
esLevel level3 =
{ 
    {FT_WALL, FT_FREE, FT_FREE, FT_WALL, FT_FREE, FT_FREE, FT_WALL, FT_FREE, FT_FREE, FT_WALL},
    {FT_FREE, FT_FREE, FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL, FT_FREE, FT_FREE},
    {FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL},
    {FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL},
    {FT_FREE, FT_FREE, FT_WALL, FT_FREE, FT_WALL, FT_WALL, FT_FREE, FT_WALL, FT_FREE, FT_FREE},
    {FT_FREE, FT_FREE, FT_WALL, FT_FREE, FT_WALL, FT_WALL, FT_FREE, FT_WALL, FT_FREE, FT_FREE},
    {FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL},
    {FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL},
    {FT_FREE, FT_FREE, FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL, FT_FREE, FT_FREE},
    {FT_WALL, FT_FREE, FT_FREE, FT_WALL, FT_FREE, FT_FREE, FT_WALL, FT_FREE, FT_FREE, FT_WALL},
};

/* Viertes Level */
esLevel level4 =
{ 
    {FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL},
    {FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE},
    {FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE},
    {FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE},
    {FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE},
    {FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE},
    {FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE},
    {FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE},
    {FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE},
    {FT_WALL, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_FREE, FT_WALL},
};

/**
 * Liefert das aktuelle Spielfeld
 * @param level das Level welches gewaehlt wurde
 * @return liefert das Level zurueck
 */
esLevel *
getLevel (int level)
{
    switch (level)
    {
    case 1:
        return &level1;
        break;
    case 2:
        return &level2;
        break;
    case 3:
        return &level3;
        break;
    case 4:
        return &level4;
        break;
    default:
        return &level1;
    }
    
}