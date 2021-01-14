# Breakout
This is a remake of the classic arcade game _Breakout_ created with the OpenGL graphics library. While exciting new features are added, the fundamentals of the original game are preserved.

## Table of Contents
- [Features](#features)

## Features
- Fully customizable levels
- Randomly spawining powerups
- Cool post-processings and particle effects

## In-game Screenshots

`level selection screenshot`

`confuse screenshot`

`winning screenshot`

## Tutorial
### Rules
The general rules are no different from those of the original game developed by Atari. The setup consists of layers of bricks as well as obstacles that cannot be destroyed. A ball travels across the screen, destroying the bricks it touches and bouncing off obstacles. The player controls a horizontally moving paddle to prevent the ball from hitting the bottom of the screen, in which case they lose a life. The goal is to eliminate all bricks without losing all lives.

> ⬅️ ➡️    Use the **A** and **D** keys to move the paddle across the screen

> ⬆️          Press **SPACE** to release the ball when it is stuck

### Powerups
Powerups are added to enhance your adventure. They are randomly spawned at the top of the screen and slowly fall down. The player can choose to activate a powerup by catching it with the paddle. Currently six types of powerups are included in the game:
1. **Size increase:** Temporarily widens the paddle, making it easier to catch the ball.  
2. **Speed increase:** Increases the speed of the ball. The effect will be cumulative throughout the game so choose wisely!   
3. **Pass-through:** Temporarily lets the ball pass through all bricks it touches without bouncing back.
4. **Sticky paddle:** Makes the ball stuck on the paddle the next time it is caught. Useful for adjusting the ball's launching position and angle.
5. **Confuse:** Inverts paddle movement and places the scene upside-down. You might not want this.
6. **Chaos:** Creates a chaotic effect where the scene is heavily distorted. Avoid at all costs!

### Customizing the Levels
The game contains 4 built-in levels; however, they can be modified based on the player's interest. Level data is stored in text files under `/levels`. Each level is represented by a matrix of numbers where the meaning of individual numbers differ:

> **0:** empty block  
> **1:** solid, undestroyable block  
> **2:** blue destroyable brick  
> **3:** green destroyable brick  
> **4:** yellow destroyable brick  
> **5:** orange destroyable brick

To prevent crashes, each row of the matrix must be fully filled (having identical size) and may only contain the above elements.

## Dependencies
