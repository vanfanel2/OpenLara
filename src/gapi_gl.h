#ifndef H_GAPI_GL
#define H_GAPI_GL

#include "core.h"

#ifdef _OS_WIN
    #include <gl/GL.h>
    #include <gl/glext.h>
#elif _OS_ANDROID
    #include <dlfcn.h>

    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
    #define GL_CLAMP_TO_BORDER          0x812D
    #define GL_TEXTURE_BORDER_COLOR     0x1004

    #define GL_TEXTURE_COMPARE_MODE     0x884C
    #define GL_TEXTURE_COMPARE_FUNC     0x884D
    #define GL_COMPARE_REF_TO_TEXTURE   0x884E

    #define GL_RGBA16F                  0x881A
    #define GL_RGBA32F                  0x8814
    #define GL_HALF_FLOAT               0x140B

    #define GL_DEPTH_STENCIL            GL_DEPTH_STENCIL_OES
    #define GL_UNSIGNED_INT_24_8        GL_UNSIGNED_INT_24_8_OES

    #define PFNGLGENVERTEXARRAYSPROC     PFNGLGENVERTEXARRAYSOESPROC
    #define PFNGLDELETEVERTEXARRAYSPROC  PFNGLDELETEVERTEXARRAYSOESPROC
    #define PFNGLBINDVERTEXARRAYPROC     PFNGLBINDVERTEXARRAYOESPROC
    #define glGenVertexArrays            glGenVertexArraysOES
    #define glDeleteVertexArrays         glDeleteVertexArraysOES
    #define glBindVertexArray            glBindVertexArrayOES

    #define PFNGLGETPROGRAMBINARYPROC    PFNGLGETPROGRAMBINARYOESPROC
    #define PFNGLPROGRAMBINARYPROC       PFNGLPROGRAMBINARYOESPROC
    #define glGetProgramBinary           glGetProgramBinaryOES
    #define glProgramBinary              glProgramBinaryOES

    #define GL_PROGRAM_BINARY_LENGTH     GL_PROGRAM_BINARY_LENGTH_OES
#elif _OS_RPI
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
    #include <EGL/egl.h>
    #include <EGL/eglext.h>

    #define GL_CLAMP_TO_BORDER          0x812D
    #define GL_TEXTURE_BORDER_COLOR     0x1004

    #define GL_TEXTURE_COMPARE_MODE     0x884C
    #define GL_TEXTURE_COMPARE_FUNC     0x884D
    #define GL_COMPARE_REF_TO_TEXTURE   0x884E

    #undef  GL_RGBA32F
    #undef  GL_RGBA16F
    #undef  GL_HALF_FLOAT

    #define GL_RGBA32F      GL_RGBA
    #define GL_RGBA16F      GL_RGBA
    #define GL_HALF_FLOAT   GL_HALF_FLOAT_OES

    #define GL_DEPTH_STENCIL        GL_DEPTH_STENCIL_OES
    #define GL_UNSIGNED_INT_24_8    GL_UNSIGNED_INT_24_8_OES
    
    #define glGenVertexArrays(...)
    #define glDeleteVertexArrays(...)
    #define glBindVertexArray(...)
    
    #define GL_PROGRAM_BINARY_LENGTH     GL_PROGRAM_BINARY_LENGTH_OES
    #define glGetProgramBinary(...)
    #define glProgramBinary(...)
    
    extern EGLDisplay display;
#elif _OS_LINUX
    #include <GL/gl.h>
    #include <GL/glext.h>
    #include <GL/glx.h>
#elif __APPLE__
    #ifdef _OS_IOS
        #include <OpenGLES/ES2/gl.h>
        #include <OpenGLES/ES2/glext.h>
        #include <OpenGLES/ES3/glext.h>

        #define PFNGLGENVERTEXARRAYSPROC    PFNGLGENVERTEXARRAYSOESPROC
        #define PFNGLDELETEVERTEXARRAYSPROC PFNGLDELETEVERTEXARRAYSOESPROC
        #define PFNGLBINDVERTEXARRAYPROC    PFNGLBINDVERTEXARRAYOESPROC
        #define glGenVertexArrays           glGenVertexArraysOES
        #define glDeleteVertexArrays        glDeleteVertexArraysOES
        #define glBindVertexArray           glBindVertexArrayOES

        #define GL_CLAMP_TO_BORDER          0x812D
        #define GL_TEXTURE_BORDER_COLOR     0x1004

        #define GL_TEXTURE_COMPARE_MODE     GL_TEXTURE_COMPARE_MODE_EXT
        #define GL_TEXTURE_COMPARE_FUNC     GL_TEXTURE_COMPARE_FUNC_EXT
        #define GL_COMPARE_REF_TO_TEXTURE   GL_COMPARE_REF_TO_TEXTURE_EXT
    #else
        #include <Carbon/Carbon.h>
        #include <AudioToolbox/AudioQueue.h>
        #include <OpenGL/OpenGL.h>
        #include <OpenGL/gl.h>
        #include <OpenGL/glext.h>
        #include <AGL/agl.h>

        #define GL_RGBA16F                  0x881A
        #define GL_RGBA32F                  0x8814
        #define GL_HALF_FLOAT               0x140B

        #define GL_RGB565                   GL_RGBA
        #define GL_TEXTURE_COMPARE_MODE     0x884C
        #define GL_TEXTURE_COMPARE_FUNC     0x884D
        #define GL_COMPARE_REF_TO_TEXTURE   0x884E

        #define glGenVertexArrays    glGenVertexArraysAPPLE
        #define glDeleteVertexArrays glDeleteVertexArraysAPPLE
        #define glBindVertexArray    glBindVertexArrayAPPLE

        #define GL_PROGRAM_BINARY_LENGTH 0
        #define glGetProgramBinary(...)  0
        #define glProgramBinary(...)     0
    #endif
#elif _OS_WEB
    #include <emscripten/emscripten.h>
    #include <emscripten/html5.h>
    #include <GLES3/gl3.h>
    #include <GLES3/gl2ext.h>

    #define GL_CLAMP_TO_BORDER          GL_CLAMP_TO_BORDER_EXT
    #define GL_TEXTURE_BORDER_COLOR     GL_TEXTURE_BORDER_COLOR_EXT

    #define glGetProgramBinary(...)
    #define glProgramBinary(...)
#endif

#if defined(_OS_WIN) || (defined(_OS_LINUX) && !defined(_OS_RPI)) || defined(_OS_ANDROID)

    #ifdef _OS_ANDROID
        #define GetProc(x) dlsym(libGL, x);
    #else
        void* GetProc(const char *name) {
            #ifdef _OS_WIN
                return (void*)wglGetProcAddress(name);
            #elif _OS_LINUX
                return (void*)glXGetProcAddress((GLubyte*)name);
            #elif _OS_RPI
                return (void*)eglGetProcAddress(name);
            #endif
        }
    #endif

    #define GetProcOGL(x) x=(decltype(x))GetProc(#x);

// Texture
    #ifdef _OS_WIN
        PFNGLACTIVETEXTUREPROC              glActiveTexture;
    #endif

// VSync
    #ifdef _OS_WIN
        typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);
        PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
    #elif _OS_LINUX
        typedef int (*PFNGLXSWAPINTERVALSGIPROC) (int interval);
        PFNGLXSWAPINTERVALSGIPROC glXSwapIntervalSGI;
    #endif

    #if defined(_OS_WIN) || defined(_OS_LINUX)
        PFNGLGENERATEMIPMAPPROC             glGenerateMipmap;
    // Profiling
        #ifdef PROFILE
            PFNGLOBJECTLABELPROC                glObjectLabel;
            PFNGLPUSHDEBUGGROUPPROC             glPushDebugGroup;
            PFNGLPOPDEBUGGROUPPROC              glPopDebugGroup;
            PFNGLGENQUERIESPROC                 glGenQueries;
            PFNGLDELETEQUERIESPROC              glDeleteQueries;
            PFNGLGETQUERYOBJECTIVPROC           glGetQueryObjectiv;
            PFNGLBEGINQUERYPROC                 glBeginQuery;
            PFNGLENDQUERYPROC                   glEndQuery;
        #endif
    // Shader
        PFNGLCREATEPROGRAMPROC              glCreateProgram;
        PFNGLDELETEPROGRAMPROC              glDeleteProgram;
        PFNGLLINKPROGRAMPROC                glLinkProgram;
        PFNGLUSEPROGRAMPROC                 glUseProgram;
        PFNGLGETPROGRAMINFOLOGPROC          glGetProgramInfoLog;
        PFNGLCREATESHADERPROC               glCreateShader;
        PFNGLDELETESHADERPROC               glDeleteShader;
        PFNGLSHADERSOURCEPROC               glShaderSource;
        PFNGLATTACHSHADERPROC               glAttachShader;
        PFNGLCOMPILESHADERPROC              glCompileShader;
        PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog;
        PFNGLGETUNIFORMLOCATIONPROC         glGetUniformLocation;
        PFNGLUNIFORM1IVPROC                 glUniform1iv;
        PFNGLUNIFORM1FVPROC                 glUniform1fv;
        PFNGLUNIFORM2FVPROC                 glUniform2fv;
        PFNGLUNIFORM3FVPROC                 glUniform3fv;
        PFNGLUNIFORM4FVPROC                 glUniform4fv;
        PFNGLUNIFORMMATRIX4FVPROC           glUniformMatrix4fv;
        PFNGLBINDATTRIBLOCATIONPROC         glBindAttribLocation;
        PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray;
        PFNGLDISABLEVERTEXATTRIBARRAYPROC   glDisableVertexAttribArray;
        PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer;
        PFNGLGETPROGRAMIVPROC               glGetProgramiv;
    // Render to texture
        PFNGLGENFRAMEBUFFERSPROC            glGenFramebuffers;
        PFNGLBINDFRAMEBUFFERPROC            glBindFramebuffer;
        PFNGLGENRENDERBUFFERSPROC           glGenRenderbuffers;
        PFNGLBINDRENDERBUFFERPROC           glBindRenderbuffer;
        PFNGLFRAMEBUFFERTEXTURE2DPROC       glFramebufferTexture2D;
        PFNGLFRAMEBUFFERRENDERBUFFERPROC    glFramebufferRenderbuffer;
        PFNGLRENDERBUFFERSTORAGEPROC        glRenderbufferStorage;
        PFNGLCHECKFRAMEBUFFERSTATUSPROC     glCheckFramebufferStatus;
        PFNGLDELETEFRAMEBUFFERSPROC         glDeleteFramebuffers;
        PFNGLDELETERENDERBUFFERSPROC        glDeleteRenderbuffers;
    // Mesh
        PFNGLGENBUFFERSARBPROC              glGenBuffers;
        PFNGLDELETEBUFFERSARBPROC           glDeleteBuffers;
        PFNGLBINDBUFFERARBPROC              glBindBuffer;
        PFNGLBUFFERDATAARBPROC              glBufferData;
        PFNGLBUFFERSUBDATAARBPROC           glBufferSubData;
    #endif

    PFNGLGENVERTEXARRAYSPROC            glGenVertexArrays;
    PFNGLDELETEVERTEXARRAYSPROC         glDeleteVertexArrays;
    PFNGLBINDVERTEXARRAYPROC            glBindVertexArray;
    PFNGLGETPROGRAMBINARYPROC           glGetProgramBinary;
    PFNGLPROGRAMBINARYPROC              glProgramBinary;
#endif

#if defined(_GAPI_GLES)
    PFNGLDISCARDFRAMEBUFFEREXTPROC      glDiscardFramebufferEXT;
#endif


#ifdef PROFILE
   //#define USE_CV_MARKERS

   #ifdef USE_CV_MARKERS
       #include <libs/cvmarkers/cvmarkersobj.h>  
       using namespace Concurrency::diagnostic;

       marker_series *series[256];
       int seriesIndex;
   #endif

    struct Marker {
        #ifdef USE_CV_MARKERS
            span *cvSpan;
        #endif

        Marker(const char *title) {
            if (Core::support.profMarker) glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, title);
            #ifdef USE_CV_MARKERS
                marker_series *&s = series[seriesIndex];
                if (s == NULL) {
                   char seriesTitle[64];
                   sprintf(seriesTitle, "events - %d", seriesIndex);
                   s = new marker_series(seriesTitle);
                }
                cvSpan = new span(*s, normal_importance, _T(title));
                seriesIndex++;
            #endif
        }

        ~Marker() {
            if (Core::support.profMarker) glPopDebugGroup();
            #ifdef USE_CV_MARKERS
                delete cvSpan;
                seriesIndex--;
            #endif
        }

        static void setLabel(GLenum id, GLuint name, const char *label) {
            if (Core::support.profMarker) glObjectLabel(id, name, -1, label);
        }
    };

    struct Timing {
        GLuint  ID;
        int     &result;

        Timing(int &result) : result(result) {
            if (!Core::support.profTiming) return;
            glGenQueries(1, &ID);
            glBeginQuery(GL_TIME_ELAPSED, ID);
        }

        ~Timing() {
            if (!Core::support.profTiming) return;
            glEndQuery(GL_TIME_ELAPSED);
            glGetQueryObjectiv(ID, GL_QUERY_RESULT, (GLint*)&result);
            glDeleteQueries(1, &ID);
        }
    };

    #define PROFILE_MARKER(title)               Marker marker(title)
    #define PROFILE_LABEL(id, name, label)      Marker::setLabel(GL_##id, name, label)
    #define PROFILE_TIMING(result)              Timing timing(result)
#else
    #define PROFILE_MARKER(title)
    #define PROFILE_LABEL(id, name, label)
    #define PROFILE_TIMING(time)
#endif


namespace GAPI {

#ifndef FFP
    const char SHADER_BASE[] =
        #include "shaders/shader.glsl"
    ;

    const char SHADER_WATER[] =
        #include "shaders/water.glsl"
    ;

    const char SHADER_FILTER[] =
        #include "shaders/filter.glsl"
    ;

    const char SHADER_GUI[] =
        #include "shaders/gui.glsl"
    ;
#endif

    using namespace Core;

    typedef ::Vertex Vertex;

    int cullMode, blendMode;

    struct Texture {
        enum Option { CUBEMAP = 1, MIPMAPS = 2, NEAREST = 4 };

        uint32  ID;
        int     width, height, origWidth, origHeight;
        Format  format;
        uint32  opt;
    };


    GLuint FBO, defaultFBO;
    struct RenderTargetCache {
        int count;
        struct Item {
            GLuint  ID;
            int     width;
            int     height;
        } items[MAX_RENDER_BUFFERS];
    } rtCache[2];

    bool extSupport(const char *str, const char *ext) {
        return strstr(str, ext) != NULL;
    }

    void init() {
        memset(rtCache, 0, sizeof(rtCache));

        #ifdef _OS_ANDROID
            void *libGL = dlopen("libGLESv2.so", RTLD_LAZY);
        #endif

        #if defined(_OS_WIN) || (defined(_OS_LINUX) && !defined(_OS_RPI)) || defined(_OS_ANDROID)
            #ifdef _OS_WIN
                GetProcOGL(glActiveTexture);
            #endif

            #ifdef _OS_WIN
                GetProcOGL(wglSwapIntervalEXT);
            #elif _OS_LINUX
                GetProcOGL(glXSwapIntervalSGI);
            #endif

            #if defined(_OS_WIN) || defined(_OS_LINUX)
                GetProcOGL(glGenerateMipmap);

                #ifdef PROFILE
                    GetProcOGL(glObjectLabel);
                    GetProcOGL(glPushDebugGroup);
                    GetProcOGL(glPopDebugGroup);
                    GetProcOGL(glGenQueries);
                    GetProcOGL(glDeleteQueries);
                    GetProcOGL(glGetQueryObjectiv);
                    GetProcOGL(glBeginQuery);
                    GetProcOGL(glEndQuery);
                #endif

                GetProcOGL(glCreateProgram);
                GetProcOGL(glDeleteProgram);
                GetProcOGL(glLinkProgram);
                GetProcOGL(glUseProgram);
                GetProcOGL(glGetProgramInfoLog);
                GetProcOGL(glCreateShader);
                GetProcOGL(glDeleteShader);
                GetProcOGL(glShaderSource);
                GetProcOGL(glAttachShader);
                GetProcOGL(glCompileShader);
                GetProcOGL(glGetShaderInfoLog);
                GetProcOGL(glGetUniformLocation);
                GetProcOGL(glUniform1iv);
                GetProcOGL(glUniform1fv);
                GetProcOGL(glUniform2fv);
                GetProcOGL(glUniform3fv);
                GetProcOGL(glUniform4fv);
                GetProcOGL(glUniformMatrix4fv);
                GetProcOGL(glBindAttribLocation);
                GetProcOGL(glEnableVertexAttribArray);
                GetProcOGL(glDisableVertexAttribArray);
                GetProcOGL(glVertexAttribPointer);
                GetProcOGL(glGetProgramiv);

                GetProcOGL(glGenFramebuffers);
                GetProcOGL(glBindFramebuffer);
                GetProcOGL(glGenRenderbuffers);
                GetProcOGL(glBindRenderbuffer);
                GetProcOGL(glFramebufferTexture2D);
                GetProcOGL(glFramebufferRenderbuffer);
                GetProcOGL(glRenderbufferStorage);
                GetProcOGL(glCheckFramebufferStatus);
                GetProcOGL(glDeleteFramebuffers);
                GetProcOGL(glDeleteRenderbuffers);

                GetProcOGL(glGenBuffers);
                GetProcOGL(glDeleteBuffers);
                GetProcOGL(glBindBuffer);
                GetProcOGL(glBufferData);
                GetProcOGL(glBufferSubData);
            #endif

            #ifdef _GAPI_GLES
                GetProcOGL(glDiscardFramebufferEXT);
            #endif

            GetProcOGL(glGenVertexArrays);
            GetProcOGL(glDeleteVertexArrays);
            GetProcOGL(glBindVertexArray);
            GetProcOGL(glGetProgramBinary);
            GetProcOGL(glProgramBinary);
        #endif

        LOG("Vendor   : %s\n", (char*)glGetString(GL_VENDOR));
        LOG("Renderer : %s\n", (char*)glGetString(GL_RENDERER));
        LOG("Version  : %s\n", (char*)glGetString(GL_VERSION));

        char *ext = (char*)glGetString(GL_EXTENSIONS);
/*
        if (ext != NULL) {
            char buf[255];
            int len = strlen(ext);
            int start = 0;
            for (int i = 0; i < len; i++)
                if (ext[i] == ' ' || (i == len - 1)) {
                    memcpy(buf, &ext[start], i - start);
                    buf[i - start] = 0;
                    LOG("%s\n", buf);
                    start = i + 1;
                }
        }
*/

    #ifdef FFP
        support.maxAniso       = 1;
        support.maxVectors     = 0;
        support.shaderBinary   = false;
        support.VAO            = false;
        support.depthTexture   = false;
        support.shadowSampler  = false;
        support.discardFrame   = false;
        support.texNPOT        = false;
        support.texRG          = false;
        support.texBorder      = false;
        support.maxAniso       = false;
        support.colorFloat     = false;
        support.colorHalf      = false;
        support.texFloatLinear = false;
        support.texFloat       = false;
        support.texHalfLinear  = false;
        support.texHalf        = false;
    #else
        support.shaderBinary   = extSupport(ext, "_program_binary");
        support.VAO            = extSupport(ext, "_vertex_array_object");
        support.depthTexture   = extSupport(ext, "_depth_texture");
        support.shadowSampler  = support.depthTexture && (extSupport(ext, "_shadow_samplers") || extSupport(ext, "GL_ARB_shadow"));
        support.discardFrame   = extSupport(ext, "_discard_framebuffer");
        support.texNPOT        = extSupport(ext, "_texture_npot") || extSupport(ext, "_texture_non_power_of_two");
        support.texRG          = extSupport(ext, "_texture_rg ");   // hope that isn't last extension in string ;)
        support.texBorder      = extSupport(ext, "_texture_border_clamp");
        support.maxAniso       = extSupport(ext, "_texture_filter_anisotropic");
        support.colorFloat     = extSupport(ext, "_color_buffer_float");
        support.colorHalf      = extSupport(ext, "_color_buffer_half_float") || extSupport(ext, "GL_ARB_half_float_pixel");
        support.texFloatLinear = support.colorFloat || extSupport(ext, "GL_ARB_texture_float") || extSupport(ext, "_texture_float_linear");
        support.texFloat       = support.texFloatLinear || extSupport(ext, "_texture_float");
        support.texHalfLinear  = support.colorHalf || extSupport(ext, "GL_ARB_texture_float") || extSupport(ext, "_texture_half_float_linear") || extSupport(ext, "_color_buffer_half_float");
        support.texHalf        = support.texHalfLinear || extSupport(ext, "_texture_half_float");

        #ifdef PROFILE
            support.profMarker     = extSupport(ext, "_KHR_debug");
            support.profTiming     = extSupport(ext, "_timer_query");
        #endif

        if (support.maxAniso)
            glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &support.maxAniso);
        #ifdef _GAPI_GLES
            glGetIntegerv(GL_MAX_VARYING_VECTORS, &support.maxVectors);
        #else
            support.maxVectors = 16;
        #endif
    #endif

        glEnable(GL_SCISSOR_TEST);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&defaultFBO);
        glGenFramebuffers(1, &FBO);
        glDepthFunc(GL_LEQUAL);

    #ifdef FFP
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
            
        glAlphaFunc(GL_GREATER, 0.5f);

        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        glScalef(1.0f / 32767.0f, 1.0f / 32767.0f, 1.0f / 32767.0f);

        glClearColor(0, 0, 0, 0);
    #endif
    }

    void deinit() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &FBO);

        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        for (int b = 0; b < 2; b++)
            for (int i = 0; i < rtCache[b].count; i++)
                glDeleteRenderbuffers(1, &rtCache[b].items[i].ID);
    }

    void resetState() {
        if (glBindVertexArray)
            glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glUseProgram(0);
    }

    int cacheRenderTarget(bool depth, int width, int height) {
        RenderTargetCache &cache = rtCache[depth];

        for (int i = 0; i < cache.count; i++)
            if (cache.items[i].width == width && cache.items[i].height == height)
                return i;

        ASSERT(cache.count < MAX_RENDER_BUFFERS);

        RenderTargetCache::Item &item = cache.items[cache.count];
        item.width  = width;
        item.height = height;

        glGenRenderbuffers(1, &item.ID);
        glBindRenderbuffer(GL_RENDERBUFFER, item.ID);
        glRenderbufferStorage(GL_RENDERBUFFER, depth ? GL_RGB565 : GL_DEPTH_COMPONENT16, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        return cache.count++;
    }

    void bindTarget(Texture *target, int face) {
        if (!target) { // may be a null
            glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
        } else {
            GLenum texTarget = GL_TEXTURE_2D;
            if (target->opt & Texture::CUBEMAP) 
                texTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X + face;

            bool depth = target->format == FMT_DEPTH || target->format == FMT_SHADOW;

            int rtIndex = cacheRenderTarget(depth, target->width, target->height);

            glBindFramebuffer(GL_FRAMEBUFFER, FBO);
            glFramebufferTexture2D    (GL_FRAMEBUFFER, depth ? GL_DEPTH_ATTACHMENT  : GL_COLOR_ATTACHMENT0, texTarget,       target->ID, 0);
            glFramebufferRenderbuffer (GL_FRAMEBUFFER, depth ? GL_COLOR_ATTACHMENT0 : GL_DEPTH_ATTACHMENT,  GL_RENDERBUFFER, rtCache[depth].items[rtIndex].ID);
        }
    }

    void discardTarget(bool color, bool depth) {
    #ifdef _GAPI_GLES
        if (Core::support.discardFrame) {
            int count = 0;
            GLenum discard[2];
            if (color) discard[count++] = Core::active.target ? GL_COLOR_ATTACHMENT0 : GL_COLOR_EXT;
            if (depth) discard[count++] = Core::active.target ? GL_DEPTH_ATTACHMENT  : GL_DEPTH_EXT;
            if (count)
                glDiscardFramebufferEXT(GL_FRAMEBUFFER, count, discard);
        }
    #endif
    }

    void copyTarget(Texture *dst, int xOffset, int yOffset, int x, int y, int width, int height) {
        Core::active.textures[0] = NULL;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, dst->ID);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, x, y, width, height);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void setVSync(bool enable) {
        #ifdef _OS_WIN
            if (wglSwapIntervalEXT) wglSwapIntervalEXT(enable ? 1 : 0);
        #elif _OS_LINUX
            if (glXSwapIntervalSGI) glXSwapIntervalSGI(enable ? 1 : 0);
        #elif _OS_RPI
            eglSwapInterval(display, enable ? 1 : 0);
        #endif
    }

    void waitVBlank() {}

    void clear(bool color, bool depth) {
        uint32 mask = (color ? GL_COLOR_BUFFER_BIT : 0) | (depth ? GL_DEPTH_BUFFER_BIT : 0);
        if (mask) glClear(mask);
    }

    void setClearColor(const vec4 &color) {
        glClearColor(color.x, color.y, color.z, color.w);
    }

    void setViewport(int x, int y, int w, int h) {
        glViewport(x, y, w, h);
        glScissor(int(viewport.x), int(viewport.y), int(viewport.z), int(viewport.w));
    }

    void setDepthTest(bool enable) {
        if (enable)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
    }

    void setDepthWrite(bool enable) {
        glDepthMask(enable ? GL_TRUE : GL_FALSE);
    }

    void setColorWrite(bool r, bool g, bool b, bool a) {
        glColorMask(r, g, b, a);
    }

    void setAlphaTest(bool enable) {
        if (enable)
            glEnable(GL_ALPHA_TEST);
        else
            glDisable(GL_ALPHA_TEST);
    }

    void setCullMode(int rsMask) {
        cullMode = rsMask;
        switch (rsMask) {
            case RS_CULL_BACK  : glCullFace(GL_BACK);     break;
            case RS_CULL_FRONT : glCullFace(GL_FRONT);    break;
            default            : glDisable(GL_CULL_FACE); return;
        }
        glEnable(GL_CULL_FACE);
    }

    void setBlendMode(int rsMask) {
        blendMode = rsMask;
        switch (rsMask) {
            case RS_BLEND_ALPHA   : glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
            case RS_BLEND_ADD     : glBlendFunc(GL_ONE, GL_ONE);                       break;
            case RS_BLEND_MULT    : glBlendFunc(GL_DST_COLOR, GL_ZERO);                break;
            case RS_BLEND_PREMULT : glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);       break;
            default               : glDisable(GL_BLEND); return;
        }
        glEnable(GL_BLEND);
    }

    void setViewProj(const mat4 &mView, const mat4 &mProj) {
    #ifdef FFP
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf((float*)&mProj);
    #endif
    }

    void DIP(int iStart, int iCount, Index *iBuffer) {
    #ifdef FFP
        mat4 m = mView * mModel;
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf((GLfloat*)&m);
    #endif

        glDrawElements(GL_TRIANGLES, iCount, GL_UNSIGNED_SHORT, iBuffer + iStart);
    }

    vec4 copyPixel(int x, int y) {
        ubyte4 c;
        glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &c);
        return vec4(float(c.x), float(c.y), float(c.z), float(c.w)) * (1.0f / 255.0f);
    }

    void initPSO(PSO *pso) {
        ASSERT(pso);
        ASSERT(pso && pso->data == NULL);
        pso->data = &pso;
    }

    void deinitPSO(PSO *pso) {
        ASSERT(pso);
        ASSERT(pso->data != NULL);
        pso->data = NULL;
    }

    void bindPSO(const PSO *pso) {
        ASSERT(pso);
        ASSERT(pso->data != NULL);

        uint32 state = pso->renderState;
        uint32 mask  = mask;

        if (Core::active.pso)
            mask ^= Core::active.pso->renderState;

        if (!Core::active.pso || Core::active.pso->clearColor != pso->clearColor)
            setClearColor(pso->clearColor);

        if (mask & RS_DEPTH_TEST)
            setDepthTest((state & RS_DEPTH_TEST) != 0);
        
        if (mask & RS_DEPTH_WRITE)
            setDepthWrite((state & RS_DEPTH_WRITE) != 0);

        if (mask & RS_COLOR_WRITE)
            setColorWrite((state & RS_COLOR_WRITE_R) != 0, (state & RS_COLOR_WRITE_G) != 0, (state & RS_COLOR_WRITE_B) != 0, (state & RS_COLOR_WRITE_A) != 0);

        if (mask & RS_CULL)
            setCullMode(state & RS_CULL);

        if (mask & RS_BLEND)
            setBlendMode(state & RS_BLEND);

    #ifdef FFP
        if (mask & RS_DISCARD)
            setAlphaTest((state & RS_DISCARD) != 0);
    #endif
    }
}

#endif