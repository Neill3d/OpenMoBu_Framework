#pragma once

#include "libraryMain.h"
#include <GL\glew.h>

namespace NOpenMobu 
{
    /// <summary>
    /// \brief wrapper around generated OGL vertex array object
    ///  The scope of generation is defined by the life of the class instance
    ///  generate vertex arrays on constructor and delete array on destructor
    ///  so it have to be called inside the active OGL context
    /// </summary>
    class GRAPHICS_LIBRARY_API CVertexArray
    {
        GLuint  id;

    public:
        //!< a constructor
        CVertexArray()
        {
            glGenVertexArrays(1, &id);
        }
        //!< a destructor
        ~CVertexArray()
        {
            glDeleteVertexArrays(1, &id);
        }

        bool    IsOk() const
        {
            return glIsVertexArray(id) == GL_TRUE;
        }

        void    Bind()
        {
            glBindVertexArray(id);
        }

        void    UnBind()
        {
            glBindVertexArray(0);
        }
    };
}


