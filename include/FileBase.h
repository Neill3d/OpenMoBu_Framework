#pragma once

namespace NOpenMobu
{
	/**
	 * Interface for the data access (by file, memory stream or archive)
	 */
	class CFileBase
	{
	public:
		virtual ~CFileBase() {}

		/// <summary>
		/// get length in bytes for the entry
		/// </summary>
		/// <returns></returns>
		virtual int GetLength() const = 0;

		/// <summary>
		/// read data from the entry into the pre - allocated buffer with possible defined read offset
		/// </summary>
		/// <param name="buffer">allocated buffer to read data into</param>
		/// <param name="readLength">number of bytes to read</param>
		/// <returns>number of readed bytes</returns>
		virtual int Read(const char* buffer, const int readLength) const = 0;
		
	};
};