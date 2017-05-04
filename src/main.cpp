//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include "myShader.h"
//para textura
#include <SOIL.h>
//para vector
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace glm;
using namespace std;
const GLint WIDTH = 800, HEIGHT = 800;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

GLfloat mixValor;
GLfloat rad=0;

int main() {
	
	//initGLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);

	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//create a window
	GLFWwindow* window;
	window = glfwCreateWindow(WIDTH, HEIGHT, "Primera ventana", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	//set GLEW and inicializate
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		cout << "Error al inicializar glew" << endl;
		glfwTerminate();
		return NULL;
	}

	//set function when callback
	glfwSetKeyCallback(window, key_callback);

	//set windows and viewport
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);

	//fondo	
	
	//cargamos los shader
	Shader myShader("./src/textureVertex.vertexshader", "./src/textureFragment.fragmentshader");

	GLfloat VertexBufferObject[] = {		
		-0.5f,  0.5f, 0.0f,   0.0, 0.0, 0.0f,   0.0f, 1.0f,  // Top Left 
		0.5f,  0.5f, 0.0f,   0.0, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right	
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0,   0.0f, 0.0f, // Bottom Left
		0.5f, -0.5f, 0.0f,   0.0f, 0.0, 0.0f,   1.0f, 0.0f, // Bottom Right
	};

	GLuint FragmentBufferObject[] = {
		0,1,2,
		1,2,3
	};

	GLfloat texCoords[] = {
		0.0f, 0.0f,  // Lower-left corner  
		1.0f, 0.0f,  // Lower-right corner
		0.5f, 1.0f   // Top-center corner
	};

	// Crear los VBO, VAO y EBO	//Declarar el VBO y el EBO	
	GLuint EBO, VBO, VAO;
	//declarar vector
	mat4 matriz;

	matriz = translate(matriz, vec3(0.5f, 0.5f, 0.0f));
	matriz = scale(matriz, vec3(0.5, -0.5, 0.0));
	
	
	//reservar memoria para el VAO, VBO y EBO
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	
	//Establecer el objeto
	glBindVertexArray(VAO);{
		//Enlazar el buffer con openGL
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferObject), VertexBufferObject, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(FragmentBufferObject), FragmentBufferObject, GL_STATIC_DRAW);

		//Establecer las propiedades de los vertices
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		//liberar el buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
	} 
	//liberar el buffer de vertices
	glBindVertexArray(0);

	//crear textura1
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	/////////////////////////////////////////////////////////////////TEXTTURA 1////////////////////////////////////////////////////////////////
	//set texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//carga image
	int width, height;
	unsigned char* image1 = SOIL_load_image("./src/texture.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);

	//liberar la image
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);
	/////////////////////////////////////////////////////////////////TEXTTURA 2////////////////////////////////////////////////////////////////
	//Establecer las propiedades de los vertices
	GLuint texture2;
	glGenTextures(1,&texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	//set texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//carga image
	int height2, width2;
	unsigned char* image2 = SOIL_load_image("./src/minecraft.jpg", &width2, &height2, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);

	//liberar la image
	SOIL_free_image_data(image2);

	//liberar el puntero
	glBindTexture(GL_TEXTURE_2D, 0);

	//bucle de dibujado
	while (!glfwWindowShouldClose(window))
	{

		//Camara
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-10.f, 10.f, -10.f, 10.f, -1.0f, 10.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Establecer el color de fondo
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1.0, 1.0, 1.0, 1.0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,texture1);
		glUniform1i(glGetUniformLocation(myShader.Program,"ourTexture1"),0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(myShader.Program, "ourTexture2"),1);

		//establecer el shader
		myShader.USE();

		matriz = rotate(matriz, rad, glm::vec3(0.0f, 0.0f, 1.0f));

		GLfloat variableShader = glGetUniformLocation(myShader.Program,"Valor");
		glUniform1f(variableShader, mixValor);
		//matrix
		GLfloat transformValor = glGetUniformLocation(myShader.Program, "transform");
		glUniformMatrix4fv(transformValor, 1, GL_FALSE, value_ptr(matriz));
		
		//pintar el VAO
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
		glBindVertexArray(0);

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	//liberar memoria texturas
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);
	// liberar la memoria de los VAO, EBO y VBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_UP && mixValor + 0.02 <= 1)
		mixValor += 0.02;
	if (key == GLFW_KEY_DOWN && mixValor - 0.02 >= 0)
		mixValor -= 0.02;
	if (key == GLFW_KEY_LEFT && rad + 0.02 <= 1&& action == GLFW_PRESS)
		rad -= 0.05;
	else if (key == GLFW_KEY_RIGHT && rad - 0.02 >= -1 && action == GLFW_PRESS)
		rad += 0.05;
	else rad = 0;

}
