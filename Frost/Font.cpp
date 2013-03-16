#define FROST_EXPORT
#include "Font.h"

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

#include <gl/GL.h>

#include <math.h>

namespace Frost
{
	List<Font *> Font::LoadedFonts;

	Font::Font(void)
	{
	}

	Font::~Font(void)
	{
	}

	void Font::Draw(float X, float Y, float Z, String Text)
	{
		glColor4ub(0, 0, 0, 255);
		
		float penX = X;
		float penY = Y;
		
		for (unsigned int i = 0; i < Text.length(); i++)
		{
			if (Text.charAt(i) == '\n')
			{
				penY += Height;
				penX = X;
				continue;
			}

			Graphics3D::Texture *letTex = Letters[Text.charAt(i)];
			GlyphMetrics *letMetrics = Metrics[Text.charAt(i)];

			float penDrawX = penX;// + letMetrics->horiBearingX;
			float penDrawY = penY;// + (letMetrics->height - letMetrics->horiBearingY);

			letTex->Bind();
			
			glBegin(GL_QUADS);

			glTexCoord2f(0, 0);
			glVertex2f(penDrawX, penDrawY);

			glTexCoord2f(1, 0);
			glVertex2f(penDrawX + letMetrics->width, penDrawY);

			glTexCoord2f(1, 1);
			glVertex2f(penDrawX + letMetrics->width, penDrawY + letMetrics->height);

			glTexCoord2f(0, 1);
			glVertex2f(penDrawX, penDrawY + letMetrics->height);

			glEnd();

			penX += letMetrics->horiAdvance;
		}
		
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Font::Draw(float x, float y, String Text)
	{
		Draw(x, y, 0, Text);
	}

	Font *Font::getFont(String path, int height)
	{
		// Check if the font was already loaded and return it's existing handle if so.
		for (unsigned int i = 0; i < LoadedFonts.count(); i++)
		{
			if (LoadedFonts[i]->Path == path)
			{
				return LoadedFonts[i];
			}
		}

		// Initialize the font and add it to the loaded fonts list.
		Font* ret = new Font();
		ret->Path = path;
		ret->Height = height;
		LoadedFonts.Add(ret);

		// Do all the freetype initialization stuff and load the images into the texture array.
		FT_Library lib;
		FT_Face face;
		FT_Glyph glyph;
		FT_Bitmap* bitmap;
		FT_BitmapGlyph* bitglyph;
		int w, h, xpos, ypos;
		unsigned char i;
		unsigned char* buffer;

		FT_Init_FreeType(&lib);
		FT_New_Face(lib, path, 0, &face);
		FT_Set_Char_Size(face, height << 6, height << 6, 96, 96);

		for (i=0;i<128;i++)
		{
			// Acquire the letter glyph.
			FT_Load_Glyph(face, FT_Get_Char_Index(face, i), FT_LOAD_DEFAULT);
			FT_Get_Glyph(face->glyph, &glyph);

			// Copy over glyph metrics for font rendering.
			// The metrics found in face->glyph->metrics are normally expressed in 26.6 pixel format (i.e., 1/64th of pixels)
			ret->Metrics[i] = new GlyphMetrics();

			ret->Metrics[i]->width = face->glyph->metrics.width / 64;
			ret->Metrics[i]->height = face->glyph->metrics.height / 64;

			ret->Metrics[i]->horiBearingX = face->glyph->metrics.horiBearingX / 64;
			ret->Metrics[i]->horiBearingY = face->glyph->metrics.horiBearingY / 64;
			ret->Metrics[i]->horiAdvance = face->glyph->metrics.horiAdvance / 64;

			ret->Metrics[i]->vertBearingX = face->glyph->metrics.vertBearingX / 64;
			ret->Metrics[i]->vertBearingY = face->glyph->metrics.vertBearingY / 64;
			ret->Metrics[i]->vertAdvance = face->glyph->metrics.vertAdvance / 64;

			// Convert the glyph to a bitmap and dispose of the original glyph object.
			FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
			bitglyph = (FT_BitmapGlyph*)&glyph;
			bitmap = &((FT_BitmapGlyph)glyph)->bitmap;
			w= 1;
			while(w<bitmap->width)
			w *= 2;
			h= 1;
			while(h<bitmap->rows)
			h *= 2;

			// Make a bitmap based on glyph information.
			buffer = (unsigned char*)calloc(sizeof(unsigned char)*2*w*h, 1);
			for (ypos=0;ypos<bitmap->rows;ypos++)
			{
				for (xpos=0;xpos<bitmap->width;xpos++)
				{
					buffer[2*(xpos+ypos*w)] = bitmap->buffer[xpos+ypos*bitmap->width];
					buffer[2*(xpos+ypos*w)+1] = bitmap->buffer[xpos+ypos*bitmap->width];
				}
			}

			// Initialize this texture and load the bitmap into it.
			ret->Letters[i] = new Graphics3D::Texture();
			ret->Letters[i]->Load(buffer, w, h, 1);
		}

		FT_Done_Face(face);

		FT_Done_FreeType(lib);

		return ret;
	}

	void Font::freeFont(Font *fnt)
	{
		for (int i = 0; i < 128; i++)
		{
			fnt->Letters[i]->Release();
		}
	}
};