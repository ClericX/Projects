///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawAQuad(LPDIRECT3DDEVICE9 Device)
{
		LoadTexture(Texture, "C:\\Users\\Yoko\\Desktop\\lul.png");
		LPDIRECT3DVERTEXBUFFER9 QuadVertexBuffer = NULL;
		VOID* pData;
		D3DVERTEX Quad[] = {	{-1.0f, -1.0f, 0.0f, 0xffffffff, 0.0f, 0.0f},
						{1.0f, -1.0f, 0.0f, 0xffffffff, 1.0f, 0.0f},
						{-1.0f, 1.0f, 0.0f, 0xffffffff, 0.0f, 1.0f},
						{1.0f, 1.0f, 0.0f, 0xffffffff, 1.0f, 1.0f}};

		Device->CreateVertexBuffer(sizeof(Quad),D3DUSAGE_WRITEONLY,D3DFVF_CUSTOMVERTEX,D3DPOOL_MANAGED,&QuadVertexBuffer,NULL);

		QuadVertexBuffer->Lock(0,sizeof(pData),(void**)&pData,0);
		memcpy(pData,Quad,sizeof(Quad));
		QuadVertexBuffer->Unlock();

		Device->SetTexture(0,Texture->Texture);
		Device->SetStreamSource(0,QuadVertexBuffer,0,sizeof(D3DVERTEX));
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

		QuadVertexBuffer->Release();
		Texture->Texture->Release();
}

void DrawAPyramid(LPDIRECT3DDEVICE9 Device)
{
	LPDIRECT3DVERTEXBUFFER9 pTriangleVB = NULL;
	LPDIRECT3DVERTEXBUFFER9 pQuadVB = NULL;
	VOID* pData;

	D3DVERTEX aTriangle[] = {{ 0.0f, 1.0f, 0.0f,0xffff0000, 0.0f, 0.0f}, //1. tri (NEW)
                         {-1.0f,-1.0f,-1.0f,0xff00ff00, 0.0f, 0.0f},
                         { 1.0f,-1.0f,-1.0f,0xff0000ff, 0.0f, 0.0f},
                         { 0.0f, 1.0f, 0.0f,0xffff0000, 0.0f, 0.0f}, //2. tri (NEW)
                         {-1.0f,-1.0f, 1.0f,0xff0000ff, 0.0f, 0.0f},
                         {-1.0f,-1.0f,-1.0f,0xff00ff00, 0.0f, 0.0f},
                         { 0.0f, 1.0f, 0.0f,0xffff0000, 0.0f, 0.0f}, //3. tri (NEW)
                         { 1.0f,-1.0f, 1.0f,0xff00ff00, 0.0f, 0.0f},
                         {-1.0f,-1.0f, 1.0f,0xff0000ff, 0.0f, 0.0f},
                         { 0.0f, 1.0f, 0.0f,0xffff0000, 0.0f, 0.0f}, //4. tri (NEW)
                         { 1.0f,-1.0f,-1.0f,0xff0000ff, 0.0f, 0.0f},
                         { 1.0f,-1.0f, 1.0f,0xff00ff00, 0.0f, 0.0f}};

	D3DVERTEX aQuad[] = {{-1.0f,-1.0f,-1.0f,0xffffff00, 0.0f, 0.0f},     //1 quad (NEW)
                     { 1.0f,-1.0f,-1.0f,0xffffff00, 0.0f, 0.0f},
                     {-1.0f,-1.0f, 1.0f,0xffffff00, 0.0f, 0.0f},
                     { 1.0f,-1.0f, 1.0f,0xffffff00, 0.0f, 0.0f}};

	Device->CreateVertexBuffer(sizeof(aTriangle),D3DUSAGE_WRITEONLY,D3DFVF_CUSTOMVERTEX,D3DPOOL_MANAGED,&pTriangleVB,NULL);

	pTriangleVB->Lock(0,sizeof(pData),(void**)&pData,0);
	memcpy(pData,aTriangle,sizeof(aTriangle));
	pTriangleVB->Unlock();

	Device->CreateVertexBuffer(sizeof(aQuad),D3DUSAGE_WRITEONLY,D3DFVF_CUSTOMVERTEX,D3DPOOL_MANAGED,&pQuadVB,NULL);

	pQuadVB->Lock(0,sizeof(pData),(void**)&pData,0);
	memcpy(pData,aQuad,sizeof(aQuad));
	pQuadVB->Unlock();

	Device->SetStreamSource(0,pTriangleVB,0,sizeof(D3DVERTEX));
	Device->DrawPrimitive(D3DPT_TRIANGLELIST,0,4);

	Device->SetStreamSource(0,pQuadVB,0,sizeof(D3DVERTEX));
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

	pTriangleVB->Release();
	pQuadVB->Release();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Needed variables.
bool bMouseCam = false;
POINT MiddleOfClient;
POINT NewMousePos;
POINT MouseMovement;

/*
This function actually performs the actions needed to achieve the desired effect.
*/
void MouseCam()
{
	//Calculate middle of client area and set cursor to it.
	MiddleOfClient.x = (App.ClientRect.right - App.ClientRect.left) / 2;
	MiddleOfClient.y = (App.ClientRect.bottom - App.ClientRect.top) / 2;
	ClientToScreen(App.hWnd, &MiddleOfClient);
	SetCursorPos(MiddleOfClient.x, MiddleOfClient.y);

	while (bMouseCam)
	{
		//Get new cursor position and make calculations of how much it moved
		GetCursorPos(&NewMousePos);
		MouseMovement.x = MiddleOfClient.x - NewMousePos.x;
		MouseMovement.y = MiddleOfClient.y - NewMousePos.y;

		//Set camera to new positioning
		D3D.TargetX -= (float)( (float)MouseMovement.x / 15.0f );
		D3D.TargetY -= (float)( (float)MouseMovement.y / 15.0f );

		//Set cursor to the middle again
		SetCursorPos(MiddleOfClient.x, MiddleOfClient.y);
		Sleep(1);
	}
}

/*
Call this function to trigger mouse camera movement on/off.
*/
void TriggerMouseCam()
{
	if (bMouseCam)
	{
		//if true
		bMouseCam = false;
		SetCursor(App.InGameCursor);
	}
	else if (!bMouseCam)
	{
		//if false
		bMouseCam = true;
		SetCursor(App.NoCursor);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&MouseCam, NULL, NULL, NULL);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
EyeZ = 5 = shows front and  back of stuff at current viewpoint (you're moving)
EyeX = shows left and right of stuff at current viewpoint (you're moving)
EyeY = shows top and bottom of stuff at current viewpoint (you're moving)

TargX = shows left and right of you (camera's moving)
TargY = shows top and bottom of you (camera's moving)
TargZ = below EyeZ = u see it, @ EyeZ and +, you don't = piece of shit :D

UpX = rotate left/right
UpY = negative u see it, 0 u dont, positive it's upside down.
UpZ = doesnt appear to do shit
*/