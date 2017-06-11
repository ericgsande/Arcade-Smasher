/* Eric Sande, sande807 (at) umn (dot) edu
 * CSCI 5611: Animation and Planning in Games
 * Final Project
 * 
 * All sound effects were taken from ttps://www.sounds-resource.com.
 * 	-they said it is okay to use in a project like this
 */

#ifdef WINDOWS
#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#endif

#ifndef WINDOWS
#include <GL/glut.h>
#include <GL/gl.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <iostream> //for IO, testing
#include <vector> //for std::vector
#include <cmath> //for trig functions
#include <ctime> //for rand()
#include <string> //for printing lives/score to screen
#include <sstream> //for converting int to string, stringstream
#include "Vector3.h"

#define PI 3.14159
#define MAX_LEVEL 2
#define MAX_AMMO 12

typedef struct {
	Vector3 position;
	Vector3 velocity;
} BulletType;

//each object of type ParticleSystemType will be a complete particle system
const int pAmt = 25; //how many particles per system
typedef struct {
	Vector3 position[pAmt];
	Vector3 velocity[pAmt];
	bool isAlive[pAmt];
	float lifeSpan[pAmt];
} ParticleSystemType;

//each object of type LevelType will contain all necessary information for
//level, including block location, particle systems for each block, and 
//block alive/free status
typedef struct {
	std::vector <Vector3> blocks;
	std::vector <bool> isFree;
	std::vector <bool> isAlive;
	std::vector <ParticleSystemType> particles;
	std::vector <float> blockAngle;
	int numBlocks;
} LevelType;

Vector3 player; //player is a Vector3
std::vector <BulletType> playerBullets; //vector to hold all player bullets
std::vector <BulletType> enemyBullets; //vector to hold all enemy bullets
GLfloat playerAngle; //player gun rotation angle
std::vector <LevelType> levels; //vector to hold all enemy bullets
int currentLevel; //keep track of current level
int playerHits; //amout of time player has been hit
int enemyBulletsCount; //random variable that will spawn enemy bullet whenever it reaches certain value
const float blockWidth = 10.0; //width of all blocks in level
const float blockHeight = 4.0; //height of all blocks in level
int lives; //number of lives for player
int score; //keep track of score
int playerAmmo; //keep track of how much ammo the playe has
ParticleSystemType victory; //victory particle system
GLfloat victoryColor[pAmt][3]; //colors for victory
ParticleSystemType victoryPs[pAmt]; //particle system for victory particle system
ParticleSystemType playerPs; //particle system for exploding player
bool vStatus; //bool for status of victory

//sdl_mixer variables
Mix_Music *bkg = NULL;
Mix_Chunk *boom = NULL;
Mix_Chunk *fire = NULL;
Mix_Chunk *ammoless = NULL;
Mix_Chunk *defeat = NULL;
Mix_Chunk *victoryS = NULL;
Mix_Chunk *blowup = NULL;
Mix_Chunk *balloonpop = NULL;
Mix_Chunk *hit = NULL;
Mix_Chunk *enemyfire = NULL;
Mix_Chunk *levelup = NULL;
Mix_Chunk *miss = NULL; //will double as richocet
Mix_Chunk *congrats = NULL;
Mix_Chunk *balloonps = NULL;
Mix_Chunk *gameover = NULL;
Mix_Chunk *ammoup = NULL;

LevelType initLevel0() {
	//hardcode level initialization
	
	LevelType tempL;
	//init each block (via UL Vector3 location)
	Vector3 block0(-15.0, 40.0, 0.0);
	tempL.blocks.push_back(block0);
	Vector3 block1(-5.0, 40.0, 0.0);
	tempL.blocks.push_back(block1);
	Vector3 block2(5.0, 40.0, 0.0);
	tempL.blocks.push_back(block2);
	Vector3 block3(-10.0, 36.0, 0.0);
	tempL.blocks.push_back(block3);
	Vector3 block4(0.0, 36.0, 0.0);
	tempL.blocks.push_back(block4);
	Vector3 block5(-5.0, 32.0, 0.0);
	tempL.blocks.push_back(block5);
	//init num of blocks
	tempL.numBlocks = tempL.blocks.size();
	//init each block to isFree = false EXCEPT block5
	//init each block to isAlive = true
	for (int i = 0; i < tempL.numBlocks; i++) {
		bool tempF;
		bool tempA;
		if (i == 5)
			tempF = true;
		else
			tempF = false;
		tempA = true;
		tempL.isFree.push_back(tempF);
		tempL.isAlive.push_back(tempA);
	}
	//init particle system for each block
	//and init blockAngle to be 0.0
	for (int i = 0; i < tempL.numBlocks; i++) {
		ParticleSystemType ps[pAmt];
		for (int j = 0; j < pAmt; j++) {
			ps[i].position[j] = Vector3();
			ps[i].velocity[j] = Vector3((rand() % 100 / 25.0) - 2.0, -1 * (rand() % 100 / 50.0), 2.5);
			ps[i].isAlive[j] = false;
			ps[i].lifeSpan[j] = 255.0;
		}
		tempL.particles.push_back(ps[i]);
		tempL.blockAngle.push_back(0.0);
	}
	
	return tempL;
	
}

LevelType initLevel1() {
	//hardcode level initialization
	
	LevelType tempL;
	//init each block (via UL Vector3 location)
	Vector3 block0(-20.0, 40.0, 0.0);
	tempL.blocks.push_back(block0);
	Vector3 block1(-10.0, 40.0, 0.0);
	tempL.blocks.push_back(block1);
	Vector3 block2(0.0, 40.0, 0.0);
	tempL.blocks.push_back(block2);
	Vector3 block3(10.0, 40.0, 0.0);
	tempL.blocks.push_back(block3);
	Vector3 block4(-15.0, 36.0, 0.0);
	tempL.blocks.push_back(block4);
	Vector3 block5(-5.0, 36.0, 0.0);
	tempL.blocks.push_back(block5);
	Vector3 block6(5.0, 36.0, 0.0);
	tempL.blocks.push_back(block6);
	Vector3 block7(-10.0, 32.0, 0.0);
	tempL.blocks.push_back(block7);
	Vector3 block8(0.0, 32.0, 0.0);
	tempL.blocks.push_back(block8);
	Vector3 block9(-5.0, 28.0, 0.0);
	tempL.blocks.push_back(block9);
	//init num of blocks
	tempL.numBlocks = tempL.blocks.size();
	//init each block to isFree = false EXCEPT block5
	//init each block to isAlive = true
	for (int i = 0; i < tempL.numBlocks; i++) {
		bool tempF;
		bool tempA;
		if (i == 9)
			tempF = true;
		else
			tempF = false;
		tempA = true;
		tempL.isFree.push_back(tempF);
		tempL.isAlive.push_back(tempA);
	}
	//init particle system for each block
	//and init blockAngle to be 0.0
	for (int i = 0; i < tempL.numBlocks; i++) {
		ParticleSystemType ps[pAmt];
		for (int j = 0; j < pAmt; j++) {
			ps[i].position[j] = Vector3();
			ps[i].velocity[j] = Vector3((rand() % 100 / 25.0) - 2.0, -1 * (rand() % 100 / 50.0), 2.5);
			ps[i].isAlive[j] = false;
			ps[i].lifeSpan[j] = 255.0;
		}
		tempL.particles.push_back(ps[i]);
		tempL.blockAngle.push_back(0.0);
	}
	
	return tempL;
}

LevelType initLevel2() {
	
	LevelType tempL;
	//init blocks
	Vector3 block0(-25.0, 40.0, 0.0);
	tempL.blocks.push_back(block0);
	Vector3 block1(-15.0, 40.0, 0.0);
	tempL.blocks.push_back(block1);
	Vector3 block2(-5.0, 40.0, 0.0);
	tempL.blocks.push_back(block2);
	Vector3 block3(5.0, 40.0, 0.0);
	tempL.blocks.push_back(block3);
	Vector3 block4(15.0, 40.0, 0.0);
	tempL.blocks.push_back(block4);
	Vector3 block5(-10.0, 36.0, 0.0);
	tempL.blocks.push_back(block5);
	Vector3 block6(0.0, 36.0, 0.0);
	tempL.blocks.push_back(block6);
	Vector3 block7(-25.0, 32.0, 0.0);
	tempL.blocks.push_back(block7);
	Vector3 block8(-5.0, 32.0, 0.0);
	tempL.blocks.push_back(block8);
	Vector3 block9(15.0, 32.0, 0.0);
	tempL.blocks.push_back(block9);
	Vector3 block10(-25.0, 24.0, 0.0);
	tempL.blocks.push_back(block10);
	Vector3 block11(15.0, 24.0, 0.0);
	tempL.blocks.push_back(block11);
	Vector3 block12(-15.0, 20.0, 0.0);
	tempL.blocks.push_back(block12);
	Vector3 block13(5.0, 20.0, 0.0);
	tempL.blocks.push_back(block13);
	//init number of blocks
	tempL.numBlocks = tempL.blocks.size();
	//init isFree = false for each block except 0, 4, 7, 8, 9, 10,
	//11, 12, 13, init each block to be alive
	for (int i = 0; i < tempL.numBlocks; i++) {
		bool tempF, tempA;
		if (i == 8 || i == 10 || i == 11 || i == 12 || i == 13)
			tempF = true;
		else
			tempF = false;
		tempA = true;
		tempL.isAlive.push_back(tempA);
		tempL.isFree.push_back(tempF);
	}
	//init each block particle system and blockAngle = 0.0
	for (int i = 0; i < tempL.numBlocks; i++) {
		ParticleSystemType ps[pAmt];
		for (int j = 0; j < pAmt; j++) {
			ps[i].position[j] = Vector3();
			ps[i].velocity[j] = Vector3((rand() % 100 / 25.0) - 2.0, -1 * (rand() % 100 / 50.0), 2.5);
			ps[i].isAlive[j] = false;
			ps[i].lifeSpan[j] = 255.0;
		}
		tempL.particles.push_back(ps[i]);
		tempL.blockAngle.push_back(0.0);
	}
	
	return tempL;
	
}

void resetGame() {
	//reset everything
	
	player = Vector3(-4.0, -35.0, 0.0);
	playerAngle = 0.0;
	
	levels[0] = initLevel0();
	levels[1] = initLevel1();
	levels[2] = initLevel2();
	
	playerBullets.erase(playerBullets.begin(), playerBullets.begin() + playerBullets.size());
	enemyBullets.erase(enemyBullets.begin(), enemyBullets.begin() + enemyBullets.size());
	BulletType bB;
	bB.position = Vector3(-100.0, -100.0, -100.0);
	bB.velocity = Vector3(0.0, 0.0, 0.0);
	playerBullets.push_back(bB);
	
	currentLevel = 0;
	playerHits = 0;
	enemyBulletsCount = -100;
	lives = 3;
	score = 0;
	playerAmmo = 5;
	vStatus = false;
	
	int r;
	for (int j = 0; j < pAmt; j++) {
		victory.position[j] = Vector3(rand() % 50 - 25, -55.0, 0.0);
		victory.velocity[j] = Vector3((rand() % 100 / 25.0) - 2.0, 1 * rand() % 100 / 25.0, 0.1);
		victory.isAlive[j] = false;
		victory.lifeSpan[j] = 255.0;
		
		//init random color for cool balloon color effects
		r = rand() % 10;
		if (r < 2) {
			victoryColor[j][0] = 1.0;
			victoryColor[j][1] = 0.0;
			victoryColor[j][2] = 0.0;
		}
		else if (r < 4) {
			victoryColor[j][0] = 0.0;
			victoryColor[j][1] = 1.0;
			victoryColor[j][2] = 0.0;
		}
		else if (r < 6) {
			victoryColor[j][0] = 0.0;
			victoryColor[j][1] = 0.0;
			victoryColor[j][2] = 1.0;
		}
		else if (r < 8) {
			victoryColor[j][0] = 1.0;
			victoryColor[j][1] = 1.0;
			victoryColor[j][2] = 0.0;
		}
		else {
			victoryColor[j][0] = 0.0;
			victoryColor[j][1] = 1.0;
			victoryColor[j][2] = 1.0;
		}
		
		for (int i = 0; i < pAmt; i++) {
			victoryPs[i].position[j] = Vector3();
			victoryPs[i].velocity[j] = Vector3((rand() % 100 / 25.0) - 2.0, (rand() % 100 / 25.0) - 2.0, 2.5);
			victoryPs[i].isAlive[j] = false;
			victoryPs[i].lifeSpan[j] = 255.0;
		}	
	}	
	
	//init player particle system
	for (int i = 0; i < pAmt; i++) {
		playerPs.position[i] = Vector3();
		playerPs.velocity[i] = Vector3((rand() % 100 / 25.0) - 2.0, -1 * (rand() % 100 / 50.0), 2.5);
		playerPs.isAlive[i] = false;
		playerPs.lifeSpan[i] = 255.0;
	}
	
	Mix_PlayMusic(bkg, -1);
	
}

bool bulletInBlock(Vector3 block, Vector3 bullet) {
	//bad but simple function to see if bullet has intersected block
	
	if (bullet.getX() >= block.getX() &&
		bullet.getX() <= (block.getX() + blockWidth) &&
		bullet.getY() >= (block.getY() - blockHeight) &&
		bullet.getY() <= block.getY()) {
			return true;
	}
	return false;
	
}

void spawnBullet() {
	
	//only fire gun when player has ammo
	if (playerAmmo > 0) {
		playerAmmo--;
		
		//init bullet
		BulletType b;
		
		//set starting location to be tip of gun INCLUDING WITH ROTATION
		Vector3 pos(player.getX() + 4.0, player.getY() + 2.0, 0.0);
		b.position = pos;
		
		//set starting velocity to be in normal of gun angle
		Vector3 vel(0.0 * cos(playerAngle * PI / 180.0) - 4.0 * sin(playerAngle * PI / 180.0),
					0.0 * sin(playerAngle * PI / 180.0) + 4.0 * cos(playerAngle * PI / 180.0),
					0);
		b.velocity = vel;
		
		playerBullets.push_back(b);	
		
		//decrement score by 5 for each bullet shot
		score -= 5;
		
		//play fire sound
		Mix_PlayChannel(-1, fire, 0);
	}
	else {
		//punish player for trying to shoot when they have zero ammo
		score -= 25;
		
		//play ammoless sound
		Mix_PlayChannel(-1, ammoless, 0);
	}
	
}

void spawnEnemyBullet() {
	
	//init bullet
	BulletType b;
	
	if (lives > 0 && !vStatus) {
		//play enemy fire sound, but only once
		Mix_PlayChannel(-1, enemyfire, 0);
		//only spawn enemy bullet if player is alive
		for (int i = 0; i < levels[currentLevel].numBlocks; i++) {
			if (levels[currentLevel].isFree[i]) {
				//set starting location to be tip of enemy gun
				Vector3 pos(levels[currentLevel].blocks[i].getX() + blockWidth / 2.0,
							levels[currentLevel].blocks[i].getY() - blockHeight,
							0.0);
				b.position = pos;
				
				//set starting velocity to be in normal of block angle
				Vector3 vel(0.0 * cos(levels[currentLevel].blockAngle[i] * PI / 180.0) + (rand() % 4 + 3) * sin(levels[currentLevel].blockAngle[i] * PI / 180.0),
							-1 * (0.0 * sin(levels[currentLevel].blockAngle[i] * PI / 180.0) + (rand() % 4 + 3) * cos(levels[currentLevel].blockAngle[i] * PI / 180.0)),
							0);
				b.velocity = vel;
				
				enemyBullets.push_back(b);
			}
		}
	}
	
}

void updateDeps() {
	//harcode cause vector of vectors is stupid
	
	bool free;
	for (int i = 0; i < levels[currentLevel].numBlocks; i++) {
		if (!levels[currentLevel].isAlive[i] ||
			levels[currentLevel].isFree[i]) {
				continue;
		}
		if (currentLevel == 0) {
			if (!levels[currentLevel].isAlive[3] && i == 0) {
				levels[currentLevel].isFree[i] = true;
			}
			else if (!levels[currentLevel].isAlive[3] &&
					 !levels[currentLevel].isAlive[4] && i == 1) {
					levels[currentLevel].isFree[i] = true;
			}
			else if (!levels[currentLevel].isAlive[4] && i == 2) {
				levels[currentLevel].isFree[i] = true;
			}
			else if (!levels[currentLevel].isAlive[5] && 
					(i == 3 || i == 4)) {
						levels[currentLevel].isFree[i] = true;
			}
		}
		else if (currentLevel == 1) {
			if (!levels[currentLevel].isAlive[4] && i == 0) {
				levels[currentLevel].isFree[i] = true;
			}
			else if (!levels[currentLevel].isAlive[4] &&
					 !levels[currentLevel].isAlive[5] && i == 1) {
					levels[currentLevel].isFree[i] = true;
			}
			else if (!levels[currentLevel].isAlive[5] &&
					 !levels[currentLevel].isAlive[6] && i == 2) {
					levels[currentLevel].isFree[i] = true;
			}
			else if (!levels[currentLevel].isAlive[6] && i == 3) {
				levels[currentLevel].isFree[i] = true;
			}
			else if (!levels[currentLevel].isAlive[7] && i == 4) {
				levels[currentLevel].isFree[i] = true;
			}
			else if (!levels[currentLevel].isAlive[7] &&
					 !levels[currentLevel].isAlive[8] && i == 5) {
					levels[currentLevel].isFree[i] = true;
			}
			else if (!levels[currentLevel].isAlive[8] && i == 6) {
				levels[currentLevel].isFree[i] = true;
			}
			else if (!levels[currentLevel].isAlive[9] && 
					(i == 7 || i == 8)) {
				levels[currentLevel].isFree[i] = true;
			}
		}
		else if (currentLevel == 2) {
			if (!levels[currentLevel].isAlive[7] && i == 0) {
				levels[currentLevel].isFree[i] = true;
			}
			else if (!levels[currentLevel].isAlive[5] && i == 1) {
				levels[currentLevel].isFree[i] = true;
			}
			else if (!levels[currentLevel].isAlive[5] &&
					 !levels[currentLevel].isAlive[6] && i == 2) {
					levels[currentLevel].isFree[i] = true;
			}
			else if (!levels[currentLevel].isAlive[6] && i == 3) {
				levels[currentLevel].isFree[i] = true;
			}
			else if (!levels[currentLevel].isAlive[9] && i == 4) {
				levels[currentLevel].isFree[i] = true;
			}
			else if (!levels[currentLevel].isAlive[8] &&
					(i == 5 || i == 6)) {
						levels[currentLevel].isFree[i] = true;
			}
			else if (!levels[currentLevel].isAlive[10] && i == 7) {
				levels[currentLevel].isFree[i] = true;
			}
			else if (!levels[currentLevel].isAlive[11] && i == 9) {
				levels[currentLevel].isFree[i] = true;
			}
		}		
	}
		
}	

void checkLevel() {
	//will see if each block is dead, and if so, change level
	//also delete all current bullets if level changes
	
	bool nalive;
	for (int i = 0; i < levels[currentLevel].numBlocks; i++) {
		if (levels[currentLevel].isAlive[i]) {
			nalive = false;
			break;
		}
		nalive = true;
	}
	if (nalive) {
		if (currentLevel == MAX_LEVEL) {
			vStatus = true;
			score += 2000;
			//victory condition
			enemyBullets.erase(enemyBullets.begin(), enemyBullets.begin() + enemyBullets.size());
			//do something else
			for (int i = 0; i < pAmt; i++) {
				victory.isAlive[i] = true;
			}
			
			//stop bkg music and play victory and congrats
			Mix_FreeMusic(bkg);
			Mix_PlayChannel(-1, victoryS, 0);
			Mix_PlayChannel(-1, congrats, 0);
		}
		else {
			//reset health upon new level
			playerBullets.erase(playerBullets.begin(), playerBullets.begin() + playerBullets.size());
			enemyBullets.erase(enemyBullets.begin(), enemyBullets.begin() + enemyBullets.size());
			playerHits = 0;
			currentLevel++;
			score += 1000;
			enemyBulletsCount = -100;
			if (playerAmmo < MAX_AMMO - 5)
				playerAmmo += 5;
			else
				playerAmmo = MAX_AMMO;
			//play levelup sound
			Mix_PlayChannel(-1, levelup, 0);
		}
	}
	
}

void resetLevel() {
	//will reset the current level to its original state
	//blockAngle, isAlive, and isFree
	//also delete all current playerBullets
	//reset ammo to 5
	//reset player position to center
	
	for (int i = 0; i < levels[currentLevel].numBlocks; i++) {
		levels[currentLevel].isAlive[i] = true;
		levels[currentLevel].blockAngle[i] = 0.0;
		if (currentLevel == 0) {
			if (i == 5)
				levels[currentLevel].isFree[i] = true;
			else
				levels[currentLevel].isFree[i] = false;
		}
		else if (currentLevel == 1) {
			if (i == 9)
				levels[currentLevel].isFree[i] = true;
			else
				levels[currentLevel].isFree[i] = false;
		}
		else if (currentLevel == 2) {
			if (i == 8 || i == 10 || i == 11 || i == 12 || i == 13)
				levels[currentLevel].isFree[i] = true;
			else
				levels[currentLevel].isFree[i] = false;
		}
	}
	
	playerBullets.erase(playerBullets.begin(), playerBullets.begin() + playerBullets.size());
	playerAmmo = 5;
	enemyBulletsCount = -100;
	
	player.setX(-4.0);
	player.setY(-35.0);
	player.setZ(0.0);
	
}	

void updatePlayerBullets() {
	//function to update the bullets shot out by the player and to
	//produce appropriate interactions with the current level
	
	for (int i = 0; i < playerBullets.size(); i++) {
		//check if bullet has reached upper-bound of playing field or
		//sides of playing field -> a bit of CCD for increased accuracy
		if (playerBullets[i].position.getY() > 39.8 ||
			playerBullets[i].position.getY() + playerBullets[i].velocity.getY() * 0.1 > 39.8) {
			//play miss sound
			Mix_PlayChannel(-1, miss, 0);
			playerBullets.erase(playerBullets.begin() + i);
			continue;
		}
		if (playerBullets[i].position.getX() > 24.8 ||
			playerBullets[i].position.getX() + playerBullets[i].velocity.getX() * 0.1 > 24.8) {
			//play miss/ricochet sound
			Mix_PlayChannel(-1, miss, 0);
			playerBullets[i].position.setX(24.8);
			playerBullets[i].velocity.setX(playerBullets[i].velocity.getX() * -0.90);
			continue;
		}
		if (playerBullets[i].position.getX() < -24.8 ||
			playerBullets[i].position.getX() + playerBullets[i].velocity.getX() * 0.1 < -24.8) {
			if (i != 0) //consider *extra bullet
				Mix_PlayChannel(-1, miss, 0); //play miss/ricochet sound
			playerBullets[i].position.setX(-24.8);
			playerBullets[i].velocity.setX(playerBullets[i].velocity.getX() * -0.90);
			continue;
		}
		
		//next, check if bullet has interacted with block in level
		//added minor "parsing" to only check when block is close
		if (playerBullets[i].position.getY() >= 15.0) {
			for (int j = 0; j < levels[currentLevel].numBlocks; j++) {
				//check each block in level
				if (!levels[currentLevel].isAlive[j]) {
					//no interaction if block is dead
					continue;
				}
				if (bulletInBlock(levels[currentLevel].blocks[j], playerBullets[i].position)) {
					//using helper func, check if player bullet is inside current block
					//if it is, spawn appropriate particle system and increase score
					//give player more ammo
					//play boom sound
					Mix_PlayChannel(-1, boom, 0);
					for (int k = 0; k < pAmt; k++) {
						levels[currentLevel].particles[j].position[k].setX(playerBullets[i].position.getX());
						levels[currentLevel].particles[j].position[k].setY(playerBullets[i].position.getY());
						levels[currentLevel].particles[j].position[k].setZ(playerBullets[i].position.getZ());
						levels[currentLevel].particles[j].isAlive[k] = true;
						levels[currentLevel].particles[j].lifeSpan[k] = 255.0;
					}
					score += 100;
					if (playerAmmo < MAX_AMMO - 2)
						playerAmmo += 2;
					else
						playerAmmo = MAX_AMMO;
						
					//remove player bullet from existance and block from level
					//also make sure blocks isFree flag is turned off as well
					//then update dependencies/isFree
					playerBullets.erase(playerBullets.begin() + i);
					levels[currentLevel].isAlive[j] = false;
					levels[currentLevel].isFree[j] = false;
					
					//update isFree
					updateDeps();
										
					//check if all blocks are dead, and update level accordingly
					checkLevel();
					return;
				}
			}
		}
		
		//eulerian integration
		playerBullets[i].velocity.setX(playerBullets[i].velocity.getX());
		playerBullets[i].position.setX(playerBullets[i].position.getX() + playerBullets[i].velocity.getX() * 0.1);
		playerBullets[i].velocity.setY(playerBullets[i].velocity.getY());
		playerBullets[i].position.setY(playerBullets[i].position.getY() + playerBullets[i].velocity.getY() * 0.1);
		playerBullets[i].velocity.setZ(playerBullets[i].velocity.getZ());
		playerBullets[i].position.setZ(playerBullets[i].position.getZ() + playerBullets[i].velocity.getZ() * 0.1);
	}

}

void updateEnemyBullets() {
	//function to update enemy bullets and give them appropriate level
	//interactions
	
	for (int i = 0; i < enemyBullets.size(); i++) {
		//check if bullet has reached bottom of playing field and delete
		//or if bullet has collided with sides of playing field
		//minor CCD
		//add small chance to grant player extra ammo when they dodge enemy bullet
		if (enemyBullets[i].position.getY() < -39.8 ||
			enemyBullets[i].position.getY() + enemyBullets[i].velocity.getY() * 0.1 < -39.8) {
			//play miss/ricochet sound
			Mix_PlayChannel(-1, miss, 0);
			if (rand() % 101 < 20) {
				if (playerAmmo < MAX_AMMO) {
					//play ammo up sound
					playerAmmo++;
					Mix_PlayChannel(-1, ammoup, 0);
				}
			}
			enemyBullets.erase(enemyBullets.begin() + i);
			continue;
		}
		if (enemyBullets[i].position.getX() > 24.8 ||
			enemyBullets[i].position.getX() + enemyBullets[i].velocity.getX() * 0.1 > 24.8) {
			//play miss/ricochet sound
			Mix_PlayChannel(-1, miss, 0);
			enemyBullets[i].position.setX(24.8);
			enemyBullets[i].velocity.setX(enemyBullets[i].velocity.getX() * -0.90);
			continue;
		}
		if (enemyBullets[i].position.getX() < -24.8 ||
			enemyBullets[i].position.getX() + enemyBullets[i].velocity.getX() * 0.1 < -24.8) {
			//play miss/ricochet sound
			Mix_PlayChannel(-1, miss, 0);
			enemyBullets[i].position.setX(-24.8);
			enemyBullets[i].velocity.setX(enemyBullets[i].velocity.getX() * -0.90);
			continue;
		}
		
		//check if bullet has hit player
		if (enemyBullets[i].position.getY() <= -30.0) {
			//only check for bullet collision when it is close
			if (enemyBullets[i].position.getY() >= player.getY() &&
				enemyBullets[i].position.getY() <= player.getY() + 2.0 &&
				enemyBullets[i].position.getX() >= player.getX() &&
				enemyBullets[i].position.getX() <= player.getX() + 8.0) {
					//if bullet has hit player, remove bullet from vector
					//and injure player and update score
					//also check for levels/lives
					enemyBullets.erase(enemyBullets.begin() + i);
					playerHits++;
					score -= 100;
					if (playerHits >= 4) {
						//spawn player ps and reset enemybullets, player bullets
						//player position
						//decrement score
						//play blow up sound
						
						for (int i = 0; i < pAmt; i++) {
							playerPs.isAlive[i] = true;
							playerPs.lifeSpan[i] = 255.0;
							playerPs.position[i].setX(player.getX() + 4.0);
							playerPs.position[i].setY(player.getY() + 2.0);
							playerPs.position[i].setZ(player.getZ());
						}
						enemyBullets.erase(enemyBullets.begin(),enemyBullets.begin() + enemyBullets.size());
						playerBullets.erase(playerBullets.begin(), playerBullets.begin() + playerBullets.size());
						lives--;
						score -= 250;
						playerHits = 0;
						if (lives <= 0) {
							playerHits = 4;
							lives = 0;
							player.setY(-100.0);
							
							//turn off bkg music and play defeat
							Mix_FreeMusic(bkg);
							Mix_PlayChannel(-1, defeat, 0);
							Mix_PlayChannel(-1, gameover, 0);
						}
						else {
							//only reset current level when life is lost but
							//game is not over, play blowup sound
							Mix_PlayChannel(-1, blowup, 0);
							resetLevel();
						}
					}
					//player is not dead, play hit sound
					Mix_PlayChannel(-1, hit, 0);
			}
		}
		
		//eulerian integration
		enemyBullets[i].velocity.setX(enemyBullets[i].velocity.getX());
		enemyBullets[i].position.setX(enemyBullets[i].position.getX() + enemyBullets[i].velocity.getX() * 0.1);
		enemyBullets[i].velocity.setY(enemyBullets[i].velocity.getY());
		enemyBullets[i].position.setY(enemyBullets[i].position.getY() + enemyBullets[i].velocity.getY() * 0.1);
		enemyBullets[i].velocity.setZ(enemyBullets[i].velocity.getZ());
		enemyBullets[i].position.setZ(enemyBullets[i].position.getZ() + enemyBullets[i].velocity.getZ() * 0.1);		
	}
	
}	

void updateEnemyAngle() {
	
	for (int i = 0; i < levels[currentLevel].numBlocks; i++) {
		if (!levels[currentLevel].isFree[i] ||
			!levels[currentLevel].isAlive[i]) {
			//do not update angle if block isn't free or alive
			continue;
		}
		//use pythagorean theorem to get angle for enemy gun
		Vector3 e = Vector3(levels[currentLevel].blocks[i].getX() + blockWidth / 2.0,
							levels[currentLevel].blocks[i].getY() - blockHeight, 0.0);
		Vector3 p = Vector3(player.getX() + 4.0, player.getY() + 2.0, 0.0);
		float x = e.getX() - p.getX();
		float y = e.getY() - p.getY();
		float z = sqrt(x * x + y * y);
		float angle = acos(y / z) / (PI / 180.0);
		if (x > 0)
			levels[currentLevel].blockAngle[i] = angle * -1;
		else
			levels[currentLevel].blockAngle[i] = angle;
	}
	
}

void updateParticles() {
	
	//decrement for particles lifespan
	float dL = 5.5;
	for (int i = 0; i < levels[currentLevel].numBlocks; i++) {
		if (levels[currentLevel].isAlive[i]) {
			//if block is alive particles are not
			continue;
		}
		for (int j = 0; j < pAmt; j++) {
			if (!levels[currentLevel].particles[i].isAlive[j]) {
				//do not update if particle is dead
				continue;
			}
			if (levels[currentLevel].particles[i].lifeSpan[j] <= dL) {
				//remove particle if lifeSpan is less than deltaL
				levels[currentLevel].particles[i].lifeSpan[j] = 0.0;
				levels[currentLevel].particles[i].isAlive[j] = false;
			}
			else {
				//decrement lifespan by deltaL
				levels[currentLevel].particles[i].lifeSpan[j] -= dL;
				
				//eulerian integration
				levels[currentLevel].particles[i].velocity[j].setX(levels[currentLevel].particles[i].velocity[j].getX());
				levels[currentLevel].particles[i].velocity[j].setY(levels[currentLevel].particles[i].velocity[j].getY());
				levels[currentLevel].particles[i].velocity[j].setZ(levels[currentLevel].particles[i].velocity[j].getZ());
				levels[currentLevel].particles[i].position[j].setX(levels[currentLevel].particles[i].position[j].getX() + levels[currentLevel].particles[i].velocity[j].getX() * 0.1);
				levels[currentLevel].particles[i].position[j].setY(levels[currentLevel].particles[i].position[j].getY() + levels[currentLevel].particles[i].velocity[j].getY() * 0.1);
				levels[currentLevel].particles[i].position[j].setZ(levels[currentLevel].particles[i].position[j].getZ() + levels[currentLevel].particles[i].velocity[j].getZ() * 0.1);
			}
		}
	}
	if (victory.isAlive[0]) {
		//update balloon ps
		for (int i = 0; i < pAmt; i++) {
			if (victory.lifeSpan[i] <= 0.75) {
				victory.lifeSpan[i] = 0.0;
				victory.isAlive[i] = false;
				
				//update balloon pop ps
				for (int j = 0; j < pAmt; j++) {
					victoryPs[i].isAlive[j] = true;
					victoryPs[i].lifeSpan[j] = 255.0;
					victoryPs[i].position[j].setX(victory.position[i].getX());
					victoryPs[i].position[j].setY(victory.position[i].getY());
					victoryPs[i].position[j].setZ(victory.position[i].getZ());
				}
			}
			else {
				//play balloon pop to time it correctly, but only once
				if (i == 0 && victory.lifeSpan[i] == 96.0)
					Mix_PlayChannel(-1, balloonpop, 0);
				victory.lifeSpan[i] -= 0.75;
				
				victory.velocity[i].setX(victory.velocity[i].getX());
				victory.velocity[i].setY(victory.velocity[i].getY());
				victory.velocity[i].setZ(victory.velocity[i].getZ());
				victory.position[i].setX(victory.position[i].getX() + victory.velocity[i].getX() * 0.1);
				victory.position[i].setY(victory.position[i].getY() + victory.velocity[i].getY() * 0.1);
				victory.position[i].setZ(victory.position[i].getZ() + victory.velocity[i].getZ() * 0.1);
			}
		}
	}
	//update balloon pop ps
	for (int i = 0; i < pAmt; i++) {
		for (int j = 0; j < pAmt; j++) {
			if (!victoryPs[i].isAlive[j])
				continue;
			if (victoryPs[i].lifeSpan[j] <= 1.5) {
				victoryPs[i].lifeSpan[j] = 0.0;
				victoryPs[i].isAlive[j] = false;
			}
			else {
				//play explosion balloon sound, but only once
				if (i == 0 && victoryPs[i].lifeSpan[j] == 255.0)
					Mix_PlayChannel(-1, balloonps, 0);
				victoryPs[i].lifeSpan[j] -= 1.5;
				
				victoryPs[i].velocity[j].setX(victoryPs[i].velocity[j].getX());
				victoryPs[i].velocity[j].setY(victoryPs[i].velocity[j].getY());
				victoryPs[i].velocity[j].setZ(victoryPs[i].velocity[j].getZ());
				victoryPs[i].position[j].setX(victoryPs[i].position[j].getX() + victoryPs[i].velocity[j].getX() * 0.1);
				victoryPs[i].position[j].setY(victoryPs[i].position[j].getY() + victoryPs[i].velocity[j].getY() * 0.1);
				victoryPs[i].position[j].setZ(victoryPs[i].position[j].getZ() + victoryPs[i].velocity[j].getZ() * 0.1);	
			}
		}
	}	
	
	//update player ps
	for (int i = 0; i < pAmt; i++) {
		if (!playerPs.isAlive[i])
			continue;
		if (playerPs.lifeSpan[i] <= dL) {
			playerPs.lifeSpan[i] = 0.0;
			playerPs.isAlive[i] = false;
		}
		else {
			playerPs.lifeSpan[i] -= dL;
			
			playerPs.velocity[i].setX(playerPs.velocity[i].getX());
			playerPs.velocity[i].setY(playerPs.velocity[i].getY());
			playerPs.velocity[i].setZ(playerPs.velocity[i].getZ());
			playerPs.position[i].setX(playerPs.position[i].getX() + playerPs.velocity[i].getX() * 0.1);
			playerPs.position[i].setY(playerPs.position[i].getY() + playerPs.velocity[i].getY() * 0.1);
			playerPs.position[i].setZ(playerPs.position[i].getZ() + playerPs.velocity[i].getZ() * 0.1);
		}
	}
	
}

void drawLevel() {
	
	LevelType l = levels[currentLevel];
	
	for (int i = 0; i < l.numBlocks; i++) {
		if (!l.isAlive[i])
			continue;
		glBegin(GL_QUADS);
		glVertex3f(l.blocks[i].getX(), l.blocks[i].getY(), l.blocks[i].getZ());
		glVertex3f(l.blocks[i].getX() + blockWidth, l.blocks[i].getY(), l.blocks[i].getZ());
		glVertex3f(l.blocks[i].getX() + blockWidth, l.blocks[i].getY() - blockHeight, l.blocks[i].getZ());
		glVertex3f(l.blocks[i].getX(), l.blocks[i].getY() - blockHeight, l.blocks[i].getZ());
		glEnd();
		
		if (l.isFree[i]) {
			//draw a gun
			glPushMatrix();
			glTranslatef(l.blocks[i].getX() + blockWidth / 2.0, l.blocks[i].getY() - blockHeight, 0.0);
			glRotatef(l.blockAngle[i], 0.0, 0.0, 1.0);
			glBegin(GL_QUADS);
			glVertex3f(-0.5, -1.0, 0.0);
			glVertex3f(0.5, -1.0, 0.0);
			glVertex3f(0.5, 1.0, 0.0);
			glVertex3f(-0.5, 1.0, 0.0);
			//glVertex3f(l.blocks[i].getX() + 4.5, l.blocks[i].getY() - (blockHeight + 1.0), l.blocks[i].getZ());
			//glVertex3f(l.blocks[i].getX() + 4.5, l.blocks[i].getY() - (blockHeight - 1.0), l.blocks[i].getZ());
			//glVertex3f(l.blocks[i].getX() + 5.5, l.blocks[i].getY() - (blockHeight - 1.0), l.blocks[i].getZ());
			//glVertex3f(l.blocks[i].getX() + 5.5, l.blocks[i].getY() - (blockHeight + 1.0), l.blocks[i].getZ());
			glEnd();
			glPopMatrix();
		}
	}
	
}

void drawPlayer() {
	
	//draw player
	glBegin(GL_QUADS);
	glVertex3f(player.getX(), player.getY(), 0.0);
	glVertex3f(player.getX(), player.getY() + 2.0, 0.0);
	glVertex3f(player.getX() + 8.0, player.getY() + 2.0, 0.0);
	glVertex3f(player.getX() + 8.0, player.getY(), 0.0);
	glEnd();
	
	//draw player gun
	glPushMatrix();
	glTranslatef(player.getX() + 4.0, player.getY() + 2.0, 0.0);
	glRotatef(playerAngle, 0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -1.0, 0.0);
	glVertex3f(0.5, -1.0, 0.0);
	glVertex3f(0.5, 1.0, 0.0);
	glVertex3f(-0.5, 1.0, 0.0);
	glEnd();
	glPopMatrix();
	
}

void drawBullets() {
	//will draw both playerBullets and enemyBullets
	
	glPointSize(4.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < playerBullets.size(); i++) {
		glVertex3f(playerBullets[i].position.getX(),
				   playerBullets[i].position.getY(),
				   playerBullets[i].position.getZ());
	}
	glEnd();
	
	glColor3f(0.62, 0.62, 0.62);
	glPointSize(4.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < enemyBullets.size(); i++) {
		glVertex3f(enemyBullets[i].position.getX(),
				   enemyBullets[i].position.getY(),
				   enemyBullets[i].position.getZ());
	}
	glEnd();
	
}

void drawParticles() {
	
	glColor3f(0.87, 0.87, 0.87);
	glPointSize(3.5);
	glBegin(GL_POINTS);
	for (int i = 0; i < levels[currentLevel].numBlocks; i++) {
		if (levels[currentLevel].isAlive[i]) {
			//if block is alive particles are not
			continue;
		}
		for (int j = 0; j < pAmt; j++) {
			if (!levels[currentLevel].particles[i].isAlive[j]) {
				//only draw particle if it is alive
				continue;
			}
			glVertex3f(levels[currentLevel].particles[i].position[j].getX(),
					   levels[currentLevel].particles[i].position[j].getY(),
					   levels[currentLevel].particles[i].position[j].getZ());
		}
	}
	glEnd();
	
	//draw victory particles
	for (int i = 0; i < pAmt; i++) {
		if (!victory.isAlive[i])
			continue;
		glColor3f(victoryColor[i][0], victoryColor[i][1], victoryColor[i][2]);
		glPushMatrix();
		glTranslatef(victory.position[i].getX(), victory.position[i].getY(), victory.position[i].getZ());
		glutSolidSphere(5.0, 25, 25);
		glTranslatef(0.0, -1.75, 0.1);
		glutSolidSphere(4.0, 25, 25);
		glTranslatef(0.0, -1.50, 0.1);
		glutSolidSphere(2.75, 25, 25);
		glPopMatrix();
	}
	
	//draw victory balloon pop particles			   
	for (int i = 0; i < pAmt; i++) {
		for (int j = 0; j < pAmt; j++) {
			if (!victoryPs[i].isAlive[j])
				continue;
			glColor3f(victoryColor[i][0], victoryColor[i][1], victoryColor[i][2]);
			glPointSize(2.0);
			glBegin(GL_POINTS);
			glVertex3f(victoryPs[i].position[j].getX(), victoryPs[i].position[j].getY(), victoryPs[i].position[j].getZ());
			glEnd();
		}
	}
	
	//draw player particles
	for (int i = 0; i < pAmt; i++) {
		if (!playerPs.isAlive[i])
			continue;
		glColor3f(1.0, 1.0, 1.0);
		glPointSize(2.0);
		glBegin(GL_POINTS);
		glVertex3f(playerPs.position[i].getX(), playerPs.position[i].getY(), playerPs.position[i].getZ());
		glEnd();
	}
}

void display() {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	//draw playing field
	glColor3f(0.1, 0.0, 0.9);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-25.0, -40.0, 0.0);
	glVertex3f(-25.0, 40.0, 0.0);
	glVertex3f(25.0, 40.0, 0.0);
	glVertex3f(25.0, -40.0, 0.0);
	glEnd();
	
	//draw things
	glColor3f(0.62, 0.62, 0.62);
	drawLevel();	
	glColor3f(1.0, 1.0, 1.0);
	drawPlayer();
	drawBullets();
	drawParticles();
	
	//draw health bar
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-75.0, 45.0, 0.0);
	glVertex3f(-35.0, 45.0, 0.0);
	glVertex3f(-35.0, 40.0, 0.0);
	glVertex3f(-75.0, 40.0, 0.0);
	glEnd();
	
	//fill health bar in RED
	if (playerHits != 4) {
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_QUADS);
		glVertex3f(-74.9, 44.9, 0.0);
		glVertex3f(-35.1 - (playerHits * 10), 44.9, 0.0);
		glVertex3f(-35.1 - (playerHits * 10), 40.1, 0.0);
		glVertex3f(-74.9, 40.1, 0.0);
		glEnd();
		glColor3f(1.0, 1.0, 1.0);
	}
	
	//draw lives to screen
	std::string ltxt = "Lives: ";
	std::stringstream ss1;
	ss1 << lives;
	std::string s1 = ltxt + ss1.str();
	glRasterPos2d(-75.0, 46.0);
	for (int i = 0; i < s1.size(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s1[i]);
	}
	
	//draw score to screen
	std::string stxt = "Score: ";
	std::stringstream ss2;
	ss2 << score;
	std::string s2 = stxt + ss2.str();
	glRasterPos2d(-75.0, 37.5);
	for (int i = 0; i < s2.size(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s2[i]);
	}
	
	//draw ammo to screen
	std::string atxt = "Ammo: ";
	std::stringstream ss3;
	ss3 << playerAmmo;
	std::string s3 = atxt + ss3.str();
	glRasterPos2d(-75.0, -40.0);
	for (int i = 0; i < s3.size(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s3[i]);
	}
	
	//draw you died to screen if you died
	if (lives <= 0) {
		glColor3f(1.0, 0.2, 0.2);
		std::string dtxt = "You died.";
		glRasterPos2d(-6.0, 0.0);
		for (int i = 0; i < dtxt.size(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, dtxt[i]);
		}
	}
	
	//draw victory to screen when you win
	if (vStatus) {
		std::string vtxt = "Victory!";
		glRasterPos2d(-5.0, 0.0);
		for (int i = 0; i < vtxt.size(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, vtxt[i]);
		}
	}
	
	glFlush();
	glutSwapBuffers();
	
}

void reshape(GLint w, GLint h) {
	
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	GLfloat aspect = GLfloat(w) / GLfloat(h);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-50.0, 50.0, -50.0 / aspect, 50.0 / aspect, -100.0, 100.0);
	else
		glOrtho(-50.0 * aspect, 50.0 * aspect, -50.0, 50.0, -100.0, 100.0);
		
}		

void keyboard(unsigned char key, int, int) {
	
	switch (key) {
	case 27: //ESC
		exit(0); //close program
		break;
	case 32: //spacebar
		//spawn bullet
		spawnBullet();
		break;
	case 82: //R
	case 114: //r
		resetGame();
		break;
	case 76: //L
	case 108: //l
		//increase lives
		if (lives < 5)
			lives++;
		break;
	case 75: //K
	case 107: //k
		//decrease lives
		if (lives > 1)
			lives--;
		break;
	case 65: //A
	case 97: //a
		//move player left
		if (player.getX() > -25.0)
			player.setX(player.getX() - 1.0);
		else
			player.setX(-25.0);
		break;
	case 68: //D
	case 100: //d
		//move player right
		if (player.getX() < 17.0)
			player.setX(player.getX() + 1.0);
		else
			player.setX(17.0);
		break;
	case 81: //Q
	case 113: //q
		//angle gun left
		if (playerAngle >= 45.0)
			playerAngle = 45.0;
		else
			playerAngle += 5.0;
		break;
	case 69: //E
	case 101: //e
		//angle gun right
		if (playerAngle <= -45.0)
			playerAngle = -45.0;
		else 
			playerAngle -= 5.0;
		break;
	}
}

void special(int key, int, int) {
	
	//left and right keys for player directional movement
	switch (key) {
	case GLUT_KEY_LEFT: //left arrow key
		//keep player inside left-bound of playing field
		if (player.getX() > -25.0)
			player.setX(player.getX() - 1.0);
		else
			player.setX(-25.0);
		break;
	case GLUT_KEY_RIGHT: //right arrow key
		//keep player inside right-bound of playing field
		if (player.getX() < 17.0)
			player.setX(player.getX() + 1.0);
		else
			player.setX(17.0);
		break;
	case GLUT_KEY_PAGE_UP: //page up key
		//rotate player gun to left
		if (playerAngle >= 45.0)
			playerAngle = 45.0;
		else
			playerAngle += 5.0;
		break;
	case GLUT_KEY_PAGE_DOWN: //page down key
		//rotate player gun to right
		if (playerAngle <= -45.0)
			playerAngle = -45.0;
		else 
			playerAngle -= 5.0;
		break;
	}
	
}

void joystick(unsigned int button, int x, int y, int z) {
	//INCREDIBLY SENSITIVE
	
	switch(button) {
	case GLUT_JOYSTICK_BUTTON_A:  //A
		//spawnBullet when A is pressed
		spawnBullet();
		break;
	case 16: //left bumper
		//rotate player gun to left
		if (playerAngle >= 45.0)
			playerAngle = 45.0;
		else
			playerAngle += 5.0;
		break;
	case 32: //right bumper
		//rotate player gun to right
		if (playerAngle <= -45.0)
			playerAngle = -45.0;
		else 
			playerAngle -= 5.0;
		break;
	}
	if (x < -100) {
		//get axes location from joystick and adjust accordingly
		if (player.getX() > -25.0)
			player.setX(player.getX() - 1.0);
		else
			player.setX(-25.0);
	}
	if (x > 100) {
		//get axes location from joystick and adjust accordingly
		if (player.getX() < 17.0)
			player.setX(player.getX() + 1.0);
		else
			player.setX(17.0);
	}	
	
}	

void timer(int v) {
	
	//update bullet vel/pos
	updatePlayerBullets();
	updateEnemyBullets();
	
	//update particles
	updateParticles();
	
	//update enemy bullets variable, but keep it from getting unnecessarily large
	if (enemyBulletsCount % 101 == 0) {
		spawnEnemyBullet();
	}
	if (enemyBulletsCount >= 1000)
		enemyBulletsCount = 0;
	else
		enemyBulletsCount++;
		
	//update enemy gun angle
	updateEnemyAngle();
	
	glutPostRedisplay();
	glutTimerFunc(16.6667, timer, v); //60 FPS
	
}

void init() {
	
	//seed random number generator
	srand(time(NULL));
	
	//init player
	player = Vector3(-4.0, -35.0, 0.0);
	playerAngle = 0.0;
	
	//initialize level0, level1, level2
	LevelType level0 = initLevel0();	
	levels.push_back(level0);
	LevelType level1 = initLevel1();
	levels.push_back(level1);
	LevelType level2 = initLevel2();
	levels.push_back(level2);
	
	//init global variables
	currentLevel = 0;
	playerHits = 0;
	enemyBulletsCount = -100;
	lives = 3;
	score = 0;
	playerAmmo = 5;
	vStatus = false;
	
	//init "background" player bullet to keep program from breaking
	BulletType bB;
	bB.position = Vector3(-100.0, -100.0, -100.0);
	bB.velocity = Vector3(0.0, 0.0, 0.0);
	playerBullets.push_back(bB);
	
	//init victory particle system
	int r;
	for (int j = 0; j < pAmt; j++) {
		victory.position[j] = Vector3(rand() % 50 - 25, -55.0, 0.0);
		victory.velocity[j] = Vector3((rand() % 100 / 25.0) - 2.0, 1 * rand() % 100 / 25.0, 0.1);
		victory.isAlive[j] = false;
		victory.lifeSpan[j] = 255.0;
		
		//init random color for cool balloon color effects
		r = rand() % 10;
		if (r < 2) {
			victoryColor[j][0] = 1.0;
			victoryColor[j][1] = 0.0;
			victoryColor[j][2] = 0.0;
		}
		else if (r < 4) {
			victoryColor[j][0] = 0.0;
			victoryColor[j][1] = 1.0;
			victoryColor[j][2] = 0.0;
		}
		else if (r < 6) {
			victoryColor[j][0] = 0.0;
			victoryColor[j][1] = 0.0;
			victoryColor[j][2] = 1.0;
		}
		else if (r < 8) {
			victoryColor[j][0] = 1.0;
			victoryColor[j][1] = 1.0;
			victoryColor[j][2] = 0.0;
		}
		else {
			victoryColor[j][0] = 0.0;
			victoryColor[j][1] = 1.0;
			victoryColor[j][2] = 1.0;
		}
		
		for (int i = 0; i < pAmt; i++) {
			victoryPs[i].position[j] = Vector3();
			victoryPs[i].velocity[j] = Vector3((rand() % 100 / 25.0) - 2.0, (rand() % 100 / 25.0) - 2.0, 2.5);
			victoryPs[i].isAlive[j] = false;
			victoryPs[i].lifeSpan[j] = 255.0;
		}	
	}	
	
	//init player particle system
	for (int i = 0; i < pAmt; i++) {
		playerPs.position[i] = Vector3();
		playerPs.velocity[i] = Vector3((rand() % 100 / 25.0) - 2.0, -1 * (rand() % 100 / 50.0), 2.5);
		playerPs.isAlive[i] = false;
		playerPs.lifeSpan[i] = 255.0;
	}
	
	//init SDL_mixer and start playing background audio, init sounds
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		std::cout << "Error: Failed to init SDL" << std::endl;
		exit(0);
	}
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 2048) == -1) {
		std::cout << "Error: Failed to init OpenAudio" << std::endl;
		exit(0);
	}
	bkg = Mix_LoadMUS("../src/bkg2.WAV");
	boom = Mix_LoadWAV("../src/boom.WAV");
	fire = Mix_LoadWAV("../src/fire.WAV");
	ammoless = Mix_LoadWAV("../src/ammoless.WAV");
	defeat = Mix_LoadWAV("../src/defeat.WAV");
	victoryS = Mix_LoadWAV("../src/victory.WAV");
	blowup = Mix_LoadWAV("../src/blowup.WAV");
	balloonpop = Mix_LoadWAV("../src/balloonpop.WAV");
	hit = Mix_LoadWAV("../src/hit.WAV");
	enemyfire = Mix_LoadWAV("../src/enemyfire.WAV");
	levelup = Mix_LoadWAV("../src/levelup.WAV");
	miss = Mix_LoadWAV("../src/miss.WAV");
	congrats = Mix_LoadWAV("../src/congrats.WAV");
	balloonps = Mix_LoadWAV("../src/balloonps.WAV");
	gameover = Mix_LoadWAV("../src/gameover.WAV");
	ammoup = Mix_LoadWAV("../src/ammoup.WAV");
	Mix_PlayMusic(bkg, -1);
		
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1280, 800);
	glutCreateWindow("Arcade Smasher");
	glutFullScreen();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutTimerFunc(100, timer, 0);
	glutJoystickFunc(joystick, 25);
	init();
	glutMainLoop();
	
}
