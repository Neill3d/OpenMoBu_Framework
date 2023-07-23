
/////////////////////////////////////////////////////////////////////////////////////////
//
// Licensed under the "New" BSD License. 
//		License page - https://github.com/Neill3d/OpenMoBu/blob/master/LICENSE
//
// GitHub repository - https://github.com/Neill3d/OpenMoBu
//
// Author Sergey Solokhin (Neill3d) 2014-2022
//  e-mail to: s@neill3d.com
//		www.neill3d.com
/////////////////////////////////////////////////////////////////////////////////////////

#include "FileUtils.h"

#include <windows.h>

namespace NOpenMobu
{
	bool IsFileExists(const char* filename)
	{
		WIN32_FIND_DATAA FindFileData;
		HANDLE hFind;

		hFind = FindFirstFileA(filename, &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		else
		{
			FindClose(hFind);
			return true;
		}
	}
}

////////////////////////////////////////////////////////////
/*
bool FindEffectLocation(const char *effect, FBString &out_path, FBString &out_fullname)
{
	FBSystem	&lSystem = FBSystem::TheOne();
	
	out_path = lSystem.UserConfigPath;
	out_fullname = out_path + "\\";
	out_fullname = out_fullname + effect;

	bool found = false;

	if ( IsFileExists(out_fullname) == true )
	{
		found = true;
	}
	else
	{
		
		FBStringList paths;
#ifndef ORSDK2013
		paths = lSystem.GetPluginPath();
#endif
		for (int i=0; i<paths.GetCount(); ++i)
		{
			out_path = paths[i];
			out_fullname = out_path + "\\";
			out_fullname = out_fullname + effect;

			if (IsFileExists(out_fullname) == true)
			{
				found = true;
				break;
			}
		}
	}

	return found;
}
*/
