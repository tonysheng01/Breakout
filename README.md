# Breakout
This is a remake of the classic arcade game _Breakout_ created with the OpenGL graphics library. While exciting new features are added, the fundamentals of the original game are preserved.

## Features
- Fully customizable levels
- Randomly spawining powerups
- Cool post-processings and particle effects

## Tutorial
The general rules are no different from those of the original game developed by Atari. The setup consists of layers of bricks as well as obstacles that cannot be destroyed. A ball travels across the screen, destroying the bricks it touches and bouncing off obstacles. The player controls a horizontally moving paddle to prevent the ball from hitting the bottom of the screen, in which case they lose a life. The goal is to eliminate all bricks without losing all lives.

> ⬅️ ➡️    Use the **A** and **D** keys to move the paddle across the screen

> ⬆️          Press **SPACE** to release the ball when it is stuck

Powerups are added to enhance your adventure. They are randomly spawned at the top of the screen and slowly fall down. The player can choose to activate a powerup by catching it with the paddle. Currently six types of powerups are included in the game:
* **Size increase:** Temporarily widens the paddle, making it easier to catch the ball.  
* **Speed increase:** Increases the speed of the ball. The effect will be cumulative throughout the game so choose wisely!   
* **Pass-through:** Temporarily lets the ball pass through all bricks it touches without bouncing back.
* **Sticky paddle:** Makes the ball stuck on the paddle the next time it is caught. Useful for adjusting the ball's launching position and angle.
* **Confuse:** Inverts paddle movement and places the scene upside-down. You might not want this.
* **Chaos:** Creates a chaotic effect where the scene is heavily distorted. Avoid at all costs!

