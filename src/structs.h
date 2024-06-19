/****************************************************************
 *
 *
 *    ███████╗████████╗██████╗ ██╗   ██╗ ██████╗████████╗███████╗
 *    ██╔════╝╚══██╔══╝██╔══██╗██║   ██║██╔════╝╚══██╔══╝██╔════╝
 *    ███████╗   ██║   ██████╔╝██║   ██║██║        ██║   ███████╗
 *    ╚════██║   ██║   ██╔══██╗██║   ██║██║        ██║   ╚════██║
 *    ███████║   ██║   ██║  ██║╚██████╔╝╚██████╗   ██║   ███████║
 *    ╚══════╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝  ╚═════╝   ╚═╝   ╚══════╝
 *
 *
 *   Structs and Enums for the game.
 *
 *
 *****************************************************************/

#ifndef STRUCTS_H
#define STRUCTS_H

#include "defs.h"
#include <raylib.h>

/*============================================================================
 *                                  ENUMS
 *
 *    Put here all sort of types you want make sure that anychange here
 *    will be reflected in the whole code. (e.g. Switch cases, etc.)
 *===========================================================================*/


// TODO: Make enum for all stats related to the specified types
// instead of no encapsulation.

// FIXME: this probably should be refactored and removed.
typedef enum
{
  W = 1,
  A = 2,
  S = 5,
  D = 9,
} KEYS;

typedef enum
{
  ACTION_NONE,
  ACTION_BULLET,
  ACTION_SLASH
} CombatActionType;

typedef enum
{
  RANGED_WEAPON,
  MELEE_WEAPON,
  NUM_OF_WEAPON_TYPES
} WeaponType;

typedef enum
{
  E_SWORD,
  NUM_OF_E_WEAPON
} E_WEAPON;

typedef enum
{
  CAT,
  WEREWOLF,
  PYROMANIAC,
  KNIGHT,
  NUM_OF_P_TYPE
} P_TYPE;

typedef enum
{
  P_GUN,
  P_LONG_SWORD,
  NUM_OF_P_WEAPON
} P_WEAPON;

typedef enum
{
  UP,
  DOWN,
  LEFT,
  RIGHT,
} DIRECTIONS;

typedef enum
{
  E_CIVILIAN,
  E_FARMER,
  E_KNIGHT,
  NUM_OF_E_TYPE
} E_TYPE;

typedef enum
{
  PATROL,
  IDLE,
  CHASE,
  ATTACK,
  FLEE
} State;


/*============================================================================
 *                                  STRUCTS
 *
 *    Here are all the structs (Think of them as object data that you use)
 *    Change here are expensive as these Almost types and used or all over
 *    the place so make sure every thing is OK after changes.
 *    
 *===========================================================================*/


// **********************
// GENERAL ENGINE STRUCTS
// **********************

typedef struct CTransform
{
  Vector2 position;
  float rotation;
  Vector2 scale;
} CTransform;

typedef struct
{
  Vector2 velocity;
  Vector2 acceleration;
  float drag;
  bool isKinematic; // Kinematic object is typically not affected by physics
                    // forces but can still interact with other objects in
                    // certain ways.
} RigidBody2D;

typedef struct
{
  Vector2 offset;
  float width;
  float height;
} Collider2D;


typedef struct AtlasImage
{
  char *filename;
  Rectangle source;
  Vector2 origin;
  struct AtlasImage *next;
} AtlasImage;

// FIXME: Rework SpriteRenderer or remove it if not used.
// Atlas Image could do the same work if it has the dest rect.
typedef struct
{
  Texture2D texture;
  int width;
  int height;
} SpriteRenderer;


// FIXME: Rework SpriteAnimation and animator
// Animator should handle the state and what animation to show.
// Animation is the animation itself.
typedef struct
{
  char **frameNames; // Idk what is the type of the animation sprites.
  int currentFrame;
  int numOfFrames;
  int framesPerSecond;
  float frameTime;
  float elapsedTime; // Time elapsed since the last frame change.
  bool loop;         // NOTE: still not used
  bool finished;     // NOTE: still not used
} Animator;

typedef struct SpriteAnimation
{
  int numOfFrames;
  char **frameNames;
  int currentFrame; // NOTE: still not used
  int framesPerSecond;
  bool loop;     // NOTE: still not used
  bool finished; // NOTE: still not used
} SpriteAnimation;


typedef struct
{
  int up;
  int down;
  int left;
  int right;
  int shoot;
  int action;
} Input;

typedef struct
{
  CTransform transform;
  RigidBody2D rigidBody;
  Collider2D collider;
  SpriteRenderer spriteRenderer;
  Animator animator;
} GameObject;


// *********************
// GAME SPECIFIC STRUCTS
// *********************

typedef struct
{
  int currentHealth;
  int maxHealth;
} Health;

// IDK if those affect the other structs or not (like leveling up)
typedef struct
{
  float power;
  float speed;
  float cooldown;
} Attack;

typedef struct
{
  int value;
  int nearHitValue; // Blocked on the last second.
  // TODO: Add defense for different type of attacks?
} Defense;

typedef struct
{
  int xp;
  int level;
} Experience;

typedef struct
{
  Health health;
  Attack attack;
  Defense defense;
  double speed;
} Stats;

typedef struct
{
  float bulletSpeed;
  float bulletDamage;
  float bulletRange;
  float bulletHealth;
  SpriteRenderer bulletSprite;
  RigidBody2D rigidBody;
  Collider2D collider;
} BulletInfo;

typedef struct
{
  int playerID;
  BulletInfo bulletInfo;
  Vector2 startPosition; // To know if the bullet exceeded the range.
  CTransform transform;
} Bullet;

typedef struct
{
  float slashRange;
  float slashDamage;
  bool isActive;
  SpriteRenderer slashSprite;
  Collider2D collider;
} SlashInfo;

typedef struct
{
  int playerID;
  SlashInfo slashInfo;
  CTransform transform;
} Slash;

typedef union
{
  Bullet bullet;
  Slash slash;
} CombatActionUnion;

typedef struct
{
  float angle;
  SpriteAnimation anime;
  CombatActionUnion action;
  CombatActionType type;
} CombatAction;

typedef struct
{
  int damage;
  float cooldown;
  float lastUseTime;
  SpriteRenderer weaponSprite;
} WeaponStats;

typedef struct
{
  WeaponStats stats;
  BulletInfo bulletInfo;
  int maxAmmo;
  int ammo;
} RangedWeapon;

typedef struct
{
  WeaponStats stats;
  SlashInfo slashInfo;
} MeleeWeapon;

typedef union
{
  RangedWeapon ranged;
  MeleeWeapon melee;
} WeaponUnion;

typedef struct
{
  const char *name;
  WeaponType type;
  WeaponUnion weapon;
} Weapon;

typedef struct
{
  int MAX_NUM_OF_WEAPONS;
  int currentNumOfWeapons;
  int currentWeapon;
  Weapon *weapons;
} Inventory;


typedef struct
{
  Vector2 patrolStart;
  Vector2 patrolEnd;
  float detectionRange;
  float attackCooldown;
  float lastAttackTime;
  float dodgePercentage; // Dodge or Parry or Block. Or do these three
                         // separately??
  float speed;
  State state;
} EnemyAI;


typedef struct
{
  char *name;
  GameObject object;
  Stats stats;
  Weapon weapon;
  E_TYPE type;
  EnemyAI ai;
  int drawDirection; // 1 for right, -1 for left
} Enemy;

typedef struct
{
  // Player Info
  char *name;
  int ID;

  // Player Selection
  P_TYPE type;

  // Player Stats
  Stats stats;
  Inventory inventory;
  GameObject object;
  Input input;
  Experience experience;
  int score;
  int fire;
  int drawDirection; // 1 for right, -1 for left
  bool isMoving;
  DIRECTIONS direction; // to get info on the direction the player is facing.
} Player;

// ******************
// GAME STATE STRUCTS
// ******************

typedef struct
{
  int screenWidth;
  int screenHeight;
  bool fullscreen;
  int musicVolume;
  int soundVolume;
  bool music_on;
  bool sfx_on;
} Settings;

typedef union
{
  Weapon weapon;
  Enemy enemy;
  Player player;
} DictionaryEntry;

typedef struct
{
  int opcode;
  DictionaryEntry entry;
} Dictionary;


typedef struct
{
  int numOfPlayers;
  Player *players;

  int numOfEnemies;
  Enemy *enemies;

  int numOfCombatActions;
  CombatAction *combatActions;

  int level;
  bool isGameOver;
  bool isFinished;
  Texture2D atlasTexture;     // The image atals containaing all the sprites and animations
  AtlasImage *atlasImages;    // Linked List of individual sprites and animations data

  Dictionary *playerWeaponDictionary;
  Dictionary *enemyWeaponDictionary;
  Dictionary *characterDictionary;
  Dictionary *enemyDictionary;

  Settings settings;
} GameState;

#endif // STRUCTS_H
