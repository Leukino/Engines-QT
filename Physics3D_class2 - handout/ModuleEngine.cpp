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
	ImGui::Text("Hello, world! %d %f ", App->input->GetMouseZMotion(), dt);

	// Render dear imgui into screen
	
	return ret;
}

// Update: draw background
update_status ModuleEngine::Update(float dt)
{
	/*Plane p(0, 1, 3, 0);
	p.axis = true;
	p.Render();*/

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

	ImGui::StyleColorsDark();

	MainMenuBar();
	Config();
	Console();

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//g_pSwapChain->Present(1, 0);

	return UPDATE_CONTINUE;
}

update_status ModuleEngine::PostUpdate(float dt)
{

	Plane p(0, 1, 3, 0);
	p.axis = true;
	p.Render();

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

void ModuleEngine::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{

			if (ImGui::BeginMenu("New"))
			{
				if (ImGui::MenuItem("New File", "Ctrl + N"))
				{
				}

				ImGui::EndMenu();
			}
			ImGui::Spacing();
			if (ImGui::BeginMenu("Load File"))
			{
				if (ImGui::MenuItem("Load File", "Ctrl + A"))
				{
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();
			ImGui::Spacing();
			if (ImGui::MenuItem("Exit", "ESC"))
			{
				//App->escape = true;
			}
			ImGui::EndMenu();
		}
		ImGui::Spacing();

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}
		ImGui::Spacing();

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Settings"))
			{
				showConfig = !showConfig;
			}
			if (ImGui::MenuItem("Console")) {
				showConsole = !showConsole;
			}
			if (ImGui::MenuItem("Hierarchy"))
			{
				showHierarchy = !showHierarchy;
			}
			if (ImGui::MenuItem("Inspector"))
			{
				showInspector = !showInspector;
			}

			ImGui::EndMenu();
		}
		ImGui::Spacing();

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("GitHub"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/Leukino/Engines-QT", NULL, NULL, SW_SHOWMAXIMIZED);
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void ModuleEngine::Config()
{
	if (showConfig)
	{
		ImGui::Begin("Settings", &showConfig);

		if (ImGui::CollapsingHeader("Window Settings"))
		{
			if (ImGui::Checkbox("Fullscreen", &fullscreen))
			{
				App->window->SetFullscreen(fullscreen);
			}
			if (ImGui::Checkbox("Borderless", &borderless))
			{
				App->window->SetBorderless(borderless);
			}
			if (ImGui::Checkbox("Full Desktop", &full_desktop))
			{
				App->window->SetFullscreenBorderless(full_desktop);
			}
			ImGui::Separator();

			if (ImGui::SliderInt("Width", &window_width, 500, 1920, "%d"))
			{
				SDL_SetWindowSize(App->window->window, window_width, window_height);
			}
			if (ImGui::SliderInt("Height", &window_height, 500, 1080, "%d"))
			{
				SDL_SetWindowSize(App->window->window, window_width, window_height);
			}
		}
		ImGui::Spacing();

		ImGui::Text("VSync: ");
		ImGui::SameLine();
		ImGui::Checkbox("", &vsync);

		ImGui::Spacing();

		ImGui::Text("MinFPS");
		ImGui::SameLine();
		ImGui::SliderInt(" MaxFPS", &fps, 0, 240);
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Text("CPU cores: ");
		ImGui::SameLine();
		//ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.47f, 1.0f), "%d", App->cpuCores);

		ImGui::Text("RAM: ");
		ImGui::SameLine();
		//ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.47f, 1.0f), "%d gb", App->systemRAM);

		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Text("Graphic card: ");
		ImGui::SameLine();
		//ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.47f, 1.0f), "%s", App->rendererChar);

		ImGui::Text("Vendor: ");
		ImGui::SameLine();
		//ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.47f, 1.0f), "%s", App->vendorChar);

		ImGui::Text("Graphic card features: ");
		ImGui::SameLine();
		//ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.47f, 1.0f), "%s%s%s%s%s%s%s%s", (App->RDTSC) ? "RDTSC " : "", (App->MMX) ? "MMX " : "", (App->SSE) ? "SSE " : "", (App->SSE2) ? "SSE2 " : "", (App->SSE3) ? "SSE3 " : "", (App->SSE41) ? "SSE41 " : "", (App->SSE42) ? "SSE42 " : "", (App->AVX) ? "AVX " : "");

		char title[25];
		//sprintf_s(title, 25, "Framerate %.1f", App->fps_log[App->fps_log.size() - 1]);
		//ImGui::PlotHistogram("##framerate", &App->fps_log[0], App->fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		//sprintf_s(title, 25, "Milliseconds %.1f", App->ms_log[App->ms_log.size() - 1]);
		//ImGui::PlotHistogram("##milliseconds", &App->ms_log[0], App->ms_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		//LOG("%d", App->fps_log[0]);
		ImGui::End();
	}
}

void ModuleEngine::Console()
{
	if (showConsole)
	{
		ImGui::Begin("Console", &showConsole);

		for (int i = 0; i < console_log.size(); i++)
		{
			ImGui::Text("%s", console_log[i].c_str());
		}
		ImGui::End();
	}
}

void ModuleEngine::ConsoleLog(std::string log) {

	if (&console_log != NULL) {
		console_log.push_back(log);
	}
}