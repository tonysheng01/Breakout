# Breakout
This is a remake of the classic arcade game _Breakout_ created with the OpenGL graphics library. While exciting new features are added, the fundamentals of the original game are preserved.

## Table of Contents
- [Features](#features)
- [In-game Screenshots](#in-game-screenshots)
- [Tutorial](#tutorial)
- [Dependencies](#dependencies)

## Features
- Fully customizable levels
- Randomly spawning powerups
- Cool post-processings and particle effects

## In-game Screenshots
![In game][in_game]
![Confuse effect][confuse_effect]
![Winning][winning]

## Tutorial
### Basic Rules
The general rules are no different from those of the original _Breakout_ developed by Atari. The setup consists of layers of bricks as well as obstacles that cannot be destroyed. A ball travels across the screen, destroying the bricks it touches and bouncing off obstacles. The player controls a horizontally moving paddle to prevent the ball from reaching the bottom of the screen, in which case they lose a life. The goal is to eliminate all bricks with at least one life remaining.

> ⬅️ ➡️    Use the **A** and **D** keys to move the paddle across the screen

> ⬆️          Press **SPACE** to release the ball when it is stuck

### Powerups
Powerups are added to enhance the adventure. They are randomly spawned at the top of the screen and slowly fall down. The player can choose to activate a powerup by catching it with the paddle. Currently six types of powerups are included in the game:
1. **Paddle size increase:** Temporarily widens the paddle, making it easier to catch the ball.  
2. **Ball speed increase:** Increases the speed of the ball. The effect will be cumulative throughout the game so choose wisely!   
3. **Pass-through:** Temporarily lets the ball pass through all bricks it touches without bouncing back.
4. **Sticky paddle:** Enables the "sticky paddle" feature for a short period of time, making the ball stuck on the paddle when being caught. Useful for adjusting the ball's launching position and angle.
5. **Confuse:** Temporarily places the scene upside-down and inverts all control. You might not want this.
6. **Chaos:** Creates a chaotic effect where the scene is heavily distorted, lasting for a few seconds. Avoid at all costs!
> Hint: powerups with reddish icons are generally associated with unwanted effects.

### Customizing the Levels
The game contains 4 built-in levels which can be easily modified based on the player's interest. Level data is stored in text files under `/levels`. Each level is represented by a matrix of numbers where the meaning of individual numbers differ:

> **0:** Empty block  
> **1:** Solid, undestroyable block  
> **2:** Blue destroyable brick  
> **3:** Green destroyable brick  
> **4:** Yellow destroyable brick  
> **5:** Orange destroyable brick

The level in the demo was created from the following array:

`5 5 5 5 5 5 5 5 5 5 5 5 5 5 5`  
`5 5 5 5 5 5 5 5 5 5 5 5 5 5 5`  
`4 4 4 4 4 0 0 0 0 0 4 4 4 4 4`  
`4 1 4 1 4 0 0 1 0 0 4 1 4 1 4`   
`3 3 3 3 3 0 0 0 0 0 3 3 3 3 3`  
`3 3 1 3 3 3 3 3 3 3 3 3 1 3 3`  
`2 2 2 2 2 2 2 2 2 2 2 2 2 2 2`  
`2 2 2 2 2 2 2 2 2 2 2 2 2 2 2`  

To prevent crashes, each row of the matrix must be fully filled (having identical size) and may only contain the above elements. The level files must not be renamed.

## Dependencies
A functioning OpenGL (3.3 or later) environment is required to run the game. In addition, make sure that you link to your project the following libraries and that all include paths are correct.
- GLFW (3.3 or later)
- GLAD (for OpenGL 3.3 or later)
- GLM (0.9.9.8 or later)
- stb_image (2.26 or later)
- FreeType (2.10.4 or later)
- irrKlang (1.6 or later)

[in_game]: /screenshots/in_game.png
[confuse_effect]: /screenshots/confuse_effect.png
[winning]: /screenshots/winning.png
