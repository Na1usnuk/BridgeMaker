export module bm.graphic;

export import bm.gfx.shader;
export import bm.gfx.buffer.index;
export import bm.gfx.buffer.vertex;
export import bm.gfx.vertexarray;
export import bm.gfx.renderer;
export import bm.gfx.utility;
export import bm.gfx.mesh;
export import bm.gfx.geometry;

import std;

export namespace bm::gfx
{

using VertexBufferPtr = std::shared_ptr<VertexBuffer>;
using IndexBufferPtr = std::shared_ptr<IndexBuffer>;
using VertexArrayPtr = std::shared_ptr<VertexArray>;
using ShaderPtr = std::shared_ptr<Shader>;
using LayoutPtr = std::shared_ptr <VertexBufferLayout>;

template<BufferConcept ContainerType>
VertexBufferPtr makeVBO(const ContainerType& container, VertexBuffer::Draw draw_hint = VertexBuffer::Draw::STATIC) { return std::make_shared<VertexBuffer>(container, draw_hint); }
VertexBufferPtr makeVBO(const void* data, std::size_t size, VertexBuffer::Draw draw_hint = VertexBuffer::Draw::STATIC) { return std::make_shared<VertexBuffer>(data, size, draw_hint); }
VertexBufferPtr makeVBO(std::size_t size, VertexBuffer::Draw draw_hint = VertexBuffer::Draw::STATIC) { return std::make_shared<VertexBuffer>(size, draw_hint); }
//std::shared_ptr<VertexBuffer> makeVBO() { return std::make_shared<VertexBuffer>(); }

template<BufferConcept ContainerType>
std::shared_ptr<IndexBuffer> makeIBO(const ContainerType& container) { return std::make_shared<IndexBuffer>(container); }
std::shared_ptr<IndexBuffer> makeIBO(const unsigned int* data, std::size_t count) { return std::make_shared<IndexBuffer>(data, count); }
//std::shared_ptr<IndexBuffer> makeIBO() { return std::make_shared<IndexBuffer>(); }

std::shared_ptr<Shader> makeShader(const std::filesystem::path& filepath) { return std::make_shared<Shader>(filepath); }

std::shared_ptr<VertexBufferLayout> makeLayout() { return std::make_shared<VertexBufferLayout>(); }

std::shared_ptr<VertexArray> makeVAO() { return std::make_shared<VertexArray>(); }
std::shared_ptr<VertexArray> makeVAO(std::shared_ptr<VertexBuffer> vbo, std::shared_ptr<VertexBufferLayout> layout) { return std::make_shared<VertexArray>(vbo, layout); }

}