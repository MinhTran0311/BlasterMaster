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
#define ANIMATION_SET_PLAYERHP				30
#define ANIMATION_SET_BRICK					20
#define ANIMATION_SET_GATE					26
#define ANIMATION_SET_JASON_BULLET			31
#define ANIMATION_SET_SMALL_SOPHIA_BULLET	34
#define ANIMATION_SET_JASON_ROCKET			32
#define ANIMATION_SET_BIG_ENEMY_BULLET		41
#define ANIMATION_SET_SMALL_ENEMY_BULLET	42
#define ANIMATION_SET_SKULL_ENEMY_BULLET	43
#define ANIMATION_SET_ITEM_POWERUP			51
#define ANIMATION_SET_ITEM_GUNUP			52

#define PLAYER_MAX_HEALTH	8
#define PLAYER_DEFAULT_GUNDAM	1

#define SAFE_DELETE(a) { delete (a); (a) = NULL; }

enum EnemyType
{
	WORM = 10,
	DOMES = 11,
	FLOATERS = 12,
	INSECT = 13,
	ORBS = 14,
	JUMPERS = 15,
	SKULLS = 16,
	EYEBALLS = 17,
	TELEPORTERS =18,
	CANNONS = 19,
};
enum EntityType
{
	//For Special Case
	NONE = -1,

	//static objects
	TAG_BRICK = 1,
	TAG_GATE = 2,

	//player
	TAG_JASON = 101,
	TAG_SMALL_SOPHIA = 102,
	TAG_BIG_SOPHIA = 103,

	////Enemies Zone 10 - 20
	ENEMY = 80,
	TAG_WORM = 10,
	TAG_DOMES = 11,
	TAG_FLOATER = 12,
	TAG_INSECT = 13,
	TAG_ORBS = 14,
	TAG_JUMPERS = 15,
	TAG_SKULLS = 16,
	TAG_EYEBALLS = 17,
	TAG_TELEPORTERS = 18,
	TAG_CANNONS = 19,

	//Bullet	30 - 50
	BULLET=30,
	JASON_NORMAL_BULLET = 31,
	JASON_UPGRADE_BULLET = 32,
	JASON_ROCKET_BULLET = 33,
	SMALL_SOPHIA_NORMAL_BULLET = 34,
	CANNONS_BULLET = 41,
	//Item,
	ITEM = 50,
	TAG_ITEM_POWER_UP = 51,
	TAG_ITEM_GUN_UP = 52,

	SKULLS_BULLET = 43,
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
