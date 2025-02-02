#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <src/renderer.h>
#include <src/scene.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>

// Callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Global variables
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    // GLFW Initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Multi-Model Scene", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
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
    Shader shader("shaders/shadermloading.vs", "shaders/shadermloading.fs");

    // Scene setup
    Scene scene;
    scene.addModel(ModelLoader("models/space/espacio.obj", glm::vec3(0.0f), glm::vec3(2.0f, 1.0f,2.0f)));
    scene.addModel(ModelLoader("models/garage/garage.obj", glm::vec3(0.0f), glm::vec3(0.05f)));
    scene.addModel(ModelLoader("models/security_room_/security.obj", glm::vec3(-13.0f, 0.07f, -3.0f), glm::vec3(0.4f), glm::vec3(-90.0f, 90.0f, 90.0f)));
    scene.addModel(ModelLoader("models/traffic/traffic.obj", glm::vec3(-13.0f, 0.0f, 3.0f), glm::vec3(0.2f), glm::vec3(0.0f, -90.0f, 0.0f)));
    scene.addModel(ModelLoader("models/ram_1500/RAM.obj", glm::vec3(4.5f, -0.15f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f, -90.0f, 0.0f)));
    scene.addModel(ModelLoader("models/chevroletCamaro/Camaro.obj", glm::vec3(0.0f, 0.0f, -2.5f), glm::vec3(0.3f), glm::vec3(0.0f, 0.0f, 0.0f)));
    scene.addModel(ModelLoader("models/lamborghini_aventador/Lambo.obj", glm::vec3(0.0f, -0.15f, 3.0f), glm::vec3(0.15f), glm::vec3(-180.0f, 0.0f, 180.0f)));
    scene.addModel(ModelLoader("models/porsche_gt3/porsche.obj", glm::vec3(-4.25f, 0.0f, -3.0f), glm::vec3(0.75f), glm::vec3(0.0f, 0.0f, 0.0f))); 
    scene.addModel(ModelLoader("models/escritorio_deco/escritorio.obj", glm::vec3(-4.0f, 0.0f, 4.0f), glm::vec3(0.5f), glm::vec3(-180.0f, -90.0f, -180.0f)));
    scene.addModel(ModelLoader("models/barril_deco/barril_deco.obj", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f), glm::vec3(-180.0f, -90.0f, -180.0f)));
    Renderer renderer;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        renderer.render(scene, shader, camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Process input
void processInput(GLFWwindow* window) {
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
