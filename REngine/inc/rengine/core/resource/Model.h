/**

    @file      Model.h
    @brief     
    @details   ~
    @author    Cool Guy
    @date      13.05.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <rengine\core\resource\Resource.h>

#include <common\math.h>

namespace rengine
{
    struct ObjectNode
    {
        tstring _name;

        Matrix _world = Matrix::Identity;

        bool _hasParent = false;
        tstring _parent;

        bool _hasMesh = false;
        tstring _mesh;

        vector<tstring> _materials;

        bool _hasBone = false;
        tstring _bone;
    };

    class Model : public Resource
    {
    public:
        Model(const uuid& uuid);

        Model(const Model& model) = default;

        Model(Model&& model) = default;

        virtual ResourceType GetResourceType()
        {
            return ResourceType::MODEL;
        }

        bool IsSkinnedModel() const { return m_isSkinned; }

        void SetIsSkinnedModel(bool val) { m_isSkinned = val; }

        const vector<ObjectNode>& GetObjectNodes()  { return m_objectNodeList; }
        const vector<uuid>& GetMeshUUIDs()          { return m_meshList; }
        const vector<uuid>& GetBoneUUIDs()          { return m_boneList; }
        const vector<uuid>& GetMaterialUUIDs()      { return m_materialList; }
        const vector<uuid>& GetClipUUIDs()          { return m_animationClipList; }

        RENGINE_API void AddNode(const ObjectNode& node)
        {
            m_objectNodeList.emplace_back(node); 
        }

        RENGINE_API void AddMeshUUID(const uuid& uuid)
        {
            m_meshList.emplace_back(uuid);
        }

        RENGINE_API void AddBoneUUID(const uuid& uuid)
        {
            m_boneList.emplace_back(uuid);
        }

        RENGINE_API void AddMaterialUUID(const uuid& uuid)
        {
            m_materialList.emplace_back(uuid);
        }

        RENGINE_API void AddClipUUID(const uuid& uuid)
        {
            m_animationClipList.emplace_back(uuid);
        }

    private:
        bool m_isSkinned = false;

        vector<ObjectNode> m_objectNodeList;

        vector<uuid> m_meshList;
        vector<uuid> m_boneList;
        vector<uuid> m_materialList;
        vector<uuid> m_animationClipList;
    };
}