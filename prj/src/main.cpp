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

  // shaders
  const char* kVertexShaderSource {
    "#version 460 core\n"
    "layout(location = 0) in vec3 aPos;\n"

    "void main() {\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0"
  };

  unsigned int vertex_shader; // create shader object
  vertex_shader = glCreateShader(GL_VERTEX_SHADER); // giving an id to vertex_shader and OpenGl creates an empty vertex shader on its side
  glShaderSource(vertex_shader, 1, &kVertexShaderSource, NULL); // attach source
  glCompileShader(vertex_shader); // compile it
  // error handling
  {
    int success;
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << "\n";
    }
  }

  const char* kFragmentShaderSource {
    "#version 460 core\n"
    "out vec4 FragColor;\n"

    "uniform vec4 ourColor;\n"

    "void main() {\n"
    "FragColor = ourColor;\n"
    "}\n"
  };

  // reference above shader
  unsigned int fragment_shader;
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &kFragmentShaderSource, NULL);
  glCompileShader(fragment_shader);
  // error handling
  {
    int success;
    char info_log[512];
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << "\n";
    }
  }

  unsigned int shader_program;
  shader_program = glCreateProgram();

  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  // error handling
  {
    int success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader_program, 512, NULL, info_log);
      std::cout << "ERROR::PROGRAM::COMPILATION_FAILED" << info_log << "\n";
    }
  }

    // vertex data
  const float kVertexPositions[] = {
    0.5f,  0.5f, 0.0f, // top right
    0.5f, -0.5f, 0.0f, // bottom right
   -0.5f, -0.5f, 0.0f, // bottom left
   -0.5f,  0.5f, 0.0f  // top left
  };
  const unsigned int kIndecies[] = {
    0, 1, 3,
    1, 2, 3
  };

  // vao init
  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // vbo init
  unsigned int vbo; // create VBO on our side
  glGenBuffers(1, &vbo); // create the VBO on OpenGL's side, and link it to ours
  glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind the VBO to GL_ARRAY_BUFFER target
  glBufferData(GL_ARRAY_BUFFER, sizeof(kVertexPositions), kVertexPositions, GL_STATIC_DRAW); // populate the VBO

  // ebo init
  unsigned int ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kIndecies), kIndecies, GL_STATIC_DRAW);

  // interperet vertex data into vao
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  



  
  
  // game loop
  while (!glfwWindowShouldClose(window)) {
    
    // input
    ProcessInput(window);

    // render
    glClearColor(1.0f, 0.5f, 0.2f, 1.0f); // select color
    glClear(GL_COLOR_BUFFER_BIT); // color window with selected color

    glUseProgram(shader_program); // select shader program

    float time_value = glfwGetTime();
    float green_value = sin(time_value) / 2.0f + 0.5f;
    int vertex_color_location = glGetUniformLocation(shader_program, "ourColor");
    glUniform4f(vertex_color_location, 0.0f, green_value, 0.0f, 1.0f);

    glBindVertexArray(vao); // select VAO

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // draw call

    // check and call events and swap buffers
  	glfwSwapBuffers(window);
  	glfwPollEvents();
  }
  
  
  glfwTerminate(); // free resources when application closes
  return 0;
}