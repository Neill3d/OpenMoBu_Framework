//
// Return floating-point texture format with given precision
//

#include "fpTexture.h"
#include <GL\glew.h>


unsigned int	fpRgbaFormatWithPrecision ( int bits )
{
#ifdef	MACOSX
	if ( isExtensionSupported ( "GL_APPLE_float_pixels" ) )
		return (bits == 16 ? GL_RGBA_FLOAT16_APPLE : GL_RGBA_FLOAT32_APPLE);
	else
		return 0;
#else
	if ( glewIsExtensionSupported ( "GL_ARB_texture_float" ) )
		return (bits == 16 ? GL_RGBA16F_ARB : GL_RGBA32F_ARB);
	else
		if ( glewIsExtensionSupported ( "GL_NV_float_buffer" ) )
		return (bits == 16 ? GL_FLOAT_RGBA16_NV : GL_FLOAT_RGBA32_NV );
	/*
	else
	if ( extgl_ExtensionSupported ( "GL_ATI_texture_float" ) )
		return (bits == 16 ? GL_RGBA_FLOAT16_ATI : GL_RGBA_FLOAT32_ATI);
		*/
#endif
	return 0;
}

unsigned int	fpAlphaFormatWithPrecision ( int bits )
{
#ifdef	MACOSX
	if ( isExtensionSupported ( "GL_APPLE_float_pixels" ) )
		return (bits == 16 ? GL_ALPHA_FLOAT16_APPLE : GL_ALPHA_FLOAT32_APPLE);
	else
		return 0;
#else
	if ( glewIsExtensionSupported ( "GL_ARB_texture_float" ) )
		return (bits == 16 ? GL_ALPHA16F_ARB : GL_ALPHA32F_ARB);
	else
	if ( glewIsExtensionSupported ( "GL_NV_float_buffer" ) )
		return (bits == 16 ? GL_FLOAT_R16_NV : GL_FLOAT_R32_NV );
	/*
	else
	if ( extgl_ExtensionSupported ( "GL_ATI_texture_float" ) )
		return (bits == 16 ? GL_ALPHA_FLOAT16_ATI : GL_ALPHA_FLOAT32_ATI);
		*/
#endif
	return 0;
}
