
#include "OGLCubeMap.h"
//#ifdef OGL_UTIL_CUBEMAP
#include "nv_dds/nv_dds.h"
//#endif

using namespace NOpenMobu;

COGLCubeMap::COGLCubeMap()
{
	cubeId = 0;
}

COGLCubeMap::~COGLCubeMap()
{
	Clear();
}

void COGLCubeMap::Clear()
{
	if (cubeId > 0)
	{
		glDeleteTextures(1, &cubeId);
		cubeId = 0;
	}
}

bool COGLCubeMap::LoadCubeMap(const char* filename)
{
	glActiveTexture(GL_TEXTURE0);

	nv_dds::CDDSImage	cubeImage;

	if (false == cubeImage.load(filename, false))
	{
		//FBTrace( "Failed to load cubeImage\n" );
		return false;
	}

	if (false == cubeImage.is_cubemap())
	{
		//FBTrace( "DDS image is not a cubemap!" );
		return false;
	}

	memorySize = cubeImage.get_size();
	isCompressed = cubeImage.is_compressed();
	numberOfMipLevels = cubeImage.get_num_mipmaps();
	format = cubeImage.get_format();
	dimention = cubeImage.get_width();

	//

	if (cubeId == 0)
	{
		glGenTextures(1, &cubeId);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeId);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (false == cubeImage.upload_textureCubemap())
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		//FBTrace( "failed to upload a texture cubemap\n" );
		return false;
	}

	return true;
}

void COGLCubeMap::GenerateMipMaps()
{
	if (cubeId > 0)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeId);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}
}

void COGLCubeMap::Bind()
{
	if (cubeId > 0)
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeId);
}

void COGLCubeMap::UnBind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

