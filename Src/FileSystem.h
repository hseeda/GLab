#pragma once
#include "pch.h"

namespace FS {

	inline std::string _shaders_path = "..\\Misc\\Shaders\\";
	inline std::string _texture_path = "..\\Misc\\Images\\";
	inline std::string _icon_path = "..\\Misc\\Images\\";
	
	bool readAbaqusInpFile(
		std::vector<float> &verts,
		std::vector<unsigned int> &indices,
		std::string file_name = "../Misc/models/aq123.inp");

	
}
