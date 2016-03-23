#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <shader.h>
#include <camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL.h>
#include <stdio.h>
#include <stdarg.h>

const GLuint SCR_WIDTH = 800, SCR_HEIGHT = 600;

GLuint loadTexture(GLchar const * path);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void cameraMovement();
void RenderScene(Shader &shader);
void RenderCube();
void RenderQuad();

Camera camera(glm::vec3(1.0f, 5.0f, 18.0f));
glm::vec3 lightPos(-2.0f, 5.0f, -2.0f);

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLboolean shadows = true;

GLuint woodTexture;
GLuint planeVAO;

const size_t messageSize = (1024 * 4);
char messages[messageSize];
int bytesUsed = 0; 

int myvsnprintf(char* s, size_t n, const char* format, ...)
{
	int ret = 0;
    va_list args;
	va_start (args, format);
    ret = vsnprintf(s, n, format, args);
	va_end(args);
	return ret;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Shadow Map Demo", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    // Set callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // glfw Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();

    woodTexture = loadTexture("./textures/wood.png");
    Shader shader("./shaders/shadow_mapping.vs", "./shaders/shadow_mapping.frag");
    Shader simpleDepthShader("./shaders/shadow_mapping_depth.vs", "./shaders/shadow_mapping_depth.frag");
    Shader debugDepthQuad("./shaders/debug_quad.vs", "./shaders/debug_quad_depth.frag");
    shader.Use();

    // Set texture samples
    glUniform1i(glGetUniformLocation(shader.Program, "diffuseTexture"), 0);
    glUniform1i(glGetUniformLocation(shader.Program, "shadowMap"), 1);

    GLfloat planeVertices[] =
	{
        // Positions            // Normals           // Texture Coords
         25.0f, -0.5f,  25.0f,  0.0f,  1.0f,  0.0f,  25.0f, 0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f,  1.0f,  0.0f,  0.0f,  25.0f,
        -25.0f, -0.5f,  25.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,

         25.0f, -0.5f,  25.0f,  0.0f,  1.0f,  0.0f,  25.0f, 0.0f,
         25.0f, -0.5f, -25.0f,  0.0f,  1.0f,  0.0f,  25.0f, 25.0f,
        -25.0f, -0.5f, -25.0f,  0.0f,  1.0f,  0.0f,  0.0f,  25.0f
    };

    GLuint planeVBO;
    glGenBuffers(1, &planeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices),
			     &planeVertices, GL_STATIC_DRAW);
	// need unbind buffer here?
	
    glGenVertexArrays(1, &planeVAO);
    glBindVertexArray(planeVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		                  8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		                  8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
	                      8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glBindVertexArray(0);

    // Create depth as a texture
    const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        cameraMovement();

        lightPos.z = cos(glfwGetTime()) * 2.0f; // Change light position over time

        // 1. Render depth of scene to texture (from light's perspective)
        //    Get light projection/view matrix.
        glm::mat4 lightProjection, lightView, lightSpaceMatrix;
        GLfloat near_plane = 1.0f, far_plane = 7.5f;
        //lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightProjection = glm::perspective(45.0f, (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(1.0));
        lightSpaceMatrix = lightProjection * lightView;
        //   now render scene from light's point of view
        simpleDepthShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(simpleDepthShader.Program, "lightSpaceMatrix"),
												1,
												GL_FALSE,
												glm::value_ptr(lightSpaceMatrix));
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        RenderScene(simpleDepthShader);

        // 2. Render scene as normal
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.Use();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"),
			               1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"),
			               1, GL_FALSE, glm::value_ptr(view));
        glUniform3fv(glGetUniformLocation(shader.Program, "lightPos"),
			                              1, &lightPos[0]);
        glUniform3fv(glGetUniformLocation(shader.Program, "viewPos"),
										1, &camera.Position[0]);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "lightSpaceMatrix"),
			                                    1,
												GL_FALSE,
												glm::value_ptr(lightSpaceMatrix));
        // Enable/Disable shadows by pressing 'SPACE'
        glUniform1i(glGetUniformLocation(shader.Program, "shadows"), shadows);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        RenderScene(shader);

        // 3. DEBUG: visualize depth map by rendering it to plane
        debugDepthQuad.Use();
        glUniform1f(glGetUniformLocation(debugDepthQuad.Program, "near_plane"), near_plane);
        glUniform1f(glGetUniformLocation(debugDepthQuad.Program, "far_plane"), far_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        //RenderQuad(); // uncomment this line to see depth map

        // Swap the buffers
        glfwSwapBuffers(window);

		printf("%s", messages);
		//printf("Bytes used: %d\n", bytesUsed);
		bytesUsed = 0;
		messages[0] = '\0';
    }

    glfwTerminate();
    return 0;
}

void RenderScene(Shader &shader)
{
    //render Floor
    glm::mat4 model;
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    //render Cubes
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    RenderCube();
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    RenderCube();
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
    model = glm::rotate(model, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.5));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    RenderCube();
}

// RenderQuad() Renders a 1x1 quad in NDC, best used for framebuffer color targets
// and post-processing effects.
GLuint quadVAO = 0;
GLuint quadVBO;
void RenderQuad()
{
    if (quadVAO == 0)
    {
        GLfloat quadVertices[] =
		{
            // Positions        // Texture Coords
            -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
             1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
             1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        };

        // Setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

// RenderCube() Renders a 1x1 3D cube in NDC.
GLuint cubeVAO = 0;
GLuint cubeVBO = 0;
void RenderCube()
{
    // Initialize (if necessary)
    if (cubeVAO == 0)
    {
        GLfloat vertices[] =
		{
            // Back face
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
            // Front face
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
            // Left face
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
            // Right face
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
            // Bottom face
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            // Top face
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);

        // Fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    // Render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// This function loads a texture from file.
// Note: texture loading functions are usually
// managed by a 'Resource Manager'
// that manages all resources (like textures, models, audio).
// we'll just define it as a utility function.
GLuint loadTexture(GLchar const * path)
{
	bytesUsed += myvsnprintf(messages + bytesUsed, messageSize - bytesUsed, "Load texture from file: %s\n", path);
    int width, height;
    unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	if (NULL == image)
	{
	    bytesUsed += myvsnprintf(messages + bytesUsed, messageSize - bytesUsed, "Load texture error! %d\n", __LINE__);
	}
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return textureID;
}

bool keys[1024];
bool keysPressed[1024];

// Moves/alters the camera positions based on user input
void cameraMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W])
    {   
		bytesUsed += myvsnprintf(messages + bytesUsed, messageSize - bytesUsed, "Camera Forward. \n");
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (keys[GLFW_KEY_S])
    {   
		bytesUsed += myvsnprintf(messages + bytesUsed, messageSize - bytesUsed, "Camera Backward. \n");
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (keys[GLFW_KEY_A])
    {   
		bytesUsed += myvsnprintf(messages + bytesUsed, messageSize - bytesUsed, "Camera Leftward. \n");
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (keys[GLFW_KEY_D])
    {   
		bytesUsed += myvsnprintf(messages + bytesUsed, messageSize - bytesUsed, "Camera Right. \n");
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (keys[GLFW_KEY_SPACE] && !keysPressed[GLFW_KEY_SPACE])
    {
        shadows = !shadows;
		bytesUsed += myvsnprintf(messages + bytesUsed, messageSize - bytesUsed, "Shadow Toggle. \n");
        keysPressed[GLFW_KEY_SPACE] = true;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key <= 1024)
    {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
            keysPressed[key] = false;
        }
    }
}

GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
	bytesUsed += myvsnprintf(messages + bytesUsed, messageSize - bytesUsed, "[%f, %f], <%f, %f> \n", xpos, ypos, xoffset, yoffset);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
