#ifndef _pgl_mesh
#define _pgl_mesh 

#include<GL/glew.h>
#include<GL/freeglut.h>
#include<glm/glm.hpp>
#include<glm/ext.hpp>
#include<iostream>
#include<string>
#include<map>
#include<Importer.hpp>
#include<scene.h>
#include<postprocess.h>
#include<vector>

#include "TeslaCompute.h"

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

using  std::cin;
using  std::cout;
using  std::endl;

class Mesh
{
public:

	Mesh();
	
	~Mesh();

	bool LoadMesh(const std::string& fileName);

	void Render();
private:

	bool InitFromScene(const aiScene* pScene, const std::string& fileName);
	void InitMesh(unsigned int Index, const aiMesh* pMesh);
	bool InitMaterials(const aiScene* pScene, const std::string& fileName);
	void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

	struct Vertex
	{
		glm::vec3 m_pos;
		glm::vec2 m_tex;
		glm::vec3 m_normal;
		glm::vec3 m_tangent;

		Vertex() {}

		Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal, const glm::vec3& Tangent)
		{
			m_pos    = pos;
			m_tex    = tex;
			m_normal = normal;
			m_tangent = Tangent;
		}
	};


	struct MeshEntry
	{
		MeshEntry();
		~MeshEntry();

		bool Init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices);

		GLuint VB;
		GLuint IB;
		unsigned int NumIndices;
		unsigned int MaterialIndex;
	};

	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_textures;
};

#endif