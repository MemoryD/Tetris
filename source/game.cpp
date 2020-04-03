#include "game.h"
#include <stack>
int set[7][4][4] = {{{ 0,1,4,5 },{ 0,1,4,5 },{ 0,1,4,5 },{ 0,1,4,5 }}, 
					{{ 0,4,5,9 },{ 1,2,4,5 },{ 0,4,5,9 },{ 1,2,4,5 }}, 
					{{ 1,4,5,8 },{ 0,1,5,6 },{ 1,4,5,8 },{ 0,1,5,6 }},
					{{ 0,1,5,9 },{ 2,4,5,6 },{ 0,4,8,9 },{ 0,1,2,4 }},
					{{ 0,1,4,8 },{ 0,1,2,6 },{ 1,5,8,9 },{ 0,4,5,6 }},
					{{ 1,4,5,6 },{ 0,4,5,8 },{ 0,1,2,5 },{ 1,4,5,9 }},
					{{ 0,1,2,3 },{ 1,5,9,13 },{ 0,1,2,3 },{ 1,5,9,13 }},
			 	   };
struct Set{
	int x;
	int y;
};
Set moveTo[3] = { {1,0}, {-1,0}, { 0,1}};

class Game{
	private:
		Set now[4][4];
		Set next[4][4];
		int now_i, now_x;
		int next_i, next_x;
		int map[20][14];
		bool end;
		bool pause;
		int mode;
		int score, highScore;
		int speed;
		int fpsTime;
		SDL_Surface *square[9];
		SDL_Surface *background;
		SDL_Surface *num;
		SDL_Surface *sco;
		SDL_Surface *hiSco;
		SDL_Surface *sped;
		SDL_Surface *pase;
		SDL_Rect numClip[10];
		Timer rightTime;
		Timer leftTime;
		Timer fps;
		FILE * fpScore;
	public:
		Game();
		~Game();
		void creat_next();
		void change();
		void show_box( Set **off );
		void handle_input( const SDL_Event &event );
		void move_down();
		bool move(int d);
		void fix_on();
		int check_bonus();
		bool check_end();
		void show_game();
		void show_num(int x, int y, int number);
		void show_pause();
		void show_other();
		bool show_end();
		bool is_end();
		void gameRun();
		void control_fps();
};

Game::Game()
{
	square[0] = load_image( "..\\pic\\1.bmp" );
	square[1] = load_image( "..\\pic\\2.bmp" );
	square[2] = load_image( "..\\pic\\3.bmp" );
	square[3] = load_image( "..\\pic\\4.bmp" );
	square[4] = load_image( "..\\pic\\5.bmp" );
	square[5] = load_image( "..\\pic\\grey.bmp" );
	square[6] = load_image( "..\\pic\\black.png" );
	square[7] = load_image( "..\\pic\\small.bmp" );
	square[8] = load_image( "..\\pic\\small_grey.bmp" );
	num = load_image( "..\\pic\\num.bmp", true );
	sco = load_image( "..\\pic\\score.png", true );
	hiSco = load_image( "..\\pic\\hi_score.png", true );
	sped = load_image( "..\\pic\\speed.png", true );
	pase = load_image( "..\\pic\\pause.bmp", true , 16711422);
	background = load_image( "..\\pic\\background.png" );
	
	for(int i = 0; i < 10; i++)
	{
		numClip[i].x = i * NUM_WIDTH;
		numClip[i].y = 0;
		numClip[i].w = NUM_WIDTH;
		numClip[i].h = NUM_HEIGHT;
	}
	
	for(int i = 0; i < GAME_ROW; i++)
	{
		for(int j = 0; j < GAME_COL; j++)
		{
			map[i][j] = -1;
		}
	}
	
	if((fpScore = fopen("..\\score\\score.map", "r")) == NULL)
	{
		fpScore = fopen("..\\score\\score.map", "w");
		fprintf(fpScore,"%d",0);
		fclose(fpScore);
	}
	fpScore = fopen("..\\score\\score.map", "r");
	fscanf(fpScore,"%d",&highScore);
	fclose(fpScore);
	
	end = false;
	pause = false;
	score = 0;
	speed = 2;
	fpsTime = speed;
	fps.start();
	
	creat_next();
	change();
}

Game::~Game()
{
	/*for(int i = 0; i < 5; i++)
		SDL_FreeSurface( square[i] );
//	SDL_FreeSurface( background );
	*/
}

void Game::creat_next()
{
	next_i = random(0, 3);
	next_x = random(0, 4);
	int x = random(0, 6);
	
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			next[i][j].x = ( set[x][i][j] % 4 ) + START_SET;
			next[i][j].y = set[x][i][j] / 4; 
		}
	}
}

void Game::change()
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			now[i][j] = next[i][j];
		}
	}
	now_i = next_i;
	now_x = next_x;
	creat_next();
}

void Game::show_game()
{
//	apply_surface( 0, 0, background, screen );
	SDL_Rect clip;
	clip.x = 420;
	clip.y = 0;
	clip.w = 200;
	clip.h = 600;
	SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0, 0, 0 ) );
	SDL_FillRect( screen, &clip, SDL_MapRGB( screen->format, 0xDD, 0xDD, 0xDD ) );
	
	for(int i = 0; i < GAME_ROW; i++)
	{
		for(int j = 0; j < GAME_COL; j++)
		{
			if( map[i][j] != -1 )
			{
				apply_surface( j * BOX_SIZE, i * BOX_SIZE, square[6], screen);	
			}
			else
			{
				apply_surface( j * BOX_SIZE, i * BOX_SIZE, square[5], screen);
			}
		}
	}
	
	for(int i = 0; i < 4; i++)
	{
		apply_surface( now[now_i][i].x * BOX_SIZE, now[now_i][i].y * BOX_SIZE, square[6], screen);
	}
	
	show_other();
	
	SDL_Flip( screen );
}



void Game::show_other()
{
	apply_surface( 450, 0, sco, screen ); 
	show_num( 430, 45, score);
	
	apply_surface(422, 110, hiSco, screen);
	show_num( 430, 150, highScore );
	
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			apply_surface( 480 + i * SMALL_SIZE, 220 + j * SMALL_SIZE, square[8], screen);
		}
	}
	for(int i = 0; i < 4; i++)
	{
		apply_surface( ( next[next_i][i].x + 18 )* SMALL_SIZE, ( next[next_i][i].y + 11) * SMALL_SIZE, square[7], screen);
	}
	apply_surface(450, 320, sped, screen);
	apply_surface(500, 370, num, screen, &numClip[speed]);
}	

void Game::show_num(int x, int y, int number)
{
	
	std::stack<int> st;
	
	while( number )
	{
		st.push( number % 10 );
		number /= 10;
	}
	int zero = 6 - st.size();
	while( zero-- )
	{
		apply_surface(x, y, num, screen, &numClip[0]);
		x += NUM_WIDTH;
	}
	
	while( !st.empty() )
	{
		int t = st.top();
		apply_surface(x, y, num, screen, &numClip[t]);
		x += NUM_WIDTH;
		st.pop();
	}
} 

void Game::show_pause()
{
	apply_surface( 430, 420, pase, screen);
	SDL_Flip(screen);
}

bool Game::show_end()
{
	
}

bool Game::move(int d)
{	
	if( pause ) return false;
	
	bool coll = false;
	
	for(int i = 0; i < 4; i++)
	{
		bool flag = false;
		for(int j = 0; j < 4; j++)
		{
			now[i][j].x += moveTo[d].x;
			now[i][j].y += moveTo[d].y;
			int x = now[i][j].x;
			int y = now[i][j].y;
			if( x < 0 || x >= GAME_COL || y < 0 || y >= GAME_ROW || map[y][x] != -1 )
			{
				flag = true;
				if( now_i == i && d == DOWN )
					coll = true;
			}
		}
		if( flag )
		{
			for(int j = 0; j < 4; j++)
			{
				now[i][j].x -= moveTo[d].x;
				now[i][j].y -= moveTo[d].y;
			}
		}
	}
	
	return coll;
}

void Game::move_down()
{	
	if( move( DOWN ) )
	{
		fpsTime = speed;
		fix_on();
		check_bonus();
		if( check_end() )
		{
			end = true;
			return;
		}
		change();
	}
	else
	{
		show_game();
	}
}

int Game::check_bonus()
{
	int s = 0;
	for(int i = 0; i < GAME_ROW; i++)
	{
		bool flag = true;
		for(int j = 0; j < GAME_COL; j++)
		{
			if( -1 == map[i][j] )
			{
				flag = false;
				break;
			}
		}
		if( flag )
		{
			s++;
			for(int p = i; p > 0; p--)
			{
				for( int j = 0; j < GAME_COL; j++)
				{
					map[p][j] = map[p-1][j];
				}
			}
		}
	}
	switch( s )
	{
		case 1: score += 10; break;
		case 2: score += 40; break;
		case 3: score += 90; break;
		case 4: score += 160; break;
	}
	if( score > highScore )
	{
		highScore = score;
		fpScore = fopen( "score.map", "w" );
		fprintf( fpScore,"%d", score);
		fclose( fpScore );
	}
}

void Game::fix_on()
{
	for(int i = 0; i < 4; i++)
	{
		int x = now[now_i][i].x;
		int y = now[now_i][i].y;
		map[y][x] = ( now_x + 1 ) % 5;
	}
}

bool Game::check_end()
{
	for(int i = 0; i < GAME_COL; i++)
	{
		if( map[0][i] != -1 )
			return true;
	}
	return false;
}

void Game::handle_input(const SDL_Event &event)
{
	if( event.type == SDL_KEYDOWN )
	{
		switch( event.key.keysym.sym )
		{
			case SDLK_DOWN:{
				if( !pause ) 
					fpsTime = FPS_FAST;
				break;
			}
			case SDLK_LEFT:{
				fpsTime = speed;
				leftTime.start();
				move(LEFT);
				break;
			}
			case SDLK_RIGHT:{
				fpsTime = speed;
				rightTime.start();
				move(RIGHT);
				break;
			}
			case SDLK_UP:{
				if( pause ) return;
				now_i = ( now_i + 1 ) % 4;
				now_x = ( now_x + 1 ) % 5;
				show_game();
				break;
			}
			case SDLK_SPACE:{
				pause = !pause;
				break;
			}
			case SDLK_KP_PLUS:{
				speed = ( speed + 1 ) % 10;
				if( speed == 0 ) speed++;
				fpsTime = speed;
				break;
			} 
		}
	}
	else if( event.type == SDL_KEYUP )
	{
		if( event.key.keysym.sym == SDLK_RIGHT )
		{
			rightTime.stop();
		}
		if( event.key.keysym.sym == SDLK_LEFT )
		{
			leftTime.stop();
		}
	}
	else if( event.type == SDL_QUIT )
	{
		end = true;
	}
}


bool Game::is_end()
{
	return end; 
}

void Game::gameRun()
{	
	if( !pause )
	{
		if( leftTime.get_ticks() > PRESS_TIME )
		{
			move( LEFT );
			leftTime.start();
		}
		if( rightTime.get_ticks() > PRESS_TIME )
		{
			move( RIGHT );
			rightTime.start();
		}
		if( fps.get_ticks() > 1000 / fpsTime )
		{
			move_down();
			fps.start();
		}
		show_game();
		show_other();
	}
	else
	{
		show_pause();
	}

}

int main(int argc, char *args[]) 
{
	srand(unsigned(time(0)));
	
	init();
	bool quit =false;
	SDL_Event event;
	
	while( !quit )
	{
		Game game;
		while( !game.is_end() )
		{
			while( SDL_PollEvent( &event ) )
			{
				game.handle_input( event );
				if( event.type == SDL_QUIT )
				{
					quit = true;
					break;
				}
			}
			game.gameRun();
		}
		/*if( !quit )
		{
			if( game.show_end() )
			{
				quit = true;
			}
		}*/
	}
	
	return 0;
}
