/**

    @file      FBXModel.h
    @brief     fbx model struct
    @details   ~
    @author    LWT
    @date      25.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <serialize\binary\AnimBin.h>
#include <serialize\binary\MeshBin.h>
#include <serialize\binary\MaterialBin.h>

namespace utility
{
    struct Node
    {
        std::string _name;

        math::Matrix _local = math::Matrix::Identity;
        math::Matrix _world = math::Matrix::Identity;

        bool _hasParent = false;

        std::string _parent;

        bool _hasMesh = false;

        std::string _mesh;

        bool _isBone = false;
    };
    
    struct FBXModel
    {
        string _filePath;
        string _name;

        //GameObjectData _rootObject;
        //GameObjectData _amatureObject;

        std::vector<Node> _nodes;

        std::vector<BoneBin> _boneDatas;

        std::map<string, MeshBin> _meshDataMap;

        std::map<string, MateriaBin> _materialDataMap;

        std::vector<AnimationClipBin> _animationClipDatas;
    };
}