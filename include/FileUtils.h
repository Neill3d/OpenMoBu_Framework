
#pragma once

/////////////////////////////////////////////////////////////////////////////////////////
//
// Licensed under the "New" BSD License. 
//		License page - https://github.com/Neill3d/OpenMoBu/blob/master/LICENSE
//
// GitHub repository - https://github.com/Neill3d/OpenMoBu
//
// Author Sergey Solokhin (Neill3d) 2014-2022
//  e-mail to: neill3d@gmail.com
//		
/////////////////////////////////////////////////////////////////////////////////////////

namespace NOpenMobu
{
	/// <summary>
	/// a simple helper method to check if file exists
	///  This is a Windows only implementation
	/// </summary>
	/// <param name="filename">path to a file</param>
	/// <returns>true if file is exist</returns>
	bool IsFileExists(const char* filename);
}

//
// search first of all in mobu config folder, then in all plugins folders
//
//bool FindEffectLocation(const char *effect, FBString &out_path, FBString &out_fullname);




