#include <array>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "graphics.h"
#include "framework.h"
#include "geometry.h"

using namespace std;

class SHLightingApp
	:public fw::Application{
public:
	SHLightingApp(array<string, 6> cube_textures, string sh_coef_file, string objfile)
		:cube_textures_(cube_textures), sh_coef_file_(sh_coef_file), objfile_(objfile)
	{}

private:
	array<string, 6> cube_textures_;
	string sh_coef_file_;
	string objfile_;

	fw::SkyBox* skybox_;

	//objects
	glm::mat4 camera;
	glm::mat4 view, proj;
	GLuint skybox_textures_;

	void OnInit() override;
	//void OnInput(fw::InputEvent e) override;
	void OnUpdate(float dt) override;
	void OnShutdown() override;
};

void SHLightingApp::OnInit()
{
	ifstream ifs(sh_coef_file_);
	if (!ifs)
		throw runtime_error("open " + sh_coef_file_ + " failed");
	glm::vec3 coefs[16];
	for (int i = 0; i < 16; i++){
		ifs >> coefs[i].r >> coefs[i].g >> coefs[i].b;
	}
	proj = glm::perspective(60.f, float(640) / float(480), 0.1f, 1000.f);
	view = glm::lookAt(glm::vec3{ 5.f, 3.f, 3.f }, 
		glm::vec3{ 0.f, 0.f, 0.f }, 
		glm::vec3{ 0.f, 1.f, 0.f });

	skybox_ = new fw::SkyBox(cube_textures_);

	glViewport(0, 0, WindowWidth(), WindowHeight());
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

}

void SHLightingApp::OnUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	skybox_->Draw(proj*view);
}

void SHLightingApp::OnShutdown()
{
	delete skybox_;
	glDeleteTextures(1, &skybox_textures_);

}

int main(int argc, char *argv[])
{
	try{
		if (argc < 9)
			throw runtime_error("Usage: sh_lighting posx negx posy negy posz negz coefficients.txt obj");
		array<string, 6> cube_textures;
		for (int i = 0; i < 6; i++)
			cube_textures[i] = argv[i + 1];
		string sh_coef_file = argv[7];
		string objfile = argv[8];

		SHLightingApp app(cube_textures, sh_coef_file, objfile);
		app.SetWindowSize(640, 480);
		app.SetWindowTitle("Spherical Harmonics Lighting");
		app.Run();
	}
	catch (exception e){
		cerr << e.what() << endl;
	}
	return 0;
}