#include <stdio.h>

#define GLFW_INCLUDE_ES3
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>

int wWidth = 1280, wHeight = 960;

GLFWwindow* window;
Shader* basicShader;

glm::mat4 projectionMatrix;
glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec2 dimension = glm::vec2(200.f, 200.f); // width and height of the cube
float speed = 10.0f;
bool mouseMode = false; 

#ifdef __EMSCRIPTEN__


EM_JS(int, getCanvasWidth, (), {
	return Module.canvas.width;
	});


EM_JS(int, getCanvasHeight, (), {
	return Module.canvas.height;
	});
#endif // __EMSCRIPTEN__


void updateProjectionMatrix() {
	glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(wWidth), 0.0f, static_cast<float>(wHeight), -1.0f, 1.0f);
	projectionMatrix = proj;
}


void resizeCanvas()
{
	int w = getCanvasWidth(), h = getCanvasHeight();
	if (w != wWidth || h != wHeight)
	{
		wWidth = w;
		wHeight = h;
		updateProjectionMatrix(); 
	}
}




void loop()
{
	glfwPollEvents();
	
	resizeCanvas(); 


	basicShader->setUniformMat4f("u_Projection", projectionMatrix);
	basicShader->setUniformMat4f("u_TranslationMat", glm::translate(glm::mat4(1), position));


	glClearColor(0.0, 0.0, 0.0, 1.0);

	glViewport(0, 0, wWidth, wHeight);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glfwSwapBuffers(window);
}

void onMouseMove(GLFWwindow* window, double offsetX, double offsetY)
{
	if (mouseMode)
		position = glm::vec3(offsetX - dimension.x / 2, wHeight - offsetY - dimension.y / 2, 1.0);
};

void onMouseButtonPressed(GLFWwindow* window, int button, int action, int mods)
{
	if(button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
		mouseMode = false; 
}


void onKeyDown(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_M)
		mouseMode = !mouseMode; // Toggle mouse mode

	if (!mouseMode && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
			switch (key)
			{
			case GLFW_KEY_W:
			case GLFW_KEY_UP:
				position += glm::vec3(0.0f, speed, 0.0f);
				break;

			case GLFW_KEY_A:
			case GLFW_KEY_LEFT:
				position += glm::vec3(-speed, 0.0f, 0.0f);
				break;

			case GLFW_KEY_S:
			case GLFW_KEY_DOWN:
				position += glm::vec3(0.0f, -speed, 0.0f);
				break;
			case GLFW_KEY_D:
			case GLFW_KEY_RIGHT:
				position += glm::vec3(speed, 0.0f, 0.0f);
				break;

			};
	}

}



static void force_exit()
{
#ifdef __EMSCRIPTEN__
	emscripten_force_exit(EXIT_FAILURE);
#else
	exit(EXIT_FAILURE);
#endif
}

extern "C" int main(int argc, const char** argv)
{


	printf("....Radhey Shyam....\n");
	system("cd");

	glfwInit();
	wWidth = getCanvasHeight();
	wHeight = getCanvasHeight();


	window = glfwCreateWindow(wWidth, wHeight, "Radha", nullptr, nullptr);
	if (!window)
	{
		force_exit();
	}


	glfwMakeContextCurrent(window);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);


	// Callbacks
	glfwSetKeyCallback(window, onKeyDown);
	glfwSetCursorPosCallback(window, onMouseMove);
	glfwSetMouseButtonCallback(window, onMouseButtonPressed); 

	resizeCanvas();

	basicShader = Shader::LoadFromFile("emscripten_project/src/resources/basic.glsl");

	GLuint VAO, IBO, VBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	float x = position.x, y = position.y, w = dimension.x, h = dimension.y;

	float vertices[] = {
		x, y, 0.0f, // Bottom-left
		x + w, y, 0.0f,  // Bottom-right
		x + w, y + h, 0.0f,   // Top-right
		x, y + h, 0.0f   // Top-left
	};

	int indices[] = { 0, 1, 2, 2, 3, 0 };

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	position = glm::vec3((wWidth / 2) - w * 0.5f, (wHeight / 2) - h * 0.5f, 1.0f);

	basicShader->use();
	glBindVertexArray(VAO);
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(loop, 0, 1);
#else 
	while (glfwShowWindow(window))
	{
		loop();
	}
#endif

	delete basicShader;
	return EXIT_SUCCESS;
}