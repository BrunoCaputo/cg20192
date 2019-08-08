#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

bool BACKGROUND_STATE = false;

// Compile shader source code from text file format
bool compileShader(const std::string & filename, GLenum type, GLuint & id) {
    // Read from text file to string
    std::ifstream file(filename, std::ifstream::in);

    if (!file.is_open())
        return false;

    std::stringstream buffer;
    std::string source;

    buffer << file.rdbuf();
    source = buffer.str();

    // Create shader
    GLuint shaderID = glCreateShader(type);

    // Setup shader source code
    const GLchar * src = source.data();
    glShaderSource(shaderID, 1, &src, nullptr);

    // Compile shader
    glCompileShader(shaderID);

    // Get compilation status
    GLint status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

    // Check compilation errors
    if (status != GL_TRUE) {
        // Get log message size of the compilation process
        GLint size = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &size);

        std::string message;
        message.resize(size);

        // Get log message of the compilation process
        glGetShaderInfoLog(shaderID, size, nullptr, (GLchar *)message.data());

        // Print log message
        std::cout << message << std::endl;

        // Delete shader
        glDeleteShader(shaderID);

        return false;
    }

    // Return shader id
    id = shaderID;

    return true;
}

// Create shader program
bool createProgram(const std::string & name, GLuint & id) {
    GLuint vertexShaderID, fragmentShaderID;

    // Load and compile vertex shader
    if (!compileShader(name + ".vert", GL_VERTEX_SHADER, vertexShaderID))
        return false;

    // Load and compile fragment shader
    if (!compileShader(name + ".frag", GL_FRAGMENT_SHADER, fragmentShaderID))
        return false;

    // Create shader program
    GLuint programID = glCreateProgram();

    // Attach compiled shaders to program
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);

    // Link attached shaders to create an executable
    glLinkProgram(programID);

    // Delete compiled shaders
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    // Get linkage status
    GLint status;
    glGetProgramiv(programID, GL_LINK_STATUS, &status);

    // Check linkage errors
    if (status != GL_TRUE) {
        // Get log message size of the linkage process
        GLint size = 0;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &size);

        std::string message;
        message.resize(size);

        // Get log message of the linkage process
        glGetProgramInfoLog(programID, size, nullptr, (GLchar *)message.data());

        // Print log message
        std::cout << message << std::endl;

        // Delete shader program
        glDeleteProgram(programID);

        return false;
    }

    // Return shader program id
    id = programID;

    return true;
}

void resize(GLFWwindow * window, int width, int heigth) {
	glViewport(0, 0, width, heigth);
}

void keyboard(GLFWwindow * window, int key, int scancode, int action, int modifiers) {
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		BACKGROUND_STATE = !BACKGROUND_STATE;
	}
}

int main(int argc, char** argv) {
	glm::vec3 u(2.0f, 0, 0);
	glm::vec3 v(0, 1.0f, 0);
	
	glm::vec3 c = glm::normalize(u);
	
	float d = glm::dot(u, v);
	
	glm::mat3 m(1, 2, 3, 4, 5, 6, 7, 8, 9);
	
	cout << glm::to_string(m) << endl;
	
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
	
	GLuint programID;
	
	if(!createProgram("../res/shaders/triangle", programID)) {
		glfwTerminate();
		
		cout << "Nao foi possivel criar o shader" << endl;
		return -1;
	}
	
	glUseProgram(programID);
	
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,	//first vertex position attribute
		1.0f, 0.0f, 0.0f,	//first vertex coloc attribute
		
		0.5f, -0.5f, 0.0f,	//second vertex position attribute
		0.0f, 1.0f, 0.0f,	//second vertex coloc attribute
		
		0.0f, 0.5f, 0.0f,	//third vertex position attribute
		0.0f, 0.0f, 1.0f	//third vertex coloc attribute
	};
	
	GLuint vao;
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	GLuint vbo;
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(
		0, 3,
		GL_FLOAT,
		false,
		6 * sizeof(GLfloat),
		(GLvoid * )(3 * sizeof(GLfloat))
	);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	while(!glfwWindowShouldClose(window)) {
		if (BACKGROUND_STATE)
			glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		else
			glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
			
		glClear(GL_COLOR_BUFFER_BIT);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1, &vao);
	
	glDeleteBuffers(1, &vbo);
	
	glfwDestroyWindow(window);
	
	glfwTerminate();
	
	return 0;
}
