#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <src/renderer.h>
#include <src/scene.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>

// Callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, std::vector<ModelLoader>& models, int& activeModelIndex);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Global variables
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;
Camera camera(glm::vec3(-20.0f, 2.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
int activeModelIndex = 0;
glm::vec3 ramPosition = glm::vec3(4.5f, -0.15f, 0.0f);
//lighting
glm::vec3 lightPos(1.0f, 0.5f, 1.0f);

int main() {
    // GLFW Initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "2024_GR1CC_GR8_Proyecto_NiseN", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Centrar la ventana en el monitor principal
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    int monitorX, monitorY, monitorWidth, monitorHeight;
    glfwGetMonitorWorkarea(primaryMonitor, &monitorX, &monitorY, &monitorWidth, &monitorHeight);
    int windowX = monitorX + (monitorWidth - SCR_WIDTH) / 2;
    int windowY = monitorY + (monitorHeight - SCR_HEIGHT) / 2;
    glfwSetWindowPos(window, windowX, windowY);

    glfwMakeContextCurrent(window); // Establece el contexto de OpenGL para la ventana.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Ajusta la ventana al redimensionar.

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Capturar el mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // Load shaders
    Shader modelShader("shaders/shadermloading.vs", "shaders/shadermloading.fs");
    Shader lightingShader("shaders/colors.vs", "shaders/colors.fs");
    Shader lightCubeShader("shaders/lightcube.vs", "shaders/lightcube.fs");
    Shader modelLight("shaders/modelLight.vs", "shaders/modelLight.fs");

    // Scene setup
    Scene scene;
    scene.addModel(ModelLoader("models/space/espacio.obj", glm::vec3(0.0f), glm::vec3(2.0f, 1.0f, 2.0f),glm::vec3(0.0f),false));
    scene.addModel(ModelLoader("models/garage/garage.obj", glm::vec3(0.0f), glm::vec3(0.05f),glm::vec3(0.0f), true));
    /*scene.addModel(ModelLoader("models/security_room_/security.obj", glm::vec3(-13.0f, 0.07f, -3.0f), glm::vec3(0.4f), glm::vec3(-90.0f, 90.0f, 90.0f),true));
    scene.addModel(ModelLoader("models/senial_traffic/senial_traffic.obj", glm::vec3(-13.0f, 0.0f, 3.0f), glm::vec3(0.2f), glm::vec3(0.0f, -90.0f, 0.0f), true));
    scene.addModel(ModelLoader("models/escritorio_deco/escritorio.obj", glm::vec3(-4.0f, 0.0f, 4.0f), glm::vec3(0.5f), glm::vec3(-180.0f, -90.0f, -180.0f),true));
    scene.addModel(ModelLoader("models/barril_deco/barril_deco.obj", glm::vec3(-7.5f, 0.0f, 2.5f), glm::vec3(1.0f), glm::vec3(-180.0f, -90.0f, -180.0f),true));
    scene.addModel(ModelLoader("models/llantas_deco/llantas_deco.obj", glm::vec3(-7.5f, 0.5f, -3.0f), glm::vec3(1.0f), glm::vec3(-180.0f, -180.0f, -180.0f),true));
    scene.addModel(ModelLoader("models/lampara_techo/lampara_techo.obj", glm::vec3(1.75f, 1.5f, 0.0f), glm::vec3(0.006f), glm::vec3(-180.0f, -180.0f, -180.0f),true));
    scene.addModel(ModelLoader("models/linterna_mano/linterna_mano.obj", glm::vec3(1.60f, 2.0f, 3.75f), glm::vec3(0.1f), glm::vec3(-180.0f, -180.0f, -180.0f),true));
    scene.addModel(ModelLoader("models/Logo_NiseN/Logo.obj", glm::vec3(-7.75f, 4.5f, 0.0f), glm::vec3(1.0f), glm::vec3(-180.0f, 90.0f, -180.0f), true));*/
    /*scene.addModel(ModelLoader("models/ram_1500/RAM.obj", glm::vec3(4.5f, -0.15f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f, -90.0f, 0.0f),true));
    scene.addModel(ModelLoader("models/porsche_gt3/porsche.obj", glm::vec3(-4.25f, 0.0f, -3.0f), glm::vec3(0.75f), glm::vec3(0.0f, 0.0f, 0.0f),true));
    scene.addModel(ModelLoader("models/chevroletCamaro/Camaro.obj", glm::vec3(0.0f, 0.0f, -2.5f), glm::vec3(0.3f), glm::vec3(0.0f, 0.0f, 0.0f),true));*/
    scene.addModel(ModelLoader("models/lamborghini_aventador/Lambo.obj", glm::vec3(0.0f, -0.15f, 3.0f), glm::vec3(0.15f), glm::vec3(-180.0f, 0.0f, 180.0f),false));
    Renderer renderer;

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    glm::vec3 cubePositions[] = {
    glm::vec3(6.0f, 2.5f, 0.0f),
    glm::vec3(-5.7f, 2.5f, 0.0f),
    glm::vec3(0.0f, 2.5f, -4.0f),
    glm::vec3(0.0f, 2.5f, 4.0f)
    };

    // Posiciones de los 4 cubos de luz
    glm::vec3 lightPositions[] = {
        glm::vec3(-6.0f,  2.8f,  5.3f),
        glm::vec3(-6.0f, 2.8f, -5.3f),
        glm::vec3(6.0f,  2.8f, -5.3f),
        glm::vec3(6.0f,  2.8f, 5.3f),
        glm::vec3(6.0f,  2.8f,  4.0f),
        glm::vec3(6.0f, 2.8f, -4.0f),
        glm::vec3(-5.7f,  2.8f, -4.0f),
        glm::vec3(-5.7f,  2.8f, 4.0f),
        glm::vec3(0.0f,  2.3f, 0.0f),
        glm::vec3(0.0f,  2.5f, 3.5f),
        glm::vec3(0.0f,  2.5f, -3.5f),
        glm::vec3(5.5f, 2.5f, 0.0f),
        glm::vec3(-5.2f, 2.5f, 0.0f)
    };

    // Colores de cada cubo de luz
    glm::vec3 lightColors[] = {
        glm::vec3(1.0f, 1.0f, 0.0f), // Amarillo
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 1.0f),  // Blanco
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f),
        glm::vec3(1.0f),
        glm::vec3(1.0f),
        glm::vec3(1.0f)
    };

    // Tamaño de cada cubo de luz
    glm::vec3 lightScale[] = {
        glm::vec3(0.5f), // Amarillo
        glm::vec3(0.5f),
        glm::vec3(0.5f),
        glm::vec3(0.5f),
        glm::vec3(0.4f),  // Blanco
        glm::vec3(0.4f),
        glm::vec3(0.4f),
        glm::vec3(0.4f),
        glm::vec3(0.6f),
        glm::vec3(0.2f),
        glm::vec3(0.2f),
        glm::vec3(0.2f),
        glm::vec3(0.2f)
    };

    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Exercise 13 Task 2
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


   /* int baseIndex = scene.getModels().size() - 4;
    scene.getModels()[baseIndex + activeModelIndex].moveTo(ramPosition);*/

    camera.MovementSpeed = 5;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, scene.getModels(), activeModelIndex);
        // Configurar iluminación para modelos
        modelLight.use();
        modelLight.setVec3("viewPos", camera.Position);
        modelLight.setFloat("shininess", 64.0f);

        // Luz direccional
        modelLight.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        modelLight.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        modelLight.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        modelLight.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        // Luces puntuales
        for (int i = 0; i < 4; i++) {
            std::string prefix = "pointLights[" + std::to_string(i) + "]";
            modelLight.setVec3(prefix + ".position", lightPositions[i]);
            modelLight.setVec3(prefix + ".ambient", glm::vec3(0.05f));
            modelLight.setVec3(prefix + ".diffuse", glm::vec3(0.8f));
            modelLight.setVec3(prefix + ".specular", glm::vec3(1.0f));
            modelLight.setFloat(prefix + ".constant", 1.0f);
            modelLight.setFloat(prefix + ".linear", 0.09f);
            modelLight.setFloat(prefix + ".quadratic", 0.032f);
        }

        // Spotlight
        modelLight.setVec3("spotLight.position", camera.Position);
        modelLight.setVec3("spotLight.direction", camera.Front);
        modelLight.setVec3("spotLight.ambient", glm::vec3(0.0f));
        modelLight.setVec3("spotLight.diffuse", glm::vec3(1.0f));
        modelLight.setVec3("spotLight.specular", glm::vec3(1.0f));
        modelLight.setFloat("spotLight.constant", 1.0f);
        modelLight.setFloat("spotLight.linear", 0.09f);
        modelLight.setFloat("spotLight.quadratic", 0.032f);
        modelLight.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        modelLight.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


        // Proyección y vista
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        modelLight.setMat4("projection", projection);
        modelLight.setMat4("view", view);

        renderer.render(scene, modelLight, modelShader, camera);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

        //Exercise 13 Task 2
        lightingShader.setVec3("lightPos", lightPos);

        //Exercise 13 Task 3
        lightingShader.setVec3("viewPos", camera.Position);

        // view/projection transformations
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        
        // world transformation
        for (unsigned int i = 0; i < 4; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::scale(model, glm::vec3(0.5f));
            lightingShader.use();
            lightingShader.setMat4("model", model);

            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // render the cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Dibujar los cubos de luz
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        glBindVertexArray(lightCubeVAO);

        for (unsigned int i = 0; i < 13; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, lightPositions[i]);
            model = glm::scale(model, lightScale[i]); // Cubo más pequeño

            lightCubeShader.setMat4("model", model);
            lightCubeShader.setVec3("lightColor", lightColors[i]); // Pasar color al shader
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Process input
void processInput(GLFWwindow* window, std::vector<ModelLoader>& models, int& activeModelIndex) {
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    

    //if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    //    int baseIndex = models.size() - 4;
    //    models[baseIndex + activeModelIndex].rotateY(1.0f * deltaTime * 50.0f);
    //} // (para activar el movimiento de la troca) 

    //if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    //    int baseIndex = models.size() - 4;
    //    // Guardar la posición actual del modelo activo
    //    glm::vec3 currentPosition = models[baseIndex + activeModelIndex].getPosition();
    //    // Cambiar al siguiente modelo activo
    //    int previousModelIndex = activeModelIndex; // Guardar índice del modelo anterior
    //    activeModelIndex = (activeModelIndex + 1) % 4;
    //    // Mover el modelo anterior a la posición del nuevo modelo activo
    //    models[baseIndex + previousModelIndex].moveTo(models[baseIndex + activeModelIndex].getPosition());
    //    // Mover el nuevo modelo activo a la posición `ramPosition`
    //    models[baseIndex + activeModelIndex].moveTo(ramPosition);
    //}

    //if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    //    int baseIndex = models.size() - 4;
    //    // Guardar la posición actual del modelo activo
    //    glm::vec3 currentPosition = models[baseIndex + activeModelIndex].getPosition();
    //    // Cambiar al modelo anterior
    //    int previousModelIndex = activeModelIndex; // Guardar índice del modelo anterior
    //    activeModelIndex = (activeModelIndex - 1 + 4) % 4;
    //    // Mover el modelo anterior a la posición del nuevo modelo activo
    //    models[baseIndex + previousModelIndex].moveTo(models[baseIndex + activeModelIndex].getPosition());
    //    // Mover el nuevo modelo activo a la posición `ramPosition`
    //    models[baseIndex + activeModelIndex].moveTo(ramPosition);
    //}
    
}

// Window resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Mouse callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// Scroll callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

