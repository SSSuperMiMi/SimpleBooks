//
//  Shader.cpp
//  Simple Books
//
//  Created by Allen Zhao on 6/27/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#include "Shader.h"
#include "macro.h"

bool Shader::CreateShader(const char* vname, const char* fname, unsigned* log, Shader** ptr)
{
    if (vname == nullptr || fname == nullptr) {
        *log = shader_log_path_error;
        *ptr = nullptr;return false;
    }
    
    Shader* newshader = new Shader(vname, fname, log);
    if (*log != shader_log_success) {
        SAFE_DELETE(newshader);
        *ptr = nullptr;
        return false;
    }

    return *ptr = newshader;
}

Shader::Shader(const char* vname, const char* fname, unsigned* log)
{
    Load(vname, fname, log);
}

void Shader::Shading()
{
    glUseProgram(m_shader_id);
}

void Shader::UpdateUniformValue(const char* name, GLsizei count, GLboolean transpose, const GLfloat *value, unsigned row,  unsigned column)
{
    unsigned location = GetUniformVariableLocation(name);

    if (row == 2)
    {
        switch (column) {
            case 2:
                return glUniformMatrix2fv(location, count, transpose, value);
            case 3:
                return glUniformMatrix2x3fv(location, count, transpose, value);
            case 4:
                return glUniformMatrix2x4fv(location, count, transpose, value);
            
            default:
                return;
        }
    }
    
    if (row == 3)
    {
        switch (column) {
            case 2:
                return glUniformMatrix3x2fv(location, count, transpose, value);
            case 3:
                return glUniformMatrix3fv(location, count, transpose, value);
            case 4:
                return glUniformMatrix3x4fv(location, count, transpose, value);
                
            default:
                return;
        }
    }
    
    if (row == 4)
    {
        switch (column) {
            case 2:
                return glUniformMatrix4x2fv(location, count, transpose, value);
            case 3:
                return glUniformMatrix4x3fv(location, count, transpose, value);
            case 4:
                return glUniformMatrix4fv(location, count, transpose, value);
                
            default:
                return;
        }
    }
}

void Shader::UpdateUniformValue(const char* name, GLsizei count, GLsizei array_length, const GLfloat *value)
{
    unsigned location = GetUniformVariableLocation(name);
    switch (array_length) {
        case 1:
            return glUniform1fv(location, count, value);
        case 2:
            return glUniform2fv(location, count, value);
        case 3:
            return glUniform3fv(location, count, value);
        case 4:
            return glUniform4fv(location, count, value);
            
        default:
            return;
    }
}

void Shader::UpdateUniformValue(const char* name, GLsizei count, GLsizei array_length, const GLint *value)
{
    unsigned location = GetUniformVariableLocation(name);
    switch (array_length) {
        case 1:
            return glUniform1iv(location, count, value);
        case 2:
            return glUniform2iv(location, count, value);
        case 3:
            return glUniform3iv(location, count, value);
        case 4:
            return glUniform4iv(location, count, value);
            
        default:
            return;
    }
}

void Shader::UpdateUniformValue(const char* name, GLsizei count, GLsizei array_length, const GLuint *value)
{
    unsigned location = GetUniformVariableLocation(name);
    switch (array_length) {
        case 1:
            return glUniform1uiv(location, count, value);
        case 2:
            return glUniform2uiv(location, count, value);
        case 3:
            return glUniform3uiv(location, count, value);
        case 4:
            return glUniform4uiv(location, count, value);
            
        default:
            return;
    }
}

int Shader::GetAttributeLocation(const char* name)
{
    return glGetAttribLocation(m_shader_id, name);
}

int Shader::GetUniformVariableLocation(const char* name)
{
    return glGetUniformLocation(m_shader_id, name);
}

bool Shader::BindAttributeLocation(GLuint type, const char* name)
{
    if (type < GLKVertexAttribPosition || type > GLKVertexAttribTexCoord1 || name == nullptr) {
        return false;
    }
    glBindAttribLocation(m_shader_id, type, name);
    
    return true;
}

bool Shader::Load(const char* vname, const char* fname, unsigned* log)
{
    GLuint vertShader, fragShader;
    NSString *vertShaderPathname, *fragShaderPathname;
    
    m_shader_id = glCreateProgram();
    
    NSString* v_filename = [NSString stringWithFormat:@"%s",vname];
    NSString* f_filename = [NSString stringWithFormat:@"%s",fname];
    
    vertShaderPathname = [[NSBundle mainBundle] pathForResource:v_filename ofType:@"vsh"];
    if (!Compile(&vertShader, GL_VERTEX_SHADER, vertShaderPathname)) {
        *log = shader_log_failed_to_compile_vertex_shader;
        return false;
    }
    
    fragShaderPathname = [[NSBundle mainBundle] pathForResource:f_filename ofType:@"fsh"];
    if (!Compile(&fragShader, GL_FRAGMENT_SHADER, fragShaderPathname)) {
        *log = shader_log_failed_to_compile_fragment_shader;
        return false;
    }
    
    glAttachShader(m_shader_id, vertShader);
    glAttachShader(m_shader_id, fragShader);
    
    if (!Link(m_shader_id)) {
         *log = shader_log_failed_to_link_shader;
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (m_shader_id) {
            glDeleteProgram(m_shader_id);
            m_shader_id = 0;
        }
        
        return false;
    }
    
    if (vertShader) {
        glDetachShader(m_shader_id, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(m_shader_id, fragShader);
        glDeleteShader(fragShader);
    }
    
    *log = shader_log_success;
    return true;
}

bool Shader::Compile(GLuint* shader, GLenum type, NSString* file)
{
    GLint status;
    const GLchar *source;
    
    source = (GLchar *)[[NSString stringWithContentsOfFile:file encoding:NSUTF8StringEncoding error:nil] UTF8String];
    if (!source) {
        NSLog(@"Failed to load vertex shader");
        return false;
    }
    
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
    
#if defined(DEBUG)
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        NSLog(@"Shader compile log:\n%s", log);
        free(log);
    }
#endif
    
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        glDeleteShader(*shader);
        return false;
    }
    
    return true;
}

bool Shader::Link(GLuint shader_id)
{
    GLint status;
    glLinkProgram(shader_id);
    
#if defined(DEBUG)
    GLint logLength;
    glGetProgramiv(shader_id, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(shader_id, logLength, &logLength, log);
        NSLog(@"Program link log:\n%s", log);
        free(log);
    }
#endif
    
    glGetProgramiv(shader_id, GL_LINK_STATUS, &status);
    if (status == 0) {
        return false;
    }
    
    return true;
}

bool Shader::Validate(GLuint shader_id)
{
    GLint logLength, status;
    
    glValidateProgram(shader_id);
    glGetProgramiv(shader_id, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(shader_id, logLength, &logLength, log);
        NSLog(@"Program validate log:\n%s", log);
        free(log);
    }
    
    glGetProgramiv(shader_id, GL_VALIDATE_STATUS, &status);
    if (status == 0) {
        return false;
    }
    
    return true;
}



























