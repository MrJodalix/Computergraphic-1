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
#include "logic.h"
#include "scene.h"
#include "debugGL.h"

/* ---- Konstanten ---- */
/** Anzahl der Aufrufe der Timer-Funktion pro Sekunde */
#define TIMER_CALLS_PS 60

/**
 * Umschalten zw. Vollbild- und Fenstermodus.
 * Beim Wechsel zum Fenstermodus wird vorherige Position und Groesse
 * wiederhergestellt. HINWEIS: Fenster wird nicht korrekt auf alte Position
 * gesetzt, da GLUT_WINDOW_WIDTH/HEIGHT verfaelschte Werte liefert.
 */
static void
toggleFullscreen (void)
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
      windowPos[0] = glutGet (GLUT_WINDOW_X);
      windowPos[1] = glutGet (GLUT_WINDOW_Y);
      windowSize[0] = glutGet (GLUT_WINDOW_WIDTH);
      windowSize[1] = glutGet (GLUT_WINDOW_HEIGHT);
      /* In den Fullscreen-Modus wechseln */
      glutFullScreen ();
    }
  else
    {
      /* alte Fenstereinstellungen wiederherstellen */
      glutReshapeWindow (windowSize[0], windowSize[1]);

      /* HINWEIS:
         Auskommentiert, da es sonst Probleme mit der Vollbildarstellung bei
         Verwendung von FreeGlut gibt */
      glutPositionWindow (windowPos[0], windowPos[1]);
    }
}

/**
 * Setzen der Projektionsmatrix.
 * Setzt die Projektionsmatrix unter Berücksichtigung des Seitenverhältnisses
 * des Anzeigefensters, sodass das Seitenverhältnisse der Szene unverändert
 * bleibt und gleichzeitig entweder in x- oder y-Richtung der Bereich von -1
 * bis +1 zu sehen ist.
 * @param aspect Seitenverhaeltnis des Anzeigefensters (In).
 */
static void
setProjection (GLdouble aspect)
{
  /* Nachfolgende Operationen beeinflussen Projektionsmatrix */
  glMatrixMode (GL_PROJECTION);
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity ();

  /* Koordinatensystem bleibt quadratisch */
  if (aspect <= 1)
    {
      gluOrtho2D (-1.0, 1.0,    /* links, rechts */
                  -1.0 / aspect, 1.0 / aspect); /* unten, oben */
    }
  else
    {
      gluOrtho2D (-1.0 * aspect, 1.0 * aspect,  /* links, rechts */
                  -1.0, 1.0);   /* unten, oben */
    }
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
debugPrintKeyboardEvent (int key, int status, GLboolean isSpecialKey, int x,
                         int y)
{
	(void)isSpecialKey;
	(void)key;
	(void)status;
	(void)x;
	(void)y;
#ifdef DEBUG
  /* Bitmask mit Status der Modifier-Tasten */
  int modifierMask = glutGetModifiers ();

  /* Spezialtaste */
  if (isSpecialKey)
    {
      printf ("Spezialtaste");

      switch (key)
        {
        case GLUT_KEY_F1:
          printf (" F1");
          break;
        case GLUT_KEY_F2:
          printf (" F2");
          break;
        case GLUT_KEY_LEFT:
          printf (" LEFT");
          break;
        case GLUT_KEY_RIGHT:
          printf (" RIGHT");
          break;
        case GLUT_KEY_HOME:
          printf (" HOME");
          break;
        case GLUT_KEY_END:
          printf (" END");
          break;
        case GLUT_KEY_INSERT:
          printf (" INSERT");
          break;
        }
    }
  /* keine Spezialtaste */
  else
    {
      printf ("Taste %c (ASCII-Code: %u)", key, key);
    }

  /* Status ausgeben */
  if (status == GLUT_DOWN)
    {
      printf (" gedrueckt.\n");
    }
  else
    {
      printf (" losgelassen.\n");
    }

  /* Status der Modifier ausgeben */
  printf
    ("  Status der Shift-Taste: %d, Status der Strg-Taste: %d, Status der Alt-Taste: %d\n",
     modifierMask & GLUT_ACTIVE_SHIFT, (modifierMask & GLUT_ACTIVE_CTRL) >> 1,
     (modifierMask & GLUT_ACTIVE_ALT) >> 2);

  printf ("  Position der Maus: (%d,%d)\n\n", x, y);

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
handleKeyboardEvent (int key, int status, GLboolean isSpecialKey, int x,
                     int y)
{
  /** Keycode der ESC-Taste */
#define ESC 27

  /* Debugausgabe */
  debugPrintKeyboardEvent (key, status, isSpecialKey, x, y);

  /* Taste gedrueckt */
  if (status == GLUT_DOWN)
    {
      /* Spezialtaste gedrueckt */
      if (isSpecialKey)
        {
          switch (key)
            {
              /* Bewegung des Rechtecks in entsprechende Richtung starten */
            case GLUT_KEY_LEFT:
              setMovement (dirLeft, GL_TRUE);
              break;
            case GLUT_KEY_RIGHT:
              setMovement (dirRight, GL_TRUE);
              break;
              /* (De-)Aktivieren des Wireframemode */
            case GLUT_KEY_F1:
              toggleWireframeMode ();
              glutPostRedisplay ();
              break;
              /* Umschalten zwischen Fenster- und Vollbilddarstellung */
            case GLUT_KEY_F2:
              toggleFullscreen ();
              break;
            }
        }
      /* normale Taste gedrueckt */
      else
        {
          switch (key)
            {
              /* Programm beenden */
            case 'q':
            case 'Q':
            case ESC:
              exit (0);
              break;
            }
        }
    }
  /* Taste losgelassen */
  else
    {
      /* Spezialtaste losgelassen */
      if (isSpecialKey)
        {
          switch (key)
            {
              /* Bewegung des Rechtecks in entsprechende Richtung beenden */
            case GLUT_KEY_LEFT:
              setMovement (dirLeft, GL_FALSE);
              break;
            case GLUT_KEY_RIGHT:
              setMovement (dirRight, GL_FALSE);
              break;
            }
        }
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
cbKeyboard (unsigned char key, int x, int y)
{
  handleKeyboardEvent (key, GLUT_DOWN, GL_FALSE, x, y);
}

/**
 * Callback fuer Tastenloslassen.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 * @param key betroffene Taste (In).
 * @param x x-Position der Maus zur Zeit des Loslassens (In).
 * @param y y-Position der Maus zur Zeit des Loslassens (In).
 */
static void
cbKeyboardUp (unsigned char key, int x, int y)
{
  handleKeyboardEvent (key, GLUT_UP, GL_FALSE, x, y);
}

/**
 * Callback fuer Druck auf Spezialtasten.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 * @param key betroffene Taste (In).
 * @param x x-Position der Maus zur Zeit des Tastendrucks (In).
 * @param y y-Position der Maus zur Zeit des Tastendrucks (In).
 */
static void
cbSpecial (int key, int x, int y)
{
  handleKeyboardEvent (key, GLUT_DOWN, GL_TRUE, x, y);
}

/**
 * Callback fuer Loslassen von Spezialtasten.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 * @param key betroffene Taste (In).
 * @param x x-Position der Maus zur Zeit des Loslassens (In).
 * @param y y-Position der Maus zur Zeit des Loslassens (In).
 */
static void
cbSpecialUp (int key, int x, int y)
{
  handleKeyboardEvent (key, GLUT_UP, GL_TRUE, x, y);
}

/**
 * Timer-Callback.
 * Initiiert Berechnung der aktuellen Position und anschliessendes Neuzeichnen,
 * setzt sich selbst erneut als Timer-Callback.
 * @param lastCallTime Zeitpunkt, zu dem die Funktion als Timer-Funktion
 *   registriert wurde (In).
 */
static void
cbTimer (int lastCallTime)
{
  /* Seit dem Programmstart vergangene Zeit in Millisekunden */
  int thisCallTime = glutGet (GLUT_ELAPSED_TIME);

  /* Seit dem letzten Funktionsaufruf vergangene Zeit in Sekunden */
  double interval = (double) (thisCallTime - lastCallTime) / 1000.0f;

  /* neue Position berechnen (zeitgesteuert) */
  calcPositionRacket (interval);

  /* neue Position berechnen (zeitgesteuert) */
  calcPositionBall (interval);

  /* Wieder als Timer-Funktion registrieren */
  glutTimerFunc (1000 / TIMER_CALLS_PS, cbTimer, thisCallTime);

  /* Neuzeichnen anstossen */
  glutPostRedisplay ();
}

/**
 * Callback fuer Aenderungen der Fenstergroesse.
 * Initiiert Anpassung der Projektionsmatrix an veränderte Fenstergroesse.
 * @param w Fensterbreite (In).
 * @param h Fensterhoehe (In).
 */
static void
cbReshape (int w, int h)
{
  /* Das ganze Fenster ist GL-Anzeigebereich */
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);

  /* Anpassen der Projektionsmatrix an das Seitenverhältnis des Fensters */
  setProjection ((GLdouble) w / (GLdouble) h);
}

/**
 * Zeichen-Callback.
 * Loescht die Buffer, ruft das Zeichnen der Szene auf und tauscht den Front-
 * und Backbuffer.
 */
static void
cbDisplay (void)
{
  /* Buffer zuruecksetzen */
  glClear (GL_COLOR_BUFFER_BIT);

  /* Nachfolgende Operationen beeinflussen Modelviewmatrix */
  glMatrixMode (GL_MODELVIEW);
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity ();

  /* Szene zeichnen */
  drawScene ();

  /* Objekt anzeigen */
  glutSwapBuffers ();
}

/**
 * Registrierung der GLUT-Callback-Routinen.
 */
static void
registerCallbacks (void)
{
  /* Tasten-Druck-Callback - wird ausgefuehrt, wenn eine Taste gedrueckt wird */
  glutKeyboardFunc (cbKeyboard);

  /* Tasten-Loslass-Callback - wird ausgefuehrt, wenn eine Taste losgelassen
   * wird */
  glutKeyboardUpFunc (cbKeyboardUp);

  /* Spezialtasten-Druck-Callback - wird ausgefuehrt, wenn Spezialtaste
   * (F1 - F12, Links, Rechts, Oben, Unten, Bild-Auf, Bild-Ab, Pos1, Ende oder
   * Einfuegen) gedrueckt wird */
  glutSpecialFunc (cbSpecial);

  /* Spezialtasten-Loslass-Callback - wird ausgefuehrt, wenn eine Spezialtaste
   * losgelassen wird */
  glutSpecialUpFunc (cbSpecialUp);

  /* Automat. Tastendruckwiederholung ignorieren */
  glutIgnoreKeyRepeat (1);

  /* Timer-Callback - wird einmalig nach msescs Millisekunden ausgefuehrt */
  glutTimerFunc (1000 / TIMER_CALLS_PS, /* msecs - bis Aufruf von func */
                 cbTimer,       /* func  - wird aufgerufen    */
                 glutGet (GLUT_ELAPSED_TIME));  /* value - Parameter, mit dem
                                                   func aufgerufen wird */

  /* Reshape-Callback - wird ausgefuehrt, wenn neu gezeichnet wird (z.B. nach
   * Erzeugen oder Groessenaenderungen des Fensters) */
  glutReshapeFunc (cbReshape);

  /* Display-Callback - wird an mehreren Stellen imlizit (z.B. im Anschluss an
   * Reshape-Callback) oder explizit (durch glutPostRedisplay) angestossen */
  glutDisplayFunc (cbDisplay);
}

/**
 * Initialisiert das Programm (inkl. I/O und OpenGL) und startet die
 * Ereignisbehandlung.
 * @param title Beschriftung des Fensters
 * @param width Breite des Fensters
 * @param height Hoehe des Fensters
 * @return ID des erzeugten Fensters, 0 im Fehlerfall
 */
int
initAndStartIO (char *title, int width, int height)
{
  int windowID = 0;

  /* Kommandozeile immitieren */
  int argc = 1;
  char *argv = "cmd";

  /* Glut initialisieren */
  glutInit (&argc, &argv);

  /* DEBUG-Ausgabe */
  INFO (("Erzeuge Fenster...\n"));

  /* Initialisieren des Fensters */
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (width, height);
  glutInitWindowPosition (0, 0);

  /* Fenster erzeugen */
  windowID = glutCreateWindow (title);

  if (windowID)
    {

      /* DEBUG-Ausgabe */
      INFO (("...fertig.\n\n"));

      /* DEBUG-Ausgabe */
      INFO (("Initialisiere Szene...\n"));

      if (initScene ())
        {
          /* DEBUG-Ausgabe */
          INFO (("...fertig.\n\n"));

          /* DEBUG-Ausgabe */
          INFO (("Registriere Callbacks...\n"));

          registerCallbacks ();

          /* DEBUG-Ausgabe */
          INFO (("...fertig.\n\n"));

          /* DEBUG-Ausgabe */
          INFO (("Trete in Schleife der Ereignisbehandlung ein...\n"));

          glutMainLoop ();
        }
      else
        {
          /* DEBUG-Ausgabe */
          INFO (("...fehlgeschlagen.\n\n"));

          glutDestroyWindow (windowID);
          windowID = 0;
        }
    }
  else
    {
      /* DEBUG-Ausgabe */
      INFO (("...fehlgeschlagen.\n\n"));
    }

  return windowID;
}
