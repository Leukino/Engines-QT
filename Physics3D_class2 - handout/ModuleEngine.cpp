#include "Globals.h"
#include "Application.h"
#include "ModuleEngine.h"
#include "Primitive.h"

#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleImport.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"

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
	Hierarchy();
	Inspector();
	About();

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//g_pSwapChain->Present(1, 0);

	if (closeEngine)
	{
		return UPDATE_STOP;
	}

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
				closeEngine = true;
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

			if (ImGui::MenuItem("About"))
			{
				showAbout = !showAbout;
			}
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

		if (ImGui::CollapsingHeader("Game engine"))
		{

			ImGui::Text("FPS Cap:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.47f, 1.0f), "%d", App->framerateCap);

			ImGui::Text("Min");
			ImGui::SameLine();
			ImGui::SliderInt("Max", &App->framerateCap, 1, 240);

			sprintf_s(title, 25, "Framerate %.1f", App->fpsVec[App->fpsVec.size() - 1]);
			ImGui::PlotHistogram("##framerate", &App->fpsVec[0], App->fpsVec.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			sprintf_s(title, 25, "Milliseconds %0.1f", App->msVec[App->msVec.size() - 1]);
			ImGui::PlotHistogram("##milliseconds", &App->msVec[0], App->msVec.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));


		}

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

			ImGui::SliderFloat("Brightness", &brightness, 0, 1, "%.3f");
			SDL_SetWindowBrightness(App->window->window, brightness);
		}
		ImGui::Spacing();

		if (ImGui::CollapsingHeader("Hardware"))
		{
			ImGui::Text("SDL Version:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.47f, 1.0f), "%d.%d.%d", sdlVersion.major, sdlVersion.minor, sdlVersion.patch);

			ImGui::Separator();

			ImGui::Text("CPUs: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.47f, 1.0f), "%d", SDL_GetCPUCount());

			ImGui::Text("System RAM: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.47f, 1.0f), " %d Gb", SDL_GetSystemRAM());
			ImGui::Separator();

			ImGui::Text("GPU: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.47f, 1.0f), "%s", glGetString(GL_RENDERER));

			ImGui::Text("Brand: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.47f, 1.0f), "%s", glGetString(GL_VENDOR));

			ImGui::Text("Version: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.47f, 1.0f), "%s", glGetString(GL_VERSION));

			ImGui::Text("VRAM Budget:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.47f, 1.0f), "%i Mb", GetBudget());

			ImGui::Text("VRAM Usage:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.47f, 1.0f), "%i Mb", GetUsage());

			ImGui::Text("VRAM Available:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.47f, 1.0f), "%i Mb", GetAvailable());

			ImGui::Text("VRAM Reserved:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.47f, 1.0f), "%i Mb", GetReserved());
			ImGui::Separator();

			ImGui::Text("GPU Features: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.47f, 1.0f), "%s", GetSystemCaps());

		}
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

	if (&console_log != NULL)
	{
		console_log.push_back(log);
	}
}

void ModuleEngine::Hierarchy()
{
	if (showHierarchy)
	{
		ImGui::Begin("Hierarchy", &showConsole);
		if (ImGui::CollapsingHeader("Game Objects:"))
		{
			ImGui::Text("Yes");
			ImGui::Separator();
		}

		ImGui::End();
	}
}

void ModuleEngine::Inspector()
{
	if (showInspector)
	{
		ImGui::Begin("Inspector", &showInspector);
		if (ImGui::CollapsingHeader("Inspector"))
		{
			ImGui::Text("Yes");
			ImGui::Separator();
		}
		ImGui::End();
	}
}

const char* ModuleEngine::GetSystemCaps()
{
	Caps.clear();
	// IF the processor has certain register it will be added to the string
	if (SDL_Has3DNow())
	{
		Caps.append("3D Now, ");
	}

	if (SDL_HasAVX())
	{
		Caps.append("AVX, ");
	}

	if (SDL_HasAVX2())
	{
		Caps.append("AVX2, ");
	}

	if (SDL_HasAltiVec())
	{
		Caps.append("AltiVec, ");
	}

	if (SDL_HasMMX())
	{
		Caps.append("MMX, ");
	}

	if (SDL_HasRDTSC())
	{
		Caps.append("RDTSC, ");
	}

	if (SDL_HasSSE())
	{
		Caps.append("SSE, ");
	}

	if (SDL_HasSSE2())
	{
		Caps.append("SSE2, ");
	}

	if (SDL_HasSSE3())
	{
		Caps.append("SSE3, ");
	}

	if (SDL_HasSSE41())
	{
		Caps.append("SSE41, ");
	}

	if (SDL_HasSSE41())
	{
		Caps.append("SSE42");
	}

	return Caps.data();
}

int ModuleEngine::GetBudget()
{
	int budget;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &budget);
	return budget / 1024.0f;
}

int ModuleEngine::GetUsage()
{
	int usage;
	glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &usage);
	return usage / 1024.0f;
}

int ModuleEngine::GetAvailable()
{
	int available;
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &available);
	return available / 1024.0f;
}

int ModuleEngine::GetReserved()
{
	int reserved;
	glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &reserved);
	return reserved / 1024.0f;
}

void ModuleEngine::About()
{
	if (showAbout)
	{
		ImGui::Begin("About", &showAbout);
		ImGui::Text("Engines QT");
		ImGui::Text("Engine developed for academic purpouses.");
		ImGui::Text("Made by Oscar Tarres & Miquel Quintana");
		sprintf(label, "Github Repository (Link)");
		if (ImGui::Selectable(label, true))
		{
			ShellExecuteA(NULL, "open", "https://github.com/Leukino/Engines-QT", NULL, NULL, SW_SHOWMAXIMIZED);
		}
		ImGui::Separator();
		ImGui::Text("Libraries used:");
		ImGui::BulletText("SDL");
		ImGui::BulletText("Glew");
		ImGui::BulletText("OpenGL");
		ImGui::BulletText("ImGui");
		ImGui::BulletText("MathGeoLib");
		ImGui::BulletText("Assimp");
		ImGui::BulletText("STB_Image");
		ImGui::Separator();
		ImGui::Text("GNU License:");
		sprintf(label, "Click here to see the full License");
		if (ImGui::Selectable(label, true))
		{
			ShellExecuteA(NULL, "open", "https://github.com/Leukino/Engines-QT/blob/main/LICENSE", NULL, NULL, SW_SHOWMAXIMIZED);
		}
		ImGui::End();
	}
}