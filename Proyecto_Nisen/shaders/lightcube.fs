#version 330 core
out vec4 FragColor;

uniform vec3 lightColor; // Recibe el color de cada cubo de luz

void main()
{
    FragColor = vec4(lightColor, 1.0); // Usa el color recibido
}