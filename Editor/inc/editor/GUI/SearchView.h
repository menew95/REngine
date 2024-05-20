/**

    @file      SearchView.h
    @brief     에셋, 컴포넌트 등과 같은 오브젝트 등을 찾고 찾은 오브젝트를 이벤트에 따라 관리
    @details   ~
    @author    LWT
    @date      22.03.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <editor\GUI\View.h>

#include <common\Event.h>

namespace rengine
{
    class Object;
}

namespace editor
{
    typedef void (*ButtonEvent)(void* thisClass, const shared_ptr<rengine::Object>& obj);

    class SearchView : public View
    {
    public:
        SearchView();

        EDITOR_API ~SearchView();

        void EDITOR_API Begin() override;
        
        void EDITOR_API Render() override;
        
        void EDITOR_API End() override;

        static void OpenSeachView(string searchType
        , const std::function<void(const shared_ptr<rengine::Object>&)>& itemClickEvent
        , const std::function<map<uuid, std::shared_ptr<rengine::Object>>()>& objectFilterFunc
        );
        //static void OpenSeachView(string searchType,  void (*ButtonEvent)(const shared_ptr<rengine::Object>& obj) = nullptr);

        void SetFindObjectListFunc(const auto& functor)
        {
            m_findObjectListEvent = functor;

            m_isAllFunctorSet = true;
        }

        void SetObjectMap(const auto& list)
        {
            m_userData._objectMap = list;

            m_userData._objectList.clear();

            for (auto _pair : m_userData._objectMap)
            {
                m_userData._objectList.push_back(_pair.second);
            }
        }

    private:
        string m_searchType;

        // input text buffer
        char m_searchTextBuf[256];

        // 람다식에서 캡처를 쓰면 함수 포인터로 사용이 불가능하기에 일단 이렇게 사용한다.
        struct ImGUiUserData
        {
            map<uuid, std::shared_ptr<rengine::Object>> _objectMap;
            vector<shared_ptr<rengine::Object>> _objectList;
        };

        ImGUiUserData m_userData;

        bool m_isAllFunctorSet = false;

        std::function<map<uuid, std::shared_ptr<rengine::Object>>()> m_findObjectListEvent;

        std::function<void(const shared_ptr<rengine::Object>&)> m_clickItemEvent;
    };
}