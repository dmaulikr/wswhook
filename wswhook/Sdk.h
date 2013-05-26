#pragma once

#define _L

typedef enum
{
	WEAP_NONE = 0,
	WEAP_GUNBLADE,
	WEAP_MACHINEGUN,
	WEAP_RIOTGUN,
	WEAP_GRENADELAUNCHER,
	WEAP_ROCKETLAUNCHER,
	WEAP_PLASMAGUN,
	WEAP_LASERGUN,
	WEAP_ELECTROBOLT,
	WEAP_INSTAGUN,

	WEAP_TOTAL
} weapon_tag_t;

#define AngMod180(angle) ((360.0 / 0x80000000)*((unsigned int)(((angle) + 180) * (0x80000000 / 360.0)) & 0x7FFFFFFF) - 180)

#define VectorSet( v, x, y, z )	  ( ( v )[0] = ( x ), ( v )[1] = ( y ), ( v )[2] = ( z ) )
#define DotProduct(x,y)			((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])
#define VectorSubtract(a,b,c)	((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define VectorAdd(a,b,c)		((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1],(c)[2]=(a)[2]+(b)[2])
#define VectorAdd2(v,x,y,z)		((v)[0]+=x,(v)[1]+=y,(v)[2]+=z)
#define VectorCopy(a,b)			((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define	VectorScale(v, s, o)	((o)[0]=(v)[0]*(s),(o)[1]=(v)[1]*(s),(o)[2]=(v)[2]*(s))
#define	VectorMA(v, s, b, o)	((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))

#define	CONTENTS_SOLID			1			// an eye is never valid in a solid
#define	CONTENTS_LAVA			8
#define	CONTENTS_SLIME			16
#define	CONTENTS_WATER			32
#define	CONTENTS_FOG			64

#define	CONTENTS_AREAPORTAL		0x8000

#define	CONTENTS_PLAYERCLIP		0x10000
#define	CONTENTS_MONSTERCLIP	0x20000

#define	CONTENTS_TELEPORTER		0x40000
#define	CONTENTS_JUMPPAD		0x80000
#define CONTENTS_CLUSTERPORTAL	0x100000
#define CONTENTS_DONOTENTER		0x200000

#define	CONTENTS_ORIGIN			0x1000000	// removed before bsping an entity

#define	CONTENTS_BODY			0x2000000	// should never be on a brush, only in game
#define	CONTENTS_CORPSE			0x4000000
#define	CONTENTS_DETAIL			0x8000000	// brushes not used for the bsp
#define	CONTENTS_STRUCTURAL		0x10000000	// brushes used for the bsp
#define	CONTENTS_TRANSLUCENT	0x20000000	// don't consume surface fragments inside
#define	CONTENTS_TRIGGER		0x40000000
#define	CONTENTS_NODROP			0x80000000	// don't leave bodies or items (death fog, lava)

#define	MASK_ALL			( -1 )
#define	MASK_SOLID			( CONTENTS_SOLID )
#define	MASK_PLAYERSOLID	( CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_BODY )
#define	MASK_DEADSOLID		( CONTENTS_SOLID|CONTENTS_PLAYERCLIP )
#define	MASK_MONSTERSOLID	( CONTENTS_SOLID|CONTENTS_MONSTERCLIP|CONTENTS_BODY )
#define	MASK_WATER			( CONTENTS_WATER|CONTENTS_LAVA|CONTENTS_SLIME )
#define	MASK_OPAQUE			( CONTENTS_SOLID|CONTENTS_SLIME|CONTENTS_LAVA )
#define	MASK_SHOT			( CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_CORPSE )

typedef enum { qfalse, qtrue }	  qboolean;
typedef unsigned char qbyte;

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];

#define MAX_ITEMS					64	
#define	PS_MAX_STATS				64

#define	CS_MODELS			32

#define	MAX_CLIENTS					256			// absolute limit
#define	MAX_EDICTS					1024		// must change protocol to increase more
#define	MAX_LIGHTSTYLES				256
#define	MAX_MODELS					256			// these are sent over the net as bytes
#define	MAX_SOUNDS					256			// so they cannot be blindly increased
#define	MAX_IMAGES					256
#define MAX_SKINFILES				256
#define MAX_ITEMS					64			// 16x4
#define MAX_GENERAL					( MAX_CLIENTS )	// general config strings
#define CS_WORLDMODEL		30
#define	MAX_QPATH					64			// max length of a quake game pathname

#define	MAX_STRING_CHARS			1024		// max length of a string passed to Cmd_TokenizeString
#define	MAX_STRING_TOKENS			256			// max tokens resulting from Cmd_TokenizeString
#define	MAX_TOKEN_CHARS				1024		// max length of an individual token
#define MAX_CONFIGSTRING_CHARS		MAX_QPATH	// max length of a configstring string

#define MAX_NAME_BYTES				32			// max length of a player name, including trailing \0
#define MAX_NAME_CHARS				15			// max visible characters in a name (color tokens and \0 not counted)

#define MAX_GAMECOMMANDS	64		// command names for command completion
#define MAX_LOCATIONS		64
#define MAX_WEAPONDEFS		64

//precache stuff begins here
#define	CS_MODELS			32
#define	CS_SOUNDS			( CS_MODELS+MAX_MODELS )
#define	CS_IMAGES			( CS_SOUNDS+MAX_SOUNDS )
#define	CS_SKINFILES		( CS_IMAGES+MAX_IMAGES )
#define	CS_LIGHTS			( CS_SKINFILES+MAX_SKINFILES )
#define	CS_ITEMS			( CS_LIGHTS+MAX_LIGHTSTYLES )
#define	CS_PLAYERINFOS		( CS_ITEMS+MAX_ITEMS )
#define CS_GAMECOMMANDS		( CS_PLAYERINFOS+MAX_CLIENTS )
#define CS_LOCATIONS		( CS_GAMECOMMANDS+MAX_GAMECOMMANDS )
#define CS_WEAPONDEFS		( CS_LOCATIONS+MAX_LOCATIONS )
#define CS_GENERAL			( CS_WEAPONDEFS+MAX_WEAPONDEFS )

#define	MAX_CONFIGSTRINGS	( CS_GENERAL+MAX_GENERAL )

typedef qbyte byte_vec4_t[4];

typedef struct cplane_s
{
	vec3_t normal;
	float dist;
	short type;					// for fast side tests
	short signbits;				// signx + (signy<<1) + (signz<<1)
} cplane_t;

#define PM_VECTOR_SNAP 16
#define MAX_PM_STATS 16

enum
{
	PM_STAT_FEATURES,
	PM_STAT_NOUSERCONTROL,
	PM_STAT_KNOCKBACK,
	PM_STAT_CROUCHTIME,
	PM_STAT_ZOOMTIME,
	PM_STAT_DASHTIME,
	PM_STAT_WJTIME,
	PM_STAT_NOAUTOATTACK,
	PM_STAT_STUN,
	PM_STAT_MAXSPEED,
	PM_STAT_JUMPSPEED,
	PM_STAT_DASHSPEED,
	PM_STAT_FWDTIME,

	PM_STAT_SIZE = MAX_PM_STATS
};

typedef struct entity_state_s
{
	int number;							// edict index

	unsigned int svflags;

	int type;							// ET_GENERIC, ET_BEAM, etc
	qboolean linearProjectile;			// is sent inside "type" as ET_INVERSE flag
	vec3_t linearProjectileVelocity;	// this is transmitted instead of origin when linearProjectile is true

	vec3_t origin;
	vec3_t angles;

	union
	{
		vec3_t old_origin;				// for lerping
		vec3_t origin2;					// ET_BEAM, ET_PORTALSURFACE, ET_EVENT specific
	};

	unsigned int modelindex;
	union
	{
		unsigned int modelindex2;
		int bodyOwner;					// ET_PLAYER specific, for dead bodies
		int channel;					// ET_SOUNDEVENT
	};

	union
	{
		int frame;
		int ownerNum;					// ET_EVENT specific
	};

	union
	{
		int counterNum;					// ET_GENERIC
		int skinnum;					// for ET_PLAYER
		int itemNum;					// for ET_ITEM
		int firemode;					// for weapon events
		int damage;						// EV_BLOOD
		int targetNum;					// ET_EVENT specific
		int colorRGBA;					// ET_BEAM, ET_EVENT specific
		int range;						// ET_LASERBEAM, ET_CURVELASERBEAM specific
		int attenuation;				// ET_SOUNDEVENT
	};

	int weapon;							// WEAP_ for players
	qboolean teleported;				// the entity was teleported this snap (sent inside "weapon" as ET_INVERSE flag)

	unsigned int effects;

	union
	{
		// for client side prediction, 8*(bits 0-4) is x/y radius
		// 8*(bits 5-9) is z down distance, 8(bits10-15) is z up
		// GClip_LinkEntity sets this properly
		int solid;	
		int eventCount;					// ET_EVENT specific
	};

	int sound;							// for looping sounds, to guarantee shutoff

	// impulse events -- muzzle flashes, footsteps, etc
	// events only go out for a single frame, they
	// are automatically cleared each frame
	int events[2];
	int eventParms[2];

	union
	{
		unsigned int linearProjectileTimeStamp;
		int light;						// constant light glow
	};

	int team;							// team in the game
} entity_state_t;

typedef struct
{
	qboolean allsolid;			// if true, plane is not valid
	qboolean startsolid;		// if true, the initial point was in a solid area
	float fraction;				// time completed, 1.0 = didn't hit anything
	vec3_t endpos;				// final position
	cplane_t plane;				// surface normal at impact
	int surfFlags;				// surface hit
	int contents;				// contents on other side of surface hit
	int ent;					// not set by CM_*() functions
} trace_t;

typedef struct
{
	int pm_type;

	float origin[3];			// 12.3
	float velocity[3];			// 12.3

	int pm_flags;				// ducked, jump_held, etc
	int pm_time;				// each unit = 8 ms
	short stats[PM_STAT_SIZE];	// Kurim : timers for knockback, stun, doublejump, walljump
	int gravity;
	short delta_angles[3];		// add to command angles to get view direction
								// changed by spawns, rotating objects, and teleporters
} pmove_state_t;

typedef struct
{
	pmove_state_t pmove;		// for prediction

	// these fields do not need to be communicated bit-precise

	vec3_t viewangles;			// for fixed views

	int event[2], eventParm[2];
	unsigned int POVnum;		// entity number of the player in POV
	unsigned int playerNum;		// client number
	float viewheight;
	float fov;					// horizontal field of view

	qbyte weaponState;

	int inventory[MAX_ITEMS];
	short stats[PS_MAX_STATS];	// fast status bar updates
	qbyte plrkeys;				// infos on the pressed keys of chased player (self if not chasing)
} player_state_t;

#define	MAX_GAME_STATS	16
#define MAX_GAME_LONGSTATS 8

typedef struct
{
	qboolean all;
	qbyte targets[MAX_CLIENTS/8];
	size_t commandOffset;			// offset of the data in gamecommandsData
} gcommand_t;

typedef struct
{
	short stats[MAX_GAME_STATS];
	unsigned int longstats[MAX_GAME_LONGSTATS];
} game_state_t;

#define	MAX_PARSE_GAMECOMMANDS	64
#define	MAX_PARSE_ENTITIES	1024
#define	MAX_STRING_CHARS			1024	

typedef struct snapshot_s
{
	qboolean valid;             // cleared if delta parsing was invalid
	int serverFrame;
	unsigned int serverTime;    // time in the server when frame was created
	unsigned int ucmdExecuted;
	qboolean delta;
	qboolean allentities;
	qboolean multipov;
	int deltaFrameNum;
	size_t areabytes;
	qbyte *areabits;             // portalarea visibility bits
	int numplayers;
	player_state_t playerState;
	player_state_t playerStates[MAX_CLIENTS];
	int numEntities;
	entity_state_t parsedEntities[MAX_PARSE_ENTITIES];
	game_state_t gameState;
	int numgamecommands;
	gcommand_t gamecommands[MAX_PARSE_GAMECOMMANDS];
	char gamecommandsData[MAX_STRING_CHARS * ( MAX_PARSE_GAMECOMMANDS / 4 )];
	size_t gamecommandsDataHead;
} snapshot_t;
 
typedef struct
{
	// if API is different, the dll cannot be used
	int ( *API )( void );

	// the init function will be called at each restart
	void ( *Init )( const char *serverName, unsigned int playerNum, int vidWidth, int vidHeight, 
		qboolean demoplaying, const char *demoName, qboolean pure, unsigned int snapFrameTime, 
		int protocol, int sharedSeed );

	// "soft restarts" at demo jumps
	void ( *Reset )( void );

	void ( *Shutdown )( void );

	void ( *ConfigString )( int number, const char *value );

	void ( *EscapeKey )( void );

	void ( *GetEntitySpatilization )( int entNum, vec3_t origin, vec3_t velocity );

	float ( *SetSensitivityScale )( const float sens );

	void ( *Trace )( trace_t *tr, vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int passent, int contentmask );

	void ( *RenderView )( float frameTime, float realFrameTime, int realTime, unsigned int serverTime, float stereo_separation, unsigned int extrapolationTime );

	void ( *NewFrameSnapshot )( snapshot_t *newSnapshot, snapshot_t *currentSnapshot );
} cgame_export_t;

typedef struct usercmd_s
{
	qbyte msec;
	qbyte buttons;
	short angles[3];
	float forwardfrac, sidefrac, upfrac;
	short forwardmove, sidemove, upmove;
	unsigned int serverTimeStamp;
} usercmd_t;

typedef int cvar_flag_t;

typedef struct cvar_s
{
	char *name;
	char *string;
	char *dvalue;
	char *latched_string;       // for CVAR_LATCH vars
	cvar_flag_t flags;
	qboolean modified;          // set each time the cvar is changed
	float value;
	int integer;
} cvar_t;

struct dynvar_s;
typedef struct dynvar_s dynvar_t;

typedef enum dynvar_get_status_t
{
	DYNVAR_GET_OK,
	DYNVAR_GET_WRITEONLY,
	DYNVAR_GET_TRANSIENT
} dynvar_get_status_t;

typedef enum dynvar_set_status_t
{
	DYNVAR_SET_OK,
	DYNVAR_SET_READONLY,
	DYNVAR_SET_INVALID,
	DYNVAR_SET_TRANSIENT
} dynvar_set_status_t;

// getter function signature
// getter writes current value to *val (if not WRITEONLY) and returns status
typedef dynvar_get_status_t ( *dynvar_getter_f )( void **val );

// setter function signature
// setter reads new value from val (if not READONLY) and returns status
typedef dynvar_set_status_t ( *dynvar_setter_f )( void *val );

// listener function signature
// called by Dynvar_SetValue, after the setter function returned with DYNVAR_SET_OK
typedef void ( *dynvar_listener_f )( void *val );

typedef struct
{
	// drops to console a client game error
	void ( *Error )( const char *msg );

	// console messages
	void ( *Print )( const char *msg );
	void ( *PrintToLog )( const char *msg );

	// dynvars
	dynvar_t *( *Dynvar_Create )( const char *name, qboolean console, dynvar_getter_f getter, dynvar_setter_f setter );
	void ( *Dynvar_Destroy )( dynvar_t *dynvar );
	dynvar_t *( *Dynvar_Lookup )( const char *name );
	const char *( *Dynvar_GetName )( dynvar_t *dynvar );
	dynvar_get_status_t ( *Dynvar_GetValue )( dynvar_t *dynvar, void **value );
	dynvar_set_status_t ( *Dynvar_SetValue )( dynvar_t *dynvar, void *value );
	void ( *Dynvar_AddListener )( dynvar_t *dynvar, dynvar_listener_f listener );
	void ( *Dynvar_RemoveListener )( dynvar_t *dynvar, dynvar_listener_f listener );

	// console variable interaction
	cvar_t *( *Cvar_Get )( const char *name, const char *value, int flags );
	cvar_t *( *Cvar_Set )( const char *name, const char *value );
	void ( *Cvar_SetValue )( const char *name, float value );
	cvar_t *( *Cvar_ForceSet )( const char *name, const char *value );      // will return 0 0 if not found
	float ( *Cvar_Value )( const char *name );
	const char *( *Cvar_String )( const char *name );

	void ( *Cmd_TokenizeString )( const char *text );
	int ( *Cmd_Argc )( void );
	char *( *Cmd_Argv )( int arg );
	char *( *Cmd_Args )( void );        // concatenation of all argv >= 1

	void ( *Cmd_AddCommand )( const char *name, void ( *cmd )( void ) );
	void ( *Cmd_RemoveCommand )( const char *cmd_name );
	void ( *Cmd_ExecuteText )( int exec_when, const char *text );
	void ( *Cmd_Execute )( void );
	void ( *Cmd_SetCompletionFunc )( const char *cmd_name, char **( *completion_func )( const char *partial ) );

	// files will be memory mapped read only
	// the returned buffer may be part of a larger pak file,
	// or a discrete file from anywhere in the quake search path
	// a -1 return means the file does not exist
	// NULL can be passed for buf to just determine existance
	int ( *FS_FOpenFile )( const char *filename, int *filenum, int mode );
	int ( *FS_Read )( void *buffer, size_t len, int file );
	int ( *FS_Write )( const void *buffer, size_t len, int file );
	int ( *FS_Print )( int file, const char *msg );
	int ( *FS_Tell )( int file );
	int ( *FS_Seek )( int file, int offset, int whence );
	int ( *FS_Eof )( int file );
	int ( *FS_Flush )( int file );
	void ( *FS_FCloseFile )( int file );
	qboolean ( *FS_RemoveFile )( const char *filename );
	int ( *FS_GetFileList )( const char *dir, const char *extension, char *buf, size_t bufsize, int start, int end );
	const char *( *FS_FirstExtension )( const char *filename, const char *extensions[], int num_extensions );
	qboolean ( *FS_IsPureFile )( const char *filename );
	qboolean ( *FS_MoveFile )( const char *src, const char *dst );
	qboolean ( *FS_IsUrl )( const char *url );
	time_t ( *FS_FileMTime )( const char *filename );
	qboolean ( *FS_RemoveDirectory )( const char *dirname );

	// key bindings
	const char *( *Key_GetBindingBuf )( int binding );
	const char *( *Key_KeynumToString )( int keynum );

	void ( *GetConfigString )( int i, char *str, int size );
	unsigned int ( *Milliseconds )( void );
	qboolean ( *DownloadRequest )( const char *filename, qboolean requestpak );

	unsigned int (* Hash_BlockChecksum )( const qbyte * data, size_t len );
	unsigned int (* Hash_SuperFastHash )( const qbyte * data, size_t len, unsigned int seed );

	void ( *NET_GetUserCmd )( int frame, usercmd_t *cmd );
	int ( *NET_GetCurrentUserCmdNum )( void );
	void ( *NET_GetCurrentState )( int *incomingAcknowledged, int *outgoingSequence, int *outgoingSent );
	void ( *RefreshMouseAngles )( void );

	// refresh system
	void ( *R_UpdateScreen )( void );
	int ( *R_GetClippedFragments )( const vec3_t origin, float radius, vec3_t axis[3], int maxfverts, vec3_t *fverts, int maxfragments, struct fragment_s *fragments );
	void ( *R_ClearScene )( void );
	void ( *R_AddEntityToScene )( const struct entity_s *ent );
	void ( *R_AddLightToScene )( const vec3_t org, float intensity, float r, float g, float b, const struct shader_s *shader );
	void ( *R_AddPolyToScene )( const struct poly_s *poly );
	void ( *R_AddLightStyleToScene )( int style, float r, float g, float b );
	void ( *R_RenderScene )( const struct refdef_s *fd );
	const char *( *R_SpeedsMessage )( char *out, size_t size );
	void ( *R_RegisterWorldModel )( const char *name );
	void ( *R_ModelBounds )( const struct model_s *mod, vec3_t mins, vec3_t maxs );
	void ( *R_ModelFrameBounds )( const struct model_s *mod, int frame, vec3_t mins, vec3_t maxs );
	struct model_s *( *R_RegisterModel )( const char *name );
	struct shader_s *( *R_RegisterPic )( const char *name );
	struct shader_s *( *R_RegisterRawPic )( const char *name, int width, int height, qbyte *data );
	struct shader_s *( *R_RegisterLevelshot )( const char *name, struct shader_s *defaultPic, qboolean *matchesDefault );
	struct shader_s *( *R_RegisterSkin )( const char *name );
	struct skinfile_s *( *R_RegisterSkinFile )( const char *name );
	struct shader_s *( *R_RegisterVideo )( const char *name );
	qboolean ( *R_LerpTag )( struct orientation_s *orient, const struct model_s *mod, int oldframe, int frame, float lerpfrac, const char *name );
	void ( *R_SetCustomColor )( int num, int r, int g, int b );
	void ( *R_LightForOrigin )( const vec3_t origin, vec3_t dir, vec4_t ambient, vec4_t diffuse, float radius );
	void ( *R_DrawStretchPic )( int x, int y, int w, int h, float s1, float t1, float s2, float t2, const vec4_t color, const struct shader_s *shader );
	void ( *R_DrawStretchPoly )( const struct poly_s *poly, float x_offset, float y_offset );
	void ( *R_DrawRotatedStretchPic )( int x, int y, int w, int h, float s1, float t1, float s2, float t2, float angle, const vec4_t color, const struct shader_s *shader );
	void ( *R_SetScissorRegion )( int x, int y, int w, int h );
	void ( *R_GetShaderDimensions )( const struct shader_s *shader, int *width, int *height, int *depth );
	void ( *R_TransformVectorToScreen )( const struct refdef_s *rd, const vec3_t in, vec2_t out );
	int ( *R_SkeletalGetNumBones )( const struct model_s *mod, int *numFrames );
	int ( *R_SkeletalGetBoneInfo )( const struct model_s *mod, int bone, char *name, size_t name_size, int *flags );
	void ( *R_SkeletalGetBonePose )( const struct model_s *mod, int bone, int frame, struct bonepose_s *bonepose );

	void ( *VID_FlashWindow )( int count );

	// collision detection
	int ( *CM_NumInlineModels )( void );
	struct cmodel_s	*( *CM_InlineModel )( int num );
	struct cmodel_s	*( *CM_ModelForBBox )( vec3_t mins, vec3_t maxs );
	struct cmodel_s	*( *CM_OctagonModelForBBox )( vec3_t mins, vec3_t maxs );
	void ( *CM_TransformedBoxTrace )( trace_t *tr, vec3_t start, vec3_t end, vec3_t mins, vec3_t maxs, struct cmodel_s *cmodel, int brushmask, vec3_t origin, vec3_t angles );
	int ( *CM_TransformedPointContents )( vec3_t p, struct cmodel_s *cmodel, vec3_t origin, vec3_t angles );
	void ( *CM_RoundUpToHullSize )( vec3_t mins, vec3_t maxs, struct cmodel_s *cmodel );
	void ( *CM_InlineModelBounds )( struct cmodel_s *cmodel, vec3_t mins, vec3_t maxs );

	// sound system
	struct sfx_s *( *S_RegisterSound )( const char *name );
	void ( *S_StartFixedSound )( struct sfx_s *sfx, const vec3_t origin, int entchannel, float fvol, float attenuation );
	void ( *S_StartRelativeSound )( struct sfx_s *sfx, int entnum, int entchannel, float fvol, float attenuation );
	void ( *S_StartGlobalSound )( struct sfx_s *sfx, int entchannel, float fvol );
	void ( *S_Update )( const vec3_t origin, const vec3_t velocity, const vec3_t v_forward, const vec3_t v_right, const vec3_t v_up, const char *identity );
	void ( *S_AddLoopSound )( struct sfx_s *sfx, int entnum, float fvol, float attenuation );
	void ( *S_StartBackgroundTrack )( const char *intro, const char *loop );
	void ( *S_StopBackgroundTrack )( void );

	// fonts
	struct mufont_s *( *SCR_RegisterFont )( const char *name );
	void ( *SCR_DrawString )( int x, int y, int align, const char *str, struct mufont_s *font, vec4_t color );
	int ( *SCR_DrawStringWidth )( int x, int y, int align, const char *str, int maxwidth, struct mufont_s *font, vec4_t color );
	void ( *SCR_DrawClampString )( int x, int y, const char *str, int xmin, int ymin, int xmax, int ymax, struct mufont_s *font, vec4_t color );
	size_t ( *SCR_strHeight )( struct mufont_s *font );
	size_t ( *SCR_strWidth )( const char *str, struct mufont_s *font, int maxlen );
	size_t ( *SCR_StrlenForWidth )( const char *str, struct mufont_s *font, size_t maxwidth );

	// managed memory allocation
	void *( *Mem_Alloc )( size_t size, const char *filename, int fileline );
	void ( *Mem_Free )( void *data, const char *filename, int fileline );
} cgame_import_t;

typedef enum
{
	IT_WEAPON = 1
	, IT_AMMO = 2
	, IT_ARMOR = 4
	, IT_POWERUP = 8
	, IT_HEALTH = 64
} itemtype_t;

#define MAX_ITEM_MODELS 2

enum
{
	LOCALEFFECT_EV_PLAYER_TELEPORT_IN
	, LOCALEFFECT_EV_PLAYER_TELEPORT_OUT
	, LOCALEFFECT_VSAY_HEADICON
	, LOCALEFFECT_VSAY_HEADICON_TIMEOUT
	, LOCALEFFECT_ROCKETTRAIL_LAST_DROP
	, LOCALEFFECT_ROCKETFIRE_LAST_DROP
	, LOCALEFFECT_GRENADETRAIL_LAST_DROP
	, LOCALEFFECT_BLOODTRAIL_LAST_DROP
	, LOCALEFFECT_FLAGTRAIL_LAST_DROP
	, LOCALEFFECT_LASERBEAM
	, LOCALEFFECT_LASERBEAM_SMOKE_TRAIL
	, LOCALEFFECT_EV_WEAPONBEAM
	, MAX_LOCALEFFECTS = 64
};

typedef vec_t dualquat_t[8];

typedef enum
{
	RT_MODEL,
	RT_SPRITE,
	RT_PORTALSURFACE,

	NUM_RTYPES
} refEntityType_t;

typedef struct bonepose_s
{
	dualquat_t dualquat;
} bonepose_t;

#ifdef _L

typedef struct gitem_s
{
	//header
	char * const classname;        // spawning name
	int tag;
	itemtype_t type;
	int flags;              // actions the item does in the game

	//media
	const char * const world_model[MAX_ITEM_MODELS];
	const char * const icon;
	const char * const simpleitem;       // Kurim : we use different images for representing simpleitems
	const char * const pickup_sound;
	int effects;


	char *name;      // for printing on pickup
	char *shortname;       // for printing on messages
	char *color;            // for printing on messages

	int quantity;           // how much it gives at picking
	int inventory_max;		// how much quantity of this the inventory can carry

	// special
	int ammo_tag;           // uses this ammo, for weapons
	int weakammo_tag;

	void *info;             // miscelanea info goes pointed in here

	// space separated string of stuff to precache that's not mentioned above
	const char * const precache_models;
	const char * const precache_sounds;
	const char * const precache_images;

	int asRefCount, asFactored;
} gsitem_t;

typedef struct entity_s
{
	refEntityType_t	rtype;
	union
	{
		int flags;
		int renderfx;
	};

	struct model_s *model;				// opaque type outside refresh

	/*
	** most recent data
	*/
	vec3_t axis[3];
	vec3_t origin, origin2;
	vec3_t lightingOrigin;
	int frame;
	bonepose_t *boneposes;				// pretransformed boneposes for current frame

	/*
	** previous data for lerping
	*/
	int oldframe;
	bonepose_t *oldboneposes;			// pretransformed boneposes for old frame
	float backlerp;						// 0.0 = current, 1.0 = old

	/*
	** texturing
	*/
	struct skinfile_s *customSkin;		// registered .skin file
	struct shader_s	*customShader;		// NULL for inline skin

	/*
	** misc
	*/
	unsigned int shaderTime;
	union
	{
		byte_vec4_t color;
		qbyte shaderRGBA[4];
	};

	float scale;
	float radius;						// used as RT_SPRITE's radius
	float rotation;

//#ifdef HARDWARE_OUTLINES
	float outlineHeight;
	union
	{
		byte_vec4_t outlineColor;
		qbyte outlineRGBA[4];
	};
//#endif
} entity_t;

typedef struct
{
	char name[MAX_QPATH];
	int flags;
	int parent;
	struct bonenode_s *node;
} cgs_bone_t;

typedef struct cgs_skeleton_s
{
	struct model_s *model;

	int numBones;
	cgs_bone_t *bones;

	int numFrames;
	bonepose_t **bonePoses;

	struct cgs_skeleton_s *next;

	// store the tagmasks as part of the skeleton (they are only used by player models, tho)
	struct cg_tagmask_s *tagmasks;

	struct bonenode_s *bonetree;
} cgs_skeleton_t;

typedef struct
{
	entity_state_t current;
	entity_state_t prev;        // will always be valid, but might just be a copy of current

	int serverFrame;            // if not current, this ent isn't in the frame
	unsigned int fly_stoptime;

	int respawnTime;

	entity_t ent;                   // interpolated, to be added to render list
	unsigned int type;
	unsigned int renderfx;
	unsigned int effects;
	struct cgs_skeleton_s *skel;

	vec3_t velocity;

	qboolean canExtrapolate;
	qboolean canExtrapolatePrev;
	vec3_t prevVelocity;
	int microSmooth;
	vec3_t microSmoothOrigin;
	vec3_t microSmoothOrigin2;
	//vec3_t prevExtrapolatedOrigin;
	//vec3_t extrapolatedOrigin;

	gsitem_t	*item;

	//effects
	vec3_t trailOrigin;         // for particle trails

	// local effects from events timers
	unsigned int localEffects[MAX_LOCALEFFECTS];

	// attached laser beam
	vec3_t laserOrigin;
	vec3_t laserPoint;
	vec3_t laserOriginOld;
	vec3_t laserPointOld;
	qboolean laserCurved;

	qboolean linearProjectileCanDraw;
	vec3_t linearProjectileViewerSource;
	vec3_t linearProjectileViewerVelocity;

	vec3_t teleportedFrom;
	byte_vec4_t outlineColor;

	// used for client side animation of player models
	qboolean pendingAnimationsUpdate;
	int lastAnims;
	int lastVelocitiesFrames[4];
	float lastVelocities[4][4];
	qboolean jumpedLeft;
	vec3_t animVelocity;
	float yawVelocity;
} centity_t;

#else 

typedef struct
{
	entity_state_t	current;
	entity_state_t	prev;			// will always be valid, but might just be a copy of current

	int			serverFrame;		// if not current, this ent isn't in the frame
	unsigned int	fly_stoptime;

	int			respawnTime;
// LERPEDCGAME
	entity_t		ent;				// interpolated, to be added to render list
	unsigned int	type;
	unsigned int	renderfx;
	unsigned int	effects;

	gitem_t			*item;

	//effects
	vec3_t			trailOrigin;	// for particle trails

	// local effects from events timers
	unsigned int	localEffects[MAX_LOCALEFFECTS];	

	vec3_t			teleportedFrom;
	byte_vec4_t		outlineColor;
	byte_vec4_t		color;

// LERPEDCGAME
} centity_t;

#endif

typedef struct
{
	float fov;
	float scale;
	vec3_t vieworg;
	vec3_t viewanglesOffset;
	qboolean noEnts;
} skyportal_t;

typedef struct refdef_s
{
	int x, y, width, height;			// in virtual screen coordinates
	float fov_x, fov_y;
	vec3_t vieworg;
	vec3_t viewaxis[3];
	float blend[4];						// rgba 0-1 full screen blend
	unsigned int time;					// time is used for timing offsets
	int rdflags;						// RDF_UNDERWATER, etc
	skyportal_t skyportal;
	qbyte *areabits;					// if not NULL, only areas with set bits will be drawn
} refdef_t;

typedef struct
{
	char name[MAX_QPATH];
	int hand;
	int fov, zoomfov;
	byte_vec4_t color;
	struct shader_s *icon;
} cg_clientInfo_t;

typedef struct cgs_media_handle_s
{
	char *name;
	void *data;
	struct cgs_media_handle_s *next;
} cgs_media_handle_t;

#define MAX_SMALL_TECHY_GIBS	4
#define MAX_BIG_TECHY_GIBS		2
#define MAX_TECHY_GIBS			( MAX_SMALL_TECHY_GIBS + MAX_BIG_TECHY_GIBS )
#define MAX_MEATY_GIBS			4

enum
{
	VSAY_GENERIC,
	VSAY_NEEDHEALTH,
	VSAY_NEEDWEAPON,
	VSAY_NEEDARMOR,
	VSAY_AFFIRMATIVE,
	VSAY_NEGATIVE,
	VSAY_YES,
	VSAY_NO,
	VSAY_ONDEFENSE,
	VSAY_ONOFFENSE,
	VSAY_OOPS,
	VSAY_SORRY,
	VSAY_THANKS,
	VSAY_NOPROBLEM,
	VSAY_YEEHAA,
	VSAY_GOODGAME,
	VSAY_DEFEND,
	VSAY_ATTACK,
	VSAY_NEEDBACKUP,
	VSAY_BOOO,
	VSAY_NEEDDEFENSE,
	VSAY_NEEDOFFENSE,
	VSAY_NEEDHELP,
	VSAY_ROGER,
	VSAY_ARMORFREE,
	VSAY_AREASECURED,
	VSAY_SHUTUP,
	VSAY_BOOMSTICK,
	VSAY_GOTOPOWERUP,
	VSAY_GOTOQUAD,
	VSAY_OK,

	VSAY_TOTAL = 128
};

enum
{
	KEYICON_FORWARD = 0,
	KEYICON_BACKWARD,
	KEYICON_LEFT,
	KEYICON_RIGHT,
	KEYICON_FIRE,
	KEYICON_JUMP,
	KEYICON_CROUCH,
	KEYICON_SPECIAL,
	KEYICON_TOTAL
};

#define NUM_CROSSHAIRS	14

typedef struct
{
	// sounds
	cgs_media_handle_t *sfxChat;

	// timers
	cgs_media_handle_t *sfxTimerBipBip;
	cgs_media_handle_t *sfxTimerPloink;

	cgs_media_handle_t *sfxRic[3];

	cgs_media_handle_t *sfxWeaponUp;
	cgs_media_handle_t *sfxWeaponUpNoAmmo;

	cgs_media_handle_t *sfxWalljumpFailed;

	//--------------------------------------

	cgs_media_handle_t *sfxWeaponHit[4];
	cgs_media_handle_t *sfxWeaponKill;
	cgs_media_handle_t *sfxWeaponHitTeam;

	cgs_media_handle_t *sfxItemRespawn;
	cgs_media_handle_t *sfxTeleportIn;
	cgs_media_handle_t *sfxTeleportOut;
	cgs_media_handle_t *sfxShellHit;

	// Gunblade sounds :
	cgs_media_handle_t *sfxGunbladeWeakShot[3];
	cgs_media_handle_t *sfxGunbladeStrongShot;
	cgs_media_handle_t *sfxBladeFleshHit[3];
	cgs_media_handle_t *sfxBladeWallHit[2];
	cgs_media_handle_t *sfxGunbladeStrongHit[3];

	// Riotgun sounds :
	cgs_media_handle_t *sfxRiotgunWeakHit;
	cgs_media_handle_t *sfxRiotgunStrongHit;

	// Grenade launcher sounds :
	cgs_media_handle_t *sfxGrenadeWeakBounce[2];
	cgs_media_handle_t *sfxGrenadeStrongBounce[2];
	cgs_media_handle_t *sfxGrenadeWeakExplosion;
	cgs_media_handle_t *sfxGrenadeStrongExplosion;

	// Rocket launcher sounds :
	cgs_media_handle_t *sfxRocketLauncherWeakHit;
	cgs_media_handle_t *sfxRocketLauncherStrongHit;

	// Plasmagun sounds
	cgs_media_handle_t *sfxPlasmaWeakHit;
	cgs_media_handle_t *sfxPlasmaStrongHit;

	// Lasergun sounds
	cgs_media_handle_t *sfxLasergunWeakHum;
	cgs_media_handle_t *sfxLasergunWeakQuadHum;
	cgs_media_handle_t *sfxLasergunStrongHum;
	cgs_media_handle_t *sfxLasergunStrongQuadHum;

	cgs_media_handle_t *sfxQuadFireSound;

	// VSAY sounds
	cgs_media_handle_t *sfxVSaySounds[VSAY_TOTAL];

	//no wsw

	// models
	//	cgs_media_handle_t		*modTeleportEffect;
	cgs_media_handle_t *modDash;
	cgs_media_handle_t *modHeadStun;

	cgs_media_handle_t *modTechyGibs[MAX_TECHY_GIBS];
	cgs_media_handle_t *modMeatyGibs[MAX_MEATY_GIBS];

	//wsw weapon sfx
	cgs_media_handle_t *modRocketExplosion;
	cgs_media_handle_t *modPlasmaExplosion;

	cgs_media_handle_t *modBulletExplode;
	cgs_media_handle_t *modBladeWallHit;
	cgs_media_handle_t *modBladeWallExplo;

	cgs_media_handle_t *modElectroBoltWallHit;
	cgs_media_handle_t *modInstagunWallHit;

	//no wsw

	cgs_media_handle_t *shaderParticle;
	cgs_media_handle_t *shaderGrenadeExplosion;
	cgs_media_handle_t *shaderRocketExplosion;
	cgs_media_handle_t *shaderRocketExplosionRing;
	cgs_media_handle_t *shaderBulletExplosion;
	cgs_media_handle_t *shaderRaceGhostEffect;
	cgs_media_handle_t *shaderWaterBubble;
	//	cgs_media_handle_t		*shaderTeleportEffect;
	cgs_media_handle_t *shaderSmokePuff;

	cgs_media_handle_t *shaderSmokePuff1;
	cgs_media_handle_t *shaderSmokePuff2;
	cgs_media_handle_t *shaderSmokePuff3;

	cgs_media_handle_t *shaderStrongRocketFireTrailPuff;
	cgs_media_handle_t *shaderWeakRocketFireTrailPuff;
	cgs_media_handle_t *shaderGrenadeTrailSmokePuff;
	cgs_media_handle_t *shaderRocketTrailSmokePuff;
	cgs_media_handle_t *shaderBloodTrailPuff;
	cgs_media_handle_t *shaderBloodTrailLiquidPuff;
	cgs_media_handle_t *shaderBloodImpactPuff;
	cgs_media_handle_t *shaderCartoonHit;
	cgs_media_handle_t *shaderCartoonHit2;
	cgs_media_handle_t *shaderCartoonHit3;
	cgs_media_handle_t *shaderTeamMateIndicator;
	cgs_media_handle_t *shaderTeleporterSmokePuff;
	cgs_media_handle_t *shaderBulletMark;
	cgs_media_handle_t *shaderExplosionMark;
	cgs_media_handle_t *shaderEnergyMark;
	cgs_media_handle_t *shaderLaser;
	cgs_media_handle_t *shaderNet;
	cgs_media_handle_t *shaderBackTile;
	cgs_media_handle_t *shaderSelect;
	cgs_media_handle_t *shaderChatBalloon;
	cgs_media_handle_t *shaderDownArrow;

	//wsw
	//----------------------------------------------

	cgs_media_handle_t *shaderAdditiveParticleShine;

	//wsw weapon sfx
	cgs_media_handle_t *shaderPlasmaMark;
	cgs_media_handle_t *shaderElectroBeamOld;
	cgs_media_handle_t *shaderElectroBeamOldAlpha;
	cgs_media_handle_t *shaderElectroBeamOldBeta;
	cgs_media_handle_t *shaderElectroBeamA;
	cgs_media_handle_t *shaderElectroBeamAAlpha;
	cgs_media_handle_t *shaderElectroBeamABeta;
	cgs_media_handle_t *shaderElectroBeamB;
	cgs_media_handle_t *shaderElectroBeamBAlpha;
	cgs_media_handle_t *shaderElectroBeamBBeta;
	cgs_media_handle_t *shaderInstaBeam;
	cgs_media_handle_t *shaderLaserGunBeam;
	cgs_media_handle_t *shaderLaserGunBeamOld;
	cgs_media_handle_t *shaderElectroboltMark;
	cgs_media_handle_t *shaderInstagunMark;

	//wsw
	cgs_media_handle_t *shaderPlayerShadow;
	cgs_media_handle_t *shaderFlagFlare;

	// hud icons
	cgs_media_handle_t *shaderWeaponIcon[WEAP_TOTAL];
	cgs_media_handle_t *shaderNoGunWeaponIcon[WEAP_TOTAL];

	cgs_media_handle_t *shaderKeyIconOn[KEYICON_TOTAL];
	cgs_media_handle_t *shaderKeyIconOff[KEYICON_TOTAL];

	//no wsw

	cgs_media_handle_t *sbNums[11];
	cgs_media_handle_t *shaderCrosshair[NUM_CROSSHAIRS];

	// VSAY icons
	cgs_media_handle_t *shaderVSayIcon[VSAY_TOTAL];
} cgs_media_t;

typedef struct orientation_s
{
	vec3_t axis[3];
	vec3_t origin;
} orientation_t;

//equivalent to pmodelinfo_t. Shared by different players, etc.
typedef struct weaponinfo_s
{
	char name[MAX_QPATH];
	qboolean inuse;

	struct	model_s	*model[5]; //one weapon consists of several models

	int firstframe[6];         //animation script
	int lastframe[6];
	int loopingframes[6];
	unsigned int frametime[6];

	orientation_t tag_projectionsource;
	byte_vec4_t outlineColor;

	// handOffset
	vec3_t handpositionOrigin;
	vec3_t handpositionAngles;

	// flash
	unsigned int flashTime;
	qboolean flashFade;
	float flashRadius;
	vec3_t flashColor;

	// barrel
	unsigned int barrelTime;
	float barrelSpeed;

	// sfx
	int num_fire_sounds;
	struct sfx_s *sound_fire[4];
	int num_strongfire_sounds;
	struct sfx_s *sound_strongfire[4];
	struct sfx_s *sound_reload;

} weaponinfo_t;

enum
{
	TEAM_SPECTATOR,
	TEAM_PLAYERS,
	TEAM_ALPHA,
	TEAM_BETA,

	GS_MAX_TEAMS
};

typedef struct
{
	const char *serverName;
	const char *demoName;
	unsigned int playerNum;

	// shaders
	struct shader_s	*shaderWhite;
	struct shader_s	*shaderMiniMap;

	// fonts
	struct mufont_s	*fontSystemSmall;
	struct mufont_s	*fontSystemMedium;
	struct mufont_s	*fontSystemBig;

	char fontNameSystemSmall[MAX_QPATH];
	char fontNameSystemMedium[MAX_QPATH];
	char fontNameSystemBig[MAX_QPATH];

	int initialSharedSeed;

	cgs_media_t media;

	qboolean precacheDone;

	int vidWidth, vidHeight;

	qboolean demoPlaying;
	qboolean demoTutorial;
	qboolean pure;
	qboolean tv, tvRequested;
	int gameProtocol;
	unsigned int snapFrameTime;
	unsigned int extrapolationTime;

	char *demoAudioStream;

	//
	// locally derived information from server state
	//
	char configStrings[MAX_CONFIGSTRINGS][MAX_CONFIGSTRING_CHARS];

	qboolean hasGametypeMenu;

	char weaponModels[WEAP_TOTAL][MAX_QPATH];
	int numWeaponModels;
	weaponinfo_t *weaponInfos[WEAP_TOTAL];    // indexed list of weapon model infos
	orientation_t weaponItemTag;

	cg_clientInfo_t	clientInfo[MAX_CLIENTS];

	struct model_s *modelDraw[MAX_MODELS];

	struct pmodelinfo_s *pModelsIndex[MAX_MODELS];
	struct pmodelinfo_s *basePModelInfo; //fall back replacements
	struct skinfile_s *baseSkin;

	// force models
	struct pmodelinfo_s *teamModelInfo[GS_MAX_TEAMS];
	struct skinfile_s *teamCustomSkin[GS_MAX_TEAMS]; // user defined
	int teamColor[GS_MAX_TEAMS];

	struct sfx_s *soundPrecache[MAX_SOUNDS];
	struct shader_s	*imagePrecache[MAX_IMAGES];
	struct skinfile_s *skinPrecache[MAX_SKINFILES];
} cg_static_t;

#define EVENT_ENTITIES_START	96

enum
{
	ET_GENERIC,
	ET_PLAYER,
	ET_CORPSE,
	ET_BEAM,
	ET_PORTALSURFACE,
	ET_PUSH_TRIGGER,

	ET_GIB,         // leave a trail
	ET_BLASTER,     // redlight + trail
	ET_ELECTRO_WEAK,
	ET_ROCKET,      // redlight + trail
	ET_GRENADE,
	ET_PLASMA,

	ET_SPRITE,

	ET_ITEM,        // for simple items
	ET_LASERBEAM,   // for continuous beams
	ET_CURVELASERBEAM, // for curved beams
	ET_FLAG_BASE,

	ET_MINIMAP_ICON,
	ET_DECAL,
	ET_ITEM_TIMER,	// for specs only
	ET_PARTICLES,
	ET_SPAWN_INDICATOR,

	// eventual entities: types below this will get event treatment
	ET_EVENT = EVENT_ENTITIES_START,
	ET_SOUNDEVENT,
	ET_TOTAL_TYPES, // current count
	MAX_ENTITY_TYPES = 128
};

typedef struct
{
	int type;
	int POVent;
	qboolean thirdperson;
	qboolean playerPrediction;
	qboolean drawWeapon;
	qboolean draw2D;
	refdef_t refdef;
	float fracDistFOV;
	vec3_t origin;
	vec3_t angles;
	vec3_t axis[3];
	vec3_t velocity;
} cg_viewdef_t;

typedef struct  
{
	vec3_t origins[32];
	unsigned int timeStamps[32];
	qboolean teleported[32];
	int head;
}gs_laserbeamtrail_t;

typedef struct 
{
	entity_t ent;

	unsigned int POVnum;
	int weapon;

	// animation
	int baseAnim;
	unsigned int baseAnimStartTime;
	int eventAnim;
	unsigned int eventAnimStartTime;

	// other effects
	orientation_t projectionSource;
	unsigned int fallEff_Time;
	int fallEff_rebTime;
} cg_viewweapon_t;

typedef struct
{
	unsigned int timestamp;
	unsigned int kicktime;
	float v_roll, v_pitch;
} cg_kickangles_t;

typedef struct
{
	unsigned int timestamp;
	unsigned int blendtime;
	float blend[4];
} cg_viewblend_t;

#define MAX_COLORBLENDS 3
#define MAX_ANGLES_KICKS 3

typedef struct
{
	unsigned int time;
	float delay;

	unsigned int realTime;
	float frameTime;
	float realFrameTime;
	int frameCount;

	unsigned int firstViewRealTime;
	int viewFrameCount;
	qboolean startedMusic;

	int sharedSeed;

	snapshot_t frame, oldFrame;
	qboolean frameSequenceRunning;
	qboolean oldAreabits;
	qboolean portalInView;
	qboolean fireEvents;
	qboolean firstFrame;

	float predictedOrigins[64][3];              // for debug comparing against server

	float predictedStep;                // for stair up smoothing
	unsigned int predictedStepTime;

	unsigned int predictingTimeStamp;
	unsigned int predictedEventTimes[32];
	vec3_t predictionError;
	player_state_t predictedPlayerState;     // current in use, predicted or interpolated
	int predictedWeaponSwitch;				// inhibit shooting prediction while a weapon change is expected
	gs_laserbeamtrail_t weaklaserTrail;

	// prediction optimization (don't run all ucmds in not needed)
	int predictFrom;
	entity_state_t predictFromEntityState;
	player_state_t predictFromPlayerState;

	int lastWeapon;

	vec3_t autorotateAxis[3];

	float lerpfrac;                     // between oldframe and frame
	float xerpTime;
	float oldXerpTime;
	float xerpSmoothFrac;

	int effects;

	vec3_t lightingOrigin;

	qboolean showScoreboard;            // demos and multipov
	qboolean specStateChanged;

	unsigned int multiviewPlayerNum;       // for multipov chasing, takes effect on next snap

	int pointedNum;

	//
	// all cyclic walking effects
	//
	float xyspeed;

	float oldBobTime;
	int bobCycle;                   // odd cycles are right foot going forward
	float bobFracSin;               // sin(bobfrac*M_PI)

	//
	// kick angles and color blend effects
	//

	cg_kickangles_t	kickangles[MAX_ANGLES_KICKS];
	cg_viewblend_t colorblends[MAX_COLORBLENDS];
	unsigned int damageBlends[4];

	//
	// transient data from server
	//
	char checkname[MAX_QPATH];
	char loadingstring[MAX_QPATH];
	int precacheCount, precacheTotal;
	const char *matchmessage;
	char *teaminfo;
	size_t teaminfo_size;
	char *motd;
	unsigned int motd_time;

	// awards
	char award_lines[3][MAX_QPATH];
	unsigned int award_times[3];
	int award_head;

	// statusbar program
	struct cg_layoutnode_s *statusBar;

	cg_viewweapon_t weapon;
	cg_viewdef_t view;

	// cg_gamechat_t chat;
} cg_state_t;

typedef struct
{
	int frames;
	unsigned int start;
	int counts[100];
} cl_timedemo_t;

typedef struct cmodel_state_s cmodel_state_t;

typedef struct
{
	int timeoutcount;

	cl_timedemo_t timedemo;

	int cmdNum;						// current cmd
	usercmd_t *cmds;				// [CMD_BACKUP] each mesage will send several old cmds
	int *cmd_time;					// [CMD_BACKUP] time sent, for calculating pings
	qboolean inputRefreshed;

	int receivedSnapNum;
	int pendingSnapNum;
	int currentSnapNum;
	int previousSnapNum;
	int suppressCount;				// number of messages rate suppressed
	snapshot_t *snapShots;			// [CMD_BACKUP]
	qbyte *frames_areabits;

	cmodel_state_t *cms;

	// the client maintains its own idea of view angles, which are
	// sent to the server each frame.  It is cleared to 0 upon entering each level.
	// the server sends a delta each frame which is added to the locally
	// tracked view angles to account for standing on rotating objects,
	// and teleport direction changes
	vec3_t viewangles;
} client_state_t;