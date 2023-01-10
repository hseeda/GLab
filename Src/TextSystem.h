#include "pch.h"
#include "ShaderSystem.h"
#include "EventSystem.h"

namespace TS {
	inline std::string font_name = "../Misc/fonts/Antonio-Bold.ttf";
	
	void resize(ES::event e);
	void init();
	void RenderChar(char t, float x, float y, float scale, glm::vec3 color);
	void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
	void exit();
}