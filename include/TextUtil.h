
/////////////////////////////////////////////////////////////////////////////////////////
//
// Licensed under the "New" BSD License. 
//		License page - https://github.com/Neill3d/MoBu/blob/master/LICENSE
//
// GitHub repository - https://github.com/Neill3d/MoBu
//
// Author Sergey Solohin (Neill3d) 2014
//  e-mail to: s@neill3d.com
//		www.neill3d.com
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef TextUtil_h_
#define TextUtil_h_

//#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

namespace NOpenMobu
{
	/*
	* A helper class to split string into tokens with any additional allocations
	*/
	class CTokenizeText
	{
	public:
		/// <summary>
		/// a constructor - initialize with a string to be tokenized
		/// </summary>
		/// <param name="s"></param>
		CTokenizeText(const char* s);

		/// <summary>
		/// get a total number of calculated tokens
		///  based on a defined DELIMETER or end of line \n
		/// </summary>
		/// <returns></returns>
		int GetCount() const { return m_Count; }

		/// <summary>
		/// return number of chars in a index's token
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		int GetTokenLength(const int index) const;

		/// <summary>
		/// copy text from a index's token into a given buffer
		///  buffer must have length greater then token length
		///  in order to inser last 0 simbol there
		/// </summary>
		/// <param name="index"></param>
		/// <param name="dstBuffer"></param>
		void GetTokenText(const int index, char* dstBuffer);

		/// <summary>
		/// allocate a new char on heap and copy text into it
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		char* CutToken(const int index);

		static constexpr char DELIMETER{ ' ' };

	private:
		const char* m_Text;
		int		m_TextPos;
		int		m_TextM;
		int		m_Count;
	};

	// TODO: the function is using a big static buffer, is it really needed any more ?
	//char* TranslateString(char* buf);
}


#endif
