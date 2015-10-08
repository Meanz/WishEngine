#pragma once

#include "stdafx.h"
#include "WishIO.hpp"
#include "WishMesh.hpp"
#include <sstream>
#include <vector>

namespace Wish
{
	inline std::string Wish_strtok(std::string& str, size_t* off) {

		//Find the first space
		size_t firstSpace = str.find_first_not_of(' ', *off);
		if (firstSpace == std::string::npos) {
			*off = std::string::npos;
			return "";
		}

		//Find the second space
		size_t secondSpace = str.find_first_of(' ', firstSpace);

		//If the newline comes before the second space
		size_t newLine = str.find_first_of('\n', firstSpace);
		if (newLine <= secondSpace && newLine != std::string::npos) {
			secondSpace = newLine;
		}

		if (secondSpace == std::string::npos) {
			*off = std::string::npos;
			return "";
		}

		//Move curr off to the end
		*off = secondSpace;

		//Substr it
		std::string thirdFloat = str.substr(firstSpace, (secondSpace - firstSpace));

		return thirdFloat;
	}

	inline int Wish_str_nextInt(std::string& str, size_t* off) {
		std::string intString = Wish_strtok(str, off);
		if (intString.size() == 0 || *off == std::string::npos) {
			return 0;
		}
		else {
			return stoi(intString);
		}
	}

	inline float Wish_str_nextFloat(std::string& str, size_t* off) {
		std::string floatString = Wish_strtok(str, off);
		if (floatString.size() == 0 || *off == std::string::npos) {
			return 0.0f;
		}
		else {
			return stof(floatString);
		}
	}

	inline std::vector<std::string> Wish_strsplit(const std::string &s, char delim) {
		std::stringstream ss(s);
		std::string item;
		std::vector<std::string> tokens;
		while (getline(ss, item, delim)) {
			tokens.push_back(item);
		}
		return tokens;
	}

	inline ivec3 Wish_strvec_toivec3(std::vector<std::string>& _strings) {
		assert(_strings.size() == 3);
		ivec3 vec;
		for (size_t i = 0; i < 3; i++) {
			if (_strings[i].size() > 0) {
				vec[i] = stoi(_strings[i]);
			}
		}
		return vec;
	}

	inline vec3 Wish_objloader_getvec3(std::vector<vec3>& vec, int idx) {
		if (idx < 0 || idx >= (int)vec.size())
			return vec3(0.0f);
		else
			return vec[idx];
	}

	inline vec2 Wish_objloader_getvec2(std::vector<vec2>& vec, int idx) {
		if (idx < 0 || idx >= (int)vec.size())
			return vec2(0.0f);
		else
			return vec[idx];
	}

	struct OBJFace
	{
		ivec3 p1;
		ivec3 p2;
		ivec3 p3;
	};

	static wish_mesh* WishOBJLoader_Load(const char* fileName) {
		using namespace glm;
		//DONT LIKE THE NEW CALL!!! :((((
		wish_mesh* mesh = new wish_mesh();
		try{
			std::string data = ReadFileToString(fileName);

			std::vector<vec3> vertices;
			std::vector<vec3> normals;
			std::vector<vec2> uvs;
			std::vector<OBJFace> faces;

			//Read line by line
			size_t nextNL = 0;
			while (nextNL != std::string::npos) {
				if ((nextNL + 1) >= data.size()) {
					printf("Thing broke it :'(\n");
					break;
				}

				//Read
				size_t firstNotSpace = data.find_first_not_of(' ', nextNL);

				if (firstNotSpace == std::string::npos) {
					//Wuuh
					printf("That's odd, nothing here?");
					break;
				}

				//printf("First not space %d :: %c\n", firstNotSpace, data[firstNotSpace + 1]);

				if (data[firstNotSpace + 1] == 'v' && data[firstNotSpace + 2] == ' ') {
					//We have our v, what's happening now!
					size_t currOff = firstNotSpace + 2;
					float f1 = Wish_str_nextFloat(data, &currOff);
					float f2 = Wish_str_nextFloat(data, &currOff);
					float f3 = Wish_str_nextFloat(data, &currOff);

					vertices.push_back(vec3(f1, f2, f3));
				}
				else if (data[firstNotSpace + 1] == 'v' && data[firstNotSpace + 2] == 'n') {
					size_t currOff = firstNotSpace + 3;
					float f1 = Wish_str_nextFloat(data, &currOff);
					float f2 = Wish_str_nextFloat(data, &currOff);
					float f3 = Wish_str_nextFloat(data, &currOff);
					normals.push_back(vec3(f1, f2, f3));
				}
				else if (data[firstNotSpace + 1] == 'v' && data[firstNotSpace + 2] == 't') {
					size_t currOff = firstNotSpace + 3;
					float f1 = Wish_str_nextFloat(data, &currOff);
					float f2 = Wish_str_nextFloat(data, &currOff);
					uvs.push_back(vec2(f1, f2));
				}
				else if (data[firstNotSpace + 1] == 'f')
				{
					size_t currOff = firstNotSpace + 2;

					std::string p1 = Wish_strtok(data, &currOff);
					std::string p2 = Wish_strtok(data, &currOff);
					std::string p3 = Wish_strtok(data, &currOff);

					//They are now in the format x/y/z but casn be x//z or any variants thereof

					//printf("Face: |%s|%s|%s|\n", p1.c_str(), p2.c_str(), p3.c_str());

					std::vector<std::string> p1splits = Wish_strsplit(p1, '/');
					std::vector<std::string> p2splits = Wish_strsplit(p2, '/');
					std::vector<std::string> p3splits = Wish_strsplit(p3, '/');

					//Gen first ivec3
					OBJFace face
					{
						Wish_strvec_toivec3(p1splits),
						Wish_strvec_toivec3(p2splits),
						Wish_strvec_toivec3(p3splits)
					};

					//printf("Face: %d, %d, %d - %d, %d, %d - %d, %d, %d\n", face.p1.x, face.p1.y, face.p1.z, face.p2.x, face.p2.y, face.p2.z, face.p3.x, face.p3.y, face.p3.z);

					faces.push_back(face);
				}

				nextNL = data.find('\n', nextNL + 1);
			}

			//We have the data, compile them into stuff we know about

			//This shit will be slow a'fk
			std::vector<Vertex_VNT> meshVertices;
			std::vector<GLuint> meshIndices;
			//Iterate through faces

			size_t vertOff = 0;
			for (auto& it : faces) {

				Vertex_VNT p1{
					Wish_objloader_getvec3(vertices, it.p1.x - 1),
					Wish_objloader_getvec3(normals, it.p1.y - 1),
					Wish_objloader_getvec2(uvs, it.p1.z - 1)
				};
				Vertex_VNT p2{
					Wish_objloader_getvec3(vertices, it.p2.x - 1),
					Wish_objloader_getvec3(normals, it.p2.y - 1),
					Wish_objloader_getvec2(uvs, it.p2.z - 1)
				};
				Vertex_VNT p3{
					Wish_objloader_getvec3(vertices, it.p3.x - 1),
					Wish_objloader_getvec3(normals, it.p3.y - 1),
					Wish_objloader_getvec2(uvs, it.p3.z - 1)
				};

				meshVertices.push_back(p1);
				meshVertices.push_back(p2);
				meshVertices.push_back(p3);

				meshIndices.push_back(vertOff++);
				meshIndices.push_back(vertOff++);
				meshIndices.push_back(vertOff++);
			}

			Wish_Mesh_SetVertices(mesh, (GLfloat*)&meshVertices[0], meshVertices.size(), sizeof(Vertex_VNT));
			Wish_Mesh_SetIndices(mesh, &meshIndices[0], meshIndices.size());
			Wish_Mesh_Compile(mesh);
		}
		catch (WishIOException&) {
			assert(false);
		}
		return mesh;
	}


}