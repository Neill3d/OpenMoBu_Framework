#include "GLSLShaderManager.h"
#include "CheckGLError.h"

using namespace NOpenMobu;

/*
void CGLSLShaderManager::SetHeaderText(std::string_view headerText)
{ 
	//m_HeaderText = headerText; 
}
*/
bool CGLSLShaderManager::ReCompileShaders(int shaderIndex, const CFileBase& vertexFile, const CFileBase& fragmentFile)
{
	if (shaderIndex < 0 && shaderIndex > m_Shaders.size())
	{
		if (m_Logger) m_Logger->Trace("ReCompileShaders received index out of range");
		return false;
	}
	
	CGLSLShader& shader = m_Shaders[shaderIndex];

	const int headerLength{ 32 };
	const int fragmentFileLength{ fragmentFile.GetLength() };

	// fragment
	int newFragment = 0;

	std::vector<char> shaderData;
	shaderData.resize(headerLength + fragmentFileLength + 1, 0);
	memcpy(shaderData.data(), m_HeaderText, sizeof(char) * headerLength);

	if (fragmentFile.Read(shaderData.data() + headerLength, fragmentFileLength))
	{
		newFragment = LoadShader(GL_FRAGMENT_SHADER_ARB, shaderData.data(), static_cast<int>(shaderData.size()));
	}

	if (newFragment == 0)
		return false;

	return TryLink(shader, 0, newFragment);
}

int CGLSLShaderManager::NewShader(const CFileBase& vertexFile, const CFileBase& fragmentFile)
{
	CGLSLShader newShader;

	const int headerLength{ 32 };
	std::vector<char> shaderData;

	int newVertex = 0, newFragment = 0;

	// vertex
	{
		const int vertexFileLength{ vertexFile.GetLength() };
		shaderData.resize(headerLength + vertexFileLength + 1, 0);
		memcpy(shaderData.data(), m_HeaderText, sizeof(char*) * headerLength);

		if (vertexFile.Read(shaderData.data() + headerLength, vertexFileLength))
		{
			newVertex = LoadShader(GL_VERTEX_SHADER_ARB, shaderData.data(), static_cast<int>(shaderData.size()));
		}
	}

	// fragment
	{
		const int fragmentFileLength{ fragmentFile.GetLength() };
		shaderData.resize(headerLength + fragmentFileLength + 1, 0);
		memcpy(shaderData.data(), m_HeaderText, sizeof(char*) * headerLength);

		if (fragmentFile.Read(shaderData.data() + headerLength, fragmentFileLength))
		{
			newFragment = LoadShader(GL_FRAGMENT_SHADER_ARB, shaderData.data(), static_cast<int>(shaderData.size()));
		}
	}

	if (newVertex == 0 && newFragment == 0)
	{
		if (m_Logger) m_Logger->Trace("Couldn't load the shader");
		return -1;
	}
	
	if (!TryLink(newShader, newVertex, newFragment))
	{
		Free(newShader);
		if (m_Logger) m_Logger->Trace("Couldn't link the shader");
		return -1;
	}

	const int shaderIndex = static_cast<int>(m_Shaders.size());
	m_Shaders.push_back(newShader);
	return shaderIndex;
}

GLint CGLSLShaderManager::TryLink(CGLSLShader& shader, int vertex, int fragment)
{
	if (shader.m_ProgramObj == 0)
		shader.m_ProgramObj = glCreateProgramObjectARB();

	if (vertex > 0 && vertex != shader.m_Vertex)
		glAttachObjectARB(shader.m_ProgramObj, vertex);
	if (fragment > 0 && fragment != shader.m_Fragment)
		glAttachObjectARB(shader.m_ProgramObj, fragment);

	glLinkProgramARB(shader.m_ProgramObj);

	CHECK_GL_ERROR();

	GLint linked;
	glGetObjectParameterivARB(shader.m_ProgramObj, GL_OBJECT_LINK_STATUS_ARB, &linked);
	Loadlog(shader.m_ProgramObj);

	if (linked != 0)
	{
		if (shader.m_Vertex > 0 && vertex > 0 && vertex != shader.m_Vertex)
		{
			glDeleteObjectARB(shader.m_Vertex);
			shader.m_Vertex = vertex;
		}
		if (shader.m_Fragment > 0 && fragment > 0 && fragment != shader.m_Fragment)
		{
			glDeleteObjectARB(shader.m_Fragment);
			shader.m_Fragment = fragment;
		}

		return true;
	}
	else
	{
		// TODO: consider return back to previous shaders if new one failed ?!
	}

	return false;
}

int CGLSLShaderManager::NewShader(const CGLSLShader& referenceShader, const CFileBase& fragmentFile)
{
	CGLSLShader newShader;

	int newVertex = referenceShader.m_Vertex;
	int newFragment = 0;

	// fragment
	const int headerLength{ 32 };
	const int fragmentFileLength{ fragmentFile.GetLength() };

	std::vector<char> shaderData;
	shaderData.resize(headerLength + fragmentFileLength + 1, 0);
	memcpy(shaderData.data(), m_HeaderText, sizeof(char*) * headerLength);

	if (fragmentFile.Read(shaderData.data() + headerLength, fragmentFileLength))
	{
		newFragment = LoadShader(GL_FRAGMENT_SHADER_ARB, shaderData.data(), static_cast<int>(shaderData.size()));
	}

	if (newFragment == 0)
		return -1;

	if (!TryLink(newShader, newVertex, newFragment))
	{
		Free(newShader);
		return -1;
	}

	const int shaderIndex = static_cast<int>(m_Shaders.size());
	m_Shaders.push_back(newShader);
	return shaderIndex;
}

GLhandleARB CGLSLShaderManager::LoadShader(const GLenum shaderType, const char* buffer, const int bufferSize) const
{
	GLhandleARB shader = glCreateShaderObjectARB(shaderType);

	const GLcharARB* bufferARB = buffer;
	glShaderSourceARB(shader, 1, &bufferARB, &bufferSize);
	glCompileShaderARB(shader);

	GLint   compileStatus;
	glGetObjectParameterivARB(shader, GL_OBJECT_COMPILE_STATUS_ARB, &compileStatus);
	Loadlog(shader);

	if (compileStatus != 0)
		shader;

	glDeleteObjectARB(shader);
	return 0;
}

void CGLSLShaderManager::Loadlog(GLhandleARB object) const
{
	GLint       logLength = 0;
	GLsizei     charsWritten = 0;

	GLcharARB* infoLog;

	glGetObjectParameterivARB(object, GL_OBJECT_INFO_LOG_LENGTH_ARB, &logLength);
	/*
		if ( checkopenglerror )          // check for OpenGL errors
			return;
	*/
	if (logLength < 1)
		return;

	// try to avoid allocating buffer
	infoLog = new GLcharARB[logLength];
	glGetInfoLogARB(object, logLength, &charsWritten, infoLog);

#ifdef _DEBUG
	if (strlen(infoLog) > 0)
	{
		LOG_TRACE(infoLog);
	}
#endif
	delete[] infoLog;
}

CGLSLShaderManager& CGLSLShaderManager::Bind(int shaderIndex) const
{
	const CGLSLShader& shader = GetShader(shaderIndex);
	if (shader.m_ProgramObj)
		glUseProgramObjectARB(shader.m_ProgramObj);
	return const_cast<CGLSLShaderManager&>(*this);
}

CGLSLShaderManager& CGLSLShaderManager::UnBind() const
{
	glUseProgramObjectARB(0);
	return const_cast<CGLSLShaderManager&>(*this);
}


void CGLSLShaderManager::Free(CGLSLShader& shader)
{
	if (shader.m_ProgramObj) glDeleteObjectARB(shader.m_ProgramObj);
	if (shader.m_Vertex)     glDeleteObjectARB(shader.m_Vertex);
	if (shader.m_Fragment)   glDeleteObjectARB(shader.m_Fragment);

	shader.m_ProgramObj = 0;
	shader.m_Vertex = 0;
	shader.m_Fragment = 0;
}

void CGLSLShaderManager::Free(int shaderIndex)
{
	CGLSLShader& shader = const_cast<CGLSLShader&>(GetShader(shaderIndex));
	Free(shader);
}

int CGLSLShaderManager::FindLocation(int shaderIndex, const char* name) const
{
	const CGLSLShader& shader = GetShader(shaderIndex);
	if (!shader.m_ProgramObj)
		return -1;
	int loc = glGetUniformLocationARB(shader.m_ProgramObj, name);
	//loadlog( programObj );
	return loc;
}



bool CGLSLShaderManager::SetUniformUINT(int shaderIndex, const char* name, const int value) const
{
	const CGLSLShader& shader = GetShader(shaderIndex);
	if (!shader.m_ProgramObj)
		return false;
	int loc = glGetUniformLocationARB(shader.m_ProgramObj, name);
	if (loc < 0)
	{
		//info("uniform location failed");
		return false;
	}

	Loadlog(shader.m_ProgramObj);

	glUniform1iARB(loc, value);

	return true;
}

bool CGLSLShaderManager::SetUniformFloat(int shaderIndex, const char* name, const float value) const
{
	const CGLSLShader& shader = GetShader(shaderIndex);
	if (!shader.m_ProgramObj)
		return false;
	int loc = glGetUniformLocationARB(shader.m_ProgramObj, name);
	if (loc < 0)
		return false;

	glUniform1fARB(loc, value);
	return true;
}

bool CGLSLShaderManager::SetUniformVector(int shaderIndex, const char* name, const float x, const float y, const float z, const float w) const
{
	const CGLSLShader& shader = GetShader(shaderIndex);
	if (!shader.m_ProgramObj) return false;
	int loc = glGetUniformLocationARB(shader.m_ProgramObj, name);
	if (loc < 0)
		return false;

	glUniform4fARB(loc, x, y, z, w);
	return true;
}

bool CGLSLShaderManager::SetUniformVector2f(int shaderIndex, const char* name, const float x, const float y) const
{
	const CGLSLShader& shader = GetShader(shaderIndex);
	if (!shader.m_ProgramObj) return false;
	int loc = glGetUniformLocationARB(shader.m_ProgramObj, name);
	if (loc < 0)
		return false;

	glUniform2fARB(loc, x, y);
	return true;
}


bool CGLSLShaderManager::SetUniformMatrix33(int shaderIndex, const char* name, const float* m) const
{
	const CGLSLShader& shader = GetShader(shaderIndex);
	if (!shader.m_ProgramObj) return false;
	int loc = glGetUniformLocationARB(shader.m_ProgramObj, name);
	if (loc < 0)
		return false;

	glUniformMatrix3fvARB(loc, 1, GL_FALSE, m);
	return true;
}

bool CGLSLShaderManager::SetUniformMatrix(int shaderIndex, const char* name, const float* m) const
{
	const CGLSLShader& shader = GetShader(shaderIndex);
	if (!shader.m_ProgramObj) return false;
	int loc = glGetUniformLocationARB(shader.m_ProgramObj, name);
	if (loc < 0)
		return false;

	glUniformMatrix4fvARB(loc, 1, GL_FALSE, m);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////

const CGLSLShaderManager& CGLSLShaderManager::SetUniformUINT(const int location, const int value) const
{
	glUniform1iARB(location, value);
	return *this;
}

const CGLSLShaderManager& CGLSLShaderManager::SetUniformFloat(const int location, const float value) const
{
	glUniform1fARB(location, value);
	return *this;
}

const CGLSLShaderManager& CGLSLShaderManager::SetUniformVector(const int location, const float x, const float y, const float z, const float w) const
{
	glUniform4fARB(location, x, y, z, w);
	return *this;
}

const CGLSLShaderManager& CGLSLShaderManager::SetUniformVector4(const int location, const float* arr) const
{
	glUniform4fv(location, 1, arr);
	return *this;
}

const CGLSLShaderManager& CGLSLShaderManager::SetUniformVector2f(const int location, const float x, const float y) const
{
	glUniform2fARB(location, x, y);
	return *this;
}

const CGLSLShaderManager& CGLSLShaderManager::SetUniformMatrix33(const int location, const float* m) const
{
	glUniformMatrix3fvARB(location, 1, GL_FALSE, m);
	return *this;
}

const CGLSLShaderManager& CGLSLShaderManager::SetUniformMatrix(const int location, const float* m) const
{
	glUniformMatrix4fvARB(location, 1, GL_FALSE, m);
	return *this;
}


//

CGLSLShaderManager& CGLSLShaderManager::BindTexture(int shaderIndex, unsigned int target, const char* texname, unsigned int texid, int texunit)
{
	glActiveTexture(GL_TEXTURE0 + texunit);
	glBindTexture(target, texid);
	SetUniformUINT(shaderIndex, texname, (GLint)texunit);
	glActiveTexture(GL_TEXTURE0);
	return *this;
}

CGLSLShaderManager& CGLSLShaderManager::BindTexture(unsigned int target, const int location, unsigned int texid, int texunit)
{
	glActiveTexture(GL_TEXTURE0 + texunit);
	glBindTexture(target, texid);
	SetUniformUINT(location, (GLint)texunit);
	glActiveTexture(GL_TEXTURE0);
	return *this;
}

CGLSLShaderManager& CGLSLShaderManager::UnBindTexture(unsigned int target, int texunit)
{
	glActiveTexture(GL_TEXTURE0 + texunit);
	glBindTexture(target, 0);
	glActiveTexture(GL_TEXTURE0);
	return *this;
}

CGLSLShaderManager& CGLSLShaderManager::BindTextureRECT(int shaderIndex, const char* texname, unsigned int texid, int texunit)
{
	BindTexture(shaderIndex, GL_TEXTURE_RECTANGLE_ARB, texname, texid, texunit);
	return *this;
}
