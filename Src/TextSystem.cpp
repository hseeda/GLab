#include "pch.h"
#include "TextSystem.h"


#include <ft2build.h>
#include FT_FREETYPE_H

#include "ShaderSystem.h"
#include "WindowSystem.h"

namespace TS {

	//int shaderNumber;
	GLuint shaderID;
	GLuint texture;
	GLuint VAO, VBO;

	/// Holds all state information relevant to a character as loaded using FreeType
	struct Character {
		unsigned int TextureID; // ID handle of the glyph texture
		glm::ivec2   Size;      // Size of glyph
		glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
		unsigned int Advance;   // Horizontal offset to advance to next glyph
	};
	std::map<GLchar, Character> Characters;
	void resize(CEvent e) {
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WS::Width), 0.0f, 
			static_cast<float>(WS::Height));
		glUseProgram(shaderID);
		glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"),
			1, GL_FALSE, glm::value_ptr(projection));
	}

	void init() {
		std::string vs = "#version 330 core\n";
		vs.append("layout(location = 0) in vec4 vertex; // <vec2 pos, vec2 tex> \n");
		vs.append("out vec2 TexCoords;  \n");
		vs.append("uniform mat4 projection;  \n");
		vs.append("void main(){  \n");
		vs.append("gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);  \n");
		vs.append("TexCoords = vertex.zw;  \n");
		vs.append(" } \n");

		std::string fs = "#version 330 core\n";
		fs.append("in vec2 TexCoords; \n");
		fs.append("out vec4 color; \n");
		fs.append("uniform sampler2D text; \n");
		fs.append("uniform vec3 textColor; \n");
		fs.append("void main(){ \n");
		fs.append("vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r); \n");
		fs.append("color = vec4(textColor, 1.0) * sampled; \n");
		fs.append("} \n");
		shaderID = SS::LoadShaderFromString(vs, fs);
		ES::subscribe(ET::window_resize, TS::resize);

		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WS::Width), 0.0f, static_cast<float>(WS::Height));
		glUseProgram(shaderID);
		glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"),
			1, GL_FALSE, glm::value_ptr(projection));

		
		// FreeType
		// --------
		FT_Library ft;
		// All functions return a value different than 0 whenever an error occurred
		if (FT_Init_FreeType(&ft))
		{
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			return;
		}
		// find path to font	
		if (font_name.empty())
		{
			std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
			return;
		}

		// load font as face
		FT_Face face;
		if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
			return;
		}
		else {
			// set size to load glyphs as
			FT_Set_Pixel_Sizes(face, 0, 48);

			// disable byte-alignment restriction
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			// load first 128 characters of ASCII set
			for (unsigned char c = 0; c < 128; c++)
			{
				// Load character glyph 
				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				{
					std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
					continue;
				}
				// generate texture
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer
				);
				// set texture options
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				// now store character for later use
				Character character = {
					texture,
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					static_cast<unsigned int>(face->glyph->advance.x)
				};
				Characters.insert(std::pair<char, Character>(c, character));
			}
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		// destroy FreeType once we're finished
		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		// configure VAO/VBO for texture quads
		// -----------------------------------
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// activate corresponding render state	
		glUseProgram(shaderID);

		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

		glUniform3f(glGetUniformLocation(shaderID, "textColor"), color.x, color.y, color.z);
		glActiveTexture(GL_TEXTURE0);
		
		glBindVertexArray(VAO);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// iterate through all characters
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[*c];

			float xpos = x + ch.Bearing.x * scale;
			float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;
			// update VBO for each character
			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },

				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};
			// render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// render quad

			glDrawArrays(GL_TRIANGLES, 0, 6);

			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		}

		glDisable(GL_BLEND);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void RenderChar(char t, float x, float y, float scale, glm::vec3 color)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// activate corresponding render state	
		glUseProgram(shaderID);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

		glUniform3f(glGetUniformLocation(shaderID, "textColor"), color.x, color.y, color.z);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Character ch = Characters[t];

		float xpos = x - ch.Size.x * scale / 2.0f;
		float ypos = y - ch.Size.y * scale /2.0f;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisable(GL_BLEND);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

	void exit()
	{
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	}
}
