/**

    @file      TextureAtlas.h
    @brief     Texture Atlas class
    @details   ~
    @author    LWT
    @date      30.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <common\math.h>

namespace graphics
{
    class QuadTreeNode
    {
    public:

        QuadTreeNode(const Vector2& startPoint, const Vector2& size)
        : _nodeStartPoint(startPoint)
        , _nodeSize(size)
        , _texture_id(TEXT(""))
        {}

        ~QuadTreeNode()
        {
            for (int i = 0; i < 4; ++i) {
                if (children[i] != nullptr) {
                    delete children[i];
                    children[i] = nullptr;
                }
            }
        }

        bool insertTexture(uuid texture_id, const Vector2& resolution, Vector2& uv0, Vector2& uv1) {
            // If the node is already filled, return false
            if (_texture_id != TEXT(""))
                return false;

            // If the node is too small to contain the texture, return false
            if (_nodeSize.x < resolution.x || _nodeSize.y < resolution.y)
                return false;

            // If the node has children, try to insert into them
            if (children[0] != nullptr) {
                for (auto child : children) {
                    if (child->insertTexture(texture_id, resolution, uv0, uv1)) {
                        return true;
                    }
                }
                return false; // Failed to insert into children
            }

            // If the node is just right, assign it the texture
            if (_nodeSize.x == resolution.x && _nodeSize.y == resolution.y) {
                _texture_id = texture_id;
                uv0 = _nodeStartPoint;
                uv1 = _nodeStartPoint + _nodeSize;
                return true;
            }

            // Otherwise, divide the node and insert into one of its children
            divide();

            return insertTexture(texture_id, resolution, uv0, uv1);
        }

        bool releaseTexture(uuid texture_id)
        {
            // 찾는 노드일 경우 id를 초기화 시킴
            if (_texture_id == texture_id)
            {
                _texture_id = TEXT("");
                
                return true;
            }

            // 자식 노드들에 대해 재귀적으로 호출하여 텍스처를 해제
            for (auto child : children) 
            {
                if (child != nullptr && child->releaseTexture(texture_id))
                {
                    // 모든 자식 노드가 해제되었는지 확인
                    bool allChildrenReleased = true;
                    for (auto child : children) 
                    {
                        if (child != nullptr && !child->isReleased()) 
                        {
                            allChildrenReleased = false;
                            break;
                        }
                    }
                    // 모든 자식 노드가 해제되었다면 부모 노드를 합침
                    if (allChildrenReleased)
                    {
                        // 자식 노드들을 삭제
                        for (auto& child : children) 
                        {
                            delete child;
                            child = nullptr;
                        }

                        m_isRootNode = true;
                    }

                    return true; // 텍스처가 해제되면 true 반환
                }
            }

            return false; // 텍스처가 존재하지 않거나 해제에 실패한 경우 false 반환
        }

        bool isReleased() const 
        {
            // 텍스처 ID가 비어있고 자식 노드로 분할이 안되었을 경우
            return _texture_id.empty() && m_isRootNode;
        }

        bool isSetTexture() { return _texture_id != TEXT(""); }

    private:
        // current node start point
        Vector2 _nodeStartPoint;

        // current node size
        Vector2 _nodeSize;
        
        // set texture id;
        uuid _texture_id;

        QuadTreeNode* children[4] = { nullptr };

        bool m_isRootNode = true;

        void divide()
        {
            m_isRootNode = false;

            Vector2 _childSize = _nodeSize / 2;

            children[0] = new QuadTreeNode({_nodeStartPoint.x, _nodeStartPoint.y}, _childSize);
            children[1] = new QuadTreeNode({_nodeStartPoint.x + _childSize.x, _nodeStartPoint.y}, _childSize);
            children[2] = new QuadTreeNode({_nodeStartPoint.x, _nodeStartPoint.y + _childSize .y}, _childSize);
            children[3] = new QuadTreeNode({_nodeStartPoint.x + _childSize.x, _nodeStartPoint.y + _childSize .y}, _childSize);
        }
    };

    class TextureAtlas
    {
    public:
        TextureAtlas(const uuid& uuid, const Vector2& size);
        ~TextureAtlas();

        /**
            @brief 아틀라스의 크기를 재설정함, 기존에 설정된 아틀라스들 정보는 삭제됨
            @param size - 설정할 크기
        **/
        void ResizeAtlas(const Vector2& resolution);

        /**
            @brief  아틀라스로부터 사용가능한 영역을 찾음
            @param  resolution - 사용할 해상도
            @param  uv0        - 왼쪽 위
            @param  uv1        - 오른쪽 아래
            @retval            - 텍스처 할당 여부
        **/
        bool GetTextureSpace(uuid texture_id, const Vector2& resolution, Vector2& uv0, Vector2& uv1);

        bool ReturnTextureSpace(uuid texture_id);

    private:
        /**
            @brief 텍스처를 생성함
        **/
        //void CreateTexture();

        uuid m_uuid;

        Vector2 m_resolution;

        shared_ptr<QuadTreeNode> m_rootNode;

        class Texture* m_texture;
    };
}