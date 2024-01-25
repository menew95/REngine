#include <Importer_pch.h>

#include <importer\FBXImporter.h>

#include <importer\FBXModel.h>

#define FBXSDK_SHARED

#include <fbx\fbxsdk.h>

std::unique_ptr<fbxsdk::FbxManager, void(*)(fbxsdk::FbxManager*)> g_pFbxManager{ nullptr, [](fbxsdk::FbxManager* ptr) {} };
std::unique_ptr<fbxsdk::FbxImporter, void(*)(fbxsdk::FbxImporter*)> g_pFbxImporter{ nullptr, [](fbxsdk::FbxImporter* ptr) {} };
std::unique_ptr<fbxsdk::FbxScene, void(*)(fbxsdk::FbxScene*)> g_pFbxScene{ nullptr, [](fbxsdk::FbxScene* ptr) {} };
std::unique_ptr<fbxsdk::FbxScene, void(*)(fbxsdk::FbxScene*)> m_FbxScene{ nullptr, [](fbxsdk::FbxScene* ptr) {} };

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
		fbxsdk::FbxMatrix _localPos = m_FbxScene->GetAnimationEvaluator()->GetNodeLocalTransform(node);

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
		fbxsdk::FbxMatrix _globalPos = m_FbxScene->GetAnimationEvaluator()->GetNodeGlobalTransform(node);

		math::Matrix _worldTM = Convert(_globalPos, lh);

		Rotate(_worldTM);

		return _worldTM;
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

		bool _result;

		if (!_path.IsEmpty())
		{
			_result = LoadScene(_path.Buffer());
		}

		fbxsdk::FbxSystemUnit lFbxFileSystemUnit = g_pFbxScene->GetGlobalSettings().GetSystemUnit();
		fbxsdk::FbxSystemUnit lFbxOriginSystemUnit = g_pFbxScene->GetGlobalSettings().GetOriginalSystemUnit();

		double factor = lFbxOriginSystemUnit.GetScaleFactor();

		const fbxsdk::FbxSystemUnit::ConversionOptions lConversionOptions =
		{
		  true,
		  true,
		  true,
		  true,
		  true,
		  true
		};

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
		m_FbxScene->FillAnimStackNameArray(OUT _animNames);

		const int animCount = _animNames.GetCount();

		for (int i = 0; i < animCount; i++)
		{
			fbxsdk::FbxAnimStack* animStack = m_FbxScene->FindMember<fbxsdk::FbxAnimStack>(_animNames[i]->Buffer());

			if (animStack == nullptr)
				continue;

			AnimationClipBin _newClip;
			_newClip._clipName = animStack->GetName();
			ReconstructionName(_newClip._clipName, "|");

			// 시작시간, 종료시간, 초당 프레임에 대한 정보
			fbxsdk::FbxTakeInfo* takeInfo = m_FbxScene->GetTakeInfo(animStack->GetName());
			double _startTime = takeInfo->mLocalTimeSpan.GetStart().GetSecondDouble();
			double _endTime = takeInfo->mLocalTimeSpan.GetStop().GetSecondDouble();
			double _frameRate = (float)FbxTime::GetFrameRate(m_FbxScene->GetGlobalSettings().GetTimeMode());

			_newClip._frameRate = static_cast<float>(_frameRate);

			if (_startTime < _endTime)
			{
				_newClip._totalKeyFrame = (int)((_endTime - _startTime) * (double)_frameRate);
				_newClip._endKeyFrame = (int)((_endTime - _startTime) * (double)_frameRate);
				_newClip._tickPerFrame = static_cast<uint32>((_endTime - _startTime) / _newClip._totalKeyFrame);
				_newClip._startKeyFrame = (int)(_startTime)*_newClip._totalKeyFrame;
			}

			model._animationClipDatas.push_back(_newClip);
		}
	}

	int32 FindBoneIndex(std::string boneName, FBXModel& model)
	{
		auto _iter = std::find_if(std::begin(model._boneDatas), std::end(model._boneDatas),
			[&boneName](auto& v)
			{
				return v._boneName == boneName;
			}
		);

		if (_iter != model._boneDatas.end())
		{
			return static_cast<int32>(_iter - model._boneDatas.begin());
		}
		else
		{
			return -1;
		}
	}

	void TraversalNode(fbxsdk::FbxNode* node, FBXModel& model)
	{
		fbxsdk::FbxNodeAttribute* _nodeAttribute = node->GetNodeAttribute();

		bool m_IsNegativeScaleFlag;

#if defined(_DEBUG) || defined(DEBUG)
		auto _nodeName = node->GetName();

		if (_nodeAttribute && _nodeAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
		{
			BoneBin _bonebin;

			_bonebin._boneName = node->GetName();

			_bonebin._parentBoneName = node->GetParent()->GetName();

			_bonebin._parentIndex = FindBoneIndex(node->GetParent()->GetName(), model);

			_bonebin._localMatrix = GetLocalMatrix(node);
			_bonebin._worldMatrix = GetWorldMatrix(node);

			/*Rotate(_newBoneData._localTM);
			Rotate(_newBoneData._worldTM);*/


			model._boneDatas.push_back(_bonebin);

			//LoadAnimationData(node, model);
		}
#endif
		if (_nodeAttribute != nullptr && _nodeAttribute->GetAttributeType() == FbxNodeAttribute::EType::eNull)
		{
			Node _node;

			_node._name = node->GetName();

			auto* _parentNode = node->GetParent();

			if (_parentNode != nullptr)
			{
				_node._hasParent = true;
				_node._parent = _parentNode->GetName();
			}

			_node._local = GetLocalMatrix(node);
			_node._world = GetWorldMatrix(node);

			model._nodes.emplace_back(_node);
		}

		if (_nodeAttribute != nullptr && _nodeAttribute->GetAttributeType() == FbxNodeAttribute::EType::eMesh)
		{
			Node _node;

			_node._name = node->GetName();

			auto* _parentNode = node->GetParent();

			if (_parentNode != nullptr)
			{
				_node._hasParent = true;
				_node._parent = _parentNode->GetName();
			}

			_node._local = GetLocalMatrix(node);
			_node._world = GetWorldMatrix(node);

			_node._hasMesh = true;

			float _det = _node._local.Determinant();

			// Negative scale 일 경우 negative scale flag 를 true로 설정
			if (_det < 0)
			{
				math::Matrix _parentWorld = _node._world * _node._local.Invert();

				m_IsNegativeScaleFlag = true;
				// Decompose 했다가 scale -주고 다시 합쳐야함..
				math::Vector3 _scale;
				math::Quaternion _rotQuat;
				math::Vector3 _trans;
				_node._local.Decompose(_scale, _rotQuat, _trans);

				if (_scale.x < 0) _scale.x *= -1;
				if (_scale.y < 0) _scale.y *= -1;
				if (_scale.z < 0) _scale.z *= -1;

				// 다시 SRT 조립
				_node._local = math::Matrix::CreateScale(_scale) * math::Matrix::CreateFromQuaternion(_rotQuat) * math::Matrix::CreateTranslation(_trans);

				_node._world = _parentWorld * _node._local;
			}

			MeshBin _newMeshData;

			_newMeshData.indices.resize(node->GetNodeAttributeCount());

			for (int _meshCnt = 0; _meshCnt < node->GetNodeAttributeCount(); _meshCnt++)
			{
				fbxsdk::FbxMesh* _meshNode = static_cast<fbxsdk::FbxMesh*>(node->GetNodeAttributeByIndex(_meshCnt));

				//LoadMesh(prefabData, _meshNode, _newMeshData, _meshCnt);

				std::string _meshName = _meshNode->GetName();

				if (_meshName == "")
				{
					ReconstructionName(_node._name, ":");

					_node._mesh = _node._name;
					_newMeshData.meshName = _node._name;

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

					//LoadMaterial(surfaceMaterial, prefabData, _newMeshData);
				}
			}

			m_IsNegativeScaleFlag = false;

			model._meshDataMap.insert(std::make_pair(_newMeshData.meshName, _newMeshData));

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

	}
}