#include <rengine\core\scene\Scene.h>

#include <rengine\core\object\GameObject.h>

#include <rengine\core\component\Camera.h>
#include <rengine\core\component\Transform.h>
#include <rengine\core\component\MeshRenderer.h>

#include <rengine\core\resources.h>
#include <rengine\core\resource\Material.h>
#include <rengine\core\resource\Mesh.h>
#include <rengine\core\resource\Prefab.h>
#include <rengine\core\resource\Model.h>

#include <rttr\registration.h>

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::Scene>("Scene")
	.constructor<tstring>()
	/*.property_readonly("m_typeName", &rengine::Object::GetType)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::WSTRING)
	)
	.property("m_uuid", &rengine::Object::GetUUID, &rengine::Object::SetUUID)
	.property("m_objectName", &rengine::Object::GetName, &rengine::Object::SetName)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::WSTRING)
	)*/
	;
}

namespace rengine
{
	Scene::Scene(uuid uuid)
		: Object(uuid, TEXT("Scene"), TEXT("Scene"))
	{

	}

	Scene::~Scene()
	{
		// 게임 오브젝트에 등록된 스마트 포인터부터 먼저 삭제
		m_gameObjects.clear();

		// 씬에 등록된 게임 루트 오브젝트들을 삭제
		for (auto& _go : m_rootGameObjects)
		{
			Object* _object = _go.get();

			_go.reset();

			DestroyImmediate(_object);
		}

		m_rootGameObjects.clear();
	}

	std::shared_ptr<GameObject> Scene::FindGameObject(tstring& objectName)
	{
		auto _iter = find_if(m_rootGameObjects.begin(), m_rootGameObjects.end(), [&objectName](auto& go)
			{
				return go->GetName() == objectName;
			});

		if (_iter != m_rootGameObjects.end())
		{
			return *_iter;
		}

		return nullptr;
	}

	void Scene::AddGameObject(const std::shared_ptr<GameObject>& go)
	{
		// 게임 오브젝트 리스트에 등록된 게임 오브젝트가 아니라면 추가
		auto _iter = std::ranges::find_if(std::begin(m_gameObjects), std::end(m_gameObjects), 
				[&](auto& obj)
				{
					return obj->GetUUID() == go->GetUUID();
				}
			);

		go->m_pScene = shared_from_this();

		if(_iter == std::end(m_gameObjects))
			m_gameObjects.push_back(go);
	}

	void Scene::RemoveGameObject(const std::shared_ptr<GameObject>& go)
	{
		auto _iter = std::ranges::find_if(m_gameObjects.begin(), m_gameObjects.end()
			, [&go](auto& item)
			{
				return item->GetUUID() == go->GetUUID();
			}
		);

		// 오브젝트 팩토리에서 삭제할때 게임 오브젝트에 씬이 할당 되어있는데 찾지 못함
		assert(_iter != m_gameObjects.end());

		m_gameObjects.erase(_iter);
		
		go->m_pScene.reset();

		RemoveRootGameObject(go);
	}

	void Scene::AddRootGameObject(std::shared_ptr<GameObject> go)
	{
		// 루트 게임 오브젝트 리스트에 등록된 게임 오브젝트가 아니라면 추가
		auto _iter = std::ranges::find_if(std::begin(m_rootGameObjects), std::end(m_rootGameObjects),
			[&](auto& obj)
			{
				return obj->GetUUID() == go->GetUUID();
			}
		);

		if (_iter == std::end(m_rootGameObjects))
			m_rootGameObjects.push_back(go);
	}

	void Scene::RemoveRootGameObject(std::shared_ptr<GameObject> go)
	{
		auto _iter = std::ranges::find_if(m_rootGameObjects.begin(), m_rootGameObjects.end()
			, [&go](auto& item)
			{
				return item->GetUUID() == go->GetUUID();
			}
		);

		if(_iter != m_rootGameObjects.end())
			m_rootGameObjects.erase(_iter);
	}
	void Scene::CreateFromMesh(const uuid& uuid)
	{
		auto _mesh = Resources::GetInstance()->GetResource<Mesh>(uuid);

		// uuid에 해당하는 mesh가 없음
		assert(_mesh != nullptr);

		auto _default_material = Resources::GetInstance()->GetResource<Material>(TEXT("00000000-0000-0000-0000-000000000004"));

		// uuid에 해당하는 _default_material가 없음
		assert(_default_material != nullptr);

		auto _emptyGO = GameObject::Instantiate();

		AddRootGameObject(_emptyGO);

		_emptyGO->SetName(_mesh->GetName());

		auto _meshComp = _emptyGO->AddComponent<MeshRenderer>();

		_meshComp->SetMesh(_mesh);

		vector<shared_ptr<Material>> _materials;

		for(size_t i = 0 ; i < _mesh->GetSubMeshCount(); i++)
		{
			_materials.push_back(_default_material);
		}

		_meshComp->SetMaterials(_materials);
	}

	void Scene::CreateFromModel(const uuid& uuid)
	{
		auto _model = Resources::GetInstance()->GetResource<Model>(uuid);

		// uuid에 해당하는 model이 없음
		assert(_model != nullptr);

		vector<shared_ptr<Mesh>> _meshList;
		//vector<shared_ptr<Bone>> _boneList;
		vector<shared_ptr<Material>> _materialList;
		vector<shared_ptr<AnimationClip>> _clipList;

		// root gameobject 추가 및 이름 설정
		shared_ptr<GameObject> _rootGameObject = GameObject::Instantiate(nullptr);

		_rootGameObject->SetName(_model->GetName());

		AddRootGameObject(_rootGameObject);

		for (auto& _uuid : _model->GetMeshUUIDs())
		{
			_meshList.push_back(Resources::GetInstance()->GetResource<Mesh>(_uuid));
		}

		for (auto& _uuid : _model->GetMaterialUUIDs())
		{
			_materialList.push_back(Resources::GetInstance()->GetResource<Material>(_uuid));
		}

		for (auto& _uuid : _model->GetClipUUIDs())
		{
			_clipList.push_back(Resources::GetInstance()->GetResource<AnimationClip>(_uuid));
		}

		if (!_model->IsSkinnedModel())
		{
			// 스키닝 매쉬가 아님
			vector<pair<ObjectNode, shared_ptr<GameObject>>> _gameObjects;

			// 빈 게임 오브젝트들 먼저 생성
			for (auto& _objectNode : _model->GetObjectNodes())
			{
				// 일단 root object 자식으로 모두 추가
				auto _newGo = GameObject::Instantiate(_rootGameObject->GetTransform());

				AddGameObject(_newGo);

				_newGo->SetName(_objectNode._name);

				_gameObjects.emplace_back(make_pair(_objectNode, _newGo));
			}

			for (auto& [_objectNode, _go] : _gameObjects)
			{
				if (_objectNode._hasParent)
				{
					auto _iter = std::ranges::find_if(std::begin(_gameObjects), std::end(_gameObjects),
						[&](auto& pair)
						{
							return _objectNode._parent == pair.second->GetName();
						});

					// 생성한 게임 오브젝트 목록중에서 해당하는 오브젝트를 찾지 못했음
					if (_iter == std::end(_gameObjects))
					{
						// root node
						_go->GetTransform()->SetParent(_rootGameObject->GetTransform());
					}
					else
					{
						_go->GetTransform()->SetParent(_iter->second->GetTransform());
					}
				}

				// 월드 좌표 설정
				_go->GetTransform()->SetWorld(_objectNode._world);

				if (_objectNode._hasMesh)
				{
					auto _meshComponent = _go->AddComponent<MeshRenderer>();

					auto _meshIter = std::ranges::find_if(std::begin(_meshList), std::end(_meshList), 
						[&](auto& mesh)
						{
							return mesh->GetName() == _objectNode._mesh;
						});

					assert(_meshIter != std::end(_meshList));

					_meshComponent->SetMesh(*_meshIter);
					
					vector<shared_ptr<Material>> _materials;

					for (auto& _mat : _objectNode._materials)
					{
						auto _matIter = std::ranges::find_if(std::begin(_materialList), std::end(_materialList),
							[&](auto& mateiral)
							{
								return mateiral->GetName() == _mat;
							});

						assert(_matIter != std::end(_materialList));

						_materials.emplace_back(*_matIter);
					}

					_meshComponent->SetMaterials(_materials);
				}
			}
		}
		else
		{
			// 스키닝 매쉬임

		}
	}
}