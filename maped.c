/********************************************
*																						*
*	BALLMAN PRIMITIVE MAP EDITOR FOR DOS			*
*																						*
*				Programmed by												*
*																						*
*				Daniel Taylor												*
*																						*
********************************************/
#define ALLEGRO_STATICLINK


#include <stdio.h>
#include <allegro.h>
#include "data.h"

BITMAP *swap_screen;
BITMAP *cursor;
BITMAP *rect_cursor;
BITMAP *tile;
DATAFILE *data;
FILE *t_text;

#define ROWS 10
#define COLUMNS 16

//Globalzzz
int x_coord = 0;
int y_coord = 0;

int col_cur = 0;
int row_cur = 0;

int tiles[ROWS][COLUMNS];

void init()
{
	t_text = fopen("tiles.txt", "w");
	//Usual allegro stuff
	allegro_init();
	install_keyboard();
	
	//Mouse Stuff
	install_mouse();
	
	data = load_datafile("data.map");
	set_palette(data[PAL].dat);
	
	
	//GFX
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,640,400,0,0) != 0 )
	{
		printf("Couldn't set GFX MODE!");
		exit(1);
	}


	cursor = ((BITMAP *)data[CURSOR].dat);

	select_mouse_cursor(MOUSE_CURSOR_NONE);
	tile = ((BITMAP *)data[TILE1].dat);
	
	
}

void clear_tile_array()
{
	int r;
	int c;
	
	for (r = 0; r < ROWS; r++)
	{
		for (c = 0; c < COLUMNS; c++)
		{
			tiles[r][c] = 0;
		}
	}
}

void draw_tiles()
{
	int r;
	int c;
	
	
	for (r = 0; r < ROWS; r++)
	{
		for (c = 0; c < COLUMNS; c++)
		{
			if (tiles[r][c] == 1)
			{
				blit(tile, swap_screen, 0, 0, c * 16, r * 16, tile->w, tile->h); //REMEMBER COLUMNS ARE X, ROWS ARE Y!
			}
		}
	}
}

void print_tiles() //prints tiles to txt file
{
	int r;
	int c;
	
	
	for (r = 0; r < 10; r++)
	{
		for (c = 0; c < 16; c++)
		{
			if (c < 15)
			{
			fprintf(t_text, "%d,",tiles[r][c]);
		} else fprintf(t_text, "%d},\n{",tiles[r][c]); //just for visual things at the moment
		}
	}
}

int main(int argc, char *argv[])
{
	int done = 0;
	
	init();
	
	rect_cursor = create_bitmap(16,16);
	swap_screen = create_bitmap(320,200);
	clear_bitmap(rect_cursor);
	clear_bitmap(swap_screen);
	
	
	//rect(rect_cursor,0, 0, 15, 15, palette_color[1]);
	 //line(rect_cursor, 0, 0, 15, 15, palette_color[255]);
	 rect_cursor = ((BITMAP *)data[RECT].dat);
	
	
	
	tiles[9][15]=33;
	clear_tile_array();
	
	int mx;
	int my;
	
	while(!done)
	{
		vsync();
		poll_mouse();
		
		mx = mouse_x / (SCREEN_W / 320);
		my = mouse_y / (SCREEN_H / 200); // These two lines are so the mouse is in the real position of the OS mouse
		
		x_coord = mx / 16 * 16;
		y_coord = my / 16 * 16;
		
		row_cur = y_coord / 16;
		col_cur = x_coord / 16;
		
		draw_tiles();
		
		if ( (mouse_b & 1) && (tiles[row_cur][col_cur] == 0) && (row_cur < ROWS) && (col_cur < COLUMNS) )
		{
			tiles[row_cur][col_cur] = 1;
			printf("Tile Placed at: %d, %d\n", row_cur, col_cur);
		} 
		else if  ( (mouse_b & 2) && (tiles[row_cur][col_cur] > 0) && (row_cur < ROWS) && (col_cur < COLUMNS) ) 
		{
			tiles[row_cur][col_cur] = 0;
			printf("Tile DELETED at: %d, %d\n", row_cur, col_cur);
		}

		textprintf_ex(swap_screen, font, 0,164, palette_color[255],-1,"col(x): %d, row(y): %d", col_cur, row_cur);
		textprintf_ex(swap_screen, font, 0,176, palette_color[255],-1,"Real X: %d, Real Y: %d", x_coord, y_coord);
		textprintf_ex(swap_screen, font, 0,188, palette_color[255],-1,"Mouse X: %d, Mouse Y: %d", mx , my);
		
		
		
		if ( (row_cur < ROWS) && (col_cur < COLUMNS) )
		{
			masked_blit(rect_cursor,swap_screen,0 ,0 ,col_cur * 16, row_cur * 16,16,16); //Drawing the mouse square thing
		} 

			//show_mouse(swap_screen);
			masked_blit(cursor,swap_screen,0 ,0 ,mx - 7, my - 7,16,16); //draw mouse cursor manually because of transparency issue!

		
		vsync();
		stretch_blit(swap_screen,screen,0,0,swap_screen->w,swap_screen->h,0,0,SCREEN_W,SCREEN_H);
		
		clear_to_color(swap_screen,16);


		if (key[KEY_ESC]) done = 1;
	}
			
	

	//Really i'm just adding this comment... It's late at night.

	destroy_bitmap(swap_screen); 
	destroy_bitmap(cursor);
	destroy_bitmap(rect_cursor);
	destroy_bitmap(tile);
	
	set_gfx_mode(GFX_TEXT,0,0,640,480);
	print_tiles();
	fclose(t_text);
	return 0;
}
END_OF_MAIN()