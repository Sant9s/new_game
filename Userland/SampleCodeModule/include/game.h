#ifndef _GAME_H
#define _GAME_H
#include <colors.h>

struct position{
    int x;
    int y;
};
typedef struct position Position;

typedef enum{
    map_barrier = 0,                // walls etc
    npc,                            // player is npc type
    enemy,
    healer,
    east_door,
    west_door,
    empty
} Blocktype;

struct mapinfo{
    Color color;
    Blocktype blocktype;
};
typedef struct mapinfo MapInfo;

typedef enum{
    STILL = 0,
    NORTH,
    EAST,
    SOUTH,
    WEST
} Direction;


// new game stuff
void start_new_game();
void new_game_title_card();

// colors a block
void change_square_color_by_square_position(int x_square, int y_square, Color color);

// sets everything to black
void set_map_positions_at_start();

void draw_bigger_square(int x_square, int y_square, Color color, Blocktype blocktype);

// player movement
void check_player_input(void);
void move_player();

// while that runs the game
void game_running();

// creates people
void create_character(int x, int y);
void create_npc(int x, int y);
void create_enemy(int x, int y);
void create_healer(int x, int y);
void draw_character();                      // does not overwrite player stats like create_character

// hp stuff
void draw_hp();
void damage_player();

// conversations
void draw_npc_conversation();
void draw_enemy_conversation();
void draw_wall_conversation();
void draw_healer_conversation();
void pick_conversation(Blocktype block);
void draw_square_for_dialog(Color color);

void game_over_screen();

// checks last block looked at
Blocktype check_block_type();

// lvls
void create_lvl_0_map();
void create_lvl_1_map();
void create_lvl_2_map();
void advance_west_door();
void advance_east_door();
void create_walls();                                // creates external walls with doors in each side

// reads map_positions and draws everything found there
void redraw_everything();



#endif /* _GAME_H */