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

/** Breite des Rechtecks */
#define QUAD_WIDTH          0.15f
/** Breite des Rechtecks */
#define QUAD_HEIGHT         0.075f

/** Parameter des Block Arrays */
// X-Koordinate
#define ARRAY_WIDTH         9
// Y-Koordinate
#define ARRAY_LENGTH        7
// 0-2 Farbwerte, 3 Kollision, 4-5 Block Koordninaten
#define ARRAY_DIMENSIONS    6


/* ---- Typedeklarationen ---- */
/** Punkt im 2D-Raum */
typedef GLfloat CGPoint2f[2];

/** Punkt im 2D-Raum */
typedef GLint CGPoint2i[2];

/** Ausmasse eines Rechtecks (Breite/Hoehe) */
typedef GLint CGDimensions2i[2];

/** RGB-Farbwert */
typedef GLfloat CGColor3f[3];

/** Kollisionsseiten. */
enum e_Side
{ sideNone, sideLeft, sideRight, sideTop, sideBottom };

/** Datentyp fuer Kollisionsseiten. */
typedef enum e_Side CGSide;

/** Bewegungsrichtungen. */
enum e_Direction
{ dirLeft, dirRight, dirUp, dirDown };

/** Datentyp fuer Bewegungsrichtungen. */
typedef enum e_Direction CGDirection;

/** Vektor im 2D-Raum */
typedef GLfloat CGVector2f[2];

#endif
