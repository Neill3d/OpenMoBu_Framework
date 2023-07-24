
#include "ShaderFactory.h"
#include <map>

using namespace NOpenMobu;

class ShaderFactoryImpl
{
protected:
	std::map<ShaderType, CreateNewShaderInstance>	m_ShaderFuncMap;

public:
	void RegisterShaderType(const ShaderType shaderType, CreateNewShaderInstance createNewShaderFunc)
	{
		auto iter = m_ShaderFuncMap.find(shaderType);
		if (iter == end(m_ShaderFuncMap))
		{
			m_ShaderFuncMap.emplace(shaderType, createNewShaderFunc);
		}
	}

	void UnRegisterShaderType(const ShaderType shaderType)
	{
		m_ShaderFuncMap.erase(shaderType);
	}

	CShaderBase* NewShader(const ShaderType shaderType)
	{
		auto iter = m_ShaderFuncMap.find(shaderType);
		if (iter != end(m_ShaderFuncMap))
		{
			return iter->second(shaderType);
		}
		return nullptr;
	}

	void FreeShader(CShaderBase* shader)
	{
		delete shader;
	}
};

static ShaderFactoryImpl* impl = nullptr;

void NOpenMobu::InitShaderFactory() {
	if (!impl)
		impl = new ShaderFactoryImpl();
}

void NOpenMobu::FreeShaderFactory() {
	if (impl)
	{
		delete impl;
		impl = nullptr;
	}
}

ShaderFactoryImpl* GetImplementation()
{
	return impl;
}

void CShaderFactory::RegisterShaderType(const ShaderType shaderType, CreateNewShaderInstance createNewShaderFunc)
{
	GetImplementation()->RegisterShaderType(shaderType, createNewShaderFunc);
}

void CShaderFactory::UnRegisterShaderType(const ShaderType shaderType)
{
	GetImplementation()->UnRegisterShaderType(shaderType);
}

CShaderBase* CShaderFactory::NewShader(const ShaderType shaderType)
{
	return GetImplementation()->NewShader(shaderType);
}

void CShaderFactory::FreeShader(CShaderBase* shader)
{
	GetImplementation()->FreeShader(shader);
}