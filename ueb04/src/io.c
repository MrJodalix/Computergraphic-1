/**
 * @file
 * Ein-/Ausgabe-Modul.
 * Das Modul kapselt die Ein- und Ausgabe-Funktionalitaet (insbesondere die GLUT-
 * Callbacks) des Programms.
 *
 * @author Joshua-Scott Schoettke, Ilana Schmara (Gruppe 3).
 */

/* ---- System Header einbinden ---- */
#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/* ---- Eigene Header einbinden ---- */
#include "io.h"
#include "types.h"
#include "scene.h"
#include "logic.h"
#include "math.h"
#include "texture.h"
#include "debugGL.h"
#include "stringOutput.h"
#include <string.h>

/* ---- Konstanten ---- */

/** Anzahl der Aufrufe der Timer-Funktion pro Sekunde */
#define TIMER_CALLS_PS 60

/**
 * Berechnung der Frames pro Sekunde.
 * @return aktuelle Framerate.
 */
static float
frameRate(void)
{
  /* Zeitpunkt der letzten Neuberechnung */
  static int timebase = 0;
  /* Anzahl der Aufrufe seit letzter Neuberechnung */
  static int frameCount = 0;
  /* Zuletzt berechneter FPS-Wert */
  static float fps = 0.0;
  /* aktuelle "Zeit" */
  static int time = 0;

  /* Diesen Aufruf hinzuzaehlen */
  frameCount++;

  /* seit dem Start von GLUT vergangene Zeit ermitteln */
  time = glutGet(GLUT_ELAPSED_TIME);

  /* Eine Sekunde ist vergangen */
  if (time - timebase > 1000)
  {
    /* FPS-Wert neu berechnen */
    fps = frameCount * 1000.0f / (time - timebase);

    /* Zureuecksetzen */
    timebase = time;
    frameCount = 0;
  }

  /* Aktuellen FPS-Wert zurueckgeben */
  return fps;
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
 * Setzen der Projektionsmatrix.
 * Setzt die Projektionsmatrix unter Beruecksichtigung des Seitenverhaeltnisses
 * des Anzeigefensters, sodass das Seitenverhaeltnisse der Szene unveraendert
 * bleibt und gleichzeitig entweder in x- oder y-Richtung der Bereich von -1
 * bis +1 zu sehen ist.
 * @param aspect Seitenverhaeltnis des Anzeigefensters (In).
 */
static void
setProjection(GLdouble aspect)
{
  /* Perspektivische Darstellung */
  gluPerspective(70,     /* Öffnungswinkel */
                 aspect, /* Seitenverhältnis */
                 0.1,    /* nahe Clipping-Ebene */
                 100);   /* ferne Clipping-Ebene */
}

/**
 * Setzen der Kameraposition.
 */
static void
setCamera(void)
{
  /* Kameraposition */
  gluLookAt(0.0, 10.0, 15.0, /* vorne */
            0.0, 0.0, 0.0,   /* Zentrum */
            0.0, 1.0, 0.0);  /* Up-Vektor */
}

/**
 * Verarbeitung der Picking-Ergebnisse.
 * Findet den Treffer, der dem Betrachter am naechsten liegt und gibt die
 * Namen dieses Treffers aus.
 * @param numHits Anzahl der getroffenen Objekte (In).
 * @param buffer Buffer, in dem die Treffer gespeichert sind (In).
 */
static void
processHits(GLint numHits, GLuint buffer[], int isLeft)
{
  int *numberOfVertices = getNumberOfVertices();

  /* Anzahl der Namen des Treffers, der am naechsten zum Betrachter ist */
  GLuint *ptrClosestNames = NULL;

  /* Laufvariable - Zeiger auf den Beginn des Hit-Records */
  GLuint *ptr = (GLuint *)buffer;

  if (numHits > 0)
  {
    /* Zeiger auf den ersten Namen merken */
    ptrClosestNames = ptr + 3;

    if (isLeft)
    {
      Vertices[(*ptrClosestNames)][CY] += HEIGHTLEVEL;
      Normals[(*ptrClosestNames) + 1][NY] += HEIGHTLEVEL;
    }
    else
    {
      Vertices[(*ptrClosestNames)][CY] -= HEIGHTLEVEL;
      Normals[(*ptrClosestNames) + 1][NY] -= HEIGHTLEVEL;
    }

    for (int i = 1; i <= (*numberOfVertices); i++)
    {
      calcNormalCoords(i, Normals[i][NX], Normals[i][NY], Normals[i][NZ]);
    }
  }
}

/**
 * Picking. Auswahl von Szenenobjekten durch Klicken mit der Maus.
 */
static void
pick(int x, int y, int isLeft)
{
  /** Groesse des Buffers fuer Picking Ergebnisse */
#define SELECTBUFSIZE 512

  /* Viewport (Darstellungsbereich des Fensters) */
  GLint viewport[4];

  /* Puffer fuer Picking-Ergebnis */
  GLuint buffer[SELECTBUFSIZE];

  /* Anzahl der getroffenen Namen beim Picking */
  GLint numHits;

  /* aktuellen Viewport ermitteln */
  glGetIntegerv(GL_VIEWPORT, viewport);

  /* Puffer festlegen, Name-Stack leeren und Rendermode wechseln */
  glSelectBuffer(SELECTBUFSIZE, buffer);
  glInitNames();
  glRenderMode(GL_SELECT);

  /* Nachfolgende Operationen beeinflussen Projektionsmatrix */
  glMatrixMode(GL_PROJECTION);
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity();

  gluPickMatrix(x, glutGet(GLUT_WINDOW_HEIGHT) - y, 5, 5, viewport);

  setProjection((double)glutGet(GLUT_WINDOW_WIDTH) /
                (double)glutGet(GLUT_WINDOW_HEIGHT));

  /* Nachfolgende Operationen beeinflussen Modelviewmatrix */
  glMatrixMode(GL_MODELVIEW);
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity();

  setCamera();

  /* Zeichnen */
  drawWaterSpheres();

  /* Zeichnen beenden und auswerten */
  glFlush();

  /* Nachfolgende Operationen beeinflussen Projektionsmatrix */
  glMatrixMode(GL_PROJECTION);
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity();

  setProjection((double)glutGet(GLUT_WINDOW_WIDTH) /
                (double)glutGet(GLUT_WINDOW_HEIGHT));

  numHits = glRenderMode(GL_RENDER);

  /*   GLGETERROR; */

  processHits(numHits, buffer, isLeft);
}

/**
 * Debug-Ausgabe eines Tasturereignisses.
 * Ausgabe der Taste, die das Ereignis ausgeloest hat, des Status der Modifier
 * und der Position des Mauszeigers.
 * Die Ausgabe erfolgt nur, wenn das Makro DEBUG definiert ist.
 * @param key Taste, die das Ereignis ausgeloest hat. (ASCII-Wert oder WERT des
 *        GLUT_KEY_<SPECIAL>.
 * @param status Status der Taste, GL_TRUE=gedrueckt, GL_FALSE=losgelassen.
 * @param isSpecialKey ist die Taste eine Spezialtaste?
 * @param x x-Position des Mauszeigers zum Zeitpunkt der Ereignisausloesung.
 * @param y y-Position des Mauszeigers zum Zeitpunkt der Ereignisausloesung.
 */
static void
debugPrintKeyboardEvent(int key, int status, GLboolean isSpecialKey, int x,
                        int y)
{
  (void)isSpecialKey;
  (void)key;
  (void)status;
  (void)x;
  (void)y;
#ifdef DEBUG
  /* Bitmask mit Status der Modifier-Tasten */
  int modifierMask = glutGetModifiers();

  /* Spezialtaste */
  if (isSpecialKey)
  {
    printf("Spezialtaste");

    switch (key)
    {
    case GLUT_KEY_F1:
      printf(" F1");
      break;
    case GLUT_KEY_F2:
      printf(" F2");
      break;
    case GLUT_KEY_UP:
      printf(" UP");
      break;
    case GLUT_KEY_DOWN:
      printf(" DOWN");
      break;
    case GLUT_KEY_LEFT:
      printf(" LEFT");
      break;
    case GLUT_KEY_RIGHT:
      printf(" RIGHT");
      break;
    case GLUT_KEY_HOME:
      printf(" HOME");
      break;
    case GLUT_KEY_END:
      printf(" END");
      break;
    case GLUT_KEY_INSERT:
      printf(" INSERT");
      break;
    }
  }
  /* keine Spezialtaste */
  else
  {
    printf("Taste %c (ASCII-Code: %u)", key, key);
  }

  /* Status ausgeben */
  if (status == GLUT_DOWN)
  {
    printf(" gedrueckt.\n");
  }
  else
  {
    printf(" losgelassen.\n");
  }

  /* Status der Modifier ausgeben */
  printf("  Status der Shift-Taste: %d, Status der Strg-Taste: %d, Status der Alt-Taste: %d\n",
         modifierMask & GLUT_ACTIVE_SHIFT, (modifierMask & GLUT_ACTIVE_CTRL) >> 1,
         (modifierMask & GLUT_ACTIVE_ALT) >> 2);

  printf("  Position der Maus: (%d,%d)\n\n", x, y);

#endif
}

/**
 * Verarbeitung eines Tasturereignisses.
 * Pfeiltasten steuern die Position des angezeigten Rechtecks.
 * F1-Taste (de-)aktiviert Wireframemodus.
 * F2-Taste schaltet zwischen Fenster und Vollbilddarstellung um.
 * ESC-Taste und q, Q beenden das Programm.
 * Falls Debugging aktiviert ist, wird jedes Tastaturereignis auf stdout
 * ausgegeben.
 * @param key Taste, die das Ereignis ausgeloest hat. (ASCII-Wert oder WERT des
 *        GLUT_KEY_<SPECIAL>.
 * @param status Status der Taste, GL_TRUE=gedrueckt, GL_FALSE=losgelassen.
 * @param isSpecialKey ist die Taste eine Spezialtaste?
 * @param x x-Position des Mauszeigers zum Zeitpunkt der Ereignisausloesung.
 * @param y y-Position des Mauszeigers zum Zeitpunkt der Ereignisausloesung.
 */
static void
handleKeyboardEvent(int key, int status, GLboolean isSpecialKey, int x,
                    int y)
{
  /** Keycode der ESC-Taste */
#define ESC 27

  /* temporaere Variable fuer Zustaende */
  int state = 0;
  /* Debugausgabe */
  debugPrintKeyboardEvent(key, status, isSpecialKey, x, y);

  int *numberofVertices = getNumberOfVertices();

  /* Taste gedrueckt */
  if (status == GLUT_DOWN)
  {
    /* Spezialtaste gedrueckt */
    if (isSpecialKey)
    {
      switch (key)
      {
        /* (De-)Aktivieren des Wireframemode */
      case GLUT_KEY_F1:
        toggleWireframeMode();
        glutPostRedisplay();
        break;
      case GLUT_KEY_F2:
        toggleNormals();
        break;
      case GLUT_KEY_F3:
        state = !getTexturingStatus();
        setTexturingStatus(state);
        INFO(("Texturierung ist aktiv: %i\n", state));
        glutPostRedisplay();
        break;
      case GLUT_KEY_F4:
        toggleAutomaticTextureCoordinates();
        glutPostRedisplay();
        break;
        /* Umschalten zwischen Fenster- und Vollbilddarstellung */
      case GLUT_KEY_F5:
        toggleFullscreen();
        break;
      }
    }
    /* normale Taste gedrueckt */
    else
    {
      switch (key)
      {
      case '+':
        (*numberofVertices) = pow((sqrt(*numberofVertices) + 1), 2.0);
        break;
      case '-':
        if (sqrt(*numberofVertices) > 2)
        {
          (*numberofVertices) = pow((sqrt(*numberofVertices) - 1), 2.0);
        }
        break;
      case 's':
      case 'S':
        toggleDrawSphere();
        break;
        // Hilfeausgabe
      case 'h':
      case 'H':
        initHelp();
        break;
        // Pause
      case 'p':
      case 'P':
        initPause();
        break;
        /* Programm beenden */
      case 'q':
      case 'Q':
      case ESC:
      {
        freeAll();
        exit(0);
      }
      break;
      }
    }
  }
}

/**
 * Verarbeitung eines Mausereignisses.
 * Durch Bewegung der Maus bei gedrueckter Maustaste kann die aktuelle
 * Zeichenfarbe beeinflusst werden.
 * Falls Debugging aktiviert ist, wird jedes Mausereignis auf stdout
 * ausgegeben.
 * @param x x-Position des Mauszeigers zum Zeitpunkt der Ereignisausloesung.
 * @param y y-Position des Mauszeigers zum Zeitpunkt der Ereignisausloesung.
 * @param eventType Typ des Ereignisses.
 * @param button ausloesende Maustaste (nur bei Ereignissen vom Typ mouseButton).
 * @param buttonState Status der Maustaste (nur bei Ereignissen vom Typ mouseButton).
 */
static void
handleMouseEvent(int x, int y, CGMouseEventType eventType, int button,
                 int buttonState)
{
  switch (eventType)
  {
  case mouseButton:
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
      if (buttonState == GLUT_UP)
      {
        pick(x, y, 1);
      }
      break;
    case GLUT_RIGHT_BUTTON:
      if (buttonState == GLUT_UP)
      {
        pick(x, y, 0);
      }
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}

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
  handleKeyboardEvent(key, GLUT_DOWN, GL_FALSE, x, y);
}

/**
 * Callback fuer Tastenloslassen.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 * @param key betroffene Taste (In).
 * @param x x-Position der Maus zur Zeit des Loslassens (In).
 * @param y y-Position der Maus zur Zeit des Loslassens (In).
 */
static void
cbKeyboardUp(unsigned char key, int x, int y)
{
  handleKeyboardEvent(key, GLUT_UP, GL_FALSE, x, y);
}

/**
 * Callback fuer Druck auf Spezialtasten.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 * @param key betroffene Taste (In).
 * @param x x-Position der Maus zur Zeit des Tastendrucks (In).
 * @param y y-Position der Maus zur Zeit des Tastendrucks (In).
 */
static void
cbSpecial(int key, int x, int y)
{
  handleKeyboardEvent(key, GLUT_DOWN, GL_TRUE, x, y);
}

/**
 * Callback fuer Loslassen von Spezialtasten.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 * @param key betroffene Taste (In).
 * @param x x-Position der Maus zur Zeit des Loslassens (In).
 * @param y y-Position der Maus zur Zeit des Loslassens (In).
 */
static void
cbSpecialUp(int key, int x, int y)
{
  handleKeyboardEvent(key, GLUT_UP, GL_TRUE, x, y);
}

/**
 * Timer-Callback.
 * Initiiert Berechnung der aktuellen Position und anschliessendes Neuzeichnen,
 * setzt sich selbst erneut als Timer-Callback.
 * @param lastCallTime Zeitpunkt, zu dem die Funktion als Timer-Funktion
 *   registriert wurde (In).
 */
static void
cbTimer(int lastCallTime)
{
  // int *pause = getPause();
  /* Seit dem Programmstart vergangene Zeit in Millisekunden */
  int thisCallTime = glutGet(GLUT_ELAPSED_TIME);

  /* Seit dem letzten Funktionsaufruf vergangene Zeit in Sekunden oder
    0 wenn die Pause aktiviert ist*/
  // double interval = (double)(thisCallTime - lastCallTime) / 1000;

  /* Wieder als Timer-Funktion registrieren */
  glutTimerFunc(1000 / TIMER_CALLS_PS, cbTimer, thisCallTime);

  /* Neuzeichnen anstossen */
  glutPostRedisplay();
}

/**
 * Setzt einen Viewport für 3-dimensionale Darstellung
 * mit perspektivischer Projektion und legt eine Kamera fest.
 * Ruft das zeichnen der Szene in diesem Viewport auf.
 *
 * @param x, y Position des Viewports im Fenster - (0, 0) ist die untere linke Ecke
 * @param width, height Breite und Höhe des Viewports
 * @param lookAt enthält die für glLookAt benötigten Daten zur Kamera (Augpunkt, Zentrum, Up-Vektor)
 */
static void
set3DViewport(GLint x, GLint y, GLint width, GLint height, GLdouble lookAt[9])
{
  /* Seitenverhältnis bestimmen */
  double aspect = (double)width / height;

  /* Folge Operationen beeinflussen die Projektionsmatrix */
  glMatrixMode(GL_PROJECTION);

  /* Einheitsmatrix laden */
  glLoadIdentity();

  /* Viewport-Position und -Ausdehnung bestimmen */
  glViewport(x, y, width, height);

  /* Perspektivische Darstellung */
  gluPerspective(70,     /* Öffnungswinkel */
                 aspect, /* Seitenverhältnis */
                 0.1,    /* nahe Clipping-Ebene */
                 100);   /* ferne Clipping-Ebene */

  /* Folge Operationen beeinflussen die Modelviewmatrix */
  glMatrixMode(GL_MODELVIEW);

  /* Einheitsmatrix laden */
  glLoadIdentity();

  /* Kameraposition */
  gluLookAt(lookAt[0], lookAt[1], lookAt[2],  /* Augpunkt */
            lookAt[3], lookAt[4], lookAt[5],  /* Zentrum */
            lookAt[6], lookAt[7], lookAt[8]); /* Up-Vektor */

  /* Szene zeichnen lassen */
  drawScene();
}

/**
 * Mouse-Button-Callback.
 * @param button Taste, die den Callback ausgeloest hat.
 * @param state Status der Taste, die den Callback ausgeloest hat.
 * @param x X-Position des Mauszeigers beim Ausloesen des Callbacks.
 * @param y Y-Position des Mauszeigers beim Ausloesen des Callbacks.
 */
static void
cbMouseButton(int button, int state, int x, int y)
{
  handleMouseEvent(x, y, mouseButton, button, state);
}

/**
 * Callback fuer Aenderungen der Fenstergroesse.
 * Initiiert Anpassung der Projektionsmatrix an veränderte Fenstergroesse.
 * @param w Fensterbreite (In).
 * @param h Fensterhoehe (In).
 */
static void
cbReshape(int w, int h)
{
  /* Das ganze Fenster ist GL-Anzeigebereich */
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);

  /* Nachfolgende Operationen beeinflussen Projektionsmatrix */
  glMatrixMode(GL_PROJECTION);
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity();

  /* Anpassen der Projektionsmatrix an das Seitenverhältnis des Fensters */
  setProjection((GLdouble)w / (GLdouble)h);
}

/**
 * Zeichen-Callback.
 * Loescht die Buffer, setzt die Viewports,
 * ruft das Zeichnen der Szene auf (indirekt) und tauscht den Front-
 * und Backbuffer.
 */
static void
cbDisplay(void)
{
  /* Fensterdimensionen auslesen */
  int width = glutGet(GLUT_WINDOW_WIDTH);
  int height = glutGet(GLUT_WINDOW_HEIGHT);

  /* Frames pro Sekunde */
  static GLfloat fps = 0.0f;

  /* Kamera von schräg oben */
  GLdouble lookAt[9] = {0.0, 10.0, 15.0, /* vorne */
                        0.0, 0.0, 0.0,   /* Zentrum */
                        0.0, 1.0, 0.0};  /* Up-Vektor */

  /* Framewbuffer und z-Buffer zuruecksetzen */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* 3D-Viewport setzen */
  set3DViewport(0, 0,          /* x, y */
                width, height, /* breite, hoehe */
                lookAt);       /* Kamera */

  char Titel[] = "Water Simulation FPS: ";
  char FPS[MAX];

  gcvt(fps, 4, FPS);

  /* Framerate ausgegeben */
  glutSetWindowTitle(strncat(Titel, FPS, sizeof(Titel) + sizeof(fps)));

  /* Objekt anzeigen */
  glutSwapBuffers();

  /* Framerate berechnen */
  fps = frameRate();
}

/**
 * Idle-Callback.
 * Stoesst Neuzeichnen an.
 */
static void
cbIdle(void)
{
  glutPostRedisplay();
}

/**
 * Registrierung der GLUT-Callback-Routinen.
 */
static void
registerCallbacks(void)
{
  /* Idle-Callback (wird ausgefuehrt, wenn das System nichts zu tun hat
   * (es ist idle) - ACHTUNG: Aufruffrequenz variiert!! ) */
  glutIdleFunc(cbIdle);

  /* Mouse-Button-Callback (wird ausgefuehrt, wenn eine Maustaste
   * gedrueckt oder losgelassen wird) */
  glutMouseFunc(cbMouseButton);

  /* Tasten-Druck-Callback - wird ausgefuehrt, wenn eine Taste gedrueckt wird */
  glutKeyboardFunc(cbKeyboard);

  /* Tasten-Loslass-Callback - wird ausgefuehrt, wenn eine Taste losgelassen
   * wird */
  glutKeyboardUpFunc(cbKeyboardUp);

  /* Spezialtasten-Druck-Callback - wird ausgefuehrt, wenn Spezialtaste
   * (F1 - F12, Links, Rechts, Oben, Unten, Bild-Auf, Bild-Ab, Pos1, Ende oder
   * Einfuegen) gedrueckt wird */
  glutSpecialFunc(cbSpecial);

  /* Spezialtasten-Loslass-Callback - wird ausgefuehrt, wenn eine Spezialtaste
   * losgelassen wird */
  glutSpecialUpFunc(cbSpecialUp);

  /* Automat. Tastendruckwiederholung ignorieren */
  glutIgnoreKeyRepeat(1);

  /* Timer-Callback - wird einmalig nach msescs Millisekunden ausgefuehrt */
  glutTimerFunc(1000 / TIMER_CALLS_PS,       /* msecs - bis Aufruf von func */
                cbTimer,                     /* func  - wird aufgerufen    */
                glutGet(GLUT_ELAPSED_TIME)); /* value - Parameter, mit dem
                                                func aufgerufen wird */

  /* Reshape-Callback - wird ausgefuehrt, wenn neu gezeichnet wird (z.B. nach
   * Erzeugen oder Groessenaenderungen des Fensters) */
  glutReshapeFunc(cbReshape);

  /* Display-Callback - wird an mehreren Stellen imlizit (z.B. im Anschluss an
   * Reshape-Callback) oder explizit (durch glutPostRedisplay) angestossen */
  glutDisplayFunc(cbDisplay);
}

/**
 * Initialisiert das Programm (inkl. I/O und OpenGL) und startet die
 * Ereignisbehandlung.
 * @param title Beschriftung des Fensters
 * @param width Breite des Fensters
 * @param height Hoehe des Fensters
 * @return ID des erzeugten Fensters, 0 im Fehlerfall
 */
int initAndStartIO(char *title, int width, int height)
{
  int windowID = 0;

  /* Kommandozeile immitieren */
  int argc = 1;
  char *argv = "cmd";

  /* Glut initialisieren */
  glutInit(&argc, &argv);

  /* DEBUG-Ausgabe */
  INFO(("Erzeuge Fenster...\n"));

  /* Initialisieren des Fensters */
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(0, 0);

  /* Fenster erzeugen */
  windowID = glutCreateWindow(title);

  if (windowID)
  {

    /* DEBUG-Ausgabe */
    INFO(("...fertig.\n\n"));

    /* DEBUG-Ausgabe */
    INFO(("Initialisiere Szene...\n"));

    if (initScene())
    {
      /* DEBUG-Ausgabe */
      INFO(("...fertig.\n\n"));

      /* DEBUG-Ausgabe */
      INFO(("Lade und initialisiere Texturen...\n"));

      if (initTextures())
      {
        /* DEBUG-Ausgabe */
        INFO(("...fertig.\n\n"));

        /* DEBUG-Ausgabe */
        INFO(("Registriere Callbacks...\n"));

        registerCallbacks();

        /* DEBUG-Ausgabe */
        INFO(("...fertig.\n\n"));

        /* DEBUG-Ausgabe */
        INFO(("Trete in Schleife der Ereignisbehandlung ein...\n"));

        glutMainLoop();
      }
      else
      {
        /* DEBUG-Ausgabe */
        INFO(("...fehlgeschlagen.\n\n"));

        glutDestroyWindow(windowID);
        windowID = 0;
      }
    }
    else
    {
      /* DEBUG-Ausgabe */
      INFO(("...fehlgeschlagen.\n\n"));

      glutDestroyWindow(windowID);
      windowID = 0;
    }
  }
  else
  {
    /* DEBUG-Ausgabe */
    INFO(("...fehlgeschlagen.\n\n"));
  }

  return windowID;
}
