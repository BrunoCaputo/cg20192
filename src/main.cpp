#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

bool BACKGROUND_STATE = false;

glm::mat4 MODEL(1.0f);
glm::mat4 PROJECTION(1.0f);

// Compile shader source code from text file format
bool compileShader(const string & filename, GLenum type, GLuint & id) {
    // Read from text file to string
    ifstream file(filename, ifstream::in);

    if (!file.is_open())
        return false;

    stringstream buffer;
    string source;

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

       	string message;
        message.resize(size);

        // Get log message of the compilation process
        glGetShaderInfoLog(shaderID, size, nullptr, (GLchar *)message.data());

        // Print log message
        cout << message << endl;

        // Delete shader
        glDeleteShader(shaderID);

        return false;
    }

    // Return shader id
    id = shaderID;

    return true;
}

// Create shader program
bool createProgram(const string & name, GLuint & id) {
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

        string message;
        message.resize(size);

        // Get log message of the linkage process
        glGetProgramInfoLog(programID, size, nullptr, (GLchar *)message.data());

        // Print log message
        cout << message << endl;

        // Delete shader program
        glDeleteProgram(programID);

        return false;
    }

    // Return shader program id
    id = programID;

    return true;
}

void resize(GLFWwindow * window, int width, int height) {
	glViewport(0, 0, width, height);
	PROJECTION = glm::perspective(45.0f, width / (float)height, 0.001f, 1000.0f);
}

void keyboard(GLFWwindow * window, int key, int scancode, int action, int modifiers) {
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		BACKGROUND_STATE = !BACKGROUND_STATE;
	}
	
	if(key == GLFW_KEY_RIGHT && (action == GLFW_PRESS | action == GLFW_REPEAT)) {
		MODEL = glm::rotate(MODEL, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
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
	glfwWindowHint(GLFW_SAMPLES, 16);
	
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
	
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,	//first vertex position attribute
		1.0f, 0.0f, 0.0f,	//first vertex coloc attribute
		
		0.5f, -0.5f, 0.0f,	//second vertex position attribute
		0.0f, 1.0f, 0.0f,	//second vertex coloc attribute
		
		0.0f, 0.5f, 0.0f,	//third vertex position attribute
		0.0f, 0.0f, 1.0f	//third vertex coloc attribute
	};
	
	GLuint programID;
	
	if(!createProgram("../res/shaders/triangle", programID)) {
		glfwTerminate();
		
		cout << "Nao foi possivel criar o shader" << endl;
		return -1;
	}
	
	glUseProgram(programID);
		
	GLuint vao;
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	GLuint vbo;
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        false,
        6 * sizeof(GLfloat),
        (GLvoid *)nullptr
	);
	
	glVertexAttribPointer(
		1, 3,
		GL_FLOAT,
		false,
		6 * sizeof(GLfloat),
		(GLvoid * )(3 * sizeof(GLfloat))
	);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glEnable(GL_DEPTH_TEST);
	
	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)	
	);
	
	while(!glfwWindowShouldClose(window)) {
		if (BACKGROUND_STATE)
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        else
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		GLint viewID = glGetUniformLocation(programID, "view");
		glUniformMatrix4fv(viewID, 1, false, glm::value_ptr(view));
		
		GLint projectionID = glGetUniformLocation(programID, "projection");
		glUniformMatrix4fv(projectionID, 1, false, glm::value_ptr(PROJECTION));
		
		GLint modelID = glGetUniformLocation(programID, "model");
		glUniformMatrix4fv(modelID, 1, false, glm::value_ptr(MODEL));
		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glDeleteProgram(programID);
	
	glDeleteVertexArrays(1, &vao);
	
	glDeleteBuffers(1, &vbo);
	
	glfwDestroyWindow(window);
	
	glfwTerminate();
	
	return 0;
}
