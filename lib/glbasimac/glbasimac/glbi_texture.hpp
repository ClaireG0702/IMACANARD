#pragma once

#include <iostream>
#include <cassert>
#include "tools/gl_tools.hpp"

using namespace STP3D;

namespace glbasimac
{

	struct GLBI_Texture
	{
		GLBI_Texture() : id_in_GL(0), width(0), height(0), channels(0) {
						 };

		~GLBI_Texture()
		{
			glDeleteTextures(1, &id_in_GL);
		};

		// move constructor(to be able to "construct" by moving the ownership of the texture)
		GLBI_Texture(GLBI_Texture &&other) noexcept
			: id_in_GL(other.id_in_GL),
			  width(other.width), height(other.height), channels(other.channels)
		{
			other.id_in_GL = 0;
			other.width = 0;
			other.height = 0;
			other.channels = 0;
		}

		// remove copy constructor and copy assignation
		GLBI_Texture(const GLBI_Texture &) = delete;
		GLBI_Texture &operator=(const GLBI_Texture &) = delete;

		// move assignation operator (to be able to "copy" by moving the ownership of the texture)
		GLBI_Texture &operator=(GLBI_Texture &&other) noexcept
		{
			if (this != &other)
			{
				// Clean up current object
				glDeleteTextures(1, &id_in_GL);
				// Move data from other
				id_in_GL = other.id_in_GL;
				width = other.width;
				height = other.height;
				channels = other.channels;
				// Reset other
				other.id_in_GL = 0;
				other.width = 0;
				other.height = 0;
				other.channels = 0;
			}
			return *this;
		}

		void createTexture();
		void attachTexture() const;
		void detachTexture() const;
		void loadImage(unsigned int w, unsigned int h, unsigned int n_chan, unsigned char *pixels);
		void setParameters(unsigned int param, unsigned int value);

		// Texture parameters
		unsigned int id_in_GL;
		unsigned int width, height;
		unsigned int channels;
	};

}
