#pragma once

#include "libraryMain.h"
#include <GL\glew.h>

namespace NOpenMobu 
{
	/// <summary>
	/// load a cubemap from DDS file by using nv_dds library
	///  creating a texture as GL_TEXTURE_CUBE_MAP
	/// </summary>
	class GRAPHICS_LIBRARY_API COGLCubeMap
	{
	public:

		//! a constructor
		COGLCubeMap();
		//! a destructor
		~COGLCubeMap();

		//
		void Clear();
		bool LoadCubeMap(const char* filename);
		void GenerateMipMaps();

		void Bind();
		void UnBind();

		const GLuint GetCubeMapId() const {
			return cubeId;
		}

	protected:

		GLuint				cubeId;

		// information about load status
		unsigned int		memorySize;
		unsigned int		dimention;
		bool				isCompressed;
		unsigned int		numberOfMipLevels;
		unsigned int		format;
	};

}

