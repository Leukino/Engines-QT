#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

#include "SDL/include/SDL_opengl.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"



ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

bool ModuleSceneIntro::Init()
{
	bool ret = true;

	//IMGUI_CHECKVERSION();
	//SDL_GetVersion(&sdlVersion);
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO();
	//(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//
	//ImGui::StyleColorsClassic();
	//ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	//
	//ImGui_ImplOpenGL3_Init(NULL);

	//brightness = SDL_GetWindowBrightness(App->window->window);
	
	


	return ret;
}

// Load assets
update_status ModuleSceneIntro::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	// Any application code here
	ImGui::Text("Hello, world!");

	// Render dear imgui into screen
	
	return ret;
}

bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init(NULL);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyWindow(App->window->window);
	SDL_Quit();
	return true;
}

void ModuleSceneIntro::RenderImgui() {
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//g_pSwapChain->Present(1, 0);

	return UPDATE_CONTINUE;
}

