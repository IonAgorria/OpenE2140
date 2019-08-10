//
// Created by Ion Agorria on 2/06/19
//
#ifndef OPENE2140_CONSTANTS_H
#define OPENE2140_CONSTANTS_H

#define MONEY_PER_CONTAINER 200
#define CAMERA_SPEED 10
#define TILE_SIZE 64

/**
 * Entity kind IDs
 */
#define ENTITY_KIND_OBJECT 0
#define ENTITY_KIND_UNIT 1
#define ENTITY_KIND_BUILDING 2
#define ENTITY_KIND_PROJECTILE 3
#define ENTITY_KIND_EXPLOSION 4

/**
 * Constants for palette indexes with special meaning
 */
//Unit movement dependent colors (threads/wheels/jets)
#define PALETTE_UNIT_MOVEMENT0 0xF0
#define PALETTE_UNIT_MOVEMENT1 0xF1
#define PALETTE_UNIT_MOVEMENT2 0xF2
#define PALETTE_UNIT_MOVEMENT3 0xF3
//Unit weapon fire
#define PALETTE_UNIT_FIRE0 0xF4
#define PALETTE_UNIT_FIRE1 0xF5
#define PALETTE_UNIT_FIRE2 0xF6
//Unit light
#define PALETTE_UNIT_LIGHT0 0xF7
//Unit player coloring
#define PALETTE_UNIT_PLAYER0 0xF8
#define PALETTE_UNIT_PLAYER1 0xF9
#define PALETTE_UNIT_PLAYER2 0xFA
#define PALETTE_UNIT_PLAYER3 0xFB
#define PALETTE_UNIT_PLAYER4 0xFC
#define PALETTE_UNIT_PLAYER5 0xFD
//Unit shadow
#define PALETTE_UNIT_SHADOW0 0xFE

//Building lights to be set as black when has no power (shipyard doesn't have so not set)
#define PALETTE_BUILDING_LIGHT0 0xF4
#define PALETTE_BUILDING_LIGHT1 0xF5
#define PALETTE_BUILDING_LIGHT2 0xF6
#define PALETTE_BUILDING_LIGHT3 0xF7
//Building player coloring
#define PALETTE_BUILDING_PLAYER0 0xF8
#define PALETTE_BUILDING_PLAYER1 0xF9
#define PALETTE_BUILDING_PLAYER2 0xFA
#define PALETTE_BUILDING_PLAYER3 0xFB
#define PALETTE_BUILDING_PLAYER4 0xFC
//Building shadow tones
#define PALETTE_BUILDING_SHADOW0 0xFD
#define PALETTE_BUILDING_SHADOW1 0xFE

#endif //OPENE2140_CONSTANTS_H
