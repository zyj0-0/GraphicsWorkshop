#pragma once
#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <string>
#include <memory>

struct GLFWwindow;
namespace fw{
	enum INPUT_ACTION{KEY_DOWN, KEY_UP, MOUSE_MOVE};
	struct InputEvent{
		INPUT_ACTION action;
		int key;
		float xpos, ypos;
		float dx, dy;
	};

	class Application{	// single instance
	public:
		Application() = default;
		void Run();
		void SetWindowSize(int width, int height)
		{
			window_width_ = width;
			window_height_ = height;
		}
		void SetWindowTitle(std::string title)
		{
			title_ = title;
		}
	protected:
		virtual void OnInit(){}
		virtual void OnInput(InputEvent e){}
		virtual void OnUpdate(float dt){}
		virtual void OnShutdown(){}

	private:
		Application(const Application&) = delete;
		void operator=(const Application&) = delete;

		int window_width_ = 640, window_height_ = 480;
		std::string title_ = "GraphicsWorkshop";

		struct DestroyglfwWin{
			void operator()(GLFWwindow* ptr);
		};

		GLFWwindow* window_;
		static Application* instance_;

		float pre_xpos_, pre_ypos_;

		void Init();
		void Shutdown();
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	};
}// namespace fw

#endif