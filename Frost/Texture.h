#pragma once

#include "DLL.h"
#include "String.h"
#include "File.h"

namespace Frost
{
	namespace Graphics3D
	{
		class FROSTAPI Texture
		{
		public:
			unsigned long Width;
			unsigned long Height;
			unsigned int ID;
			unsigned int BPP;
			unsigned char *RawImageData;
			File *ImgFile;

			Texture();
			Texture(String ImagePath);
			~Texture();
			void Load(unsigned char *data, int width, int height, int bitsperpixel);
			void Load();
			void Load(String ImagePath);
			void Bind();
			void Unbind();
			void FreeGLTexture();
			void FreeRawImage();
			void Release();

		private:
			static unsigned int LastBoundTextureID;
		};
	};
}