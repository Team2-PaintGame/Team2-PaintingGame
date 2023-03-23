#include "Debug.h"
#include "Window.h"

using namespace NCL;

std::vector<Debug::DebugStringEntry>	Debug::stringEntries;
std::vector<Debug::DebugLineEntry>		Debug::lineEntries;

SimpleFont* Debug::debugFont = nullptr;

const Vector4 Debug::RED		= Vector4(1, 0, 0, 1);
const Vector4 Debug::GREEN		= Vector4(0, 1, 0, 1);
const Vector4 Debug::BLUE		= Vector4(0, 0, 1, 1);

const Vector4 Debug::BLACK		= Vector4(0, 0, 0, 1);
const Vector4 Debug::WHITE		= Vector4(1, 1, 1, 1);

const Vector4 Debug::YELLOW		= Vector4(1, 1, 0, 1);
const Vector4 Debug::MAGENTA	= Vector4(1, 0, 1, 1);
const Vector4 Debug::CYAN		= Vector4(0, 1, 1, 1);

int Debug::frames = 0;
double Debug::startTime = 0;
double Debug::currentTime = 0;
bool Debug::first = true;
float Debug::fps = 0.0f;
float Debug::renderingTime = 0.0f;
int Debug::numberOfParticals = 0;
int Debug::numberOfGameObjects = 0;
int Debug::numberOfPaints = 0;
#ifdef _WIN32
DWORD Debug::PageFaultCount = 0;
size_t Debug::PeakWorkingSetSize = 0;
size_t Debug::WorkingSetSize = 0;
size_t Debug::QuotaPeakPagedPoolUsage = 0;
size_t Debug::QuotaPagedPoolUsage = 0;
size_t Debug::QuotaPeakNonPagedPoolUsage = 0;
size_t Debug::QuotaNonPagedPoolUsage = 0;
size_t Debug::PagefileUsage = 0;
size_t Debug::PeakPagefileUsage = 0;

DWORDLONG Debug::totalVirtualMemory = 0;
DWORDLONG Debug::usedVirtualMemory = 0;
DWORDLONG Debug::totalPhysMemory = 0;
DWORDLONG Debug::usedphysMemory = 0;
#endif

void Debug::Print(const std::string& text, const Vector2& pos, const Vector4& colour) {
	DebugStringEntry newEntry;

	newEntry.data = text;
	newEntry.position = pos;
	newEntry.colour = colour;

	stringEntries.emplace_back(newEntry);
}

void Debug::DrawLine(const Vector3& startpoint, const Vector3& endpoint, const Vector4& colour, float time) {
	DebugLineEntry newEntry;

	newEntry.start = startpoint;
	newEntry.end = endpoint;
	newEntry.colourA = colour;
	newEntry.colourB = colour;
	newEntry.time = time;

	lineEntries.emplace_back(newEntry);
}

void Debug::DrawTriangle(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector4& colour, float time) {
	Debug::DrawLine(v1, v2, colour, time);
	Debug::DrawLine(v2, v3, colour, time);
	Debug::DrawLine(v3, v1, colour, time);
}

void Debug::DrawAxisLines(const Matrix4& modelMatrix, float scaleBoost, float time) {
	Matrix4 local = modelMatrix;
	local.SetPositionVector({ 0, 0, 0 });

	Vector3 fwd = local * Vector4(0, 0, -1, 1.0f);
	Vector3 up = local * Vector4(0, 1, 0, 1.0f);
	Vector3 right = local * Vector4(1, 0, 0, 1.0f);

	Vector3 worldPos = modelMatrix.GetPositionVector();

	DrawLine(worldPos, worldPos + (right * scaleBoost), Debug::RED, time);
	DrawLine(worldPos, worldPos + (up * scaleBoost), Debug::GREEN, time);
	DrawLine(worldPos, worldPos + (fwd * scaleBoost), Debug::BLUE, time);
}

void Debug::UpdateRenderables(float dt) {
	int trim = 0;
	for (int i = 0; i < lineEntries.size(); ) {
		DebugLineEntry* e = &lineEntries[i];
		e->time -= dt;
		if (e->time < 0) {
			trim++;
			lineEntries[i] = lineEntries[lineEntries.size() - trim];
		}
		else {
			++i;
		}
		if (i + trim >= lineEntries.size()) {
			break;
		}
	}
	lineEntries.resize(lineEntries.size() - trim);
	stringEntries.clear();
}

SimpleFont* Debug::GetDebugFont() {
	if (!debugFont) {
		debugFont = new SimpleFont("PressStart2P.fnt", "PressStart2P.png");
	}
	return debugFont;
}

const std::vector<Debug::DebugStringEntry>& Debug::GetDebugStrings() {
	return stringEntries;
}

const std::vector<Debug::DebugLineEntry>& Debug::GetDebugLines() {
	return lineEntries;
}

void NCL::Debug::DrawFPS()
{
	if (first)
	{
		frames = 0;
		startTime = Window::GetTimer()->GetTotalTimeSeconds();
		first = false;
	}
	frames++;
	currentTime = Window::GetTimer()->GetTotalTimeSeconds();
	if (currentTime - startTime > 0.25 && frames > 10)
	{
		fps = (double)frames / (currentTime - startTime);
		startTime = currentTime;
		frames = 0;
	}

	//std::cout << "FPS: " << fps << "\n";

	//Print("FPS: " + std::to_string(fps),10,1);
}

void NCL::Debug::ShowMemoryUsage(/*DWORD processID*/)
{
#ifdef _WIN32
	//HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);

	totalVirtualMemory = memInfo.ullTotalPageFile;
	usedVirtualMemory = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;

	totalPhysMemory = memInfo.ullTotalPhys;
	usedphysMemory = memInfo.ullTotalPhys - memInfo.ullAvailPhys;

	if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
	{
		PageFaultCount = pmc.PageFaultCount;
		PeakWorkingSetSize = pmc.PeakWorkingSetSize;
		WorkingSetSize = pmc.WorkingSetSize;
		QuotaPeakPagedPoolUsage = pmc.QuotaPeakPagedPoolUsage;
		QuotaPagedPoolUsage = pmc.QuotaPagedPoolUsage;
		QuotaPeakNonPagedPoolUsage = pmc.QuotaPeakNonPagedPoolUsage;
		QuotaNonPagedPoolUsage = pmc.QuotaNonPagedPoolUsage;
		PagefileUsage = pmc.PagefileUsage;
		PeakPagefileUsage = pmc.PeakPagefileUsage;
	}
#endif
	//CloseHandle(hProcess);
}

void NCL::Debug::ShowNumberOfParticals(size_t nbParticals)
{
	numberOfParticals = nbParticals;
	//Print("Number of particals Generated: " + std::to_string(nbParticals), 10, 5);
}

void NCL::Debug::ShowNumberOfGameObjects(size_t nbObjects)
{
	numberOfGameObjects = nbObjects;
	//Print("No of GameObjects in world is: " + std::to_string(nbObjects), 10, 8);
}

void NCL::Debug::ShowNumberOfPaintedPositions(size_t nbPaints)
{
	numberOfPaints = nbPaints;
	//Print("No of PaintedPositions in world is: " + std::to_string(nbPaints), 10, 10);
}

void NCL::Debug::ShowRenderTime(float time)
{
	renderingTime = time;
}
