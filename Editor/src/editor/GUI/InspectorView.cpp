#include <Editor_pch.h>

#include <editor/GUI/InspectorView.h>

#include <rengine\core\object\GameObject.h>
#include <rengine\core\component\Component.h>

#include <rttr\registration.h>
#include <rttr\type.h>
#include <rengine\core\object\MetaInfo.h>

namespace editor
{
    InspectorView::InspectorView()
    {
        m_ViewName = "InspectorView";
    }

    InspectorView::~InspectorView()
    {
    }

    void InspectorView::Begin()
    {
        __super::Begin();

    }
    void InspectorView::Render()
    {
        __super::Render();

        if (m_pSelectObj != nullptr)
        {
            for (auto& _comp : m_pSelectObj->GetComponents())
            {
                DrawComponent(_comp.get());
            }
        }
    }
    void InspectorView::End()
    {
        __super::End();

    }
    void InspectorView::SetObject(rengine::GameObject* gameObj)
    {
        assert(gameObj == nullptr);

        m_pSelectObj = gameObj;
    }

    void InspectorView::DrawComponent(rengine::Component* comp)
    {
        const rttr::type component_type = rttr::type::get_by_name(StringHelper::ToString(comp->GetName()));

        for (rttr::property _prop : component_type.get_properties())
        {
            const rttr::variant _value = _prop.get_value(comp);

            rttr::variant _metaVariant = _prop.get_metadata(rengine::MetaData::Serializable);

            if (!_metaVariant.is_valid())
                continue;

            rengine::MetaDataType _metaDataType = _metaVariant.get_value<rengine::MetaDataType>();

            if (_value.is_sequential_container())
            {
                auto _seq = _value.create_sequential_view();

                for (const auto& item : _seq) // iterates over all elements of the std::vector<T>
                {
                    auto _extractVar = item.extract_wrapped_value();
                    //GetProperty(_metaDataType, _comp, component_type, _extractVar, _prop, _item, true);
                }
            }
            else
            {
                //GetProperty(_metaDataType, _comp, component_type, _value, _prop, _comp_pt);
            }
        }
    }
}