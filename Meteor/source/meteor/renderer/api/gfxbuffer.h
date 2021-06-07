#pragma once

#include "meteor/core/base.h"

namespace meteor
{
    enum class ShaderDataType
    {
        None = 0,
        Int, Int2, Int3, Int4,
        Float, Float2, Float3, Float4
    };

    struct BufferElement
    {
        mtrString       Semantic;
        mtrUInt         Offset = 0;
        ShaderDataType  Type;
        mtrBool         Normalized;

        BufferElement(const mtrString& semantic, ShaderDataType type, mtrBool normalized)
            : Semantic(semantic), Type(type), Normalized(normalized)
        {
        }

        mtrUInt GetComponentCount() const
        {
            switch (Type)
            {
                case ShaderDataType::Int:       return 1;
                case ShaderDataType::Int2:      return 2;
                case ShaderDataType::Int3:      return 3;
                case ShaderDataType::Int4:      return 4;
                case ShaderDataType::Float:     return 1;
                case ShaderDataType::Float2:    return 2;
                case ShaderDataType::Float3:    return 3;
                case ShaderDataType::Float4:    return 4;
            }

            METEOR_ENGINE_ASSERT(false, "Unknown shader data type!");
            return 0;
        }

        mtrUInt GetElementSize() const
        {
            switch (Type)
            {
                case ShaderDataType::Int:       return 4 * 1;
                case ShaderDataType::Int2:      return 4 * 2;
                case ShaderDataType::Int3:      return 4 * 3;
                case ShaderDataType::Int4:      return 4 * 4;
                case ShaderDataType::Float:     return 4 * 1; 
                case ShaderDataType::Float2:    return 4 * 2;
                case ShaderDataType::Float3:    return 4 * 3;
                case ShaderDataType::Float4:    return 4 * 4;
            }

            METEOR_ENGINE_ASSERT(false, "Unknown shader data type!");
            return 0;
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout() = default;
        BufferLayout(const std::initializer_list<BufferElement>& elements)
            : m_Elements(elements)
        {
            CalculateStrideAndOffsets();
        }

        void CalculateStrideAndOffsets()
        {
            mtrUInt offset = 0;

            for (auto& element : m_Elements)
            {
                element.Offset = offset;
                offset += element.GetElementSize();
                m_Stride += element.GetElementSize();
            }
        }

        inline const std::vector<BufferElement>&            GetElements() const { return m_Elements; }
        inline mtrUInt                                      GetStride() const { return m_Stride; }

        inline std::vector<BufferElement>::iterator         begin() { return m_Elements.begin(); }
        inline std::vector<BufferElement>::const_iterator   begin() const  { return m_Elements.begin(); }
        inline std::vector<BufferElement>::iterator         end() { return m_Elements.end(); }
        inline std::vector<BufferElement>::const_iterator   end() const { return m_Elements.end(); }
    private:
        std::vector<BufferElement> m_Elements;
        mtrUInt m_Stride = 0;
    };

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void                Bind() = 0;
        virtual void                SetBufferLayout(const BufferLayout& layout) = 0;

        virtual const BufferLayout& GetBufferLayout() const = 0;
    };

    enum class PrimitiveTopology
    {
        None = 0,
        Triangles,
        Lines
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void    Bind() = 0;
        virtual void    SetTopology(PrimitiveTopology topology) = 0;

        virtual mtrUInt GetCount() const = 0;
    };
}