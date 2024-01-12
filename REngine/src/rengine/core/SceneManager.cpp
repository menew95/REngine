#include <rengine\core\sceneManager.h>
#include <rengine\core\scene\Scene.h>

#include <rengine\System\ObjectFactory.h>

#include <common\AssetPath.h>
#include <serialize\Serializer.h>

namespace rengine
{
	//std::shared_ptr<Scene> g_pCurrentScene;

	//std::vector<std::shared_ptr<Scene>> g_Scenes;

    DEFINE_SINGLETON_CLASS(SceneManager,
        {

        },
        {

        },
        {

        })

    bool SceneManager::Initialize()
    {
        bool _hr = true;

        utility::Serializer::DeSerialize(g_assetPath + TEXT("Main Scene.scene"));

        auto _scene = SceneManager::CreateScene(L"Main Scene");

        _scene->SetPath(g_assetPath + _scene->GetName() + TEXT(".scene"));

        utility::Serializer _serializer(_scene.get());

        _serializer.Serialize(_scene.get());

        m_scenes.push_back(_scene);

        LoadScene(0);

        return _hr;
    }

    std::shared_ptr<Scene>& SceneManager::GetCurrentScene()
    {
        return m_pCurrentScene;
    }

    size_t SceneManager::GetSceneCount()
    {
        return m_scenes.size();
    }

    std::shared_ptr<Scene> SceneManager::GetScene(size_t idx)
    {
        try
        {
            return m_scenes.at(idx);
        }
        catch (std::out_of_range& e)
        {
            assert(false);

            return nullptr;
        }
    }

    void SceneManager::LoadScene(size_t idx)
    {
        if (m_pCurrentScene)
            m_pCurrentScene.reset();

        m_pCurrentScene = GetScene(idx);
    }

    RENGINE_API std::shared_ptr<Scene> SceneManager::CreateScene(tstring name)
    {
        auto _newScene = ObjectFactory::GetInstance()->CreateObject<Scene>();

        _newScene->SetName(name);

        return _newScene;
    }

    void SceneManager::SaveScene()
    {
        utility::Serializer _serializer(m_pCurrentScene.get());

        _serializer.Serialize(m_pCurrentScene.get());
    }
}