#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <learnopengl/model.h>
#include <learnopengl/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <learnopengl/stb_image.h>
#include <string>

class ModelLoader {
public:
    Model model;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    glm::vec3 originalPosition;
    bool useLighting;

    // IDs de texturas
    unsigned int diffuseMap;   // map_kd
    unsigned int specularMap;  // map_ks
    unsigned int normalMap;    // map_normal

    // Constructor
    ModelLoader(const std::string& path,
        glm::vec3 pos = glm::vec3(0.0f),
        glm::vec3 scl = glm::vec3(1.0f),
        glm::vec3 rot = glm::vec3(0.0f),
        bool lighting = true,
        const std::string& diffusePath = "",
        const std::string& specularPath = "",
        const std::string& normalPath = "")
        : model(path), position(pos), originalPosition(pos), scale(scl), rotation(rot), useLighting(lighting)
    {
        // Cargar texturas
        if (!diffusePath.empty()) {
            diffuseMap = loadTexture(diffusePath.c_str());
        }
        if (!specularPath.empty()) {
            specularMap = loadTexture(specularPath.c_str());
        }
        if (!normalPath.empty()) {
            normalMap = loadTexture(normalPath.c_str());
        }
    }

    // Dibujar el modelo con la transformación aplicada
    void draw(Shader& lightingShader, Shader& noLightingShader) {
        glm::mat4 modelMatrix = glm::mat4(1.0f);

        // Aplicar traslación
        modelMatrix = glm::translate(modelMatrix, position);

        // Aplicar rotación
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        // Aplicar escala
        modelMatrix = glm::scale(modelMatrix, scale);

        // Configurar los uniformes
        if (useLighting) { 
            lightingShader.use();
            lightingShader.setMat4("model", modelMatrix);

            // Vincular texturas
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap);
            lightingShader.setInt("map_kd", 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularMap);
            lightingShader.setInt("map_ks", 1);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, normalMap);
            lightingShader.setInt("map_normal", 2);
            // Dibujar el modelo
            model.Draw(lightingShader);
        }
        else {
            noLightingShader.use();
            noLightingShader.setMat4("model", modelMatrix);
            model.Draw(noLightingShader);
        }
   
        
    }

    // Métodos adicionales
    void moveTo(glm::vec3 newPos) {
        position = newPos;
    }

    void resetPosition() {
        position = originalPosition;
    }

    void rotateY(float degrees) {
        rotation.y += degrees;
    }

    glm::vec3 getPosition() const {
        return position;
    }

private:
    // Cargar textura desde archivo
    unsigned int loadTexture(const char* path) {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
        if (data) {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else {
            std::cerr << "Failed to load texture at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }
};

#endif
