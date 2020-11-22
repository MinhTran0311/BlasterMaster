﻿#pragma once

#include <Windows.h>  
#include <d3d9.h>
#include <d3dx9.h>
#include <map>
#include "debug.h" 

#include <fstream> 
#include <string>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <iomanip>
#include <vector>
#include <iostream>

#define WINDOW_CLASS_NAME L"BLASTER MASTER"
#define MAIN_WINDOW_TITLE L"HOMIES GAME"

#define BGColor D3DCOLOR_XRGB(255,255,200)
#define BGColorCrossing D3DCOLOR_XRGB(255,255,200)
#define SCREEN_WIDTH 320	//530
#define SCREEN_HEIGHT 274	//480
#define BOTTOM_SCREEN 450
#define CAMERA_SPEED_WORLD1 0.223
#define CAMERA_SPEED_OVERWORLD 0.15
#define MAX_FRAME_RATE 600	//default Frame Rate is the truth

#define SCENEGAME_GAMETIMEMAX		300

#define DIRECTINPUT_VERSION 0x0800
#define KEYBOARD_BUFFER_SIZE 1024

#define MAX_SCENE_LINE 1024

#define ID_AREA1			11
#define ID_AREA2			12
#define ID_AREA3			13
#define ID_AREA4			14
#define ID_AREA5			15
#define ID_AREA6			16
#define ID_AREA7			17
#define ID_AREA8			18
#define ID_AREA9			19
#define ID_MAPOVERWORLD		20


#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_OBJECTS			1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_SCENEFILEPATH		6
#define SCENE_SECTION_MAPTEXTURES		12
#define SCENE_SECTION_CLEARTEXTURES			7
#define SCENE_SECTION_CLEARSPRITES			8
#define SCENE_SECTION_CLEARANIMATIONS		9
#define SCENE_SECTION_CLEARANIMATION_SETS	10


#define ANIMATION_SET_BBOX					0
#define ANIMATION_SET_PLAYER				1
#define ANIMATION_SET_PLAYERHP				11
//#define ANIMATION_SET_ITEM_POWERUP			12
//#define ANIMATION_SET_ITEM_GUNUP			13
#define ANIMATION_SET_BRICK					20
#define ANIMATION_SET_GATE					26
#define ANIMATION_SET_JASON_BULLET			3000
#define ANIMATION_SET_JASON_ROCKET			3001
#define ANIMATION_SET_BIG_ENEMY_BULLET		4001
#define ANIMATION_SET_SMALL_ENEMY_BULLET		4002
#define ANIMATION_SET_SKULL_ENEMY_BULLET		4003

#define SAFE_DELETE(a) { delete (a); (a) = NULL; }

enum EnemyType
{
	WORM = 10,
	DOME = 11,
	FLOATER = 12,
	INSECT = 13,
	ORB = 14,
	JUMPER = 15,
	SKULL = 16,
	EYEBALL = 17,
	TELEPOTER =18,
	CANNON = 19,
};
enum EntityType
{
	//For Special Case
	NONE = -1,

	////Bounding Box
	//BBOX = 0,
	TAG_JASON = 101,
	TAG_SMALL_SOPHIA = 102,
	TAG_BIG_SOPHIA = 103,
	////Player Zone 1-10
	//PLAYER = 1,
	//MORNINGSTAR = 2,
	//DAGGER = 3,
	//BOOMERANG = 4,
	//AXE = 5,
	//WATERPOTION = 6,
	//STOPWATCH = 7,
	//BONE = 9,

	////UI Zone 10-20
	//BLACKBOARD = 10,
	//PLAYERHP = 11,
	//TEXTS = 12,
	//UIEXTRASHOT = 13,

	////Static Obj Zone 20-40
	TAG_BRICK = 1,
	TAG_GATE = 2,
	//BREAKABLEBRICK = 21,
	//TORCH = 22,
	//CANDLE = 23,
	//MOVINGPLATFORM = 24,
	//STAIRS = 25,

	//BUSH = 27,
	TAG_ITEM = 40,
	////Item Zone 40-70
	//POWERUP = 40,
	//GUNUP = 41,
	//YUMMICHICKENLEG = 42,
	//MONEYBAGS = 43,
	//MONEYBAGRED = 44,
	//MONEYBAGWHITE = 45,
	//MONEYBAGBLUE = 46,
	//UPGRADEMORNINGSTAR = 47,
	//CRYSTALBALL = 48,
	//INVIPOTION = 49,
	//ITEMDAGGER = 50,
	//ITEMSTOPWATCH = 51,
	//CROSS = 52,
	//CROWN = 53,
	//ITEMEXTRASHOT = 54,
	//ITEMBOOMERANG = 55,
	//ITEMAXE = 56,
	//ITEMWATERPOTION = 57,

	////Effect Zone 70-80
	//FIREEFFECT = 70,
	//HITEFFECT = 71,
	//BRICKEXPLODEEFFECT = 72,
	//ADDSCOREEFFECT = 73,

	////Enemies Zone 80-100
	ENEMY = 80,
	TAG_WORM = 10,
	TAG_DOMES = 11,
	TAG_FLOATER = 12,
	TAG_INSECT = 13,
	TAG_ORB = 14,
	TAG_JUMPER = 15,
	TAG_SKULL = 16,
	TAG_EYEBALLS = 17,
	TAG_TELEPOTERS = 18,
	TAG_CANNONS = 19,
	////GOLEM = 80,
	//Worm = 81,
	////DOMES = 82,
	//bullet 30 ->50
	BULLET=30,
	JASON_NORMAL_BULLET = 31,
	JASON_UPGRADE_BULLET = 32,
	JASON_ROCKET_BULLET = 33,
	CANNONS_BULLET = 41,
	//KNIGHT = 83,
	//GHOST = 84,
	//RAVEN = 85,
	//HUNCHMAN = 86,
	//SKELETON = 87,
	//TLEBAT = 88,

	////Scene Zone 100-120
	//TITLESCENE = 100,
	//TITLEBAT = 101,
	//STARTTEXT = 102,
	//INTROSCENE = 103,
	//INTROSIMON = 104,
	//PLAYSCENE = 105,
	//MAPSTAGE1 = 106,
	//MAPSTAGE2_1 = 107,
	//MAPSTAGE2_2 = 108,
	//MAPSTAGE3_1 = 109,
	//MAPSTAGE3_2 = 110,

	////Ex
	//ITEMPOKEBALL = -9,
	//POKEBALL = -10
};

class define
{
};

