#include "battleships_types.h"

#ifndef BATTLESHIPS_H_INCLUDED
#define BATTLESHIPS_H_INCLUDED

/*!
\file battleships.h
\author Mikael REMY
\version 24.01
\date 7 january 2024
\brief Definition of functions
*/

/*!
\fn create_battleship : Function to create a boat which takes the necessary parameters and which will allocate a boat structure described previously
*/
    battleship *create_battleship(unsigned short int size, game_board *board);

/*!
\fn is_ship_alive : Function to check if a boat is still alive.
\param as parameter the address of a boat structure
\return Boolean
*/
    unsigned short int is_ship_alive(game_board *board, battleship *ship);

/*!
\fn create_game : Function to create a game (two boards, two tables of initialized boats), this function places the boats randomly and ensures that two boats do not touch each other
*/
    game_state *create_game();
    
/*!
\fn display_board : Function which displays a board with the state of the boats (player view) or (boolean parameter) displays a board with just the result of the shots (computer field view)
*/
    void display_board(unsigned short int computer);

/*!
\fn run_player_gameturn : Fonction qui sequence un tour de jeu joueur, demande case X / Y, effectue le tir et modifie en conséquence le plateau de jeu, affiche un message en fonction du résultat de l'attaque
*/
    void run_player_gameturn();
    
/*!
\fn run_computer_gameturn : Function which sequences a game turn of the computer which performs a random shot where it has not already fired (well normally!!)
*/
    void run_computer_gameturn();
    
/*!
\fn launch_game : Main function to sequence everything and display the winner
*/    
    void launch_game();


#endif // BATTLESHIPS_H_INCLUDED
