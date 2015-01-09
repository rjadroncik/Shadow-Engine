#pragma once

#include "Mesh.h"

namespace Resources
{
	class CLoaderMD5Mesh
	{
	public:
		static bool Load(_IN CString& rFile, _OUT Resources::CMesh& rOutMesh);

	private:
 		struct SVertex 
		{
			//Weights[0] - start weight index, Weights[1] - weight count
			Vector2i Weights;
			Vector2f TexCoords;
		};

		struct SWeight
		{
			UINT  uiJoint;
			float fBias;

			Vector3f Position;
		};

		struct SJoint
		{
			CString csName;
			UINT    uiParent;

			//Orientation is a Quaternion
			Vector3f Position;
			Vector4f Orientation;
		}

		struct SMesh
		{
			CString csTexture;

			SVertex* pVertices;
			UINT     uiVertexCount;
			
			Vector3ui* pTriangles;
			UINT       uiTriangleCount;

			SWeight* pWeights;
			UINT     uiWeightCount;
		};
	};
};
