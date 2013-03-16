#define FROST_EXPORT
#include "Texture.h"

#include <Windows.h>
#include <GL/gl.h>

#include "String.h"
#include "Log.h"

#include "stb_image.h"

namespace Frost
{
	namespace Graphics3D
	{
		unsigned int Texture::LastBoundTextureID = 0;

		Texture::Texture()
		{
			ImgFile = 0;
		}

		Texture::Texture(String ImagePath)
		{
			Load(ImagePath);
		}

		Texture::~Texture()
		{
			Release();
			if (ImgFile)
			{
				delete ImgFile;
			}
		}

		void Texture::Load(unsigned char *data, int width, int height, int bitsperpixel)
		{
			glGenTextures( 1, &ID );

			Bind();

			if (glIsTexture(ID) == GL_FALSE)
			{
				Log::Msg("Failed to initialize an OpenGL texture! OpenGL did not return a valid Texture ID.", Log::FatalError);
			}

			RawImageData = data;
			Width = width;
			Height = height;
			BPP = bitsperpixel;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			// If you do not want lighting and coloring to effect your texture and you would
			// like to display the texture unchanged when coloring is applied replace GL_MODULATE with GL_DECAL.
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

			short dataFmt = 0;

			if (BPP == 4)
			{
				dataFmt = GL_RGBA;
			}
			else if (BPP == 3)
			{
				dataFmt = GL_RGB;
			}
			else if (BPP == 1)
			{
				dataFmt = GL_LUMINANCE_ALPHA;
			}
			else
			{
				Log::Msg(String("Attempted to create a texture with an unrecognized bits per pixel (") + BPP + ") image format.", Log::FatalError);
			}

			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, dataFmt, GL_UNSIGNED_BYTE, data );

			Unbind();
		}

		void Texture::Load()
		{
			int x, y, n;
			unsigned char *data = stbi_load(ImgFile->Path.Text, &x, &y, &n, 0);

			if (data == NULL)
			{
				Log::Msg("Failed to open a texture image. This could be due to the path being invalid or the file already being in use.", Log::FatalError);
			}

			Load(data, x, y, n);
		}

		void Texture::Load(String ImagePath)
		{
			ImgFile = new File();
			ImgFile->Path = ImagePath;
			Load();
		}

		void Texture::Bind()
		{
			if (LastBoundTextureID != ID)
			{
				glBindTexture( GL_TEXTURE_2D, ID );
				LastBoundTextureID = ID;
			}
		}

		void Texture::Unbind()
		{
			if (LastBoundTextureID != 0)
			{
				glBindTexture( GL_TEXTURE_2D, 0 );
				LastBoundTextureID = 0;
			}
		}

		void Texture::FreeGLTexture()
		{
			glDeleteTextures( 1, &ID );
			ID = -1;
		}

		void Texture::FreeRawImage()
		{
			stbi_image_free(RawImageData);
			RawImageData = 0;
		}

		void Texture::Release()
		{
			FreeGLTexture();
			FreeRawImage();
		}
	}
}