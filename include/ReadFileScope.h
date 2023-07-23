#pragma once

#include "FileBase.h"
#include <cstdio>

namespace NOpenMobu
{
	/// <summary>
	/// class to read file in a local scope
	///  the file will be closed automatically on class destructor
	/// </summary>
	class CReadFileScope : public CFileBase
	{
	public:
		CReadFileScope(const char* filename);
		virtual ~CReadFileScope();

		int GetLength() const override;
		
		/// <summary>
		/// in case readLength is zero, we read full file length
		/// </summary>
		/// <param name="buffer"></param>
		/// <param name="readLength"></param>
		/// <returns>number of read bytes</returns>
		int Read(const char* buffer, const int readLength=0) const override;

	protected:
		FILE*	m_fp;
		errno_t m_lastError;
	};
}