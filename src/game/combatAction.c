/***************************************************************
 *
 *
 * 
 *     ██████╗ ██████╗ ███╗   ███╗██████╗  █████╗ ████████╗   █████╗  ██████╗████████╗██╗ ██████╗ ███╗   ██╗
 *    ██╔════╝██╔═══██╗████╗ ████║██╔══██╗██╔══██╗╚══██╔══╝  ██╔══██╗██╔════╝╚══██╔══╝██║██╔═══██╗████╗  ██║
 *    ██║     ██║   ██║██╔████╔██║██████╔╝███████║   ██║     ███████║██║        ██║   ██║██║   ██║██╔██╗ ██║
 *    ██║     ██║   ██║██║╚██╔╝██║██╔══██╗██╔══██║   ██║     ██╔══██║██║        ██║   ██║██║   ██║██║╚██╗██║
 *    ╚██████╗╚██████╔╝██║ ╚═╝ ██║██████╔╝██║  ██║   ██║     ██║  ██║╚██████╗   ██║   ██║╚██████╔╝██║ ╚████║
 *     ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚═════╝ ╚═╝  ╚═╝   ╚═╝     ╚═╝  ╚═╝ ╚═════╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝
 *                                                                                                          
 *     Combat Action Module Header. (Game Object)
 *     Exposes the logic for the Combat Action object.
 *
 *     - Setup Combat Action
 *     - Update Combat Action
 *     - Draw Combat Action
 *     - Check for collision
 *     - Clear Combat Action
 *
 **************************************************************/

#include "combatAction.h"

#include "../system/anime.h"
// FIXME: delete me later
#include "../system/init.h"
#include <raymath.h>

// ***************************
// Private Function Prototypes
// ***************************
static bool checkCollision(Rectangle rect1, Rectangle rect2);
static bool bulletCollision(CombatAction *combatAction);
static bool slashCollision(CombatAction *combatAction);
static void drawBullet(CombatAction **combatAction);
static void drawSlash(CombatAction **combatAction);
static void clearCombatAction(CombatAction **combatAction);

/**
 *  initBullet - initialize a bullet object
 * 
 * @param ID Player's ID
 * @param bulletInfo Bullet's information from the used weapon
 * 
 * @return Pointer to the combat action object
 * 
 * @details Initialize a bullet object and link it to the player
 * by `ID`, its information is provided by the fired weapon.
 *
 */
CombatAction *initBullet(int ID, BulletInfo bulletInfo, Vector2 src, Vector2 dest) {
  if (gameState->numOfCombatActions == DEFAULT_MAX_COMBAT_ACTIONS) return NULL;
  // Init bullet
  Bullet bullet;
  bullet.playerID = ID;
  bullet.bulletInfo = bulletInfo;
  bullet.startPosition = src;
  bullet.transform = (CTransform){src, 0, (Vector2){1, 1}};

  // Init combatAction
  CombatAction *combatAction = (gameState->combatActions + gameState->numOfCombatActions++);
  combatAction->angle = GetAngleBetweenPoints(src, dest);
  combatAction->type = ACTION_BULLET;
  combatAction->action.bullet = bullet;
  return combatAction;
}

/**
 *  initSlash - initialize a slash object
 * 
 * @param ID Player's ID
 * @param bulletInfo Slash's information from the used melee weapon
 * 
 * @return Pointer to the combat action object
 * 
 * @details Initialize a slash object and link it to the player
 * by `ID`, its information is provided by the used melee weapon.
 *
 */
CombatAction *initSlash(int ID, SlashInfo slashInfo, Vector2 src, Vector2 dest) {
  if (gameState->numOfCombatActions == DEFAULT_MAX_COMBAT_ACTIONS) return NULL;
  // Init slash
  Slash slash;
  slash.playerID = ID;
  slash.slashInfo = slashInfo;
  slash.transform = (CTransform){src, 0, (Vector2){1, 1}};

  // Init combatAction
  CombatAction *combatAction = &(gameState->combatActions[gameState->numOfCombatActions++]);
  combatAction->angle = GetAngleBetweenPoints(src, dest);
  printf("%f\n", combatAction->angle);
  combatAction->type = ACTION_SLASH;
  combatAction->action.slash = slash;
  return combatAction;
}

/**
 * drawCombatActions - draw all combat actions and update them
 */
void drawCombatActions() {
  int x = 320, y = 96;
  int actions = gameState->numOfCombatActions;
  CombatAction *combatActions = gameState->combatActions;

  SpriteAnimation fireAnime = createSpriteAnimation(6,
                                                    (char *[]){
                                                        "fire_1_0_0",
                                                        "fire_1_0_1",
                                                        "fire_1_0_2",
                                                        "fire_1_0_3",
                                                        "fire_1_0_4",
                                                        "fire_1_0_5",
                                                    },
                                                    12, true);

  while (actions--) {
    if (combatActions->type == ACTION_BULLET) {
      combatActions->anime = fireAnime;
      drawBullet(&combatActions);
    }
    combatActions++;
  }
  disposeSpriteAnimation(&fireAnime);
}

/**
 * clearCombatActions - free combat actions array from heap
 */
void clearCombatActions() {
  printf("Deleting combat actions\n");
  int combatActionNum = gameState->numOfCombatActions;
  CombatAction *combatActions = gameState->combatActions;

  while (combatActionNum--) {
    clearCombatAction(&combatActions);
    combatActions++;
  }
  printf("Deleted combat actions\n");
}

// *****************
// PRIVATE FUNCTIONS
// *****************

/**
 * checkCollision - check for collision
 * 
 * @param rect1 combat action boundary box
 * @param rect2 enemy boundary box
 * 
 * @return true if the combat action collides, false otherwise.
 * 
 */
static bool checkCollision(Rectangle rect1, Rectangle rect2) {
  // collision x-axis?
  bool collisionX =
      rect1.x + rect1.width >= rect2.x && rect2.x + rect2.width >= rect1.x;
  // collision y-axis?
  bool collisionY =
      rect1.y + rect1.height >= rect2.y && rect2.y + rect2.height >= rect1.y;
  // collision only if on both axes
  return collisionX && collisionY;
}

/**
 * bulletCollision - check for bullet collision with the borders and enemies
 * 
 * @param combatAction pointer to the combat action object holding the bullet
 * 
 * @return true if the bullet collides, false otherwise.
 * 
 */
static bool bulletCollision(CombatAction *combatAction) {
  Bullet *bullet = &combatAction->action.bullet;
  for (int j = 0; j < gameState->numOfEnemies; j++) {
    Enemy *enemy = &gameState->enemies[j];
    Vector2 bulletPosition = bullet->transform.position;
    Vector2 enemyPosition = enemy->object.transform.position;
    Collider2D bulletCollider = bullet->bulletInfo.collider;
    Collider2D enemyCollider = enemy->object.collider;

    if (checkCollision((Rectangle){bulletPosition.x,
                                   bulletPosition.y,
                                   bulletCollider.width,
                                   bulletCollider.height},
                       (Rectangle){enemyPosition.x,
                                   enemyPosition.y,
                                   enemyCollider.width,
                                   enemyCollider.height})) {
      // Clear combatAction
      // TODO: decrease the bullet's health and indicate that it is currently
      // on a certain enemy (store the enemy id?) so that it affect different enemies.
      *combatAction =
          gameState->combatActions[--(gameState->numOfCombatActions)];
      // Damage Enemy
      enemy->stats.health.currentHealth -= bullet->bulletInfo.bulletDamage;
      if (enemy->stats.health.currentHealth <= 0) {
        gameState->enemies[j] = gameState->enemies[gameState->numOfEnemies - 1];
        gameState->numOfEnemies--;
        // TODO: add score to the player (maybe each enemy has its own score).
      }
      return true;
    }
  }
  return false;
}

/**
 * slashCollision - check for slash collision with the borders and enemies
 * 
 * @param combatAction pointer to the combat action object holding the slash
 * 
 * @return true if the slash collides, false otherwise.
 * 
 */
static bool slashCollision(CombatAction *combatAction) {
  Slash *slash = &combatAction->action.slash;
  for (int j = 0; j < gameState->numOfEnemies; j++) {
    Enemy *enemy = &gameState->enemies[j];
    Vector2 slashPosition = slash->transform.position;
    Vector2 enemyPosition = enemy->object.transform.position;
    Collider2D slashCollider = slash->slashInfo.collider;
    Collider2D enemyCollider = enemy->object.collider;

    if (checkCollision((Rectangle){slashPosition.x,
                                   slashPosition.y,
                                   slashCollider.width,
                                   slashCollider.height},
                       (Rectangle){enemyPosition.x,
                                   enemyPosition.y,
                                   enemyCollider.width,
                                   enemyCollider.height})) {
      // Clear combatAction
      // TODO: decrease the bullet's health and indicate that it is currently
      // on a certain enemy (store the enemy id?) so that it affect different enemies.
      *combatAction =
          gameState->combatActions[--(gameState->numOfCombatActions)];
      // Damage Enemy
      enemy->stats.health.currentHealth -= slash->slashInfo.slashDamage;
      if (enemy->stats.health.currentHealth <= 0) {
        gameState->enemies[j] = gameState->enemies[gameState->numOfEnemies - 1];
        gameState->numOfEnemies--;
        // TODO: add score to the player (maybe each enemy has its own score).
      }
      return true;
    }
  }
  return false;
}

static void drawBullet(CombatAction **combatActions) {
  CombatAction *combatAction = *combatActions;
  Bullet *bullet = &(combatAction->action.bullet);
  Vector2 *pos = &(bullet->transform.position);
  Rectangle dest = (Rectangle){pos->x, pos->y, bullet->bulletInfo.collider.width,
                    bullet->bulletInfo.collider.height};
  drawSpriteAnimationPro(&(combatAction->anime), dest, (Vector2){0, 0}, 0, WHITE,
                         false);
  pos->x +=
      bullet->bulletInfo.bulletSpeed * cos(combatAction->angle * DEG2RAD);
  pos->y +=
      bullet->bulletInfo.bulletSpeed * sin(combatAction->angle * DEG2RAD);

  (*combatActions) -= bulletCollision(combatAction);
}

static void drawSlash(CombatAction **combatActions) {
  CombatAction *combatAction = *combatActions;
  Slash *slash = &combatAction->action.slash;
  Vector2 *pos = &slash->transform.position;
  Rectangle dest = {pos->x, pos->y, slash->slashInfo.collider.width,
                    slash->slashInfo.collider.height};
  drawSpriteAnimationPro(&(combatAction->anime), dest, (Vector2){0, 0}, 0, WHITE,
                         false);
  combatAction->angle++;
  (*combatActions) -= slashCollision(combatAction);
}

static void clearCombatAction(CombatAction **combatAction) {
  if (combatAction == NULL || *combatAction == NULL)
    return;

  free(*combatAction);
  *combatAction = NULL;
}
