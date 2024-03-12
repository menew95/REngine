#include <graphics_core\RenderQueue.h>


namespace graphics
{
	DEFINE_SINGLETON_CLASS(RenderQueue,
		{

		},
		{

		}, 
		{

		});

	RenderObject* RenderQueue::CreateRenderObject(const RenderType type, uuid uuid)
	{
		RenderObject* _ret = nullptr;

		switch (type)
		{
			case RenderType::MESH:
			{
				auto _obj = make_unique<MeshObject>(uuid);

				_ret = _obj.get();

				m_meshObjectList.insert(make_pair(uuid, std::forward<std::unique_ptr<MeshObject>>(_obj)));

				break;
			}
			case RenderType::PARTICLE:
			{
				auto _obj = make_unique<ParticleObject>(uuid);
			
				_ret = _obj.get();

				m_particleObjectList.insert(make_pair(uuid, std::forward<std::unique_ptr<ParticleObject>>(_obj)));

				break;
			}
			default:
			{
				assert(false);
			}
		}

		return _ret;
	}

	void RenderQueue::DeleteObject(RenderObject* object)
	{
		switch (object->GetRenderType())
		{
			case RenderType::MESH:
			{
				auto _iter = m_meshObjectList.find(object->GetUUID());

				assert(_iter != m_meshObjectList.end());

				m_meshObjectList.erase(_iter);
				
				break;
			}
			case RenderType::PARTICLE:
			{
				auto _iter = m_particleObjectList.find(object->GetUUID());

				assert(_iter != m_particleObjectList.end());

				m_particleObjectList.erase(_iter);

				break;
			}
			default:
			{
				assert(false);
			}
		}

	}
}