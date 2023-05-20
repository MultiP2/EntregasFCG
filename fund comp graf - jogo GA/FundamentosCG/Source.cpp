#include <iostream>
#include <string>
#include <assert.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "FrameTimer.h"
#include "Game.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
int setupGeometry();
//dimensões da janela
const GLuint WIDTH = 1000, HEIGHT = 800;
int main()
{
	//inicializa GLFW
	glfwInit();
	//cria a janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	//carrega todos os ponteiros
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "OpenGL version supported " << version << std::endl;
	
	Shader shader("../Shaders\\shader_texture_vs.txt", "../Shaders\\shader_texture_fs.txt");
	//ativa shader
	glUseProgram(shader.ID);
	//cria matriz de projeção com GLM
	glm::mat4 projection = glm::mat4(1);
	projection = glm::ortho(0.0f, float(WIDTH), 0.0f, float(HEIGHT), -1.0f, 1.0f);
	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, false, glm::value_ptr(projection));
	//habilita profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	//habilita transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FrameTimer ft{};
	Game game{window, &shader};

	//game loop
	while (!glfwWindowShouldClose(window))
	{
		//checa se houveram inputs e chama as funções de callback apropriadas
		glfwPollEvents();

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		//limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		game.update(window, ft.Mark());
		game.draw();

		//troca os buffers da tela
		glfwSwapBuffers(window);
	}
	//termina a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}
//função de callback de teclado, chamada sempre que uma tecla é pressionada ou solta
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
int setupGeometry()
{
	//coordenadas x, y e z prestes a ser enviadas pro(s) VBO(s)
	GLfloat vertices[] = {
		-0.4, 0.4, 0.0,
		 0.4, 0.4, 0.0,
		0.0, -0.4, 0.0,
	};

	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//desconecta o VAO
	glBindVertexArray(0);

	return VAO;
}

//Pedro Tavares Rodrigues