#include "pch.h"
#include "mesh.h"

#include "meteor\renderer\api\gfxrendererapi.h"
#include "meteor\renderer\api\gfxcontext.h"

namespace meteor
{
    
    // --------------------------------------------------------------------------------------------------------------------------------------
    Mesh::Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& uvs, const std::vector<glm::vec3>& normals, const std::vector<glm::vec4>& colors, const std::vector<mtrU32>& indices)
        : m_Indices(indices)
    {
        BufferLayout layout = {
            { "POSITION", meteor::ShaderDataType::Float3, false },
            { "TEXCOORD", meteor::ShaderDataType::Float2, false },
            { "NORMAL", meteor::ShaderDataType::Float3, false },
            { "COLOR", meteor::ShaderDataType::Float4, false },
        };

        for (mtrInt i = 0; i < positions.size(); i++)
        {
            Vertex v;
            v.Position = positions[i];
            v.TexCoords = uvs[i];
            v.Normal = normals[i];
            v.Color = colors[i];
            m_Vertices.push_back(v);
        }

        // TODO: Bind proper shader before creating vertex buffers

        if (m_Vertices.size() && m_Indices.size())
        {
            m_VertexBuffer = GfxContext::GetDevice()->CreateVertexBuffer(m_Vertices.data(), sizeof(Vertex) * m_Vertices.size());
            m_VertexBuffer->SetBufferLayout(layout);

            m_IndexBuffer = GfxContext::GetDevice()->CreateIndexBuffer(m_Indices.data(), m_Indices.size());
            m_IndexBuffer->SetTopology(PrimitiveTopology::Triangles);

            m_IsValid = true;
        }
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void Mesh::Render()
    {
        if (m_IsValid)
        {
            GfxContext::SetVertexBuffer(m_VertexBuffer);

            mtrU32 materialFlags = m_Material->GetRenderFlags();
            RendererAPI::SetDepthStencilTest(!(materialFlags & Material::DisableDepthTest));
            RendererAPI::SetWireframeMode(materialFlags & Material::Wireframe);
            RendererAPI::SetBlendState(materialFlags & Material::Transparent);

            m_Material->Bind();
            GfxContext::SetIndexBuffer(m_IndexBuffer);
            RendererAPI::DrawIndexed(m_IndexBuffer);
        }
    }
}
