
#include "CppUnitTest.h"
#include "postEffectFishEye.h"
#include "ShaderFactory.h"
#include <cstdarg>
//#include "glslang_c_interface.h"
#include <array>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace PostProcessingEffects;

namespace OpenMoBuUnitTests
{
	class MyLogger : public CLoggerCallback
	{
	public:
		void Trace(const char* message, ...) override
		{
			char buffer[256]{ 0 };

			va_list argptr;
			va_start(argptr, message);
			vsprintf_s(buffer, sizeof(char)*256, message, argptr);
			va_end(argptr);

			Logger::WriteMessage(buffer);
		}
	};

	class MyUICallback : public UICallback
	{
	public:
		int RegisterUIElement(int type, const char* name) override
		{
			return -1;
		}

		const double GetUIElementValue(const int elementID) override
		{
			return 0.0;
		}

		bool Ok() const override
		{
			return false;
		}
	};
	
	class MyShaderMOC : public ShaderBase
	{
	private:

		template<typename ... Args>
		std::string execCmd(const char* cmd, Args ... args)
		{
			int size_s = std::snprintf(nullptr, 0, cmd, args ...) + 1; // Extra space for '\0'
			if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
			auto size = static_cast<size_t>(size_s);
			std::unique_ptr<char[]> buf(new char[size]);
			std::snprintf(buf.get(), size, cmd, args ...);

			std::array<char, 128> buffer;
			std::string result;
			std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(buf.get(), "r"), _pclose);
			if (!pipe) {
				throw std::runtime_error("popen() failed!");
			}
			while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
				result += buffer.data();
			}
			return result;
		}

		bool ValidateShader(FILE* file) //, glslang_program_t* program, glslang_stage_t stage)
		{
			size_t headerLen = strlen(m_HeaderText); // number of bytes in header

			fseek(file, 0, SEEK_END);
			size_t fileLen = ftell(file);
			fseek(file, 0, SEEK_SET);

			char* buffer = new char[headerLen + fileLen + 1];
			if (!buffer)
				return false;

			// read shader from file
			memset(&buffer[0], 0, sizeof(char) * (headerLen + fileLen + 1));
			if (headerLen)
				memcpy(&buffer[0], &m_HeaderText[0], sizeof(char) * headerLen);
			void* buf = (void*)&buffer[headerLen];

			size_t readlen = fread(buf, sizeof(char), fileLen, file);

			if (readlen == 0) //(readlen != len)
			{
				delete[] buffer;
				//ERR("glsl shader file size" );
				return false;
			}

			// trick to zero all outside memory
			memset(&buffer[readlen + headerLen], 0, sizeof(char) * (fileLen + 1 - readlen));

			//glslang_program_add_source_text(program, stage, buffer, headerLen + fileLen + 1);

			delete[] buffer;
			return true;
		}

	public:
		MyShaderMOC()
			: ShaderBase(ShaderType::GLSL)
		{}
		virtual ~MyShaderMOC() override {}

		// resources

		bool LoadShaders(const char* vertex_file, const char* fragment_file) override
		{
			bool status = true;

			FILE* fp = nullptr;
			fopen_s(&fp, vertex_file, "r");
			if (fp)
			{
				// TODO: do a glslangValidator pass
				status |= ValidateShader(fp); //, validateProgram, glslang_stage_t::GLSLANG_STAGE_VERTEX);
				fclose(fp);
			}

			fopen_s(&fp, fragment_file, "r");
			if (fp)
			{
				status |= ValidateShader(fp); // , validateProgram, glslang_stage_t::GLSLANG_STAGE_FRAGMENT);
				fclose(fp);
			}

			std::string debuglog = execCmd("C:\\Work\\Temp\\glslang-main-windows-x64-Release\\bin\\glslangValidator.exe -l -g %s %s", vertex_file, fragment_file);

			if (strstr(debuglog.c_str(), "ERROR") != nullptr)
			{
				Logger::WriteMessage(debuglog.c_str());
				return false;
			}

			return status;
		}
		// reuse vertex program from teh referenceShader
		bool LoadShaders(ShaderBase* referenceShader, const char* fragment_file) override
		{
			return false;
		}

		bool ReCompileShaders(const char* vertex_file, const char* fragment_file) override
		{
			return false;
		}

		// use of it

		void Bind() const override
		{}
		void UnBind() const override
		{}

		// uniforms

		bool SetUniformUINT(const char* name, const int value) override
		{
			return false;
		}
		bool SetUniformFloat(const char* name, const float value) override
		{
			return false;
		}
		bool SetUniformVector(const char* name, const float x, const float y, const float z, const float w) override
		{
			return false;
		}
		bool SetUniformVector2f(const char* name, const float x, const float y) override
		{
			return false;
		}
		bool SetUniformMatrix33(const char* name, const float* m) override
		{
			return false;
		}
		bool SetUniformMatrix(const char* name, const float* m) override
		{
			return false;
		}

		int FindLocation(const char* name) const override { return -1; }

		void SetUniformUINT(const int location, const int value) override { }
		void SetUniformFloat(const int location, const float value) override {}
		void SetUniformVector(const int location, const float x, const float y, const float z, const float w) override {}
		void SetUniformVector4(const int location, const float* arr) override {}
		void SetUniformVector2f(const int location, const float x, const float y) override {}
		void SetUniformMatrix33(const int location, const float* m) override {}
		void SetUniformMatrix(const int location, const float* m) override {}

		void BindTexture(unsigned int target, const char* texname, unsigned int texid, int texunit) override {}
		void BindTexture(unsigned int target, const int location, unsigned int texid, int texunit) override {}
		void UnBindTexture(unsigned int target, int texunit) override {}
		void BindTextureRECT(const char* texname, unsigned int texid, int texunit) override {}
	};

	/// <summary>
	/// Test class
	/// </summary>
	TEST_CLASS(OpenMoBuUnitTests)
	{
	public:
		
		static ShaderBase* CreateMyShader(ShaderType shaderType) { return new MyShaderMOC(); }

		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			Logger::WriteMessage("In Class Initialize");
			//glewInit();
			InitShaderFactory();
			//glslang_initialize_process();
		}
		TEST_CLASS_CLEANUP(ClassCleanup)
		{
			Logger::WriteMessage("In Class Cleanup");
			FreeShaderFactory();
			//glslang_finalize_process();
		}

		TEST_METHOD(TestShaderLoad)
		{
			MyLogger logger;

			ShaderFactory::RegisterShaderType(ShaderType::GLSL, CreateMyShader);

			PostEffectFishEye* effectFishEye = new PostEffectFishEye(&logger);

			constexpr const char* shadersPath{ "C:\\Users\\Sergei\\Documents\\GitHub\\OpenMoBu\\Src\\Projects\\post_processing_effects" };

			const std::string vertexPath = std::string(shadersPath) + effectFishEye->GetVertexFname(0);
			const std::string fragmentPath = std::string(shadersPath) + effectFishEye->GetFragmentFname(0);
			const std::string uiPath = std::string(shadersPath) + effectFishEye->GetUIFName();

			bool success = effectFishEye->Load(0, vertexPath.c_str(), fragmentPath.c_str(), uiPath.c_str());

			Assert::IsTrue(success, L"Failed to load the post effect");
		}

		// TODO:
		TEST_METHOD(TestUICallback)
		{
			MyUICallback	uiCallback;
			MyLogger		logger;
			SViewInfo		viewInfo;

			PostEffectFishEye* effectFishEye = new PostEffectFishEye(&logger);

			constexpr const char* shadersPath{ "C:\\Users\\Sergei\\Documents\\GitHub\\OpenMoBu\\Src\\Projects\\post_processing_effects" };

			const std::string vertexPath = std::string(shadersPath) + effectFishEye->GetVertexFname(0);
			const std::string fragmentPath = std::string(shadersPath) + effectFishEye->GetFragmentFname(0);
			const std::string uiPath = std::string(shadersPath) + effectFishEye->GetUIFName();

			bool success = effectFishEye->Load(0, vertexPath.c_str(), fragmentPath.c_str(), uiPath.c_str());

			Assert::IsTrue(success, L"Failed to load the post effect");

			// 1 - create properties
			//  optional to have a property layout (categories, parent, etc.)
			// 2 - change property value
			// 3 - collect property values by effect
			// 4 - check that uniform has correct value equal to property value


		}
	};
}
