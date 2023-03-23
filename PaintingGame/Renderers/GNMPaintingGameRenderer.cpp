#ifdef __ORBIS__
#include "GNMPaintingGameRenderer.h"
#include "GameWorld.h"
#include <GNMTexture.h>
#include "GNMShader.h"
#include <GNMMesh.h>
#include "Camera.h"
#include "Debug.h"

using namespace NCL;
using namespace CSC8508;
using namespace GNM;

GNMPaintingGameRenderer::GNMPaintingGameRenderer(Window& w) : GNMRenderer(w) {
	
	//Set up the light properties
	lightColour = Vector4(0.8f, 0.8f, 0.5f, 1.0f);
	lightRadius = 1000.0f;
	lightPosition = Vector3(0.0f, 20.0f, 0.0f);

	//shaderVariables.cameraVariables = (CameraVariables*)onionAllocator->allocate(sizeof(CameraVariables), Gnm::kEmbeddedDataAlignment4);
	//shaderVariables.renderObjectVariables = (RenderObjectVariables*)onionAllocator->allocate(sizeof(RenderObjectVariables), Gnm::kEmbeddedDataAlignment4);
	//shaderVariables.fragmentVariables = (FragmentVariables*)garlicAllocator->allocate(sizeof(FragmentVariables), Gnm::kEmbeddedDataAlignment4);

	//shaderBuffers.cameraBuffer.initAsConstantBuffer(shaderVariables.cameraVariables, sizeof(CameraVariables));
	//shaderBuffers.cameraBuffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO); // it's a constant buffer, so read-only is OK

	//shaderBuffers.objBuffer.initAsConstantBuffer(shaderVariables.renderObjectVariables, sizeof(RenderObjectVariables));
	//shaderBuffers.objBuffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO); // it's a constant buffer, so read-only is OK

	//shaderBuffers.fragmentBuffer.initAsConstantBuffer(shaderVariables.fragmentVariables, sizeof(FragmentVariables));
	//shaderBuffers.fragmentBuffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO); // it's a constant buffer, so read-only is OK
	
	//SetDebugStringBufferSizes(10000);
	//SetDebugLineBufferSizes(1000);

	viewProjMat = (Matrix4*)onionAllocator->allocate(sizeof(Matrix4), Gnm::kEmbeddedDataAlignment4);
	//*viewProjMat = Matrix4();

	shaderBuffers.cameraBuffer.initAsConstantBuffer(viewProjMat, sizeof(Matrix4));
	shaderBuffers.cameraBuffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO); // it's a constant buffer, so read-only is OK
}

GNMPaintingGameRenderer::~GNMPaintingGameRenderer() {}

void GNMPaintingGameRenderer::RenderFrame() {
	currentFrame->StartFrame();

	currentGFXContext->waitUntilSafeForRendering(videoHandle, currentGPUBuffer);

	SetRenderBuffer(currentGNMBuffer, true, true, true);

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
	currentFrame->EndFrame();
}

void GNMPaintingGameRenderer::RenderBasicScreen() { //change this to render static obj
	currentGFXContext->setupScreenViewport(0, 0, currentGNMBuffer->colourTarget.getWidth(), currentGNMBuffer->colourTarget.getHeight(), 0.5f, 0.5f);
	RenderObject* r = boundScreen->GetSceneNode()->GetRenderObject();
	
	Gnm::Sampler trilinearSampler;
	trilinearSampler.init();
	trilinearSampler.setMipFilterMode(Gnm::kMipFilterModeLinear);

	currentGFXContext->setTextures(Gnm::kShaderStagePs, 0, 1, &((GNMTexture*)(r->GetDefaultTexture()))->GetAPITexture());
	currentGFXContext->setSamplers(Gnm::kShaderStagePs, 0, 1, &trilinearSampler);
	
	((GNMShader*)(r->GetShader()))->SubmitShaderSwitch(*currentGFXContext);
	((GNMMesh*)(r->GetMesh()))->SubmitDraw(*currentGFXContext, Gnm::ShaderStage::kShaderStageVs);
	
	boundScreen->RenderMenu();
}
void GNMPaintingGameRenderer::RenderGameScreen() { //change this to RenderScreen
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	////send camera things and light things to shader
	boundScreen->GetSceneNode()->OperateOnCameras(
		[&](Camera* cam) {
			cam->SetPerspectiveCameraParameters(Window::GetWindow()->GetScreenAspect() * cam->GetAspectMultiplier());
			currentGFXContext->setupScreenViewport(cam->GetVpStartPos().x * currentGNMBuffer->colourTarget.getWidth(), cam->GetVpStartPos().y * currentGNMBuffer->colourTarget.getWidth(), currentGNMBuffer->colourTarget.getWidth() * cam->GetVpSize().x, currentGNMBuffer->colourTarget.getHeight() * cam->GetVpSize().y, 0.5f, 0.5f);
			*viewProjMat = cam->BuildProjectionMatrix() * cam->BuildViewMatrix();
			/*shaderVariables.cameraVariables->viewProjMatrix = cam->BuildProjectionMatrix() * cam->BuildViewMatrix();
			shaderVariables.fragmentVariables->cameraPos = cam->GetPosition();
			shaderVariables.fragmentVariables->lightColour = lightColour;
			shaderVariables.fragmentVariables->lightPos = lightPosition;
			shaderVariables.fragmentVariables->lightRadius = lightRadius;*/

			for (const auto& i : activeObjects) {
				//if (i->GetIsInstanced()) {
				//	return;
				//}
				//Primitive Setup State
				Gnm::PrimitiveSetup primitiveSetup;
				primitiveSetup.init();
				primitiveSetup.setFrontFace(Gnm::kPrimitiveSetupFrontFaceCcw);

				////Screen Access State
				Gnm::DepthStencilControl dsc;
				dsc.init();
				dsc.setDepthControl(Gnm::kDepthControlZWriteEnable, Gnm::kCompareFuncLessEqual);

				Gnm::BlendControl blend;
				blend.setBlendEnable(true);

				if (i->GetIsOccluded()) {
					primitiveSetup.setCullFace(Gnm::kPrimitiveSetupCullFaceBack);
					dsc.setDepthEnable(true);
				}
				else {
					primitiveSetup.setCullFace(Gnm::kPrimitiveSetupCullFaceNone);
					dsc.setDepthEnable(false);
				}
				currentGFXContext->setPrimitiveSetup(primitiveSetup);
				currentGFXContext->setDepthStencilControl(dsc);
				

				GNMShader* shader = (GNMShader*)(i)->GetShader();
				//((GNMShader*)(i->GetShader()))->SubmitShaderSwitch(*currentGFXContext);

				if (i->IsRigged()) {
					//vector<Matrix4> frameMatrices;
					//i->GetFrameMatrices(frameMatrices);
					//glUniformMatrix4fv(locations.jointsLocation, frameMatrices.size(), false, (float*)frameMatrices.data());
				}
				RenderPaintSplat(shader);
				
				

				if (i->GetDefaultTexture()) {
					//shaderVariables.fragmentVariables->hasTexture = true;
					Gnm::Sampler trilinearSampler;
					trilinearSampler.init();
					trilinearSampler.setMipFilterMode(Gnm::kMipFilterModeLinear);

					currentGFXContext->setTextures(Gnm::kShaderStagePs, 0, 1, &((GNMTexture*)(i->GetDefaultTexture()))->GetAPITexture());
					currentGFXContext->setSamplers(Gnm::kShaderStagePs, 0, 1, &trilinearSampler);
					//currentGFXContext->setBlendControl(0, blend);
				}
				else if (i->HasTextureAtIndex(0)) {
					Gnm::Sampler trilinearSampler;
					trilinearSampler.init();
					trilinearSampler.setMipFilterMode(Gnm::kMipFilterModeLinear);

					currentGFXContext->setTextures(Gnm::kShaderStagePs, 0, 1, &((GNMTexture*)(i->GetTextures(0)[0].second))->GetAPITexture());
					currentGFXContext->setSamplers(Gnm::kShaderStagePs, 0, 1, &trilinearSampler);
				}

				if (i->isSingleTextured()) {
					RenderWithDefaultTexture(i);
				}
				else {
					RenderWithMultipleTexture(i);
				}
				//unsigned int numInstances = i->GetInstanceCount();

				SendModelMatrices(shader, i);
				SetShaderBufffers(shader);
				((GNMShader*)(i->GetShader()))->SubmitShaderSwitch(*currentGFXContext);
				((GNMMesh*)(i->GetMesh()))->SubmitDraw(*currentGFXContext, Gnm::ShaderStage::kShaderStageVs, 0);
			}
		}
	);
	RenderDebugInformation();
	boundScreen->RenderMenu();
}

void GNMPaintingGameRenderer::RenderPaintSplat(GNMShader* shader) {
	/*GameWorld* world = boundScreen->GetSceneNode()->GetWorld();
	if (world) {
		world->OperateOnPaintedPositions(
			[&](int index, Vector3& pos) {
				std::string i = std::to_string(index);
				glUniform3fv(glGetUniformLocation(shader->GetProgramID(), ("paintedPos[" + i + "]").c_str()), 1, pos.array);
			}
		);
		int splatVectorSize = glGetUniformLocation(shader->GetProgramID(), "numOfSplats");
		glUniform1i(splatVectorSize, world->GetNumPaintedPositions());
	}*/
}

void GNMPaintingGameRenderer::BuildObjectList() {
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

void GNMPaintingGameRenderer::SetShaderBufffers(GNMShader* shader) {
	int camIndex = shader->GetConstantBufferIndex("CameraData", Gnm::kShaderStageVs);

	if (camIndex >= 0) {
		currentGFXContext->setConstantBuffers(Gnm::kShaderStageVs, camIndex, 1, &shaderBuffers.cameraBuffer);
	}
	/*int objIndex = shader->GetConstantBufferIndex("RenderObjectData", Gnm::kShaderStageVs);
	int camIndex = shader->GetConstantBufferIndex("CameraData", Gnm::kShaderStageVs);
	int fragIndex = shader->GetConstantBufferIndex("FragmentData", Gnm::kShaderStagePs);

	if (objIndex >= 0) {
		currentGFXContext->setConstantBuffers(Gnm::kShaderStageVs, objIndex, 1, &shaderBuffers.objBuffer);
	}
	if (camIndex >= 0) {
		currentGFXContext->setConstantBuffers(Gnm::kShaderStageVs, camIndex, 1, &shaderBuffers.cameraBuffer);
	}
	if (fragIndex >= 0) {
		currentGFXContext->setConstantBuffers(Gnm::kShaderStagePs, fragIndex, 1, &shaderBuffers.fragmentBuffer);
	}*/
}

void GNMPaintingGameRenderer::SortObjectList() {

}

void GNMPaintingGameRenderer::SendModelMatrices(GNMShader* shader, const RenderObject* r) {


	unsigned int numInstances = r->GetInstanceCount();
	Gnm::Buffer constantBuffer;
	if (r->GetIsInstanced()) {
		if (numInstances == 0) {
			return;
		}
		//std::vector<Transform*> transforms = r->GetTransforms();
		//Matrix4* modelMat[100];
		//for (int i = 0; i < numInstances; i++) {

		//	modelMat[i] = (Matrix4*)currentGFXContext->allocateFromCommandBuffer(sizeof(Matrix4), Gnm::kEmbeddedDataAlignment4);
		//	*modelMat[i] = transforms[i]->GetMatrix();
		//}
		//constantBuffer.initAsConstantBuffer(modelMat[0], sizeof(Matrix4) * numInstances);
		//constantBuffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO); // it's a constant buffer, so read-only is OK
	}
	else {
		Matrix4* modelMat = (Matrix4*)currentGFXContext->allocateFromCommandBuffer(sizeof(Matrix4), Gnm::kEmbeddedDataAlignment4);
		*modelMat = r->GetTransform()->GetMatrix();

		constantBuffer.initAsConstantBuffer(modelMat, sizeof(Matrix4));
		constantBuffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO); // it's a constant buffer, so read-only is OK

		/*currentGFXContext->setConstantBuffers(Gnm::kShaderStageVs, objIndex, 1, &constantBuffer);
		currentGFXContext->setConstantBuffers(Gnm::kShaderStageVs, camIndex, 1, &cameraBuffer);*/

		//shaderVariables.renderObjectVariables->modelMatrix = r->GetTransform()->GetMatrix();
		//shaderVariables.renderObjectVariables->inverseModel = Matrix3(r->GetTransform()->GetMatrix().Inverse());
	}

	int objIndex = shader->GetConstantBufferIndex("RenderObjectData", Gnm::kShaderStageVs);

	if (objIndex >= 0) {
		currentGFXContext->setConstantBuffers(Gnm::kShaderStageVs, objIndex, 1, &constantBuffer);
	}

	//send color data
	//shaderVariables.renderObjectVariables->hasVertexColours = !(*r).GetMesh()->GetColourData().empty();
	//shaderVariables.renderObjectVariables->objectColour = r->GetColour();
	//*(shaderVariables.renderObjectVariables) = 
}

void GNMPaintingGameRenderer::RenderWithDefaultTexture(const RenderObject* r) {
	/*unsigned int numInstances = r->GetInstanceCount();
	Gnm::Sampler trilinearSampler;
	trilinearSampler.init();
	trilinearSampler.setMipFilterMode(Gnm::kMipFilterModeLinear);

	currentGFXContext->setTextures(Gnm::kShaderStagePs, 0, 1, &((GNMTexture*)(r->GetDefaultTexture()))->GetAPITexture());
	currentGFXContext->setSamplers(Gnm::kShaderStagePs, 0, 1, &trilinearSampler);
	
	((GNMShader*)(r->GetShader()))->SubmitShaderSwitch(*currentGFXContext);
	((GNMMesh*)(r->GetMesh()))->SubmitDraw(*currentGFXContext, Gnm::ShaderStage::kShaderStageVs);*/

	/*int layercount = r->GetMesh()->GetSubMeshCount();
	int index = 0;
	do {
		DrawBoundMesh(index++, numInstances);
	} while (index < layercount);*/

}

void GNMPaintingGameRenderer::RenderWithMultipleTexture(const RenderObject* r) {
	//unsigned int numInstances = r->GetInstanceCount();
	//BindMesh(r->GetMesh());
	//int layercount = r->GetMesh()->GetSubMeshCount();
	//int index = 0;
	//do {
	//	glUniform1i(locations.hasTexLocation, r->GetTextures(index).size() ? 1 : 0);
	//	//for the current submesh, get the vector of textures and send them to shader
	//	std::vector<std::pair<std::string, TextureBase*>> submeshtextures = r->GetTextures(index);
	//	int texunit = 0;
	//	for (const auto& texturepairs : submeshtextures) {
	//		BindTextureToShader(texturepairs.second, texturepairs.first, texunit);
	//		texunit++;
	//	}
	//	DrawBoundMesh(index++, numInstances);
	//} while (index < layercount);
}

//Debug methods

void GNMPaintingGameRenderer::RenderDebugInformation() const {
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	//if (boundScreen->GetSceneNode()->GetPhysicsWorld() && boundScreen->GetSceneNode()->GetPhysicsWorld()->getIsDebugRenderingEnabled()) {
	//	reactphysics3d::DebugRenderer* debugRenderer = &boundScreen->GetSceneNode()->GetPhysicsWorld()->getDebugRenderer();

	//	//render triangles
	//	int numTri = debugRenderer->getNbTriangles();
	//	if (numTri) {
	//		const reactphysics3d::DebugRenderer::DebugTriangle* tri = debugRenderer->getTrianglesArray();
	//		for (int i = 0; i < numTri; i++) {
	//			Debug::DrawTriangle(tri->point1, tri->point2, tri->point3, Debug::YELLOW);
	//			tri++;
	//		}
	//	}

	//	//render lines
	//	int numLines = debugRenderer->getNbLines();
	//	if (numLines) {
	//		const reactphysics3d::DebugRenderer::DebugLine* line = debugRenderer->getLinesArray();
	//		for (int i = 0; i < numLines; i++) {
	//			Debug::DrawLine(line->point1, line->point2, Debug::CYAN);
	//			line++;
	//		}
	//	}
	//}
}

void GNMPaintingGameRenderer::NewRenderText() {
	/*const std::vector<Debug::DebugStringEntry>& strings = Debug::GetDebugStrings();
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
	glBindVertexArray(0);*/
}

void GNMPaintingGameRenderer::NewRenderLines() {
	/*if (!debugShader) return;
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
		});*/
}

void GNMPaintingGameRenderer::SetDebugStringBufferSizes(size_t newVertCount) {
	/*if (newVertCount > debugTextRenderer.count) {
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
	}*/
}

void GNMPaintingGameRenderer::SetDebugLineBufferSizes(size_t newVertCount) {
	/*if (newVertCount > debugLineRenderer.count) {
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
	}*/
}
#endif // __ORBIS__
