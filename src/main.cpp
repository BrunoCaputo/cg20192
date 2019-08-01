#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

using namespace std;

bool BACKGROUND_STATE = false;

void resize(GLFWwindow * window, int width, int heigth) {
	glViewport(0, 0, width, heigth);
}

void keyboard(GLFWwindow * window, int key, int scancode, int action, int modifiers) {
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		BACKGROUND_STATE = !BACKGROUND_STATE;
	}
}

int main(int argc, char** argv) {
	if(!glfwInit()) {
		cout << "Não foi possível inicializar o glfw" << endl;
		return -1;		
	}

	// Setup OpenGL context	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Create window
	GLFWwindow * window = glfwCreateWindow(800, 600, "Teste", nullptr, nullptr);
	
	if(window == nullptr) {
		glfwTerminate();
		
		cout << "Não foi possível abrir a janela" << endl;
		return -1;
	}
	
	glfwSetFramebufferSizeCallback(window, resize);
	glfwSetKeyCallback(window, keyboard);
	
	// Setup window context
	glfwMakeContextCurrent(window);
	
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		
		cout << "Não foi possível carregar o OpenGL" << endl;
		return -1;
	}
	
	while(!glfwWindowShouldClose(window)) {
		if (BACKGROUND_STATE)
			glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		else
			glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
			
		glClear(GL_COLOR_BUFFER_BIT);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	
	return 0;
}
