/**

    @file      AssetManager.h
    @brief     Editor에서 Asset들을 관리하는 클래스
    @details   ~
    @author    LWT
    @date      13.03.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>
#include <common\singleton.h>
#include <rengine\core\resources.h>

namespace rengine
{
    class Object;
}

namespace editor
{
    class AssetManager
    {
        DECLARE_SINGLETON_CLASS(AssetManager);
    public:
        /**
            @brief asset data base를 로드
        **/
        void Initialze();

        /**
            @brief 
            @param objec - 
            @param path  - 
        **/
        void CreateAsset(rengine::Object* objec, const tstring& path);

        /**
            @brief 
            @param path - 
        **/
        void ImportAsset(const tstring& path);

        /**
            @brief 
            @param oldPath - 
            @param newPath - 
        **/
        void MoveAsset(const tstring& oldPath, const tstring& newPath);

        /**
            @brief 
            @param filter - 
        **/
        void FindAsset(const tstring& filter);
        
        /**
            @brief 변경된 Asset들을 체크함
        **/
        void Refresh();


    private:
        /**
            @brief 경로를 순회하며 추가가 안된 에셋을 추가
            @param path - 경로
        **/
        void CheckMetaFile(const tstring& path);
        
        vector<string> m_componentList;

        unordered_map<uuid, string> m_assetList;
    };
}