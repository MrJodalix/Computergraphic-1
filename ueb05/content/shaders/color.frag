#version 330 core

/**
 * Interpolierte Textur-Koordinate des Fragments.
 */
in vec2 fTexCoord;

/**
 * Interpolierte Normalen fuer Lichtberechnung
 */
in vec4 Normal;

/**
 * Interpolierte Intensitaet fuer Lichtberechnung
 */
in vec4 Intensity;

/**
 * Farbe des Fragments, die auf den Framebuffer geschrieben wird.
 */
out vec4 FragColor;

/**
 * Texture-Sampler, um auf die Heightmap-Textur zuzugreifen. Die Einstellungen
 * des Samplers (Interpolation und Randbehandlung) werden beim Erstellen
 * der Textur gesetzt.
 */
uniform sampler2D Heightmap;

/**
 * Texture-Sampler, um auf die Rock-Textur zuzugreifen. Die Einstellungen
 * des Samplers (Interpolation und Randbehandlung) werden beim Erstellen
 * der Textur gesetzt.
 */
uniform sampler2D Rock;

/**
 * Texture-Sampler, um auf die Snow-Textur zuzugreifen. Die Einstellungen
 * des Samplers (Interpolation und Randbehandlung) werden beim Erstellen
 * der Textur gesetzt.
 */
uniform sampler2D Snow;

/**
 * Betrachterposition für Lichtberechnung
 */
uniform vec4 ViewPos;

/**
 * Lichtposition für Lichtberechnung
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
 * Wahrheitswert, ob die Texturen eingeschaltet sind
 */
uniform int ToggleTexture;

/**
 * Wahrheitswert, welcher Shader eingeschaltet ist
 */
uniform int ToggleShader;

/**
 * Hauptprogramm des Fragment-Shaders.
 * Diese Funktion wird für jedes Fragment ausgeführt!
 */
void main(void)
{  
  vec4 result;
  vec4 norm = normalize(Normal);

  if(ToggleShader != 0)
  {
    result = Intensity;
  } 
  else 
  {
    // ambient
    vec3 ambient = Ambient.rgb;

    // diffuse 
    vec4 lightDir = abs(normalize(LightPos - gl_FragCoord));
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * Diffuse.rgb;

    // specular
    vec4 viewDir = normalize(ViewPos - gl_FragCoord);
    vec4 reflectDir = normalize(reflect(-lightDir, norm));  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    vec3 specular = spec * Specular.rgb;  

    result = vec4((ambient + diffuse + specular), 1.0);
  }

  if(ToggleTexture != 0)
  {
    FragColor = result;
    //FragColor = norm;
  } 
  else 
  {
    if (texture2D(Heightmap, fTexCoord).t <= 0.2f) 
    {
      FragColor = texture(Rock, fTexCoord) * result;
    } 
    else if (texture2D(Heightmap, fTexCoord).t >= 0.65f)
    {
      FragColor = texture(Snow, fTexCoord) * result;
    } 
    else 
    {
      float a = smoothstep(0.2, 0.65, texture2D(Heightmap, fTexCoord).t);
      vec4 final_texture = mix(texture(Rock, fTexCoord), texture(Snow, fTexCoord), a);
      FragColor = final_texture * result;
    }
  }
}
