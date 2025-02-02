#ifndef RENDERER_H
#define RENDERER_H

#include "Scene.h"
#include <learnopengl/camera.h>

class Renderer {
public:
    void render(Scene& scene, Shader& shader, Camera& camera) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setVec3("viewPos", camera.Position);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        scene.draw(shader);
    }
};

#endif
