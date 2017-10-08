#ifndef TJH_DRAW_H
#define TJH_DRAW_H

////// UNLICENSE ///////////////////////////////////////////////////////////////
//
// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org/>

////// DEPENDENCIES ////////////////////////////////////////////////////////////
//
// This single header library relies on the following:
//
// Programable pipline OpenGL. You can load all your OpenGL functions with a
// library like glew.h. See here for more http://glew.sourceforge.net/

////// README //////////////////////////////////////////////////////////////////
//
// USAGE:
//
//  1) #define TJH_DRAW_IMPLEMENTATION then #include this file in *ONE*
//  .cpp file in your project.
//
//  2) Call `Draw::init()` somewhere after you have setup your OpenGL context.
//  Beware this library will not setup an OpenGL context for you! You should
//  create one yourself, or use my handy TJH_WINDOW library if you like :)
//  
//  3) Call `Draw::shutdown()` somewhere before you have destroyed your OpenGL
//  context.
//
//  4) #include this file in all the files that want to draw stuff
//
//  5) Call `Draw::quad()` and such to  draw the things, then call `Draw::flush()`
//  as the last thing you do to ensure everything is written
//

////// LIBRARY OPTIONS /////////////////////////////////////////////////////////
//
// You can modify the following #defines to enable or disable additional library
// features.
//

// If defined this file will include SDL.h
#ifndef TJH_DRAW_INCLUDE_SDL
#define TJH_DRAW_INCLUDE_SDL 1
#endif

// If defined this file will include glew.h
#ifndef TJH_DRAW_INCLUDE_GLEW
#define TJH_DRAW_INCLUDE_GLEW 1
#endif

// IMPORTANT: If you want this to include glew for your, you will need to set these correctly
#ifndef TJH_DRAW_SDL_H_LOCATION
#define TJH_DRAW_SDL_H_LOCATION <SDL2/SDL.h>
#endif

#ifndef TJH_DRAW_GLEW_H_LOCATION
#define TJH_DRAW_GLEW_H_LOCATION <GL/glew.h>
#endif

// Change this to customise the namespace for this library
#ifndef TJH_DRAW_NAMESPACE
#define TJH_DRAW_NAMESPACE draw
#endif

// Change this to use a custom printf like function for your platform, for example SDL_Log
#ifndef TJH_DRAW_PRINTF
#define TJH_DRAW_PRINTF printf
#endif

////// TODO ////////////////////////////////////////////////////////////////////
//
//  - convert line() to use triangles, optional settable width
//  - make it possible to pass a pointer to a custom ortho matrix
//  - test setOrthoMatrix x_offset and y_offset
//      - i implemented something along those lines but i don't know if it actually works
//  - solid shapes have a line draw mode
//      - when drawing shapes in line mode, lines expand inwards to preserve specified size
//  - pointSize would be nice to have
//  - 3d quad from point, normal, width, height
//  - 3d textured quad
//  - 3d circle from point, line, radius
//  - 3d sphere
//  - 3d cylinder
//  - 3d line
//  - setWireframe toggle wireframe rendering for at least all the 2d stuff
//      - wireframes should also respond to lineWidth
//      - wireframes should work with textured draw calls
//      - how they work in 3d will be more complicated
//  - setOrtho matrix and setMVP matrix should flush the vertex_buffer
//      - that way you can set the ortho and mvp whenever you want and everything after
//        it will use whatever was last set
//  - ditch flush !!! 
//      - let the user choose either to use begin() and end() which will cache the verts in
//        in the buffer till the end
//      - or just call the functions whenever and it will render immidiately
//      - also flush is problematic because you might never call it and run out of memory!
//  - If i want to not clobber the GL state then there will need to be a begin() function
//    stores any state that the renderer will change and stores is on end (end should also flush)
//
// From tjh_window.h
//
// - choose requested opengl version
// - set fullscreen state
// - optional antialiasing
// - make is so you don't need to modify this file to change the #defines
//   have them check to see if they are already defined first

////// HEADER //////////////////////////////////////////////////////////////////

#if TJH_DRAW_INCLUDE_SDL
#include TJH_DRAW_SDL_H_LOCATION
#endif

#if TJH_DRAW_INCLUDE_GLEW
#include TJH_DRAW_GLEW_H_LOCATION
#endif

namespace TJH_DRAW_NAMESPACE
{
    // WINDOW /////////////////////////////////////////////////////////////////
    
    extern SDL_Window* sdl_window;
    extern SDL_GLContext sdl_gl_context;

    bool init( const char* title, GLfloat x_offset, GLfloat y_offset, GLfloat width, GLfloat height );
    bool init( const char* title, GLfloat width, GLfloat height );
    void shutdown();

    void clear( GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f )             { glClearColor( r, g, b, a ); glClear( GL_COLOR_BUFFER_BIT ); }
    void flush();
    void present()                                                              { flush(); SDL_GL_SwapWindow( sdl_window ); }

    bool setVsync( bool enable );

    void getSize( int* width, int* height )                                     { SDL_GetWindowSize( sdl_window, width, height ); }

    // DRAWING ////////////////////////////////////////////////////////////////

    extern const float PI;

    extern float red;           // Colour to draw with (does not affect clear colour!)
    extern float green;         //  Normal values are in the range [0.0, 1.0]
    extern float blue;          //  Set them direclty or use setColor(r,g,b,a);
    extern float alpha;         // 0.0 == transparent, 1.0 == opaque/solid

    extern float lineWidth;     // 
    extern float orthoDepth;    // Depth (z value) at which to draw 2D shapes
    extern bool  wireframe;     //

    void flush();

    void setColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f )          { red = r; green = g; blue = b; alpha = a; }
    void setDepth( GLfloat depth )                                              { orthoDepth = depth; }
    void setLineWidth( GLfloat width )                                          { lineWidth = width; }
    void setWireframe( bool enable )                                            { wireframe = enable; };
    void setOrthoMatrix( GLfloat width, GLfloat height );
    void setOrthoMatrix( GLfloat x_offset, GLfloat y_offset, GLfloat width, GLfloat height );
    // void setOrthoMatrix( GLfloat* matrix );
    void setMVPMatrix( GLfloat* matrix );
    void setViewDirection( float x, float y, float z );

    // PRIMATIVES //////////////////////////////////////////////////////////////

    // 2D

    void point( GLfloat x, GLfloat y );
    void line( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );
    void rectangle( GLfloat x, GLfloat y, GLfloat width, GLfloat height );
    void triangle( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3 );
    void circle( GLfloat x, GLfloat y, GLfloat radius, int segments = 16 );

    void texturedQuad( GLfloat x, GLfloat y, GLfloat width, GLfloat height );
    void texturedQuad( GLfloat x, GLfloat y, GLfloat width, GLfloat height,
        GLfloat s, GLfloat t, GLfloat s_width, GLfloat t_height );
    void texturedTriangle( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3,
        GLfloat s1, GLfloat t1, GLfloat s2, GLfloat t2, GLfloat s3, GLfloat t3 );

    // 3D

    void triangle( GLfloat x1, GLfloat y1, GLfloat z1,
        GLfloat x2, GLfloat y2, GLfloat z2,
        GLfloat x3, GLfloat y3, GLfloat z3 );
    void quad( GLfloat x1, GLfloat y1, GLfloat z1,
        GLfloat x2, GLfloat y2, GLfloat z2,
        GLfloat x3, GLfloat y3, GLfloat z3,
        GLfloat x4, GLfloat y4, GLfloat z4 );
    //void billboard( float x, float y, float z, float width, float height,
    //    bool lockx = false, bool locky = false, bool lockz = false );
}

#endif

////// IMPLEMENTATION //////////////////////////////////////////////////////////
#ifdef TJH_DRAW_IMPLEMENTATION

#include <vector>
#include <cmath>

namespace TJH_DRAW_NAMESPACE
{
    // PUBLIC MEMBER VARIABLES
    SDL_Window*     sdl_window      = NULL;
    SDL_GLContext   sdl_gl_context  = NULL;

    const float PI          = 3.14159265359;

    float red               = 1.0f;
    float green             = 1.0f;
    float blue              = 1.0f;
    float alpha             = 1.0f;
    float lineWidth         = 1.0f;
    float orthoDepth        = 0.0f;
    bool  wireframe         = false;

    // 'PRIVATE' MEMBER VARIABLES
    enum class DrawMode { Colour2D, Texture2D, Colour3D, Texture3D };
    DrawMode current_mode_  = DrawMode::Colour2D;

    GLuint colour_program_  = 0;
    GLuint texture_program_ = 0;

    GLuint colour_vao_   = 0;
    GLuint colour_vbo_   = 0;
    GLuint texture_vao_  = 0;
    GLuint texture_vbo_  = 0;

    GLint colour_3d_mvp_uniform_    = 0;
    GLint texture_3d_mvp_uniform_   = 0;
    float width_                    = 1.0f;
    float height_                   = 1.0f;
    float x_offset_                 = 0.0f;
    float y_offset_                 = 0.0f;

    float view_x_                   = 0.0f;
    float view_y_                   = 0.0f;
    float view_z_                   = 0.0f;

    GLfloat mvp_matrix_[16]         = { 0.0f };
    GLfloat ortho_matrix_[16]       = { 0.0f };
    std::vector<GLfloat> vertex_buffer_;

    // 'PRIVATE' MEMBER FUNCTIONS
    void push2( GLfloat one, GLfloat two );
    void push3( GLfloat one, GLfloat two, GLfloat three );
    void push4( GLfloat one, GLfloat two, GLfloat three, GLfloat four );
    void pushTriangle( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3 );
    void pushQuad( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4 );
    void send_ortho_matrix();
    void send_mvp_matrix();

    GLuint create_shader( GLenum type, const char* source );
    GLuint create_program( GLuint vertex_shader, GLuint fragment_shader );

    // LIBRARY FUNCTIONS ///////////////////////////////////////////////////////

    bool init( const char* title, GLfloat width, GLfloat height )
    {
        return init( title, 0, 0, width, height );
    }

    bool init( const char* title, GLfloat x_offset, GLfloat y_offset, GLfloat width, GLfloat height )
    {
        if( SDL_Init(SDL_INIT_EVERYTHING) )
        {
            TJH_DRAW_PRINTF("ERROR: could not init SDL2 %s\n", SDL_GetError());
            return false;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

        sdl_window = SDL_CreateWindow( title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL );
        if( sdl_window == NULL )
        {
            TJH_DRAW_PRINTF("ERROR: creating window %s\n", SDL_GetError());
            return false;
        }

        sdl_gl_context = SDL_GL_CreateContext( sdl_window );
        if( sdl_gl_context == NULL )
        {
            TJH_DRAW_PRINTF("ERROR: creating opengl context %s\n", SDL_GetError());
            return false;
        }

        glewExperimental = GL_TRUE;
        GLenum error = glewInit();
        if( error != GLEW_OK )
        {
            TJH_DRAW_PRINTF("ERROR: starting glew %d", error);
            return false;
        }

        glEnable(GL_MULTISAMPLE);
        setVsync( true );

        const char* colour_3d_vert_src =
            R"(#version 150 core
            uniform mat4 mvp;
            in vec3 vPos;
            in vec4 vCol;
            out vec4 fCol;
            void main()
            {
               fCol = vCol;
               gl_Position = mvp * vec4(vPos, 1.0);
            })";
        const char* colour_3d_frag_src =
            R"(#version 150 core
            in vec4 fCol;
            out vec4 outColour;
            void main()
            {
                outColour = fCol;
            })";

        colour_program_ = create_program(
            create_shader( GL_VERTEX_SHADER, colour_3d_vert_src ),
            create_shader( GL_FRAGMENT_SHADER, colour_3d_frag_src ) );

        colour_3d_mvp_uniform_ = glGetUniformLocation( colour_program_, "mvp" );

        glGenVertexArrays( 1, &colour_vao_ );
        glBindVertexArray( colour_vao_ );
        glGenBuffers( 1, &colour_vbo_ );
        glBindBuffer( GL_ARRAY_BUFFER, colour_vbo_ );

        GLint posAtrib = glGetAttribLocation(colour_program_, "vPos");
        if( posAtrib == -1 ) { TJH_DRAW_PRINTF("ERROR: position attribute not found in shader\n"); }
        glEnableVertexAttribArray( posAtrib );
        glVertexAttribPointer( posAtrib, 3, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), 0 );

        GLint colAtrib = glGetAttribLocation(colour_program_, "vCol");
        if( colAtrib == -1 ) { TJH_DRAW_PRINTF("ERROR: Colour attribute not found in shader\n"); }
        glEnableVertexAttribArray( colAtrib );
        glVertexAttribPointer( colAtrib, 4, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), (void*)(3*sizeof(float)) );

        const char* texture_3d_vert_src =
            R"(#version 150 core
            uniform mat4 mvp;
            in vec3 vPos;
            in vec4 vCol;
            in vec2 vTex;
            out vec4 fCol;
            out vec2 fTex;
            void main()
            {
               fCol = vCol;
               fTex = vTex;
               gl_Position = mvp * vec4(vPos, 1.0);
            })";
        const char* texture_3d_frag_src =
            R"(#version 150 core
            uniform sampler2D tex;
            in vec4 fCol;
            in vec2 fTex;
            out vec4 outColour;
            void main()
            {
                outColour = fCol * texture(tex, fTex);
            })";

        texture_program_ = create_program(
            create_shader( GL_VERTEX_SHADER, texture_3d_vert_src ),
            create_shader( GL_FRAGMENT_SHADER, texture_3d_frag_src ) );

        texture_3d_mvp_uniform_ = glGetUniformLocation( texture_program_, "mvp" );

        glGenVertexArrays( 1, &texture_vao_ );
        glBindVertexArray( texture_vao_ );
        glGenBuffers( 1, &texture_vbo_ );
        glBindBuffer( GL_ARRAY_BUFFER, texture_vbo_ );

        posAtrib = glGetAttribLocation( texture_program_, "vPos" );
        if( posAtrib == -1 ) { TJH_DRAW_PRINTF("ERROR: Position attribute not found in shader\n"); }
        glEnableVertexAttribArray( posAtrib );
        glVertexAttribPointer( posAtrib, 3, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), 0 );

        colAtrib = glGetAttribLocation( texture_program_, "vCol" );
        if( colAtrib == -1 ) { TJH_DRAW_PRINTF("ERROR: Colour attribute not found in shader\n"); }
        glEnableVertexAttribArray( colAtrib );
        glVertexAttribPointer( colAtrib, 4, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), (void*)(3*sizeof(float)) );

        GLint texAtrib = glGetAttribLocation( texture_program_, "vTex" );
        if( texAtrib == -1 ) { TJH_DRAW_PRINTF("ERROR: Texture attribute not found in shader\n"); }
        glEnableVertexAttribArray( texAtrib );
        glVertexAttribPointer( texAtrib, 2, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), (void*)(7*sizeof(float)) );

        setOrthoMatrix( x_offset, y_offset, width, height );

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glUseProgram(0);

        return true;
    }

    void delete_and_zero_program( GLuint program ) { if(program){glDeleteProgram(program);program=0;} }

    void shutdown()
    {
    #define DELETE_AND_ZERO_RESOURCE( res, delete_func ) if(res){delete_func(1,&res);res=0;}
        DELETE_AND_ZERO_RESOURCE( colour_vao_, glDeleteVertexArrays );
        DELETE_AND_ZERO_RESOURCE( texture_vao_, glDeleteVertexArrays );
        DELETE_AND_ZERO_RESOURCE( colour_vbo_, glDeleteBuffers );
        DELETE_AND_ZERO_RESOURCE( texture_vbo_, glDeleteBuffers );
    #undef DELETE_AND_ZERO_RESOURCE

        delete_and_zero_program( colour_program_ );
        delete_and_zero_program( texture_program_ );

        SDL_GL_DeleteContext( sdl_gl_context );
        sdl_gl_context = NULL;
        SDL_DestroyWindow( sdl_window );
        sdl_window = NULL;
        SDL_Quit();
    }

    bool setVsync( bool enable )
    {
        if( enable )
        {
            // try late swap tearing first
            bool success = (SDL_GL_SetSwapInterval( -1 ) == 0);
            if( success )
            {
                return true;
            }
            else
            {
                // If that fails try normal vsync
                return SDL_GL_SetSwapInterval( 1 ) == 0;
            }
        }
        else
        {
            // Pass zero to disable vysinc
            return SDL_GL_SetSwapInterval( 0 ) == 0;
        }
    }

    void flush()
    {
        if( vertex_buffer_.empty() ) return;

        switch( current_mode_ )
        {
        case DrawMode::Colour2D:
            glUseProgram( colour_program_ );
            glBindVertexArray( colour_vao_ );
            glBindBuffer( GL_ARRAY_BUFFER, colour_vbo_ );
            send_ortho_matrix();
        break;
        case DrawMode::Texture2D:
            glUseProgram( texture_program_ );
            glBindVertexArray( texture_vao_ );
            glBindBuffer( GL_ARRAY_BUFFER, texture_vbo_ );
            send_ortho_matrix();
        break;
        case DrawMode::Colour3D:
            glUseProgram( colour_program_ );
            glBindVertexArray( colour_vao_ );
            glBindBuffer( GL_ARRAY_BUFFER, colour_vbo_ );
            send_mvp_matrix();
        break;
        case DrawMode::Texture3D:
            glUseProgram( texture_program_ );
            glBindVertexArray( texture_vao_ );
            glBindBuffer( GL_ARRAY_BUFFER, texture_vbo_ );
            send_mvp_matrix();
        break;
        default:
            TJH_DRAW_PRINTF("ERROR: unknown draw mode!\n");
        break;
        }

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_[0]) * vertex_buffer_.size(), vertex_buffer_.data(), GL_STREAM_DRAW);
        if( current_mode_ == DrawMode::Colour2D || current_mode_ == DrawMode::Colour3D )
        {
            glDrawArrays( GL_TRIANGLES, 0, vertex_buffer_.size()/7 );
        }
        if( current_mode_ == DrawMode::Texture2D || current_mode_ == DrawMode::Texture3D )
        {
            glDrawArrays( GL_TRIANGLES, 0, vertex_buffer_.size()/9 );
        }

        vertex_buffer_.clear();
    }

    // STATE ///////////////////////////////////////////////////////////////////

    void setOrthoMatrix( GLfloat width, GLfloat height )
    {
        flush();
        x_offset_ = 0.0f;
        y_offset_ = 0.0f;
        width_ = width;
        height_ = height;
    }
    void setOrthoMatrix( GLfloat x_offset, GLfloat y_offset, GLfloat width, GLfloat height )
    {
        flush();
        x_offset_ = x_offset;
        y_offset_ = y_offset;
        height_ = height;
        width_ = width;
    }
    void setMVPMatrix( GLfloat* matrix )
    {
        flush();
        std::memcpy( mvp_matrix_, matrix, sizeof(GLfloat) * 16 );
    }
    void setViewDirection( float x, float y, float z )
    {
        flush();
        view_x_ = x; view_y_ = y; view_z_ = z;
    }

    // PRIMATIVES //////////////////////////////////////////////////////////////

    //
    // Colour 2D primatives
    //
    
    void point( GLfloat x, GLfloat y )
    {
        if( current_mode_ != DrawMode::Colour2D ) flush();

        pushTriangle( x    , y    , x + 1, y    , x + 1, y + 1 );
        pushTriangle( x    , y    , x + 1, y + 1, x    , y + 1 );

        current_mode_ = DrawMode::Colour2D;
    }
    void line( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 )
    {
        if( current_mode_ != DrawMode::Colour2D ) flush();
        current_mode_ = DrawMode::Colour2D;

        GLfloat x12 = x2 - x1;
        GLfloat y12 = y2 - y1;
        GLfloat invLength = 1.0f / std::sqrt(x12 * x12 + y12 * y12);

        GLfloat xperp = -y12;
        GLfloat yperp = x12;

        xperp *= invLength * lineWidth;
        yperp *= invLength * lineWidth;

        x1 -= xperp * 0.5f;
        y1 -= yperp * 0.5f;

        pushTriangle( x1, y1,
            x1 + x12, y1 + y12,
            x1 + xperp, y1 + yperp );
        pushTriangle( x1 + xperp, y1 + yperp,
            x1 + x12, y1 + y12,
            x1 + x12 + xperp, y1 + y12 + yperp );
    }
    void rectangle( GLfloat x, GLfloat y, GLfloat width, GLfloat height )
    {
        if( current_mode_ != DrawMode::Colour2D ) flush();
        current_mode_ = DrawMode::Colour2D;

        if( !wireframe )
        {
            pushTriangle( x, y, x + width, y         , x + width, y + height );
            pushTriangle( x, y, x + width, y + height, x        , y + height );
        } else {
            const float midx = x + width * 0.5f;
            const float midy = y + height * 0.5f;
            float cornerx = midx - x;
            float cornery = midy - y;
            const float cornerLength = std::sqrt( cornerx * cornerx + cornery * cornery );

            // TODO: fix this!! incorrect line thickness
            //
            // Corner x/y is supposed to be the vector offset from the corner towards the middle to give the
            // wireframe it's correct line thickness, currently though this is incorrect as i'm using lineWidth as the length
            // of the hypotenuse while the thickness of the line is the side of the triangle
            // This means that with a line thickness of 1 when in wireframe mode the edge lines are actually slightly thinner
            // than one unit

            cornerx = (cornerx / cornerLength) * lineWidth;
            cornery = (cornery / cornerLength) * lineWidth;

            pushQuad( x, y, x + cornerx, y + cornery, x + width - cornerx, y + cornery, x + width, y );
            pushQuad( x, y, x + cornerx, y + cornery, x + cornerx, y - cornery + height, x, y + height );
            pushQuad( x + width, y + height, x + width, y, x + width - cornerx, y + cornery, x + width - cornerx, y + height - cornery );            
            pushQuad( x, y + height, x + width, y + height, x + width - cornerx, y + height - cornery, x + cornerx, y + height - cornery );
        }
    }
    void triangle( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3 )
    {
        if( current_mode_ != DrawMode::Colour2D ) flush();
        current_mode_ = DrawMode::Colour2D;

        if( !wireframe )
        {
            pushTriangle( x1, y1, x2, y2, x3, y3 );
        } else {

            // TODO: triangle wireframe that isn't dumb

            line( x1, y1, x2, y2 );
            line( x2, y2, x3, y3 );
            line( x3, y3, x1, y1 );
        }
    }
    void circle( GLfloat x, GLfloat y, GLfloat radius, int segments )
    {
        if( current_mode_ != DrawMode::Colour2D ) flush();
        current_mode_ = DrawMode::Colour2D;

        float frac = (PI*2) / (float)segments;

        if( !wireframe )
        {
            for( int i = 0; i < segments; i++ )
            {
                pushTriangle(
                    x,
                    y,
                    x + std::sin(frac*i) * radius,
                    y + std::cos(frac*i) * radius,
                    x + std::sin(frac*(i+1)) * radius,
                    y + std::cos(frac*(i+1)) * radius );
            }
        } else {
            float innerRadius = radius - lineWidth;

            for( int i = 0; i < segments; i++ )
            {
                pushQuad(
                    x + std::sin(frac*i) * innerRadius,
                    y + std::cos(frac*i) * innerRadius,
                    x + std::sin(frac*i) * radius,
                    y + std::cos(frac*i) * radius,
                    x + std::sin(frac*(i+1)) * radius,
                    y + std::cos(frac*(i+1)) * radius,
                    x + std::sin(frac*(i+1)) * innerRadius,
                    y + std::cos(frac*(i+1)) * innerRadius );
            }
        }
    }

    //
    // Textured 2D primatives
    //

    void texturedQuad( GLfloat x, GLfloat y, GLfloat width, GLfloat height )
    {
        if( current_mode_ != DrawMode::Texture2D ) flush();
        current_mode_ = DrawMode::Texture2D;

        if( !wireframe )
        {
            push3( x, y, orthoDepth );                  push4( red, green, blue, alpha ); push2( 0, 1 );
            push3( x + width, y, orthoDepth );          push4( red, green, blue, alpha ); push2( 1, 1 );
            push3( x + width, y + height, orthoDepth ); push4( red, green, blue, alpha ); push2( 1, 0 );

            push3( x, y, orthoDepth );                  push4( red, green, blue, alpha ); push2( 0, 1 );
            push3( x + width, y + height, orthoDepth ); push4( red, green, blue, alpha ); push2( 1, 0 );
            push3( x, y + height, orthoDepth );         push4( red, green, blue, alpha ); push2( 0, 0 );
        } else {

        }
    }
    void texturedQuad( GLfloat x, GLfloat y, GLfloat width, GLfloat height,
        GLfloat s, GLfloat t, GLfloat s_width, GLfloat t_height )
    {
        if( current_mode_ != DrawMode::Texture2D ) flush();
        current_mode_ = DrawMode::Texture2D;

        if( !wireframe )
        {
            push3( x, y, orthoDepth );                  push4( red, green, blue, alpha ); push2( s, t + t_height );
            push3( x + width, y, orthoDepth );          push4( red, green, blue, alpha ); push2( s + s_width, t + t_height );
            push3( x + width, y + height, orthoDepth ); push4( red, green, blue, alpha ); push2( s + s_width, t );

            push3( x, y, orthoDepth );                  push4( red, green, blue, alpha ); push2( s, t + t_height );
            push3( x + width, y + height, orthoDepth ); push4( red, green, blue, alpha ); push2( s + s_width, t );
            push3( x, y + height, orthoDepth );         push4( red, green, blue, alpha ); push2( s, t );
        } else {
        }
    }
    void texturedTriangle( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3,
        GLfloat s1, GLfloat t1, GLfloat s2, GLfloat t2, GLfloat s3, GLfloat t3 )
    {
        if( current_mode_ != DrawMode::Texture2D ) flush();
        current_mode_ = DrawMode::Texture2D;

        if( !wireframe )
        {
            push3( x1, y1, orthoDepth ); push4( red, green, blue, alpha ); push2( s1, t1 );
            push3( x2, y2, orthoDepth ); push4( red, green, blue, alpha ); push2( s2, t2 );
            push3( x3, y3, orthoDepth ); push4( red, green, blue, alpha ); push2( s3, t3 );
        } else {

        }
    }

    //
    // Colour 3D primatives
    //
    
    void triangle( GLfloat x1, GLfloat y1, GLfloat z1,
        GLfloat x2, GLfloat y2, GLfloat z2,
        GLfloat x3, GLfloat y3, GLfloat z3 )
    {
        if( current_mode_ != DrawMode::Colour3D ) flush();
        current_mode_ = DrawMode::Colour3D;

        if( !wireframe )
        {
            push3( x1, y1, z1 ); push4( red, green, blue, alpha );
            push3( x2, y2, z2 ); push4( red, green, blue, alpha );
            push3( x3, y3, z3 ); push4( red, green, blue, alpha );
        } else {

        }
    }
    void quad( GLfloat x1, GLfloat y1, GLfloat z1,
        GLfloat x2, GLfloat y2, GLfloat z2,
        GLfloat x3, GLfloat y3, GLfloat z3,
        GLfloat x4, GLfloat y4, GLfloat z4 )
    {
        if( current_mode_ != DrawMode::Colour3D ) flush();
        current_mode_ = DrawMode::Colour3D;

        if( !wireframe )
        {
            push3( x1, y1, z1 ); push4( red, green, blue, alpha );
            push3( x2, y2, z2 ); push4( red, green, blue, alpha );
            push3( x3, y3, z3 ); push4( red, green, blue, alpha );
            push3( x1, y1, z1 ); push4( red, green, blue, alpha );
            push3( x3, y3, z3 ); push4( red, green, blue, alpha );
            push3( x4, y4, z4 ); push4( red, green, blue, alpha );
        } else {

        }
    }
    /*
    void billboard( float x, float y, float z, float width, float height,
        bool lockx, bool locky, bool lockz )
    {
        // TODO: implement me!!!!!!!!!!

        
    }
    //*/

    // UTILS //////////////////////////////////////////////////////////////////
    GLuint create_shader( GLenum type, const char* source )
    {
        GLuint shader = glCreateShader( type );
        if( shader == 0 )
        {
            TJH_DRAW_PRINTF("ERROR: could not create shaders!\n");
            return shader;
        }

        // Compile and check the vertex shader
        glShaderSource( shader, 1, &source, NULL );
        glCompileShader( shader );
        GLint status = GL_TRUE;
        glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
        if( status != GL_TRUE )
        {
            TJH_DRAW_PRINTF( "ERROR: draw could not compile %d shader\n", type );
            // Get the length of the error log
            GLint log_length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

            // Now get the error log itself
            GLchar buffer[log_length];
            glGetShaderInfoLog( shader, log_length, NULL, buffer );
            TJH_DRAW_PRINTF("%s\n", buffer);

            // Clean up the resources, not that it really matters at this point
            // we should really just crash
            glDeleteShader( shader );
            shader = 0;
        }

        return shader;
    }
    GLuint create_program( GLuint vertex_shader, GLuint fragment_shader )
    {
        GLuint program = glCreateProgram();
        glAttachShader( program, vertex_shader );
        glAttachShader( program, fragment_shader );
        glBindFragDataLocation( program, 0, "outColour" );
        glLinkProgram( program );
        glUseProgram( program );
        glDeleteShader( vertex_shader );
        glDeleteShader( fragment_shader );
        return program;
    }
    void push2( GLfloat one, GLfloat two )
    {
        vertex_buffer_.push_back( one );
        vertex_buffer_.push_back( two );
    }
    void push3( GLfloat one, GLfloat two, GLfloat three )
    {
        vertex_buffer_.push_back( one );
        vertex_buffer_.push_back( two );
        vertex_buffer_.push_back( three );
    }
    void push4( GLfloat one, GLfloat two, GLfloat three, GLfloat four )
    {
        vertex_buffer_.push_back( one );
        vertex_buffer_.push_back( two );
        vertex_buffer_.push_back( three );
        vertex_buffer_.push_back( four );
    }
    void pushTriangle( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3 )
    {
        push3( x1, y1, orthoDepth ); push4( red, green, blue, alpha );
        push3( x2, y2, orthoDepth ); push4( red, green, blue, alpha );
        push3( x3, y3, orthoDepth ); push4( red, green, blue, alpha );
    }
    void pushQuad( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4 )
    {
        // Expects points in clockwise order
        push3( x1, y1, orthoDepth ); push4( red, green, blue, alpha );
        push3( x2, y2, orthoDepth ); push4( red, green, blue, alpha );
        push3( x3, y3, orthoDepth ); push4( red, green, blue, alpha );

        push3( x1, y1, orthoDepth ); push4( red, green, blue, alpha );
        push3( x3, y3, orthoDepth ); push4( red, green, blue, alpha );
        push3( x4, y4, orthoDepth ); push4( red, green, blue, alpha );
    }
    void send_ortho_matrix()
    {
        GLfloat xs =  2.0f / width_;     // x scale
        GLfloat ys = -2.0f / height_;    // y scale
        GLfloat xo = -1.0f - (x_offset_ * 2) / width_; // x offset
        GLfloat yo =  1.0f + (y_offset_ * 2) / height_; // y offset
        
        // NOTE: not sure if the `* 2` above is actualy correct?
        // TODO: more testing to find out what is...

        /*
        GLfloat proj[16] = {
            xs,  0,  0,  0,
             0, ys,  0,  0,
             0,  0,  1,  0,
            xo, yo,  0,  1
        };*/
        
        ortho_matrix_[0] = xs;
        ortho_matrix_[5] = ys;
        ortho_matrix_[10] = 1;
        ortho_matrix_[15] = 1;
        ortho_matrix_[12] = xo;
        ortho_matrix_[13] = yo;

        glUniformMatrix4fv( colour_3d_mvp_uniform_, 1, GL_FALSE, ortho_matrix_ );
        glUniformMatrix4fv( texture_3d_mvp_uniform_, 1, GL_FALSE, ortho_matrix_ );
    }
    void send_mvp_matrix()
    {
        glUniformMatrix4fv( colour_3d_mvp_uniform_, 1, GL_FALSE, mvp_matrix_ );
        glUniformMatrix4fv( texture_3d_mvp_uniform_, 1, GL_FALSE, mvp_matrix_ );
    }
}
// Prevent the implementation from leaking into subsequent includes
#undef TJH_DRAW_IMPLEMENTATION
#endif
