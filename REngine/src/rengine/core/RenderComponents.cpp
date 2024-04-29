#include <rengine\core\RenderComponents.h>
#include <rengine\core\component\Renderer.h>

namespace rengine
{
	RenderComponents::RenderComponents(const tstring& componentName)
		: m_componentName(componentName)
	{

	}

	RenderComponents::~RenderComponents()
	{

	}

	void RenderComponents::AddComponent(Renderer* addComponent)
	{
		auto _iter = std::ranges::find_if(std::begin(m_components), std::end(m_components),
			[&](auto& comp)
			{
				return addComponent == comp;
			});

		assert(_iter != m_components.end());

		m_components.push_back(addComponent);
	}

	void RenderComponents::DeleteComponent(Renderer* deleteComponent)
	{
		auto _iter = std::ranges::find_if(std::begin(m_components), std::end(m_components),
			[&](auto& comp)
			{
				return deleteComponent == comp;
			});

		assert(_iter == m_components.end());

		m_components.erase(_iter);
	}

	void RenderComponents::RenderComponent()
	{
		for (auto* _renderer : m_components)
		{
			_renderer->Render();
		}
	}
}