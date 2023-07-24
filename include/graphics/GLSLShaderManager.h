#pragma once

#include "libraryMain.h"
#include <stdio.h>
#include <GL\glew.h>
#include "GLSLShader.h"
#include "ShaderManager.h"
#include "FileBase.h"
#include <vector>
#include <string_view>
#include <Logger.h>

namespace NOpenMobu 
{
	//
	// profile for GLSL, OpenGL 2.0+
	//
	class GRAPHICS_LIBRARY_API CGLSLShaderManager : public CShaderManager<CGLSLShaderManager, CGLSLShader>
	{
	private:

		CLoggerCallback*			m_Logger{ nullptr };
		//char						m_HeaderText[32]{ 0 };
		std::vector<CGLSLShader>	m_Shaders; //!< all loaded shaders

		GLhandleARB LoadShader(const GLenum shaderType, const char* buffer, const int bufferSize) const;
		void Loadlog(GLhandleARB object) const;

		GLint TryLink(CGLSLShader& shader, int newVertex, int newFragment);

	public:
		//! a constructor
		CGLSLShaderManager(CLoggerCallback* logger)
			: m_Logger(logger)
		{}

		void Free(CGLSLShader& shader);
		void Free(int shaderIndex);

		virtual ~CGLSLShaderManager() {
			//Free();
		}

		// resources
		//void SetHeaderText(std::string_view headerText);

		int NewShader(const NOpenMobu::CFileBase& vertexFile,
                    const NOpenMobu::CFileBase& fragmentFile) override;

		// the vertex program from referenceShader will be reused
        int NewShader(
                    const CGLSLShader& referenceShader,
                    const NOpenMobu::CFileBase& fragmentFile) override;

		bool ReCompileShaders(int shader,
                    const NOpenMobu::CFileBase& vertexFile,
                    const NOpenMobu::CFileBase& fragmentFile) override;

		const CGLSLShader& GetShader(int shaderIndex) const { return m_Shaders[shaderIndex]; }

		// user of it

		CGLSLShaderManager& Bind(int shader) const override;
		CGLSLShaderManager& UnBind() const override;

		// define uniforms

		bool SetUniformUINT(int shader, const char* name, const int value) const override;
		bool SetUniformFloat(int shader, const char* name, const float value) const override;
		bool SetUniformVector(int shader, const char* name, const float x, const float y, const float z, const float w) const override;
		bool SetUniformVector2f(int shader, const char* name, const float x, const float y) const override;
		bool SetUniformMatrix33(int shader, const char* name, const float* m) const override;
		bool SetUniformMatrix(int shader, const char* name, const float* m) const override;

		int FindLocation(int shader, const char* name) const override;

		const CGLSLShaderManager& SetUniformUINT(const int location, const int value) const override;
		const CGLSLShaderManager& SetUniformFloat(const int location, const float value) const override;
		const CGLSLShaderManager& SetUniformVector(const int location, const float x, const float y, const float z, const float w) const override;
		const CGLSLShaderManager& SetUniformVector4(const int location, const float* arr) const override;
		const CGLSLShaderManager& SetUniformVector2f(const int location, const float x, const float y) const override;
		const CGLSLShaderManager& SetUniformMatrix33(const int location, const float* m) const override;
		const CGLSLShaderManager& SetUniformMatrix(const int location, const float* m) const override;

		CGLSLShaderManager& BindTexture(unsigned int target, const int location, unsigned int texid, int texunit) override;
		CGLSLShaderManager& UnBindTexture(unsigned int target, int texunit) override;
		CGLSLShaderManager& BindTexture(int shader, unsigned int target, const char* texname, unsigned int texid, int texunit) override;
		CGLSLShaderManager& BindTextureRECT(int shader, const char* texname, unsigned int texid, int texunit) override;
	};

}
