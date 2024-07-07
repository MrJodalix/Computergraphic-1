/**
 * @file
 * Einfaches Beispielprogramm fuer OpenGL, GLUT & GLEW
 * sowie einen Shader in GLSL.
 * Vier texturierte Quadrate werden gezeigt.
 * @author copyright (C) Fachhochschule Wedel 1999-2018. All rights reserved.
 */

/* ---- System Header einbinden ---- */
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

/* ---- Eigene Header einbinden ---- */
#include "utility.h"
#include "debugGL.h"

/* Bibliothek um Bilddateien zu laden. Es handelt sich um eine
 * Bibliothek, die sowohl den Header als auch die Quelle in einer Datei
 * bereitstellt. Die Quelle kann durch die Definition des Makros
 * STB_IMAGE_IMPLEMENTATION eingebunden werden. Genauere Informationen
 * sind der "stb_image.h" zu entnehmen.
 *
 * Quelle: https://github.com/nothings/stb */
// define global functions as static inline
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h> // Bibliothek um Bilder zu laden
                           // Quelle: https://github.com/nothings/stb

/* ---- Konstanten ---- */

/** Anzahl der Aufrufe der Timer-Funktion pro Sekunde */
#define TIMER_CALLS_PS 60
/** Anzahl der Vertices die fuer das Mesh erstellt werden sollen */
#define VERTICES 62500
// #define VERTICES 225
/** xz-Koordinate links-unten */
#define START -1.0
/** xz-Koordinate rechts-oben */
#define END 1.0
/** Die Differenz zwischen zwei Texturkoordinaten*/
#define TEXELCOORD 1.0f / (sqrt(numberOfVertices) - 1.0f)
/** Die Differenz zwischen zwei Vertexkoordinaten*/
#define VERTEXCOORD ((abs(START) + END) / (sqrt(numberOfVertices) - 1.0f))

/** Punkt im 3D-Raum (homogene Koordinaten) */
typedef GLfloat CGPoint4f[4];

/** Punkt im 2D-Raum */
typedef GLint CGPoint2i[2];

/** Ausmasse eines Rechtecks (Breite/Hoehe) */
typedef GLint CGDimensions2i[2];

/* ---- Globale Variablen ---- */

/** Buffer-Objekt, um die Vertizes zu speichern. */
static GLuint g_arrayBuffer;

/** Vertex-Array-Objekt, um die Attribut-Pointer zu speichern. */
static GLuint g_vertexArrayObject;

/** Programm-Objekt zum Rendern */
static GLuint g_program;

/** Heightmap-Textur */
static GLuint g_heightmap;

/** Rock-Textur */
static GLuint g_rock;

/** Snow-Textur */
static GLuint g_snow;

/** Schaltet die Heightmap ein/aus */
static GLuint g_toggleHeight;

/** Schaltet die Texture ein/aus */
static GLuint g_toggleTexture;

/** Schaltet die Shader um */
static GLuint g_toggleShader;

/** Schaltet die Sinuswelle ein/aus */
static GLuint g_toggleSinus;

/** Schaltet die Sinuswelle ein/aus */
static GLuint g_togglePause;

/** Gibt die Distanz zu den jeweiligen Vertices zurueck */
static GLfloat g_vertexDistance;

/** Gibt die Distanz zu den jeweiligen Texeln zurueck */
static GLfloat g_texelDistance;

/** Viewposition */
static CGPoint4f g_viewPos;

/** Lichtposition */
static CGPoint4f g_lightPos;

/** Ambienter Lichtanteil */
static CGPoint4f g_matAmbient = {0.5f, 0.5f, 0.5f, 1.0f};

/** Diffuser Lichtanteil */
static CGPoint4f g_matDiffuse = {0.7f, 0.7f, 0.7f, 1.0f};

/** Spekularer Lichtanteil */
static CGPoint4f g_matSpecular = {0.4f, 0.4f, 0.4f, 1.0f};

/** Schillernder Lichtanteil */
static GLfloat g_matShininess = 160.0f;

/** Zeit */
static GLfloat g_time;

/** Zeit */
static GLfloat g_pauseStart = 0.0f;
/** Zeit */
static GLfloat g_pauseEnd = 0.0f;

/** Location der uniform-Variable "ModelView" */
static GLint g_locationModelViewMatrix;

/** Location der uniform-Variable "Heightmap" */
static GLuint g_locationHeightmap;

/** Location der uniform-Variable "Rock" */
static GLint g_locationRock;

/** Location der uniform-Variable "Snow" */
static GLint g_locationSnow;

/** Location der uniform-Variable "ToggleHeight" */
static GLint g_locationToggleHeight;

/** Location der uniform-Variable "ToggleTexture" */
static GLint g_locationToggleTexture;

/** Location der uniform-Variable "ToggleShader" */
static GLint g_locationToggleShader;

/** Location der uniform-Variable "ToggleSinus" */
static GLint g_locationToggleSinus;

/** Location der uniform-Variable "TogglePause" */
static GLint g_locationTogglePause;

/** Location der uniform-Variable "VertexDistance" */
static GLint g_locationVertexDistance;

/** Location der uniform-Variable "TexelDistance" */
static GLint g_locationTexelDistance;

/** Location der uniform-Variable "ViewPos" */
static GLint g_locationViewPos;

/** Location der uniform-Variable "LightPos" */
static GLint g_locationLightPos;

/** Location der uniform-Variable "MatAmbient" */
static GLint g_locationMatAmbient;

/** Location der uniform-Variable "MatDiffuse" */
static GLint g_locationMatDiffuse;

/** Location der uniform-Variable "MatSpecular" */
static GLint g_locationMatSpecular;

/** Location der uniform-Variable "MatShininess" */
static GLint g_locationMatShininess;

/** Location der uniform-Variable "Time" */
static GLint g_locationTime;

/** Anzahl der Vertices */
int numberOfVertices = VERTICES;

/** Anzahl der Indices */
#define COUNT_OF_INDICES pow((sqrt(numberOfVertices) - 1), 2.0) * 2 * 3

/* ---- Funktionen ---- */

/**
 * Zeichen-Funktion.
 * Stellt die Szene dar.
 * Ausgabe eines Rechtecks.
 */
static void
drawScene(void)
{

  float viewMatrix[16];
  
  g_time = (float)glutGet(GLUT_ELAPSED_TIME) / 1000;

  const float distance = 2.0f;

  g_lightPos[0] = distance;
  g_lightPos[1] = 3.0f;
  g_lightPos[2] = distance;
  g_lightPos[3] = 0.0f;

  if (g_togglePause != 0)
  {
    g_viewPos[0] = distance * sinf(g_pauseStart);
    g_viewPos[1] = 1.0f;
    g_viewPos[2] = distance * cosf(g_pauseStart);
    g_viewPos[3] = 1.0;
    fprintf(stderr, "time       : %f\n", g_time);
    fprintf(stderr, "pauseStart: %f\n", g_pauseStart);
    fprintf(stderr, "pauseEnd: %f\n\n", g_pauseEnd);
  }
  else
  {
    g_viewPos[0] = distance * sinf(g_time - (g_pauseEnd - g_pauseStart));
    g_viewPos[1] = 1.0f;
    g_viewPos[2] = distance * cosf(g_time - (g_pauseEnd - g_pauseStart));
    g_viewPos[3] = 1.0;
    fprintf(stderr, "time       : %f\n", g_time);
    fprintf(stderr, "pauseStart: %f\n", g_pauseStart);
    fprintf(stderr, "pauseEnd: %f\n\n", g_pauseEnd);
  }

  lookAt(g_viewPos[0], g_viewPos[1], g_viewPos[2],
         0, 0, 0,
         0, 1, 0,
         viewMatrix);

  /* Aktivieren des Programms. Ab jetzt ist die Fixed-Function-Pipeline
   * inaktiv und die Shader des Programms aktiv. */
  glUseProgram(g_program);


  {
    /* Erstellen der Projektions-Matrix.
     * Da die Projektions-Matrix für die Laufzeit des Programms konstant
     * ist, wird sie einmalig gesetzt. */
    float projectionMatrix[16];
    perspective(60, (float)glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT), 0.01f, 5.0f, projectionMatrix);
    glUniformMatrix4fv(glGetUniformLocation(g_program, "Projection"), 1, GL_FALSE, projectionMatrix);
  }

  /* Übermitteln der View-Matrix an den Shader.
   * OpenGL arbeitet intern mit Matrizen in column-major-layout. D.h.
   * nicht die Reihen, sondern die Spalten liegen hintereinander im
   * Speicher. Die Funktionen zur Erzeugen von Matrizen in diesem
   * Programm berücksichtigen dies. Deswegen müssen die Matrizen nicht
   * transponiert werden und es kann GL_FALSE übergeben werden. Beim
   * Schreiben eigener Matrix-Funktionen oder beim Verwenden von
   * Mathematik-Bibliotheken muss dies jedoch berücksichtigt werden. */
  glUniformMatrix4fv(g_locationModelViewMatrix, 1, GL_FALSE, viewMatrix);

  g_vertexDistance = VERTEXCOORD;
  g_texelDistance = TEXELCOORD;

  /* Übermitteln der aktuellen Anhebung */
  glUniform1i(g_locationToggleHeight, g_toggleHeight);
  /* Übermitteln der aktuellen  */
  glUniform1i(g_locationToggleTexture, g_toggleTexture);
  /* Übermitteln der aktuellen  */
  glUniform1i(g_locationToggleShader, g_toggleShader);
  /* Übermitteln der aktuellen  */
  glUniform1i(g_locationToggleSinus, g_toggleSinus);
  /* Übermitteln der aktuellen  */
  glUniform1i(g_locationTogglePause, g_togglePause);
  /* Übermitteln der aktuellen  */
  glUniform1f(g_locationVertexDistance, g_vertexDistance);
  /* Übermitteln der aktuellen  */
  glUniform1f(g_locationTexelDistance, g_texelDistance);
  /* Übermitteln der aktuellen  */
  glUniform4f(g_locationViewPos, g_viewPos[0], g_viewPos[1], g_viewPos[2], g_viewPos[3]);
  /* Übermitteln der aktuellen  */
  glUniform4f(g_locationLightPos, g_lightPos[0], g_lightPos[1], g_lightPos[2], g_lightPos[3]);
  /* Übermitteln der aktuellen  */
  glUniform4f(g_locationMatAmbient, g_matAmbient[0], g_matAmbient[1], g_matAmbient[2], g_matAmbient[3]);
  /* Übermitteln der aktuellen  */
  glUniform4f(g_locationMatDiffuse, g_matDiffuse[0], g_matDiffuse[1], g_matDiffuse[2], g_matDiffuse[2]);
  /* Übermitteln der aktuellen  */
  glUniform4f(g_locationMatSpecular, g_matSpecular[0], g_matSpecular[1], g_matSpecular[2], g_matSpecular[3]);
  /* Übermitteln der aktuellen  */
  glUniform1f(g_locationMatShininess, g_matShininess);
  /* Übermitteln der aktuellen  */
  glUniform1f(g_locationTime, g_time);

  /* Übergeben der Textur an den Shader.
   * Texturen werden nicht direkt an den Shader übergeben, sondern
   * zuerst an eine Textureinheit gebunden. Anschließend wird dem
   * Programm nur der Index der Textureinheit übergeben. */
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, g_heightmap);
  glUniform1i(g_locationHeightmap, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, g_rock);
  glUniform1i(g_locationRock, 1);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, g_snow);
  glUniform1i(g_locationSnow, 2);

  /* Aktivieren des Vertex-Array-Objekts (VAO).
   * Hiermit werden alle Attribut-Pointer aktiv, die auf diesem VAO
   * gesetzt wurden. */
  glBindVertexArray(g_vertexArrayObject);

  /* Rendern der Dreiecke.
   * Ab dem ersten Dreieck im Buffer werden alle 8 Dreiecke gerendert.
   * Dem Draw-Command wird jedoch die Anzahl der Vertizes übergeben, die
   * gezeichnet werden sollen. */
  glDrawArrays(GL_TRIANGLES, 0, COUNT_OF_INDICES);

  /* Zurücksetzen des OpenGL-Zustands, um Seiteneffekte zu verhindern */
  glBindVertexArray(0);
  glUseProgram(0);
}

/**
 * Alle Daten eines Vertexes.
 */
typedef struct
{
  float x, y, z; /** Position */
  float s, t;    /** Textur-Koordinate */
} Vertex;

/**
 * Bereitet die Szene vor.
 */
static void
initScene(void)
{
  int count = 0;
  float x = START;
  float z = START;
  float s = 0;
  float t = 0;
  float vDistance = VERTEXCOORD;
  float tDistance = TEXELCOORD;
  int i = 0;
  int countOfIndices = COUNT_OF_INDICES;

  /** Erstellen des Vertex Arrays */
  Vertex vertices[countOfIndices];

  for (int j = 0; j < (numberOfVertices)-sqrt(numberOfVertices); j += sqrt((numberOfVertices)))
  {
    for (i = 0; i < (sqrt((numberOfVertices)) - 1); i++)
    {
      for (int k = 0; k < 6; k++)
      {
        vertices[count].x = x;
        vertices[count].y = 0.0f;
        vertices[count].z = z;
        vertices[count].s = s;
        vertices[count++].t = t;

        if (k == 0)
        {
          z += vDistance;
          t += tDistance;
        }
        else if (k == 1)
        {
          z -= vDistance;
          x += vDistance;

          t -= tDistance;
          s += tDistance;
        }
        else if (k == 3)
        {
          x -= vDistance;
          z += vDistance;
          s -= tDistance;
          t += tDistance;
        }
        else if (k == 4)
        {
          x += vDistance;
          s += tDistance;
        }
      }
      x -= vDistance;
      s -= tDistance;
    }
    z = START;
    x += vDistance;
    t = 0;
    s += tDistance;
  }

  {
    /* Erstellen eines Buffer-Objektes.
     * In modernem OpenGL werden alle Vertex-Daten in Buffer-Objekten
     * gespeichert. Dabei handelt es sich um Speicherbereiche die von
     * der OpenGL-Implementierung verwaltet werden und typischerweise
     * auf der Grafikkarte angelegt werden.
     *
     * Mit der Funktion glGenBuffers können Namen für Buffer-Objekte
     * erzeugt werden. Mit glBindBuffer werden diese anschließend
     * erzeugt, ohne jedoch einen Speicherbereich für die Nutzdaten
     * anzulegen. Dies geschieht nachfolgend mit einem Aufruf der
     * Funktion glBufferData.
     *
     * Um mit Buffer-Objekten zu arbeiten, mussen diese an Targets
     * gebunden werden. Hier wird das Target GL_ARRAY_BUFFER verwendet.
     *
     * Der OpenGL-Implementierung wird zusätzlich ein Hinweis mitgegeben,
     * wie die Daten eingesetzt werden. Hier wird GL_STATIC_DRAW
     * übergeben. OpenGL kann diesen Hinweis nutzen, um Optimierungen
     * vorzunehmen. */
    glGenBuffers(1, &g_arrayBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, g_arrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  {
    const GLuint positionLocation = 0;
    const GLuint texCoordLocation = 1;
    /* Erstellen eines Vertex-Array-Objektes (VAO).
     * Damit die OpenGL-Implementierung weiß welche Daten der Pipeline
     * bereitgestellt werden müssen, werden Attribut-Pointer gesetzt.
     * Um mehrere Datenquellen (unterschiedliche Meshes) zu verwalten
     * können die Attribut-Pointer in VAOs gruppiert werden.
     *
     * Die Erzeugung von VAOs geschieht prinzipiell genauso wie bei
     * Buffern oder anderen OpenGL-Objekten. */
    glGenVertexArrays(1, &g_vertexArrayObject);
    glBindVertexArray(g_vertexArrayObject);

    /* Die Pointer werden immer in den Buffer gesetzt, der am
     * GL_ARRAY_BUFFER-Target gebunden ist! */
    glBindBuffer(GL_ARRAY_BUFFER, g_arrayBuffer);

    /* Im Vertex-Shader existieren folgende Zeilen:
     * > layout (location = 0) in vec4 vPosition;
     * > layout (location = 1) in vec2 vTexCoord;
     *
     * Beim Aufruf einen Draw-Command, müssen diesen beiden Attributen
     * Daten bereitgestellt werden. Diese sollen aus dem oben erstellten
     * Buffer gelesen werden. Dafür müssen zwei Attribut-Pointer aktiviert
     * und eingerichtet werden. */
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(
        positionLocation,             /* location (siehe Shader) */
        3,                            /* Dimensionalität */
        GL_FLOAT,                     /* Datentyp im Buffer */
        GL_FALSE,                     /* Keine Normierung notwendig */
        sizeof(Vertex),               /* Offset zum nächsten Vertex */
        (void *)offsetof(Vertex, x)); /* Offset zum ersten Vertex */

    /* Zweiter Attribut-Pointer für die Textur-Koordinate */
    glEnableVertexAttribArray(texCoordLocation);
    glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, s));

    /* Um Programmierfehler leichter zu finden, sollte der OpenGL-
     * Zustand wieder zurückgesetzt werden. Wird beispielweise das Binden
     * eines Vertex-Array-Objekts vergessen werden, arbeitet OpenGL
     * auf dem vorher gebundenen. Vor allem bei starker Modularisierung
     * sind diese Fehler schwer zu finden. */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  {
    /* Erstellen des Programms */
    g_program = createProgram("../content/shaders/color.vert", "../content/shaders/color.frag");

    /* Abfragen der uniform-locations.
     * Dies kann auch zur Laufzeit gemacht werden, es ist jedoch
     * performanter dies vorab zu tun, da String-Vergleiche gemacht
     * werden müssen. */
    g_locationModelViewMatrix = glGetUniformLocation(g_program, "ModelView");
    g_locationHeightmap = glGetUniformLocation(g_program, "Heightmap");
    g_locationRock = glGetUniformLocation(g_program, "Rock");
    g_locationSnow = glGetUniformLocation(g_program, "Snow");
    g_locationToggleHeight = glGetUniformLocation(g_program, "ToggleHeight");
    g_locationToggleTexture = glGetUniformLocation(g_program, "ToggleTexture");
    g_locationToggleShader = glGetUniformLocation(g_program, "ToggleShader");
    g_locationToggleSinus = glGetUniformLocation(g_program, "ToggleSinus");
    g_locationTogglePause = glGetUniformLocation(g_program, "TogglePause");
    g_locationVertexDistance = glGetUniformLocation(g_program, "VertexDistance");
    g_locationTexelDistance = glGetUniformLocation(g_program, "TexelDistance");
    g_locationViewPos = glGetUniformLocation(g_program, "ViewPos");
    g_locationLightPos = glGetUniformLocation(g_program, "LightPos");
    g_locationMatAmbient = glGetUniformLocation(g_program, "Ambient");
    g_locationMatDiffuse = glGetUniformLocation(g_program, "Diffuse");
    g_locationMatSpecular = glGetUniformLocation(g_program, "Specular");
    g_locationMatShininess = glGetUniformLocation(g_program, "Shininess");
    g_locationTime = glGetUniformLocation(g_program, "Time");

    /* DEBUG-Ausgabe
    printf("ModelView hat 'location': %i\n", g_locationModelViewMatrix);
    printf("Heightmap hat 'location': %i\n", g_locationHeightmap);
    printf("Rock hat 'location': %i\n", g_locationRock);
    printf("Snow hat 'location': %i\n", g_locationSnow);
    printf("ToggleHeight hat 'location': %i\n", g_toggleHeight);
    printf("ToggleTexture hat 'location': %i\n", g_toggleTexture);
    printf("ToggleShader hat 'location': %i\n", g_toggleShader);
    printf("ToggleShader hat 'location': %i\n", g_toggleSinus);
    printf("VertexDistance hat 'location': %f\n", g_vertexDistance);
    printf("TexelDistance hat 'location': %f\n", g_texelDistance);
    printf("ViewPos hat 'location': %f\n", g_viewPos[0]);
    printf("LightPos hat 'location': %f\n", g_lightPos[0]);
    printf("Ambient hat 'location': %f\n", g_matAmbient[0]);
    printf("Diffuse hat 'location': %f\n", g_matDiffuse[0]);
    printf("Specular hat 'location': %f\n", g_matSpecular[0]);
    printf("Shininess hat 'location': %f\n", g_matShininess);
    printf("Time hat 'location': %f\n", g_time);*/
  }

  {
    /* Laden der Textur. */
    int width, height, comp;
    GLubyte *dataHeight = stbi_load("../content/textures/heightmap.png", &width, &height, &comp, 4);
    GLubyte *dataRock = stbi_load("../content/textures/rock.jpg", &width, &height, &comp, 4);
    GLubyte *dataSnow = stbi_load("../content/textures/snow.jpg", &width, &height, &comp, 4);

    /* Erstellen des Heightmap-Objekts. */
    glGenTextures(1, &g_heightmap);
    glBindTexture(GL_TEXTURE_2D, g_heightmap);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataHeight);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(dataHeight);

    /* Erstellen des Rock-Objekts. */
    glGenTextures(1, &g_rock);
    glBindTexture(GL_TEXTURE_2D, g_rock);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataRock);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 1);

    stbi_image_free(dataRock);

    /* Erstellen des Snow-Objekts. */
    glGenTextures(1, &g_snow);
    glBindTexture(GL_TEXTURE_2D, g_snow);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataSnow);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 2);

    stbi_image_free(dataSnow);
  }

  /* Z-Buffer-Test aktivieren */
  glEnable(GL_DEPTH_TEST);

  /* Polygonrueckseiten nicht anzeigen */
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
}

/**
 * Initialisierung des OpenGL-Systems.
 * Setzt Shading, Hintergrund- und Zeichenfarbe.
 * @return Rueckgabewert: im Fehlerfall 0, sonst 1.
 */
static int
initOpenGL(void)
{
  /* DEBUG-Ausgabe */
  INFO(("Initialisiere OpenGL...\n"));

  /* Beim Aufruf von glClear werden alle Pixel des Framebuffers auf
   * diesen Wert gesetzt. */
  glClearColor(0, 0, 0, 0);

  /* Beim Aufrus von glClear werden alle Pixel des Depthbuffer auf
   * diesen Wert gesetzt */
  glClearDepth(1);

  /* Aktivieren des Tiefentests */
  glEnable(GL_DEPTH_TEST);

  /* DEBUG-Ausgabe */
  INFO(("...fertig.\n\n"));

  /* Alles in Ordnung? */
  return 1;
}

/**
 * Timer-Callback.
 * Initiiert Berechnung der aktuellen Position und Farben und anschliessendes
 * Neuzeichnen, setzt sich selbst erneut als Timer-Callback.
 * @param lastCallTime Zeitpunkt, zu dem die Funktion als Timer-Funktion
 *   registriert wurde (In).
 */
static void
cbTimer(int lastCallTime)
{
  (void)lastCallTime;
  /* Seit dem Programmstart vergangene Zeit in Millisekunden */
  int thisCallTime = glutGet(GLUT_ELAPSED_TIME);

  /* Wieder als Timer-Funktion registrieren */
  glutTimerFunc(1000 / TIMER_CALLS_PS, cbTimer, thisCallTime);

  /* Neuzeichnen anstossen */
  glutPostRedisplay();
}

/**
 * Zeichen-Callback.
 * Loescht die Buffer, ruft das Zeichnen der Szene auf und erzwingt die
 * Darstellung durch OpenGL.
 */
static void
cbDisplay(void)
{
  /* Buffer zuruecksetzen */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Szene zeichnen */
  drawScene();

  /* Objekt anzeigen */
  glutSwapBuffers();
}

/**
 * Umschalten zw. Vollbild- und Fenstermodus.
 * Beim Wechsel zum Fenstermodus wird vorherige Position und Groesse
 * wiederhergestellt. HINWEIS: Fenster wird nicht korrekt auf alte Position
 * gesetzt, da GLUT_WINDOW_WIDTH/HEIGHT verfaelschte Werte liefert.
 */
static void
toggleFullscreen(void)
{
  /* Flag: Fullscreen: ja/nein */
  static GLboolean fullscreen = GL_FALSE;
  /* Zwischenspeicher: Fensterposition */
  static CGPoint2i windowPos;
  /* Zwischenspeicher: Fenstergroesse */
  static CGDimensions2i windowSize;

  /* Modus wechseln */
  fullscreen = !fullscreen;

  if (fullscreen)
  {
    /* Fenstereinstellungen speichern */
    windowPos[0] = glutGet(GLUT_WINDOW_X);
    windowPos[1] = glutGet(GLUT_WINDOW_Y);
    windowSize[0] = glutGet(GLUT_WINDOW_WIDTH);
    windowSize[1] = glutGet(GLUT_WINDOW_HEIGHT);
    /* In den Fullscreen-Modus wechseln */
    glutFullScreen();
  }
  else
  {
    /* alte Fenstereinstellungen wiederherstellen */
    glutReshapeWindow(windowSize[0], windowSize[1]);

    /* HINWEIS:
       Auskommentiert, da es sonst Probleme mit der Vollbildarstellung bei
       Verwendung von FreeGlut gibt */
    glutPositionWindow(windowPos[0], windowPos[1]);
  }
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
 * Debug-Callback.
 * @param type Art der Debug-Information. Einer der folgenden Werte:
 *     DEBUG_TYPE_ERROR_ARB
 *     DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB
 *     DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB
 *     DEBUG_TYPE_PORTABILITY_ARB
 *     DEBUG_TYPE_PERFORMANCE_ARB
 *     DEBUG_TYPE_OTHER_ARB
 * @param message Debug-Nachricht.
 */
#ifdef GLEW_CORRECT
static void
cbDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
  if (type == GL_DEBUG_TYPE_ERROR_ARB)
  {
    printf("ARB_debug_output: %s\n", message);
    exit(1);
  }
}
#endif

/**
 * Callback fuer Tastendruck.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 * @param key betroffene Taste (In).
 * @param x x-Position der Maus zur Zeit des Tastendrucks (In).
 * @param y y-Position der Maus zur Zeit des Tastendrucks (In).
 */
static void
cbKeyboard(unsigned char key, int x, int y)
{
  (void)x;
  (void)y;
/** Keycode der ESC-Taste */
#define ESC 27

  switch (key)
  {
  case 'w':
  case 'W':
    toggleWireframeMode();
    glutPostRedisplay();
    break;
    break;
  case 'f':
  case 'F':
    toggleFullscreen();
    break;
    break;
  /* Programm beenden */
  case 'q':
  case 'Q':
  case ESC:
    exit(0);
    break;
  /* Hilfeausgabe */
  case 'h':
  case 'H':
    printf("---\n");
    printf("Tastenbelegung\n\n");
    printf("f/F     zeigt alles im Fullscreen-Mode\n");
    printf("h/H     ruft diese Hilfe auf\n");
    printf("l/L     wechselt zwischen Phong und Gouraud ein/aus\n");
    printf("m/M     schaltet die Heightmap ein/aus\n");
    printf("p/P     schaltet die Pause ein/aus\n");
    printf("s/S     schaltet die Sinusmap  ein/aus\n");
    printf("t/T     schaltet die Textur    ein/aus\n");
    printf("w/W     schaltet den Wireframe-Mode ein/aus\n");
    printf("q/Q/ESC beenden das Programm.\n");
    printf("---\n");
    break;
  /* Umschalten des Shaders */
  case 'l':
  case 'L':
    g_toggleShader = (g_toggleShader + 1) % 2;
    break;
  /* Anzeigen Heightmap */
  case 'm':
  case 'M':
    g_toggleHeight = (g_toggleHeight + 1) % 2;
    break;
  /* Anzeigen Heightmap */
  case 'p':
  case 'P':
    g_togglePause = (g_togglePause + 1) % 2;
    if (g_togglePause != 0)
    {
      g_pauseStart = (float)glutGet(GLUT_ELAPSED_TIME) / 1000 - (g_pauseEnd - g_pauseStart);
    } else {

      g_pauseEnd = (float)glutGet(GLUT_ELAPSED_TIME) / 1000 - (g_pauseEnd - g_pauseStart);
    }
    break;
  /* Anzeigen Sinuswelle */
  case 's':
  case 'S':
    g_toggleSinus = (g_toggleSinus + 1) % 2;
    break;
  /* Anzeigen Texture */
  case 't':
  case 'T':
    g_toggleTexture = (g_toggleTexture + 1) % 2;
    break;
  }
}

/**
 * Registrierung der GLUT-Callback-Routinen.
 */
static void
registerCallbacks(void)
{
  /* DEBUG-Ausgabe */
  INFO(("Registriere Callbacks...\n"));

  /* Ob die extension ARB_debug_output bereit steht hängt vom Treiber
   * ab. Deswegen muss zur Laufzeit geprüft werden, ob das Callback
   * registriert werden kann. */
  if (GLEW_ARB_debug_output)
  {
    /* Verhindert, dass die OpenGL Implementierung das Callback
     * asynchron aufruft. Damit ist sichergestellt, dass der Callstack
     * im Falle eines Fehlers bereit steht.  */
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    /* In den Rechenzentren ist im Sommersemester 2016 eine veraltete
     * GLEW-Version installiert. Die dort erwartete Funktions-
     * signatur des Callback-Handlers stimmt nicht mit der hier
     * implementierten überein. Wenn ihr (Studenten) auf euren
     * Laptops arbeitet, ist es jedoch durchaus möglich, dass dieser
     * Code fehlerfrei kompiliert. Definiert einfach mal das Makro
     * und testet das. */
#ifdef GLEW_CORRECT
    /* Wenn dieses Callback gesetzt ist, wird die übergebene Funktion
     * immer dann aufgerufen, wenn Fehler auftreten oder weitere
     * nennenswerte Informationen ausgegeben werden müssen. Ihr
     * müsst dann nicht immer glGetError aufrufen und bekommt
     * zusätzlich auch noch einige Zusatzinformationen, die das
     * Debuggen vereinfachen. */
    glDebugMessageCallback(cbDebugOutput, NULL);
#endif

    /* Aktivieren der Extension */
    glEnable(GL_DEBUG_OUTPUT);
  }

  /* Display-Callback (wird ausgefuehrt, wenn neu gezeichnet wird
   * z.B. nach Erzeugen oder Groessenaenderungen des Fensters) */
  glutDisplayFunc(cbDisplay);

  /* Tasten-Druck-Callback - wird ausgefuehrt, wenn eine Taste gedrueckt wird */
  glutKeyboardFunc(cbKeyboard);

  /* Timer-Callback - wird einmalig nach msescs Millisekunden ausgefuehrt */
  glutTimerFunc(1000 / TIMER_CALLS_PS,       /* msecs - bis Aufruf von func */
                cbTimer,                     /* func  - wird aufgerufen    */
                glutGet(GLUT_ELAPSED_TIME)); /* value - Parameter, mit dem
                                                func aufgerufen wird */

  /* DEBUG-Ausgabe */
  INFO(("...fertig.\n\n"));
}

/**
 * Erzeugt ein Fenster.
 * @param title Beschriftung des Fensters
 * @param width Breite des Fensters
 * @param height Hoehe des Fensters
 * @return ID des erzeugten Fensters, 0 im Fehlerfall
 */
static int
createWindow(char *title, int width, int height)
{
  int windowID = 0;

  /* DEBUG-Ausgabe */
  INFO(("Erzeuge Fenster...\n"));

  /* Initialisieren des Fensters */
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(0, 0);

  /* Es wird explizit ein Context mit der Version 3.3 und dem Kern-
   * Profil angefordert. */
  glutInitContextVersion(3, 3);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  windowID = glutCreateWindow(title);

  /* DEBUG-Ausgabe */
  INFO(("...fertig..\n\n"));

  return windowID;
}

/**
 * Hauptprogramm.
 * Initialisiert Fenster, Anwendung und Callbacks, startet glutMainLoop.
 * @param argc Anzahl der Kommandozeilenparameter (In).
 * @param argv Kommandozeilenparameter (In).
 * @return Rueckgabewert im Fehlerfall ungleich Null.
 */
int main(int argc, char **argv)
{
  GLenum error;

  /* Glut initialisieren */
  glutInit(&argc, argv);

  /* Erzeugen des Fensters */
  if (!createWindow("CG1 Shader", 500, 500))
  {
    fprintf(stderr, "Initialisierung des Fensters fehlgeschlagen!");
    exit(1);
  }

  /* Initialisieren von GLEW.
   * Wenn glewExperimental auf GL_TRUE gesetzt wird, läd GLEW
   * zusätzliche Extensions. In diesem konkreten Programm wird dies
   * beispielsweise benötigt, um glGenVertexArrays() aufzurufen. (Ohne
   * glewExperimental auf GL_TRUE zu setzen tritt ein Speicherzugriffs-
   * fehler auf.) */
  glewExperimental = GL_TRUE;
  error = glewInit();
  if (error != GLEW_OK)
  {
    fprintf(stderr, "Initialisierung von GLEW fehlgeschlagen!");
    exit(1);
  }

  /* Initialen OpenGL-Zustand setzen */
  if (!initOpenGL())
  {
    fprintf(stderr, "Initialisierung von OpenGL fehlgeschlagen!");
    exit(1);
  }

  /* registriere Callbacks */
  registerCallbacks();

  /* Initialisiere Szene */
  initScene();

  /* Hauptschleife der Ereignisbehandlung starten */
  glutMainLoop();

  return 0;
}
