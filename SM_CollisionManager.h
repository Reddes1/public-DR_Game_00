#pragma once
//#include "EM_EntityInterface.h"	//Parent
#include "EntityPool.h"	//Pooling System
#include "SM_EntityManager.h"
#include "Sm_DataManager.h"		//Map Collision Data Types
/*
	Collection of collision systems for the MANY collisions in game.
	Manages different collision types for all collision objects.
*/
class SM_CollisionManager
{
public:

	///////////////////
	/// Definitions ///
	///////////////////


	////////////
	/// Data ///
	////////////


	////////////////////
	/// Constructors ///
	////////////////////

	SM_CollisionManager() { Init(); }
	~SM_CollisionManager() { Release(); }


	//////////////////
	/// Operations ///
	//////////////////

	void Update(GameTimer& gt);


private:

	//////////////////
	/// Operations ///
	//////////////////

	//Basic Init Function
	void Init();
	//Basic Release Function
	void Release();

	//
	//Collision Passes
	//

	//Single Pool (Internal Collision Only)
	bool SinglePoolCollision(EntityPool<EM_EntityInterface2D*>& pool);
	//EnemyPool_01 > EnemyPool_00
	bool EP00_EP01_Collision(EntityPool<EM_EntityInterface2D*>& pool00, EntityPool<EM_EntityInterface2D*>& pool01);
	//PlayerProj > EnemyPool_00/01
	bool PlayerProj_Enemy_Collision(EntityPool<Projectile_Interface*>& proj00, EntityPool<EM_EntityInterface2D*>& pool00);
	//Player > EnemyPool_00
	bool Player_EP00_Collision(Entity_Player& player, EntityPool<EM_EntityInterface2D*>& pool);
	//Player > EnemyPool_01 (Currently the same as above, but may be expanded later based on enemy time)
	bool Player_EP01_Collision(Entity_Player& player, EntityPool<EM_EntityInterface2D*>& pool);
	//Player Proj > Enemy Proj (NYI)
	bool PlayerProj_EnemyProj_Collision(EntityPool<Projectile_Interface*>& proj00, EntityPool<Projectile_Interface*>& proj01);
	//Player < Enemy Projectile (NYI)
	bool Player_EnemyProj_Collision(Entity_Player& player, EntityPool<Projectile_Interface*>& proj00);
	//Player > Terrain Collision (NYI)
	bool Player_Terrain_Collision(Entity_Player& player, unsigned terrainIndex);
	//Player > Loot Collision (NYI)
	bool Player_Loot_Collision(Entity_Player& player, EntityPool<Loot_Interface*>& loot00);
	//Terrain > Loot Collision (NYI)
	bool Terrain_Loot_Collision(unsigned terrainIndex, EntityPool<Loot_Interface*>& loot00);
	//
	//Collision Resolution
	//

	//Enemy

	//Enemy > Other Enemy
	void EnemyToEnemyResolution(EM_EntityInterface2D* ent00, EM_EntityInterface2D* ent01);
	//Enemy > Player
	void EnemyToPlayerResolution(EM_EntityInterface2D* ent00, Entity_Player* player);

	//Player

	//Player Proj > Enemy
	void PlayerProjToEnemyResolution(Projectile_Interface* proj, EM_EntityInterface2D* ent00);

	//Player < Enemy Projectiles (NYI)
	void PlayerToEnemyProjResolution(Entity_Player* player, Projectile_Interface* proj);

	void PlayerProjToEnemyProjResolution(Projectile_Interface* proj00, Projectile_Interface* proj01);

	//Player > Terrain
	void PlayerToTerrainResolution(CollisionArea terrain, Entity_Player* player);
};
