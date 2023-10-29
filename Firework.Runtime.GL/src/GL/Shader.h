#pragma once

#include "Firework.Runtime.GL.Exports.h"

#include <bgfx/bgfx.h>
#include <cstring>
#include <map>
#include <vector>

#define __concat(a, b) a##b
#define __recat(a, b) __concat(a, b)
#define getGeometryProgramArgsFromPrecompiledShaderName(shaderName, backend) \
(void*)__recat(__recat(__recat(__concat(shader, shaderName), Vertex), backend), Data), \
(uint32_t)__recat(__recat(__recat(__recat(__concat(shader, shaderName), Vertex), backend), Data), _Size), \
(void*)__recat(__recat(__recat(__concat(shader, shaderName), Fragment), backend), Data), \
(uint32_t)__recat(__recat(__recat(__recat(__concat(shader, shaderName), Fragment), backend), Data), _Size)

namespace Firework
{
    namespace GL
    {
        class Renderer;

        enum class UniformType
        {
            Vec4 = bgfx::UniformType::Vec4,
            Mat3 = bgfx::UniformType::Mat3,
            Mat4 = bgfx::UniformType::Mat4
        };
        struct ShaderUniform
        {
            const char* name;
            UniformType type;
            uint16_t count = 1;
        };

        class __firework_gl_api GeometryProgramHandle final
        {
            struct Comp
            {
                bool operator()(const char* lhs, const char* rhs) const
                {
                    return strcmp(lhs, rhs) < 0;
                }
            };

            bgfx::ProgramHandle internalHandle;
            std::map<const char*, bgfx::UniformHandle, Comp> internalUniformHandles;
            
            static GeometryProgramHandle create
            (
                void* vertexShaderData, uint32_t vertexShaderDataSize,
                void* fragmentShaderData, uint32_t fragmentShaderDataSize,
                const ShaderUniform* uniforms, size_t uniformsLength
            );
        public:
            template <size_t N>
            inline static GeometryProgramHandle create
            (
                void* vertexShaderData, uint32_t vertexShaderDataSize,
                void* fragmentShaderData, uint32_t fragmentShaderDataSize,
                const ShaderUniform (&uniforms)[N] = { }
            )
            {
                return GeometryProgramHandle::create(vertexShaderData, vertexShaderDataSize, fragmentShaderData, fragmentShaderDataSize, uniforms, N);
            }
            inline static GeometryProgramHandle create(void* vertexShaderData, uint32_t vertexShaderDataSize, void* fragmentShaderData, uint32_t fragmentShaderDataSize)
            {
                return GeometryProgramHandle::create(vertexShaderData, vertexShaderDataSize, fragmentShaderData, fragmentShaderDataSize, nullptr, 0);
            }
            void destroy();

            void setUniform(const char* name, const void* value);
            void setUniform(const char* name, const void* value, uint16_t count);

            friend class Firework::GL::Renderer;
        };
    }
}