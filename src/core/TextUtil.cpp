
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

#include "TextUtil.h"
#include <string>

using namespace NOpenMobu;

CTokenizeText::CTokenizeText(const char *s)
{
	m_Text = s;

	m_TextPos = 0;
	m_TextM = 0;
	m_Count = 0;
	
	if (s)
	{
		int res = 0;
		const int len = static_cast<int>(strlen(s));
		bool text = false;

		for (int i = 0; i < len; i++) 
		{
			if (s[i] == DELIMETER || s[i] == '\n')
			{
				if (text) { res++; text = false; }
			}
			else text = true;
		}

		if (text) res++;
		m_Count = res;
	}
}


// n from 1 to ...
int CTokenizeText::GetTokenLength(const int index) const
{
	int l = 0;
	int res=0;
	const int len = static_cast<int>(strlen(m_Text));
	bool text=false;
	
	for (int i=0; i<len; i++) 
	{
		if (m_Text[i] == DELIMETER || m_Text[i]=='\n')
		{
			if (text) { res++; text=false; }
		} else {
			text = true;
			
			if (res == index) l++;
		}
	}

	return l;
}

void CTokenizeText::GetTokenText(const int index, char* dstBuffer)
{
	int i = m_TextPos;
	int res = 0;
	int	count = 0;

	for ( ; i<strlen(m_Text); i++)
	{
		if (m_TextM > index) break;

		if (m_Text[i] != DELIMETER) {
			if (m_TextM == index) dstBuffer[count++] = m_Text[i];
			res=1;
		} else if (res == 1) {
				m_TextM++;
				res=0;
			}
	}
	m_TextPos = i;
	dstBuffer[count] = '\0';
}

char* CTokenizeText::CutToken(int index)
{
	int l = GetTokenLength(index);
	int res=0;
	int len = static_cast<int>(strlen(m_Text));
	bool text=false;
	char *part = new char[len-l];

	l=0;

	for (int i=0; i<len; i++) {
		if (m_Text[i] == DELIMETER) {
			if (text) { text=false; res++; part[l++]=m_Text[i];}
		} else {
			text = true;
			
			if (res != (index-1)) 
				part[l++]=m_Text[i];
		}
	}

	return part;
}
/*
char* SharedMotionLibrary::TranslateString (char *buf)
{
	static	char	buf2[32768];
	int		i, l;
	char	*out;

	l = static_cast<int>(strlen(buf));
	out = buf2;
	for (i=0 ; i<l ; i++)
	{
		if (buf[i] == '\n')
		{
			*out++ = '\r';
			*out++ = '\n';
		}
		else
			*out++ = buf[i];
	}
	*out++ = 0;

	return buf2;
}
*/