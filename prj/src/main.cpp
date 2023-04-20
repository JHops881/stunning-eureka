// KThisIsAConstant
// this_is_a_variable
// ThisIsAFunction
// ThisIsAClass
// ThisIsAnEnum
// this_is_a_member_variable_
// kThisIsAnEnumMember




// C headers

// C++ headers

// std headers
#include <iostream>

// library headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// project headers
#include "shader.h"
#include "stb_image.h"



// resized viewport when called
void FrameBufferCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

// contains logic for keypress states/what to do if a keys are pressed
void ProcessInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}


int main() {

  // initialize GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  // create the window with error handling
  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
  	std::cout << "ERROR : FAILED TO CREATE GLFW WINDOW!\n";
  	glfwTerminate();
  	return -1;
  }
  glfwMakeContextCurrent(window); // assign OpenGl context to this thread/window
  
  // initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
  	std::cout << "ERROR : FAILED TO INITIALIZE GLAD!\n";
  	return -1;
  }
  
  // setup viewport
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, FrameBufferCallback); // setup resize  callback
  
  // OpenGL stuff
  //------------------------------------------------------------------------------

  // make a shader
  Shader my_shader(".\\res\\shaders\\vertex_shader.vert", ".\\res\\shaders\\fragment_shader.frag");


  // vertex data
  float vertex_data[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
  };

  unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
  };

  unsigned int vbo, vao, ebo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);


  // pick texture unit
  glActiveTexture(GL_TEXTURE0);
  // make texture
  unsigned int texture_0;
  glGenTextures(1, &texture_0);
  glBindTexture(GL_TEXTURE_2D, texture_0);
  // configure wrapping and filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and generate texture
  {
    int width, height, nr_channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(".\\res\\textures\\container.jpg", &width, &height, &nr_channels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
      std::cout << "Failed to load texture 0." << std::endl;
    }
    stbi_image_free(data);
  }

  // pick texture unit
  glActiveTexture(GL_TEXTURE1);
  // make texture
  unsigned int texture_1;
  glGenTextures(1, &texture_1);
  glBindTexture(GL_TEXTURE_2D, texture_1);
  // configure wrapping and filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and generate texture
  {
    int width, height, nr_channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(".\\res\\textures\\awesomeface.png", &width, &height, &nr_channels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
      std::cout << "Failed to load texture 1." << std::endl;
    }
    stbi_image_free(data);
  }


  my_shader.use();// select shader program
  my_shader.SetInt("texture_0", 0);
  my_shader.SetInt("texture_1", 1);

  
  // game loop
  while (!glfwWindowShouldClose(window)) {
    
    // input
    ProcessInput(window);

    // render
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // select color
    glClear(GL_COLOR_BUFFER_BIT); // color window with selected color

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_1);

    glBindVertexArray(vao); // select VAO
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // draw call

    // check and call events and swap buffers
  	glfwSwapBuffers(window);
  	glfwPollEvents();
  }
  
  
  glfwTerminate(); // free resources when application closes
  return 0;
}