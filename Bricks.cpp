#include "stdafx.h"
#include "resource.h"
#include <fstream.h>
#include "Bricks.h"
#include <windows.h>
#include <stdio.h>
#include <Mmsystem.h>

#define ID_TIMER    1
#define DIVISIONS	12
#define MAX_LOADSTRING 100
#define BITMAPSIZE 200
#define DEBUG 1
#define LEVELS 7
#define BALLINITXPOS 600
#define BALLINITYPOS 600

//.............................................VARIABLE DECLARATIONS
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// The title bar text
HDC hdcmem;
HDC hdc;
HWND hRwnd,hWnd;
HRGN rgn,rgn1;
HBITMAP hPicBeg,hPicInter,hPicExp;
BITMAP bitmap;
TCHAR szbuffer[100];
TCHAR szbuffer1[100];
static int TimerInterval = 20;
BOOL IsPaused = FALSE;

typedef struct BRICK_ST
{
	int flag,stat;
	int xinit,yinit,xfin,yfin;
	
}BST ;

int xPaddle=600,yPaddle=630;		//initial paddle coordinates
int r=0,c=0;
int i=0,j=0;
static int BALLS = 9;
static int TOTFLAGS=0;
static int CURRLEVEL=0;
int timer_flag;
BST brickst[DIVISIONS/2][DIVISIONS];


//............................................FUNCTION DECL

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	Levels(HWND , UINT, WPARAM, LPARAM);
LRESULT CALLBACK	Flash(HWND , UINT, WPARAM, LPARAM);
LRESULT CALLBACK	StartMenu(HWND , UINT, WPARAM, LPARAM);
LRESULT CALLBACK	Controls(HWND , UINT , WPARAM, LPARAM );
LRESULT CALLBACK	BallSpeed(HWND, UINT, WPARAM, LPARAM);
VOID CALLBACK		FlashTimer(HWND,UINT,UINT_PTR,DWORD);

VOID DrawBitmap(HDC hdc, int xStart, int yStart, HBITMAP hBitmap);
VOID resetBall(HWND hwnd,HDC hdc, HBITMAP hBitmap);

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BRICKS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	
	// Perform application initialization:
	
	nCmdShow = SW_SHOWMAXIMIZED;
	
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}
	
	
	//DialogBox(hInst, (LPCTSTR)IDD_START_MENU, hWnd, (DLGPROC)StartMenu);	
	
	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_BRICKS);
	
	
	// Main message loop:
		
	
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	
	wcex.cbSize = sizeof(WNDCLASSEX); 
	
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_ICON1);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+4);
	wcex.lpszMenuName	= (LPCSTR)IDC_BRICKS;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);
	
	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable
	
	hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUPWINDOW,//WS_SYSMENU |WS_CAPTION |WS_MAXIMIZE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	
	if (!hWnd)
	{
		return FALSE;
	}
	
	DialogBox(hInst, (LPCTSTR)IDD_FLASH, hWnd, (DLGPROC)Flash);
		
	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	DialogBox(hInst, (LPCTSTR)IDD_START_MENU, hWnd, (DLGPROC)StartMenu);		
	return TRUE;
}




LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	
	
	hRwnd = hWnd;
	
	//.....................MAYUR START
	int wmId, wmEvent;
	TCHAR szHello[MAX_LOADSTRING];
	//LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	
	//.....................MAYUR END
	
	
	
	//KGB CODE-----------------------------START
	static HBITMAP		hBitmap1,hBitmap2,hPaddle,hBall,hBrick1,hBrick2,hBrick3,hBrick4,hBackgr1,hSide;
	static int			cxClient, cyClient, padWidth, padHeight ,cxs,cys,cx,cy,ballWidth,
		ballHeight,xCenter, yCenter, cxTotal, cyTotal,scrxCenter,scryCenter,
		cxRadius, cyRadius, ballxMove, ballyMove, xPixel, yPixel,radius,score;
	
	BITMAP         bitmap ;
	HDC            hdc, hdcMem ,hdcMemBall;
	HINSTANCE      hInstance ;
	HFONT			hfont ;
	PAINTSTRUCT    ps ;
	HRGN			hRgn;
	TCHAR			szb[100] ;
	TCHAR			szbuffer[100] ;
	//-----------------------------
	/*checker code variables*/
	
	static BOOL fState[DIVISIONS][DIVISIONS] ;
	static int  brWidth, brHeight;
	int         gridX, gridY ;
	RECT        rect ;
	const char* name[7] = {"Level01.txt","Level02.txt","Level03.txt",
		"Level04.txt","Level05.txt","Level06.txt","Level07.txt"};
	
	FILE *inp ;
	
	//----------------------------
	
	switch (message) 
	{
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	case WM_CREATE:
		
		timer_flag = 0;
		//populate the Brick structure.......
		
		
		//----*****************FILE READING***************-------//
		
		inp=fopen(name[CURRLEVEL],"r");
		for( i=0;i<DIVISIONS/2;i++)
		{	
			for( j=0;j<DIVISIONS;j++)
			{
				fscanf( inp,"%d",&(brickst[i][j].flag) );
				TOTFLAGS += (brickst[i][j].flag);
				//wsprintf(szbuffer,TEXT("totflag= %d"),TOTFLAG);
				//MessageBox(hWnd,szbuffer,TEXT("rad"),0);
		

			}
		}
		//fclose(inp);
		//----***************END FILE READING ***********--------//
		
		hInstance = ((LPCREATESTRUCT) lParam)->hInstance ;
		
		hPaddle = LoadBitmap (hInstance, TEXT("paddle")) ;
		GetObject (hPaddle, sizeof (BITMAP), &bitmap) ;
		
		padWidth = bitmap.bmWidth ;
		padHeight = bitmap.bmHeight ;
		
		hBrick1 = (HBITMAP)LoadImage(hInstance, TEXT("br_red1.bmp"),IMAGE_BITMAP,68,48,LR_LOADFROMFILE) ;

		hBrick2 = (HBITMAP)LoadImage(hInstance, TEXT("br_cyan2.bmp"),IMAGE_BITMAP,68,48,LR_LOADFROMFILE) ;
		
		hBrick3 = (HBITMAP)LoadImage(hInstance, TEXT("br_green3.bmp"),IMAGE_BITMAP,68,48,LR_LOADFROMFILE) ;

		hBrick4 = (HBITMAP)LoadImage(hInstance, TEXT("br_purple4.bmp"),IMAGE_BITMAP,68,48,LR_LOADFROMFILE) ;

	//	hBrick5 = (HBITMAP)LoadImage(hInstance, TEXT("br_green4.bmp"),IMAGE_BITMAP,68,48,LR_LOADFROMFILE) ;

		hSide=	(HBITMAP)LoadImage(hInstance, TEXT("side1.bmp"),IMAGE_BITMAP,200,768,LR_LOADFROMFILE) ;

		hBackgr1 = (HBITMAP)LoadImage(hInstance, TEXT("back1.bmp"),IMAGE_BITMAP,824,768,LR_LOADFROMFILE) ;
		//cyClient = HIWORD (lParam);
		//cxClient = LOWORD (lParam) ;
		
		//.............ball
		
		hBall=LoadBitmap(hInstance,TEXT("Ball"));
		GetObject(hBall,sizeof(BITMAP),&bitmap);
		
		ballWidth=bitmap.bmWidth;
		ballHeight=bitmap.bmHeight;
		
		
		radius=ballWidth/2;
		
		//wsprintf(szbuffer,TEXT("radius= %d, xCenter=%d, yCenter=%d,xPaddle=%d,yPaddle=%d"),radius,xCenter,yCenter,xPaddle,yPaddle);
		//MessageBox(hWnd,szbuffer,TEXT("rad"),0);
		
		//.......................Initializing the TIMER
		
		//SetTimer (hWnd, ID_TIMER, 20, NULL);
		PlaySound(TEXT("Start_game.wav"),NULL,SND_FILENAME | SND_ASYNC );		
		return 0 ;
		
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		
	case WM_SIZE:		
		
		//...............................	Get window width and height
		cxClient = LOWORD (lParam) ;
		cyClient = HIWORD (lParam) ;
		//-------------------------------//
		
		//Screen center------------//
		scrxCenter = (cxClient = LOWORD (lParam)) / 2 ;
		scryCenter = (cyClient = HIWORD (lParam)) / 2 ;
		
		//...............................reducing the client area
		cxClient=((LOWORD(lParam))-200);		

		xCenter = BALLINITXPOS;
		yCenter = BALLINITYPOS;
		
		ballxMove = 3 ;
		ballyMove = 3 ;
		
		//................. BRICK width and height
		//brWidth = LOWORD (lParam) / DIVISIONS ;
		
		brWidth = cxClient / DIVISIONS ;
		//brHeight = HIWORD (lParam) / DIVISIONS ;
		brHeight = HIWORD (lParam) / 16 ;
		
		//wsprintf(szbuffer,TEXT("brWidth= %d, brHeight=%d"),cxClient,cyClient);
		//MessageBox(hWnd,szbuffer,TEXT("rad"),0);
		
		
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
		
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:

			if(IsPaused == TRUE) break;			
			hRgn = CreateRectRgn(xPaddle-20, yPaddle,xPaddle+padWidth,yPaddle+padHeight);
			
			xPaddle-=15;
			
			if(xPaddle<=0)
				xPaddle=0;
			
			InvalidateRgn(hWnd,hRgn,TRUE);
			UpdateWindow(hWnd);
			

			if(!timer_flag)
			{
				hRgn = CreateRectRgn( xCenter-15 , yCenter-10, xCenter+ballWidth+15,yCenter+ballHeight+15 );
				

				if(xCenter-radius>10)
				{
					xCenter-=15;
					InvalidateRgn(hWnd,hRgn,TRUE);
					UpdateWindow(hWnd);
				}
			}
			
			break ;
			
		case VK_RIGHT:

			if(IsPaused == TRUE) break;			
			
			hRgn = CreateRectRgn(xPaddle, yPaddle,xPaddle+padWidth+20,yPaddle+padHeight);
			
			xPaddle+=15;
			
			if(xPaddle+padWidth>=cxClient)
				xPaddle=cxClient-padWidth;
			
			InvalidateRgn(hWnd,hRgn,TRUE);
			UpdateWindow(hWnd);
			
			if(!(timer_flag))
			{
				hRgn = CreateRectRgn( xCenter-10, yCenter-10, xCenter+ballWidth+15,yCenter+ballHeight+5 );
				
				if(xCenter+ballWidth<cxClient)
				{
					xCenter+=15;
					InvalidateRgn(hWnd,hRgn,TRUE);
					UpdateWindow(hWnd);
				}
			}
			
			
			break ;
			
			
		case VK_SPACE:
			
			PlaySound(TEXT("SpaceBar.wav"),NULL,SND_FILENAME | SND_ASYNC );
			SetTimer (hWnd, ID_TIMER, TimerInterval, NULL);
			timer_flag=1;
			IsPaused = FALSE;

			break;
		}
		return 0 ;
		
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		//...............................................MAYUR
		
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
			case IDM_ABOUT:

				DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				break;
				
			case IDM_EXIT:

				DestroyWindow(hWnd);
				break;
				
			case IDM_LEVEL:

				DialogBox(hInst, (LPCTSTR)IDD_SPEED, hWnd, (DLGPROC)BallSpeed);
			case IDM_PAUSE:

				timer_flag = 0;
				KillTimer(hWnd,ID_TIMER);
				IsPaused = TRUE;
		
				break;
				
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
			
			
			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			//..........................................KGB
			
			case WM_PAINT:
				
				//this code

					
				
				
				hdc = BeginPaint (hWnd, &ps) ;
								
				hdcMem = CreateCompatibleDC (hdc) ;
				SelectObject (hdcMem, hBackgr1) ;

				BitBlt(hdc,0,0,824,768,hdcMem,0,0,SRCCOPY);

				DeleteDC(hdcMem); 


				hdcMem = CreateCompatibleDC (hdc) ;
				SelectObject (hdcMem, hSide) ;

				BitBlt(hdc,824,0,200,768,hdcMem,0,0,SRCCOPY);

				DeleteDC(hdcMem);

				//----------------------------
				
				wsprintf(szbuffer,TEXT("%d"),score);
							
							hfont = CreateFont(50,18,0,80,900,false,false,false,false,false,false,false,FF_ROMAN,NULL);
							SetBkColor(hdc,RGB(0,0,0));
							SelectObject(hdc,hfont);
							SetTextColor(hdc,RGB(255,255,255));
							
							TextOut(hdc, 875, 70, szbuffer,lstrlen(szbuffer));
				
				
				hfont = CreateFont(50,18,0,80,900,false,false,false,false,false,false,false,FF_ROMAN,NULL);
						SetBkColor(hdc,RGB(0,0,0));
						SelectObject(hdc,hfont);
						SetTextColor(hdc,RGB(255,255,255));
						wsprintf(szbuffer1,TEXT("%d"),BALLS);
						TextOut(hdc, 875, 210, szbuffer1,lstrlen(szbuffer1));
			
				
				
							//_---------------------
							hdcMem = CreateCompatibleDC (hdc) ;
				SelectObject (hdcMem, hPaddle) ;
				
				TransparentBlt(hdc,xPaddle,yPaddle,padWidth,padHeight,
								hdcMem,0,0,padWidth,padHeight,RGB(255,255,255));
				
				DeleteDC (hdcMem) ;
				

				

				

				//......................Drawing a vertical line
				MoveToEx(hdc,824,0,NULL);LineTo(hdc,824,768);

				//.......................
				
				hdcMemBall = CreateCompatibleDC (hdc) ;
				SelectObject (hdcMemBall, hBall) ;
				
				TransparentBlt(hdc,xCenter,yCenter, ballWidth, ballHeight,
					hdcMemBall,0,0,ballWidth,ballHeight,RGB(255,255,255));
				
				
				//.............................determine the bricks to be 'displayed'
				
				for (gridY = 0 ; gridY < DIVISIONS/2 ; gridY++)

				{
					for (gridX = 0 ; gridX < DIVISIONS ; gridX++)
					{
						//Rectangle (hdc, gridX * brWidth, gridY * brHeight,
						//	(gridX + 1) * brWidth, (gridY + 1) * brHeight) ;
						
						if(brickst[gridY][gridX].flag!=0)
						{
							
							switch(brickst[gridY][gridX].flag)
							{
							case 0:
								break;
							case 1:
								DrawBitmap(hdc,gridX*brWidth,gridY*brHeight,hBrick1);
								break;
							case 2:
								DrawBitmap(hdc,gridX*brWidth,gridY*brHeight,hBrick2);
								break;
							case 3:
								DrawBitmap(hdc,gridX*brWidth,gridY*brHeight,hBrick3);
								break;
							case 4:
								DrawBitmap(hdc,gridX*brWidth,gridY*brHeight,hBrick4);
								break;
							
							
							default:
								{
									wsprintf(szbuffer,TEXT("gridx= %d, grid=%d"),gridX,gridY);
									MessageBox(hWnd,szbuffer,TEXT("rad"),0);
									break;
								}
							
							}
							
						}
						
						//TODOOO extend here for different flags using else if
					}
				}
				//end checker code -------------
				
				ReleaseDC (hWnd, hdc) ;
				DeleteDC (hdcMemBall) ;			
				
				EndPaint (hWnd, &ps) ;
				
				//----------------
				break ;
				
				//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%55	
				//...kgb	
			case WM_TIMER:
				
				
				// bouncing code
				//hRgn=CreateEllipticRgn ( xCenter+radius, yCenter+radius,2*(xCenter+(radius)),2*(yCenter+(radius)) );
				hdc=GetDC(hWnd);
				
				{
					
					hRgn=CreateEllipticRgn ( xCenter-15 , yCenter-15, xCenter+ballWidth+15,yCenter+ballHeight+15 );
					
					if (!hBall)
						break ;
					
					if(!hPaddle)
						break;

					if(!hBrick1)
						break;

					xCenter += ballxMove ;
					yCenter += ballyMove ;
					
					if(yCenter>cyClient)
						break;
					
					//CHECKER CODE----------------------
					
					if(brWidth!=0 && brHeight!=0)
					{
						gridX=xCenter/brWidth;
						gridY=yCenter/brHeight;
					}
					
					if (gridX < DIVISIONS && gridY < DIVISIONS/2)
					{
						if(brickst[gridY][gridX].flag>0)
						{	
							if( brickst[gridY][gridX].flag<4)
							{
								(brickst[gridY][gridX].flag)--;
								TOTFLAGS--;
								PlaySound(TEXT("Bricks.wav"),NULL,SND_FILENAME | SND_ASYNC );
							}
							else
							{
								PlaySound(TEXT("Blocks.wav"),NULL,SND_FILENAME | SND_ASYNC );
							}
							
							
							//fState [gridX][gridY] ^= 1 ;
							
							rect.left   = gridX * brWidth ;
							rect.top    = gridY * brHeight;
							rect.right  = (gridX + 1) * brWidth ;
							rect.bottom = (gridY + 1) * brHeight+2;
							
							InvalidateRect (hWnd, &rect, FALSE) ;
							
							ballyMove=-ballyMove;
							//ballxMove=-ballxMove;
						
						
							
							//**************SCORING*******************************88
							

							score = score + 30; //####################
							wsprintf(szbuffer,TEXT("%d"),score);
							
							hfont = CreateFont(50,18,0,80,900,false,false,false,false,false,false,false,FF_ROMAN,NULL);
							SetBkColor(hdc,RGB(0,0,0));
							SelectObject(hdc,hfont);
							SetTextColor(hdc,RGB(255,255,255));
							
							TextOut(hdc, 875, 70, szbuffer,lstrlen(szbuffer));
													
											
							//**************88SCORING END *******************************88
							
						}
					}
					
					/*
					//--------------------CHEKCER cODE ENDS--
					
					  //FROM LEFT DIR---------------
					  if(((((yCenter + (radius)<=yPaddle-9)&&(yCenter + (radius))>=yPaddle-12))
					  &&((xCenter+radius)>=xPaddle-15 && (xCenter+radius) <= xPaddle+20)))
					  {
					  ballyMove=-ballyMove;
					  ballxMove=-ballxMove-2;
					  }
					  
						
						  else if(((((yCenter + (radius)<=yPaddle-9)&&(yCenter + (radius))>=yPaddle-12))
						  &&((xCenter+radius)>=xPaddle+80 && (xCenter+radius) <= (xPaddle+115))))
						  {
						  ballyMove=-ballyMove;
						  ballxMove=ballxMove+1;
						  }
						  
							//FROM RIGHT DIR---------------
							else if(((((yCenter + (radius)<=yPaddle-9)&&(yCenter + (radius))>=yPaddle-12))
							&&((xCenter-radius)<=xPaddle+110 && (xCenter-radius) >= xPaddle+80)))
							{
							ballyMove=-ballyMove;
							ballxMove=-ballxMove-2;
							}
							else if(((((yCenter + (radius)<=yPaddle-9)&&(yCenter + (radius))>=yPaddle-12))
							&&((xCenter-radius)<=xPaddle+20 && (xCenter-radius) > (xPaddle-15))))
							{
							ballyMove=-ballyMove;
							ballxMove=ballxMove+2;
							}
							
							  //middle section----------
							  else if(((((yCenter + (radius)<=yPaddle-9)&&(yCenter + (radius))>=yPaddle-12))
							  &&(xCenter+radius>xPaddle+20 && (xCenter-radius) < (xPaddle+80))))
							  {
							  ballyMove=-ballyMove;
							  ballxMove=ballxMove;
							  }
					*/
					
					
					//...........................kgb CLIENT SECTION
					//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


					//NEW MIDDLE SECTION
					if( (yCenter<=yPaddle-20) && (yCenter>=yPaddle-27) 
						&& (xCenter>xPaddle+10) && (xCenter<xPaddle+90) )
					{
						ballyMove=-ballyMove;
						ballxMove=ballxMove;
						PlaySound(TEXT("Paddle.wav"),NULL,SND_FILENAME | SND_ASYNC );
					}
					
					//NEW LEFT TO LEFT
					if( (yCenter<=yPaddle-20) && (yCenter>=yPaddle-27) 
						&& (xCenter>=xPaddle-20) && (xCenter<=xPaddle+10) )
						
					{

						ballyMove=-ballyMove;
						ballxMove=-ballxMove-2;
						PlaySound(TEXT("Paddle.wav"),NULL,SND_FILENAME | SND_ASYNC );
					}
					
					//NEW RIGHT TO RIGHT
					if( (yCenter<=yPaddle-20) && (yCenter>=yPaddle-27) 
						&& (xCenter>=xPaddle+90) && (xCenter<=xPaddle+110) )
						
					{
						ballyMove=-ballyMove;
						ballxMove=-ballxMove-2;
						PlaySound(TEXT("Paddle.wav"),NULL,SND_FILENAME | SND_ASYNC );
					}
					
					//LEFT DOWN
					if( (yCenter<=yPaddle+5) && (yCenter>=yPaddle-21)
						&& (xCenter<xPaddle) && (xCenter>xPaddle-25) )
					{
						ballyMove=ballyMove;
						ballxMove=-ballxMove;
						PlaySound(TEXT("Paddle.wav"),NULL,SND_FILENAME | SND_ASYNC );
					}
					
					//RIGHT DOWN
					if( (yCenter<=yPaddle+5) && (yCenter>=yPaddle-21)
						&& (xCenter>xPaddle+100) && (xCenter<xPaddle+125) )
					{
						ballyMove=ballyMove;
						ballxMove=-ballxMove;
						PlaySound(TEXT("Paddle.wav"),NULL,SND_FILENAME | SND_ASYNC );
					}
					
					
					
					//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&7		
					
					if(yCenter - radius <= 0)
						ballyMove=-ballyMove;
					
					if ((xCenter + ballWidth >= cxClient))
						ballxMove = -ballxMove ;
					
					if (xCenter -radius-5 <= 0)
						ballxMove=-ballxMove;
					
					
					//............................Ball Missed!
					
					if((yCenter> yPaddle + 30))
							PlaySound(TEXT("Ball_Miss.wav"),NULL,SND_FILENAME | SND_ASYNC );

					if( ( yCenter > yPaddle + 115 ) && BALLS > 0 )
					{
						BALLS--;
						KillTimer(hWnd,ID_TIMER);
						timer_flag=0;
						
						resetBall(hWnd,hdc,hBall);
						
						ballxMove=3;
						ballyMove=3;
						
						xCenter = xPaddle + 30;
						yCenter = yPaddle - 30;

						hfont = CreateFont(50,18,0,80,900,false,false,false,false,false,false,false,FF_ROMAN,NULL);
						SetBkColor(hdc,RGB(0,0,0));
						SelectObject(hdc,hfont);
						SetTextColor(hdc,RGB(255,255,255));
						wsprintf(szbuffer1,TEXT("%d"),BALLS);
						TextOut(hdc, 875, 210, szbuffer1,lstrlen(szbuffer1));

						//SendMessage(hWnd,WM_SIZE,NULL,NULL);
						//SendMessage(hWnd,WM_PAINT,NULL,NULL);*/
					}
					
					//TO CHANGE LEVELS-----------
					if( TOTFLAGS == 0 )
					{
						if(CURRLEVEL<6) CURRLEVEL++;
						KillTimer(hWnd,ID_TIMER);
										
						
						//call the file read
						//----*****************FILE READING***************-------//
						
						inp=NULL;
						inp=fopen(name[CURRLEVEL],"r");
						
						for( i=0;i<DIVISIONS/2;i++)
						{	
							for( j=0;j<DIVISIONS;j++)
							{
								fscanf( inp,"%d",&(brickst[i][j].flag) );
								TOTFLAGS += (brickst[i][j].flag);
							}
						}
						//fclose(inp);
						//----***************END FILE READING ***********--------//

						xCenter = xPaddle + 30;
						yCenter = yPaddle - 30;
						
						rgn1=CreateRectRgn(0,0,cxClient,cyClient);
						
						InvalidateRgn(hWnd,rgn1,TRUE);
						
						//ShowWindow(hWnd,nCmdShow);
						UpdateWindow(hWnd);
						
						resetBall(hWnd,hdc,hBall);
					}
					
					InvalidateRgn(hWnd,hRgn,TRUE);
					UpdateWindow(hWnd);
					DeleteObject(hRgn);
					
			}	//--------------------------------
			
			break;
			//END OF TIMER-------------------
			
			case WM_DESTROY:

				DeleteObject (hBall) ;
				DeleteObject (hBrick1) ;
				DeleteObject (hBrick2) ;
				DeleteObject (hBrick3) ;
				DeleteObject (hBrick4) ;
				DeleteObject (hBackgr1) ;
				DeleteObject (hSide) ;
				DeleteObject (hPaddle) ;

				KillTimer(hWnd,ID_TIMER);

				PostQuitMessage(0);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//							***KGB ENDS HERE***
//%%%%%%%%%%%%%%%%%%%%%%%%%%%					%%%%%%%%%%%%%%%%%%%%%%5




// -----------------------Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
		
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
    return FALSE;
}

//-----------------------------------message handler for controls box

LRESULT CALLBACK Controls(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_COMMAND:
			if (LOWORD(wParam) == ID_COK) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
		break;
	}

  return FALSE;
}

// -----------------------------Mesage handler for Levels box-----------------------
LRESULT CALLBACK BallSpeed(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int temp;
	
	switch(message)
	{
	case WM_CREATE:

        return TRUE ;
		
	case WM_INITDIALOG:
		temp = 20;
		CheckRadioButton (hDlg,IDC_BEG, IDC_EXP,IDC_BEG);
		
		return TRUE;
		
	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			TimerInterval = temp;
			EndDialog (hDlg, FALSE) ;
			return TRUE ;
			
		case IDCANCEL:
			
			EndDialog (hDlg, FALSE) ;
			return TRUE ;
			
		case IDC_BEG:
			temp= 20;
			
			CheckRadioButton (hDlg,IDC_BEG, IDC_EXP,IDC_BEG);
			
			return TRUE ;
			
		case IDC_INTER:
			temp= 16;
			CheckRadioButton (hDlg,IDC_BEG, IDC_EXP,IDC_INTER);
			return TRUE ;
			
		case IDC_EXP:
			temp= 12;
			CheckRadioButton (hDlg,IDC_BEG, IDC_EXP,IDC_EXP);
			return TRUE ;
		}
	}
	
	return FALSE;
}

//------------------------start-menu-----------------------------


LRESULT CALLBACK	StartMenu(HWND hSdlg, UINT message,WPARAM wParam , LPARAM lParam)
{
	TCHAR sztemp[100];
	
	switch(message)
	{
	case WM_CREATE:
		ShowWindow(hSdlg,SW_SHOWMAXIMIZED);		
		
        return TRUE ;
		
	case WM_INITDIALOG:
		
		return TRUE;
		
	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDC_SSTART: 
			
			EndDialog (hSdlg, FALSE) ;
			return TRUE ;
			
		case IDC_SSLIDER:
			DialogBox(hInst, (LPCTSTR)IDD_SPEED, hWnd, (DLGPROC)BallSpeed);
			SetFocus((HWND)lParam);
			return TRUE ;
			
		case IDC_SABOUT:
			
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			SetFocus(hSdlg);
			return TRUE;
			
		case IDC_SEXIT:
			
			DestroyWindow(hWnd);
			return TRUE;
			
		case IDC_SHSCORE:
			
			return TRUE;
			
		case IDC_SCONTROLS:
			DialogBox(hInst, (LPCTSTR)IDD_CONTROLS, hWnd, (DLGPROC)Controls);
			SetFocus(hSdlg);
			

			return TRUE;
		}
		return TRUE;
	}
	return  FALSE;
}


//-------------------------flash screen----------------------------

LRESULT CALLBACK Flash(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	SetTimer(hDlg,1,3000,(TIMERPROC)FlashTimer);
	
	return FALSE;
}

VOID CALLBACK FlashTimer(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
				EndDialog(hwnd,TRUE);
				KillTimer(hwnd,1);
}


VOID DrawBitmap (HDC hdc, int xStart, int yStart, HBITMAP hBitmap)
{
				BITMAP bm ;
				HDC    hMemDC ;
				POINT  pt ;
				
				hMemDC = CreateCompatibleDC (hdc);
				SelectObject (hMemDC, hBitmap) ;
				GetObject (hBitmap, sizeof (BITMAP), &bm) ;
				pt.x = bm.bmWidth ;
				pt.y = bm.bmHeight ;
				
				BitBlt (hdc, xStart, yStart, pt.x, pt.y, hMemDC, 0, 0, SRCCOPY) ;
				
				//	ReleaseDC(hWnd,hdc);
				DeleteDC (hMemDC) ;
				
}

VOID resetBall(HWND hwnd,HDC hdc, HBITMAP hBitmap)
{
	BITMAP bm ;
	HDC    hMemDC ;
	TCHAR sztemp[100];

	hMemDC = CreateCompatibleDC (hdc);
	
	SelectObject (hMemDC, hBitmap) ;
	GetObject (hBitmap, sizeof (BITMAP), &bm) ;
	
	TransparentBlt (hdc, xPaddle+30, yPaddle-30, 30, 30, hMemDC, 0, 0,30,30, RGB(255,255,255)) ;

	DeleteDC (hMemDC) ;
	
}
