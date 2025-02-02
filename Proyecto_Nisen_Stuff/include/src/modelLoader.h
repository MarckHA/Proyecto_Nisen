#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <learnopengl/model.h>
#include <learnopengl/shader.h>
#include <glm/glm.hpp>

class ModelLoader {
public:
    Model model;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;

    ModelLoader(const std::string& path, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
        : model(path), position(pos), scale(scale), rotation(glm::vec3(0.0f)) {}

    void draw(Shader& shader) {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::scale(modelMatrix, scale);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

        shader.use();
        shader.setMat4("model", modelMatrix);
        model.Draw(shader);
    }
};

#endif
