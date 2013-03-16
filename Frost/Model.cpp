#define FROST_EXPORT
#include "Model.h"

#include "File.h"
#include "StringList.h"

namespace Frost
{
	namespace Graphics3D
	{
		Model::Model(void)
		{
		}

		Model::~Model(void)
		{
		}

		Vertex *Model::getVertex(unsigned int id)
		{
			for (unsigned int mi = 0; mi < meshList.count(); mi++)
			{
				Mesh *curMesh = meshList[mi];
				for (unsigned int i = 0; i < curMesh->Vertices.count(); i++)
				{
					if (curMesh->Vertices[i]->ID == id)
					{
						return curMesh->Vertices[i];
					}
				}
			}
			return (Vertex *)0;
		}

		TexCoord *Model::getTexCoord(unsigned int id)
		{
			for (unsigned int mi = 0; mi < meshList.count(); mi++)
			{
				Mesh *curMesh = meshList[mi];
				for (unsigned int i = 0; i < curMesh->TexCoords.count(); i++)
				{
					if (curMesh->TexCoords[i]->ID == id)
					{
						return curMesh->TexCoords[i];
					}
				}
			}
			return (TexCoord *)0;
		}

		Material *Model::getMaterial(String name)
		{
			for (unsigned int i = 0; i < matLib.count(); i++)
			{
				if (matLib[i]->getName().equals(name))
				{
					return matLib[i];
				}
			}
			return (Material *)0;
		}

		void Model::Draw(MovableObject3D *pos)
		{
			for (unsigned int i = 0; i < meshList.count(); i++)
			{
				meshList[i]->Draw(pos);
			}
		}

		void Model::LoadXML(String path)
		{
			Log::Msg("Unsupported.", Log::FatalError);
			/*
			Mesh *m = new Mesh();
			XmlDocument doc;
			doc.Load(path);

			// NOTE: All vertices used as part of a face must be specified before the face that uses them.
			for (unsigned int i = 0; i < doc.DocumentElement->SubElements.count(); i++)
			{
				XmlElement *elem = doc.DocumentElement->SubElements[i];

				if (elem->Name == "v")
				{
					Vertex *v = new Vertex();

					// All vertices should have at least these core attributes.
					v->ID = elem->getAttribute("id")->Value.toInt();
					v->Pos.x = (float)elem->getAttribute("x")->Value.toInt();
					v->Pos.y = (float)elem->getAttribute("y")->Value.toInt();
					v->Pos.z = (float)elem->getAttribute("z")->Value.toInt();

					// Check for a color specification.
					XmlAttribute *clrElem = elem->getAttribute("clr");
					
					if (clrElem != 0)
					{
						//v->UseColor = true;
						//v->Clr = Color::FromIntRGBA(clrElem->Value.hexToInt());
					}
					
					// Check for a texture coord specification.
					XmlAttribute *texX = elem->getAttribute("tx");
					XmlAttribute *texY = elem->getAttribute("ty");

					// If one texture coord is specified, both have to be.
					if (texX)
					{
						//v->UseTexture = true;
						//v->TexturePos.x = (float)texX->Value.toInt();
						//v->TexturePos.y = (float)texY->Value.toInt();
					}

					// Push the constructed vertex into the vertex vector.
					//m->Vertices.Add(v);
				}
				else if (elem->Name == "f")
				{
					Face *f = new Face();

					f->ID = elem->getAttribute("id")->Value.toInt();

					XmlAttribute *texIdAttrib = elem->getAttribute("tid");

					if (texIdAttrib)
					{
						// TODO
						// Load the texture, but there needs to be some resource pool shit or something...
						// Implement static global texture acquision functions within the Texture class
						// and remember scope + identifier information. Possibly use an enum for resource pool types.
						// TODO
						unsigned int tid = texIdAttrib->Value.toInt();
						// Check if the texture resource exists within valid resource pool scopes,
						// then f->UseTexture = true;
					}

					// Initialize some necessary variables for iteration.
					unsigned int curVertexNum = 1;
					String curVertexStr = "v1";
					XmlAttribute *curVertexAttrib = elem->getAttribute(curVertexStr);

					// Make sure this is not a broken face entry.
					if (!curVertexAttrib)
					{
						Log::Msg("An attempt to load a face with no associated vertices was made.", Log::FatalError);
					}

					// Load all the vertices involved in the constructing of this face.
					while (curVertexAttrib != 0)
					{
						// Make sure that an attempt to use a vertex that hasn't been loaded (or doesn't exist) doesn't happen.
						// Storing references to vertices directly is a lot faster than storing id integers and iterating through the vectors
						// every time you want a certain vertex, so this has to be done this way.
						Vertex *acquiredVertex = 0;//m->getVertex(curVertexAttrib->Value.toInt());

						if (!acquiredVertex)
						{
							Log::Msg("A face attempted to use a vertex that was not initialized or doesn't exist.", Log::FatalError);
						}

						f->Vertices.Add(acquiredVertex);
						curVertexNum++;
						curVertexStr = String("v") + curVertexNum;
						curVertexAttrib = elem->getAttribute(curVertexStr);
					}

					// Push the constructed face into the face vector.
					m->Faces.Add(f);
				}
			}

			Log::Msg(String("Completed loading mesh \"") + path + "\".", Log::Info);
			*/
		}

		void Model::LoadOBJ(String path)
		{
			File f = File(path);
			f.Open();

			unsigned int CurrentVertexID = 1;
			unsigned int CurrentTexCoordID = 1;
			unsigned int CurrentFaceID = 1;

			Mesh *curMesh = 0;

			while (!f.eof())
			{
				String line = f.readLine();

				// Do not attempt to parse the line if it's empty.
				if (line.length() == 0)
				{
					continue;
				}

				StringList splitLine = line.split(" ");

				if (splitLine[0][0] == '#')
				{
					// This line is a comment so do nothing.
				}
				else if (splitLine[0] == "mtllib")
				{
					// implement a material library file
					// NOTE THIS NEEDS TO BE ADDED TO A RESOURCE POOL INSTEAD OF AUTO-APPLIED.
					LoadMTL(String("res/") + splitLine[1]);
				}
				else if (splitLine[0] == "o")
				{
					// implement an object (model)
					// Initialize new mesh instance.
					curMesh = new Mesh();
					curMesh->setName(splitLine[1]);
					meshList.Add(curMesh);

					// Reset face ID's for new mesh.
					CurrentFaceID = 1;
				}
				else if (splitLine[0] == "v")
				{
					// implement a vertex addition
					Vertex *v = new Vertex();
					v->ID = CurrentVertexID;
					v->Pos.x = splitLine[1].toFloat();
					v->Pos.y = splitLine[2].toFloat();
					v->Pos.z = splitLine[3].toFloat();
					curMesh->Vertices.Add(v);
					CurrentVertexID++;
				}
				else if (splitLine[0] == "vt")
				{
					// implement texture coord addition
					TexCoord *vt = new TexCoord();
					vt->ID = CurrentTexCoordID;
					vt->u = splitLine[1].toFloat();
					vt->v = splitLine[2].toFloat();
					curMesh->TexCoords.Add(vt);
					CurrentTexCoordID++;
				}
				else if (splitLine[0] == "usemtl")
				{
					// implement use material
					curMesh->usedMaterial = getMaterial(splitLine[1]);
				}
				else if (splitLine[0] == "s")
				{
					// implement smooth shading
				}
				else if (splitLine[0] == "f")
				{
					// implement faces
					Face *f = new Face();
					f->ID = CurrentFaceID;

					for (unsigned int i = 1; i < splitLine.count(); i++)
					{
						StringList faceDatas = splitLine[i].split("/");

						f->Vertices.Add(getVertex(faceDatas[0].toInt()));

						if (faceDatas.count() == 2)
						{
							// Add texture coordinate.
							f->TexCoords.Add(getTexCoord(faceDatas[1].toInt()));
						}
						else if (faceDatas.count() == 3)
						{
							//Log::Msg("Face data with a normal data member was specified. (not currently supported)", Log::FatalError);
							if (!faceDatas[1].empty())
							{
								// add tex coord
								f->TexCoords.Add(getTexCoord(faceDatas[1].toInt()));
							}

							// add normal info as faceDatas[2].toInt(); as id of normal coord
						}
					}

					curMesh->Faces.Add(f);
					CurrentFaceID++;
				}
			}

			Log::Msg(String("Completed loading OBJ file \"") + path + "\".", Log::Info);
		}

		void Model::LoadMTL(String path)
		{
			File f = File(path);
			f.Open();

			Material *curMat = 0;

			while (!f.eof())
			{
				String line = f.readLine();

				// Do not attempt to parse the line if it's empty.
				if (line.length() == 0)
				{
					continue;
				}

				StringList splitLine = line.split(" ");

				if (splitLine[0][0] == '#')
				{
					// This line is a comment so do nothing.
				}
				else if (splitLine[0] == "newmtl")
				{
					// Add a new material.
					curMat = new Material();
					curMat->setName(splitLine[1]);
					matLib.Add(curMat);
				}
				else if (splitLine[0] == "Ns")
				{
					curMat->SpecularExponent = splitLine[1].toFloat();
				}
				else if (splitLine[0] == "Ka")
				{
					curMat->Specular.R = (unsigned char)(splitLine[1].toFloat() * 255);
					curMat->Specular.G = (unsigned char)(splitLine[2].toFloat() * 255);
					curMat->Specular.B = (unsigned char)(splitLine[3].toFloat() * 255);
				}
				else if (splitLine[0] == "Kd")
				{
					curMat->Specular.R = (unsigned char)(splitLine[1].toFloat() * 255);
					curMat->Specular.G = (unsigned char)(splitLine[2].toFloat() * 255);
					curMat->Specular.B = (unsigned char)(splitLine[3].toFloat() * 255);
				}
				else if (splitLine[0] == "Ks")
				{
					curMat->Specular.R = (unsigned char)(splitLine[1].toFloat() * 255);
					curMat->Specular.G = (unsigned char)(splitLine[2].toFloat() * 255);
					curMat->Specular.B = (unsigned char)(splitLine[3].toFloat() * 255);
				}
				else if (splitLine[0] == "Ni")
				{
					curMat->OpticalDensity = splitLine[1].toFloat();
				}
				else if (splitLine[0] == "d")
				{
					curMat->Dissolve = splitLine[1].toFloat();
				}
				else if (splitLine[0] == "illum")
				{
					//
				}
				else if (splitLine[0] == "map_Kd")
				{
					// load material texture resource & add to pool or something
					curMat->DiffuseMap = new Texture();
					curMat->DiffuseMap->Load(String("res/") + splitLine[1]);
				}
			}
		}
	};
};