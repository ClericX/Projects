#include "Direct3D.h"
#include "Main.h"

Direct3D::Direct3D(void)
{
	CamX = 0;
	CamY = 0;
}

Direct3D::~Direct3D(void)
{
}

void Direct3D::Initialize(bool Windowed)
{
	AspectRatio = (float)App.ClientWidth / (float)App.ClientHeight;
	FieldOfView = D3DX_PI / 4.0f;
	NearClippingPane = 0.0f;
	FarClippingPane = 1000.0f;

	ColourFormat = D3DFMT_A8R8G8B8; //Possible values are D3DFMT_R5G6B5, D3DFMT_X1R5G5B5, D3DFMT_A8R8G8B8 and D3DFMT_X8R8G8B8, D3DFMT_A32B32G32R32F. 
	DepthStencilFormat = D3DFMT_D16; //Possible values are D3DFMT_D16, D3DFMT_D15S1, D3DFMT_D24X8, D3DFMT_D24X4S4 and D3DFMT_D24S8.
	MultiSampling = D3DMULTISAMPLE_NONE; //Possible values are either D3DMULTISAMPLE_NONE or D3DMULTISAMPLE_n_SAMPLES, where n is the number of samples (1-16).

	Anisotropy = 0;

	D3DObject = Direct3DCreate9(D3D_SDK_VERSION);

	if (D3DObject == NULL)
	{
		MessageBox(App.hWnd, "The DirectX Runtime library is not installed.","Frost - Error", MB_OK);
		TerminateProcess(GetCurrentProcess(), EXIT_SUCCESS);
	}
 
	ZeroMemory(&PresentParameters,sizeof(PresentParameters));
	if(Windowed)
	{
		PresentParameters.Windowed = true;
	}
	else if (!Windowed)
	{
		PresentParameters.Windowed = false;
	}
	PresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	PresentParameters.BackBufferCount = 1;
	PresentParameters.EnableAutoDepthStencil = true;
	PresentParameters.AutoDepthStencilFormat = DepthStencilFormat;
	PresentParameters.hDeviceWindow = App.hWnd;
	PresentParameters.BackBufferWidth = App.ClientWidth;
	PresentParameters.BackBufferHeight = App.ClientHeight;
	PresentParameters.BackBufferFormat = ColourFormat;
	PresentParameters.MultiSampleType = MultiSampling;

	/*
	if (FAILED(D3DObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, App.hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &PresentParameters, &D3DDevice)))
	{
		if (FAILED(D3DObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, App.hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &PresentParameters, &D3DDevice)))
		{
			MessageBox(App.hWnd, "Epic failure has occured! Vertex processing type has not been assigned.", "Frost - Error", MB_OK);
			TerminateProcess(GetCurrentProcess(), EXIT_SUCCESS);
		}
	}
	*/

    //Get device capabilities
    ZeroMemory (&d3dCaps, sizeof(d3dCaps));
    if (FAILED(D3DObject->GetDeviceCaps (D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps)))
	{
		MessageBox(App.hWnd, "Unable to retrieve device capabilities!", "Frost - Fatal error", MB_OK);
	}

    //Check if hardware vertex processing is available
    if (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    {    
        //Create device with hardware vertex processing
        D3DObject->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, App.hWnd,
            D3DCREATE_HARDWARE_VERTEXPROCESSING, &PresentParameters, &D3DDevice);        
    }
    else
    {
        //Create device with software vertex processing
        D3DObject->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, App.hWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING, &PresentParameters, &D3DDevice);
    }
}

void Direct3D::InitializeFont()
{
	//Create/get the Windows font to be used in the D3D font
	//HFONT hFont = (HFONT)GetStockObject( SYSTEM_FONT );
	//HFONT MyFont = CreateFontA( 0, 0, 0, 0, FW_DONTCARE, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, "Arial" );
	// Create the D3DX Font
	D3DXCreateFontA( D3DDevice, 0, 0, FW_BOLD, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &FrostFont );
}

void Direct3D::InitScene()
{
	D3DDevice->SetRenderState(D3DRS_AMBIENT,RGB(255,255,255));
	D3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	D3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE); //CAN BE SET TO TRUE, if so, add D3DCLEAR_ZBUFFER after clear target (2nd clearscene param)
	
    //Set vertex shader
    D3DDevice->SetVertexShader(NULL);
    //D3DDevice->SetFVF (D3DFVF_TLVERTEX);

    //Create vertex buffer and set as stream source
	//Using D3DXSprite now, don't need these things.
    //D3DDevice->CreateVertexBuffer(sizeof(TLVERTEX) * 4, NULL, D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &vertexBuffer, NULL);
    //D3DDevice->SetStreamSource (0, vertexBuffer, 0, sizeof(TLVERTEX));

    //Setup rendering states
    D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	//Create the sprite interface needed.
	D3DXCreateSprite(D3DDevice, &Sprite);

	/*
	D3DXMATRIX Projection;
	D3DXMatrixPerspectiveFovLH(&Projection, FieldOfView, AspectRatio, NearClippingPane, FarClippingPane);
	D3DDevice->SetTransform(D3DTS_PROJECTION,&Projection);
	*/
}

void Direct3D::UpdateCamera() //Move the 3D camera, not needed in 2D.
{
	D3DXVECTOR3 EyePos(0, 0, 0);
	D3DXVECTOR3 TargetPos(0, 0, 0);
	D3DXVECTOR3 UpVector(0, 0, 0);

	D3DXMATRIXA16 View;
	D3DXMatrixLookAtLH(&View, &EyePos, &TargetPos, &UpVector);
	D3DDevice->SetTransform(D3DTS_VIEW, &View);
}

void Direct3D::Text( LPD3DXFONT Font, int X, int Y, D3DCOLOR Color, char *String )
{
	// DT_WORDBREAK might be needed.
	// Rectangle where the text will be located
	RECT TextRect={X,Y,0,0};
	// Calculate the rectangle the text will occupy
	Font->DrawTextA( Sprite, String, -1, &TextRect, DT_CALCRECT, 0 );
	// Output the text, left aligned
	Font->DrawTextA( Sprite, String, -1, &TextRect, DT_LEFT, Color );
}

void Direct3D::TextWorld( LPD3DXFONT Font, int X, int Y, D3DCOLOR Color, char *String )
{
	Text(Font, ( X - CamX ), ( Y - CamY ), Color, String);
}

//Draw a textured quad
void Direct3D::BlitD3D (IDirect3DTexture9 *texture, RECT *rDest, D3DCOLOR vertexColour, float rotate)
{
	TLVERTEX* vertices;

	//Lock the vertex buffer
	vertexBuffer->Lock(0, 0, (void **)&vertices, NULL);

	//Setup vertices
	//A -0.5f modifier is applied to vertex coordinates to match texture and screen coords
	//Some drivers may compensate for this automatically, but on others texture alignment errors are introduced
	//More information on this can be found in the Direct3D 9 documentation
	vertices[0].colour = vertexColour;
	vertices[0].x = (float) rDest->left/* - 0.5f*/;
	vertices[0].y = (float) rDest->top/* - 0.5f*/;
	vertices[0].z = 0.0f;
	vertices[0].rhw = 1.0f;
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	vertices[1].colour = vertexColour;
	vertices[1].x = (float) rDest->right/* - 0.5f*/;
	vertices[1].y = (float) rDest->top/* - 0.5f*/;
	vertices[1].z = 0.0f;
	vertices[1].rhw = 1.0f;
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;

	vertices[2].colour = vertexColour;
	vertices[2].x = (float) rDest->right/* - 0.5f*/;
	vertices[2].y = (float) rDest->bottom/* - 0.5f*/;
	vertices[2].z = 0.0f;
	vertices[2].rhw = 1.0f;
	vertices[2].u = 1.0f;
	vertices[2].v = 1.0f;

	vertices[3].colour = vertexColour;
	vertices[3].x = (float) rDest->left/* - 0.5f*/;
	vertices[3].y = (float) rDest->bottom/* - 0.5f*/;
	vertices[3].z = 0.0f;
	vertices[3].rhw = 1.0f;
	vertices[3].u = 0.0f;
	vertices[3].v = 1.0f;

  //Handle rotation
  if (rotate != 0)
  {
      RECT rOrigin;
      float centerX, centerY;

      //Find center of destination rectangle
      centerX = (float)(rDest->left + rDest->right) / 2;
      centerY = (float)(rDest->top + rDest->bottom) / 2;

      //Translate destination rect to be centered on the origin
      rOrigin.top = rDest->top - (int)(centerY);
      rOrigin.bottom = rDest->bottom - (int)(centerY);
      rOrigin.left = rDest->left - (int)(centerX);
      rOrigin.right = rDest->right - (int)(centerX);

      //Rotate vertices about the origin
      vertices[index].x = rOrigin.left * cosf(rotate) -
                                rOrigin.top * sinf(rotate);
      vertices[index].y = rOrigin.left * sinf(rotate) +
                                rOrigin.top * cosf(rotate);

      vertices[index + 1].x = rOrigin.right * cosf(rotate) -
                                    rOrigin.top * sinf(rotate);
      vertices[index + 1].y = rOrigin.right * sinf(rotate) +
                                    rOrigin.top * cosf(rotate);

      vertices[index + 2].x = rOrigin.right * cosf(rotate) -
                                    rOrigin.bottom * sinf(rotate);
      vertices[index + 2].y = rOrigin.right * sinf(rotate) +
                                    rOrigin.bottom * cosf(rotate);

      vertices[index + 3].x = rOrigin.left * cosf(rotate) -
                                    rOrigin.bottom * sinf(rotate);
      vertices[index + 3].y = rOrigin.left * sinf(rotate) +
                                    rOrigin.bottom * cosf(rotate);

      //Translate vertices to proper position
      vertices[index].x += centerX;
      vertices[index].y += centerY;
      vertices[index + 1].x += centerX;
      vertices[index + 1].y += centerY;
      vertices[index + 2].x += centerX;
      vertices[index + 2].y += centerY;
      vertices[index + 3].x += centerX;
      vertices[index + 3].y += centerY;
  }

	//Unlock the vertex buffer
	vertexBuffer->Unlock();

	//Set texture
	D3DDevice->SetTexture (0, texture);

	//Draw image
	D3DDevice->DrawPrimitive (D3DPT_TRIANGLEFAN, 0, 2);
}

//Draw a textured quad
void Direct3D::BlitExD3D (IDirect3DTexture9 *texture, RECT *rDest, D3DCOLOR *vertexColours /* -> D3DCOLOR[4] */,
             float rotate)
{
	TLVERTEX* vertices;

	//Lock the vertex buffer
	vertexBuffer->Lock(0, 0, (void **)&vertices, NULL);

	//Setup vertices
	//A -0.5f modifier is applied to vertex coordinates to match texture and screen coords
	//Some drivers may compensate for this automatically, but on others texture alignment errors are introduced
	//More information on this can be found in the Direct3D 9 documentation
	vertices[0].colour = vertexColours[0];
	vertices[0].x = (float) rDest->left - 0.5f;
	vertices[0].y = (float) rDest->top - 0.5f;
	vertices[0].z = 0.0f;
	vertices[0].rhw = 1.0f;
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	vertices[1].colour = vertexColours[1];
	vertices[1].x = (float) rDest->right - 0.5f;
	vertices[1].y = (float) rDest->top - 0.5f;
	vertices[1].z = 0.0f;
	vertices[1].rhw = 1.0f;
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;

	vertices[2].colour = vertexColours[2];
	vertices[2].x = (float) rDest->right - 0.5f;
	vertices[2].y = (float) rDest->bottom - 0.5f;
	vertices[2].z = 0.0f;
	vertices[2].rhw = 1.0f;
	vertices[2].u = 1.0f;
	vertices[2].v = 1.0f;

	vertices[3].colour = vertexColours[3];
	vertices[3].x = (float) rDest->left - 0.5f;
	vertices[3].y = (float) rDest->bottom - 0.5f;
	vertices[3].z = 0.0f;
	vertices[3].rhw = 1.0f;
	vertices[3].u = 0.0f;
	vertices[3].v = 1.0f;

  //Handle rotation
  if (rotate != 0)
  {
      RECT rOrigin;
      float centerX, centerY;

      //Find center of destination rectangle
      centerX = (float)(rDest->left + rDest->right) / 2;
      centerY = (float)(rDest->top + rDest->bottom) / 2;

      //Translate destination rect to be centered on the origin
      rOrigin.top = rDest->top - (int)(centerY);
      rOrigin.bottom = rDest->bottom - (int)(centerY);
      rOrigin.left = rDest->left - (int)(centerX);
      rOrigin.right = rDest->right - (int)(centerX);

      //Rotate vertices about the origin
      vertices[index].x = rOrigin.left * cosf(rotate) -
                                rOrigin.top * sinf(rotate);
      vertices[index].y = rOrigin.left * sinf(rotate) +
                                rOrigin.top * cosf(rotate);

      vertices[index + 1].x = rOrigin.right * cosf(rotate) -
                                    rOrigin.top * sinf(rotate);
      vertices[index + 1].y = rOrigin.right * sinf(rotate) +
                                    rOrigin.top * cosf(rotate);

      vertices[index + 2].x = rOrigin.right * cosf(rotate) -
                                    rOrigin.bottom * sinf(rotate);
      vertices[index + 2].y = rOrigin.right * sinf(rotate) +
                                    rOrigin.bottom * cosf(rotate);

      vertices[index + 3].x = rOrigin.left * cosf(rotate) -
                                    rOrigin.bottom * sinf(rotate);
      vertices[index + 3].y = rOrigin.left * sinf(rotate) +
                                    rOrigin.bottom * cosf(rotate);

      //Translate vertices to proper position
      vertices[index].x += centerX;
      vertices[index].y += centerY;
      vertices[index + 1].x += centerX;
      vertices[index + 1].y += centerY;
      vertices[index + 2].x += centerX;
      vertices[index + 2].y += centerY;
      vertices[index + 3].x += centerX;
      vertices[index + 3].y += centerY;
  }

	//Unlock the vertex buffer
	vertexBuffer->Unlock();

	//Set texture
	D3DDevice->SetTexture (0, texture);

	//Draw image
	D3DDevice->DrawPrimitive (D3DPT_TRIANGLEFAN, 0, 2);
}

//Load a texture
//Supported formats: BMP, PPM, DDS, JPG, PNG, TGA, DIB
IDirect3DTexture9 *Direct3D::LoadTexture(LOADEDTEXTURE* lt, char *fileName)
{
    IDirect3DTexture9 *d3dTexture;
    D3DXIMAGE_INFO SrcInfo;			//Optional

    //Use a magenta colourkey
    D3DCOLOR colorkey = 0xFFFF00FF;

    // Load image from file
    if (FAILED(D3DXCreateTextureFromFileEx (D3DDevice, fileName, 0, 0, 1, 0, 
          D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
          colorkey, &SrcInfo, NULL, &d3dTexture)))
    {
		MessageBox(App.hWnd, "Failed in loading a texture!", "Frost - Error!", MB_OK);
        return NULL;
    }

	lt->ImgWidth = SrcInfo.Width;
	lt->ImgHeight = SrcInfo.Height;

	//Return the newly made texture
    return d3dTexture;
}

UINT Direct3D::GetTexWidth(LPDIRECT3DTEXTURE9 Texture)
{
	D3DSURFACE_DESC surfaceDesc;
	Texture->GetLevelDesc(0, &surfaceDesc);
	return surfaceDesc.Width;
}

UINT Direct3D::GetTexHeight(LPDIRECT3DTEXTURE9 Texture)
{
	D3DSURFACE_DESC surfaceDesc;
	Texture->GetLevelDesc(0, &surfaceDesc);
	return surfaceDesc.Height;
}

//Draw texture with limited colour modulation
void Direct3D::Blit (LOADEDTEXTURE* lt, int X, int Y, D3DCOLOR vertexColour, float rotate)
{
    RECT rDest;

    //Setup destination rectangle
    rDest.left = X;
    rDest.right = X + lt->Width;
    rDest.top = Y;
    rDest.bottom = Y + lt->Height;

    //Draw texture
    BlitD3D (lt->Texture, &rDest, vertexColour, rotate);
}

void Direct3D::ScreenBlit (LOADEDTEXTURE* lt, int X, int Y, D3DCOLOR vertexColour, float rotate)
{
    RECT rDest;

    //Setup destination rectangle
    rDest.left = X;
    rDest.right = rDest.left + lt->Width;
    rDest.top = Y;
    rDest.bottom = rDest.top + lt->Height;

    //Draw texture
    BlitD3D (lt->Texture, &rDest, vertexColour, rotate);
}

void Direct3D::ScreenBlit (LOADEDTEXTURE* lt, int X, int Y, int Width, int Height, D3DCOLOR vertexColour, float rotate)
{
    RECT rDest;

    //Setup destination rectangle
    rDest.left = X;
	if (Width != -1)
	{
		rDest.right = rDest.left + Width;
	}
	else
	{
		rDest.right = rDest.left + lt->Width;
	}
    rDest.top = Y;
	if (Height != -1)
	{
		rDest.bottom = rDest.top + Height;
	}
	else
	{
		rDest.bottom = rDest.top + lt->Height;
	}

    //Draw texture
    BlitD3D (lt->Texture, &rDest, vertexColour, rotate);
}

void Direct3D::WorldBlit (LOADEDTEXTURE* lt, int X, int Y, D3DCOLOR vertexColour, float rotate)
{
    RECT rDest;

    //Setup destination rectangle
    rDest.left = X + CamX;
    rDest.right = rDest.left + lt->Width;
    rDest.top = Y + CamY;
    rDest.bottom = rDest.top + lt->Height;

    //Draw texture
    BlitD3D (lt->Texture, &rDest, vertexColour, rotate);
}

void Direct3D::Draw(LPDIRECT3DTEXTURE9 Texture, long X, long Y, RECT *pSrcRect, D3DCOLOR Color, bool WorldDraw)
{
	long CX = 0;
	long CY = 0;

	if (WorldDraw)
	{
		CX = CamX;
		CY = CamY;
	}

	D3DXVECTOR3 Position((float)X - CX, (float)Y - CY, (float)0);
	Sprite->Draw(Texture, pSrcRect, NULL, &Position, Color);
}

//Draw texture with full colour modulation
void Direct3D::BlitEx (LOADEDTEXTURE* lt, int X, int Y, D3DCOLOR* vertexColours, float rotate)
{
    RECT rDest;

    //Setup destination rectangle
    rDest.left = X;
    rDest.right = X + lt->Width;
    rDest.top = Y;
    rDest.bottom = Y + lt->Height;

    //Draw texture
    BlitExD3D (lt->Texture, &rDest, vertexColours, rotate);
}