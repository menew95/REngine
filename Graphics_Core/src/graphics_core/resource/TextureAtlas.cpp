#include <graphics_core\resource\TextureAtlas.h>

#include <graphics_core\ResourceManager.h>

#include <graphics_module\Texture.h>

namespace graphics
{
	TextureAtlas::TextureAtlas(const uuid& uuid, const Vector2& size)
	: m_uuid(uuid)
	{
		ResizeAtlas(size);
	}

	TextureAtlas::~TextureAtlas()
	{

	}

	void TextureAtlas::ResizeAtlas(const Vector2& resolution)
	{
		m_rootNode.reset();

		m_resolution = resolution;

		m_rootNode = make_shared<QuadTreeNode>(Vector2{0, 0}, resolution);
	}

	bool TextureAtlas::GetTextureSpace(uuid texture_id, const Vector2& resolution, Vector2& uv0, Vector2& uv1)
	{
		return m_rootNode->insertTexture(texture_id, resolution, uv0, uv1);
	}

	bool TextureAtlas::ReturnTextureSpace(uuid texture_id)
	{
		return m_rootNode->releaseTexture(texture_id);
	}
}