/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#ifdef _WIN32
#include "OGLRenderer.h"
#include "OGLShader.h"
#include "OGLMesh.h"
#include "OGLTexture.h"

#include "SimpleFont.h"
#include "TextureLoader.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"

#include "MeshGeometry.h"

#ifdef _WIN32
#include "Win32Window.h"

#include "KHR\khrplatform.h"
#include "glad\gl.h"
#include "KHR/WGLext.h"

PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
#endif

using namespace NCL;
using namespace NCL::Rendering;

#ifdef OPENGL_DEBUGGING
static void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
#endif;

OGLRenderer::OGLRenderer(Window& w) : RendererBase(w)	{
	initState = false;
#ifdef _WIN32
	InitWithWin32(w);
#endif
	boundMesh	= nullptr;
	boundShader = nullptr;

	windowWidth	= (int)w.GetScreenSize().x;
	windowHeight	= (int)w.GetScreenSize().y;

	if (initState) {
		TextureLoader::RegisterAPILoadFunction(OGLTexture::RGBATextureFromFilename);
	}

	forceValidDebugState = false;
}

OGLRenderer::~OGLRenderer()	{
#ifdef _WIN32
	DestroyWithWin32();
#endif
}

void OGLRenderer::OnWindowResize(int w, int h)	 {
	windowWidth	= w;
	windowHeight	= h;
	glViewport(0, 0, windowWidth, windowHeight);
}

void OGLRenderer::BeginFrame()		{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	BindShader(nullptr);
	BindMesh(nullptr);
}

void OGLRenderer::RenderFrame()		{

}

void OGLRenderer::EndFrame()		{
}

void OGLRenderer::SwapBuffers()   {
	::SwapBuffers(deviceContext);
}

void OGLRenderer::BindShader(ShaderBase*s) {
	if (!s) {
		glUseProgram(0);
		boundShader = nullptr;
	}
	else if (OGLShader* oglShader = dynamic_cast<OGLShader*>(s)) {
		glUseProgram(oglShader->programID);
		boundShader = oglShader;
	}
	else {
		std::cout << __FUNCTION__ << " has received invalid shader?!" << std::endl;
		boundShader = nullptr;
	}
}

void OGLRenderer::BindMesh(MeshGeometry*m) {
	if (!m) {
		glBindVertexArray(0);
		boundMesh = nullptr;
	}
	else if (OGLMesh* oglMesh = dynamic_cast<OGLMesh*>(m)) {
		if (oglMesh->GetVAO() == 0) {
			std::cout << __FUNCTION__ << " has received invalid mesh?!" << std::endl;
		}
		glBindVertexArray(oglMesh->GetVAO());
		boundMesh = oglMesh;
	}
	else {
		std::cout << __FUNCTION__ << " has received invalid mesh?!" << std::endl;
		boundMesh = nullptr;
	}
}

void OGLRenderer::DrawBoundMesh(int subLayer, int numInstances) {
	if (!boundMesh) {
		std::cout << __FUNCTION__ << " has been called without a bound mesh!" << std::endl;
		return;
	}
	if (!boundShader) {
		std::cout << __FUNCTION__ << " has been called without a bound shader!" << std::endl;
		return;
	}
	GLuint	mode	= 0;
	int		count	= 0;
	int		offset	= 0;

	if (boundMesh->GetSubMeshCount() == 0) {
		if (boundMesh->GetIndexCount() > 0) {
			count = boundMesh->GetIndexCount();
		}
		else{
			count = boundMesh->GetVertexCount();
		}
	}
	else {
		const SubMesh* m = boundMesh->GetSubMesh(subLayer);
		offset = m->start;
		count  = m->count;
	}

	switch (boundMesh->GetPrimitiveType()) {
		case GeometryPrimitive::Triangles:		mode = GL_TRIANGLES;		break;
		case GeometryPrimitive::Points:			mode = GL_POINTS;			break;
		case GeometryPrimitive::Lines:			mode = GL_LINES;			break;
		case GeometryPrimitive::TriangleFan:	mode = GL_TRIANGLE_FAN;		break;
		case GeometryPrimitive::TriangleStrip:	mode = GL_TRIANGLE_STRIP;	break;
		case GeometryPrimitive::Patches:		mode = GL_PATCHES;			break;
	}

	if (boundMesh->GetIndexCount() > 0) {
		if (numInstances > 0) {
			glDrawElementsInstanced(mode, count, GL_UNSIGNED_INT, (const GLvoid*)(offset * sizeof(unsigned int)), numInstances);
		}
		else {
			glDrawElements(mode, count, GL_UNSIGNED_INT, (const GLvoid*)(offset * sizeof(unsigned int)));
		}
	}
	else {
		if (numInstances > 0) {
			glDrawArraysInstanced(mode, 0, count, numInstances);
		}
		else {
			glDrawArrays(mode, 0, count);
		}
	}
}

void OGLRenderer::BindTextureToShader(const TextureBase*t, const std::string& uniform, int texUnit) const{
	GLint texID = 0;

	if (!boundShader) {
		std::cout << __FUNCTION__ << " has been called without a bound shader!" << std::endl;
		return;//Debug message time!
	}
	
	GLuint slot = glGetUniformLocation(boundShader->programID, uniform.c_str());

	if (slot < 0) {
		return;
	}

	if (const OGLTexture* oglTexture = dynamic_cast<const OGLTexture*>(t)) {
		texID = oglTexture->GetObjectID();
	}

	glUniform1i(slot, texUnit);
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glUniform1i(slot, texUnit);
}

//void OGLRenderer::DrawDebugLines() {
//	vector<Vector3> vertPos;
//	vector<Vector4> vertCol;
//
//	for (DebugLine&s : debugLines) {
//		vertPos.emplace_back(s.start);
//		vertPos.emplace_back(s.end);
//
//		vertCol.emplace_back(s.colour);
//		vertCol.emplace_back(s.colour);
//	}
//
//	debugLinesMesh->SetVertexPositions(vertPos);
//	debugLinesMesh->SetVertexColours(vertCol);
//	debugLinesMesh->UpdateGPUBuffers(0, (unsigned int)vertPos.size());
//
//	BindMesh(debugLinesMesh);
//	DrawBoundMesh();
//
//	debugLines.clear();
//}

#ifdef _WIN32
void OGLRenderer::InitWithWin32(Window& w) {
	Win32Code::Win32Window* realWindow = (Win32Code::Win32Window*)&w;

	if (!(deviceContext = GetDC(realWindow->GetHandle()))) {
		std::cout << __FUNCTION__ << " Failed to create window!" << std::endl;
		return;
	}

	//A pixel format descriptor is a struct that tells the Windows OS what type of front / back buffers we want to create etc
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize		= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion	= 1;
	pfd.dwFlags		= PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   //It must be double buffered, it must support OGL(!), and it must allow us to draw to it...
	pfd.iPixelType	= PFD_TYPE_RGBA;	//We want our front / back buffer to have 4 channels!
	pfd.cColorBits	= 32;				//4 channels of 8 bits each!
	pfd.cDepthBits	= 24;				//24 bit depth buffer
	pfd.cStencilBits = 8;				//plus an 8 bit stencil buffer
	pfd.iLayerType	= PFD_MAIN_PLANE;

	GLuint		PixelFormat;
	if (!(PixelFormat = ChoosePixelFormat(deviceContext, &pfd))) {	// Did Windows Find A Matching Pixel Format for our PFD?
		std::cout << __FUNCTION__ << " Failed to choose a pixel format!" << std::endl;
		return;
	}

	if (!SetPixelFormat(deviceContext, PixelFormat, &pfd)) {		// Are We Able To Set The Pixel Format?
		std::cout << __FUNCTION__ << " Failed to set a pixel format!" << std::endl;
		return;
	}

	HGLRC		tempContext;		//We need a temporary OpenGL context to check for OpenGL 3.2 compatibility...stupid!!!
	if (!(tempContext = wglCreateContext(deviceContext))) {	// Are We Able To get the temporary context?
		std::cout << __FUNCTION__ <<"  Cannot create a temporary context!" << std::endl;
		wglDeleteContext(tempContext);
		return;
	}

	if (!wglMakeCurrent(deviceContext, tempContext)) {	// Try To Activate The Rendering Context
		std::cout << __FUNCTION__ << " Cannot set temporary context!" << std::endl;
		wglDeleteContext(tempContext);
		return;
	}
	if (!gladLoaderLoadGL()) {
		std::cout << __FUNCTION__ << " Cannot initialise GLAD!" << std::endl;	//It's all gone wrong!
		return;
	}
	//Now we have a temporary context, we can find out if we support OGL 4.x
	char* ver = (char*)glGetString(GL_VERSION); // ver must equal "4.1.0" (or greater!)
	int major = ver[0] - '0';		//casts the 'correct' major version integer from our version string
	int minor = ver[2] - '0';		//casts the 'correct' minor version integer from our version string

	if (major < 3) {					//Graphics hardware does not support OGL 4! Erk...
		std::cout << __FUNCTION__ << " Device does not support OpenGL 4.x!" << std::endl;
		wglDeleteContext(tempContext);
		return;
	}

	if (major == 4 && minor < 1) {	//Graphics hardware does not support ENOUGH of OGL 4! Erk...
		std::cout << __FUNCTION__ << " Device does not support OpenGL 4.1!" << std::endl;
		wglDeleteContext(tempContext);
		return;
	}
	//We do support OGL 4! Let's set it up...

	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
#ifdef OPENGL_DEBUGGING 
		| WGL_CONTEXT_DEBUG_BIT_ARB
#endif		//No deprecated stuff!! DIE DIE DIE glBegin!!!!
		,WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0					//That's enough attributes...
	};

	//Everywhere else in the Renderers, we use function pointers provided by GLEW...but we can't initialise GLEW yet! So we have to use the 'Wiggle' API
	//to get a pointer to the function that will create our OpenGL 3.2 context...
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	renderContext = wglCreateContextAttribsARB(deviceContext, 0, attribs);

	// Check for the context, and try to make it the current rendering context
	if (!renderContext || !wglMakeCurrent(deviceContext, renderContext)) {
		std::cout << __FUNCTION__ <<" Cannot set OpenGL 3 context!" << std::endl;	//It's all gone wrong!
		wglDeleteContext(renderContext);
		wglDeleteContext(tempContext);
		return;
	}

	wglDeleteContext(tempContext);	//We don't need the temporary context any more!

	std::cout << __FUNCTION__ << " Initialised OpenGL " << major << "." << minor << " rendering context" << std::endl;	//It's all gone wrong!

	glEnable(GL_FRAMEBUFFER_SRGB);

#ifdef OPENGL_DEBUGGING
	glDebugMessageCallback(DebugCallback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif

	//If we get this far, everything's going well!
	initState = true; 

	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

	w.SetRenderer(this);
}

HGLRC  OGLRenderer::CreateAnotherContext()
{
	HGLRC currContext;
	if (!initState) return currContext; //did this before initialisation, which is bad

	//Now we have a temporary context, we can find out if we support OGL 4.x
	char* ver = (char*)glGetString(GL_VERSION); // ver must equal "4.1.0" (or greater!)
	int major = ver[0] - '0';		//casts the 'correct' major version integer from our version string
	int minor = ver[2] - '0';		//casts the 'correct' minor version integer from our version string

	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
#ifdef OPENGL_DEBUGGING 
		| WGL_CONTEXT_DEBUG_BIT_ARB
#endif		//No deprecated stuff!! DIE DIE DIE glBegin!!!!
		,WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0					//That's enough attributes...
	};

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	currContext = wglCreateContextAttribsARB(deviceContext, 0, attribs);

	if (!wglShareLists(renderContext, currContext))
	{
		wglDeleteContext(currContext);
		std::cout << "Error sharing lists! ";
	}

	return currContext;
}

bool OGLRenderer::MakeCurrent(HGLRC context)
{
	return wglMakeCurrent(deviceContext, context);
}

void OGLRenderer::ResetDefaultContext() {
	MakeCurrent(renderContext);
}

void OGLRenderer::ResetContext() {
	wglMakeCurrent(NULL, NULL);
}

void OGLRenderer::DestroyWithWin32() {
	wglDeleteContext(renderContext);
}

bool OGLRenderer::SetVerticalSync(VerticalSyncState s) {
	if (!wglSwapIntervalEXT) {
		return false;
	}
	GLuint state;

	switch (s) {
		case VerticalSyncState::VSync_OFF:		state =  0; break;
		case VerticalSyncState::VSync_ON:		state =  1; break;
		case VerticalSyncState::VSync_ADAPTIVE:	state = -1; break;
	}

	return wglSwapIntervalEXT(state);
}
#endif

#ifdef OPENGL_DEBUGGING
static void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
	string sourceName;
	string typeName;
	string severityName;

	switch (source) {
	case GL_DEBUG_SOURCE_API: sourceName = "Source(OpenGL)"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceName = "Source(Window System)"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceName = "Source(Shader Compiler)"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: sourceName = "Source(Third Party)"; break;
	case GL_DEBUG_SOURCE_APPLICATION: sourceName = "Source(Application)"; break;
	case GL_DEBUG_SOURCE_OTHER: sourceName = "Source(Other)"; break;
	}

	switch (type) {
	case GL_DEBUG_TYPE_ERROR: typeName = "Type(Error)"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeName = "Type(Deprecated Behaviour)"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typeName = "Type(Undefined Behaviour)"; break;
	case GL_DEBUG_TYPE_PORTABILITY: typeName = "Type(Portability)"; break;
	case GL_DEBUG_TYPE_PERFORMANCE: typeName = "Type(Performance)"; break;
	case GL_DEBUG_TYPE_OTHER: typeName = "Type(Other)"; break;
	}

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: severityName = "Priority(High)"; break;
	case GL_DEBUG_SEVERITY_MEDIUM: severityName = "Priority(Medium)"; break;
	case GL_DEBUG_SEVERITY_LOW: severityName = "Priority(Low)"; break;
	}

	if (severity == GL_DEBUG_SEVERITY_HIGH) {
		bool a = true;
	}

	std::cout << "OpenGL Debug Output: " + sourceName + ", " + typeName + ", " + severityName + ", " + string(message) << std::endl;
}
#endif
#endif