#include "GameTechRenderer.h"
#include "GameObject.h"
#include "RenderObject.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "Debug.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>

using namespace NCL;
using namespace Rendering;
using namespace CSC8508;

#define SHADOWSIZE 4096

Matrix4 biasMatrix = Matrix4::Translation(Vector3(0.5f, 0.5f, 0.5f)) * Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f));

GameTechRenderer::GameTechRenderer(GameWorld& world, reactphysics3d::PhysicsWorld* physicsWorld) : OGLRenderer(*Window::GetWindow()), gameWorld(world), settings(physicsWorld) 
{
	skybox = new OGLSkybox();

	glEnable(GL_DEPTH_TEST);

	debugShader  = new OGLShader("debug.vert", "debug.frag");
	shadowShader = new OGLShader("shadow.vert", "shadow.frag");

	quadModel = OGLMesh::GenerateHUDQuad();
	hudShader = new OGLShader("hud.vert", "hud.frag");

	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			     SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(1, 1, 1, 1);

	//Set up the light properties
	lightColour = Vector4(0.8f, 0.8f, 0.5f, 1.0f);
	lightRadius = 1000.0f;
	lightPosition = Vector3(0.0f, 20.0f, 0.0f);

	glGenVertexArrays(1, &lineVAO);
	glGenVertexArrays(1, &textVAO);

	glGenBuffers(1, &lineVertVBO);
	glGenBuffers(1, &textVertVBO);
	glGenBuffers(1, &textColourVBO);
	glGenBuffers(1, &textTexVBO);

	SetDebugStringBufferSizes(10000);
	SetDebugLineBufferSizes(1000);
}

GameTechRenderer::~GameTechRenderer()	{
	delete skybox;
	delete defaultShader;
	delete debugShader;
	delete shadowShader;
	delete hudShader;


	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);

	
	for (const auto& hudTexture : hudTextures) {
		delete hudTexture.texture;
	}

	delete quadModel;
}

void GameTechRenderer::RenderFrame() {

	if (renderMode == RenderMode::MainMenu) {
		RenderMainMenu();
		return;
	}
	if (renderMode == RenderMode::SingleViewport) {
		RenderInSingleViewport();
		return;
	}
	if (renderMode == RenderMode::SplitScreen) {
		RenderFirstFrame();
		RenderSecondFrame();
		return;
	}
}

void NCL::CSC8508::GameTechRenderer::RenderInSingleViewport()
{
	glEnable(GL_CULL_FACE);
	glClearColor(1, 1, 1, 1);
	BuildObjectList();
	SortObjectList();
	RenderShadowMap();
	RenderDebugInformation(isDebugInfo);
	glViewport(0, 0, windowWidth, windowHeight);
	RenderSkybox(*gameWorld.GetMainCamera());
	RenderCamera(*gameWorld.GetMainCamera());
	RenderHUD();
	glDisable(GL_CULL_FACE); //Todo - text indices are going the wrong way...
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	NewRenderLines(*gameWorld.GetMainCamera());
	NewRenderText();
	RenderGUI();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GameTechRenderer::RenderMainMenu()
{
	glEnable(GL_CULL_FACE);
	glClearColor(1, 1, 1, 1);
	glViewport(0, 0, windowWidth, windowHeight);
	RenderSkybox(*gameWorld.GetMainCamera());
	RenderGUI(true);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void NCL::CSC8508::GameTechRenderer::RenderFirstFrame()
{
	glEnable(GL_CULL_FACE);
	glClearColor(1, 1, 1, 1);
	BuildObjectList();
	SortObjectList();
	RenderShadowMap();
	RenderDebugInformation(isDebugInfo);
	glViewport(0, 0, windowWidth / 2, windowHeight);
	RenderSkybox(*gameWorld.GetMainCamera());

	glViewport(windowWidth / 2, 0, windowWidth / 2, windowHeight);
	RenderSkybox(*gameWorld.GetSecondCamera());
	glViewport(0, 0, windowWidth / 2, windowHeight);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	RenderCamera(*gameWorld.GetMainCamera());
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	RenderHUD();
	glDisable(GL_CULL_FACE); //Todo - text indices are going the wrong way...
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	NewRenderLines(*gameWorld.GetMainCamera());
	NewRenderText();
	
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void NCL::CSC8508::GameTechRenderer::RenderSecondFrame()
{
	glEnable(GL_CULL_FACE);
	glClearColor(1, 1, 1, 1);
	BuildObjectList();
	SortObjectList();
	RenderShadowMap();
	RenderDebugInformation(isDebugInfo);
	glViewport(windowWidth / 2, 0, windowWidth / 2, windowHeight);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	RenderCamera(*gameWorld.GetSecondCamera());
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	RenderHUD();
	glDisable(GL_CULL_FACE); //Todo - text indices are going the wrong way...
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	NewRenderLines(*gameWorld.GetSecondCamera());
	NewRenderText();

	RenderGUI();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GameTechRenderer::Update(float dt) {
	skybox->Update(dt);
}

void GameTechRenderer::BuildObjectList() {
	activeObjects.clear();

	gameWorld.OperateOnContents(
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

void GameTechRenderer::SortObjectList() {

}

void GameTechRenderer::RenderShadowMap() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glCullFace(GL_FRONT);

	BindShader(shadowShader);
	int mvpLocation = glGetUniformLocation(shadowShader->GetProgramID(), "mvpMatrix");

	Matrix4 shadowViewMatrix = Matrix4::BuildViewMatrix(lightPosition, Vector3(0, 0, 0), Vector3(0,1,0));
	Matrix4 shadowProjMatrix = Matrix4::Perspective(100.0f, 500.0f, 1, 45.0f);

	Matrix4 mvMatrix = shadowProjMatrix * shadowViewMatrix;

	shadowMatrix = biasMatrix * mvMatrix; //we'll use this one later on

	for (const auto&i : activeObjects) {
		unsigned int numInstances = i->GetInstanceCount();
		if (!i->GetIsInstanced()) {
			Matrix4 modelMatrix = (*i).GetTransform()->GetMatrix();
			Matrix4 mvpMatrix = mvMatrix * modelMatrix;
			glUniformMatrix4fv(mvpLocation, 1, false, (float*)&mvpMatrix);
			BindMesh((*i).GetMesh());
			int layerCount = (*i).GetMesh()->GetSubMeshCount();
			for (int i = 0; i < layerCount; ++i) {
				DrawBoundMesh(i, numInstances);
			}
		}
	}

	glViewport(0, 0, windowWidth, windowHeight);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCullFace(GL_BACK);
}

void GameTechRenderer::RenderSky(Camera& cam) {
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	//float screenAspect = (float)windowWidth / (float)windowHeight;
	Matrix4 viewMatrix = cam.BuildViewMatrix();
	Matrix4 projMatrix = cam.BuildProjectionMatrix();
	Matrix4 transformationMatrix = skybox->GetTransformationMatrix();


	OGLShader* skyboxShader = skybox->GetShader();
	BindShader(skyboxShader);

	int projLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "projMatrix");
	int viewLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "viewMatrix");
	int transformationLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "transformationMatrix");
	int useFogLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "useFog");;
	int fogColourLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "fogColour");;
	int texLocationDay = glGetUniformLocation(skyboxShader->GetProgramID(), "cubeTexDay");
	int texLocationNight = glGetUniformLocation(skyboxShader->GetProgramID(), "cubeTexNight");
	int dayNightRatio = glGetUniformLocation(skyboxShader->GetProgramID(), "dayNightRatio");


	glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
	glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);
	glUniformMatrix4fv(transformationLocation, 1, false, (float*)&transformationMatrix);
	glUniform1i(useFogLocation, useFog);
	glUniform3fv(fogColourLocation, 1, (float*)&fogColour);
	glUniform1f(dayNightRatio, skybox->GetDayNightRatio());

	glUniform1i(texLocationDay, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->GetDayTexID());

	glUniform1i(texLocationNight, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->GetNightTexID());

	BindMesh(skybox->GetMesh());
	DrawBoundMesh();

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void GameTechRenderer::RenderSkybox(Camera& cam) {
	glBindFramebuffer(GL_FRAMEBUFFER, skybox->GetFBO());
	//glClear(GL_COLOR_BUFFER_BIT);
	RenderSky(cam);
	// ----------------------------------------------------------------------------------------------------
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT);
	RenderSky(cam);
}

void GameTechRenderer::RenderHUD() {
	if (!hudTextures.empty()) {
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		BindShader(hudShader);
		for (const auto& hudTexture : hudTextures) {
			BindTextureToShader(hudTexture.texture, "guiTexture", 0);
			Matrix4 modelMatrix = Matrix4::Translation(hudTexture.position) * Matrix4::Scale(hudTexture.scale);
			glUniformMatrix4fv(glGetUniformLocation(hudShader->GetProgramID(), "modelMatrix"), 1, false, (float*)&modelMatrix);
			BindMesh(quadModel);
			DrawBoundMesh();
		}
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
}

void GameTechRenderer::RenderDebugInformation(bool isDebugInfo) {
	if (isDebugInfo == false) { return; }
	if (settings.GetIsDebugRenderingModeEnabled()) {
		//render triangles
		int numTri = settings.debugRendererSettings.debugRenderer->getNbTriangles();
		if (numTri) {
			const reactphysics3d::DebugRenderer::DebugTriangle* tri = settings.debugRendererSettings.debugRenderer->getTrianglesArray();
			for (int i = 0; i < numTri; i++) {
				Debug::DrawTriangle(tri->point1, tri->point2, tri->point3, Debug::YELLOW);
				tri++;
			}
		}

		//render lines
		int numLines = settings.debugRendererSettings.debugRenderer->getNbLines();
		if (numLines) {
			const reactphysics3d::DebugRenderer::DebugLine* line = settings.debugRendererSettings.debugRenderer->getLinesArray();
			for (int i = 0; i < numLines; i++) {
				Debug::DrawLine(line->point1, line->point2, Debug::CYAN);
				line++;
			}
		}
	}

}

void GameTechRenderer::RenderGUI(bool showWindow) {
	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GameTechRenderer::ShowMainMenuWindow()
{
	bool isMainMenu = true; //(gameState == MainMenu);

	ImGui::Begin("Splat Main Menu", &isMainMenu);
	ImGui::Text("This is going to be the splat main menu!");

	if (ImGui::Button("Single Player"))
	{
		//SetGameState(SinglePlayer);
	}
	if (ImGui::Button("Split Screen"))
	{
		//SetGameState(SplitScreen);
	}
	if (ImGui::Button("LAN"))
	{
		//SetGameState(LAN);
	}
	if (ImGui::Button("Exit"))
	{
		//SetGameState(ExitGame);
	}
	ImGui::End();
}

void GameTechRenderer::ShowPauseMenuWindow()
{
	bool isMainMenu = true;// (gameState == MainMenu);
	ImGui::Begin("Pause Menu", &isMainMenu);
	if (ImGui::Button("Resume"))
	{
		//SetGameState(previousGameState);
	}
	if (ImGui::Button("Toggle Debug Info"))
	{
		//ToggleDebugInfo();
	}
	if (ImGui::Button("Exit to Main Menu"))
	{
		//SetGameState(MainMenu);
	}
	ImGui::End();
}


void GameTechRenderer::RenderCamera(Camera& cam) {
	/*if (settings.GetIsWireFrameModeEnabled()) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}*/
	//float screenAspect = (float)windowWidth / (float)windowHeight;
	Matrix4 viewMatrix = cam.BuildViewMatrix();
	Matrix4 projMatrix = cam.BuildProjectionMatrix();

	OGLShader* activeShader = nullptr;
	int projLocation	= 0;
	int viewLocation	= 0;
	int modelLocation	= 0;
	int colourLocation  = 0;
	int hasVColLocation = 0;
	int hasTexLocation  = 0;
	int useFogLocation = 0;
	int fogColourLocation = 0;
	int skyboxTexLocation = 0;
	int shadowLocation  = 0;
	int jointsLocation	= 0;
	int paintedLocation = 0;


	int lightPosLocation	= 0;
	int lightColourLocation = 0;
	int lightRadiusLocation = 0;

	int cameraLocation = 0;

	//TODO - PUT IN FUNCTION
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	for (const auto&i : activeObjects) {
		OGLShader* shader = (OGLShader*)(*i).GetShader();
		BindShader(shader);

		BindTextureToShader((OGLTexture*)(*i).GetDefaultTexture(), "mainTex", 0);
		/*std::vector<TextureBase*> textures = (*i).GetTextures();
		for (const auto& texture : textures) {
			BindTextureToShader(texture, "mainTex", 0);
		}*/

		if (activeShader != shader) {
			projLocation	= glGetUniformLocation(shader->GetProgramID(), "projMatrix");
			viewLocation	= glGetUniformLocation(shader->GetProgramID(), "viewMatrix");
			modelLocation	= glGetUniformLocation(shader->GetProgramID(), "modelMatrix");
			shadowLocation  = glGetUniformLocation(shader->GetProgramID(), "shadowMatrix");
			colourLocation  = glGetUniformLocation(shader->GetProgramID(), "objectColour");
			hasVColLocation = glGetUniformLocation(shader->GetProgramID(), "hasVertexColours");
			hasTexLocation  = glGetUniformLocation(shader->GetProgramID(), "hasTexture");
			useFogLocation	= glGetUniformLocation(shader->GetProgramID(), "useFog");
			fogColourLocation = glGetUniformLocation(shader->GetProgramID(), "fogColour");
			skyboxTexLocation = glGetUniformLocation(shader->GetProgramID(), "skyboxTex");

			lightPosLocation	= glGetUniformLocation(shader->GetProgramID(), "lightPos");
			lightColourLocation = glGetUniformLocation(shader->GetProgramID(), "lightColour");
			lightRadiusLocation = glGetUniformLocation(shader->GetProgramID(), "lightRadius");

			cameraLocation = glGetUniformLocation(shader->GetProgramID(), "cameraPos");
			jointsLocation = glGetUniformLocation(shader->GetProgramID(), "joints");

			Vector3 camPos = cam.GetPosition();

			for (int i = 0; i < gameWorld.painted.size(); i++) {
				Vector4 paintedPos = Vector4(gameWorld.painted[i], 0);
				char buffer[64];
				sprintf_s(buffer, "paintedPos[%i]", i);
				paintedLocation = glGetUniformLocation(shader->GetProgramID(), buffer);
				glUniform4fv(paintedLocation, 1, paintedPos.array);
			}

			int splatVectorSize = glGetUniformLocation(shader->GetProgramID(), "numOfSplats");
			glUniform1i(splatVectorSize, gameWorld.painted.size());

			glUniform3fv(cameraLocation, 1, camPos.array);

			glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
			glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

			glUniform1i(useFogLocation, useFog);
			glUniform3fv(fogColourLocation, 1, (float*)&fogColour);


			glUniform3fv(lightPosLocation	, 1, (float*)&lightPosition);
			glUniform4fv(lightColourLocation, 1, (float*)&lightColour);
			glUniform1f(lightRadiusLocation , lightRadius);

			int shadowTexLocation = glGetUniformLocation(shader->GetProgramID(), "shadowTex");

			glUniform1i(shadowTexLocation, 1);
						
			//binding skybox texture to shader:
			glUniform1i(skyboxTexLocation, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, skybox->GetFinalTexID());
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			activeShader = shader;
		}

		unsigned int numInstances = i->GetInstanceCount();
		if (i->GetIsInstanced()) {
			if (numInstances == 0) {
				continue;
			}
			std::vector<Transform*> transforms = i->GetTransforms();
			for (int i = 0; i < numInstances; i++) {
				std::string index = std::to_string(i);
				Matrix4 modelMatrix = transforms[i]->GetMatrix();
				int modelArrayLocation = glGetUniformLocation(shader->GetProgramID(), ("modelMatrices[" + index + "]").c_str());
				glUniformMatrix4fv(modelArrayLocation, 1, false, (float*)&modelMatrix);

				Matrix4 fullShadowMat = shadowMatrix * modelMatrix;
				int shadowArrayLocation = glGetUniformLocation(shader->GetProgramID(), ("shadowMatrices[" + index + "]").c_str());
				glUniformMatrix4fv(shadowArrayLocation, 1, false, (float*)&fullShadowMat);
			}
		}
		else {
			Matrix4 modelMatrix = (*i).GetTransform()->GetMatrix();
			glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);

			Matrix4 fullShadowMat = shadowMatrix * modelMatrix;
			glUniformMatrix4fv(shadowLocation, 1, false, (float*)&fullShadowMat);
		}
	

		Vector4 colour = i->GetColour();
		glUniform4fv(colourLocation, 1, colour.array);

		glUniform1i(hasVColLocation, !(*i).GetMesh()->GetColourData().empty());

		glUniform1i(hasTexLocation, (OGLTexture*)(*i).GetDefaultTexture() ? 1 : 0);

		
		BindMesh((*i).GetMesh());
		int layercount = (*i).GetMesh()->GetSubMeshCount();
		for (int index = 0; index < layercount; ++index) {

			glUniform1i(hasTexLocation, i->GetTextures(index).size() ? 1 : 0);

			//for the current submesh, get the vector of textures and send them to shader
			std::vector<std::pair<std::string, TextureBase*>> submeshtextures = i->GetTextures(index);
			int texunit = 2;
			for (const auto& texturepairs : submeshtextures) {
				BindTextureToShader(texturepairs.second, texturepairs.first, texunit);
				texunit++;
			}
			DrawBoundMesh(index, numInstances);
		}
		if (i->IsRigged()) {
			vector<Matrix4> frameMatrices;
			i->GetFrameMatrices(frameMatrices);
			glUniformMatrix4fv(jointsLocation, frameMatrices.size(), false, (float*)frameMatrices.data());
		}
	}
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

MeshGeometry* GameTechRenderer::LoadMesh(const string& name) {
	OGLMesh* mesh = new OGLMesh(name);
	mesh->SetPrimitiveType(GeometryPrimitive::Triangles);
	mesh->UploadToGPU();
	return mesh;
}

MeshGeometry* GameTechRenderer::LoadQuad() {
	return OGLMesh::GenerateQuad();
}

MeshGeometry* GameTechRenderer::LoadFlatMesh(int hVertexCount, int wVertexCount) {
	return OGLMesh::GenerateFlatMesh(hVertexCount, wVertexCount);
}

MeshGeometry* GameTechRenderer::LoadHeightMap(const std::string& filename, int heightMultiplier) {
	return OGLMesh::GenerateHeightMap(filename, heightMultiplier);
}

void GameTechRenderer::NewRenderLines(Camera& cam) {
	const std::vector<Debug::DebugLineEntry>& lines = Debug::GetDebugLines();
	if (lines.empty()) {
		return;
	}
	//float screenAspect = (float)windowWidth / (float)windowHeight;
	Matrix4 viewMatrix = cam.BuildViewMatrix();
	Matrix4 projMatrix = cam.BuildProjectionMatrix();
	
	Matrix4 viewProj  = projMatrix * viewMatrix;

	BindShader(debugShader);
	int matSlot = glGetUniformLocation(debugShader->GetProgramID(), "viewProjMatrix");
	GLuint texSlot = glGetUniformLocation(debugShader->GetProgramID(), "useTexture");
	glUniform1i(texSlot, 0);

	glUniformMatrix4fv(matSlot, 1, false, (float*)viewProj.array);

	debugLineData.clear();

	int frameLineCount = lines.size() * 2;

	SetDebugLineBufferSizes(frameLineCount);

	glBindBuffer(GL_ARRAY_BUFFER, lineVertVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, lines.size() * sizeof(Debug::DebugLineEntry), lines.data());
	

	glBindVertexArray(lineVAO);
	glDrawArrays(GL_LINES, 0, frameLineCount);
	glBindVertexArray(0);
}

void GameTechRenderer::NewRenderText() {
	const std::vector<Debug::DebugStringEntry>& strings = Debug::GetDebugStrings();
	if (strings.empty()) {
		return;
	}

	BindShader(debugShader);

	OGLTexture* t = (OGLTexture*)Debug::GetDebugFont()->GetTexture();

	if (t) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t->GetObjectID());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);	
		BindTextureToShader(t, "mainTex", 0);
	}
	Matrix4 proj = Matrix4::Orthographic(0.0, 100.0f, 100, 0, -1.0f, 1.0f);

	int matSlot = glGetUniformLocation(debugShader->GetProgramID(), "viewProjMatrix");
	glUniformMatrix4fv(matSlot, 1, false, (float*)proj.array);

	GLuint texSlot = glGetUniformLocation(debugShader->GetProgramID(), "useTexture");
	glUniform1i(texSlot, 1);

	debugTextPos.clear();
	debugTextColours.clear();
	debugTextUVs.clear();

	int frameVertCount = 0;
	for (const auto& s : strings) {
		frameVertCount += Debug::GetDebugFont()->GetVertexCountForString(s.data);
	}
	SetDebugStringBufferSizes(frameVertCount);

	for (const auto& s : strings) {
		float size = 20.0f;
		Debug::GetDebugFont()->BuildVerticesForString(s.data, s.position, s.colour, size, debugTextPos, debugTextUVs, debugTextColours);
	}


	glBindBuffer(GL_ARRAY_BUFFER, textVertVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, frameVertCount * sizeof(Vector3), debugTextPos.data());
	glBindBuffer(GL_ARRAY_BUFFER, textColourVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, frameVertCount * sizeof(Vector4), debugTextColours.data());
	glBindBuffer(GL_ARRAY_BUFFER, textTexVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, frameVertCount * sizeof(Vector2), debugTextUVs.data());

	glBindVertexArray(textVAO);
	glDrawArrays(GL_TRIANGLES, 0, frameVertCount);
	glBindVertexArray(0);
}

TextureBase* GameTechRenderer::LoadTexture(const string& name) {
	return TextureLoader::LoadAPITexture(name);
}

ShaderBase* GameTechRenderer::LoadShader(const string& vertex, const string& fragment) {
	return new OGLShader(vertex, fragment);
}

void GameTechRenderer::AddHudTextures(const string& name, const Vector2& position, const Vector2& scale) {
	hudTextures.emplace_back(TextureHUD(TextureLoader::LoadAPITexture(name), position, scale));
}

void GameTechRenderer::SetDebugStringBufferSizes(size_t newVertCount) {
	if (newVertCount > textCount) {
		textCount = newVertCount;

		glBindBuffer(GL_ARRAY_BUFFER, textVertVBO);
		glBufferData(GL_ARRAY_BUFFER, textCount * sizeof(Vector3), nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, textColourVBO);
		glBufferData(GL_ARRAY_BUFFER, textCount * sizeof(Vector4), nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, textTexVBO);
		glBufferData(GL_ARRAY_BUFFER, textCount * sizeof(Vector2), nullptr, GL_DYNAMIC_DRAW);

		debugTextPos.reserve(textCount);
		debugTextColours.reserve(textCount);
		debugTextUVs.reserve(textCount);

		glBindVertexArray(textVAO);

		glVertexAttribFormat(0, 3, GL_FLOAT, false, 0);
		glVertexAttribBinding(0, 0);
		glBindVertexBuffer(0, textVertVBO, 0, sizeof(Vector3));

		glVertexAttribFormat(1, 4, GL_FLOAT, false, 0);
		glVertexAttribBinding(1, 1);
		glBindVertexBuffer(1, textColourVBO, 0, sizeof(Vector4));

		glVertexAttribFormat(2, 2, GL_FLOAT, false, 0);
		glVertexAttribBinding(2, 2);
		glBindVertexBuffer(2, textTexVBO, 0, sizeof(Vector2));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}
}

void GameTechRenderer::SetDebugLineBufferSizes(size_t newVertCount) {
	if (newVertCount > lineCount) {
		lineCount = newVertCount;

		glBindBuffer(GL_ARRAY_BUFFER, lineVertVBO);
		glBufferData(GL_ARRAY_BUFFER, lineCount * sizeof(Debug::DebugLineEntry), nullptr, GL_DYNAMIC_DRAW);

		debugLineData.reserve(lineCount);

		glBindVertexArray(lineVAO);

		int realStride = sizeof(Debug::DebugLineEntry) / 2;

		glVertexAttribFormat(0, 3, GL_FLOAT, false, offsetof(Debug::DebugLineEntry, start));
		glVertexAttribBinding(0, 0);
		glBindVertexBuffer(0, lineVertVBO, 0, realStride);

		glVertexAttribFormat(1, 4, GL_FLOAT, false, offsetof(Debug::DebugLineEntry, colourA));
		glVertexAttribBinding(1, 0);
		glBindVertexBuffer(1, lineVertVBO, sizeof(Vector4), realStride);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}
}

void GameTechRenderer::SetRenderMode(RenderMode mode)
{
	renderMode = mode;
}

void GameTechRenderer::ToggleDebugInfo() {
	isDebugInfo = !isDebugInfo;
}

