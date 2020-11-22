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

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"TUAN - 1"

#define BGColor D3DCOLOR_XRGB(255,255,200)
#define BGColorCrossing D3DCOLOR_XRGB(255,255,200)
#define SCREEN_WIDTH 320	//530
#define SCREEN_HEIGHT 275	//480
#define BOTTOM_SCREEN 450

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
#define ANIMATION_SET_ITEM_POWERUP			12
#define ANIMATION_SET_ITEM_GUNUP			13
#define ANIMATION_SET_BRICK					20
#define ANIMATION_SET_GATE					26


enum EnemyType
{

	FLOATER = 12,
	INSECT = 13,
	WORM = 1,
	//FLOATER = 2,
	ORBS = 14,
	JUMPER = 15,
	SKULLS  = 16,

};
enum EntityType
{
	//For Special Case
	NONE = -1,

	////Bounding Box
	//BBOX = 0,

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
	TAG_BRICK = 20,
	TAG_GATE = 21,
	//BREAKABLEBRICK = 21,
	//TORCH = 22,
	//CANDLE = 23,
	//MOVINGPLATFORM = 24,
	//STAIRS = 25,
	
	//BUSH = 27,

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
	Tag_Worm = 81,
	Tag_Floater = 82,
	Tag_Insect = 83,
	Tag_Orbs = 94,
	Tag_Jumpers = 95,
	Tag_Skulls = 96,
	////GOLEM = 80,
	//Worm = 81,
	////DOMES = 82,

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

