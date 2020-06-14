#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
GLFWwindow *window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>

#include "playground.hpp"

// One color for each vertex. They were generated randomly.
static GLfloat s_color_buffer_data[] = {
    0.583f, 0.771f, 0.014f,
    0.609f, 0.115f, 0.436f,
    0.327f, 0.483f, 0.844f,
    0.822f, 0.569f, 0.201f,
    0.435f, 0.602f, 0.223f,
    0.310f, 0.747f, 0.185f,
    0.597f, 0.770f, 0.761f,
    0.559f, 0.436f, 0.730f,
    0.359f, 0.583f, 0.152f,
    0.483f, 0.596f, 0.789f,
    0.559f, 0.861f, 0.639f,
    0.195f, 0.548f, 0.859f,
    0.014f, 0.184f, 0.576f,
    0.771f, 0.328f, 0.970f,
    0.406f, 0.615f, 0.116f,
    0.676f, 0.977f, 0.133f,
    0.971f, 0.572f, 0.833f,
    0.140f, 0.616f, 0.489f,
    0.997f, 0.513f, 0.064f,
    0.945f, 0.719f, 0.592f,
    0.543f, 0.021f, 0.978f,
    0.279f, 0.317f, 0.505f,
    0.167f, 0.620f, 0.077f,
    0.347f, 0.857f, 0.137f,
    0.055f, 0.953f, 0.042f,
    0.714f, 0.505f, 0.345f,
    0.783f, 0.290f, 0.734f,
    0.722f, 0.645f, 0.174f,
    0.302f, 0.455f, 0.848f,
    0.225f, 0.587f, 0.040f,
    0.517f, 0.713f, 0.338f,
    0.053f, 0.959f, 0.120f,
    0.393f, 0.621f, 0.362f,
    0.673f, 0.211f, 0.457f,
    0.820f, 0.883f, 0.371f,
    0.982f, 0.099f, 0.879f};

static glm::mat4 GenerateMVP(float width, float height);
static void RandomStepColor(GLfloat colors[], size_t size);
static GLuint LoadBMP(const char *imgPath);
static GLuint LoadDDS(const char *imgPath);

int main(void) {
    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                   GL_TRUE);  // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    int width = 1024;
    int height = 768;
    window = glfwCreateWindow(width, height, "Playground", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are "
                "not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Initialize VAO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // 0: Initialize vertex buffer
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
                 g_vertex_buffer_data, GL_STATIC_DRAW);

    // 1: Initialize uv buffer
    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("simple.vert", "simple.frag");

    // Get a handle for our "MVP" uniform
    // Only during the initialisation
    GLuint matrixID = glGetUniformLocation(programID, "mvp");

    // Get a handle for our "myTextureSampler" uniform
    // GLuint texture = LoadBMP("uvtemplate.bmp");
    GLuint texture = LoadDDS("uvtemplate.dds");
    GLuint textureID = glGetUniformLocation(programID, "textureSampler");

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
        mat4 mvp = GenerateMVP((float)width, (float)height);
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(textureID, 0);

        // 1st attribute buffer: vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,         // attribute 0. No particular reason for 0, but
                       // must match the layout in the shader.
            3,         // size
            GL_FLOAT,  // type
            GL_FALSE,  // normalized?
            0,         // stride
            (void *)0  // array buffer offset
        );

        // 2nd attribute buffer: texture UV
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
            1,         // attribute. No particular reason for 1, but must match the layout in the
                       // shader.
            2,         // size
            GL_FLOAT,  // type
            GL_FALSE,  // normalized?
            0,         // stride
            (void *)0  // array buffer offset
        );

        // Draw the triangle !
        // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }  // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}

static glm::mat4 GenerateMVP(float width, float height) {
    // Projection matrix : 45° Field of View, 4:3 ratio, display range:
    // 0.1 unit <-> 100 units
    mat4 Projection = glm::perspective(glm::radians(60.0f), width / height, 0.1f, 100.0f);

    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(4, 3, -3),  // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0),   // and looks at the origin
        glm::vec3(0, 1, 0)    // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);

    // Our ModelViewProjection : multiplication of our 3 matrices
    // Matrix multiplication is the other way around
    return Projection * View * Model;
}

static void RandomStepColor(GLfloat colors[], size_t size) {
    int i;
    for (i = 0; i < size; i++) {
        GLfloat col = colors[i] + (rand() & 20 - 10) * 0.01f / 60;
        colors[i] = (col < 0) ? 0.66f : (col > 1) ? 0.33f : col;
    }
}

static GLuint LoadBMP(const char *imgPath) {
    // Data read from the header of the BMP file
    // Each BMP file begins by a 54-bytes header
    unsigned char header[54];
    // Position in the file where the actual data begins
    unsigned int dataPos;
    unsigned int width, height;
    unsigned int imageSize;  // = width*height*3
    // Actual RGB data
    unsigned char *data;

    // Open the file
    FILE *file = fopen(imgPath, "rb");
    if (!file) {
        printf("Image could not be opened\n");
        return 0;
    }

    // If not 54 bytes read: problem
    if (fread(header, 1, 54, file) != 54) {
        printf("Not a correct BMP file\n");
        return 0;
    }

    // Read attributes from the header
    dataPos = *(int *)&(header[0x0A]);
    width = *(int *)&(header[0x12]);
    height = *(int *)&(header[0x16]);
    imageSize = *(int *)&(header[0x22]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize == 0) imageSize = width * height * 3;  // 3 : one byte for each Red, Green and Blue component
    if (dataPos == 0) dataPos = 54;                      // The BMP header is done that way

    // Create a buffer
    data = new unsigned char[imageSize];

    // Read the actual data from the file into the buffer
    fread(data, 1, imageSize, file);

    // Everything is in memory now, the file can be closed
    fclose(file);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    free(data);

    return textureID;
}

static GLuint LoadDDS(const char *imgPath) {
    unsigned char header[124];

    FILE *fp;

    // Try to open the file
    fp = fopen(imgPath, "rb");
    if (fp == NULL)
        return 0;

    // Verify the type of file
    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0) {
        fclose(fp);
        printf("Invalid file type.\n");
        return 0;
    }

    // Get the surface desc
    fread(&header, 124, 1, fp);

    unsigned int height = *(unsigned int *)&(header[8]);
    unsigned int width = *(unsigned int *)&(header[12]);
    unsigned int linearSize = *(unsigned int *)&(header[16]);
    unsigned int mipMapCount = *(unsigned int *)&(header[24]);
    unsigned int fourCC = *(unsigned int *)&(header[80]);

    unsigned char *buffer;
    unsigned int bufsize;
    // How big is it going to be including all mipmaps?
    bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = (unsigned char *)malloc(bufsize * sizeof(unsigned char));
    fread(buffer, 1, bufsize, fp);
    // Close the file pointer
    fclose(fp);

    unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;
    switch (fourCC) {
        case FOURCC_DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case FOURCC_DXT3:
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        case FOURCC_DXT5:
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        default:
            free(buffer);
            return 0;
    }

    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load the mipmaps
    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
        unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
        glCompressedTexImage2D(
            GL_TEXTURE_2D,
            level,
            format,
            width,
            height,
            0,
            size,
            buffer + offset);

        offset += size;
        width /= 2;
        height /= 2;
    }

    free(buffer);

    return textureID;
}