#pragma once
#include <GLFW/glfw3.h>

class Window
{
public: 
	struct WindowData
	{
		unsigned int width;
		unsigned int height;
		bool vSync; 
		WindowData(unsigned int width, unsigned int height) : width(width), height(height), vSync(false) {}
	};

	Window(); 
	~Window();

	void Destory(); 
private: 
	GLFWwindow* window;
	WindowData* mData; 
};