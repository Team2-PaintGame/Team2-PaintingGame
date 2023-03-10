#include "GameAssets.h"

using namespace NCL;
using namespace Assets;

GameAssets::GameAssets(Assets::AssetLoader* loader) {
	this->loader = loader;
	LoadMeshes();
	LoadMeshMaterials();
	LoadMeshAnimations();
	LoadTextures();
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
	meshes.insert(std::make_pair("floorMesh", loader->LoadMesh("BasicLVL.msh")));
	meshes.insert(std::make_pair("cubeMesh", loader->LoadMesh("cube.msh")));
	meshes.insert(std::make_pair("mainChar", loader->LoadMesh("Aj_TPose.msh")));
	meshes.insert(std::make_pair("sphereMesh", loader->LoadMesh("sphere.msh")));
	meshes.insert(std::make_pair("coinMesh", loader->LoadMesh("coin.msh")));
	meshes.insert(std::make_pair("capsuleMesh", loader->LoadMesh("capsule.msh")));
	meshes.insert(std::make_pair("gunMesh", loader->LoadMesh("PaintingGun.msh")));
	meshes.insert(std::make_pair("quadMesh", loader->LoadMesh(MeshType::Quad)));
}

void GameAssets::LoadMeshMaterials() {
	meshMaterials.insert(std::make_pair("goatMat", std::make_unique<MeshMaterial>("goat.mat")));
	meshMaterials.insert(std::make_pair("redMainCharMat", new MeshMaterial("red_Aj_TPose.mat")));
	meshMaterials.insert(std::make_pair("blueMainCharMat", new MeshMaterial("blue_Aj_TPose.mat")));
	meshMaterials.insert(std::make_pair("monaLisaMat", new MeshMaterial("MonaLisa.mat")));
	meshMaterials.insert(std::make_pair("appleFaceMat", new MeshMaterial("appleFace.mat")));
	meshMaterials.insert(std::make_pair("handsPaintingMat", new MeshMaterial("handsPainting.mat")));
	meshMaterials.insert(std::make_pair("nightSkyMat", new MeshMaterial("nightSky.mat")));
	meshMaterials.insert(std::make_pair("screamPaintMat", new MeshMaterial("screamPaint.mat")));
	meshMaterials.insert(std::make_pair("sunflowersMat", new MeshMaterial("sunflowers.mat")));
	meshMaterials.insert(std::make_pair("gunMat", new MeshMaterial("PaintingGun.mat")));
}

void GameAssets::LoadMeshAnimations() {
	meshAnimations.insert(std::make_pair("mainCharTauntAnim", std::make_unique<MeshAnimation>("Taunt.anm")));
	meshAnimations.insert(std::make_pair("mainCharIdleAnim", std::make_unique<MeshAnimation>("AJIdle.anm")));
	meshAnimations.insert(std::make_pair("mainCharRunAnim", std::make_unique<MeshAnimation>("AJRun.anm")));
}

void GameAssets::LoadTextures() {
	textures.insert(std::make_pair("basicTex", loader->LoadTexture("checkerboard.png")));
	textures.insert(std::make_pair("grassTex", loader->LoadTexture("grass.jpg")));
	textures.insert(std::make_pair("coinTex", loader->LoadTexture("coin.png")));
	textures.insert(std::make_pair("floorTex", loader->LoadTexture("ground.png")));
	textures.insert(std::make_pair("goatTex", loader->LoadTexture("goat1.jpg")));
	textures.insert(std::make_pair("doorTex", loader->LoadTexture("door.jpg")));
	textures.insert(std::make_pair("gunFocusTex", loader->LoadTexture("gunFocusPoint.png")));
	textures.insert(std::make_pair("splashScreenTex", loader->LoadTexture("Screens/bg.jpg"))); 
	textures.insert(std::make_pair("mainMenuScreenTex", loader->LoadTexture("Screens/bg2.jpg")));
}

void GameAssets::LoadShaders() {
	shaders.insert(std::make_pair("basicShader", loader->LoadShader("scene.vert", "scene.frag")));
	shaders.insert(std::make_pair("terrainShader", loader->LoadShader("terrain.vert", "terrain.frag")));
	shaders.insert(std::make_pair("skinningShader", loader->LoadShader("skinning.vert", "character.frag")));
	shaders.insert(std::make_pair("screenShader", loader->LoadShader("screen.vert", "screen.frag")));
	shaders.insert(std::make_pair("inkShader", loader->LoadShader("ink.vert", "ink.frag")));
	shaders.insert(std::make_pair("debugShader", loader->LoadShader("Debug.vert", "Debug.frag")));
	shaders.insert(std::make_pair("hudShader", loader->LoadShader("hud.vert", "hud.frag")));
}
