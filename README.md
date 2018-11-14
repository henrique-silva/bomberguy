# Bomberguy

## Installation

First you need to install the following libraries:

* libportaudio2
* libncurses5-dev
* make

If you use ubuntu, then you can install them with the following command:

	sudo apt-get install libportaudio2 libncurses-dev make

Then build the game from its root folder:

	cd bomberguy/
	make

The server binary will be at `bomberguy/build/bomberguy`. The clients will be at `bomberguy/build/client`

## Controls

* **W, A, S, D**:  Move player
* **Space bar**: Drop bomb
* **q**: Exit game

## Rules

Your objective in the multiplayer game is to destroy all the other players. There'll be enemies to make your life a little bit more complicated (:

You must not touch any enemies or get hit by an explosion.


### Bombs

The bombs dropped by the player have a explosion time of 2,5s. If a bomb is hit by the flames of a different bomb, it'll also explode instantly.

The bomb explosion range starts at 1 and can be increased with power ups.

**NOTE**: *Players can't walk over bombs after dropping them, so be careful to not get trapped by your own bomb.*

### Enemies

Enemies will spawn at random locations in the map and walk either horizontally or vertically. They are destroyed if hit by a bomb explosion.

If any player touches a enemy, it'll lose a life instantly and be teleported to the initial position (top-left corner)

### Power-ups

There are 3 different power-ups hidden under bricks:

* Bomb-up
	* Increase by one the number of bombs you can hold at the same time
* Flame-up
	* Increase by one the bombs explosion range (in all directions)
* Extra Life
	* Add one life

## Graphics

<center>
|Type | Symbol on the map|
|:------------:|:-------------:|
|Player |**<number>**|
|Brick | **#**|
|Bomb | **o**|
|Enemy | **@**|
|Bomb-up | **B**|
|Flame-up | **F**|
|Extra Life | **L**|
</center>
