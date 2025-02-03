#version 330 core

// Entradas
layout(location = 0) in vec3 aPos;       // Posición del vértice
layout(location = 1) in vec3 aNormal;    // Normal del vértice
layout(location = 2) in vec2 aTexCoords; // Coordenadas de textura
layout(location = 3) in vec3 aTangent;   // Tangente para el mapeado normal

// Salidas
out vec3 FragPos;       // Posición del fragmento en espacio mundial
out vec2 TexCoords;     // Coordenadas de textura
out vec3 TangentLightPos; // Posición de la luz en espacio tangente
out vec3 TangentViewPos;  // Posición de la cámara en espacio tangente
out vec3 TangentFragPos;  // Posición del fragmento en espacio tangente

// Uniformes
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;  // Posición de la luz en espacio mundial
uniform vec3 viewPos;   // Posición de la cámara en espacio mundial

void main()
{
    // Transformar la posición del vértice a espacio mundial
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;

    // Calcular la matriz TBN (Tangent, Bitangent, Normal)
    vec3 T = normalize(mat3(model) * aTangent);
    vec3 N = normalize(mat3(model) * aNormal);
    T = normalize(T - dot(T, N) * N); // Ortogonalizar
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));

    // Transformar posiciones a espacio tangente
    TangentLightPos = TBN * lightPos;
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;

    // Proyectar la posición del vértice
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}