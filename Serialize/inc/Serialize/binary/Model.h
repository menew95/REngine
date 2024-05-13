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
        vector<std::string> _materials;

        bool _isBone = false;
    };
    
    struct Model
    {
        string _filePath;
        string _name;

        //GameObjectData _rootObject;
        //GameObjectData _amatureObject;
        
        std::vector<Node> _nodes;

        std::vector<BoneBin> _bones;

        std::map<string, MeshBin> _meshMap;

        std::map<string, MaterialBin> _materialMap;

        std::vector<AnimationClipBin> _animationClips;
    };
}