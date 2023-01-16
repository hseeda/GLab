#include "pch.h"
#include "GLabSystem.h"
#include "WindowSystem.h"
#include "EventSystem.h"
#include "ShaderSystem.h"
#include "camera.h"

namespace GS {
	
	void init() {
		WS::init();
		IS::init();
		SS::init();
		TS::init();
		BS::pre_init();

		ES::subscribe("open__file", std::bind([]() -> std::string {return openFileDialog(); }));
	}

	void exit() {
		BS::exit();
		TS::exit();
		SS::exit();
		IS::exit();
		WS::exit();
	}
}