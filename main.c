#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

const int width = 800;
const int height = 600;



int UpDo;
int MudaTex;

SDL_Event e;

SDL_Window* janela = NULL;

SDL_Renderer* Render = NULL;

SDL_Texture* Background;

SDL_Texture* ArrowD; SDL_Texture* ArrowE;

SDL_Texture* Player;SDL_Texture* Inimigo1;SDL_Texture* Inimigo2;
SDL_Texture* Inimigo3;SDL_Texture* Inimigo4;SDL_Texture* Item;

SDL_Rect aDir; SDL_Rect aEsq;

SDL_Rect posIn1; SDL_Rect posIn2;
SDL_Rect posIn3; SDL_Rect posIn4;
SDL_Rect posIn5;

SDL_Rect itemPos; SDL_Rect playerPos;

Mix_Chunk* somfundo1; Mix_Chunk* somfundo2; Mix_Chunk* somgrito; Mix_Chunk* somAndar;
Mix_Chunk* somMons1; Mix_Chunk* somMons2; Mix_Chunk* somMons3; Mix_Chunk* somMons4;
Mix_Chunk* somMorreu; Mix_Chunk* somAle1; Mix_Chunk* somAle2; Mix_Chunk* somAle3;

int inicializa()
{
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
     {
        printf( "SDL Nao foi inicializado! SDL_Error: %s\n", SDL_GetError() );
        return 1;
     }
     else
     {
		 if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		 {
			printf( "Texturas nao puderam ser lidas!" );
		 }

         janela= SDL_CreateWindow("Hospital X", 100, 100, width,height, SDL_WINDOW_SHOWN);
         if(janela == NULL)
         {
             printf( "Janela nao pode ser criada! SDL_Error: %s\n", SDL_GetError() );
             return 1;
         }
         else
         {
			Render = SDL_CreateRenderer( janela, -1, SDL_RENDERER_SOFTWARE );
			if( Render == NULL )
			{
				printf( "Render nao pode ser criado! SDL Error: %s\n", SDL_GetError() );
				return 1;
			}
			else
			{
				SDL_SetRenderDrawColor( Render, 0xFF, 0xFF, 0xFF, 0xFF );
                 int imgFlags = IMG_INIT_PNG;
                 if(!(IMG_Init(imgFlags) & imgFlags))
                 {
                     printf( "SDL_Image nao pode ser incializada! SDL_image Error: %s\n", IMG_GetError() );
                     return 1;
                 }
                 if( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                 {
                     printf( "SDL_mixer nao pode ser incializado! SDL_mixer Error: %s\n", Mix_GetError() );
                     return 1;
                 }
			}

         }
     }

     return 0;
}
void iniSons()
{
    somfundo1 = Mix_LoadWAV("sons/fundo1.mp3"); somfundo2 = Mix_LoadWAV("sons/fundo2.mp3");
    somgrito = Mix_LoadWAV("sons/grito.mp3"); somAndar = Mix_LoadWAV("sons/andar.mp3");
    somAle1 = Mix_LoadWAV("sons/aleotori1.mp3"); somAle2 = Mix_LoadWAV("sons/aleotori2.mp3");
    somAle3 = Mix_LoadWAV("sons/aleotori3.mp3"); somMons1 = Mix_LoadWAV("sons/Monstro1.mp3");
    somMons2 = Mix_LoadWAV("sons/Monstro2.mp3"); somMons3 = Mix_LoadWAV("sons/Monstro3.mp3");
    somMons4 = Mix_LoadWAV("sons/Monstro4.mp3"); somMorreu = Mix_LoadWAV("sons/morreu.mp3");

    Mix_VolumeChunk(somfundo1, 10); Mix_VolumeChunk(somfundo2, 15); Mix_VolumeChunk(somAle1, MIX_MAX_VOLUME);
    Mix_VolumeChunk(somAle2, MIX_MAX_VOLUME); Mix_VolumeChunk(somAle3, MIX_MAX_VOLUME);
    Mix_VolumeChunk(somgrito, 50); Mix_VolumeChunk(somAndar, MIX_MAX_VOLUME);
    Mix_VolumeChunk(somMorreu, MIX_MAX_VOLUME); Mix_VolumeChunk(somMons1, 5);
    Mix_VolumeChunk(somMons2, 6); Mix_VolumeChunk(somMons3, 7); Mix_VolumeChunk(somMons4, 8);

}

void iniVars()
{
    Background = NULL; Player = NULL; Inimigo1 = NULL; Inimigo2 = NULL; Inimigo3 = NULL; Inimigo4 = NULL; Item = NULL; ArrowD = NULL; ArrowE = NULL;
    UpDo = 0;    MudaTex = 0;
    posIn1.x = 110; posIn1.y = 10; posIn1.h = 50;  posIn1.w = 40;
    posIn2.x = 605; posIn2.y = 9; posIn2.h = 50;  posIn2.w = 40;
    posIn3.x = 360; posIn3.y = 8; posIn3.h = 50;  posIn3.w = 40;
    posIn4.x = 100; posIn4.y = 170; posIn4.h = 50;  posIn4.w = 40;
    posIn5.x = 150; posIn5.y = 380; posIn5.h = 50;  posIn5.w = 40;
//
    itemPos.x = 300; itemPos.y = 30;itemPos.w = 30; itemPos.h = 20;

    playerPos.x = 730; playerPos.y = 525; playerPos.h = 50; playerPos.w = 40;

}

Mix_Chunk* Musicplay(Mix_Chunk* music, int channel)
{
    if( Mix_Playing(channel) == 0 )
    {
         Mix_PlayChannel( channel, music, 0 );
    }
    else
    {
        Mix_HaltChannel(channel);
        Mix_PlayChannel( channel, music, 0 );
    }
}

void Musicpause(int channel)
{
    if(Mix_Paused(channel) == 0 ){
          Mix_Pause(channel);
    }
    else
    {
        Mix_Resume(channel);
    }
}

void MusicClose(int channel)
{
    if(channel < 0)
    {
        for(int i = 0; i < 12; i++)
        {
            Mix_HaltChannel(i);
        }
    }
    else
    {
        Mix_HaltChannel(channel);
    }
}

int jogoImagens()
{
    SDL_DestroyTexture( Background ); Background = NULL;
    if(Background == NULL)
    {
        Background = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/background.png" ));
    }
    if(Item == NULL)
    {
        Item = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/item.png" ));
    }
    if(Player == NULL)
    {
        Player = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/Player1.png" ));
    }

    if(MudaTex == 0)
    {
        Inimigo1 = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/monstro1.png" ));
        if(Inimigo1 == NULL)
        {
            printf( "Falha ao carregar Inimigo!\n" );
            return 1;
        }
        Inimigo2 = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/monstro2.png" ));
        if(Inimigo2 == NULL)
        {
            printf( "Falha ao carregar Inimigo 2!\n" );
            return 1;
        }
        Inimigo3 = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/monstro3.png" ));
        if(Inimigo3 == NULL)
        {
            printf( "Falha ao carregar Inimigo 3!\n" );
            return 1;
        }
        Inimigo4 = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/monstro4.png" ));
        if(Inimigo4 == NULL)
        {
            printf( "Falha ao carregar Inimigo 4!\n" );
            return 1;
        }
    }
    else
    {
        Inimigo1 = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/monstro1-1.png" ));
        if(Inimigo1 == NULL)
        {
            printf( "Falha ao carregar Inimigo!\n" );
            return 1;
        }
        Inimigo2 = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/monstro2-1.png" ));
        if(Inimigo2 == NULL)
        {
            printf( "Falha ao carregar Inimigo 2!\n" );
            return 1;
        }
        Inimigo3 = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/monstro3-1.png" ));
        if(Inimigo3 == NULL)
        {
            printf( "Falha ao carregar Inimigo 3!\n" );
            return 1;
        }
        Inimigo4 = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/monstro4-1.png" ));
        if(Inimigo4 == NULL)
        {
            printf( "Falha ao carregar Inimigo 4!\n" );
            return 1;
        }

    }

    return 0;
}

void itemBehaviour()
{
    int Velo = 40;
if(UpDo == 0)
    {
        if(itemPos.y < 350 )
        {
            itemPos.y += Velo;
        }
        else
        {
            itemPos.x += Velo;
        }
    }
    else
    {
        if(itemPos.y > 120 ){
            itemPos.y -= Velo;
        }
        else
        {
            itemPos.x -= Velo;
        }
    }
}



void enemyBehaviour()
{
    int Velo = 30;
    if(UpDo == 0)
    {
        if(posIn1.y < 400 )
        {
            posIn1.y += Velo;
            posIn2.y += Velo;
            posIn3.y += Velo;
            posIn4.x += Velo;
            posIn5.x += Velo;
        }
        else{ MudaTex = 1; UpDo = 1; jogoImagens(); }
    }
    else
    {
        if(posIn1.y > 60 ){
            posIn1.y -= Velo;
            posIn2.y -= Velo;
            posIn3.y -= Velo;
            posIn4.x -= Velo;
            posIn5.x -= Velo;
        }
        else{ MudaTex = 0; UpDo = 0; jogoImagens(); }
    }
}

void playerBehaviour()
{
    int Velo = 10;

    if (e.type == SDL_KEYDOWN)
    {
        if (e.key.keysym.sym == SDLK_UP )
        {
            if(playerPos.y > 10)
            {
               Player = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/Player1.png" ));
               playerPos.y -= Velo;
               if(Mix_Playing(8) == 0)
               {
                  Musicplay(somAndar, 8);
               }
            }
        } else if (e.key.keysym.sym == SDLK_DOWN) {
            if(playerPos.y < 540)
            {
                Player = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/Player1-2.png" ));
                playerPos.y += Velo;
               if(Mix_Playing(8) == 0)
               {
                  Musicplay(somAndar, 8);
               }
            }
        } else if (e.key.keysym.sym == SDLK_LEFT ) {
            if(playerPos.x > 10)
            {
               Player = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/Player1-3.png" ));
               playerPos.x -= Velo;
               if(Mix_Playing(8) == 0)
               {
                  Musicplay(somAndar, 8);
               }
            }
        } else if (e.key.keysym.sym == SDLK_RIGHT ) {
            if(playerPos.x < 750)
            {
                Player = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/Player1-1.png" ));
                playerPos.x += Velo;
                if(Mix_Playing(8) == 0)
               {
                  Musicplay(somAndar, 8);
               }
            }
        }
    }else if (e.type == SDL_KEYUP)
    {
        if(Mix_Playing(8) == 1)
        {
            Mix_HaltChannel(8);
        }
    }
}

int collision()
{
    if((playerPos.y + playerPos.h) >= posIn1.y && (playerPos.y - playerPos.h) <= posIn1.y)
    {
        if((playerPos.x + playerPos.w) >= posIn1.x && (playerPos.x - playerPos.w) <= posIn1.x)
        {
            return 1;
        }
    }
    if((playerPos.y + playerPos.h) >= posIn2.y && (playerPos.y - playerPos.h) <= posIn2.y)
    {
        if((playerPos.x + playerPos.w) >= posIn2.x && (playerPos.x - playerPos.w) <= posIn2.x)
        {
            return 1;
        }
    }
    if((playerPos.y + playerPos.h) >= posIn3.y && (playerPos.y - playerPos.h) <= posIn3.y)
    {
        if((playerPos.x + playerPos.w) >= posIn3.x && (playerPos.x - playerPos.w) <= posIn3.x)
        {
            return 1;
        }
    }
    if((playerPos.y + playerPos.h) >= posIn4.y && (playerPos.y - playerPos.h) <= posIn4.y)
    {
        if((playerPos.x + playerPos.w) >= posIn4.x && (playerPos.x - playerPos.w) <= posIn4.x)
        {
            return 1;
        }
    }
    if((playerPos.y + playerPos.h) >= posIn5.y && (playerPos.y - playerPos.h) <= posIn5.y)
    {
        if((playerPos.x + playerPos.w) >= posIn5.x && (playerPos.x - playerPos.w) <= posIn5.x)
        {
            return 1;
        }
    }
    if((playerPos.y + playerPos.h) >= itemPos.y && (playerPos.y - playerPos.h) <= itemPos.y)
    {
        if((playerPos.x + playerPos.w) >= itemPos.x && (playerPos.x - playerPos.w) <= itemPos.x)
        {
            return 2;
        }
    }
    return 0;
}

void menuIni()
{
   SDL_DestroyTexture( Background ); Background = NULL;
    if(Background == NULL)
    {
        Background = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/menu-01.png" ));
    }
    if(ArrowD == NULL)
    {
        ArrowD = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/setas-01.png" ));
    }
    if(ArrowE == NULL)
    {
        ArrowE = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/setas-02.png" ));
    }

    aDir.h = 75; aDir.w = 90; aDir.x= 260; aDir.y = 185;
    aEsq.h = 75; aEsq.w = 90; aEsq.x= 80; aEsq.y = 300;
}

int mouseClick()
{
    if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        if(e.button.button == SDL_BUTTON_LEFT)
        {
            if((aDir.y + aDir.h) >= e.button.y && aDir.y  <= e.button.y)
            {
                if((aDir.x + aDir.w) >= e.button.x && aDir.x <= e.button.x)
                {
                    return 1;
                }
            }
            if((aEsq.y + aEsq.h) >= e.button.y && aEsq.y  <= e.button.y)
            {
                if((aEsq.x + aEsq.w) >= e.button.x && aEsq.x <= e.button.x)
                {
                    return 2;
                }
            }
            SDL_Delay(100);
        }
    }
    return 0;
}

void menuCre()
{
    SDL_DestroyTexture( Background ); Background = NULL;
    if(Background == NULL)
    {
        Background = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/creditos-01.png" ));
    }
    aEsq.h = 75; aEsq.w = 90; aEsq.x= 80; aEsq.y = 500;
}

void mFinPerdeu()
{
    SDL_DestroyTexture( Background ); Background = NULL;
    if(Background == NULL)
    {
        Background = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/TelaPerdeu-01.png" ));
    }
    aDir.h = 75; aDir.w = 90; aDir.x= 690; aDir.y = 40;
}

void mFinGanhou()
{
    SDL_DestroyTexture( Background ); Background = NULL;
    if(Background == NULL)
    {
        Background = SDL_CreateTextureFromSurface(Render, IMG_Load( "images/TelaVenceu-01.png" ));
    }
}


void sair()
{
    Mix_FreeChunk(somAle1); Mix_FreeChunk(somAle2); Mix_FreeChunk(somAle3);
    Mix_FreeChunk(somAndar); Mix_FreeChunk(somfundo1); Mix_FreeChunk(somfundo2);
    Mix_FreeChunk(somgrito); Mix_FreeChunk(somMons1); Mix_FreeChunk(somMons2);
    Mix_FreeChunk(somMons3); Mix_FreeChunk(somMons4); Mix_FreeChunk(somMorreu);

    SDL_DestroyTexture( Inimigo1 );
	SDL_DestroyTexture( Inimigo2 );
    SDL_DestroyTexture( Inimigo3 );
	SDL_DestroyTexture( Inimigo4 );
    SDL_DestroyTexture( Player );
    SDL_DestroyTexture( Background );
    SDL_DestroyTexture( Item );
    SDL_DestroyTexture( ArrowD );
    SDL_DestroyTexture( ArrowE );

    Inimigo1 = NULL; Inimigo2 = NULL; Inimigo3 = NULL; Inimigo4 = NULL;
	Background = NULL; Item = NULL; Player = NULL; ArrowD = NULL; ArrowE = NULL;

	SDL_DestroyRenderer( Render );
    SDL_DestroyWindow(janela);
    janela = NULL;

    IMG_Quit();
    SDL_Quit();
}



int main( int argc, char* args[] )
{
	if( inicializa() != 0 )
	{
		printf( "Falha ao inicializar!\n" );
	}
	else
	{
        int quitGame = 0; int mInicial = 0; int mCreditos = 1; int jogar = 1; int finalPerdeu = 1; int finalGanhou = 1;
        iniVars();
        iniSons();
            while( quitGame == 0 )
			{
				if( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
                        quitGame == 1;
					}
				}
                    menuIni();
                    if(mInicial == 0)
                    {
                        MusicClose(-1);
                        Musicplay(somAle2, 0);

                        while(mInicial == 0)
                        {
                            if( SDL_PollEvent( &e ) != 0 )
                            {
                                if( e.type == SDL_QUIT )
                                {
                                    quitGame = 1; mInicial = 1; mCreditos = 1; jogar = 1; finalPerdeu = 1; finalGanhou = 1;
                                }
                            }
                            if( mouseClick() == 1)
                            {
                                mInicial = 1; mCreditos = 1; jogar = 0; finalPerdeu = 1; finalGanhou = 1;
                            }
                            else if( mouseClick() == 2)
                            {
                                mInicial = 1; mCreditos = 0; jogar = 1; finalPerdeu = 1; finalGanhou = 1;
                            }
                            SDL_RenderClear( Render );
                            SDL_RenderCopy( Render, Background,NULL, NULL );
                            SDL_RenderCopy( Render, ArrowD, NULL , &aDir );
                            SDL_RenderCopy( Render, ArrowE, NULL , &aEsq );
                            SDL_RenderPresent( Render );

                        }
                    }
                    else if(mCreditos == 0)
                    {
                        menuCre();
                        MusicClose(-1);
                        Musicplay(somAle1, 0);
                        while(mCreditos == 0)
                        {
                            if( SDL_PollEvent( &e ) != 0 )
                            {
                                if( e.type == SDL_QUIT )
                                {
                                    quitGame = 1; mInicial = 1; mCreditos = 1; jogar = 1; finalPerdeu = 1; finalGanhou = 1;
                                }
                            }
                            if( mouseClick() == 2)
                            {
                                mInicial = 0; mCreditos = 1; jogar = 1; finalPerdeu = 1; finalGanhou = 1;
                            }

                             SDL_RenderClear( Render );
                             SDL_RenderCopy( Render, Background,NULL, NULL );
                             SDL_RenderCopy( Render, ArrowE, NULL , &aEsq );
                             SDL_RenderPresent( Render );
                        }
                    }
                    else if( jogar == 0)
                    {
                        jogoImagens();
                        MusicClose(0);
                        while( jogar == 0)
                        {
                            if( SDL_PollEvent( &e ) != 0 )
                            {
                                if( e.type == SDL_QUIT )
                                {
                                    quitGame = 1; mInicial = 1; mCreditos = 1; jogar = 1; finalPerdeu = 1; finalGanhou = 1;
                                }
                            }
                            if(collision() == 1)
                            {
                                mInicial = 1; mCreditos = 1; jogar = 1; finalPerdeu = 0; finalGanhou = 1;
                            }
                            if(collision() == 2)
                            {
                                mInicial = 1; mCreditos = 1; jogar = 1; finalPerdeu = 1; finalGanhou = 0;
                            }
                            if(Mix_Playing(3) == 0)
                            {
                                for(int i = 0; i < 1; i++)
                                {
                                    Musicplay(somMons1, 3);
                                    SDL_Delay(10);
                                    Musicplay(somMons2, 4);
                                    SDL_Delay(10);
                                    Musicplay(somMons3, 5);
                                    SDL_Delay(10);
                                    Musicplay(somMons4, 6);
                                }
                            }
                            itemBehaviour();
                            enemyBehaviour();
                            playerBehaviour();
                            SDL_RenderClear( Render );
                            SDL_RenderCopy( Render, Background,NULL, NULL );
                            SDL_RenderCopy( Render, Inimigo1, NULL , &posIn1 );
                            SDL_RenderCopy( Render, Inimigo2, NULL , &posIn2 );
                            SDL_RenderCopy( Render, Inimigo1, NULL , &posIn3 );
                            SDL_RenderCopy( Render, Inimigo4, NULL , &posIn4 );
                            SDL_RenderCopy( Render, Inimigo3, NULL , &posIn5 );
                            SDL_RenderCopy( Render, Player, NULL , &playerPos );
                            SDL_RenderCopy( Render, Item, NULL , &itemPos );
                            SDL_RenderPresent( Render );
                            SDL_Delay(50);
                            if(Mix_Playing(2) == 0)
                            {
                                for(int i = 0; i < 1; i++)
                                {
                                    Musicplay(somfundo1, 1);
                                    SDL_Delay(10);
                                    Musicplay(somfundo2, 2);
                                    SDL_Delay(20);
                                }
                            }
                        }
                    }
                    else if(finalPerdeu == 0)
                    {
                        mFinPerdeu();
                        MusicClose(-1);
                        Musicplay(somMorreu, 0);
                        while(finalPerdeu == 0)
                        {
                            if( SDL_PollEvent( &e ) != 0 )
                            {
                                if( e.type == SDL_QUIT )
                                {
                                    quitGame = 1; mInicial = 1; mCreditos = 1; jogar = 1; finalPerdeu = 1; finalGanhou = 1;
                                }
                            }
                            SDL_RenderClear( Render );
                            SDL_RenderCopy( Render, Background,NULL, NULL );
                            SDL_RenderCopy( Render, ArrowD, NULL , &aDir );
                            SDL_RenderPresent( Render );
                            if( mouseClick() == 1)
                            {
                                mInicial = 0; mCreditos = 1; jogar = 1; finalPerdeu = 1; finalGanhou = 1;
                                iniVars();
                            }
                        }

                    }
                    else if(finalGanhou == 0)
                    {
                        mFinGanhou();
                        MusicClose(-1);
                        Musicplay(somAle3, 0);
                        while(finalGanhou == 0)
                        {
                            if( SDL_PollEvent( &e ) != 0 )
                            {
                                if( e.type == SDL_QUIT )
                                {
                                    quitGame = 1; mInicial = 1; mCreditos = 1; jogar = 1; finalPerdeu = 1; finalGanhou = 1;
                                }
                            }
                            SDL_RenderClear( Render );
                            SDL_RenderCopy( Render, Background,NULL, NULL );
                            SDL_RenderPresent( Render );
                        }
                    }
			}
	}
	sair();

	return 0;
}
