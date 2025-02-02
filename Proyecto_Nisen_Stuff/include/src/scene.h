#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "ModelLoader.h"

class Scene {
public:
    std::vector<ModelLoader> models;

    void addModel(const ModelLoader& model) {
        models.push_back(model);
    }

    void draw(Shader& shader) {
        for (auto& model : models) {
            model.draw(shader);
        }
    }
};

#endif
