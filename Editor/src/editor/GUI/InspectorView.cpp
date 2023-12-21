#include <Editor_pch.h>

#include <editor\Core\EventManager.h>

#include <editor/GUI/InspectorView.h>

#include <editor\Widget\Button.h>
#include <editor\Widget\InputText.h>

#include <rengine\core\object\GameObject.h>
#include <rengine\core\component\Component.h>

#include <rttr\registration.h>
#include <rttr\type.h>
#include <rengine\core\object\MetaInfo.h>

namespace editor
{
    InspectorView::InspectorView()
    : View("Inspector View")
    {

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


        if (ImGui::ButtonEx("Button1"))
        {

        }

        if (EventManager::GetInstance()->GetFocusObject() != nullptr)
        {
            auto* _go = reinterpret_cast<rengine::GameObject*>(EventManager::GetInstance()->GetFocusObject());

            string _str = _go->GetNameStr();

            Event<rengine::Object, void, string> _event(*_go, &rengine::GameObject::SetNameStr);
            InputText _inputText(_go->GetNameStr(), _str, 0, _event);

            _inputText.Render();

            for (auto& _comp : _go->GetComponents())
            {
                DrawComponent(_comp.get());
            }
        }
    }
    void InspectorView::End()
    {
        __super::End();

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