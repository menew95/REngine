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

	/*void TextureAtlas::CreateTexture()
	{
		TextureDesc _pointShadowMapDesc;

		_pointShadowMapDesc._extend = { (uint32)m_resolution.x, (uint32)m_resolution.y, 0 };
		_pointShadowMapDesc._bindFlags = BindFlags::ShaderResource | BindFlags::DepthStencil;
		_pointShadowMapDesc._format = Format::R32_TYPELESS;

		_pointShadowMapDesc._textureType = TextureType::Texture2D;

		m_texture = ResourceManager::GetInstance()->CreateTexture(TEXT("Shadow Map Texture"), _pointShadowMapDesc);
	}*/
}