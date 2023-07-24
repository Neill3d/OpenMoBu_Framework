#pragma once

#include <string.h>
#include "libraryMain.h"
#include "FileBase.h"

namespace NOpenMobu 
{

	/**
	 * interface for glsl shaderand moc shader class
	 */
	enum class ShaderManagerType
	{
		GLSL,
		Count
	};



	class CShaderManagerBase {};

	/** \class ShaderBase
	 * \brief A base class for gpu shaders, it's not ogl dependent
	 *  The class has some derives with ogl glsl implementation and test implementation
	 * \see GLSLShader
	 */
	template<typename T, typename shaderT>
	class GRAPHICS_LIBRARY_API CShaderManager : public CShaderManagerBase
	{
	public:
		//!< a constructor
		CShaderManager()
		{
			memset(m_HeaderText, 0, sizeof(char) * 256);
		}
		//!< a destructor
		virtual ~CShaderManager() = default;

		/**
		 * an abstract for derived implementation, to know an implementation type
		 *
		 * \return an implementation type (glsl, moc shader. etc.)
		 */
		virtual const ShaderManagerType GetShaderType() const = 0;

		// resources

		void SetHeaderText(const char* text)
		{
			memset(m_HeaderText, 0, sizeof(char) * 256);
			strcpy_s(m_HeaderText, 256, text);
		}

		// TODO: why shader have to manage any file system stuff ?!
                virtual int NewShader(
                    const NOpenMobu::CFileBase& vertex_file,
                    const NOpenMobu::CFileBase& fragment_file) = 0;
		// reuse vertex program from teh referenceShader
                virtual int NewShader(
                    const shaderT& referenceShader,
                    const NOpenMobu::CFileBase& fragment_file) = 0;

		// TODO: do we want to recompile optional one of shaders ?!
                virtual bool ReCompileShaders(
                    int shaderId,
                    const NOpenMobu::CFileBase& vertex_file,
                    const NOpenMobu::CFileBase& fragment_file) = 0;

		//! bind a shader, have to be accompine with UnBind call
		virtual T& Bind(int shaderId) const = 0;
		virtual T& UnBind() const = 0;

		// uniforms
#pragma region Uniforms

		virtual bool SetUniformUINT(int shaderId, const char* name, const int value) const = 0;
		virtual bool SetUniformFloat(int shaderId, const char* name, const float value) const = 0;
		virtual bool SetUniformVector(int shaderId, const char* name, const float x, const float y, const float z, const float w) const = 0;
		virtual bool SetUniformVector2f(int shaderId, const char* name, const float x, const float y) const = 0;
		virtual bool SetUniformMatrix33(int shaderId, const char* name, const float* m) const = 0;
		virtual bool SetUniformMatrix(int shaderId, const char* name, const float* m) const = 0;

		//!< from a compiled shader, try to find a location for the uniform by name
		virtual int FindLocation(int shaderId, const char* name) const = 0;

		virtual const T& SetUniformUINT(const int location, const int value) const = 0;
		virtual const T& SetUniformFloat(const int location, const float value) const = 0;
		virtual const T& SetUniformVector(const int location, const float x, const float y, const float z, const float w) const = 0;
		virtual const T& SetUniformVector4(const int location, const float* arr) const = 0;
		virtual const T& SetUniformVector2f(const int location, const float x, const float y) const = 0;
		virtual const T& SetUniformMatrix33(const int location, const float* m) const = 0;
		virtual const T& SetUniformMatrix(const int location, const float* m) const = 0;

#pragma endregion

#pragma region Textures
		virtual T& BindTexture(unsigned int target, const int location, unsigned int texid, int texunit) = 0;
		virtual T& UnBindTexture(unsigned int target, int texunit) = 0;
		virtual T& BindTexture(int shaderId, unsigned int target, const char* texname, unsigned int texid, int texunit) = 0;
		virtual T& BindTextureRECT(int shaderId, const char* texname, unsigned int texid, int texunit) = 0;
#pragma 

		int GetHeaderLength() const { return static_cast<int>(strlen(m_HeaderText)); }

	protected:
		const static int HEADER_SIZE{ 256 };
		//!< a text that could be inserted into the beginning of the shader code, some shared definitions and const
		char          m_HeaderText[HEADER_SIZE];

		const int GetHeaderCapacity() const { return HEADER_SIZE; }
		const char* GetHeaderText() const { return m_HeaderText; }
	};
}

