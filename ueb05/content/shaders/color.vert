#version 330 core

/** 
 * Position des Vertex.
 */
layout (location = 0) in vec4 vPosition;

/**
 * Textur-Koordinate des Vertex (Heightmap).
 */
layout (location = 1) in vec2 vHeightCoord;

/**
 * Textur-Koordinate des Vertex (Rock-Textur).
 */
layout (location = 2) in vec2 vRockCoord;

/**
 * Textur-Koordinate des Vertex (Snow-Textur).
 */
layout (location = 3) in vec2 vSnowCoord;

/** 
 * Ausgabe des Vertex-Shaders. 
 * Die Textur-Koordinate wird interpoliert vom Fragment-Shader 
 * entgegengenommen.
 */
out vec2 fTexCoord;

/**
 * Ausgabe der Normalen
 */
out vec4 Normal;

/**
 * Ausgabe der Intensitaeten
 */
out vec4 Intensity;

/** 
 * Projektions-Matrix.
 * Wird dem Shader mit einem Aufruf von glUniformMatrix4fv übergeben.
 */
uniform mat4 Projection;

/**
 * View-Matrix. 
 * Wird dem Shader mit einem Aufruf von glUniformMatrix4fv übergeben.
 */
uniform mat4 ModelView;

/**
 * Texture-Sampler, um auf die Heightmap-Textur zuzugreifen. Die Einstellungen
 * des Samplers (Interpolation und Randbehandlung) werden beim Erstellen
 * der Textur gesetzt.
 */
uniform sampler2D Heightmap;

/**
 * Betrachterposition fuer die Lichtberechnung
 */
uniform vec4 ViewPos;

/**
 * Lichtposition fuer die Lichtberechnung
 */
uniform vec4 LightPos;

/**
 * Ambienter Teil der Materialeigenschaften
 */
uniform vec4 Ambient;

/**
 * Spekularer Teil der Materialeigenschaften
 */
uniform vec4 Specular;

/**
 * Diffuser Teil der Materialeigenschaften
 */
uniform vec4 Diffuse;

/**
 * Schillernder Teil der Materialeigenschaften
 */
uniform float Shininess;

/**
 * Wahrheitswert, ob die Heightmap eingeschaltet sind
 */
uniform int ToggleHeight;

/**
 * Wahrheitswert, welcher Shader eingeschaltet ist
 */
uniform int ToggleShader;

/**
 * Wahrheitswert, ob die Sinuswelle eingeschaltet ist
 */
uniform int ToggleSinus;

/**
 * Wahrheitswert, ob die Pause eingeschaltet ist
 */
uniform int TogglePause;

/**
 * Distanz der Vertices zueinander
 */
uniform float VertexDistance;

/**
 * Anhebung der Vertizes.
 * Wird dem Shader mit einem Aufruf von glUniform1f übergeben.
 */
uniform float TexelDistance;

/**
 * Zeitintervall aus der Host-Anwendung
 */
uniform float Time;

/**
 * Hauptprogramm des Vertex-Shaders.
 * Diese Funktion wird für jeden Vertex ausgeführt!
 */
void main(void)
{
  /* Neue Vertex-Position berechnen auf Grundlage der übergebenen
   * Anhebung. vPosition ist die Vertex-Position, die von der Host-
   * Anwendung im Buffer gespeichert wurde. Die w-Komponente wird
   * von der OpenGL-Implementierung automatisch auf 1 gesetzt. */

  vec4 elevatedPosition;
  vec3 normalVectorXLeft;
  vec3 normalVectorXRight;
  vec3 normalVectorZUp;
  vec3 normalVectorZDown;
  vec3 normalVector;
  vec2 otherHeight;
  float wave = 10.0f;
  float time = Time;

  elevatedPosition.x = vPosition.x;
  elevatedPosition.y = vPosition.y;
  elevatedPosition.z = vPosition.z;
  elevatedPosition.w = vPosition.w;

  if (TogglePause != 0) {
    time = 0;
  }

  /* Veraenderung der Hoehe */
  if(ToggleHeight == 0) 
  {
    elevatedPosition.y += texture2D(Heightmap, vHeightCoord.st).r;
  }
    
  /* Veraenderung der Hoehe durch Sinus */
  if (ToggleSinus == 0) 
  {
    elevatedPosition.y += (sin((elevatedPosition.x + elevatedPosition.z + time)*wave)/wave);
  } 

  /* Normalenberechnung */
  normalVector.x = 0.0;
  normalVector.y = 1.0;
  normalVector.z = 0.0;
  normalVectorXLeft  = normalVector;
  normalVectorXRight = normalVector;
  normalVectorZUp    = normalVector;
  normalVectorZDown  = normalVector;

  normalVectorXLeft.x  = elevatedPosition.x - VertexDistance;
  normalVectorXLeft.z  = elevatedPosition.z;
  normalVectorXRight.x = elevatedPosition.x + VertexDistance;
  normalVectorXRight.z = elevatedPosition.z;
  normalVectorZUp.x    = elevatedPosition.x;
  normalVectorZUp.z    = elevatedPosition.z + VertexDistance;
  normalVectorZDown.x  = elevatedPosition.x;
  normalVectorZDown.z  = elevatedPosition.z - VertexDistance;

  if (ToggleSinus == 0) {
    normalVectorXLeft.y  = (sin(((elevatedPosition.x - VertexDistance) + elevatedPosition.z + time)*wave)/wave);
    normalVectorXRight.y = (sin(((elevatedPosition.x + VertexDistance) + elevatedPosition.z + time)*wave)/wave);
    normalVectorZUp.y    = (sin((elevatedPosition.x + (elevatedPosition.z + VertexDistance) + time)*wave)/wave);
    normalVectorZDown.y  = (sin((elevatedPosition.x + (elevatedPosition.z - VertexDistance) + time)*wave)/wave);
  }

  if(ToggleHeight == 0) 
  {
    if(vHeightCoord.s != 0.0 && vHeightCoord.s != 1.0 && 
       vHeightCoord.t != 0.0 && vHeightCoord.t != 1.0) 
    {
      otherHeight.s         = vHeightCoord.s - TexelDistance;
      otherHeight.t         = vHeightCoord.t;
      normalVectorXLeft.y  +=  texture2D(Heightmap, otherHeight.st).r;

      otherHeight.s         = vHeightCoord.s + TexelDistance;
      otherHeight.t         = vHeightCoord.t;
      normalVectorXRight.y +=  texture2D(Heightmap, otherHeight.st).r;

      otherHeight.s         = vHeightCoord.s;
      otherHeight.t         = vHeightCoord.t + TexelDistance;
      normalVectorZUp.y    +=  texture2D(Heightmap, otherHeight.st).r;

      otherHeight.s         = vHeightCoord.s;
      otherHeight.t         = vHeightCoord.t - TexelDistance;
      normalVectorZDown.y  +=  texture2D(Heightmap, otherHeight.st).r;
    }
  }
  vec3 normalX = normalVectorXLeft - normalVectorXRight;
  vec3 normalZ = normalVectorZUp - normalVectorZDown;

  normalVector = normalize(abs(cross((normalZ), (normalX))));

  Normal = vec4(normalize(normalVector), 1.0);

  //Normal = vec4(0,normalVector.y * -1,0, 1.0);

  /* Die Textur-Koordinate wird untransformiert an den Fragment-
   * Shader weitergereicht. Bei der Rasterization wird dieser Wert
   * jedoch interpoliert. */
  fTexCoord = vHeightCoord;

  if(ToggleShader != 0)
  {
    // ambient
    vec3 ambient = Ambient.rgb;

    // diffuse 
    vec4 lightDir = normalize(LightPos - elevatedPosition);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * Diffuse.rgb;

    // specular
    vec4 viewDir = normalize(ViewPos - elevatedPosition);
    vec4 reflectDir = reflect(-lightDir, Normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    vec3 specular = spec * Specular.rgb;  

    vec4 result = vec4((ambient + diffuse + specular), 1.0);
    Intensity = result;
  }

  /* Setzen der Vertex-Position im Device-Koordinatensystem.
   * Nachfolgend findet das Clipping und die Rasterization statt. */
  gl_Position = Projection * ModelView * elevatedPosition;   
}
