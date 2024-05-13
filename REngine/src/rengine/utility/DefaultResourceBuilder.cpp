#include <rengine\utility\DefaultResourceBuilder.h>

#include <rengine\core\Resources.h>
#include <rengine\core\resource\Mesh.h>
#include <rengine\core\resource\Material.h>

#include <Serialize\Serializer.h>
#include <common\AssetPath.h>

#include <filesystem>

namespace fs = std::filesystem;

namespace rengine
{
	void DefaultSphereBuild()
	{
		float _radius = 0.5f;	// 구의 반지름
		uint32 _stackCount = 20; // 가로 분할
		uint32 _sliceCount = 20; // 세로 분할

		std::vector<VertexAttribute> _vertices;

		VertexAttribute _vertex;

		// 북극
		_vertex._position = Vector3(0.0f, _radius, 0.0f);
		_vertex._uv = Vector2(0.5f, 0.0f);
		_vertex._normal = _vertex._position;
		XMVector3Normalize(_vertex._normal);
		_vertex._normal.Normalize();
		_vertex._tangent = Vector3(1.0f, 0.0f, 1.0f);
		_vertices.push_back(_vertex);

		float _stackAngle = XM_PI / _stackCount;
		float _sliceAngle = XM_2PI / _sliceCount;

		float _deltaU = 1.f / static_cast<float>(_sliceCount);
		float _deltaV = 1.f / static_cast<float>(_stackCount);

		Vector3 _boundingBoxMin;
		Vector3	_boundingBoxMax;

		// 고리마다 돌면서 정점을 계산한다 (북극/남극 단일점은 고리가 X)
		for (uint32 y = 1; y <= _stackCount - 1; ++y)
		{
			float _phi = y * _stackAngle;

			// 고리에 위치한 정점
			for (uint32 x = 0; x <= _sliceCount; ++x)
			{
				float theta = x * _sliceAngle;

				_vertex._position.x = _radius * sinf(_phi) * cosf(theta);
				_vertex._position.y = _radius * cosf(_phi);
				_vertex._position.z = _radius * sinf(_phi) * sinf(theta);

				_boundingBoxMin.x = std::min(_vertex._position.x, _boundingBoxMin.x);
				_boundingBoxMin.y = std::min(_vertex._position.y, _boundingBoxMin.y);
				_boundingBoxMin.z = std::min(_vertex._position.z, _boundingBoxMin.z);

				_boundingBoxMax.x = std::max(_vertex._position.x, _boundingBoxMax.x);
				_boundingBoxMax.y = std::max(_vertex._position.y, _boundingBoxMax.y);
				_boundingBoxMax.z = std::max(_vertex._position.z, _boundingBoxMax.z);

				_vertex._uv = Vector2(_deltaU * x, _deltaV * y);

				_vertex._normal = _vertex._position;
				_vertex._normal.Normalize();

				_vertex._tangent.x = -_radius * sinf(_phi) * sinf(theta);
				_vertex._tangent.y = 0.0f;
				_vertex._tangent.z = _radius * sinf(_phi) * cosf(theta);
				_vertex._tangent.Normalize();

				_vertices.push_back(_vertex);
			}
		}

		// 남극
		_vertex._position = Vector3(0.0f, -_radius, 0.0f);
		_vertex._uv = Vector2(0.5f, 1.0f);
		_vertex._normal = _vertex._position;
		_vertex._normal.Normalize();
		_vertex._tangent = Vector3(1.0f, 0.0f, 0.0f);
		_vertices.push_back(_vertex);

		std::vector<std::vector<uint32>> _indices(1);

		// 북극 인덱스
		for (uint32 i = 0; i <= _sliceCount; ++i)
		{
			//  [0]
			//   |  \
			//  [i+1]-[i+2]
			_indices[0].push_back(0);
			_indices[0].push_back(i + 1);
			_indices[0].push_back(i + 2);
		}

		// 몸통 인덱스
		uint32 ringVertexCount = _sliceCount + 1;
		for (uint32 y = 0; y < _stackCount - 2; ++y)
		{
			for (uint32 x = 0; x < _sliceCount; ++x)
			{
				//  [y, x]-[y, x+1]
				//  |		/
				//  [y+1, x]
				_indices[0].push_back(1 + (y)*ringVertexCount + (x));
				_indices[0].push_back(1 + (y + 1) * ringVertexCount + (x));
				_indices[0].push_back(1 + (y)*ringVertexCount + (x + 1));
				//		 [y, x+1]
				//		 /	  |
				//  [y+1, x]-[y+1, x+1]
				_indices[0].push_back(1 + (y + 1) * ringVertexCount + (x));
				_indices[0].push_back(1 + (y + 1) * ringVertexCount + (x + 1));
				_indices[0].push_back(1 + (y)*ringVertexCount + (x + 1));
			}
		}

		// 남극 인덱스
		uint32 _bottomIndex = static_cast<uint32>(_vertices.size()) - 1;
		uint32 _lastRingStartIndex = _bottomIndex - ringVertexCount;
		for (uint32 i = 0; i < _sliceCount; ++i)
		{
			//  [last+i]-[last+i+1]
			//  |      /
			//  [bottom]
			_indices[0].push_back(_bottomIndex);
			_indices[0].push_back(_lastRingStartIndex + i + 1);
			_indices[0].push_back(_lastRingStartIndex + i);
		}


		auto _mesh = Resources::GetInstance()->CreateResource<Mesh>(TEXT("00000000-0000-0000-0000-000000000003"));

		_mesh->SetVertices(_vertices);
		_mesh->Setindices(_indices);

		tstring _path = g_assetPath + TEXT("bin\\mesh\\Sphere.mesh");

		_mesh->SetName(TEXT("Sphere"));
		_mesh->SetPath(_path);

		_mesh->SetBoundingBoxMin(_boundingBoxMin);
		_mesh->SetBoundingBoxMax(_boundingBoxMax);

		utility::Serializer::CreateMetaInfo(_mesh->GetPath(), _mesh.get());

		utility::Serializer::Serialize(_mesh->GetPath(), _mesh.get());
	}

	void DefaultQuardBuild()
	{
		vector<VertexAttribute> _vertices(4);

		_vertices[0] = { { -0.5, -0.5, +0.0 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 1.0f } , { +0.0f, +0.0f, -1.0f } , { +1.0f, +0.0f, +0.0f } /*, { } , { }*/ };
		_vertices[1] = { { -0.5, +0.5, +0.0 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 0.0f } , { +0.0f, +0.0f, -1.0f } , { +1.0f, +0.0f, +0.0f } /*, { } , { }*/ };
		_vertices[2] = { { +0.5, +0.5, +0.0 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 1.0f, 0.0f } , { +0.0f, +0.0f, -1.0f } , { +1.0f, +0.0f, +0.0f } /*, { } , { }*/ };
		_vertices[3] = { { +0.5, -0.5, +0.0 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 1.0f, 1.0f } , { +0.0f, +0.0f, -1.0f } , { +1.0f, +0.0f, +0.0f } /*, { } , { }*/ };

		vector<vector<uint32>> _indices(1, vector<uint32>(6));

		_indices[0][0] = 0, _indices[0][1] = 2, _indices[0][2] = 1;
		_indices[0][3] = 0, _indices[0][4] = 3, _indices[0][5] = 2;


		auto _mesh = Resources::GetInstance()->CreateResource<Mesh>(TEXT("00000000-0000-0000-0000-000000000002"));

		_mesh->SetVertices(_vertices);
		_mesh->Setindices(_indices);

		tstring _path = g_assetPath + TEXT("bin\\mesh\\Quard.mesh");

		_mesh->SetName(TEXT("Quard"));
		_mesh->SetPath(_path);

		_mesh->SetBoundingBoxMin(Vector3(-0.5, -0.5, +0.0));
		_mesh->SetBoundingBoxMax(Vector3(+0.5, +0.5, +0.0));

		utility::Serializer::CreateMetaInfo(_mesh->GetPath(), _mesh.get());

		utility::Serializer::Serialize(_mesh->GetPath(), _mesh.get());
	}

	void DefaultCubeBuild()
	{
		float w2 = 0.5f;
		float h2 = 0.5f;
		float d2 = 0.5f;

		vector<VertexAttribute> _vertices(24);

		// front
		_vertices[0]  = { { -w2, -h2, -d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 1.0f } , { +0.0f, +0.0f, -1.0f } , { +1.0f, +0.0f, +0.0f } /*, { } , { }*/ };
		_vertices[1]  = { { -w2, +h2, -d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 0.0f } , { +0.0f, +0.0f, -1.0f } , { +1.0f, +0.0f, +0.0f } /*, { } , { }*/ };
		_vertices[2]  = { { +w2, +h2, -d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 1.0f, 0.0f } , { +0.0f, +0.0f, -1.0f } , { +1.0f, +0.0f, +0.0f } /*, { } , { }*/ };
		_vertices[3]  = { { +w2, -h2, -d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 1.0f, 1.0f } , { +0.0f, +0.0f, -1.0f } , { +1.0f, +0.0f, +0.0f } /*, { } , { }*/ };
		
		// back
		_vertices[4]  = { { -w2, -h2, +d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 1.0f, 1.0f } , { +0.0f, +0.0f, +1.0f } , { -1.0f, +0.0f, +0.0f } /*, { } , { }*/ };
		_vertices[5]  = { { +w2, -h2, +d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 1.0f } , { +0.0f, +0.0f, +1.0f } , { -1.0f, +0.0f, +0.0f } /*, { } , { }*/ };
		_vertices[6]  = { { +w2, +h2, +d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 0.0f } , { +0.0f, +0.0f, +1.0f } , { -1.0f, +0.0f, +0.0f } /*, { } , { }*/ };
		_vertices[7]  = { { -w2, +h2, +d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 1.0f, 0.0f } , { +0.0f, +0.0f, +1.0f } , { -1.0f, +0.0f, +0.0f } /*, { } , { }*/ };
		
		// top
		_vertices[8]  = { { -w2, +h2, -d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 1.0f } , { +0.0f, +1.0f, +0.0f } , { +1.0f, +0.0f, +0.0f } /*, { } , { }*/ };
		_vertices[9]  = { { -w2, +h2, +d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 0.0f } , { +0.0f, +1.0f, +0.0f } , { +1.0f, +0.0f, +0.0f } /*, { } , { }*/ };
		_vertices[10] = { { +w2, +h2, +d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 1.0f, 0.0f } , { +0.0f, +1.0f, +0.0f } , { +1.0f, +0.0f, +0.0f } /*, { } , { }*/ };
		_vertices[11] = { { +w2, +h2, -d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 1.0f, 1.0f } , { +0.0f, +1.0f, +0.0f } , { +1.0f, +0.0f, +0.0f } /*, { } , { }*/ };
		
		// bottom
		_vertices[12] = { { -w2, -h2, -d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 1.0f, 1.0f } , { +0.0f, -1.0f, +0.0f } , { -1.0f, +0.0f, +0.0f } /*, { } , { }*/ };
		_vertices[13] = { { +w2, -h2, -d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 1.0f } , { +0.0f, -1.0f, +0.0f } , { -1.0f, +0.0f, +0.0f } /*, { } , { }*/ };
		_vertices[14] = { { +w2, -h2, +d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 0.0f } , { +0.0f, -1.0f, +0.0f } , { -1.0f, +0.0f, +0.0f } /*, { } , { }*/ };
		_vertices[15] = { { -w2, -h2, +d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 1.0f, 0.0f } , { +0.0f, -1.0f, +0.0f } , { -1.0f, +0.0f, +0.0f } /*, { } , { }*/ };
		
		//left
		_vertices[16] = { { -w2, -h2, +d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 1.0f } , { -1.0f, +0.0f, +0.0f } , { +0.0f, +0.0f, -1.0f } /*, { } , { }*/ };
		_vertices[17] = { { -w2, +h2, +d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 0.0f } , { -1.0f, +0.0f, +0.0f } , { +0.0f, +0.0f, -1.0f } /*, { } , { }*/ };
		_vertices[18] = { { -w2, +h2, -d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 1.0f, 0.0f } , { -1.0f, +0.0f, +0.0f } , { +0.0f, +0.0f, -1.0f } /*, { } , { }*/ };
		_vertices[19] = { { -w2, -h2, -d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 1.0f, 1.0f } , { -1.0f, +0.0f, +0.0f } , { +0.0f, +0.0f, -1.0f } /*, { } , { }*/ };
		
		//right
		_vertices[20] = { { +w2, -h2, -d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 1.0f } , { +1.0f, +0.0f, +0.0f } , { +0.0f, +0.0f, +1.0f } /*, { } , { }*/ };
		_vertices[21] = { { +w2, +h2, -d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 0.0f } , { +1.0f, +0.0f, +0.0f } , { +0.0f, +0.0f, +1.0f } /*, { } , { }*/ };
		_vertices[22] = { { +w2, +h2, +d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 1.0f, 0.0f } , { +1.0f, +0.0f, +0.0f } , { +0.0f, +0.0f, +1.0f } /*, { } , { }*/ };
		_vertices[23] = { { +w2, -h2, +d2 }, { 1.0f, 1.0f, 1.0f, 1.0f } , { 1.0f, 1.0f } , { +1.0f, +0.0f, +0.0f } , { +0.0f, +0.0f, +1.0f } /*, { } , { }*/ };

		vector<vector<uint32>> _indices(1, vector<uint32>(36));

		// 앞면
		_indices[0][0] = 0; _indices[0][1] = 2; _indices[0][2] = 1;
		_indices[0][3] = 0; _indices[0][4] = 3; _indices[0][5] = 2;
		// 뒷면
		_indices[0][6] = 4; _indices[0][7] = 6; _indices[0][8] = 5;
		_indices[0][9] = 4; _indices[0][10] = 7; _indices[0][11] = 6;
		// 윗면
		_indices[0][12] = 8; _indices[0][13] = 10; _indices[0][14] = 9;
		_indices[0][15] = 8; _indices[0][16] = 11; _indices[0][17] = 10;
		// 아랫면
		_indices[0][18] = 12; _indices[0][19] = 14; _indices[0][20] = 13;
		_indices[0][21] = 12; _indices[0][22] = 15; _indices[0][23] = 14;
		// 왼쪽면
		_indices[0][24] = 16; _indices[0][25] = 18; _indices[0][26] = 17;
		_indices[0][27] = 16; _indices[0][28] = 19; _indices[0][29] = 18;
		// 오른쪽면
		_indices[0][30] = 20; _indices[0][31] = 22; _indices[0][32] = 21;
		_indices[0][33] = 20; _indices[0][34] = 23; _indices[0][35] = 22;

		auto _mesh = Resources::GetInstance()->CreateResource<Mesh>(TEXT("00000000-0000-0000-0000-000000000001"));

		_mesh->SetVertices(_vertices);
		_mesh->Setindices(_indices);

		tstring _path = g_assetPath + TEXT("bin\\mesh\\Cube.mesh");

		_mesh->SetName(TEXT("Cube"));
		_mesh->SetPath(_path);

		_mesh->SetBoundingBoxMin(Vector3(-0.5, -0.5, -0.5));
		_mesh->SetBoundingBoxMax(Vector3(+0.5, +0.5, +0.5));

		utility::Serializer::CreateMetaInfo(_mesh->GetPath(), _mesh.get());

		utility::Serializer::Serialize(_mesh->GetPath(), _mesh.get());
	}

	void DefaultResourceBuilder::DefaultMeshBuild()
	{
		fs::path _path(g_assetPath + TEXT("bin\\mesh"));

		assert(fs::is_directory(_path));
		
		bool _cube = false, _sphere = false, _quard = false;

		for (auto& _p : fs::directory_iterator(_path, fs::directory_options::skip_permission_denied))
		{
			if (!_cube   && _p.path().stem().wstring() == TEXT("Cube")) _cube = true;
			if (!_sphere && _p.path().stem().wstring() == TEXT("Sphere")) _sphere = true;
			if (!_quard && _p.path().stem().wstring() == TEXT("Quard"))	_quard = true;
		}

		if (!_cube) DefaultCubeBuild();
		if (!_quard) DefaultQuardBuild();
		if (!_sphere) DefaultSphereBuild();
	}

	void DefaultResourceBuilder::DefaultMaterialBuild()
	{
		auto _defaultMaterial = Resources::GetInstance()->CreateResource<Material>(TEXT("00000000-0000-0000-0000-000000000004"));

		_defaultMaterial->SetRenderPassID(TEXT("Deferred Pass"));

		_defaultMaterial->SetPipelineID(TEXT("Standard"));

		auto _defaultSkyBoxProceduralMaterial = Resources::GetInstance()->CreateResource<Material>(TEXT("00000000-0000-0000-0000-000000000005"));

		_defaultSkyBoxProceduralMaterial->SetName(TEXT("Default"));

		_defaultSkyBoxProceduralMaterial->SetRenderPassID(TEXT("SkyBox Pass"));

		_defaultSkyBoxProceduralMaterial->SetPipelineID(TEXT("Skybox-Procedural"));
	}
}