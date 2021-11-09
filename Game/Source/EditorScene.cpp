#include "EditorScene.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"

EditorScene::EditorScene(Application* App, vector<Shape3D*>* s)
{
	this->app = App;
	this->shapes = s;
}

EditorScene::~EditorScene()
{
}

bool EditorScene::Start()
{
	Plane3D* p = new Plane3D();
	p->axis = true;
	shapes->push_back(p);

	Model* m = new Model({ 0, 0, 0 }, 1);
	m->LoadModel("Assets/Models/BakerHouse.fbx", "Assets/Textures/baker_house_texture.png");
	shapes->push_back(m);

	Pyramid3D* py = new Pyramid3D({ 5, 0, 0 }, 3);
	py->axis = true;
	shapes->push_back(py);

	Model* m1 = new Model({ 10, 1, 0 }, 0.05f);
	m1->LoadModel("Assets/Models/cube.fbx", "Assets/Textures/cube_texture.png");
	shapes->push_back(m1);

	//Sphere3D* s = new Shape3D({0, 0, 0}, 1);

	return true;
}

bool EditorScene::Update()
{
	bool ret = true;

	onWindow = ImGui::IsAnyItemActive();

	if (demoWindow) ImGui::ShowDemoWindow(&demoWindow);
	ret = DrawMenuBar();
	aboutPopup = ShowAboutWindow(aboutPopup);
	outputWindow = ShowOutputWindow(outputWindow);
	configWindow = ShowConfigWindow(configWindow);
	hierarchyWindow = ShowHierarchyWindow(hierarchyWindow);

	ret = ShortCuts();

	return ret;
}

bool EditorScene::CleanUp()
{
	bool ret = true;

	return ret;
}

bool EditorScene::DrawMenuBar()
{
	bool ret = true;

	ImGui::BeginMainMenuBar();
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save File", " Ctrl + S"))
				app->Save("NNE_Project_Saving", FileContent::PROJECT);

			if (ImGui::MenuItem("Load File", "Ctrl + L"))
				app->Load("NNE_Project_Saving", FileContent::PROJECT);

			if (ImGui::MenuItem("Exit", " Esc"))
				ret = false;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Configuration"))
				configWindow = !configWindow;

			if (ImGui::MenuItem("Output Log"))
				outputWindow = !outputWindow;

			if (ImGui::MenuItem("Hierarchy"))
				hierarchyWindow = !hierarchyWindow;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Delete Model", " Supr"))
				PopShape();

			if (ImGui::MenuItem("Delete All Models", " Supr + Shift"))
				PopAllShapes();

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::BeginMenu("Primitives"))
			{
				if (ImGui::MenuItem("Cube"))
					CreatePrimitive(CUBE3D);

				if (ImGui::MenuItem("Line"))
					CreatePrimitive(LINE3D);

				if (ImGui::MenuItem("Pyramid"))
					CreatePrimitive(PYRAMID3D);

				if (ImGui::MenuItem("Cylinder"))
					CreatePrimitive(CYLINDER3D);

				if (ImGui::MenuItem("Plane"))
					CreatePrimitive(PLANE3D);

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Test Models"))
			{
				if (ImGui::MenuItem("Cube"))
				{
					Model* m = new Model({ 0, 0, 0 }, 1.0f);
					m->LoadModel("Assets/Models/cube.fbx");
					PushShape3D(m);
				}
				if (ImGui::MenuItem("Warrior"))
				{
					Model* m = new Model({ 0, 0, 0 }, 1.0f);
					m->LoadModel("Assets/Models/warrior.FBX");
					PushShape3D(m);
				}
				if (ImGui::MenuItem("Baker House"))
				{
					Model* m = new Model({ 0, 0, 0 }, 1.0f);
					m->LoadModel("Assets/Models/BakerHouse.fbx", "Assets/Textures/baker_house_texture.png");
					PushShape3D(m);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::BeginMenu("Geometry"))
			{
				if (ImGui::MenuItem("Edges"))
				{
					edges = !edges;
					app->scene->SetGeometryInfo(EDGES);
				}

				if (ImGui::MenuItem("Normals"))
				{
					normals = !normals;
					app->scene->SetGeometryInfo(NORMALS);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Gui Demo"))
				demoWindow = !demoWindow;

			if (ImGui::MenuItem("Repository"))
				LinkBrowser("https://github.com/BooStarGamer/Game-Engine-1.0v");

			if (ImGui::MenuItem("Documentation"))
				LinkBrowser("https://github.com/BooStarGamer/Game-Engine-1.0v/wiki");

			if (ImGui::MenuItem("Download latest"))
				LinkBrowser("https://github.com/BooStarGamer/Game-Engine-1.0v/releases");

			if (ImGui::MenuItem("Report Bug"))
				LinkBrowser("https://github.com/BooStarGamer/Game-Engine-1.0v/issues");

			if (ImGui::MenuItem("About"))
				aboutPopup = !aboutPopup;

			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	return ret;
}

bool EditorScene::ShowAboutWindow(bool open)
{
	if (open)
	{
		ImGui::OpenPopup("About");

		if (ImGui::BeginPopupModal("About", &open))
		{
			ImGui::TextColored({ 1,0,0,1 },"NoNameEngine v0.1\n");
			ImGui::Text("The best 3D engine for the best games.\nCreated by:\n\nMarti Buxeda: \n");
			if (ImGui::Button("Github link"))
			{
				LinkBrowser("https://github.com/BooStarGamer");
			}
			ImGui::Text("Victor Jara: \n");
			if (ImGui::Button("Github link"))
			{
				LinkBrowser("https://github.com/Kerali");
			}

			ImGui::Text("\n3rd Party Libraries used:\n");
			ImGui::BulletText("SDL 2.0.16");
			ImGui::BulletText("Glew 2.2.0");
			ImGui::BulletText("ImGui v1.85");
			ImGui::BulletText("OpenGL 2.0.6");
			ImGui::BulletText("Assimp 5.0.1");
			ImGui::BulletText("JSON 1.9.4");
			ImGui::BulletText("Parson 1.2.1");
			ImGui::BulletText("STB Image v2.27");

			ImGui::Text("\n\nMIT License\n\nCopyright (c) 2021 Marti Buxeda and Victor Jara\n\n");
			
			ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy \nof this software and associated documentation files(the 'Software'), to deal \nin the Software without restriction, including without limitation the rights \nto use, copy, modify, merge, publish, distribute, sublicense, and /or sell \ncopies of the Software, and to permit persons to whom the Software is \nfurnished to do so, subject to the following conditions :\n ");
			ImGui::Text("The above copyright notice and this permission notice shall be included in all\ncopies or substantial portions of the Software.\n\n");
			ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE.\n\n");
			
			if (ImGui::Button("Close"))
			{
				open = false;
			}

			ImGui::EndPopup();
		}
	}

	return open;
}

bool EditorScene::ShowOutputWindow(bool open)
{
	if (open)
	{
		if (ImGui::Begin("Output Log", &open))
		{
			for (int i = 0; i < GetOutputSize(); i++)
			{
				ImGui::Text(GetOutputText(i).c_str());
			}
		}
		ImGui::End();
	}

	return open;
}

bool EditorScene::ShowConfigWindow(bool open)
{
	if (open)
	{
		if (ImGui::Begin("Configuration", &open))
		{
			if (ImGui::BeginMenu("Options"))
			{
				if (ImGui::MenuItem("Save"))
					app->Save("NNE_Preferences_Saving", FileContent::CONFIG_PREFERENCES);

				if (ImGui::MenuItem("Load"))
					app->Load("NNE_Preferences_Saving", FileContent::CONFIG_PREFERENCES);

				if (ImGui::MenuItem("Default"))
					app->Load("NNE_Preferences_Default", FileContent::CONFIG_PREFERENCES);

				ImGui::EndMenu();
			}
			if (ImGui::BeginTabBar("#tabs"))
			{
				if (ImGui::BeginTabItem("General"))
				{
					if (ImGui::CollapsingHeader("Application"))
						{
							AddSpacing(0);
							ImGui::Text("PROJECT INFORMATION");
							AddSpacing(1);
							ImGui::Text("Project Name");
							ImGui::InputText("New Project", &projectName);
							AddSpacing(0);
							ImGui::Text("Developer Team");
							ImGui::InputText("Team Name", &teamName);
							AddSpacing(1);

							AddSeparator(2);

							AddSpacing(0);
							ImGui::Text("ENGINE PERFORMANCE");
							AddSpacing(1);
							ImGui::Text("Framerate");
							ImGui::SliderInt(" FPS", &app->fps, 5, 60, "FPS: %d");
							AddSpacing(0);

							char title[25];
							framerate = app->fpsLog[app->fpsLog.size() - 2];
							sprintf_s(title, 25, "Framerate %.1f", framerate);
							ImGui::Text("Framerate Graph");
							ImGui::PlotHistogram("##framerate", &app->fpsLog[0], app->fpsLog.size(), 0, title, 0.0f, 100.0f, ImVec2(307, 100));
							ImGui::SliderInt(" FBL", &app->frameBarLimit, 30, 80, "Bars: %d");

							AddSpacing(0);

							milliseconds = app->msLog[app->msLog.size() - 2];
							sprintf_s(title, 25, "Milliseconds %0.1f", milliseconds);
							ImGui::Text("Milliseconds Graph");
							ImGui::PlotHistogram("##milliseconds", &app->msLog[0], app->msLog.size(), 0, title, 0.0f, 40.0f, ImVec2(307, 100));
							ImGui::SliderInt(" MSG", &app->msBarLimit, 30, 80, "Bars: %d");

							AddSpacing(1);

							AddSeparator(2);
						}
					if (ImGui::CollapsingHeader("Window"))
					{
						Point prev = wSize;
						int prevProportion = wSizeProportion;
						float prevBright = brightLevel;

						AddSpacing(0);
						ImGui::Text("Brightness");
						ImGui::SliderFloat("B", &brightLevel, 0.0f, 1.0f, "Value: %.3f");
						if (prevBright != brightLevel) app->window->SetWinBrightness(brightLevel);

						AddSpacing(0);
						if (ImGui::Checkbox("Keep Proportion", &wKeepProportion)) wKeepProportion = wKeepProportion;

						if (!wKeepProportion)
						{
							AddSpacing(0);
							ImGui::Text("Width");
							ImGui::SliderInt("W", (int*)&wSize.x, 1, SCREEN_WIDTH, "%d");

							AddSpacing(0);
							ImGui::Text("Height");
							ImGui::SliderInt("H", (int*)&wSize.y, 1, SCREEN_HEIGHT, "%d");

							if (wSize != prev) app->window->SetWinSize((int)wSize.x, (int)wSize.y);
						}
						else
						{
							AddSpacing(0);
							ImGui::Text("Width / Height");
							ImGui::SliderInt("W", &wSizeProportion, 1, 100, "%d");

							if (wSizeProportion != prevProportion) app->window->SetWinSize(int(floor(SCREEN_WIDTH * wSizeProportion / 100)), int(floor(SCREEN_HEIGHT * wSizeProportion / 100)));
						}

						AddSpacing(1);
						if (ImGui::Checkbox(" VSync", &wVSync)) app->render->SetVSync(wVSync);

						AddSpacing(0);
						if (ImGui::Checkbox(" Full Screen    ", &wFullScreen)) app->window->SetWinFullScreen(wFullScreen);
						ImGui::SameLine();
						if (ImGui::Checkbox(" Resizable", &wResizable)) app->SaveRestartPropierties();

						AddSpacing(0);
						if (ImGui::Checkbox(" Borderless     ", &wBorderless)) app->window->SetWinBorders(wBorderless);
						ImGui::SameLine();
						if (ImGui::Checkbox(" Full Desktop", &wFullDesktop)) app->window->SetWinFullScreen(wFullDesktop);

						AddSpacing(1);
					}
					if (ImGui::CollapsingHeader("Input"))
					{
						AddSpacing(0);
						ImGui::Text("Mouse Position: %d %d", app->input->GetMouseX(), app->input->GetMouseY());
						AddSpacing(0);
						ImGui::Text("Mouse Speed: %d %d", app->input->GetMouseXMotion(), app->input->GetMouseYMotion());
						AddSpacing(0);
						ImGui::Text("Mouse Wheel: %d %d", app->input->GetMouseWheel(), app->input->GetMouseZH());
						AddSpacing(1);
					}
					if (ImGui::CollapsingHeader("Camera"))
					{
						AddSpacing(0);
						float* speed = app->camera->GetSpeed();
						ImGui::SliderFloat("Speed", speed, 0.1f, 1.0f);
						AddSpacing(0);
						float* sens = app->camera->GetSensitivity();
						ImGui::SliderFloat("Sensitivity", sens, 0.1f, 0.5f);
						AddSpacing(1);
					}
					if (ImGui::CollapsingHeader("Hardware"))
						{
							AddSpacing(0);
							SDL_version v;
							SDL_GetVersion(&v);
							ImGui::Text("SDL Version: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(YELLOW), "%d.%d.%d", v.major, v.minor, v.patch);
							AddSpacing(0);
							AddSeparator(2);
							AddSpacing(0);
							ImGui::Text("CPUs: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(YELLOW), "%d (Cache: %dkb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
							AddSpacing(0);
							ImGui::Text("System RAM: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(YELLOW), "%d", SDL_GetSystemRAM);
							AddSpacing(0);
							ImGui::Text("Caps: "); ImGui::SameLine();
							{
								if (SDL_Has3DNow()) ImGui::TextColored(ImVec4(YELLOW), "3DNow"); ImGui::SameLine();
								if (SDL_HasAltiVec()) ImGui::TextColored(ImVec4(YELLOW), "AltiVec"); ImGui::SameLine();
								if (SDL_HasAVX()) ImGui::TextColored(ImVec4(YELLOW), "AVX"); ImGui::SameLine();
								if (SDL_HasAVX2()) ImGui::TextColored(ImVec4(YELLOW), "AVX2"); ImGui::SameLine();
								if (SDL_HasMMX()) ImGui::TextColored(ImVec4(YELLOW), "MMX"); ImGui::SameLine();
								if (SDL_HasRDTSC()) ImGui::TextColored(ImVec4(YELLOW), "RDTSC");
								if (SDL_HasSSE()) ImGui::TextColored(ImVec4(YELLOW), "SSE"); ImGui::SameLine();
								if (SDL_HasSSE2()) ImGui::TextColored(ImVec4(YELLOW), "SSE2"); ImGui::SameLine();
								if (SDL_HasSSE3()) ImGui::TextColored(ImVec4(YELLOW), "SSE3"); ImGui::SameLine();
								if (SDL_HasSSE41()) ImGui::TextColored(ImVec4(YELLOW), "SSE41"); ImGui::SameLine();
								if (SDL_HasSSE42()) ImGui::TextColored(ImVec4(YELLOW), "SSE42");
							}
							AddSpacing(0);
							AddSeparator(2);
						}

					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Geometry View"))
				{
					AddSpacing(0);
					ImGui::Text("VIEW SETTINGS");
					AddSpacing(1);

					if (ImGui::Checkbox("Depth Test", &depthTest))
						app->render->ToggleGeometryView(GeometryView::DEPTH_TEST, depthTest);

					AddSpacing(0);

					if (ImGui::Checkbox("Cull Face", &cullFace))
						app->render->ToggleGeometryView(GeometryView::CULL_FACE, cullFace);

					AddSpacing(0);

					if (ImGui::Checkbox("Lighting", &lighting))
						app->render->ToggleGeometryView(GeometryView::LIGHTING, lighting);

					AddSpacing(0);

					if (ImGui::Checkbox("Color Material", &colorMaterial))
						app->render->ToggleGeometryView(GeometryView::COLOR_MATERIAL, colorMaterial);

					AddSpacing(0);

					if (ImGui::Checkbox("Texture 2D", &texture2D))
						app->render->ToggleGeometryView(GeometryView::TEXTURE_2D, texture2D);

					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}

	return open;
}

bool EditorScene::ShowHierarchyWindow(bool open)
{
	if (open)
	{
		if (ImGui::Begin("Hierarchy", &open))
		{
			AddSeparator(2);
			AddSpacing(1);
			int select = 0;
			for (int i = 1; i < shapes->size(); i++)
			{
				char buffer[24] = {};
				sprintf_s(buffer, " %s %d", shapes->at(i)->GetName(), i);
				// Multiselection
				/*if (ImGui::Selectable(buffer, &shapes->at(i)->selected))
				{
					if (app->input->GetKey(SDL_SCANCODE_LSHIFT) != KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_LSHIFT) != KEY_DOWN)
						if (shapes->at(i)->selected)
						{
							for (int a = 0; a < shapes->size(); a++) if (i != a) shapes->at(a)->selected = false;
						}
				}*/

				// Uniselection
				if (ImGui::Selectable(buffer, &shapes->at(i)->selected))
				{
					for (int a = 0; a < shapes->size(); a++) if (i != a) shapes->at(a)->selected = false;
				}
				AddSpacing(0);


				if (select == 0 && shapes->at(i)->selected) select = i;
			}

			if (select != 0)
			{
				Shape3D* s = shapes->at(select);

				AddSpacing(4);
				AddSeparator(2);
				AddSpacing(0);

				// NAME
				char buffer[24] = {};
				sprintf_s(buffer, " Name: %s %d", s->GetName(), select);
				ImGui::Text(buffer);
				AddSpacing(1);
				AddSeparator(1);
				AddSpacing(1);

				// TRANSFORM
				ImGui::Text(" Transform:");
				AddSpacing(0);

				// Position
				ImGui::BulletText("Position:");
				ImGui::Text("    X: %.2f", s->GetPosition().x);
				ImGui::Text("    Y: %.2f", s->GetPosition().y);
				ImGui::Text("    Z: %.2f", s->GetPosition().z);
				AddSpacing(1);

				// Rotation
				ImGui::BulletText("Rotation:");
				ImGui::Text("    Angle: %.2f", s->GetRotation().angle);
				ImGui::Text("    Plane: {%.2f, %.2f, %.2f}", s->GetRotation().planeX, s->GetRotation().planeY, s->GetRotation().planeZ);
				AddSpacing(1);

				// Scale
				ImGui::BulletText("Scale: %.3f", s->GetScale());
				AddSpacing(1);

				// Type
				ImGui::BulletText("Type: %s", s->WriteShapeType(s->GetShapeType()).c_str());
				AddSpacing(0);
				switch (s->GetShapeType())
				{
				case PYRAMID3D: 
				{
					Pyramid3D* py = (Pyramid3D*)s;
					ImGui::Text("    Height: %.2f", py->GetHeight());
					break;
				}
				case CYLINDER3D:
				{
					Cylinder3D* cy = (Cylinder3D*)s;
					ImGui::Text("    Height: %.2f", cy->GetHeight());
					ImGui::Text("    Radius: %.2f", cy->GetRadius());
					ImGui::Text("    Segments: %d", cy->GetSegments());
					break;
				}
				case PLANE3D:
				{
					Plane3D* p = (Plane3D*)s;
					ImGui::Text("    Normal: {%d, %d, %d}", (int)p->GetNormal().x, (int)p->GetNormal().y, (int)p->GetNormal().z);
					break;
				}
				}
				AddSpacing(1);
				AddSeparator(1);
				AddSpacing(1);

				// MESH
				ImGui::Text(" Mesh:");
				AddSpacing(0);

				if (s->GetShapeType() != MODEL3D)
				{
					ImGui::BulletText("Num: %d", 1);
					AddSpacing(1);
				}
				else
				{
					Model* m = (Model*)s;
					// Num Of Mesh
					ImGui::BulletText("Num: %d", m->meshes.size());
					AddSpacing(1);

					// Model Path
					ImGui::BulletText("Model path: %s", m->filePath.c_str());
					AddSpacing(1);
				}

				// Edges
				ImGui::BulletText("Edges: "); ImGui::SameLine(); s->edges ? ImGui::Text("true") : ImGui::Text("false");
				AddSpacing(1);

				// Normals
				ImGui::BulletText("Normals: "); ImGui::SameLine(); s->normals ? ImGui::Text("true") : ImGui::Text("false");
				AddSpacing(1);
				AddSeparator(1);
				AddSpacing(1);

				// TEXTURE
				ImGui::Text(" Texture:");
				AddSpacing(0);

				if (s->GetShapeType() != MODEL3D)
				{
					ImGui::BulletText("No Texture");
					AddSpacing(1);
				}
				else
				{
					Model* m = (Model*)s;

					// Size
					ImGui::BulletText("Size: ");
					ImGui::Text("    Width: %.2f", m->GetSize().x);
					ImGui::Text("    Height: %.2f", m->GetSize().y);
					AddSpacing(1);

					// Texture Path
					ImGui::BulletText("Texture path: %s", m->texturePath.c_str());
					AddSpacing(1);

					// Show texture
					ImGui::Checkbox("Texture", &m->showTexture);
					AddSpacing(1);
				}
			}
		}
		ImGui::End();
	}

	return open;
}

bool EditorScene::ShortCuts()
{
	bool ret = true;
	bool shift = (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT);
	bool ctrl = (app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_RCTRL) == KEY_REPEAT);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) ret = false;

	if (!shift && !ctrl)
	{
		if (app->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN) PopShape();
	}
	else if (shift && !ctrl)
	{
		if (app->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN) PopAllShapes();
	}
	else if (!shift && ctrl)
	{
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) app->Save("NNE_Project_Saving", FileContent::PROJECT);
		if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) app->Load("NNE_Project_Saving", FileContent::PROJECT);
	}

	return ret;
}

void EditorScene::CreatePrimitive(ShapeType sT)
{
	switch (sT)
	{
	case CUBE3D:
	{
		shapes->push_back(new Cube3D()); break;
	}
	case LINE3D:
	{
		shapes->push_back(new Line3D()); break;
	}
	case PYRAMID3D:
	{
		shapes->push_back(new Pyramid3D()); break;
	}
	case CYLINDER3D:
	{
		shapes->push_back(new Cylinder3D()); break;
	}
	case PLANE3D:
	{
		shapes->push_back(new Plane3D()); break;
	}
	case SPHERE3D:
	{
		/*shapes->push_back(new Sphere3D({}));*/ break;
	}
	}
}

void EditorScene::PushShape3D(Shape3D* s3D)
{
	shapes->push_back(s3D);
}

void EditorScene::PopShape()
{
	int select = -1;
	for (int i = 1; i < shapes->size(); i++)
	{
		if (shapes->at(i)->selected)
		{
			select = i; 
			break;
		}
	}

	if (select != -1)
	{
		shapes->at(select)->~Shape3D();
		shapes->erase(shapes->begin() + select);
	}
	else
	{
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_INFORMATION,
			"Shapes Error",
			"\nAny shape selected",
			app->window->mainWindow
		);
	}
}

void EditorScene::PopAllShapes()
{
	if (shapes->size() > 1) shapes->erase(shapes->begin() + 1, shapes->end());
	else
	{
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_INFORMATION,
			"Shapes Error",
			"\nCan not delete a Model\nNo shape left in the scene",
			app->window->mainWindow
		);
	}
}
