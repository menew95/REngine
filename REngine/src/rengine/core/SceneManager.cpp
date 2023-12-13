#include <rengine\core\sceneManager.h>
#include <rengine\core\scene\scene.h>


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

    bool SceneManager::initialize()
    {
        bool _hr = true;



        return _hr;
    }

    std::shared_ptr<Scene>& SceneManager::GetCurrentScene()
    {
        return m_pCurrentScene;
    }

    size_t SceneManager::GetSceneCount()
    {
        return m_Scenes.size();
    }

    std::shared_ptr<Scene> SceneManager::GetScene(size_t idx)
    {
        try
        {
            return m_Scenes.at(idx);
        }
        catch (std::out_of_range& e)
        {
            assert(false, e.what());

            return nullptr;
        }
    }

    void SceneManager::LoadScene(size_t idx)
    {
        if (m_pCurrentScene)
            m_pCurrentScene.reset();

        m_pCurrentScene = GetScene(idx);
    }
}