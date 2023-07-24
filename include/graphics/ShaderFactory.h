#pragma once

#include "libraryMain.h"
#include "Shader.h"

namespace NOpenMobu 
{
	typedef CShaderBase* (*CreateNewShaderInstance)(const ShaderType);

	/// <summary>
	///  Initialize glew and opengl extensions
	/// </summary>
	void GRAPHICS_LIBRARY_API InitShaderFactory();
	void GRAPHICS_LIBRARY_API FreeShaderFactory();

	class GRAPHICS_LIBRARY_API CShaderFactory
	{
	public:
		static void RegisterShaderType(const ShaderType shaderType, CreateNewShaderInstance createNewShaderFunc);
		static void UnRegisterShaderType(const ShaderType shaderType);

		static CShaderBase* NewShader(const ShaderType shaderType);
		static void FreeShader(CShaderBase* shader);

		template <typename T> static T* NewShaderTyped()
		{
			return reinterpret_cast<T*>(NewShader(T::TypeId()));
		}
	};
}
