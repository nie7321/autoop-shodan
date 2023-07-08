#ifndef TESTENTITY_H
#define TESTENTITY_H

class CAutoOPGun: public CBaseEntity {
 public:    
  
  void Spawn( void );
  void Precache(void);

  void KeyValue( KeyValueData *pkvd );
  int  Save( CSave &save ){ return -1;} 
  int  Restore( CRestore &restore ) {return -1;}    

  int  ObjectCaps( void ) { return FCAP_ACROSS_TRANSITION; }    
  void Activate( void ) {}
  void SetObjectCollisionBox( void ){}
  int  Classify ( void ) { return CLASS_NONE; };      
  void DeathNotice ( entvars_t *pevChild ) {}// monster maker children use 
  void TraceAttack( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType) {}
  int  TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType ){return -1;}
  int  TakeHealth( float flHealth, int bitsDamageType ){return -1;}
  void Killed( entvars_t *pevAttacker, int iGib ){}
  int  BloodColor( void ) { return DONT_BLEED; }
  void TraceBleed( float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType ) {}
  BOOL IsTriggered( CBaseEntity *pActivator ) {return TRUE;}
  int   GetToggleState( void ) { return TS_AT_TOP; }
  void  AddPoints( int score, BOOL bAllowNegativeScore ) {}
  void  AddPointsToTeam( int score, BOOL bAllowNegativeScore ) {}
  BOOL  AddPlayerItem( CBasePlayerItem *pItem ) { return 0; }
  BOOL  RemovePlayerItem( CBasePlayerItem *pItem ) { return 0; }
  int   GiveAmmo( int iAmount, char *szName, int iMax ) { return -1; };
  float GetDelay( void ) { return 0; }
  int   IsMoving( void ) { return pev->velocity != g_vecZero; }
  void  OverrideReset( void ) {}
  int   DamageDecal( int bitsDamageType ) {return -1;}
  // This is ONLY used by the node graph to test movement through a door
  void  SetToggleState( int state ) {}
  void  StartSneaking( void ) {}
  void  StopSneaking( void ) {}
  BOOL  OnControls( entvars_t *pev ) { return FALSE; }
  BOOL  IsSneaking( void ) { return FALSE; }
  BOOL  IsAlive( void ) { return 0; }
  BOOL  IsBSPModel( void ) { return 0; }
  BOOL  ReflectGauss( void ) { return ( 0 ); }
  BOOL  HasTarget( string_t targetname ) { return 0 ; }
  BOOL  IsInWorld( void ) {return FALSE;}
  BOOL  IsPlayer( void ) { return FALSE; }
  BOOL  IsNetClient( void ) { return FALSE; }
  const char *TeamID( void ) { return ""; }
  CBaseEntity *GetNextTarget( void ) { return this;}

  void Think(void);

  void Touch( CBaseEntity *pOther ) {  };
  void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
  {
  }
  void Blocked( CBaseEntity *pOther ) { };
  CBaseEntity *Respawn( void ) { return this; }
  void UpdateOwner( void ) { return; };    
  BOOL FBecomeProne( void ) {return FALSE;};
  Vector Center( ) { return (pev->absmax + pev->absmin) * 0.5; }; // center point of entity
  Vector EyePosition( ) { return pev->origin + pev->view_ofs; };                  // position of eyes
  Vector EarPosition( ) { return pev->origin + pev->view_ofs; };                  // position of ears
  Vector BodyTarget( const Vector &posSrc ) { return Center( ); };                // position to shoot at

  int Illumination( ) { return 0; };
  BOOL FVisible ( CBaseEntity *pEntity ) { return FALSE;}
  BOOL FVisible ( const Vector &vecOrigin )  { return FALSE;}
  static CBaseEntity *Create( char *szName, const Vector &vecOrigin, const Vector &vecAngles, edict_t *pentOwner = NULL );
  void    MakeDormant( void ) {};
 
 private:
  int m_iGunType;
  int m_fRespawnTime;
};

#endif //TESTENTITY_H