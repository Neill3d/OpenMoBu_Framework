#pragma once

#ifdef OPENMOBU_GRAPHICS_LIBRARY_EXPORT
#define GRAPHICS_LIBRARY_API __declspec(dllexport)
#else
#define GRAPHICS_LIBRARY_API __declspec(dllimport)
#endif