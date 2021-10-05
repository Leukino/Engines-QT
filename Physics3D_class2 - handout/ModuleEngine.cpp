#include "Globals.h"
#include "Application.h"
#include "ModuleEngine.h"
#include "Primitive.h"

#include "SDL/include/SDL_opengl.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"



ModuleEngine::ModuleEngine(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEngine::~ModuleEngine()
{}

bool ModuleEngine::Init()
{
	bool ret = true;

	return ret;
}

// Load assets
bool ModuleEngine::Start()
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


update_status ModuleEngine::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	// Any application code here
	ImGui::Text("Hello, world! %d", App->input->GetMouseZMotion());

	// Render dear imgui into screen
	
	return ret;
}

// Update: draw background
update_status ModuleEngine::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	ImGuiWindowFlags window = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGuiViewport* viewport = ImGui::GetWindowViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Docking", &dockingWindow, window);
	ImGui::PopStyleVar(3);

	ImGuiIO& io = ImGui::GetIO();
	ImGuiID dockspace_id = ImGui::GetID("Dockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//g_pSwapChain->Present(1, 0);

	return UPDATE_CONTINUE;
}

// Load assets
bool ModuleEngine::CleanUp()
{
	LOG("Unloading Intro scene");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyWindow(App->window->window);
	SDL_Quit();
	return true;
}

void ModuleEngine::RenderImgui() {
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return;
}
