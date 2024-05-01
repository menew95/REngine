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
        }

        bool insertTexture(uuid texture_id, const Vector2& resolution, Vector2& uv0, Vector2& uv1) 
        {
            // 이미 텍스처 공간을 할당함
            if (_texture_id != TEXT(""))
                return false;

            // 텍스처 공간을 할당하기에 너무 작을 경우 불가능
            if (_nodeSize.x < resolution.x || _nodeSize.y < resolution.y)
                return false;

            // 만약 공간이 분할 되어있다면 자식 객체중에 할당이 가능한 영역을 찾음
            if (children[0] != nullptr) {
                for (auto child : children) {
                    if (child->insertTexture(texture_id, resolution, uv0, uv1)) {
                        return true;
                    }
                }

                // 어느 자식에도 더이상 할당이 불가능함
                return false;
            }

            // 텍스처 공간이 이미 점유되어 있지않고 사이즈가 맞을 경우 공간을 할당함
            if (_nodeSize.x == resolution.x && _nodeSize.y == resolution.y) {
                _texture_id = texture_id;
                uv0 = _nodeStartPoint;
                uv1 = _nodeStartPoint + _nodeSize;
                return true;
            }

            // 만약 너무 크다면 공간을 분할함
            divide();

            // 분할 후 다식 자식 객체에서 테스츠
            for (auto child : children) {
                if (child->insertTexture(texture_id, resolution, uv0, uv1)) {
                    return true;
                }
            }

            return false;
        }

        bool releaseTexture(uuid texture_id)
        {
            // 찾는 노드일 경우 id를 초기화 시킴
            if (_texture_id == texture_id)
            {
                _texture_id = TEXT("");
                
                return true;
            }

            if (children[0] == nullptr)
            {
                // 공간 분할 되어있지 않는 텍스처임
                return false;
            }
            else
            {
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
                                //delete child;
                                //child = nullptr;

                                child.reset();
                            }

                            m_isRootNode = true;
                        }

                        return true; // 텍스처가 해제되면 true 반환
                    }
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

        //QuadTreeNode* children[4] = { nullptr };
        shared_ptr<QuadTreeNode> children[4];

        bool m_isRootNode = true;

        void divide()
        {
            m_isRootNode = false;

            Vector2 _childSize = _nodeSize / 2;

            //children[0] = new QuadTreeNode({_nodeStartPoint.x, _nodeStartPoint.y}, _childSize);
            //children[1] = new QuadTreeNode({_nodeStartPoint.x + _childSize.x, _nodeStartPoint.y}, _childSize);
            //children[2] = new QuadTreeNode({_nodeStartPoint.x, _nodeStartPoint.y + _childSize .y}, _childSize);
            //children[3] = new QuadTreeNode({_nodeStartPoint.x + _childSize.x, _nodeStartPoint.y + _childSize .y}, _childSize);
            
            children[0] = make_shared<QuadTreeNode>(Vector2{_nodeStartPoint.x, _nodeStartPoint.y}, _childSize);
            children[1] = make_shared<QuadTreeNode>(Vector2{_nodeStartPoint.x + _childSize.x, _nodeStartPoint.y}, _childSize);
            children[2] = make_shared<QuadTreeNode>(Vector2{_nodeStartPoint.x, _nodeStartPoint.y + _childSize .y}, _childSize);
            children[3] = make_shared<QuadTreeNode>(Vector2{_nodeStartPoint.x + _childSize.x, _nodeStartPoint.y + _childSize .y}, _childSize);
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