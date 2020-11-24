#pragma once

#include <glad/glad.h>
#include <thread>
#include "HTPP.h"
#include "ImGui/imgui.h"
#include "stb_image.h"
#include "stb_image_write.h"

class ImageView final
{
	GLuint out_texture = 0;
	int out_width;
	int out_height;
	bool imageHere = false;
	bool needToLoadImage = false;
public:
	std::string ip;
	bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
	{
		// Load from file
		int image_width = 0;
		int image_height = 0;
		int comp;
		unsigned char* image_data = stbi_load(filename, &image_width, &image_height, &comp, STBI_rgb_alpha);
		stbi_write_jpg("fuckit.jpg", image_width, image_height, comp, image_data, 100);

		if (image_data == NULL)
			return false;

		// Create a OpenGL texture identifier
		GLuint image_texture;
		glGenTextures(1, &image_texture);
		glBindTexture(GL_TEXTURE_2D, image_texture);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

		// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		//stbi_image_free(image_data);
		glBindTexture(GL_TEXTURE_2D, 0);
		*out_texture = image_texture;
		*out_width = image_width;
		*out_height = image_height;

		return true;
	}
	void loadImageFromLocalFile()
	{
		if (LoadTextureFromFile("data.jpg", &out_texture, &out_width, &out_height))
			imageHere = true;
		needToLoadImage = false;
	}
	void loadImage()
	{
		std::thread([&]() {
			HTTP::getRequest(ip, [&](std::string responce)
				{
					if (!responce.empty())
					{
						std::fstream f("data.jpg", std::fstream::out | std::fstream::binary);
						f.write(responce.c_str(), responce.length());
						f.close();

						//log("image saved");

						needToLoadImage = true;
					}
				}, "getImg");
			}).detach();
	}
	void onImGuiDraw()
	{
		if (needToLoadImage)
			loadImageFromLocalFile();

		ImGui::Begin("Screen");
		if (ImGui::Button("Load"))
		{
			loadImage();
		}
		if (imageHere)
		{
			ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
			ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
			ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
			ImGui::Image((void*)(intptr_t)out_texture, ImVec2(out_width, out_height), uv_min, uv_max, tint_col, border_col);
		}
		ImGui::End();
	}
};
