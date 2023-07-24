#ifndef GLSLSHADER_H_INCLUDED
#define GLSLSHADER_H_INCLUDED

/*
Sergei <Neill3d> Solokhin 2018-2022
		class is based on http://steps3d.narod.ru library
		Thanks for Alex Boreskov

GitHub page - https://github.com/Neill3d/OpenMoBu
Licensed under The "New" BSD License - https://github.com/Neill3d/OpenMoBu/blob/master/LICENSE
*/

#include "libraryMain.h"
#include <stdio.h>
#include <GL\glew.h>
#include <string_view>

namespace NOpenMobu 
{
	struct GRAPHICS_LIBRARY_API CGLSLShader
	{
		//!< vertex shader handle
		GLhandleARB     m_Vertex{ 0 };

		//!< fragment handle
		GLhandleARB     m_Fragment{ 0 };

		//!< v & f
		GLhandleARB     m_ProgramObj{ 0 };
	};
}


#endif // GLSLSHADER_H_INCLUDED
