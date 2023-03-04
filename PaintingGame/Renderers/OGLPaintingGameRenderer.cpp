#ifdef _WIN32
#include "OGLPaintingGameRenderer.h"
#include <Win32Window.h>
#include "GameWorld.h"

using namespace NCL;
using namespace CSC8508;

OGLPaintingGameRenderer::OGLPaintingGameRenderer(Window& w) : OGLRenderer(w) {
	CreateImGuiContext();
}

OGLPaintingGameRenderer::~OGLPaintingGameRenderer() {
	DeleteImGuiContext();
}

void OGLPaintingGameRenderer::RenderFrame() {
	if (boundScreen) {
		if (boundScreen->GetScreenType() == ScreenType::GameScreen) {
			BuildObjectList();
			SortObjectList();
			RenderGameScreen();
		}
		else {
			RenderBasicScreen();
		}
	}
}

void OGLPaintingGameRenderer::CreateImGuiContext() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//Init Win32
	ImGui_ImplWin32_Init(((NCL::Win32Code::Win32Window*)Window::GetWindow())->GetHandle());
	//Init OpenGL Imgui Implementation
	ImGui_ImplOpenGL3_Init();
	// Setup style
	ImGui::StyleColorsClassic();
}

void OGLPaintingGameRenderer::DeleteImGuiContext() {
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
void OGLPaintingGameRenderer::RenderBasicScreen() {
	RenderObject* r = boundScreen->GetSceneNode()->GetRenderObject();
	BindShader(r->GetShader());
	BindMesh(r->GetMesh());
	BindTextureToShader(r->GetDefaultTexture(), "mainTex", 0);
	DrawBoundMesh();
	boundScreen->RenderMenu();
}
void OGLPaintingGameRenderer::RenderGameScreen() {
	//send camera things and light things to shader
	RenderObject* r = boundScreen->GetSceneNode()->GetRenderObject();
	BindShader(r->GetShader());
	BindMesh(r->GetMesh());
	int layercount = r->GetMesh()->GetSubMeshCount();
	for (int index = 0; index <= layercount; ++index) {
		std::vector<std::pair<std::string, TextureBase*>> submeshtextures = r->GetTextures(index);
		int texunit = 0;
		for (const auto& texturepairs : submeshtextures) {
			BindTextureToShader(texturepairs.second, texturepairs.first, texunit);
			texunit++;
		}
		DrawBoundMesh(index);
	}
	boundScreen->RenderMenu();
}

void OGLPaintingGameRenderer::BuildObjectList() {
	activeObjects.clear();

	boundScreen->GetSceneNode()->GetWorld()->OperateOnContents(
		[&](GameObject* o) {
			if (o->IsActive()) {
				const RenderObject* g = o->GetRenderObject();
				if (g) {
					activeObjects.emplace_back(g);
				}
			}
		}
	);
}

void OGLPaintingGameRenderer::SortObjectList() {

}

#endif