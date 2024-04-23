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
        void CreateAsset(rengine::Object* object, const tstring& path);

        /**
            @brief 
            @param path - 
        **/
        void ImportAsset(const tstring& path);

        /**
            @brief 해당 경로에 있는 에셋을 로드함
            @param path - 에셋의 경로
            @retval      - asset raw pointer(invalid asset path return nullptr)
        **/
        rengine::Object* LoadAsset(const tstring& path);

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
            @brief  경로로부터 에셋 uuid를 가져옴
            @param  path - asset absolute path
            @retval      - asset uuid(invalid asset path return 0)
        **/
        uuid UUIDFromAssetPath(const tstring& path);

        /**
            @brief  경로로부터 에셋를 가져옴
            @param  path - asset absolute path
            @param  type - asset type
            @retval      - asset shared ptr(invalid asset path return nullptr)
        **/
        shared_ptr<rengine::Object> AssetFromPath(const tstring& path, const tstring& type = TEXT(""));

        /**
            @brief 해당 위치에 새로운 폴더를 생성
            @param path - 폴더를 생성할 위치
            @param name - 폴더의 이름
        **/
        void CreateFolder(const tstring& path, const tstring& name);

        /**
            @brief 변경된 Asset들을 체크함
        **/
        void Refresh();


        /**
            @brief  경로와 파일 이름을 조합하여 경로를 생성(이미 해당 경로와 중복된 경로가 존재하면 인덱스를 붙여 새로운 경로를 생성)
            @param  path      - 생성할 경로
            @param  name      - 생성할 파일 \ 폴더 이름
            @param  extension - 생성할 파일 확장자
            @retval           - 생성된 경로
        **/
        static tstring MakeNewPath(const tstring& path, const tstring& name, const tstring& extension = TEXT(""));

        const auto& GetComponentList() { return m_componentList; }

    private:
        /**
            @brief 경로를 순회하며 추가가 안된 에셋을 추가
            @param path - 경로
        **/
        void CheckMetaFile(const tstring& path);

        /**
            @brief asset data를 읽고 에셋 리스트에 할당
        **/
        void LoadAssetData();

        /**
            @brief asset list를 asset data에 저장
        **/
        void SaveAssetData();
        
        bool m_bIsDirty = false;

        vector<string> m_componentList;

        unordered_map<tstring, uuid> m_assetList;
    };
}