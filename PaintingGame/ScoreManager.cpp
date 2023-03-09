#include "ScoreManager.h"
#include "Debug.h"
#include "Window.h"

using namespace NCL;


void ScoreManager::ShowTimer()
{
	Debug::Print("Time: " + std::to_string(Window::GetTimer()->GetTotalTimeSeconds()), Vector2(35, 5));
}
