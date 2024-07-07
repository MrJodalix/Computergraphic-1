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
#define XSTART -10.0
/** x-Koordinate rechter Rand */
#define XEND 10.0
/** y-Koordinate oberer Rand */
#define ZSTART -10.0
/** y-Koordinate unterer Rand */
#define ZEND 10.0
/** Buffer Anzahl der FPS */
#define MAX 100

#define CX (0)
#define CY (1)
#define CZ (2)
#define CR (3)
#define CG (4)
#define CB (5)
#define CS (6)
#define CT (7)

#define NX (0)
#define NY (1)
#define NZ (2)

#define VERTICE_START 49

//Beispielrechnugn  25 nOV     (5 - 1) * ((5 - 1) * 2) * 3 = 4 * 8 * 3 = 72
#define COUNT_OF_INDICES ((sqrt(*numberOfVertices) - 1.0f) * ((sqrt(*numberOfVertices) - 1.0f) * 2) * 3)

/* ---- Konstanten 3D ---- */

//Farbkonstanten
#define COLOR     0.6f
#define NOCOLOR   0.1f

//Erhoehung/Verringerung
#define HEIGHTLEVEL 1.0

// Groesse des 3D Feldes
#define FIELDSIZE3D 20.0f

/* ---- Typedeklarationen ---- */
typedef GLdouble Vertex[8];
Vertex *Vertices;

typedef GLdouble Normal[3];
Normal *Normals;

// Indexarray anlegen
GLuint *Indices;

/** Punkt im 2D-Raum */
typedef GLfloat CGPoint2f[2];

/** Punkt im 2D-Raum */
typedef GLint CGPoint2i[2];

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

/** Bewegungsrichtungen. */
enum e_Direction
{
    dirLeft,
    dirRight,
    dirUp,
    dirDown
};

/** Datentyp fuer Bewegungsrichtungen. */
typedef enum e_Direction CGDirection;

/** Vektor im 2D-Raum */
typedef GLfloat CGVector2f[2];

/** Mausereignisse. */
enum e_MouseEventType
{ mouseButton, mouseMotion, mousePassiveMotion };

/** Datentyp fuer Mausereignisse. */
typedef enum e_MouseEventType CGMouseEventType;

#endif
