#include <Importer_pch.h>
#include <importer\FBXImporter.h>
#include <importer\FBXModel.h>

#include <common\AssetPath.h>

#include <rengine\core\Resources.h>
#include <rengine\core\resource\Material.h>
#include <rengine\core\resource\Mesh.h>
#include <rengine\core\resource\Texture.h>
#include <rengine\core\resource\AnimationClip.h>

#include <serialize\Serializer.h>

#include <filesystem>

#define FBXSDK_SHARED

#include <fbx\fbxsdk.h>


std::unique_ptr<fbxsdk::FbxManager, void(*)(fbxsdk::FbxManager*)> g_pFbxManager{ nullptr, [](fbxsdk::FbxManager* ptr) {} };
std::unique_ptr<fbxsdk::FbxImporter, void(*)(fbxsdk::FbxImporter*)> g_pFbxImporter{ nullptr, [](fbxsdk::FbxImporter* ptr) {} };
std::unique_ptr<fbxsdk::FbxScene, void(*)(fbxsdk::FbxScene*)> g_pFbxScene{ nullptr, [](fbxsdk::FbxScene* ptr) {} };

static bool _bIsNegativeScaleFlag = false;

namespace utility
{
#pragma region math <=> fbxmath converter
	math::Vector4 Convert(fbxsdk::FbxVector4& v, bool lh = true)
	{
		if (lh == false)
		{
			return math::Vector4
			(
				static_cast<float>(v.mData[0]),
				static_cast<float>(v.mData[1]),
				static_cast<float>(v.mData[2]),
				static_cast<float>(v.mData[3])
			);
		}

		// xyzw -> xzyw
		return math::Vector4
		(
			static_cast<float>(v.mData[0]),
			static_cast<float>(v.mData[2]),
			static_cast<float>(v.mData[1]),
			static_cast<float>(v.mData[3])
		);
	}

	math::Vector3 Convert(fbxsdk::FbxDouble3 v, bool lh = true)
	{
		if (lh == false)
		{
			return math::Vector3
			(
				static_cast<float>(v.mData[0]),
				static_cast<float>(v.mData[1]),
				static_cast<float>(v.mData[2])
			);
		}

		// xyz -> xzy
		return math::Vector3
		(
			static_cast<float>(v.mData[0]),
			static_cast<float>(v.mData[2]),
			static_cast<float>(v.mData[1])
		);
	}

	math::Matrix Convert(fbxsdk::FbxMatrix& matrix, bool lh = true)
	{
		math::Matrix _result;

		FbxVector4 r1 = matrix.GetRow(0);
		FbxVector4 r2 = matrix.GetRow(1);
		FbxVector4 r3 = matrix.GetRow(2);
		FbxVector4 r4 = matrix.GetRow(3);

		if (lh == false)
		{
			return math::Matrix
			(
				Convert(r1, lh),
				Convert(r2, lh),
				Convert(r3, lh),
				Convert(r4, lh)
			);
		}

		return math::Matrix
		(
			Convert(r1, lh),
			Convert(r3, lh),
			Convert(r2, lh),
			Convert(r4, lh)
		);
	}

	math::Matrix Convert(fbxsdk::FbxAMatrix& aMatrix, bool lh = true)
	{
		fbxsdk::FbxMatrix matrix = aMatrix;

		return Convert(matrix, lh);
	}

	void Rotate(math::Matrix& matrix)
	{
		const auto _pitch = -90.0f * math::Deg2Rad;

		math::Quaternion _q = math::Quaternion::CreateFromYawPitchRoll(0, _pitch, 0);

		matrix *= math::Matrix::CreateFromQuaternion(_q);
	}

	math::Matrix GetLocalMatrix(fbxsdk::FbxNode* node, bool lh = true)
	{
		fbxsdk::FbxMatrix _localPos = g_pFbxScene->GetAnimationEvaluator()->GetNodeLocalTransform(node);

		math::Matrix _localTM = Convert(_localPos, lh);

		auto _fbxtran = Convert(node->LclTranslation.Get(), lh);
		auto _fbxRot = Convert(node->LclRotation.Get(), lh);
		auto _fbxscale = Convert(node->LclScaling.Get(), lh);

		auto _rotQuat = math::Quaternion::CreateFromYawPitchRoll(_fbxRot * math::Deg2Rad);

		auto _scaleTM = math::Matrix::CreateScale(_fbxscale);
		auto _rotTM = math::Matrix::CreateFromQuaternion(_rotQuat);
		auto _transTM = math::Matrix::CreateTranslation(_fbxtran);

		auto _localTM2 = math::Matrix::CreateScale(_fbxscale) * math::Matrix::CreateFromQuaternion(_rotQuat) * math::Matrix::CreateTranslation(_fbxtran);

		auto _det = _localTM.Determinant();

		return _localTM;
	}

	math::Matrix GetWorldMatrix(fbxsdk::FbxNode* node, bool lh = true)
	{
		fbxsdk::FbxMatrix _globalPos = g_pFbxScene->GetAnimationEvaluator()->GetNodeGlobalTransform(node);

		math::Matrix _worldTM = Convert(_globalPos, lh);

		Rotate(_worldTM);

		return _worldTM;
	}

	fbxsdk::FbxAMatrix GetTransformMatrix(fbxsdk::FbxNode* node)
	{
		const fbxsdk::FbxVector4 translation = node->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
		const fbxsdk::FbxVector4 rotation = node->GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
		const fbxsdk::FbxVector4 scaling = node->GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);
		return fbxsdk::FbxAMatrix(translation, rotation, scaling);
	}
#pragma endregion
	
	void ReconstructionName(string& name, const char* c)
	{
		if (name.find(c) != string::npos)
		{
			size_t idx = name.find(c) + 1;

			name.erase(0, idx);

			ReconstructionName(name, c);
		}
	}

	void Initialize()
	{
		g_pFbxManager = std::move(std::unique_ptr<fbxsdk::FbxManager, void(*)(fbxsdk::FbxManager*)>(fbxsdk::FbxManager::Create(),
			[](fbxsdk::FbxManager* ptr) {ptr->Destroy(); }));

		if (g_pFbxManager == nullptr)
		{
			FBXSDK_printf("Error: Unable to create FBX Manager!\n");
		}
		else FBXSDK_printf("Autodesk FBX SDK version %s\n", g_pFbxManager->GetVersion());

		fbxsdk::FbxIOSettings* _pFbxIOSettings = fbxsdk::FbxIOSettings::Create(g_pFbxManager.get(), IOSROOT);

		g_pFbxManager->SetIOSettings(_pFbxIOSettings);

		g_pFbxImporter = std::move(std::unique_ptr<fbxsdk::FbxImporter, void(*)(fbxsdk::FbxImporter*)>(fbxsdk::FbxImporter::Create(g_pFbxManager.get(), ""),
			[](fbxsdk::FbxImporter* ptr) {ptr->Destroy(); }));

		g_pFbxScene = std::move(std::unique_ptr<fbxsdk::FbxScene, void(*)(fbxsdk::FbxScene*)>(fbxsdk::FbxScene::Create(g_pFbxManager.get(), ""),
			[](fbxsdk::FbxScene* ptr) {ptr->Destroy(); }));
	}

	bool LoadScene(const char* path)
	{
		int _iFileMajor, _iFileMinor, _iFileRevision;
		int _iSDKMajor, _iSDKMinor, _iSDKRevision;

		FbxManager::GetFileFormatVersion(_iSDKMajor, _iSDKMinor, _iSDKRevision);

		bool _result = g_pFbxImporter->Initialize(path, -1, g_pFbxManager->GetIOSettings());
		g_pFbxImporter->GetFileVersion(_iFileMajor, _iFileMinor, _iFileRevision);

		if (!_result)
		{
			FbxString error = g_pFbxImporter->GetStatus().GetErrorString();
			FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
			FBXSDK_printf("Error returned: %s\n\n", error.Buffer());

			if (g_pFbxImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
			{
				FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", _iSDKMajor, _iSDKMinor, _iSDKRevision);
				FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", path, _iFileMajor, _iFileMinor, _iFileRevision);
			}

			return _result;
		}

		_result = g_pFbxImporter->Import(g_pFbxScene.get());

		if (!_result)
		{
			if (g_pFbxImporter->GetStatus() == FbxStatus::ePasswordError)
			{
				FBXSDK_printf("\nPassword is wrong, import aborted.\n");
			}
			else if (g_pFbxImporter->GetStatus() != FbxStatus::eSuccess)
			{
				FBXSDK_printf("Importer failed to load the file!\n\n");
			}
		}

		return _result;
	}

	void LoadFile(const tstring& path)
	{
		fbxsdk::FbxString _path(StringHelper::WStringToString(path).c_str());

		if (_path.IsEmpty())
			return;
		
		assert(LoadScene(_path.Buffer()));
			
		fbxsdk::FbxSystemUnit lFbxFileSystemUnit = g_pFbxScene->GetGlobalSettings().GetSystemUnit();
		fbxsdk::FbxSystemUnit lFbxOriginSystemUnit = g_pFbxScene->GetGlobalSettings().GetOriginalSystemUnit();

		double factor = lFbxFileSystemUnit.GetScaleFactor();
		double factor2 = lFbxOriginSystemUnit.GetScaleFactor();

		const fbxsdk::FbxSystemUnit::ConversionOptions lConversionOptions =
		{
		  true,
		  true,
		  true,
		  true,
		  true,
		  true
		};

		
		//fbxsdk::FbxAxisSystem::DirectX.ConvertScene(g_pFbxScene.get());

		lFbxFileSystemUnit.m.ConvertScene(g_pFbxScene.get(), lConversionOptions);
		lFbxOriginSystemUnit.m.ConvertScene(g_pFbxScene.get(), lConversionOptions);

		// GeometryConverter 객체 생성
		FbxGeometryConverter* _geometryConverter = new FbxGeometryConverter(g_pFbxManager.get());

		// 씬 내에서 삼각형화 할 수 있는 모든 노드를 삼각형화 시킨다. 
		_geometryConverter->Triangulate(g_pFbxScene.get(), true, true);

		// 여러 머터리얼이 하나의 메쉬에 할당된것을 하나의 메쉬가 하나의 머터리얼로 가지게함
		_geometryConverter->SplitMeshesPerMaterial(g_pFbxScene.get(), true);
	}


	void FindAnimationData(FBXModel& model)
	{
		FbxArray<FbxString*> _animNames;
		g_pFbxScene->FillAnimStackNameArray(OUT _animNames);

		const int animCount = _animNames.GetCount();

		for (int i = 0; i < animCount; i++)
		{
			fbxsdk::FbxAnimStack* animStack = g_pFbxScene->FindMember<fbxsdk::FbxAnimStack>(_animNames[i]->Buffer());

			if (animStack == nullptr)
				continue;

			AnimationClipBin _newClip;
			_newClip._clipName = animStack->GetName();
			ReconstructionName(_newClip._clipName, "|");

			// 시작시간, 종료시간, 초당 프레임에 대한 정보
			fbxsdk::FbxTakeInfo* takeInfo = g_pFbxScene->GetTakeInfo(animStack->GetName());
			double _startTime = takeInfo->mLocalTimeSpan.GetStart().GetSecondDouble();
			double _endTime = takeInfo->mLocalTimeSpan.GetStop().GetSecondDouble();
			double _frameRate = (float)FbxTime::GetFrameRate(g_pFbxScene->GetGlobalSettings().GetTimeMode());

			_newClip._frameRate = static_cast<float>(_frameRate);

			if (_startTime < _endTime)
			{
				_newClip._totalKeyFrame = (int)((_endTime - _startTime) * (double)_frameRate);
				_newClip._endKeyFrame = (int)((_endTime - _startTime) * (double)_frameRate);
				_newClip._tickPerFrame = static_cast<uint32>((_endTime - _startTime) / _newClip._totalKeyFrame);
				_newClip._startKeyFrame = (int)(_startTime)*_newClip._totalKeyFrame;
			}

			model._animationClips.push_back(_newClip);
		}
	}

	// fbx 노드를 이용하여 animation clip의 snap 데이터 생성
	void LoadAnimationData(fbxsdk::FbxNode* node, FBXModel& model)
	{
		const int animCount = static_cast<const int>(model._animationClips.size());

		for (int animIdx = 0; animIdx < animCount; animIdx++)
		{
			AnimationSnapBin _snap;

			_snap._nodeName = node->GetName();

			FbxNode* _parentNode = node->GetParent();

			if (_parentNode != nullptr)
			{
				_snap._parentNodeName = _parentNode->GetName();
			}

			FbxTime::EMode timeMode = g_pFbxScene->GetGlobalSettings().GetTimeMode();

			std::string _animName = model._animationClips[animIdx]._clipName;

			FbxAnimStack* animStack = g_pFbxScene->FindMember<FbxAnimStack>(_animName.c_str());
			g_pFbxScene->SetCurrentAnimationStack(OUT animStack);

			for (FbxLongLong frame = 0; frame < model._animationClips[animIdx]._totalKeyFrame; frame++)
			{
				KeyFrameBin _posKeyFrameInfo;
				KeyFrameBin _rotKeyFrameInfo;
				KeyFrameBin _scaleKeyFrameInfo;

				FbxTime fbxTime = 0;

				fbxTime.SetFrame(frame, timeMode);

				// Local Transform = 부모 Bone의 Global Transform의 inverse Transform * 자신 Bone의 Global Transform;
				FbxAMatrix localTransform = node->EvaluateGlobalTransform(fbxTime);

				math::Matrix localTM;

				if (FbxNode* parent = node->GetParent())
				{
					FbxNodeAttribute* ParentAttribute = parent->GetNodeAttribute();

					//if (ParentAttribute && ParentAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
					{
						FbxAMatrix GlobalParentTransform = parent->EvaluateGlobalTransform(fbxTime);

						localTransform = GlobalParentTransform.Inverse() * localTransform;

						localTM = Convert(localTransform);
					}
				}
				else
				{
					localTM = Convert(localTransform);
				}

				math::Vector3 _pos, _scale;
				math::Quaternion _rot;

				localTM.Decompose(_scale, _rot, _pos);

				_posKeyFrameInfo._frameRate = static_cast<float>(fbxTime.GetSecondDouble());
				_rotKeyFrameInfo._frameRate = static_cast<float>(fbxTime.GetSecondDouble());
				_scaleKeyFrameInfo._frameRate = static_cast<float>(fbxTime.GetSecondDouble());

				_posKeyFrameInfo._data = math::Vector4(_pos);
				_rotKeyFrameInfo._data = _rot;
				_scaleKeyFrameInfo._data = math::Vector4(_scale);

				_snap._posKeyFrameList.push_back(_posKeyFrameInfo);
				_snap._rotKeyFrameList.push_back(_rotKeyFrameInfo);
				_snap._scaleKeyFrameList.push_back(_scaleKeyFrameInfo);
			}

			model._animationClips[animIdx]._snapList.push_back(_snap);
		}
	}

	int32 FindBoneIndex(std::string boneName, FBXModel& model)
	{
		auto _iter = std::find_if(std::begin(model._bones), std::end(model._bones),
			[&boneName](auto& v)
			{
				return v._boneName == boneName;
			}
		);

		if (_iter != model._bones.end())
		{
			return static_cast<int32>(_iter - model._bones.begin());
		}
		else
		{
			return -1;
		}
	}

	Node GetNode(fbxsdk::FbxNode* node, bool& negativeScale)
	{
		Node _ret;

		_ret._name = node->GetName();

		auto* _parentNode = node->GetParent();

		if (_parentNode != nullptr)
		{
			_ret._hasParent = true;
			_ret._parent = _parentNode->GetName();
		}

		_ret._local = GetLocalMatrix(node);
		_ret._world = GetWorldMatrix(node);

		_ret._hasMesh = true;

		float _det = _ret._local.Determinant();

		// Negative scale 일 경우 negative scale flag 를 true로 설정
		if (_det < 0)
		{
			math::Matrix _parentWorld = _ret._world * _ret._local.Invert();

			negativeScale = true;

			// Decompose 했다가 scale -주고 다시 합쳐야함..
			math::Vector3 _scale;
			math::Quaternion _rotQuat;
			math::Vector3 _trans;
			_ret._local.Decompose(_scale, _rotQuat, _trans);

			if (_scale.x < 0) _scale.x *= -1;
			if (_scale.y < 0) _scale.y *= -1;
			if (_scale.z < 0) _scale.z *= -1;

			// 다시 SRT 조립
			_ret._local = math::Matrix::CreateScale(_scale) * math::Matrix::CreateFromQuaternion(_rotQuat) * math::Matrix::CreateTranslation(_trans);

			_ret._world = _parentWorld * _ret._local;
		}

		return _ret;
	}

	void GetBoundingBox(MeshBin& meshBin, fbxsdk::FbxMesh* meshNode)
	{
		meshNode->ComputeBBox();

		auto _mixBox = Convert(meshNode->BBoxMin);;
		auto _maxBox = Convert(meshNode->BBoxMax);;

		meshBin._boundingMinBox.x = meshBin._boundingMinBox.x < _mixBox.x ? meshBin._boundingMinBox.x : _mixBox.x;
		meshBin._boundingMinBox.y = meshBin._boundingMinBox.y < _mixBox.y ? meshBin._boundingMinBox.y : _mixBox.y;
		meshBin._boundingMinBox.z = meshBin._boundingMinBox.z < _mixBox.z ? meshBin._boundingMinBox.z : _mixBox.z;
		meshBin._boundingMaxBox.x = meshBin._boundingMaxBox.x > _maxBox.x ? meshBin._boundingMaxBox.x : _maxBox.x;
		meshBin._boundingMaxBox.y = meshBin._boundingMaxBox.y > _maxBox.y ? meshBin._boundingMaxBox.y : _maxBox.y;
		meshBin._boundingMaxBox.z = meshBin._boundingMaxBox.z > _maxBox.z ? meshBin._boundingMaxBox.z : _maxBox.z;
	}

	void GetBone(fbxsdk::FbxSkin* fbxSkin, fbxsdk::FbxMesh* fbxMesh, std::vector<VertexAttribute>& vertexList, FBXModel& model)
	{
		fbxsdk::FbxSkin::EType _type = fbxSkin->GetSkinningType();

		if (_type == fbxsdk::FbxSkin::eRigid || fbxsdk::FbxSkin::eLinear)
		{
			const int _clusterCount = fbxSkin->GetClusterCount();

			for (int j = 0; j < _clusterCount; j++)
			{
				fbxsdk::FbxCluster* _cluster = fbxSkin->GetCluster(j);

				if (_cluster->GetLink() == nullptr)
					continue;

				int _boneIdx = FindBoneIndex(_cluster->GetLink()->GetName(), model);
				assert(_boneIdx >= 0);

				const int _indicesCount = _cluster->GetControlPointIndicesCount();

				for (int k = 0; k < _indicesCount; k++)
				{
					auto _weight = _cluster->GetControlPointWeights()[k];

					int _vtxIdx = _cluster->GetControlPointIndices()[k];

					for (int weightIdx = 0; weightIdx < 8; weightIdx++)
					{
						if (vertexList[_vtxIdx]._bone[weightIdx] == -1)
						{
							vertexList[_vtxIdx]._weight[weightIdx] = static_cast<float>(_weight);

							vertexList[_vtxIdx]._bone[weightIdx] = _boneIdx;

							break;
						}
					}
				}

				fbxsdk::FbxAMatrix geometryTransform = GetTransformMatrix(fbxMesh->GetNode());
				fbxsdk::FbxAMatrix transformMatrix;
				fbxsdk::FbxAMatrix transformLinkMatrix;
				fbxsdk::FbxAMatrix globalBindposeInverseMatrix;

				_cluster->GetTransformMatrix(transformMatrix);
				// The transformation of the mesh at binding time 
				_cluster->GetTransformLinkMatrix(transformLinkMatrix);
				// The transformation of the cluster(joint) at binding time from joint space to world space 
				globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometryTransform;
				math::Matrix _globalBindposeInverseMatrix = Convert(globalBindposeInverseMatrix, true);

				fbxsdk::FbxAMatrix matClusterTransformMatrix;
				fbxsdk::FbxAMatrix matClusterLinkTransformMatrix;

				_cluster->GetTransformMatrix(matClusterTransformMatrix);
				_cluster->GetTransformLinkMatrix(matClusterLinkTransformMatrix);

				// Bone Matrix 설정..
				math::Matrix _clusterMatrix = Convert(matClusterTransformMatrix, true);
				math::Matrix _clusterlinkMatrix = Convert(matClusterLinkTransformMatrix, true);

				// BindPose 행렬을 구하자
				fbxsdk::FbxAMatrix _geometryTransform = GetTransformMatrix(fbxMesh->GetNode());
				math::Matrix _geometryMatrix = Convert(_geometryTransform, true);

				// OffsetMatrix는 WorldBindPose의 역행렬
				math::Matrix _offsetMatrix = _clusterMatrix * _clusterlinkMatrix.Invert();//* _geometryMatrix;

				model._bones[_boneIdx]._offsetMatrix = _offsetMatrix;
			}
		}
	}

	math::Vector3 GetNormal(fbxsdk::FbxMesh* fbxMesh, uint32 controlPointIndex, uint32 vertexCounter, bool negativeScale)
	{
		math::Vector3 _normal;

		if (fbxMesh->GetElementNormalCount() >= 1)
		{
			FbxGeometryElementNormal* normal = fbxMesh->GetElementNormal();
			int normalIdx = 0;

			// 인덱스를 기준으로 노멀 값이 들어간다
			// 버텍스 스플릿이 필요하다.
			if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
					normalIdx = vertexCounter;
				else
					normalIdx = normal->GetIndexArray().GetAt(vertexCounter);
			}
			// 정점을 기준으로 노멀 값이 들어간다.
			else if (normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
					normalIdx = controlPointIndex;
				else
					normalIdx = normal->GetIndexArray().GetAt(controlPointIndex);
			}

			FbxVector4 vec = normal->GetDirectArray().GetAt(normalIdx);

			if (negativeScale)
			{
				_normal.x = -static_cast<float>(vec.mData[0]);
				_normal.y = -static_cast<float>(vec.mData[2]);
				_normal.z = -static_cast<float>(vec.mData[1]);
			}
			else
			{
				_normal.x = static_cast<float>(vec.mData[0]);
				_normal.y = static_cast<float>(vec.mData[2]);
				_normal.z = static_cast<float>(vec.mData[1]);
			}

			return _normal;
		}

		_normal = { -1.f, -1.f, -1.f };

		return _normal;
	}

	void GetTangent(MeshBin& meshBin, int meshCnt)
	{
		for (int i = 0; i < meshBin._indices[meshCnt].size(); i += 3) // 삼각형의 개수
		{
			int _i0 = meshBin._indices[meshCnt][i];
			int _i1 = meshBin._indices[meshCnt][i + (int)1];
			int _i2 = meshBin._indices[meshCnt][i + (int)2];

			// e1 = p1 - p0, e2 = p2 - p0
			math::Vector3 _e1 = meshBin._vertices[_i1]._position - meshBin._vertices[_i0]._position;
			math::Vector3 _e2 = meshBin._vertices[_i2]._position - meshBin._vertices[_i0]._position;

			float _x1 = meshBin._vertices[_i1]._uv.x - meshBin._vertices[_i0]._uv.x; // u1 - u0
			float _y1 = meshBin._vertices[_i1]._uv.y - meshBin._vertices[_i0]._uv.y; // v1 - v0
			float _x2 = meshBin._vertices[_i2]._uv.y - meshBin._vertices[_i0]._uv.x; // u2 - u0
			float _y2 = meshBin._vertices[_i2]._uv.y - meshBin._vertices[_i0]._uv.y; // v2 - v0

			float _det = (_x1 * _y2) - (_x2 * _y1); // a * d - b * c
			if (_det == 0.f) { _det = 0.00001f; }
			float _r = 1.0f / _det;

			math::Vector3 _tangent = (_e1 * _y2 - _e2 * _y1) * _r;

			meshBin._vertices[_i0]._tangent += _tangent;
			meshBin._vertices[_i1]._tangent += _tangent;
			meshBin._vertices[_i2]._tangent += _tangent;
		}
	}

	math::Vector2 GetUV(fbxsdk::FbxMesh* fbxMesh, int controlPointIndex, int vertexCounter)
	{
		math::Vector2 _uv;

		if (fbxMesh->GetElementUVCount() >= 1)
		{
			int _uvIndex = 0;

			if (fbxMesh->GetElementUVCount() != 1)
				int a = 0;

			// Texture channel 이 여러개인 경우가 있다고 한다.
			fbxsdk::FbxGeometryElementUV* _fbxUV2 = fbxMesh->GetLayer(0)->GetUVs();
			fbxsdk::FbxGeometryElementUV* _fbxUV = fbxMesh->GetElementUV(0);


			if (_fbxUV->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex)
			{
				if (_fbxUV->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
					_uvIndex = vertexCounter;
				else
					_uvIndex = _fbxUV->GetIndexArray().GetAt(vertexCounter);
			}
			else if (_fbxUV->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint)
			{
				if (_fbxUV->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
					_uvIndex = controlPointIndex;
				else
					_uvIndex = _fbxUV->GetIndexArray().GetAt(controlPointIndex);
			}

			_uv.x = static_cast<float>(_fbxUV->GetDirectArray().GetAt(_uvIndex).mData[0]);
			_uv.y = 1.f - static_cast<float>(_fbxUV->GetDirectArray().GetAt(_uvIndex).mData[1]);

			if (_uv.x < 0)
				_uv.x += 1;

			return _uv;
		}

		_uv = { 0.f, 0.f };

		return _uv;
	}

	void LoadMesh(fbxsdk::FbxMesh* meshNode, FBXModel& model, MeshBin& mesh, uint32 subMeshCnt)
	{
		GetBoundingBox(mesh, meshNode);


		auto _scale = meshNode->GetNode()->LclScaling.Get();

		static int num = 0;
		mesh._name = meshNode->GetName();

		uint32 _vertexCount = meshNode->GetControlPointsCount();

		std::vector<VertexAttribute> _tempMeshVertexList;
		_tempMeshVertexList.resize(_vertexCount);

		fbxsdk::FbxVector4* _controlPoints = meshNode->GetControlPoints();

		for (uint32 i = 0; i < _vertexCount; i++)
		{
			_tempMeshVertexList[i]._position =
			{
				static_cast<float>(_controlPoints[i].mData[0]),
				static_cast<float>(_controlPoints[i].mData[2]),
				static_cast<float>(_controlPoints[i].mData[1])
			};
		}

		const int _deformerCount = meshNode->GetDeformerCount(fbxsdk::FbxDeformer::eSkin);

		for (int i = 0; i < _deformerCount; i++)
		{
			mesh._isSkinned = true;

			mesh._boneName = model._name;

			fbxsdk::FbxSkin* _fbxSkin = static_cast<fbxsdk::FbxSkin*>(meshNode->GetDeformer(i, fbxsdk::FbxDeformer::eSkin));

			if (_fbxSkin != nullptr)
			{
				GetBone(_fbxSkin, meshNode, _tempMeshVertexList, model);
			}
		}

		const int polygonSize = meshNode->GetPolygonSize(0);

		assert(polygonSize == 3);		// 삼각형이 아니면 오류

		int arrIdx[3];
		int vertexCounter = 0;			// 정점의 개수

		const int triCount = meshNode->GetPolygonCount(); // 메쉬의 삼각형 개수를 가져온다

		std::map<std::tuple<unsigned, float, float, float, float, float>, unsigned> indexMap;

		/*if (!meshNode->CheckIfVertexNormalsCCW())
		{
			m_IsNegativeScaleFlag = false;
		}
		else
		{
			m_IsNegativeScaleFlag = true;
		}*/

		for (int i = 0; i < triCount; i++) // 삼각형의 개수
		{
			for (int j = 0; j < 3; j++)	   // 삼각형은 세 개의 정점으로 구성
			{
				int controlPointIndex = meshNode->GetPolygonVertex(i, j); // 제어점의 인덱스 추출

				arrIdx[j] = controlPointIndex;

				math::Vector3 _normal = GetNormal(meshNode, controlPointIndex, vertexCounter, _bIsNegativeScaleFlag);

				math::Vector2 _uv = GetUV(meshNode, controlPointIndex, vertexCounter);

				const auto indexPair = std::make_tuple(controlPointIndex, _uv.x, _uv.y, _normal.x, _normal.y, _normal.z);

				const auto iter = indexMap.find(indexPair);

				// map 에 없으면 insert 및 새로운 버텍스 제작
				if (iter == indexMap.end())
				{
					VertexAttribute _vertex;
					_vertex._position = _tempMeshVertexList[controlPointIndex]._position;	// 포지션은 동일

					// 가중치 정보 동일
					for (int weightIdx = 0; weightIdx < 4; weightIdx++)
					{
						_vertex._weight[weightIdx] = _tempMeshVertexList[controlPointIndex]._weight[weightIdx];

						_vertex._bone[weightIdx] = _tempMeshVertexList[controlPointIndex]._bone[weightIdx];
					}

					_vertex._uv = _uv;

					_vertex._normal = _normal;

					mesh._vertices.push_back(_vertex);								// 새로운 버텍스 삽입

					controlPointIndex = static_cast<int>(mesh._vertices.size()) - 1;				// index 새로운 버텍스 껄로 바꾸기

					arrIdx[j] = controlPointIndex;

					indexMap.insert({ indexPair, controlPointIndex });
				}
				else // map 에 있던거라면
				{
					arrIdx[j] = iter->second;
				}

				vertexCounter++;
			}

			//if (meshNode->CheckIfVertexNormalsCCW())
			{
				mesh._indices[subMeshCnt].push_back(arrIdx[0]);
				mesh._indices[subMeshCnt].push_back(arrIdx[2]);
				mesh._indices[subMeshCnt].push_back(arrIdx[1]);
			}
			/*else
			{
				mesh._indexAttributes[subMeshCnt].push_back(arrIdx[0]);
				mesh._indexAttributes[subMeshCnt].push_back(arrIdx[2]);
				mesh._indexAttributes[subMeshCnt].push_back(arrIdx[1]);
			}*/
		}

		// tangent 정보를 가져온다.
		if (meshNode->GetElementNormalCount() >= 1)
			GetTangent(mesh, subMeshCnt);
	}

	string GetTextureRelativeName(fbxsdk::FbxSurfaceMaterial* surface, const char* materialProperty)
	{
		std::string _textureName;

		FbxProperty textureProperty = surface->FindProperty(materialProperty);

		if (textureProperty.IsValid())
		{
			int count = textureProperty.GetSrcObjectCount();

			if (1 <= count)
			{
				FbxFileTexture* texture = textureProperty.GetSrcObject<FbxFileTexture>(0);
				if (texture)
					_textureName = texture->GetRelativeFileName();
			}
		}

		std::filesystem::path _path(_textureName);

		return _path.filename().string();
	}

	void LoadMaterial(fbxsdk::FbxSurfaceMaterial* surfaceMaterial, FBXModel& model, MeshBin& mesh)
	{
		string _matName = surfaceMaterial->GetName();

		//mesh._materials.push_back(_matName);

		auto _find = model._materialMap.find(surfaceMaterial->GetName());

		if (_find != model._materialMap.end())
		{
			return;
		}

		MaterialBin _material;

		_material._materialName = _matName;

		if (surfaceMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
		{
			FbxSurfacePhong* _phongMaterial = reinterpret_cast<FbxSurfacePhong*>(surfaceMaterial);

			// Ambient Data
			_material._ambient.x = static_cast<float>(_phongMaterial->Ambient.Get()[0]) * 10.0f;
			_material._ambient.y = static_cast<float>(_phongMaterial->Ambient.Get()[1]) * 10.0f;
			_material._ambient.z = static_cast<float>(_phongMaterial->Ambient.Get()[2]) * 10.0f;
			_material._ambient.w = 1.0f;

			// Diffuse Data
			_material._diffuse.x = static_cast<float>(_phongMaterial->Diffuse.Get()[0]);
			_material._diffuse.y = static_cast<float>(_phongMaterial->Diffuse.Get()[1]);
			_material._diffuse.z = static_cast<float>(_phongMaterial->Diffuse.Get()[2]);
			_material._diffuse.w = 1.0f;

			// Specular Data
			_material._specular.x = static_cast<float>(_phongMaterial->Specular.Get()[0]);
			_material._specular.y = static_cast<float>(_phongMaterial->Specular.Get()[1]);
			_material._specular.z = static_cast<float>(_phongMaterial->Specular.Get()[2]);
			_material._specular.w = 1.0f;

			// Emissive Datam_FbxScene
			_material._emissive.x = static_cast<float>(_phongMaterial->Emissive.Get()[0]);
			_material._emissive.y = static_cast<float>(_phongMaterial->Emissive.Get()[1]);
			_material._emissive.z = static_cast<float>(_phongMaterial->Emissive.Get()[2]);
			_material._emissive.w = 1.0f;

			// Emissive Strength
			_material._emissionFactor = static_cast<float>(_phongMaterial->EmissiveFactor);

			// Transparency Data
			_material._transparency = static_cast<float>(_phongMaterial->TransparencyFactor.Get());

			// Shininess Data
			_material._roughness = 1.0f - float(sqrt(fmax(static_cast<float>(_phongMaterial->Shininess.Get()), 0.0)) / 10.0);

			// 메탈이 맞을까?
			_material._metallic = static_cast<float>(_phongMaterial->ReflectionFactor.Get());

			// Reflectivity Data
			_material._reflectivity = static_cast<float>(_phongMaterial->ReflectionFactor.Get());
		}
		else if (surfaceMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
		{
			FbxSurfaceLambert* _lambertMaterial = reinterpret_cast<FbxSurfaceLambert*>(surfaceMaterial);

			// Ambient Data
			_material._ambient.x = static_cast<float>(_lambertMaterial->Ambient.Get()[0]);
			_material._ambient.y = static_cast<float>(_lambertMaterial->Ambient.Get()[1]);
			_material._ambient.z = static_cast<float>(_lambertMaterial->Ambient.Get()[2]);
			_material._ambient.w = 1.0f;

			// Diffuse Data
			_material._diffuse.x = static_cast<float>(_lambertMaterial->Diffuse.Get()[0]);
			_material._diffuse.y = static_cast<float>(_lambertMaterial->Diffuse.Get()[1]);
			_material._diffuse.z = static_cast<float>(_lambertMaterial->Diffuse.Get()[2]);
			_material._diffuse.w = 1.0f;

			// Emissive Data
			_material._specular.x = static_cast<float>(_lambertMaterial->Emissive.Get()[0]);
			_material._specular.y = static_cast<float>(_lambertMaterial->Emissive.Get()[1]);
			_material._specular.z = static_cast<float>(_lambertMaterial->Emissive.Get()[2]);
			_material._specular.w = 1.0f;

			// Emissive Strength
			_material._emissionFactor = static_cast<float>(_lambertMaterial->EmissiveFactor);

			// Transparency Data
			_material._transparency = static_cast<float>(_lambertMaterial->TransparencyFactor.Get());
		}

		_material._albedoMapTexture = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sDiffuse);
		_material._normalMapTexture = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sNormalMap);
		_material._metallicRoughnessMapTexture = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sShininess);
		_material._emissiveMapTexture = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sEmissive);

		// 머터리얼 리스트에 추가
		model._materialMap.insert(std::make_pair(_matName, _material));
	}

	void TraversalNode(fbxsdk::FbxNode* node, FBXModel& model)
	{
		fbxsdk::FbxNodeAttribute* _nodeAttribute = node->GetNodeAttribute();

#if defined(_DEBUG) || defined(DEBUG)
		auto _nodeName = node->GetName();

		auto _scale = node->LclScaling.Get();



		if (_nodeAttribute && _nodeAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
		{
			BoneBin _bonebin;

			_bonebin._boneName = node->GetName();

			_bonebin._parentBoneName = node->GetParent()->GetName();

			_bonebin._parentIndex = FindBoneIndex(node->GetParent()->GetName(), model);

			_bonebin._localMatrix = GetLocalMatrix(node);
			_bonebin._worldMatrix = GetWorldMatrix(node);

			model._bones.push_back(_bonebin);

			LoadAnimationData(node, model);
		}
#endif
		if (_nodeAttribute != nullptr && _nodeAttribute->GetAttributeType() == FbxNodeAttribute::EType::eNull)
		{
			Node _node = GetNode(node, _bIsNegativeScaleFlag);
		}

		if (_nodeAttribute != nullptr && _nodeAttribute->GetAttributeType() == FbxNodeAttribute::EType::eMesh)
		{
			Node _node = GetNode(node, _bIsNegativeScaleFlag);

			MeshBin _meshBin;

			_meshBin._indices.resize(node->GetNodeAttributeCount());

			for (int _meshCnt = 0; _meshCnt < node->GetNodeAttributeCount(); _meshCnt++)
			{
				fbxsdk::FbxMesh* _meshNode = static_cast<fbxsdk::FbxMesh*>(node->GetNodeAttributeByIndex(_meshCnt));

				LoadMesh(_meshNode, model, _meshBin, _meshCnt);

				std::string _meshName = _meshNode->GetName();

				if (_meshName == "")
				{
					ReconstructionName(_node._name, ":");

					_node._mesh = _node._name;
					_meshBin._name = _node._name;
				}
				else
				{
					_node._mesh = _meshNode->GetName();
				}

				fbxsdk::FbxLayerElementMaterial* _findMatIndex = _meshNode->GetElementMaterial(0);

				if (_findMatIndex != nullptr)
				{
					int index = _findMatIndex->GetIndexArray().GetAt(0);

					fbxsdk::FbxSurfaceMaterial* surfaceMaterial = _meshNode->GetNode()->GetSrcObject<fbxsdk::FbxSurfaceMaterial>(index);

					LoadMaterial(surfaceMaterial, model, _meshBin);
				}
			}

			_bIsNegativeScaleFlag = false;

			model._meshMap.insert(std::make_pair(_meshBin._name, _meshBin));

			model._nodes.emplace_back(_node);
		}

		const int _childCount = node->GetChildCount();

		for (int i = 0; i < _childCount; i++)
		{
			TraversalNode(node->GetChild(i), model);
		}
	}

	void FBXImporter::Import(const tstring& path)
	{
		Initialize();

		LoadFile(path);

		FBXModel _model;

		FindAnimationData(_model);

		fbxsdk::FbxNode* _rootNode = g_pFbxScene->GetRootNode();

		TraversalNode(_rootNode, _model);

		vector<rengine::Object*> _assets;
		
		for (auto& _iter : _model._meshMap)
		{
			auto _bin = _iter.second;

			tstring _binPath = g_assetPath + TEXT("bin\\mesh\\") + StringHelper::StringToWString(_iter.first) + TEXT(".mesh");

			auto _resource = rengine::Resources::GetInstance()->CreateResource<rengine::Mesh>();

			_resource->SetNameStr(_iter.first);

			_resource->SetPath(_binPath);

			_resource->SetVertices(_bin._vertices);
			_resource->Setindices(_bin._indices);
			_resource->SetBoundingBoxMin(_bin._boundingMinBox);
			_resource->SetBoundingBoxMax(_bin._boundingMaxBox);

			_resource->SetBoneName(StringHelper::StringToWString(_bin._boneName));
			_resource->SetIsSkinned(_bin._isSkinned);

			utility::Serializer::CreateMetaInfo(_resource->GetPath(), _resource.get());

			utility::Serializer::Serialize(_resource->GetPath(), _resource.get());

			_assets.push_back(_resource.get());
			//_assets.push_back(make_pair(_resource->GetUUID(), _resource->GetPath()));
		}

		for (auto& _bin : _model._animationClips)
		{
			tstring _binPath = g_assetPath + TEXT("bin\\anim\\") + StringHelper::StringToWString(_bin._clipName) + TEXT(".anim");

			auto _resource = rengine::Resources::GetInstance()->CreateResource<rengine::AnimationClip>();

			_resource->SetNameStr(_bin._clipName);

			_resource->SetPath(_binPath);

			_resource->SetFrameRate(_bin._frameRate);
			_resource->SetTickPerFrame(_bin._tickPerFrame);
			_resource->SetTotalKeyFrame(_bin._totalKeyFrame);
			_resource->SetStartKeyFrame(_bin._startKeyFrame);
			_resource->SetEndKeyFrame(_bin._endKeyFrame);

			float _totalTime = (float)(_bin._totalKeyFrame - 1) / _bin._frameRate;

			vector<rengine::AnimationSnap> _snaps;

			for (auto& _snapBin : _bin._snapList)
			{
				_snaps.push_back(_snapBin.Convert());
			}

			_resource->SetSnaps(_snaps);

			utility::Serializer::CreateMetaInfo(_resource->GetPath(), _resource.get());

			utility::Serializer::Serialize(_resource->GetPath(), _resource.get());

			_assets.push_back(_resource.get());
			//_assets.push_back(make_pair(_resource->GetUUID(), _resource->GetPath()));
		}

		for (auto& _bin : _model._materialMap)
		{
			tstring _binPath = g_assetPath + TEXT("bin\\mat\\") + StringHelper::StringToWString(_bin.first) + TEXT(".mat");

			auto _resource = rengine::Resources::GetInstance()->CreateResource<rengine::Material>();

			_resource->SetNameStr(_bin.first);

			_resource->SetPath(_binPath);

			/*auto _albedoMapTex = rengine::Resources::GetInstance()->Load<rengine::Texture>(StringHelper::StringToWString(_bin.second._albedoMapTexture));
			auto _normalMapTex = rengine::Resources::GetInstance()->Load<rengine::Texture>(StringHelper::StringToWString(_bin.second._normalMapTexture));
			auto _metallicRougnessMapTex = rengine::Resources::GetInstance()->Load<rengine::Texture>(StringHelper::StringToWString(_bin.second._metallicRoughnessMapTexture));
			auto _emissiveMapTex = rengine::Resources::GetInstance()->Load<rengine::Texture>(StringHelper::StringToWString(_bin.second._emissiveMapTexture));
			auto _ambientMapTex = rengine::Resources::GetInstance()->Load<rengine::Texture>(StringHelper::StringToWString(_bin.second._ambientMapTexture));
			auto _specularMapTex = rengine::Resources::GetInstance()->Load<rengine::Texture>(StringHelper::StringToWString(_bin.second._specularMapTexture));

			vector<rengine::MaterialProperty> _properties =
			{
				{ TEXT("Albedo Map"),				_albedoMapTex },
				{ TEXT("Normal Map"),				_normalMapTex },
				{ TEXT("Metallic Rougness Map"),	_metallicRougnessMapTex },
				{ TEXT("Emissive Map"),				_emissiveMapTex },
				{ TEXT("Ambient Map"),				_ambientMapTex },
				{ TEXT("Specular Map"),				_specularMapTex },
				{ TEXT("Albedo"),					_bin.second._diffuse },
				{ TEXT("Ambient"),					_bin.second._ambient },
				{ TEXT("Specular"),					_bin.second._specular },
				{ TEXT("Emissive"),					_bin.second._emissive },
				{ TEXT("Metallic"),					_bin.second._metallic },
				{ TEXT("Roughness"),				_bin.second._roughness },
				{ TEXT("EmissionFactor"),			_bin.second._emissionFactor },
				{ TEXT("Transparency"),				_bin.second._transparency },
				{ TEXT("Reflectivity"),				_bin.second._reflectivity }
			};
			
			_resource->AddProperties(_properties);*/

			auto _albedoMapTex = rengine::Resources::GetInstance()->Load<rengine::Texture>(StringHelper::StringToWString(_bin.second._albedoMapTexture));
			auto _normalMapTex = rengine::Resources::GetInstance()->Load<rengine::Texture>(StringHelper::StringToWString(_bin.second._normalMapTexture));
			auto _metallicRougnessMapTex = rengine::Resources::GetInstance()->Load<rengine::Texture>(StringHelper::StringToWString(_bin.second._metallicRoughnessMapTexture));
			auto _emissiveMapTex = rengine::Resources::GetInstance()->Load<rengine::Texture>(StringHelper::StringToWString(_bin.second._emissiveMapTexture));
			auto _ambientMapTex = rengine::Resources::GetInstance()->Load<rengine::Texture>(StringHelper::StringToWString(_bin.second._ambientMapTexture));

			/*vector<rengine::MaterialProperty> _properties =
			{
				{ TEXT("AlbedoMap"),				_albedoMapTex },
				{ TEXT("NormalMap"),				_normalMapTex },
				{ TEXT("MetallicRougnessMap"),	_metallicRougnessMapTex },
				{ TEXT("EmissiveMap"),				_emissiveMapTex },
				{ TEXT("AmbientOcclusionMap"),				_ambientMapTex },
				{ TEXT("_albedoColor"),					_bin.second._diffuse },
				{ TEXT("Ambient"),					_bin.second._ambient },
				{ TEXT("Specular"),					_bin.second._specular },
				{ TEXT("Emissive"),					_bin.second._emissive },
				{ TEXT("_roughness"),				_bin.second._roughness },
				{ TEXT("_metallic"),					_bin.second._metallic },
				{ TEXT("_bumpScale"),			_bin.second._emissionFactor },
				{ TEXT("Transparency"),				_bin.second._transparency },
				{ TEXT("Reflectivity"),				_bin.second._reflectivity }
			};

			_resource->AddProperties(_properties);*/

			_resource->SetRenderPassID(TEXT("Deferred Pass"));

			_resource->SetPipelineID(TEXT("Standard"));

			if(_albedoMapTex != nullptr)			_resource->SetTexture(TEXT("AlbedoMap"), _albedoMapTex);
			if(_normalMapTex != nullptr)			_resource->SetTexture(TEXT("NormalMap"), _normalMapTex);
			if(_metallicRougnessMapTex != nullptr)	_resource->SetTexture(TEXT("MetallicRougnessMap"), _metallicRougnessMapTex);
			if(_emissiveMapTex != nullptr)			_resource->SetTexture(TEXT("EmissiveMap"), _emissiveMapTex);
			if(_ambientMapTex != nullptr)			_resource->SetTexture(TEXT("AmbientOcclusionMap"), _ambientMapTex);

			_resource->SetColor(TEXT("_albedoColor"), _bin.second._diffuse);
			_resource->SetFloat(TEXT("_roughness"), _bin.second._roughness);
			_resource->SetFloat(TEXT("_metallic"), _bin.second._metallic);
			_resource->SetColor(TEXT("_emissionColor"), _bin.second._emissive);

			utility::Serializer::CreateMetaInfo(_resource->GetPath(), _resource.get());

			utility::Serializer::Serialize(_resource->GetPath(), _resource.get());

			_assets.push_back(_resource.get());
		}

		utility::Serializer::CreateMetaInfoModel(path, _assets);

	}
}