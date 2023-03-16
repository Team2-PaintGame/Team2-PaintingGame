#ifdef _WIN32
#include "OGLPaintingGameRenderer.h"
#include <Win32Window.h>
#include "GameWorld.h"
#include <OGLTexture.h>
#include "OGLShader.h"
#include "Camera.h"
#include "Debug.h"

using namespace NCL;
using namespace CSC8508;

OGLPaintingGameRenderer::OGLPaintingGameRenderer(Window& w) : OGLRenderer(w) {
	CreateImGuiContext();
	//Set up the light properties
	lightColour = Vector4(0.8f, 0.8f, 0.5f, 1.0f);
	lightRadius = 1000.0f;
	lightPosition = Vector3(0.0f, 20.0f, 0.0f);

	SetDebugStringBufferSizes(10000);
	SetDebugLineBufferSizes(1000);

	scoreQuad = new OGLMesh();
	scoreQuad->SetVertexPositions({ Vector3(-1, 1, 1), Vector3(-1, -1, 1), Vector3(1, -1, 1), Vector3(1, 1, 1) });
	scoreQuad->SetVertexTextureCoords({ Vector2(0.0f,1.0f), Vector2(0.0f,0.0f), Vector2(1.0f,0.0f), Vector2(1.0f,1.0f) });
	scoreQuad->SetVertexIndices({ 0,1,2,2,3,0 });
	scoreQuad->UploadToGPU();

	team1Percentage = 0;
	team2Percentage = 0;
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

	NewRenderLines();
	NewRenderText();
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
	glViewport(0, 0, windowWidth, windowHeight);
	RenderObject* r = boundScreen->GetSceneNode()->GetRenderObject();
	BindShader(r->GetShader());
	BindMesh(r->GetMesh());
	BindTextureToShader(r->GetDefaultTexture(), "mainTex", 0);
	DrawBoundMesh();
	boundScreen->RenderMenu();
}
void OGLPaintingGameRenderer::RenderGameScreen() { //change this to RenderScreen
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//send camera things and light things to shader
	boundScreen->GetSceneNode()->OperateOnCameras(
		[&](Camera* cam) {
			cam->SetPerspectiveCameraParameters(Window::GetWindow()->GetScreenAspect() * cam->GetAspectMultiplier());
			glViewport(cam->GetVpStartPos().x * windowWidth, cam->GetVpStartPos().y * windowWidth, windowWidth * cam->GetVpSize().x, windowHeight * cam->GetVpSize().y);
			Matrix4 viewMatrix = cam->BuildViewMatrix();
			Matrix4 projMatrix = cam->BuildProjectionMatrix();
			Vector3 camPos = cam->GetPosition();
			ShaderVariablesLocations locations;
			OGLShader* activeShader = nullptr;
			for (const auto& i : activeObjects) {
				if (i->GetIsOccluded()) {
					glEnable(GL_CULL_FACE);
					glCullFace(GL_BACK);
					glEnable(GL_DEPTH_TEST);
				}
				else {
					glDisable(GL_CULL_FACE);
					glDisable(GL_DEPTH_TEST);
				}
				OGLShader* shader = (OGLShader*)(i)->GetShader();
				BindShader(shader);
				if (activeShader != shader) {
					locations.projLocation = glGetUniformLocation(shader->GetProgramID(), "projMatrix");
					locations.viewLocation = glGetUniformLocation(shader->GetProgramID(), "viewMatrix");
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

				SendModelMatrices(shader, i);

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

	/*Render ScoreBar*/
	DrawScoreBar();

	RenderDebugInformation();
	boundScreen->RenderMenu();
}

void OGLPaintingGameRenderer::DrawScoreBar() {
	for (const auto& i : activeObjects) {
		OGLShader* shader = (OGLShader*)(i)->GetShader();
	
	BindShader(shader);

	if (true/*gameWorld->DoesMapNeedChecking()*/) {
		RetrieveAtomicValues();

		glUniform1f(glGetUniformLocation(shader->GetProgramID(), "team1PercentageOwned"), team1Percentage);
		glUniform1f(glGetUniformLocation(shader->GetProgramID(), "team2PercentageOwned"), team2Percentage);

		glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "defaultGray"), 1, defaultColour.array);
		glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "team1Colour"), 1, teamColours[0].array);
		glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "team2Colour"), 1, teamColours[1].array);

		gameWorld->MapNeedsChecking(false);
	}

	Matrix4 identityMatrix = Matrix4();

	int projLocation = glGetUniformLocation(shader->GetProgramID(), "projMatrix");
	int viewLocation = glGetUniformLocation(shader->GetProgramID(), "viewMatrix");
	int modelLocation = glGetUniformLocation(shader->GetProgramID(), "modelMatrix");

	glUniformMatrix4fv(modelLocation, 1, false, (float*)&identityMatrix);
	glUniformMatrix4fv(viewLocation, 1, false, (float*)&identityMatrix);
	glUniformMatrix4fv(projLocation, 1, false, (float*)&identityMatrix);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	Matrix4 scoreBarModelMatrix = Matrix4::Translation(Vector3(0, 0.85f, 0)) * Matrix4::Scale(Vector3(0.4f, 0.035f, 1));
	glUniformMatrix4fv(modelLocation, 1, false, (float*)&scoreBarModelMatrix);

	BindMesh(scoreQuad);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	}
}
void OGLPaintingGameRenderer::GenerateAtomicBuffer() {
	glGenBuffers(1, &atomicsBuffer[0]);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicsBuffer[0]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomicsBuffer[0]);
	glBufferStorage(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint) * ATOMIC_COUNT, NULL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT | GL_MAP_COHERENT_BIT);

	glGenBuffers(1, &atomicsBuffer[1]);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicsBuffer[1]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 1, atomicsBuffer[1]);
	glBufferStorage(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint) * ATOMIC_COUNT, NULL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT | GL_MAP_COHERENT_BIT);

	glGenBuffers(1, &atomicsBuffer[2]);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicsBuffer[2]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 2, atomicsBuffer[2]);
	glBufferStorage(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint) * ATOMIC_COUNT, NULL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT | GL_MAP_COHERENT_BIT);

	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, 0);

	currentAtomicCPU = 2;
	curretAtomicReset = 1;
	currentAtomicGPU = 0;
}


void OGLPaintingGameRenderer::RetrieveAtomicValues() {
	GLuint pixelCount[ATOMIC_COUNT];
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicsBuffer[currentAtomicCPU]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, currentAtomicCPU, atomicsBuffer[currentAtomicCPU]);

	glGetBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint) * ATOMIC_COUNT, pixelCount);

	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
	totalPixelCount = pixelCount[0];

	for (GLuint i = 1; i < ATOMIC_COUNT; i++)
	{
		teamPixelCount[i - 1] = pixelCount[i];
	}

	CalculatePercentages(totalPixelCount, teamPixelCount[0], teamPixelCount[1], teamPixelCount[2], teamPixelCount[3]);

	ResetAtomicBuffer();
}

void OGLPaintingGameRenderer::ResetAtomicBuffer() {
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicsBuffer[currentAtomicCPU]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, currentAtomicCPU, atomicsBuffer[currentAtomicCPU]);
	GLuint a[ATOMIC_COUNT];
	for (GLuint i = 0; i < ATOMIC_COUNT; i++)
	{
		a[i] = 0;
	}
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint) * ATOMIC_COUNT, a);

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
		int splatVectorSize = glGetUniformLocation(shader->GetProgramID(), "numOfSplats");
		glUniform1i(splatVectorSize, world->GetNumPaintedPositions());
	}
}

void OGLPaintingGameRenderer::CalculatePercentages(const int& totalPixels, const int& team1Pixels, const int& team2Pixels, const int& team3Pixels, const int& team4Pixels) {
	float totalPaintedPixels = (float)team1Pixels + (float)team2Pixels + (float)team3Pixels + (float)team4Pixels;
	if (totalPaintedPixels != 0) {
		team1Percentage = (float)team1Pixels / totalPaintedPixels;
		team2Percentage = (float)team2Pixels / totalPaintedPixels;

	}
	else {
		team1Percentage = 0;
		team2Percentage = 0;
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

void OGLPaintingGameRenderer::SendModelMatrices(OGLShader* shader, const RenderObject* r) {
	unsigned int numInstances = r->GetInstanceCount();
	if (r->GetIsInstanced()) {
		if (numInstances == 0) {
			return;
		}
		std::vector<Transform*> transforms = r->GetTransforms();
		for (int i = 0; i < numInstances; i++) {
			std::string index = std::to_string(i);
			Matrix4 modelMatrix = transforms[i]->GetMatrix();
			int modelArrayLocation = glGetUniformLocation(shader->GetProgramID(), ("modelMatrices[" + index + "]").c_str());
			glUniformMatrix4fv(modelArrayLocation, 1, false, (float*)&modelMatrix);
		}
	}
	else {
		Matrix4 modelMatrix = r->GetTransform()->GetMatrix();
		int modelLocation = glGetUniformLocation(shader->GetProgramID(), "modelMatrix");
		glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);
	}
}

void OGLPaintingGameRenderer::RenderWithDefaultTexture(const ShaderVariablesLocations& locations, const RenderObject* r) {
	unsigned int numInstances = r->GetInstanceCount();
	glUniform1i(locations.hasTexLocation, r->GetDefaultTexture() ? 1 : 0);
	BindMesh(r->GetMesh());
	BindTextureToShader(r->GetDefaultTexture(), "mainTex", 0);
	int layercount = r->GetMesh()->GetSubMeshCount();
	int index = 0;
	do {
		DrawBoundMesh(index++, numInstances);
	} while (index < layercount);
}

void OGLPaintingGameRenderer::RenderWithMultipleTexture(const ShaderVariablesLocations& locations, const RenderObject* r) {
	unsigned int numInstances = r->GetInstanceCount();
	BindMesh(r->GetMesh());
	int layercount = r->GetMesh()->GetSubMeshCount();
	int index = 0;
	do {
		glUniform1i(locations.hasTexLocation, r->GetTextures(index).size() ? 1 : 0);
		//for the current submesh, get the vector of textures and send them to shader
		std::vector<std::pair<std::string, TextureBase*>> submeshtextures = r->GetTextures(index);
		int texunit = 0;
		for (const auto& texturepairs : submeshtextures) {
			BindTextureToShader(texturepairs.second, texturepairs.first, texunit);
			texunit++;
		}
		DrawBoundMesh(index++, numInstances);
	} while (index < layercount);
}

//Debug methods

void OGLPaintingGameRenderer::RenderDebugInformation() const {
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	if (boundScreen->GetSceneNode()->GetPhysicsWorld() && boundScreen->GetSceneNode()->GetPhysicsWorld()->getIsDebugRenderingEnabled() )  {
		reactphysics3d::DebugRenderer* debugRenderer = &boundScreen->GetSceneNode()->GetPhysicsWorld()->getDebugRenderer();

		//render triangles
		int numTri = debugRenderer->getNbTriangles();
		if (numTri) {
			const reactphysics3d::DebugRenderer::DebugTriangle* tri = debugRenderer->getTrianglesArray();
			for (int i = 0; i < numTri; i++) {
				Debug::DrawTriangle(tri->point1, tri->point2, tri->point3, Debug::YELLOW);
				tri++;
			}
		}

		//render lines
		int numLines = debugRenderer->getNbLines();
		if (numLines) {
			const reactphysics3d::DebugRenderer::DebugLine* line = debugRenderer->getLinesArray();
			for (int i = 0; i < numLines; i++) {
				Debug::DrawLine(line->point1, line->point2, Debug::CYAN);
				line++;
			}
		}
	}
}

void OGLPaintingGameRenderer::NewRenderText() {
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

	int matSlot = glGetUniformLocation(((OGLShader*)debugShader)->GetProgramID(), "viewProjMatrix");
	glUniformMatrix4fv(matSlot, 1, false, (float*)proj.array);

	GLuint texSlot = glGetUniformLocation(((OGLShader*)debugShader)->GetProgramID(), "useTexture");
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


	glBindBuffer(GL_ARRAY_BUFFER, debugTextRenderer.vertVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, frameVertCount * sizeof(Vector3), debugTextPos.data());
	glBindBuffer(GL_ARRAY_BUFFER, debugTextRenderer.colourVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, frameVertCount * sizeof(Vector4), debugTextColours.data());
	glBindBuffer(GL_ARRAY_BUFFER, debugTextRenderer.texCoordVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, frameVertCount * sizeof(Vector2), debugTextUVs.data());

	glBindVertexArray(debugTextRenderer.vao);
	glDrawArrays(GL_TRIANGLES, 0, frameVertCount);
	glBindVertexArray(0);
}

void OGLPaintingGameRenderer::NewRenderLines() {
	if (!debugShader) return;
	const std::vector<Debug::DebugLineEntry>& lines = Debug::GetDebugLines();
	if (lines.empty()) {
		return;
	}

	boundScreen->GetSceneNode()->OperateOnCameras(
		[&](Camera* cam) {
			cam->SetPerspectiveCameraParameters(Window::GetWindow()->GetScreenAspect() * cam->GetAspectMultiplier());
			glViewport(cam->GetVpStartPos().x * windowWidth, cam->GetVpStartPos().y * windowWidth, windowWidth * cam->GetVpSize().x, windowHeight * cam->GetVpSize().y);
			Matrix4 viewMatrix = cam->BuildViewMatrix();
			Matrix4 projMatrix = cam->BuildProjectionMatrix();

			Matrix4 viewProj = projMatrix * viewMatrix;
			BindShader(debugShader);
			int matSlot = glGetUniformLocation(((OGLShader*)debugShader)->GetProgramID(), "viewProjMatrix");
			GLuint texSlot = glGetUniformLocation(((OGLShader*)debugShader)->GetProgramID(), "useTexture");
			glUniform1i(texSlot, 0);

			glUniformMatrix4fv(matSlot, 1, false, (float*)viewProj.array);

			debugLineData.clear();

			int frameLineCount = lines.size() * 2;

			SetDebugLineBufferSizes(frameLineCount);

			glBindBuffer(GL_ARRAY_BUFFER, debugLineRenderer.vertVbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, lines.size() * sizeof(Debug::DebugLineEntry), lines.data());


			glBindVertexArray(debugLineRenderer.vao);
			glDrawArrays(GL_LINES, 0, frameLineCount);
			glBindVertexArray(0);
		});
}

void OGLPaintingGameRenderer::SetDebugStringBufferSizes(size_t newVertCount) {
	if (newVertCount > debugTextRenderer.count) {
		debugTextRenderer.count = newVertCount;

		glBindBuffer(GL_ARRAY_BUFFER, debugTextRenderer.vertVBO);
		glBufferData(GL_ARRAY_BUFFER, debugTextRenderer.count * sizeof(Vector3), nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, debugTextRenderer.colourVBO);
		glBufferData(GL_ARRAY_BUFFER, debugTextRenderer.count * sizeof(Vector4), nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, debugTextRenderer.texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, debugTextRenderer.count * sizeof(Vector2), nullptr, GL_DYNAMIC_DRAW);

		debugTextPos.reserve(debugTextRenderer.count);
		debugTextColours.reserve(debugTextRenderer.count);
		debugTextUVs.reserve(debugTextRenderer.count);

		glBindVertexArray(debugTextRenderer.vao);

		glVertexAttribFormat(0, 3, GL_FLOAT, false, 0);
		glVertexAttribBinding(0, 0);
		glBindVertexBuffer(0, debugTextRenderer.vertVBO, 0, sizeof(Vector3));

		glVertexAttribFormat(1, 4, GL_FLOAT, false, 0);
		glVertexAttribBinding(1, 1);
		glBindVertexBuffer(1, debugTextRenderer.colourVBO, 0, sizeof(Vector4));

		glVertexAttribFormat(2, 2, GL_FLOAT, false, 0);
		glVertexAttribBinding(2, 2);
		glBindVertexBuffer(2, debugTextRenderer.texCoordVBO, 0, sizeof(Vector2));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}
}

void OGLPaintingGameRenderer::SetDebugLineBufferSizes(size_t newVertCount) {
	if (newVertCount > debugLineRenderer.count) {
		debugLineRenderer.count = newVertCount;

		glBindBuffer(GL_ARRAY_BUFFER, debugLineRenderer.vertVbo);
		glBufferData(GL_ARRAY_BUFFER, debugLineRenderer.count * sizeof(Debug::DebugLineEntry), nullptr, GL_DYNAMIC_DRAW);

		debugLineData.reserve(debugLineRenderer.count);

		glBindVertexArray(debugLineRenderer.vao);

		int realStride = sizeof(Debug::DebugLineEntry) / 2;

		glVertexAttribFormat(0, 3, GL_FLOAT, false, offsetof(Debug::DebugLineEntry, start));
		glVertexAttribBinding(0, 0);
		glBindVertexBuffer(0, debugLineRenderer.vertVbo, 0, realStride);

		glVertexAttribFormat(1, 4, GL_FLOAT, false, offsetof(Debug::DebugLineEntry, colourA));
		glVertexAttribBinding(1, 0);
		glBindVertexBuffer(1, debugLineRenderer.vertVbo, sizeof(Vector4), realStride);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}
}
#endif