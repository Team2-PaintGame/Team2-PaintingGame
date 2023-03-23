#include "GameAssets.h"


#include "Assets.h"

using namespace NCL;
using namespace Assets;

GameAssets::GameAssets(Assets::AssetLoader* loader) {
	this->loader = loader;

#ifdef __ORBIS__ // Load without loading screen on ps4
	Load();
#endif
}

void GameAssets::Load()
{
	// Clear just in case
	meshes.clear();
	meshAnimations.clear();
	meshMaterials.clear();
	shaders.clear();
	textures.clear();

	LoadTextures();
	LoadMeshes();
	LoadMeshMaterials();
	LoadMeshAnimations();
	LoadShaders();
}

MeshGeometry* NCL::GameAssets::GetMesh(const std::string& identifier) const {
	return meshes.count(identifier) ? meshes.at(identifier).get() : nullptr;
}

MeshMaterial* NCL::GameAssets::GetMeshMaterial(const std::string& identifier) const {
	return meshMaterials.count(identifier) ? meshMaterials.at(identifier).get() : nullptr;
}

MeshAnimation* NCL::GameAssets::GetMeshAnimation(const std::string& identifier) const {
	return meshAnimations.count(identifier) ? meshAnimations.at(identifier).get() : nullptr;
}

TextureBase* NCL::GameAssets::GetTexture(const std::string& identifier) const {
	return textures.count(identifier) ? textures.at(identifier).get() : nullptr;
}

ShaderBase* NCL::GameAssets::GetShader(const std::string& identifier) const {
	return shaders.count(identifier) ? shaders.at(identifier).get() : nullptr;
}


void GameAssets::LoadMeshes() {
	meshes.insert(std::make_pair("floorMesh", loader->LoadMesh("BasicLVL1.msh")));
	meshes.insert(std::make_pair("cubeMesh", loader->LoadMesh("cube.msh")));
	meshes.insert(std::make_pair("mainChar", loader->LoadMesh("Aj_TPose.msh")));
	meshes.insert(std::make_pair("sphereMesh", loader->LoadMesh("sphere.msh")));
	meshes.insert(std::make_pair("capsuleMesh", loader->LoadMesh("capsule.msh")));
	meshes.insert(std::make_pair("gunMesh", loader->LoadMesh("PaintingGun.msh")));
	meshes.insert(std::make_pair("quadMesh", loader->LoadMesh(MeshType::Quad)));

	meshes.insert(std::make_pair("AiMesh", loader->LoadMesh("X_Bot.msh")));
}

void GameAssets::LoadMeshMaterials() {
	meshMaterials.insert(std::make_pair("redMainCharMat", loader->LoadMeshMaterial("red_Aj_TPose.mat")));
	meshMaterials.insert(std::make_pair("blueMainCharMat", loader->LoadMeshMaterial("blue_Aj_TPose.mat")));
	meshMaterials.insert(std::make_pair("monaLisaMat", loader->LoadMeshMaterial("MonaLisa.mat")));
	meshMaterials.insert(std::make_pair("handsPaintingMat", loader->LoadMeshMaterial("handsPainting.mat")));
	meshMaterials.insert(std::make_pair("nightSkyMat", loader->LoadMeshMaterial("nightSky.mat")));
	meshMaterials.insert(std::make_pair("screamPaintMat", loader->LoadMeshMaterial("screamPaint.mat")));
	meshMaterials.insert(std::make_pair("sunflowersMat", loader->LoadMeshMaterial("sunflowers.mat")));
	meshMaterials.insert(std::make_pair("gunMat", loader->LoadMeshMaterial("PaintingGun.mat")));
	meshMaterials.insert(std::make_pair("AiMat", loader->LoadMeshMaterial("X_Bot.mat")));
}

void GameAssets::LoadMeshAnimations() {
	meshAnimations.insert(std::make_pair("mainCharTauntAnim", std::make_unique<MeshAnimation>("Taunt.anm")));
	meshAnimations.insert(std::make_pair("mainCharIdleAnim", std::make_unique<MeshAnimation>("AJIdle.anm")));
	meshAnimations.insert(std::make_pair("mainCharRunAnim", std::make_unique<MeshAnimation>("AJRun.anm")));
	meshAnimations.insert(std::make_pair("AiIdleAnim", std::make_unique<MeshAnimation>("XBot_Idle.anm")));
	meshAnimations.insert(std::make_pair("AiRunAnim", std::make_unique<MeshAnimation>("XBot_Running.anm")));
}

void GameAssets::LoadTextures() {
	textures.insert(std::make_pair("basicTex", loader->LoadTexture("WhiteMarble.jpg")));
//	textures.insert(std::make_pair("basicTex", loader->LoadTexture("checkerboard.png")));
	textures.insert(std::make_pair("loadingTex", loader->LoadTexture("loadingSprites.png")));
	textures.insert(std::make_pair("gunFocusTex", loader->LoadTexture("gunFocusPoint.png")));
	textures.insert(std::make_pair("splashScreenTex", loader->LoadTexture("Screens/bg.png"))); 
	textures.insert(std::make_pair("mainMenuScreenTex", loader->LoadTexture("Screens/bg2.png")));
	textures.insert(std::make_pair("gameOverScreenTex", loader->LoadTexture("Screens/bg3.jpg")));
}

void GameAssets::LoadShaders() {
#ifdef _WIN32
	shaders.insert(std::make_pair("basicShader", loader->LoadShader("scene.vert", "scene.frag")));
	shaders.insert(std::make_pair("terrainShader", loader->LoadShader("terrain.vert", "terrain.frag")));
	shaders.insert(std::make_pair("skinningShader", loader->LoadShader("skinning.vert", "character.frag")));
	shaders.insert(std::make_pair("SecondskinningShader", loader->LoadShader("skinning.vert", "character.frag")));
	shaders.insert(std::make_pair("THIRDskinningShader", loader->LoadShader("skinning.vert", "character.frag")));
	shaders.insert(std::make_pair("screenShader", loader->LoadShader("screen.vert", "screen.frag")));
	shaders.insert(std::make_pair("inkShader", loader->LoadShader("ink.vert", "ink.frag")));
	shaders.insert(std::make_pair("debugShader", loader->LoadShader("Debug.vert", "Debug.frag")));
	shaders.insert(std::make_pair("hudShader", loader->LoadShader("hud.vert", "hud.frag")));
#endif // _WIN32
#ifdef __ORBIS__
	shaders.insert(std::make_pair("basicShader", loader->LoadShader("VertexShader_vv.sb", "PixelShader_p.sb")));
	shaders.insert(std::make_pair("skinningShader", loader->LoadShader("VertexShader_vv.sb", "PixelShader_p.sb")));
	shaders.insert(std::make_pair("SecondskinningShader", loader->LoadShader("VertexShader_vv.sb", "PixelShader_p.sb")));
	shaders.insert(std::make_pair("THIRDskinningShader", loader->LoadShader("VertexShader_vv.sb", "PixelShader_p.sb")));
	shaders.insert(std::make_pair("screenShader", loader->LoadShader("screen_vv.sb", "simplePixel_p.sb")));
	shaders.insert(std::make_pair("inkShader", loader->LoadShader("VertexShader_vv.sb", "PixelShader_p.sb")));
	shaders.insert(std::make_pair("debugShader", loader->LoadShader("screen_vv.sb", "simplePixel_p.sb")));
	shaders.insert(std::make_pair("hudShader", loader->LoadShader("hud_vv.sb", "simplePixel_p.sb")));
#endif // __ORBIS__
}

void GameAssets::ReloadMeshes()
{
	meshes.clear();
	LoadMeshes();
}
void GameAssets::ReloadShaders()
{

	for (auto& pair : shaders)
	{
		if (pair.second)
		{
			pair.second->ReloadShader();
		}
	}
	//shaders.clear();
	//LoadShaders();
}
void GameAssets::ReloadDebug()
{
	shaders.erase("debugShader");
	shaders.insert(std::make_pair("debugShader", loader->LoadShader("Debug.vert", "Debug.frag")));
}
