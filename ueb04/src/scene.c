/**
 * @file
 * Darstellungs-Modul.
 * Das Modul kapselt die Rendering-Funktionalitaet (insbesondere der OpenGL-
 * Aufrufe) des Programms.
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
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* ---- Eigene Header einbinden ---- */
#include "scene.h"
#include "logic.h"
#include "types.h"
#include "texture.h"
#include "debugGL.h"
#include "stringOutput.h"

/* ---- Globale Daten ---- */

// Display-List ID (Wasserkugeln)
static GLuint g_waterSphereID = 1;

int vertexCountChange = 0;

/**
 * Zeichen-Funktion zur Erstellung des Einheitskreis.
 */
void drawUnitCircle(void)
{
  GLUquadricObj *unitCircle = 0;
  unitCircle = gluNewQuadric();
  gluQuadricTexture(unitCircle, GL_TRUE);
  gluDisk(unitCircle, 0, 1, 32, 1);
  gluDeleteQuadric(unitCircle);
}

/**
 * Zeichen-Funktion zur Erstellung der Einheitskugel.
 */
void drawUnitSphere(void)
{
  GLUquadricObj *unitCircle = 0;
  unitCircle = gluNewQuadric();
  gluQuadricNormals(unitCircle, 1);
  gluSphere(unitCircle, 1, 32, 32);
  gluDeleteQuadric(unitCircle);
}

/**
 * Malt eine einzige Kugel
 */
void drawWaterSphere()
{
  glPushMatrix();
  {
    glScalef(0.1f, 0.1f, 0.1f);
    drawUnitSphere();
  }
  glPopMatrix();
}

/**
 * Malt alle Kugeln an den Positionen der Vertices
 */
void drawWaterSpheres(void)
{
  GLuint *waterSphereID = getDisplayList();
  int *numberOfVertices = getNumberOfVertices();
  float red = NOCOLOR;
  float green = NOCOLOR;
  float blue = NOCOLOR;

  for (GLuint count = 0; count < (*numberOfVertices); count++)
  {
    glPushMatrix();
    {
      glTranslatef(Vertices[count][CX], Vertices[count][CY], Vertices[count][CZ]);
      if (Vertices[count][CY] < 2.0)
      {
        red = NOCOLOR;
        green = NOCOLOR;
        blue = COLOR;
      }
      else if (Vertices[count][CY] < 4.0)
      {
        red = NOCOLOR;
        green = COLOR;
        blue = NOCOLOR;
      }
      else
      {
        red = COLOR;
        green = NOCOLOR;
        blue = NOCOLOR;
      }
      glPushName(count);
      {
        float matAmbient[] = {red, green, blue, 1.0};
        float matDiffuse[] = {red, green, blue, 1.0};
        float matSpecular[] = {0.1, 0.1, 0.1, 1.0};
        float matShininess = 1;
        // Setzen der Material-Parameter
        glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, &matShininess);

        glCallList(*waterSphereID);
      }
      glPopName();
    }
    glPopMatrix();
  }
}

/**
 * Zeichen-Funktion zeigt den Hilfe Screen.
 */
void drawHelp(void)
{
  /* Textfarbe */
  GLfloat textColor[3] = {1.0f, 0.0f, 0.0f};
  /* Text ausgegeben */
  drawString(0.05f, 0.09f, textColor, "Hilfeausgabe:");
  drawString(0.05f, 0.14f, textColor, "Keyboard-Steuerung:");
  drawString(0.05f, 0.19f, textColor, "q/Q/Esc : Beendet das Programm.");
  drawString(0.05f, 0.24f, textColor, "h/H     : Ruft diese Hilfeausgabe auf.");
  drawString(0.05f, 0.29f, textColor, "F1      : Toggle Wireframe-Modus");
  drawString(0.05f, 0.34f, textColor, "F2      : Toggle Normals");
  drawString(0.05f, 0.39f, textColor, "F3      : Toggle Texture");
  drawString(0.05f, 0.44f, textColor, "F4      : Toggle auto-Texture-Modus");
  drawString(0.05f, 0.49f, textColor, "F5      : Toggle Fullscrene-Modus");
  drawString(0.05f, 0.54f, textColor, "s/S     : Toggle Anzeige Kugeln.");
  drawString(0.05f, 0.59f, textColor, "+       : Grid-Auflösung erhöhen.");
  drawString(0.05f, 0.64f, textColor, "-       : Grid-Auflösung verringern");
  drawString(0.05f, 0.69f, textColor, "-----------------------------------");
  drawString(0.05f, 0.74f, textColor, "Maus-Steuerung:");
  drawString(0.05f, 0.79f, textColor, "LMB     : Click auf kugeln Y-koordinate erhöhen");
  drawString(0.05f, 0.84f, textColor, "RMB     : Click auf kugeln Y-koordinate verringern");
}

/**
 * Malt die ganze Szene
 */
void drawScene(void)
{
  int *numberOfVertices = getNumberOfVertices();

  /* Lichtberechnung aktivieren */
  glEnable(GL_LIGHTING);

  /* erste (feste) Lichtquelle */
  CGPoint4f lightPos = {3.0f, 2.0f, 1.0f, 0.0f};

  /* Position der ersten Lichtquelle setzen */
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  /* Erste Lichtquelle aktivieren */
  glEnable(GL_LIGHT0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if (vertexCountChange != (*numberOfVertices))
  {
    if (vertexCountChange != 0)
    {
      freeAll();
    }
    setWaterPlane();
    // Vertex und Color Array definieren und bei Änderungen aktualisieren
    glVertexPointer(3,                   // Komponenten pro Vertex (x,y,z)
                    GL_DOUBLE,           // Typ der Komponenten
                    sizeof(Vertex),      // Offset zwischen 2 Vertizes im Array
                    &(Vertices[0][CX])); // Zeiger auf die 1. Komponente

    glColorPointer(3, GL_DOUBLE, sizeof(Vertex), &(Vertices[0][CR]));

    glTexCoordPointer(2, GL_DOUBLE, sizeof(Vertex), &(Vertices[0][CS]));

    glNormalPointer(GL_DOUBLE, sizeof(Normal), &(Normals[1][NX]));

    vertexCountChange = (*numberOfVertices);
  }

  if (getTexturingStatus())
  {
    /* Texturierung aktivieren */
    glEnable(GL_TEXTURE_2D);
    bindTexture(texWater);
  }

  glEnable(GL_COLOR_MATERIAL);
  // Zeichnen (in der drawScene für jeden Frame)
  glDrawElements(GL_TRIANGLES,     // Primitivtyp
                 COUNT_OF_INDICES, // Anzahl Indizes zum Zeichnen
                 GL_UNSIGNED_INT,  // Typ der Indizes
                 Indices);         // Index Array

  glDisable(GL_COLOR_MATERIAL);

  if (getTexturingStatus())
  {
    //Texturierung aktivieren 
    glEnable(GL_TEXTURE_2D);
    bindTexture(texIsle);
  }

  glEnable(GL_COLOR_MATERIAL);
  glPushMatrix();
  {
    glRotatef(-90.0, 1, 0, 0);
    glTranslatef(0.0f, -2.0f, 2.0f);
    //glRotatef(90.0, 1.0f, 0.0f, 0.0f);
    glScalef(2.0f, 2.0f, 2.0f);
    drawUnitCircle();
  }
  glPopMatrix();
  glDisable(GL_COLOR_MATERIAL);

  if ((*getDrawWaterSpheres()))
  {
    glPushMatrix();
    {
      drawWaterSpheres();
    }
    glPopMatrix();
  }

  if ((*getDrawHelp()))
  {
    drawHelp();
  }

  if ((*getDrawNorm()))
  {
    for (int i = 0; i < (*numberOfVertices); i++)
    {
      glPushMatrix();
      {
        glBegin(GL_LINES);
        {
          glVertex3d(Vertices[i][CX], Vertices[i][CY] + Normals[i + 1][NY] - HEIGHTLEVEL, Vertices[i][CZ]);
          glVertex3d(Vertices[i][CX] + Normals[i + 1][NX], Vertices[i][CY] + Normals[i + 1][NY], Vertices[i][CZ] + Normals[i + 1][NZ]);
        }
        glEnd();
      }
      glPopMatrix();
    }
  }

  /* Lichtberechnung deaktivieren */
  glDisable(GL_LIGHTING);
  /* Texturierung deaktivieren */
  glDisable(GL_TEXTURE_2D);
}

/**
 * Initialisierung der Lichtquellen.
 * Setzt Eigenschaften der Lichtquellen (Farbe, Oeffnungswinkel, ...)
 */
void initLight(void)
{
  /* Farbe der ersten Lichtquelle */
  CGPoint4f lightColor0[3] =
      {{0.3f, 0.3f, 0.3f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.8f, 0.8f, 0.8f, 1.0f}};

  /* Farbe der ersten Lichtquelle setzen */
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightColor0[0]);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0[1]);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor0[2]);
}

/**
 * Erzeugen der Display-Listen (Wasser-Kugeln)
 */
static void
initDisplayLists(void)
{
  /* Display-Liste anfordern */
  g_waterSphereID = glGenLists((GLsizei)1);
  if (g_waterSphereID != 0)
  {
    glNewList(g_waterSphereID, GL_COMPILE);
    drawWaterSphere();
    glEndList();
  }
  else
  {
    CG_ERROR(("Konnte Displaylisten nicht erzeuge\n"));
  }
}

/**
 * Initialisierung der Szene (inbesondere der OpenGL-Statusmaschine).
 * Setzt Hintergrundfarbe.
 * Initialisiert die Displaylisten, die Schlangenliste.
 * loescht Explosionen im Level, falls noch welche vorhanden sind.
 * scale und toggle werden auf ihren Startwert zurueck gesetzt.
 * Die Position fuer das Food wird initialisiert.
 * @return Rueckgabewert: im Fehlerfall 0, sonst 1.
 */
int initScene(void)
{
  /* Setzen der Farbattribute */
  /* Hintergrundfarbe */
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  /* Z-Buffer-Test aktivieren */
  glEnable(GL_DEPTH_TEST);

  /* Polygonrueckseiten nicht anzeigen */
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  /* Normalen nach Transformationen wieder auf die
   * Einheitslaenge bringen */
  glEnable(GL_NORMALIZE);

  initLight();

  initDisplayLists();

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  initVertices();

  /* Alles in Ordnung? */
  return (GLGETERROR == GL_NO_ERROR);
}

/**
 * löscht alle Displaylisten
 */
void deleteDisplayLists(void)
{
  GLuint *waterSphereID = getDisplayList();
  glDeleteLists(*waterSphereID, 1);
}

/**
 * (De-)aktiviert den Wireframe-Modus.
 */
void toggleWireframeMode(void)
{
  /* Flag: Wireframe: ja/nein */
  static GLboolean wireframe = GL_FALSE;

  /* Modus wechseln */
  wireframe = !wireframe;

  if (wireframe)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  else
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
}

/**
 * (De-)aktiviert den Normalen-Modus.
 */
void toggleNormals(void)
{
  /* Flag: Normals: ja/nein */
  static GLboolean normals = GL_FALSE;
  int *drawNorm = getDrawNorm();

  /* Modus wechseln */
  normals = !normals;

  if (normals)
  {
    (*drawNorm) = 1;
  }
  else
  {
    (*drawNorm) = 0;
  }
}

/**
 * (De-)aktiviert den Kugel-Modus.
 */
void toggleDrawSphere(void)
{
  // Flag: Normals: ja/nein
  static GLboolean spheres = GL_FALSE;
  int *drawSpheres = getDrawWaterSpheres();

  // Modus wechseln
  spheres = !spheres;

  if (spheres)
  {
    (*drawSpheres) = 1;
  }
  else
  {
    (*drawSpheres) = 0;
  }
}

/**
 * Gibt die DisplayListe zurueck
 * @return die Displayliste
 */
GLuint *
getDisplayList(void)
{
  return &g_waterSphereID;
}