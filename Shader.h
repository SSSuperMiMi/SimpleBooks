//
//  Shader.h
//  Simple Books
//
//  Created by Allen Zhao on 6/27/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#ifndef __Simple_Books__Shader__
#define __Simple_Books__Shader__

#include <stdio.h>
#import <GLKit/GLKit.h>
#import <OpenGLES/ES3/gl.h>

class Shader {
public:
    static bool CreateShader(const char*, const char*, unsigned*, Shader**);
    
    Shader(const char*, const char*, unsigned*);
    
    bool Load(const char*, const char*, unsigned*);
    bool Compile(GLuint*, GLenum, NSString*);
    bool Link(GLuint);
    bool Validate(GLuint);
    bool BindAttributeLocation(GLuint, const char*);
    
    int GetUniformVariableLocation(const char*);
    int GetAttributeLocation(const char*);
    
    void UpdateUniformValue(const char* name, GLfloat v_){glUniform1f(GetUniformVariableLocation(name), v_);}
    void UpdateUniformValue(const char* name, GLfloat v_, GLfloat v1){glUniform2f(GetUniformVariableLocation(name), v_, v1);}
    void UpdateUniformValue(const char* name, GLfloat v_, GLfloat v1, GLfloat v2){glUniform3f(GetUniformVariableLocation(name), v_, v1, v2);}
    void UpdateUniformValue(const char* name, GLfloat v_, GLfloat v1, GLfloat v2, GLfloat v3){glUniform4f(GetUniformVariableLocation(name), v_, v1, v2, v3);}
    void UpdateUniformValue(const char* name, GLint v_){glUniform1i(GetUniformVariableLocation(name), v_);}
    void UpdateUniformValue(const char* name, GLint v_, GLint v1){glUniform2i(GetUniformVariableLocation(name), v_, v1);}
    void UpdateUniformValue(const char* name, GLint v_, GLint v1, GLint v2){glUniform3i(GetUniformVariableLocation(name), v_, v1, v2);}
    void UpdateUniformValue(const char* name, GLint v_, GLint v1, GLint v2, GLint v3){glUniform4i(GetUniformVariableLocation(name), v_, v1, v2, v3);}
    void UpdateUniformValue(const char* name, GLuint v_){glUniform1ui(GetUniformVariableLocation(name), v_);}
    void UpdateUniformValue(const char* name, GLuint v_, GLuint v1){glUniform2ui(GetUniformVariableLocation(name), v_, v1);}
    void UpdateUniformValue(const char* name, GLuint v_, GLuint v1, GLuint v2){glUniform3ui(GetUniformVariableLocation(name), v_, v1, v2);}
    void UpdateUniformValue(const char* name, GLuint v_, GLuint v1, GLuint v2, GLuint v3){glUniform4ui(GetUniformVariableLocation(name), v_, v1, v2, v3);}
    void UpdateUniformValue(const char* name, GLsizei count, GLsizei array_length, const GLfloat *value);
    void UpdateUniformValue(const char* name, GLsizei count, GLsizei array_length, const GLint *value);
    void UpdateUniformValue(const char* name, GLsizei count, GLsizei array_length, const GLuint *value);
    void UpdateUniformValue(const char* name, GLsizei count, GLboolean transpose, const GLfloat *value, unsigned row,  unsigned column);
    
    void Shading();
    
    GLuint m_shader_id;
};
#endif /* defined(__Simple_Books__Shader__) */
