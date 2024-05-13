#include <Editor_pch.h>

#include <editor\Widget\WidgetContainer.h>

#include <editor\Widget\WidgetManager.h>

namespace editor
{
	WidgetContainer::WidgetContainer()
	{

	}

	WidgetContainer::~WidgetContainer()
	{
		m_childs.clear();

		m_addReservedWidgets.clear();
	}

	void WidgetContainer::RenderChild()
	{
		// 이번 프레임을 그리기 전 삭제 예약되어 있는 녀석을 삭제합니다.
		DestroyReservedWidgets();

		for (auto& _childs : m_childs)
		{
			if(_childs->GetEnable())
				_childs->Render();
		}

		// 이번 프레임 마지막에 추가된 녀석들을 그려줍니다.
		AddReservedWidgets();
	}

	void WidgetContainer::DestroyReservedWidgets()
	{
		std::erase_if(m_childs, [](auto& widget)
			{
				if (widget->m_isReservedDestroy)
				{
					widget.reset();

					return true;
				}
				else
					return false;
			});

		std::erase_if(m_addReservedWidgets, [](auto& widget)
			{
				if (widget->m_isReservedDestroy)
				{
					widget.reset();

					return true;
				}
				else
					return false;
			});

	}

	void WidgetContainer::AddReservedWidgets()
	{
		for (auto widget : m_addReservedWidgets)
		{
			m_childs.push_back(widget);

			auto widgetGroupBase = dynamic_cast<WidgetContainer*>(widget.get());

			if (widgetGroupBase != nullptr)
				widgetGroupBase->AddReservedWidgets();
		}

		if (!m_addReservedWidgets.empty())
			m_addReservedWidgets.clear();
	}

	Widget* WidgetContainer::GetChild(const string& lable)
	{
		auto _childIter = find_if(m_childs.begin(), m_childs.end(), [&lable](auto& childs)
			{
				return childs->GetLable() == lable;
			});

		if (_childIter != m_childs.end())
		{
			return (*_childIter).get();
		}

		auto _reserveIter = find_if(m_addReservedWidgets.begin(), m_addReservedWidgets.end(), [&lable](auto& childs)
			{
				return childs->GetLable() == lable;
			});

		if (_reserveIter != m_addReservedWidgets.end())
		{
			return (*_reserveIter).get();
		}

		return nullptr;
	}

	void WidgetContainer::RemoveWidget(Widget* widget)
	{
		for (auto _iter = m_childs.begin(); _iter != m_childs.end(); _iter++)
		{
			if ((*_iter)->GetLable() == widget->GetLable())
			{
				m_childs.erase(_iter);

				break;
			}
		}
	}

	void WidgetContainer::RemoveAllWidget()
	{
		m_childs.clear();
	}

	void WidgetContainer::RemoveAllWidgetReserved()
	{
		// 모든 위젯에 대해서 삭제를 예약합니다.
		for (auto& widget : m_childs)
		{
			widget->m_isReservedDestroy = true;
		}

		// 모든 위젯에 대해서 삭제를 예약합니다.
		for (auto& widget : m_addReservedWidgets)
		{
			widget->m_isReservedDestroy = true;
		}
	}
}