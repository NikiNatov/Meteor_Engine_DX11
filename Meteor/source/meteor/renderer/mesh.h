#pragma once

#include "meteor\core\base.h"

#include "meteor\renderer\api\gfxbuffer.h"
#include "meteor\renderer\material.h"

#include <glm\glm.hpp>

namespace meteor
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec2 TexCoords;
        glm::vec3 Normal;
        glm::vec4 Color;
    };

    class Mesh
    {
    public:
        Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& uvs, const std::vector<glm::vec3>& normals, const std::vector<glm::vec4>& colors, const std::vector<mtrU32>& indices);

        void                                    Render();
        void                                    SetName(const mtrString& name) { m_Name = name; }
        void                                    SetMaterial(Ref<MaterialInstance> material) { m_Material = material; }

        inline const mtrString&                 GetName() { return m_Name; }
        inline Ref<MaterialInstance>            GetMaterial() const { return m_Material; }
    private:                                 
        mtrString                               m_Name;
        std::vector<Vertex>                     m_Vertices;
        std::vector<mtrU32>                     m_Indices;

        Ref<VertexBuffer>                       m_VertexBuffer;
        Ref<IndexBuffer>                        m_IndexBuffer;

        Ref<MaterialInstance>                   m_Material;
        mtrBool                                 m_IsValid = false;
    };
}