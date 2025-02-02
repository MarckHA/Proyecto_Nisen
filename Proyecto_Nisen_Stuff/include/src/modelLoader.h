#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <learnopengl/model.h>
#include <learnopengl/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ModelLoader {
public:
    Model model;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;

    // Constructor con rotación incluida
    ModelLoader(const std::string& path, 
                glm::vec3 pos = glm::vec3(0.0f), 
                glm::vec3 scl = glm::vec3(1.0f), 
                glm::vec3 rot = glm::vec3(0.0f)) 
        : model(path), position(pos), scale(scl), rotation(rot) {}

    // Dibujar el modelo con la transformación aplicada
    void draw(Shader& shader) {
        glm::mat4 modelMatrix = glm::mat4(1.0f);

        // Aplicar traslación
        modelMatrix = glm::translate(modelMatrix, position);
        
        // Aplicar rotación en X, Y y Z
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotar en X
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotar en Y
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotar en Z
        
        // Aplicar escala
        modelMatrix = glm::scale(modelMatrix, scale);

        // Pasar la matriz transformada al shader
        shader.use();
        shader.setMat4("model", modelMatrix);

        // Dibujar el modelo
        model.Draw(shader);
    }
};

#endif
