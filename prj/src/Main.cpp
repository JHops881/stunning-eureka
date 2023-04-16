#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


void frameBufferCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
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
	glfwSetFramebufferSizeCallback(window, frameBufferCallback); // setup resize callback
	

	// game loop
	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	
	glfwTerminate(); // free resources when application closes
	return 0;
}