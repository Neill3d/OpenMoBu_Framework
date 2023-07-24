Shared Graphics Library                         {#mainpage}
============

 The library with helper classes to operate with OpenGL graphics
 This contains classes to work with glsl shaders, vertex/fragment/compute
 check gl error, asserts, shader factory

 ShaderBase ? Shader
  - GLSLShader
  - GLSLComputeShader

  FramebufferBase ? Framebuffer
  - GLFrameBuffer

  VertexArray
  - GLVertexArray

  ShaderFactor - create a new shader by type, but also handle context reload with managing to reload shaders

  utils:
  - fptexture
  - checkglerror
  - OGLUtils

  public interface of graphics, not dependent on API
   implementation is OGL based
  I don't have a real goal to fully decouple the ogl backend from the interface


  All shader code is not reading file directly, but using a SharedMotionLibrary FileBase class
  to get access to the file