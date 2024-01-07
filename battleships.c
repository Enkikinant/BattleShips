#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "battleships.h"

/*!
\file battleships.c
\author Mikael REMY
\version 24.01
\date 7 january 2024
\brief Let's play battleship !!!
*/

static game_state *state;

static unsigned short int ship_position_compliant(battleship *ship, game_board *board)
{
    short row_min,row_max,column_min,column_max;

    if (ship->orientation == HORIZONTAL && ship->position.upper_left_column + ship->ship_size > BOARD_SIZE)
        return 0;

    if (ship->orientation == VERTICAL && ship->position.upper_left_row + ship->ship_size > BOARD_SIZE)
        return 0;

    row_min = ship->position.upper_left_row - 1;

    if (ship->orientation == HORIZONTAL)
        row_max = ship->position.upper_left_row + 1;
    else
        row_max = ship->position.upper_left_row + ship->ship_size;

    column_min = ship->position.upper_left_column - 1;

    if (ship->orientation == VERTICAL)
        column_max = ship->position.upper_left_column + 1;
    else
        column_max = ship->position.upper_left_column + ship->ship_size;

    if  (row_min < 0)
        row_min = 0;
    else
        row_min = row_min;

    if(row_max > BOARD_SIZE - 1)
        row_max = BOARD_SIZE - 1;
     else
        row_max = row_max;

    if(column_min < 0)
        column_min = 0;
    else
        column_min = column_min;

    if(column_max > BOARD_SIZE - 1)
        column_max = BOARD_SIZE - 1;
    else
        column_max = column_max;

    for(int i = row_min ; i <= row_max ; i++)
    {
        for(int j = column_min ; j <= column_max ; j++)
        {
            if(board->matrix[i][j] != WATER)
                return 0;
        }
    }
    return 1;
}


battleship *create_battleship(unsigned short int size, game_board *board)
{
    battleship *ship = malloc(sizeof(battleship));
    if (!ship)
    {
        printf("Unable to create Boat");
        exit(66); //execute order 66 !
    }

    ship->ship_size = size;
    for (int i = 0 ; i < SHIP_PLACEMENT_ATTEMPTS ; i++)
    {
        ship->position.upper_left_row = rand() % BOARD_SIZE;
        ship->position.upper_left_column = rand() % BOARD_SIZE;
        if (rand() % 2 == 0)
            ship->orientation = HORIZONTAL;
         else
            ship->orientation = VERTICAL;

        if (ship_position_compliant(ship, board))
            break;

        if (i + 1 == SHIP_PLACEMENT_ATTEMPTS)
        {
           printf("Unable to create Boat, try increasing/decreasing Board size, number of boats, min/max boat size and placement attempts ...\n");
            exit(66); //execute order 66 !
        }
    }


    for (int i = 0 ; i < ship->ship_size ; i++)
    {
        if(ship->orientation == HORIZONTAL)
            board->matrix[ship->position.upper_left_row][ship->position.upper_left_column + i] = BOAT;
        else if (ship->orientation == VERTICAL)
            board->matrix[ship->position.upper_left_row + i][ship->position.upper_left_column] = BOAT;
        else{
            printf("BUG !!! Unknown ship orientation !!!");
            exit(66); //execute order 66 !
        }
    }
    return ship;
}

unsigned short int is_ship_alive(game_board *board, battleship *ship)
{
    unsigned short int number_of_boat_part = 0;
    CASE_TYPE ship_case = board->matrix[ship->position.upper_left_row][ship->position.upper_left_column];
    if (ship_case == WATER || ship_case == WATER_SHOT)
    {
        printf("BUG !!! Given ship not found on the game board !\n");
        exit(66); //execute order 66 !
    }

    int pos = 0;
    if (ship->orientation == HORIZONTAL)
    {
        int pos = ship->position.upper_left_column;
    }
    else
    {
        int pos = ship->position.upper_left_row;
    }
    int pos_end = pos + ship->ship_size;

    while (pos < pos_end)
    {
        if (ship->orientation == HORIZONTAL)
            ship_case = board->matrix[ship->position.upper_left_row][pos];
        else
            ship_case = board->matrix[pos][ship->position.upper_left_column];

        if (ship_case == BOAT)
            number_of_boat_part++;
        pos++;
    }
    return number_of_boat_part;
}

game_state *create_game()
{
    printf("Creating Game State ...\n");
    game_state *state = malloc(sizeof(game_state));
    if (!state)
    {
        printf("Unable to allocate memory to create game !\n");
        exit(66); //execute order 66 !
    }

    printf("Creating Computer Game Board ...\n");
    game_board *computer_board = malloc(sizeof(game_board));
    CASE_TYPE **computer_matrix = malloc(BOARD_SIZE * sizeof(CASE_TYPE*));

    for (int i = 0 ; i < BOARD_SIZE ; i++)
    {
        computer_matrix[i] = malloc(BOARD_SIZE * sizeof(CASE_TYPE*));
    }

    if (!computer_board || !computer_matrix)
    {
        printf("Unable to allocate memory to create computer game board!\n");
        exit(66); //execute order 66 !
    }

    computer_board->matrix = computer_matrix;
    computer_board->matrix_size = BOARD_SIZE;

    printf("Creating Player Game Board ...\n");
    game_board *player_board = malloc(sizeof(game_board));
    CASE_TYPE **player_matrix = malloc(BOARD_SIZE * sizeof(CASE_TYPE*));
    for (int i = 0; i < BOARD_SIZE;i++)
    {
        player_matrix[i] = malloc(BOARD_SIZE * sizeof(CASE_TYPE*));
    }

    if (!player_board || !player_matrix)
    {
        printf("Unable to allocate memory to create player game board!\n");
        exit(66); //execute order 66 !
    }
    player_board->matrix = player_matrix;
    player_board->matrix_size = BOARD_SIZE;

    printf("Filling boards with WATER...\n");
    for (int x = 0 ; x < BOARD_SIZE ; x++)
    {
        for (int y = 0 ; y < BOARD_SIZE ; y++)
        {
            computer_matrix[x][y] = WATER;
            player_matrix[x][y] = WATER;
        }
    }
    state->computer_board = computer_board;
    state->player_board = player_board;

    printf("Creating ships ...\n");
    for (int i = 0 ; i < NUMBER_OF_SHIPS ; i++)
    {
        unsigned short int ship_size = SHIP_MIN_SIZE + rand() % (SHIP_MAX_SIZE - SHIP_MIN_SIZE + 1);
        state->player_ships[i] = create_battleship(ship_size, state->player_board);
        state->computer_ships[i] = create_battleship(ship_size, state->computer_board);
    }
    return state;
}

static void print_header()
{
    putchar(' ');
    for (int i = 0 ; i < BOARD_SIZE ; i++)
    {
        putchar('0' + i);
    }
    putchar('\n');
}

void display_board(unsigned short int computer)
{
    char water_char,water_shot_char,boat_char,wreck_char;
    game_board *board = NULL;
    water_char = '~';
    water_shot_char = 'X';
    wreck_char = 'O';

    if(computer)
    {
        printf("Computer Board :\n\n");
        board = state->computer_board;
        boat_char = water_char;
    }
    else
    {
        printf("Player Board :\n\n");
        board = state->player_board;
        boat_char = '#';
    }
    print_header();
    for(int row = 0 ; row < BOARD_SIZE ; row++)
    {
        putchar('0' + row);
        for(int col = 0 ; col < BOARD_SIZE ; col++)
        {
            switch (board->matrix[row][col])
            {
                case WATER:
                    putchar(water_char);
                    break;
                case WATER_SHOT:
                    putchar(water_shot_char);
                    break;
                case BOAT:
                    putchar(boat_char);
                    break;
                case WRECK:
                    putchar(wreck_char);
                    break;
            }
        }
        putchar('\n');
    }
    putchar('\n');
}

void run_player_gameturn()
{
    int row, column, ret;
    row = -1;
    column = -1;
    display_board(0);
    display_board(1);
    printf("Your turn, Human !\n");
    printf("Enter Shot coordinates : \n");

    do
    {
        printf("Row (0-%d) :\n", BOARD_SIZE - 1);
        ret = scanf("%d", &row);
        if (ret != 1)
            while(getchar() != '\n');
    } while(ret != 1 || row < 0 || row > BOARD_SIZE -1);

    do
    {
        printf("Column (0-%d) :\n", BOARD_SIZE - 1);
        ret = scanf("%d", &column);
        if (ret != 1)
            while(getchar() != '\n');
    } while(ret != 1 || column < 0 || column > BOARD_SIZE -1);

    switch(state->computer_board->matrix[row][column])
    {
        case BOAT:
            printf("\nYou hit an ennemy Boat !!!\n");
            state->computer_board->matrix[row][column] = WRECK;
            break;
        case WATER_SHOT:
        case WRECK:
            printf("\nYou already shot there previously, sorry !!! :)\n");
            break;
        case WATER:
            printf("\nYou just wasted ammo ... Water Shot !\n");
            state->computer_board->matrix[row][column] = WATER_SHOT;
            break;
        default:
            printf("\nBUG !!! No reason to get here :(\n");
            exit(66); //execute order 66 !
    }
    printf("ENTER to continue\n");
    while(getchar() != '\n');
}

void run_computer_gameturn()
{
    unsigned short int row, col;
    CASE_TYPE shot_case;
    printf("\nEnemy is firing !!!\n");
    do
    {
        row = rand() % BOARD_SIZE;
        col = rand() % BOARD_SIZE;
        shot_case = state->player_board->matrix[row][col] ;
    }while(shot_case == WATER_SHOT || shot_case == WRECK);

    switch(shot_case)
    {
        case BOAT:
            printf("Enemy just hit one of your vessel !!!\n");
            state->player_board->matrix[row][col] = WRECK;
            break;
        case WATER:
            printf("You feel salt drip of water as enemy ammo fall around you\n");
            state->player_board->matrix[row][col] = WATER_SHOT;
            break;
        default:
            printf("\nBUG !!! No reason to get here :(\n");
            exit(66); //execute order 66 !
    }
    printf("ENTER to continue\n");
    while(getchar() != '\n');
}

static char some_ships_are_still_alive(game_board *board, battleship *ships[]){
    for (int i = 0 ; i < NUMBER_OF_SHIPS ; i++)
    {
        if (is_ship_alive(board, ships[i]))
            return 1;
    }
    return 0;
}

static void cleanup_game()
{
    printf("Cleaning up resources ...");
    for (int i = 0 ; i < NUMBER_OF_SHIPS ; i++)
    {
        if (state->computer_ships[i])
            free(state->computer_ships[i]);
        if (state->player_ships[i])
            free(state->player_ships[i]);
    }
    for (int i = 0 ; i < BOARD_SIZE ; i++)
    {
        if (state->computer_board->matrix[i])
            free(state->computer_board->matrix[i]);
        if (state->player_board->matrix[i])
            free(state->player_board->matrix[i]);
    }
    if (state->computer_board->matrix)
        free(state->computer_board->matrix);
    if (state->player_board->matrix)
        free(state->player_board->matrix);
    free(state->computer_board);
    free(state->player_board);
    free(state);
}

void launch_game()
{
    unsigned short int player_ship_alive = 1;
    unsigned short int computer_ship_alive = 1;
    unsigned short int game_turn = 0;
    srand(time(NULL));
    state = create_game();

    printf("Battlefield is ready ...\n");
    //0 --> human 1 --> computer
    unsigned short int current_player = rand() % 2;
    if (current_player)
        printf("Computer will begin\n");
    else
        printf("You have the privilege to open the fire !\n");

    printf("ENTER to continue\n");
    while(getchar() != '\n');

    do
    {
        game_turn++;
        printf("\nTurn N° %d\n", game_turn / 2 + 1);
        if (current_player)
            run_computer_gameturn();
        else
            run_player_gameturn();

        player_ship_alive = some_ships_are_still_alive(state->player_board, state->player_ships);
        computer_ship_alive = some_ships_are_still_alive(state->computer_board, state->computer_ships);
        current_player = !current_player;

    }while(player_ship_alive && computer_ship_alive);

    printf("\nGame ended on turn %d\n", game_turn / 2 + 1);
    if (player_ship_alive)
        printf("You won, Human !\nCongratulations !!!\n");
    else if(computer_ship_alive)
        printf("Ennemy destroyed all your vessels ...You lose the Game !\n");
    else {
        printf("BUG!!! Should not get here !!!");
        exit(66); //execute order 66 !
    }

    cleanup_game();

    printf("See ya !\nENTER to exit\n");
    while(getchar() != '\n');
}
