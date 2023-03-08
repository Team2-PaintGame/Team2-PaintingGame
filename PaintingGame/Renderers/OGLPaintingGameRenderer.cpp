#ifdef _WIN32
#include "OGLPaintingGameRenderer.h"
#include <Win32Window.h>
#include "GameWorld.h"
#include <OGLTexture.h>
#include "OGLShader.h"
#include "Camera.h"

using namespace NCL;
using namespace CSC8508;

OGLPaintingGameRenderer::OGLPaintingGameRenderer(Window& w) : OGLRenderer(w) {
	CreateImGuiContext();
	//Set up the light properties
	lightColour = Vector4(0.8f, 0.8f, 0.5f, 1.0f);
	lightRadius = 1000.0f;
	lightPosition = Vector3(0.0f, 20.0f, 0.0f);
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
void OGLPaintingGameRenderer::RenderBasicScreen() { //change this to render static obj
	RenderObject* r = boundScreen->GetSceneNode()->GetRenderObject();
	BindShader(r->GetShader());
	BindMesh(r->GetMesh());
	BindTextureToShader(r->GetDefaultTexture(), "mainTex", 0);
	DrawBoundMesh();
	boundScreen->RenderMenu();
}
void OGLPaintingGameRenderer::RenderGameScreen() { //change this to RenderScreen
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	//send camera things and light things to shader
	boundScreen->GetSceneNode()->OperateOnCameras(
		[&](Camera* cam) {
			glViewport(cam->GetViewportSize().x * windowWidth, cam->GetViewportSize().y * windowWidth, windowWidth * cam->GetViewportDivider(), windowHeight);
			Matrix4 viewMatrix = cam->BuildViewMatrix();
			Matrix4 projMatrix = cam->BuildProjectionMatrix();
			Vector3 camPos = cam->GetPosition();
			ShaderVariablesLocations locations;
			OGLShader* activeShader = nullptr;
			for (const auto& i : activeObjects) {
				OGLShader* shader = (OGLShader*)(i)->GetShader();
				BindShader(shader);
				if (activeShader != shader) {
					locations.projLocation = glGetUniformLocation(shader->GetProgramID(), "projMatrix");
					locations.viewLocation = glGetUniformLocation(shader->GetProgramID(), "viewMatrix");
					locations.modelLocation = glGetUniformLocation(shader->GetProgramID(), "modelMatrix");
					locations.shadowLocation = glGetUniformLocation(shader->GetProgramID(), "shadowMatrix");
					locations.colourLocation = glGetUniformLocation(shader->GetProgramID(), "objectColour");
					locations.hasVColLocation = glGetUniformLocation(shader->GetProgramID(), "hasVertexColours");
					locations.hasTexLocation = glGetUniformLocation(shader->GetProgramID(), "hasTexture");
					locations.useFogLocation = glGetUniformLocation(shader->GetProgramID(), "useFog");
					locations.fogColourLocation = glGetUniformLocation(shader->GetProgramID(), "fogColour");
					locations.skyboxTexLocation = glGetUniformLocation(shader->GetProgramID(), "skyboxTex");
					locations.lightPosLocation = glGetUniformLocation(shader->GetProgramID(), "lightPos");
					locations.lightColourLocation = glGetUniformLocation(shader->GetProgramID(), "lightColour");
					locations.lightRadiusLocation = glGetUniformLocation(shader->GetProgramID(), "lightRadius");
					locations.cameraLocation = glGetUniformLocation(shader->GetProgramID(), "cameraPos");
					locations.jointsLocation = glGetUniformLocation(shader->GetProgramID(), "joints");
					locations.shadowTexLocation = glGetUniformLocation(shader->GetProgramID(), "shadowTex");


					glUniform3fv(locations.cameraLocation, 1, camPos.array);
					glUniformMatrix4fv(locations.projLocation, 1, false, (float*)&projMatrix);
					glUniformMatrix4fv(locations.viewLocation, 1, false, (float*)&viewMatrix);
					glUniform3fv(locations.lightPosLocation, 1, (float*)&lightPosition);
					glUniform4fv(locations.lightColourLocation, 1, (float*)&lightColour);
					glUniform1f(locations.lightRadiusLocation, lightRadius);
					//glUniform1i(locations.shadowTexLocation, 1);

					////binding skybox texture to shader:
					//glUniform1i(locations.skyboxTexLocation, 0);
					//glActiveTexture(GL_TEXTURE0);
					//glBindTexture(GL_TEXTURE_2D, skybox->GetFinalTexID());
					//glGenerateMipmap(GL_TEXTURE_2D);
					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

					activeShader = shader;
				}

				Matrix4 modelMatrix = (*i).GetTransform()->GetMatrix();
				glUniformMatrix4fv(locations.modelLocation, 1, false, (float*)&modelMatrix);

				/*Matrix4 fullShadowMat = modelMatrix;
				glUniformMatrix4fv(locations.shadowLocation, 1, false, (float*)&fullShadowMat);*/

				Vector4 colour = i->GetColour();
				glUniform4fv(locations.colourLocation, 1, colour.array);

				glUniform1i(locations.hasVColLocation, !(*i).GetMesh()->GetColourData().empty());

				if (i->isSingleTextured()) {
					RenderWithDefaultTexture(locations, i);
				}
				else {
					RenderWithMultipleTexture(locations, i);
				}
				if (i->IsRigged()) {
					vector<Matrix4> frameMatrices;
					i->GetFrameMatrices(frameMatrices);
					glUniformMatrix4fv(locations.jointsLocation, frameMatrices.size(), false, (float*)frameMatrices.data());
				}
				RenderPaintSplat(shader);
			}
		}
	);
	boundScreen->RenderMenu();
}

void OGLPaintingGameRenderer::RenderPaintSplat(OGLShader* shader) {
	GameWorld* world = boundScreen->GetSceneNode()->GetWorld();
	if (world) {
		world->OperateOnPaintedPositions(
			[&](int index, Vector3& pos) {
				std::string i = std::to_string(index);
				glUniform3fv(glGetUniformLocation(shader->GetProgramID(), ("paintedPos[" + i + "]").c_str()), 1, pos.array);
			}
		);
		int splatVectorSize = glGetUniformLocation(shader->GetProgramID(), "numOfSplats"); //not used anywhere in the shader
		glUniform1i(splatVectorSize, world->GetNumPaintedPositions());
	}
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

void OGLPaintingGameRenderer::RenderWithDefaultTexture(const ShaderVariablesLocations& locations, const RenderObject* r) {
	glUniform1i(locations.hasTexLocation, (OGLTexture*)r->GetDefaultTexture() ? 1 : 0);
	BindMesh(r->GetMesh());
	BindTextureToShader(r->GetDefaultTexture(), "mainTex", 0);
	int layercount = r->GetMesh()->GetSubMeshCount();
	for (int index = 0; index < layercount; ++index) {
		DrawBoundMesh(index);
	}
}

void OGLPaintingGameRenderer::RenderWithMultipleTexture(const ShaderVariablesLocations& locations, const RenderObject* r) {
	BindMesh(r->GetMesh());
	int layercount = r->GetMesh()->GetSubMeshCount();
	for (int index = 0; index < layercount; ++index) {
		glUniform1i(locations.hasTexLocation, r->GetTextures(index).size() ? 1 : 0);
		//for the current submesh, get the vector of textures and send them to shader
		std::vector<std::pair<std::string, TextureBase*>> submeshtextures = r->GetTextures(index);
		int texunit = 0;
		for (const auto& texturepairs : submeshtextures) {
			BindTextureToShader(texturepairs.second, texturepairs.first, texunit);
			texunit++;
		}
		DrawBoundMesh(index);
	}
}

#endif