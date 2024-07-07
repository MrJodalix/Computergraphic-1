#ifndef __RENDERING_GL_H__
#define __RENDERING_GL_H__

#ifndef M_PI
#    define M_PI 3.14159265358979323846f
#endif

#ifdef __unix
	#define fopen_s(pF,fn,m) (*(pF))=fopen((fn), (m))
#endif

/**
 * @file
 * Schnittstelle des Rendering-Moduls.
 * @author copyright (C) Fachhochschule Wedel 1999-2011. All rights reserved.
 */

/* ---- System Header einbinden ---- */
#include <math.h>

/* Stellt moderne OpenGL-Funktionalität bereit. */
#include <GL/glew.h>

/* Anstelle der "glut.h" wird die "freeglut.h" eingebunden, da nur diese
 * das explizite Einstellen einer OpenGL-Version erlaubt. */
#include <GL/freeglut.h>


/**
 * Erzeugt die Identitätsmatrix.
 * @param m Pointer auf ein Array mit 16 Einträgen.
 */
void identity(float* m);

/**
 * Erzeugt eine perspektivische Projektions-Matrix.
 *
 * @param left Linke Grenze des View-Frustums an der Near-Plane.
 * @param right Rechte Grenze des View-Frustums an der Near-Plane.
 * @param top Obere Grenze des View-Frustums an der Near-Plane.
 * @param bottom Untere Grenze des View-Frustums an der Near-Plane.
 * @param znear Abstand zur Near-Plane.
 * @param zfar Abstand zur Far-plane.
 * @param[out] m Pointer auf die Matrix, die mit Werten gefüllt werden soll.
 *
 * @remarks Diese Funktion entspricht funktional glFrustum.
 */
void frustum(float left, float right, float top, float bottom, float znear, float zfar, float* m);

/**
 * Erzeugt eine perspektivische Projektions-Matrix.
 *
 * @param fov Öffnungswinkel des View-Frustums in deg.
 * @param aspect Verhältnis zwischen Breite und Höhe des View-Frustums.
 * @param znear Abstand zur Near-Plane.
 * @param zfar Abstand zur Far-Plane.
 * @param[out] m Pointer auf die Matrix, die mit Werten gefüllt werden soll.
 *
 * @remarks Diese Funktion entspricht funktional gluPerspective.
 */
void perspective(float fov, float aspect, float znear, float zfar, float* m);

/**
 * Berechnet ein normiertes Kreuzprodukt.
 *
 * @param lhs Linker Operant.
 * @param rhs Rechter Operant.
 * @param[out] res Pointer auf das Resultat.
 */
void unitCross(float lhsX, float lhsY, float lhsZ, float rhsX, float rhsY, float rhsZ, float* resX, float* resY, float* resZ);

/**
 * Berechnet eine View-Matrix.
 *
 * @param center Betrachterposition.
 * @param target Betrachtungspunkt.
 * @param up Oben.
 * @param[out] m Pointer auf eine Matrix, die mit Werten gefüllt werden soll.
 *
 * @remarks Diese Funktion entspricht funktional gluLookAt.
 */
void lookAt(float centerX, float centerY, float centerZ, float targetX, float targetY, float targetZ, float upX, float upY, float upZ, float* m);


/**
 * Liest den Inhalt einer Datei in einen String.
 *
 * @param filepath Pfad zur Datei.
 * @return Neuer String mit dem Inhalt der Datei. Muss freigegeben
 *         werden!
 */
char* readfile(const char* filepath);

/**
 * Überprüft, ob ein Shader richtig kompiliert wurde.
 *
 * @param filename Name des Shaders.
 * @param shader Id des Shader-Objekts.
 */
void checkShaderLog(const char* filename, GLuint shader);

/**
 * Erzeugt ein neues Shader-Objekt.
 *
 * @param shaderType Typ des zu erstellenden Shaders
 * @param filename Shader-Datei.
 * @return Id des neu erstellten Shader-Objekts.
 */
GLuint createShader(GLenum shaderType, const char* filename);

/**
 * Überprüft, ob ein Program richtig gelinkt werden konnte.
 *
 * @param program Id des Program-Objekts.
 */
void checkProgramLog(GLuint program);

/**
 * Erstellt ein neues Program.
 *
 * @param vertexShaderFilename Dateiname des Vertex-Shaders
 * @param fragmentShaderFilename Dateiname des Fragment-Shaders.
 * @return Id des neu erstellten Programm-Objekts.
 */
GLuint createProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename);

#endif
