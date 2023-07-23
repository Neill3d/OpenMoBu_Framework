
#include "CppUnitTest.h"
#include "FileUtils.h"
#include "ReadFileScope.h"
#include "Logger.h"
#include <cstdarg>
#include "TextUtil.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OpenMobuUnitTests
{
	class CMyLogger : public CLoggerCallback
	{
	public:
		void Trace(const char* message, ...) override
		{
			memset(buffer, 0, sizeof(char) * 256);

			va_list argptr;
			va_start(argptr, message);
			vsprintf_s(buffer, sizeof(char) * 256, message, argptr);
			va_end(argptr);
		}

		char buffer[256];
	};

	/// <summary>
	/// File Classes and Utilities Tests
	/// </summary>
	TEST_CLASS(CoreUnitTests)
	{
	public:

		static constexpr char* FILE_NAME{ "test.txt" };

		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			FILE* fp = nullptr;
			fopen_s(&fp, FILE_NAME, "w");
			constexpr char* text{ "my test text" };
			fwrite(text, sizeof(char), strlen(text), fp);
			fclose(fp);
		}
		TEST_CLASS_CLEANUP(ClassCleanup)
		{
			std::remove(FILE_NAME);
		}

		TEST_METHOD(IsFileExistsTest)
		{
			Assert::IsTrue(NOpenMobu::IsFileExists(FILE_NAME));
			Assert::IsFalse(NOpenMobu::IsFileExists("test2.txt"));
		}

		TEST_METHOD(ReadFileScopeTest)
		{
			NOpenMobu::CReadFileScope readFile(FILE_NAME);

			Assert::AreEqual(12, readFile.GetLength());
			char* buffer = new char[13];
			memset(buffer, 0, sizeof(char) * 13);
			const int readed = readFile.Read(buffer);

			Assert::AreEqual(12, readed);
			Assert::AreEqual("my test text", buffer);
		}

		TEST_METHOD(LoggerTest)
		{
			CMyLogger myLogger;
			SetLogger(&myLogger);

			LOG_TRACE("my test msg");
			Assert::AreEqual("my test msg", myLogger.buffer);
		}

		TEST_METHOD(TextUtilTest)
		{
			NOpenMobu::CTokenizeText tokenizer("my test msg");

			Assert::AreEqual(3, tokenizer.GetCount());
			Assert::AreEqual(4, tokenizer.GetTokenLength(1));

			char buffer[5]{ 0 };
			tokenizer.GetTokenText(1, buffer);
			Assert::AreEqual("test", buffer);
		}
	};
}
