#ifndef _GAME_H
#define _GAME_H
#include <colors.h>

struct position{
    int x;
    int y;
};

typedef enum{
    map_barrier = 0,
    npc,
    enemy,
    healer,
    east_door,
    west_door,
    empty
} Blocktype;

typedef struct position Position;

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





void start_new_game();
void new_game_title_card();
void make_game_square(int x, int y, int color);
void create_new_game_map();
void change_square_color_by_square_position(int x_square, int y_square, Color color);
void set_map_positions_at_start();
void create_character();
void draw_bigger_square(int x_square, int y_square, Color color);
void check_player_input(void);
void game_running();
void move_player();
void create_npc();
void draw_hp();
void create_enemy();
void damage_player();
void draw_npc_conversation();
void draw_enemy_conversation();
void create_healer();
void draw_wall_conversation();
void draw_healer_conversation();
void game_over_screen();
Blocktype check_block_type();
void pick_conversation(Blocktype block);
void redraw_everything();
void draw_square_for_dialog(Color color);
void create_walls();
void move_west_door();
void move_east_door();
void draw_door_conversation();




#endif /* _GAME_H */