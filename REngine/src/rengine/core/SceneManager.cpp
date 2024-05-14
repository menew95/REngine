#include <rengine\core\sceneManager.h>
#include <rengine\core\scene\Scene.h>

#include <rengine\System\ObjectFactory.h>
#include <rengine\System\GraphicsSystem.h>
#include <rengine\core\resources.h>
#include <rengine\core\object\GameObject.h>
#include <rengine\core\component\Camera.h>

#include <serialize\Serializer.h>
#include <common\AssetPath.h>

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
            Instance.UnInitialize();
        })

    bool SceneManager::Initialize()
    {
        bool _hr = true;

        auto _scene = utility::Serializer::DeSerialize(g_assetPath + TEXT("Main Scene.scene"));

        /*auto _scene = SceneManager::CreateScene(L"Main Scene");

        _scene->SetPath(g_assetPath + _scene->GetName() + TEXT(".scene"));

        utility::Serializer _serializer;

        _serializer.Serialize(_scene.get());*/

        m_scenes.push_back(std::static_pointer_cast<Scene>(_scene));

        LoadScene(0);

        return _hr;
    }

    void SceneManager::UnInitialize()
    {
        m_pCurrentScene.reset();

        for (auto& _scene : m_scenes)
        {
            Object* _object = _scene.get();

            _scene.reset();

            Object::DestroyImmediate(_object);
        }
    }

    std::shared_ptr<Scene>& SceneManager::GetCurrentScene()
    {
        return m_pCurrentScene;
    }

    size_t SceneManager::GetSceneCount()
    {
        return m_scenes.size();
    }

    void SceneManager::UpdateGraphicsSetting()
    {
        GraphicsSystem::GetInstance()->SetGraphicsSetting(m_pCurrentScene->GetGraphicsSetting());
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

        UpdateGraphicsSetting();
    }

    const std::shared_ptr<Scene>& SceneManager::LoadScene(const tstring& path)
    {
        auto _iter = std::ranges::find_if(std::begin(m_scenes), std::end(m_scenes),
            [&path](auto& scene)
            {
                return scene->GetPath() == path;
            });

        // 해당 경로의 씬 파일은 이미 로드한 상태임
        if (_iter != std::end(m_scenes))
        {
            // 해당 경로 씬 파일이 지금 씬이라면 레퍼런스 카운트를 위해 리셋
            if (m_pCurrentScene->GetPath() == path)
                m_pCurrentScene.reset();

            Object* _object = (*_iter).get();

            // 씬 shared ptr 레퍼런스 카운트 관리
            _iter->reset();

            // 해당 씬을 바로 파괴함
            Object::DestroyImmediate(_object);

        }

        auto _scene = utility::Serializer::DeSerialize(g_assetPath + TEXT("Main Scene.scene"));

        // 원래 있던 인덱스 자리에 새롭게 할당
        (*_iter) = std::static_pointer_cast<Scene>(_scene);

        m_pCurrentScene = (*_iter);

        return m_scenes.back();
    }

    RENGINE_API std::shared_ptr<Scene> SceneManager::CreateScene(tstring name)
    {
        auto _newScene = ObjectFactory::GetInstance()->CreateObject<Scene>();

        _newScene->SetName(name);

        auto _camGO = GameObject::Instantiate();
        _camGO->SetName(L"Main Camera");

        _camGO->AddComponent<Camera>();

        _newScene->AddRootGameObject(_camGO);
        //_camGO->SetScene(_newScene);

        auto _lightGO = GameObject::Instantiate();

        _lightGO->SetName(L"Directional Light");

        _newScene->AddRootGameObject(_lightGO);
        //_lightGO->SetScene(_newScene);

        //_newScene->AddRootGameObject(_camGO);
        //_newScene->AddRootGameObject(_lightGO);

        return _newScene;
    }

    void SceneManager::SaveScene()
    {
        utility::Serializer::Serialize(m_pCurrentScene->GetPath(), m_pCurrentScene.get());
    }

    void SceneManager::ReloadCurrentScene()
    {
        assert(false);

        auto _path = m_pCurrentScene->GetPath();

        auto* _object = m_pCurrentScene.get();

        m_pCurrentScene.reset();

        ObjectFactory::GetInstance()->ReserveDestroyObject(_object);

        auto _scene = utility::Serializer::DeSerialize(_path);

        m_scenes.push_back(std::static_pointer_cast<Scene>(_scene));
    }
}