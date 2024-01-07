#ifndef BATTLESHIPS_TYPES_H_INCLUDED
#define BATTLESHIPS_TYPES_H_INCLUDED

/*!
\file battleships_types.h
\author Mikael REMY
\version 24.01
\date 7 january 2024
\brief Definition of enumerations and structures
*/

/*!
Creation of constants to define the game board, the number and size of boats and ship placement attemps
*/
#define SHIP_MIN_SIZE 2
#define SHIP_MAX_SIZE 5
#define NUMBER_OF_SHIPS 5
#define BOARD_SIZE 10
#define SHIP_PLACEMENT_ATTEMPTS BOARD_SIZE * BOARD_SIZE

/*!
\fn CASE_TYPE : enumeration to define the type of squares on the board
*/
typedef enum CASE_TYPE {WATER, WATER_SHOT, BOAT, WRECK} CASE_TYPE;

/*!
\fn ORIENTATION : enumeration to define the orientation of the boats
*/
typedef enum ORIENTATION {HORIZONTAL, VERTICAL} ORIENTATION;

/*!
\fn position : structure to define the position of a boat
*/
typedef struct position {
    unsigned short int upper_left_row;
    unsigned short int upper_left_column;
} position;

/*!
\fn battleship : structure to define placement of a boats
*/
typedef struct battleship {
    unsigned short int ship_size;
    position position;
    ORIENTATION orientation;
} battleship;

/*!
\fn game_board : Game board structure (one board per player), double pointer for the matrix, size of the matrix dimension, initialized empty
*/
typedef struct game_board {
    CASE_TYPE **matrix;
    unsigned short int matrix_size;
} game_board;

/*!
\fn game_state : Game state for all players
*/
typedef struct game_state {
    game_board *player_board;
    game_board *computer_board;
    battleship *player_ships[NUMBER_OF_SHIPS];
    battleship *computer_ships[NUMBER_OF_SHIPS];
} game_state;

#endif // BATTLESHIPS_TYPES_H_INCLUDED
