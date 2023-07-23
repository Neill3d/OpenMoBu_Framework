#include "ReadFileScope.h"

namespace NOpenMobu
{
    CReadFileScope::CReadFileScope(const char* filename)
    {
        m_lastError = fopen_s(&m_fp, filename, "r");
    }

    CReadFileScope::~CReadFileScope()
    {
        if (m_fp)
            fclose(m_fp);
    }

    int CReadFileScope::GetLength() const
    {
        fseek(m_fp, 0, SEEK_END);
        const size_t fileLen = ftell(m_fp);
        fseek(m_fp, 0, SEEK_SET);
        return static_cast<int>(fileLen);
    }

    int CReadFileScope::Read(const char* buffer, const int readLength) const
    {
        if (m_fp == nullptr)
            return 0;

        const int len = (readLength > 0) ? readLength : GetLength();

        const size_t readlen = fread((void*)buffer, sizeof(char), len, m_fp);
        return static_cast<int>(readlen);
    }
}