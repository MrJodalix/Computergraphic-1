#ifndef __TYPES_H__
#define __TYPES_H__
/**
 * @file
 * Typenschnittstelle.
 * Das Modul kapselt die Typdefinitionen und globalen Konstanten des Programms.
 *
 * @author Joshua-Scott Schoettke, Ilana Schmara (Gruppe 3).
 */

/* ---- System Header einbinden ---- */
#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

/* ---- Konstanten ---- */
/** x-Koordinate linker Rand */
#define BORDER_LEFT        -0.9f
/** x-Koordinate rechter Rand */
#define BORDER_RIGHT        0.9f
/** y-Koordinate oberer Rand */
#define BORDER_TOP          0.9f
/** y-Koordinate unterer Rand */
#define BORDER_BOTTOM      -0.9f

/** Seite des Quadrats */
#define QUAD_SIDE           0.18f

#define FIELDBORDER         -0.81f

// Startposition X
#define STARTPOSX           1
// Startposition Y
#define STARTPOSY           4
// Startrotation
#define STARTROT            0

//Start Laenge der Schlange
#define SCORE               3

//Der Start der Skalierung
#define SCALING             300


/* ---- Typedeklarationen ---- */
/** Punkt im 2D-Raum */
typedef GLfloat CGPoint2f[2];

/** Punkt im 2D-Raum */
typedef GLint CGPoint2i[2];

/** Punkt im 2D-Raum mit Richtung*/
typedef GLint CGPoint3i[3];

/** Ausmasse eines Rechtecks (Breite/Hoehe) */
typedef GLint CGDimensions2i[2];

/** RGB-Farbwert */
typedef GLfloat CGColor3f[3];

/** Bewegungsrichtungen. */
enum e_Direction
{ dirLeft, dirRight, dirUp, dirDown };

/** Datentyp fuer Bewegungsrichtungen. */
typedef enum e_Direction CGDirection;

/** Vektor im 2D-Raum */
typedef GLfloat CGVector2f[2];




#endif
