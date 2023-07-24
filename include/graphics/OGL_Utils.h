
#pragma once

/*
	Sergei <Neill3d> Solokhin 2018-2022

	GitHub page - https://github.com/Neill3d/OpenMoBu
	Licensed under The "New" BSD License - https://github.com/Neill3d/OpenMoBu/blob/master/LICENSE
*/

#include "libraryMain.h"
#include <GL\glew.h>

namespace NOpenMobu 
{

	// Calculates log2 of number.  
	//double log2( double n );


	/////////////////////////////////////////////////////////
	#pragma region DrawPrimitives

	void GRAPHICS_LIBRARY_API Push2dViewport(int g_w, int g_h);
	void GRAPHICS_LIBRARY_API Pop2dViewport();
	void GRAPHICS_LIBRARY_API Set2dViewport(int g_w, int g_h);

	//void drawOrthoQuad2d(const double w, const double h);
	void GRAPHICS_LIBRARY_API drawOrthoQuad2dOffset(const double x, const double y, const double w, const double h);
	//void drawOrthoGradientQuad2d(const int w, const int h);

	void GRAPHICS_LIBRARY_API PrepareOrthoView(const double w, const double h);

	// prepare vertex buffer for faster draw primitives
	void GRAPHICS_LIBRARY_API PrepareDrawPrimitives();
	// will draw quad 1x1 with texcoords
	void GRAPHICS_LIBRARY_API DrawQuad2d();

	// old staff
	void GRAPHICS_LIBRARY_API drawOrthoQuad2d(const double w, const double h);
	void GRAPHICS_LIBRARY_API drawOrthoQuad2d(int x, int y, int x2, int y2, int w, int h);
	void GRAPHICS_LIBRARY_API drawQuad2d(double x, double y, double x2, double y2);

	// if using vao, we need to recreate vao each time we change context
	void GRAPHICS_LIBRARY_API ChangeContextDrawPrimitives();
	// cleanup drawing primitives buffers
	void GRAPHICS_LIBRARY_API FreeDrawPrimitives();

	void GRAPHICS_LIBRARY_API FitImageWithAspect(const int imgWidth, const int imgHeight, const int regionWidth, const int regionHeight, int& x, int& y, int& width, int& height);

	void GRAPHICS_LIBRARY_API GenerateFBO(const int Width, const int Height, GLuint& color_buffer_id, GLuint& depth_id, GLuint& fbo_id, GLubyte* data = nullptr);
	void GRAPHICS_LIBRARY_API CleanupFBO(GLuint& color_buffer_id, GLuint& depth_id, GLuint& fbo_id);

	//////////////////

	bool GRAPHICS_LIBRARY_API BlitFBOToFBO(const GLint FBO, const int width, const int height, const GLint defaultFBO,
		const int defWidth, const int defHeight, const bool copyColor1, const bool copyColor2, const bool copyColor3);
	bool GRAPHICS_LIBRARY_API BlitFBOToFBOOffset(const GLint FBO, int x, int y, const int width, const int height, const GLint defaultFBO,
		int defX, int defY, const int defWidth, const int defHeight, bool copyDepth, const bool copyColor1, const bool copyColor2, const bool copyColor3);
	bool GRAPHICS_LIBRARY_API BlitFBOToScreen(const GLint defaultFBO, const int defWidth, const int defHeight, const int screenWidth, const int screenHeight);

	#pragma endregion

	////////////////////////////////////////////////
	#pragma region Cull Face Info
	struct	OGLCullFaceInfo
	{
		GLboolean			enabled;	// glIsEnabled
		GLint				mode;		// GL_CULL_FACE_MODE
	};

	void	StoreCullMode(OGLCullFaceInfo& info);
	void	FetchCullMode(const OGLCullFaceInfo& info);
	#pragma endregion
	////////////////////////////////////////////////

	void TextureObjectGetInfo(const GLenum target, int& width, int& height, int& internalFormat, int& format, bool& isCompressed, int& pixelMemorySize, int& imageSize, int& numberOfLods);
	void TextureObjectGetData(const GLenum target, GLubyte* localImageBuffer, int& width, int& height, int& internalFormat, int& format, bool& isCompressed, int& pixelMemorySize, int& imageSize, int& numberOfLods);

	//////////////////////////////////////////////////////////////////////////////////////
	#pragma region Frame Buffer Info

	constexpr int MAX_DRAW_BUFFERS{ 10 };

	struct FrameBufferInfo
	{
		GLint		gViewport[4];
		GLenum		gDrawBuffers[MAX_DRAW_BUFFERS];
		GLint		gMaxDrawBuffers;
		GLint		gLastFramebuffer;
	};

	void SaveFrameBuffer(FrameBufferInfo* pInfo = nullptr);
	void RestoreFrameBuffer(FrameBufferInfo* pInfo = nullptr);

	void BindFrameBufferLayers(const GLuint textureId);

	#pragma endregion

	#pragma region FrameBuffer MS
	struct CFrameBufferMS
	{
		GLuint			fbo;
		GLuint			rbColor;
		GLuint			rbNormal;
		GLuint			rbDepth;
		GLuint			tColor;
		GLuint			tNormal;
		GLuint			tDepth;

		//
		int				width;
		int				height;
		int				samples;

		// creating flags for generating a framebuffer
		enum EFlag
		{
			TEXTURES = 2,
			RENDERBUFFERS = 4,
			NORMAL = 8
		};
		int				flags;
	};

	void	ZeroFrameBufferMS(CFrameBufferMS& framebuffer);
	bool	PrepFrameBufferMS(CFrameBufferMS& framebuffer, const int width, const int height, const int samples, const int flags, const int normalFormat);
	void	FreeFrameBufferMS(CFrameBufferMS& framebuffer);
	#pragma endregion

	//////////////////
	/*
	bool BlitFBOToFBO(const GLint FBO, const int width, const int height, const GLint defaultFBO, const int defWidth, const int defHeight, const bool copyColor1);
	bool BlitFBOToScreen(const GLint defaultFBO, const int defWidth, const int defHeight, const int screenWidth, const int screenHeight);
	*/
	//////////////////////////////////////////////////////////////////////////////////////
	//
}

