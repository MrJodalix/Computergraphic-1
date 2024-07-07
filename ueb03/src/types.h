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

/* ---- Konstanten 2D ---- */

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

// Rand des Feldes
#define FIELDBORDER         -0.81f

// Startposition X
#define STARTPOSX           1
// Startposition Y
#define STARTPOSY           4
// Startrotation
#define STARTROT            0

// Start Laenge der Schlange
#define SCORE               3

// Der Start der Skalierung
#define SCALING             300


/* ---- Konstanten 3D ---- */

// Groesse des 3D Feldes
#define FIELDSIZE3D         20.0f


/* ---- Typedeklarationen ---- */

/** Punkt im 2D-Raum */
typedef GLfloat CGPoint2f[2];

/** Punkt im 2D-Raum */
typedef GLint CGPoint2i[2];

/** Punkt im 2D-Raum mit Richtung*/
typedef GLint CGPoint3i[3];

/** Punkt im 3D-Raum */
typedef GLfloat CGPoint3f[3];

/** Vektor im 3D-Raum */
typedef GLfloat CGVector3f[3];

/** Punkt im 3D-Raum (homogene Koordinaten) */
typedef GLfloat CGPoint4f[4];

/** Ausmasse eines Rechtecks (Breite/Hoehe) */
typedef GLint CGDimensions2i[2];

/** RGB-Farbwert */
typedef GLfloat CGColor3f[3];

/** RGBA-Farbwert */
typedef GLfloat CGColor4f[4];

/** Bewegungsrichtungen. */
enum e_Direction
{ dirLeft, dirRight, dirUp, dirDown };

/** Datentyp fuer Bewegungsrichtungen. */
typedef enum e_Direction CGDirection;

/** Vektor im 2D-Raum */
typedef GLfloat CGVector2f[2];


#endif
