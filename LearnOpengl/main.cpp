#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "shader.h"

using namespace std;

bool POLYGON_MODE_ON = false;

// vertex shader written in glsl
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"out vec3 ourColor;\n"
	"void main()\n"
	"{\n"
	" gl_Position = vec4(aPos, 1.0);\n"
	" ourColor = aColor;\n"
	"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	//"in vec4 vertexColor;\n"
	//"uniform vec4 ourColor;\n" // uniform color, used for setting color from our program
	"in vec3 ourColor;\n"
	"void main()\n"
	"{\n"
	//"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	//"FragColor = vertexColor;\n"
	"FragColor = vec4(ourColor, 1.0);\n"
	"}\n";

// Function for handling resizing window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	bool Q_PRESSED = false;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	/*if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && !Q_PRESSED)
	{
		glPolygonMode(GL_FRONT_AND_BACK, POLYGON_MODE_ON ? GL_LINE : GL_FILL);
		POLYGON_MODE_ON = !POLYGON_MODE_ON;
		Q_PRESSED = true;
		cout << "pressed Q" << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE)
		Q_PRESSED = false;*/
	
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// changes polygon mode (if shapes are filled or just lines)
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, POLYGON_MODE_ON ? GL_FILL : GL_LINE);
		POLYGON_MODE_ON = !POLYGON_MODE_ON;
	}
}

int main() 
{
	glfwInit();
	// Setting required version of OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	/*	Core profile -> smaller subset of OpenGL features without backwards-compatible
	features we no longer need.*/
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // This only on mac
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, keyCallback);

	float vertices_trojkat[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f
	};

	float vertices[] = {
		// positions      // colors
		0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // top
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f // bottom right
	};
	unsigned int indices[] = { // note that we start from 0!
		0, 1, 2 // first triangle
	};

	// creating and binding virtual buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// creating  VERTEX SHADER
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// checking if shader compilation was successful
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}
	// creating FRAGMENT SHADER
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// checking if compiling was successful
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}
	// combining shaders into one program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// success or not? hmm...
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAMT::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}
	
	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// testing shader class
	Shader ourShader("shaders/shader.sv", "shaders/shader.sf");

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	//// Bind vertex array to object
	//glBindVertexArray(VAO);
	//// Copy verticies array in a buffer for OpenGL to use
	//glBindBuffer(GL_ARRAY_BUFFER, VAO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//// Set vertex attribute pointers
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
	//	(void*)0);
	//glEnableVertexAttribArray(0);

	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		GL_STATIC_DRAW);
	// 4. then set the vertex attributes pointers
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);



	while (!glfwWindowShouldClose(window)) 
	{
		// input
		processInput(window);

		// rendering
		// clean the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activating the shader
		//glUseProgram(shaderProgram);
		
		// update the uniform color
		/*float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);*/

		// testing our shader
		ourShader.use();
	
		// render the triangle
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);

		// check and call events, swap buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}