# Developer Documentation

***This File belongs to and was entirely produced by Clément DAUBEUF***

## Main

All the game is launched from here

## Tick

The tick moduule ensure the game run at 60 tick per seconds and take control of the user input and render ins separated thread. It use a **mutex** to synchronise the two parts.

## Game

On each tick all the game routine is executed to manage all the game logic.

Fact : with the tick systems and the separation of render and user input catching, monsters are updated every 2 seconds and not at each usesr action.

## Render

On each tick, after the game, the render create a view of the game and send the data to the output stream (like print) but all is optimized because the system send only chars that change on the screen.

## Functions

Functions are called multiples times are stored in the same area.

## Generation

The system that manage the maze generation with the algorithm requested in the subject.

## Loader

The loader is the interface between files and game.
