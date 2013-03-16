#pragma once

#include "DLL.h"

#include "List.h"
#include "String.h"
#include "Texture.h"

namespace Frost
{
	struct FROSTAPI GlyphMetrics
	{
		long width;
		long height;

		long horiBearingX;
		long horiBearingY;
		long horiAdvance;

		long vertBearingX;
		long vertBearingY;
		long vertAdvance;
	};

	class FROSTAPI Font
	{
	public:
		Font(void);
		~Font(void);

		String Path;
		int Height;
		Graphics3D::Texture *Letters[128];
		GlyphMetrics *Metrics[128];

		void Draw(float X, float Y, float Z, String Text);
		void Draw(float X, float Y, String Text);

		static Font *getFont(String path, int height);
		static void freeFont(Font *fnt);

	private:
		static List<Font *> LoadedFonts;
	};
};