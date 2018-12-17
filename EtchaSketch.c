#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


//source from Dr. Eike Anderson starts here; it originates from Lecture notes for CATA Lvl4 and it manipulates color

//colour struct
typedef struct RGBcolour
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
} RGBcolour; 

//function for getting pixel color from renderer
RGBcolour getPixelColour(SDL_Renderer *renderer,int width,int height,int x,int y)
{
	RGBcolour pcol;
	Uint32 pixels[10]; /* pixel and safety buffer (although 1 should be enough) */
	pcol.r=0;
	pcol.g=0;
	pcol.b=0;
	if(x>=0 && x<width && y>=0 && y<height) /* test if the coordinates are valid */
	{
		SDL_Rect rect; /* SDL rectangle of 1 pixel */
						/* 2 helper structures to get SDL to generate the right pixel format */
		SDL_Surface *s = SDL_CreateRGBSurface(0,5,5,32,0,0,0,0); /* helper 1 */
		SDL_Surface *ns = SDL_ConvertSurfaceFormat(s,SDL_PIXELFORMAT_ARGB8888,0); /* 2 */
		rect.x=x;
		rect.y=y;
		rect.w=1;
		rect.h=1;
		/* renderer, pixel, target format, target array, safety value 5 (1 was needed) */
		if(!SDL_RenderReadPixels(renderer,&rect,SDL_PIXELFORMAT_ARGB8888,pixels,5))
		{ /* pixel, pixel format (from helper 2), colour channels by reference */
			SDL_GetRGB(pixels[0],ns->format,&(pcol.r),&(pcol.g),&(pcol.b));
		}
		SDL_FreeSurface(s); /* free helper 1 */
		SDL_FreeSurface(ns); /* free helper 2 */
	}
	return pcol;
}
//source from Dr. Eike Anderson ends here

//function for loading images
void LoadImage(char *Image_Path, SDL_Window *window,SDL_Renderer *renderer, int img_x, int img_y)
{
	SDL_Surface *pic = IMG_Load(Image_Path);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, pic);
		
	SDL_Rect pos; //set up position of the image
	pos.x = img_x;
	pos.y = img_y;
	SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
	SDL_RenderCopy(renderer, texture, NULL, &pos);
		
	//Free surface and texture
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(pic);
		
}

//function for loading color swatches
void LoadColors(SDL_Window* window, SDL_Renderer *renderer, int img_x, int img_y)
{
	char imgpath[25];
	int i=0, c=0;
	for (i=1; i<=7; i++, img_y+=15)
	{
		for (c=1; c<=5; c++)
		{
			sprintf(imgpath, "./Images/Colors/%d_%d.jpg", i, c);
			LoadImage(imgpath,window,renderer, img_x+15*(c-1), img_y);
		}
	}
}

//function for setting up user interface, including color swatches
void LoadUI(SDL_Window *window, SDL_Renderer *renderer)
{
	//Clear renderer
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	
	//Load Title bar, tool bar
	LoadImage("./Images/Base.png",window,renderer,-2,0);
	LoadImage("./Images/Tool_Bar.png",window,renderer,0,0);
	LoadImage("./Images/Head_Title.png",window,renderer,0,0);
	
	//Load Icons
	LoadImage("./Images/Icon_New.png",window,renderer,30,32);
	LoadImage("./Images/Icon_Open.png",window,renderer,80,32);
	LoadImage("./Images/Icon_Save.png",window,renderer,130,32);
	LoadImage("./Images/Icon_Refresh.png",window,renderer,180,32);
	LoadImage("./Images/Icon_Help.png",window,renderer,1040,32);
	
	//Load Trays
	LoadImage("./Images/Tray_LineColor.png",window,renderer,1012,90);
	LoadImage("./Images/Tray_LineWidth.png",window,renderer,1012,270);
	LoadImage("./Images/Tray_Background.png",window,renderer,1012,440);
	LoadImage("./Images/Tray_Speed.png",window,renderer,1012,545);
	
	//Load other buttons
	LoadImage("./Images/Colors/1_4.jpg",window,renderer,1080,467);
	LoadImage("./Images/Colors/1_4.jpg",window,renderer,1080,493);

	char imgpath[25];
	LoadColors(window, renderer, 1025, 127); //color swatches on Line Color Tray
	
	int i = 0;
	for (i=1; i<=4; i++) //speed buttons
	{
		sprintf(imgpath, "./Images/Speed_%d.png", i);
		LoadImage(imgpath,window,renderer, 1024+22*(i-1), 575);
	}
	
	for (i=1; i<=7; i++) //line buttons
	{
		sprintf(imgpath, "./Images/Line_%d.png", i);
		LoadImage(imgpath, window, renderer, 1024, 302+15*(i-1));
	}

}	



//function for the Help button
void Help(SDL_Renderer *renderer, SDL_Rect board)
{
	//set font and color
	TTF_Font *font = TTF_OpenFont("LiberationMono-Regular.ttf", 15);
	SDL_Color color = { 240, 240, 240 };
	
	//Draw a gray board
	SDL_Texture * texture;
	SDL_Surface *surface = SDL_CreateRGBSurface(0, board.w, board.h, 32, 0, 0, 0, 0);
	SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 100, 100, 100, 10));
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderCopy(renderer, texture, NULL, &board);
	
	if (font == NULL) //check if the font is loaded
	{
		printf("Sorry there's no font");
	}
	else //print Help message to the screen
	{
		surface = TTF_RenderText_Solid (font, "Press A to move up, D to move down. Left and right arrow to move left and right. ", color);
		texture = SDL_CreateTextureFromSurface(renderer,surface);
		SDL_Rect text = {110, 315, 0, 0};
		SDL_QueryTexture(texture, NULL, NULL, &text.w, &text.h);
		SDL_RenderCopy(renderer, texture, NULL, &text);
		SDL_RenderPresent(renderer);
	}
	
	//Release sources
	TTF_CloseFont(font);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}


//Configuration settings
typedef struct config
{
   char filename[256];
   SDL_Point linepos;
   RGBcolour linecol;
   int psize;
   int speed;
   
} config;



//function for Save files, with help from Eike Anderson
int SaveFile(SDL_Surface *boardsurface, SDL_Surface *pixelsurface, SDL_Point linepos, RGBcolour linecol, int psize, int speed, SDL_Rect board)
{
	char filename[256];
	char filename2[256];
	config savefile;
	
	//ask for file name, there's no extension so we can add .esc and save related images later
	printf("File name: (no extension)\n");
	scanf("%s",filename);
	
	strcat(filename, ".esc"); //add .esc for configuration file
	
	//store file name, line position, color, point size, speed to savefile
	strcpy(savefile.filename,filename);
	savefile.linepos = linepos;
	savefile.linecol = linecol;
	savefile.psize = psize;
	savefile.speed = speed;
	//save an image for the board, and an image for pixels
    sprintf(filename2,"%s_brd.png",savefile.filename);
    IMG_SavePNG(boardsurface, filename2);
    sprintf(filename2,"%s_pix.png",savefile.filename);
    IMG_SavePNG(pixelsurface, filename2);
    
	FILE *f= fopen(filename,"wb");
	if(f!=NULL)
	{
		fwrite(&savefile,sizeof(savefile),1,f);	
		printf("Save successfully");
	}
	else
	{
		printf("File saving failed");
		return 0;
	}
	fclose(f);
	
}

//function for Open files (configuration), with help from Eike Anderson
int OpenFile(config *openfile)
{
	char filename[256];
	
	//ask for configuration file
	printf("Open file (*.esc): \n");
	scanf("%s",filename);
	
	//read the file to openfile
	FILE *f= fopen(filename,"rb");
	if(f!=NULL)
	{
		fread(openfile,sizeof(config),1,f);
		printf("Open successfully. \n");
	}
	else 
	{
		perror("File opening failed!\n");
		return 0;
	}
	fclose(f);
	return 1;
}



//function for saving images
void SaveImage ( SDL_Surface *boardsurface, SDL_Surface *pixelsurface, SDL_Rect board)
{
	
	SDL_Surface *save = SDL_CreateRGBSurface(0, board.w, board.h, 32, 0, 0, 0, 0);


	SDL_BlitScaled(boardsurface, NULL, save, NULL);
	SDL_BlitSurface(pixelsurface, &board, save, NULL);
	char* imgpath = (char*) malloc(sizeof(char*)*100);
	char type[10];
	int i = 0;
	printf("What type of image format? (bmp, png) ");
	scanf("%s", type);
	while (type[i])
	{
		type[i] = (tolower(type[i]));
		i++;
	}
	printf("\nfile name? (can add img path, no need to put image format name ");
	scanf("%s", imgpath);
	sprintf(imgpath, "%s.%s", imgpath, type);
	if (strcmp(type, "bmp")==0) 
	{
		SDL_SaveBMP(save, imgpath);
		printf("Save successfully");
	}
	else if(strcmp(type, "png")==0)
	{
		IMG_SavePNG(save, imgpath);
		printf("Save successfully\n");
	}
	else printf("Sorry, wrong format. please try to save again\n");
	free(imgpath);
	SDL_FreeSurface(save);
}





//MAIN FUNCTION HERE
int main(int argc, char** argv)
{
	//Initialize and set-up everything here
	
		//window and board areas
	SDL_Rect wrect = {0,0,1122,842};
	SDL_Rect board = {12, 90, 984, 722};
	
		//Initialize window, surfaces and textures.
		//The code creates a layer for UI, a layer for the board, and a layer for pixels
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
	printf("Error: %s \n",SDL_GetError());
	return 1;
	}
	
	SDL_Window *window = SDL_CreateWindow("Etch a Sketch", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, wrect.w, wrect.h, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
	printf("Error: %s \n",SDL_GetError());
	return 1;
	}
	
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
	printf("Error: %s \n",SDL_GetError());
	return 1;
	}
	
	IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG|IMG_INIT_TIF);
	TTF_Init();
	
	SDL_Surface *boardsurface = SDL_CreateRGBSurface(0, board.w, board.h, 32, 0, 0, 0, 0);
	SDL_Surface *tempsurface = SDL_CreateRGBSurface(0, board.w, board.h, 32, 0, 0, 0, 0);
	SDL_Surface *pixelsurface = IMG_Load("./Images/transparent.png"); //a transparent surface for pixels
	SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, wrect.w, wrect.h);
	SDL_Texture * boardtexture;
	SDL_Texture * pixeltexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, wrect.w, wrect.h);
	//Finish initialization, should all be freed later
	
	
	
	//set buttons size and coordinate and colors
	SDL_Rect Icon_Save = {130, 32, 39, 26};
	SDL_Rect Icon_Help = {1040, 32, 64, 25};
	SDL_Rect Icon_Refresh = {180, 32, 39, 26};
	SDL_Rect BgColor = {1080, 467, 13, 13};
	SDL_Rect BgImage = {1080, 493, 13, 13};
	SDL_Rect Icon_Ref = {180, 32, 39, 26};
	SDL_Rect Icon_New = {30, 32, 39, 26};
	SDL_Rect Icon_Open = {80,32, 39, 26};
	
	SDL_Point topleft = {board.x, board.y};

	RGBcolour black = {0,0,0};
	RGBcolour white = {240,240,240};
	
	//default settings
	config def = {"etch", topleft, black, 3, 0}; 
	
	RGBcolour linecol = def.linecol;
	RGBcolour bgcol = white;
	SDL_Point point = def.linepos;
	
	
	
	
	//Load user interface and render UI to texture
	SDL_SetRenderTarget(renderer, texture);
	LoadUI(window, renderer);
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	
		//Load the board
	SDL_FillRect(boardsurface, NULL, SDL_MapRGBA(boardsurface->format, 240, 240, 240, 255));
	boardtexture = SDL_CreateTextureFromSurface(renderer, boardsurface);
	SDL_RenderCopy(renderer, boardtexture, NULL, &board);
	
		//load pixel layer
	pixeltexture = SDL_CreateTextureFromSurface(renderer, pixelsurface);
	SDL_RenderCopy(renderer, pixeltexture, NULL, NULL);
	SDL_RenderPresent(renderer);

	
	
	//EVENT LOOP
	SDL_Event event;
	int quit=1;
	int i,x,y, psize=3, speed=0;
	int Colorx = 0, Colory = 0, c;
	char imgpath[256];
	int click = 0;
	
	
		//main event loop
	while (quit)
	{
	
	SDL_WaitEvent(&event);
	SDL_Rect prect = {point.x, point.y, psize, psize};
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	
	switch(event.type)
		{
		case SDL_QUIT:  //quit window
						printf("Window closed\n");
						quit=0; break; 
						
		case SDL_KEYUP:	
						printf("Key up\n");
						break;
						
		case SDL_KEYDOWN: 
						//reLoad UI and board
						boardtexture = SDL_CreateTextureFromSurface(renderer, boardsurface);
						SDL_RenderCopy(renderer, texture, NULL, NULL);
						SDL_RenderCopy(renderer, boardtexture, NULL, &board);
						
						//Draw pixel on pixel layer, with help from Yannis Ioannidis
						if (point.x>=board.x && point.x<=board.x + board.w && point.y>=board.y && point.y<=board.y + board.h) //if point position is within the board
						{
							if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D] || state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_RIGHT])
							//check if keys are pressed. Sometimes keys not pressed but the program still goes to KEYDOWN event (due to SDL_Delay)
							{
								if (state[SDL_SCANCODE_A]) point.y-=1;							
								if (state[SDL_SCANCODE_D]) point.y+=1;
								if (state[SDL_SCANCODE_LEFT]) point.x-=1;
								if (state[SDL_SCANCODE_RIGHT]) point.x+=1;
							}
							else //if state[..] are false, because we took fingers off the keyboard
							{
							SDL_RenderCopy(renderer, pixeltexture, NULL, NULL);
							SDL_RenderPresent(renderer);
							break; //break out of the SDL_KEYDOWN event
							}
													
						}
						else //point position outside the board, reset it back
						{
							if (point.x<board.x) point.x = board.x;
							if (point.x>board.x + board.w) point.x = board.x + board.w;
							if (point.y<board.y) point.y=board.y;
							if (point.y>board.y + board.h) point.y=board.y + board.h;
						}
						
							//Draw pixels to pixelsurface and show on screen
						SDL_FillRect(pixelsurface, &prect, SDL_MapRGBA(pixelsurface->format, linecol.r, linecol.g, linecol.b, 255));
						pixeltexture = SDL_CreateTextureFromSurface(renderer, pixelsurface);
						SDL_RenderCopy(renderer, pixeltexture, NULL, NULL);
						SDL_RenderPresent(renderer);
						
						//Delay speed
						if (speed)
						{ 
							SDL_Delay(speed*35);
							printf("Delay = %d\n", speed*35);
						}
						
						break; 
		
		
		case SDL_MOUSEBUTTONDOWN:
						x = event.motion.x;
						y = event.motion.y;
						printf("Mouse pressed: \n");
						printf("Position: %d, %d\n",x, y);
						
						//reload UI and board and pixels
						boardtexture = SDL_CreateTextureFromSurface(renderer, boardsurface);
						pixeltexture = SDL_CreateTextureFromSurface(renderer, pixelsurface);
						SDL_RenderCopy(renderer, texture, NULL, NULL);
						SDL_RenderCopy(renderer, boardtexture, NULL, &board);
						SDL_RenderCopy(renderer, pixeltexture, NULL, NULL);
												
						//Mouse click on New button, reset everything
						if (x>=Icon_New.x && x<=Icon_New.x + Icon_New.w && y>=Icon_New.y && y<=Icon_New.y + Icon_New.h)
						{
							printf("New\n");
							
							//Load new board
							bgcol = white;
							SDL_FillRect(boardsurface, NULL, SDL_MapRGBA(boardsurface->format, bgcol.r, bgcol.g, bgcol.b, 255));
							boardtexture = SDL_CreateTextureFromSurface(renderer, boardsurface);
							SDL_RenderCopy(renderer, boardtexture, NULL, &board);
							
							//Load new pixel layer
							SDL_FillRect(pixelsurface, NULL, SDL_MapRGBA(pixelsurface->format, 0, 0, 0, 0));
							pixeltexture = SDL_CreateTextureFromSurface(renderer, pixelsurface);
							SDL_RenderCopy(renderer, pixeltexture, NULL, &wrect);
							
							//reset coordinate
							point = def.linepos;
							//reset line color
							linecol = def.linecol;
							//reset line width
							psize = def.psize;
							//reset speed
							speed = def.speed;
						
							break;
						}
						
						//Mouse click on Open Icon
						if (x>=Icon_Open.x && x<=Icon_Open.x + Icon_Open.w && y>=Icon_Open.y && y<=Icon_Open.y + Icon_Open.h)
						{
							printf("Open\n");
							config Open;
							
							if(OpenFile(&Open)) //load config file
							{
								char fname[256];
								
								//copy settings from config file to current variables
								sprintf(fname,"%s_brd.png",Open.filename);
								boardsurface=IMG_Load(fname);
								sprintf(fname,"%s_pix.png",Open.filename);
								pixelsurface=IMG_Load(fname);
								//load related board and pixel images
								point.x = Open.linepos.x;
								point.y = Open.linepos.y;
								linecol = Open.linecol;
								psize = Open.psize;
								speed = Open.speed;
							
								printf("Point: %d - %d, Color: %d %d %d, Size: %d, speed %d\n", point.x, point.y, linecol.r, linecol.g, linecol.b, psize, 4-speed);
							}
							break;
						}
							
						
						//Mouse click on Save Icon, Load choices
						if (x>=Icon_Save.x && x<=Icon_Save.x + Icon_Save.w && y>=Icon_Save.y && y<=Icon_Save.y + Icon_Save.h)
						{
							printf("Save\n");
							
							//Drop down choices
							LoadImage("./Images/Button_SaveImage.png",window,renderer,140,55);
							LoadImage("./Images/Button_SaveFile.png",window,renderer,140,78);
							SDL_RenderPresent(renderer);
							SDL_Event event_saveimg;
							click = 1;
							
							//Wait for another click
							while(click)
							{
								SDL_WaitEvent(&event_saveimg);
								switch(event_saveimg.type)
								{
								case SDL_MOUSEBUTTONDOWN:
										click = 0;
										x = event_saveimg.motion.x;
										y = event_saveimg.motion.y;
										printf("Mouse pressed: \n");
										printf("Position: %d, %d\n",x, y);
										printf("click\n");
										SDL_Rect Button_SaveImg = {140, 55, 89, 23};
										SDL_Rect Button_SaveFile = {140, 78, 87, 23};
										if (x>=Button_SaveImg.x && x<=Button_SaveImg.x + Button_SaveImg.w && y>=Button_SaveImg.y && y<=Button_SaveImg.y + Button_SaveImg.h)
										{
											SaveImage(boardsurface, pixelsurface, board);
											break;
										}
										if (x>=Button_SaveFile.x && x<=Button_SaveFile.x + Button_SaveFile.w && y>=Button_SaveFile.y && y<=Button_SaveFile.y + Button_SaveFile.h)
										{
											SaveFile(boardsurface, pixelsurface, point, linecol, psize, speed, board);
											break;
										}
										
										break;
										
								case SDL_KEYDOWN: click = 0; break;
								}
							}
							break;
						}

						//if Mouse click on Help Icon, Load choices
						if (x>=Icon_Help.x && x<=Icon_Help.x + Icon_Help.w && y>=Icon_Help.y && y<=Icon_Help.y + Icon_Help.h)
						{
							printf("Help\n");
							Help(renderer, board);
							break;
						}
						
						//if Mouse click on Background color, load color swatches
						if (x>=BgColor.x && x<=BgColor.x + BgColor.w && y>=BgColor.y && y<=BgColor.y + BgColor.h)
						{
							LoadColors(window, renderer, BgColor.x-70, BgColor.y+10);
							SDL_RenderPresent(renderer);
							printf("background\n");
							click = 1;
							SDL_Event event_background;
							
							//Wait for another click
							while (click)
							{
								SDL_WaitEvent(&event_background);
								switch(event_background.type)
								{
								case SDL_MOUSEBUTTONDOWN:
									x = event_background.motion.x;
									y = event_background.motion.y;
									printf("Mouse pressed: \n");
									printf("Position: %d, %d\n",x, y);
									printf("click\n");
									
									//Mouse click on color swatch
									if (x>=1010 && x<=1010 + 105 && y>=477 && y<=477 + 105)
									{
										for (i=1, Colory=477; i<=7; i++, Colory+=15)
										{
										for (c=1, Colorx=1010; c<=5; c++, Colorx+=15)
										{
											if (x>=Colorx && x<=Colorx + 13 && y>=Colory && y<=Colory + 13)
											{
												bgcol = getPixelColour(renderer, wrect.w, wrect.h, x, y);
												printf("Color: %d_%d\n",i,c);
												SDL_FillRect(boardsurface, NULL, SDL_MapRGBA(boardsurface->format, bgcol.r, bgcol.g, bgcol.b, 255));
												boardtexture = SDL_CreateTextureFromSurface(renderer, boardsurface);
												SDL_RenderCopy(renderer, boardtexture, NULL, &board);
												SDL_RenderCopy(renderer, pixeltexture, NULL, NULL);
												SDL_RenderPresent(renderer);
												break;
											}
										}
										}
									}
									else click = 0;
																			
									break;
								case SDL_KEYDOWN: click = 0; break;
								}
							}
							//update everything again
							SDL_RenderCopy(renderer, texture, NULL, NULL);
							SDL_RenderCopy(renderer, boardtexture, NULL, &board);
							SDL_RenderCopy(renderer, pixeltexture, NULL, NULL);
							SDL_RenderPresent(renderer);
							
							
							break;
						}
							
						//if mouse click on background image
						if (x>=BgImage.x && x<=BgImage.x + BgImage.w && y>=BgImage.y && y<=BgImage.y + BgImage.h)
						{
							//ask for image path
							printf("Please enter image path: ");
							scanf("%s",imgpath);
							
							//load image to tempsurface
							tempsurface = IMG_Load(imgpath); //remember to free tempsurface
							if (tempsurface == NULL)
							{
								printf("Sorry, there's no such image:\n");
							}
							else
							{
								char scale[5] = {};
								printf("Your image is %s\n",imgpath);
								SDL_FillRect(boardsurface, NULL, SDL_MapRGBA(boardsurface->format, 240, 240, 240, 255));
								
								//ask if user want to scale image
								while (strcmp(scale,"yes")!=0 && strcmp(scale,"no")!=0)
								{
									printf("Do you want to scale it? (yes/no)\n");
									scanf("%s", scale);
								}
								if (strcmp(scale,"yes")==0)
								{
									SDL_BlitScaled(tempsurface, NULL, boardsurface, NULL);
								}
								else
								{
									SDL_BlitSurface(tempsurface, NULL, boardsurface, NULL);
								}
								
								//update everything
								boardtexture = SDL_CreateTextureFromSurface(renderer, boardsurface);
								SDL_RenderCopy(renderer, boardtexture, NULL, &board);
								SDL_RenderCopy(renderer, pixeltexture, NULL, NULL);
								printf("Load image successfully\n");
								
							}
							
							break;
						}
							
						//Mouse click on refresh button, reload board and create new pixel layer
						if (x>=Icon_Ref.x && x<=Icon_Ref.x + Icon_Ref.w && y>=Icon_Ref.y && y<=Icon_Ref.y + Icon_Ref.h)
						{
							printf("Refresh\n");
							SDL_RenderCopy(renderer, boardtexture, NULL, &board);
							SDL_FillRect(pixelsurface, NULL, SDL_MapRGBA(pixelsurface->format, 0, 0, 0, 0));
							pixeltexture = SDL_CreateTextureFromSurface(renderer, pixelsurface);
							SDL_RenderCopy(renderer, pixeltexture, NULL, &wrect);
							break;
						}
						
						//Mouse click to choose line width
						int linex = 1024, liney = 302;
						for (i=1; i<=7; i++, liney+=15)
						{
							if (x>=linex && x<=linex + 75 && y>=liney && y<=liney + 7)
								{psize = i; printf("width: %d\n",psize);
								break;}
							
						}
						
							//Mouse click on speed buttons
						int speedx = 1024, speedy = 575;
						for (i=3; i>=0; --i, speedx+=22)
						{
							if (x>=speedx && x<=speedx + 15 && y>=speedy && y<=speedy + 15)
								{speed = i; printf("Speed: %d\n",4-speed);
								break;}
						}
							
						//Mouse click to pick color from line color tray
						for (i=1, Colory=127; i<=7; i++, Colory+=15)
						{
							for (c=1, Colorx=1025; c<=5; c++, Colorx+=15)
							{
								if (x>=Colorx && x<=Colorx + 13 && y>=Colory && y<=Colory + 13)
									{
										linecol = getPixelColour(renderer, wrect.w, wrect.h, x, y);
										printf("Color: %d_%d\n",i,c);
										break;
									}
							}
						}
						
						
						break; //break out of MOUSEBUTTONDOWN event
			default: break;
		} //out of switch

	
	SDL_RenderPresent(renderer); //update
	
	}
	//end events
	
	

	//Free memory
	TTF_Quit();
	IMG_Quit();
	SDL_FreeSurface(boardsurface);
	SDL_FreeSurface(tempsurface);
	SDL_FreeSurface(pixelsurface);
	SDL_DestroyTexture(boardtexture);
	SDL_DestroyTexture(pixeltexture);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
	
	
}

