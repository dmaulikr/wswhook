#include "Main.h"

cgame_export_t *(*o_cgameapi) ( cgame_import_t *import );

cgame_import_t* imports;
cgame_export_t* exports;

HHOOK wmMouse;
bool mouseHold;

/*
 * client_state_t: str search on "Owner left the listen server"
 * second call outside:  sub_000000(); <-- this sub_00000(); if ( dword_00000 )
 * log @ "..\\client\\cl_poobear.c" -> first memset: memset(&dword_00000, 0, 0x00000;
 */
client_state_t* cl = ( client_state_t* ) 0;

/*
 * git note: memory locations have been erased for code release
 * bet you can guess why. No need to ruine that precious game!
 */

/* 
 * R_AddEntityToScene, for cg_entities 
 */
const DWORD ad_addent = ( const DWORD ) 0;

/* 
 * R_RenderView 
 */
const DWORD ad_renv = ( const DWORD ) 0;

/* 
 * CL_GameModule_R_RegisterWorldModel, for cgs 
 */
const DWORD ad_regwm = ( const DWORD ) 0;

float last_x, last_y;
void Trap_GetProc( void );

float Q_rsqrt( float number ) {
	// en.wikipedia.org/wiki/Fast_inverse_square_root
	float x2 = number * 0.5F;
	float y  = number;
	long i  = * ( long * ) &y; // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 ); // what the fuck?
	y  = * ( float * ) &i;
	y  = y * ( 1.5f - ( x2 * y * y ) );   

	// wot, linux? assert( !isnan(y) ); 

	return y;
}

struct shader_s* GetMediaShader( cgs_media_handle_t *mediashader ) {
	if( !mediashader->data )
		mediashader->data = ( void * )imports->R_RegisterPic( mediashader->name );

	return ( struct shader_s * )mediashader->data;
}

bool IsVisible( vec3_t point ) {
	trace_t t;
	static vec3_t vec3_origin = { 0, 0, 0 };
	
	/*vec3_t end;
	VectorMA(wsw.refdef->vieworg, 8192, wsw.refdef->viewaxis[0], end);*/

	// use MASK_SHOT instead of MASK_OPAQUE to make sure no enemy is in front
	exports->Trace( &t, wsw.refdef->vieworg, vec3_origin, vec3_origin, point, -1 , MASK_OPAQUE );

	return ( t.fraction == 1.0f );
}

enum cvars_e {
	LG_VAL,   // lasergun strength
	EB_VAL,   // thunderbold strength
	LG_FOV,
	EB_FOV,
	LG_HORONLY,
	MAX_CVARS // used to memalloc our cvars struct
};
cvar_t** cvars = ( cvar_t** ) malloc( MAX_CVARS * sizeof( cvar_t ) );

void RegisterCvars( ) {
	cvars[LG_VAL] = imports->Cvar_Get( "hlg_val", "0.45", 1 );
	cvars[EB_VAL] = imports->Cvar_Get( "heb_val", "0.25", 1 );

	cvars[LG_FOV] = imports->Cvar_Get( "hlg_fov", "20", 1 );
	cvars[EB_FOV] = imports->Cvar_Get( "heb_fov", "5", 1 );

	cvars[LG_HORONLY] = imports->Cvar_Get( "hlg_horaim", "1", 1 );
}

bool IsEnemy( centity_t* cent ) {
	// old entity?
	if ( cent->serverFrame != wsw.cg->frame.serverFrame ) // our entity wasn't in the frame list
		return false;

	// 31: SOLID_BMODEL
	int team = cent->current.team;
	if( !cent->current.modelindex || !cent->current.solid || cent->current.solid == 31 || cent->current.team == TEAM_SPECTATOR || cent->type != ET_PLAYER )
		return false;

	if ( team != 1 )
		if ( team == wsw.me( )->current.team )
			return false;

	return true;
}

void CG_Init( const char* serverName, unsigned int playerNum, int vidWidth, int vidHeight, qboolean demoplaying, const char* demoName, qboolean pure, unsigned int snapFrameTime, int protocol, int sharedSeed ) {
	wsw.playerNum = playerNum;

	exports->Init( serverName, playerNum, vidWidth, vidHeight, demoplaying, demoName, pure, snapFrameTime, protocol, sharedSeed );
}

void assist( float mm, float& angle ) {
	float hv = 0;
	int weap = wsw.me( )->current.weapon;

	switch ( weap ) {
		case WEAP_LASERGUN:
			hv = cvars[LG_VAL]->value; 
			break;
		case WEAP_ELECTROBOLT:
			hv = cvars[EB_VAL]->value;
			break;
		case WEAP_RIOTGUN:
		case WEAP_MACHINEGUN:
			hv = 0.5;
			break;
	}

	if ( hv == 0 ) {
		angle = 0;
		return;
	}

	// mousemove, correct our angle
	if ( mm >= 0 ) {
		if ( angle > ( hv + 1 ) * mm )	
			angle = hv * mm;
		else if ( angle < -hv * mm )
			angle = -hv * mm;
		else angle = 0;
	} else if ( mm < 0 ) {
		if( angle < ( hv + 1 )* mm )	
			angle = hv * mm;
		else if ( angle > -hv * mm )
			angle = -hv * mm;
		else angle = 0;
	} 

	// smooth it out maybe?
	// angle *= 0.5;
}

void SimulateMouseClick( void* args ) {  
	INPUT    Input={0};											

	Input.type        = INPUT_MOUSE;								
	Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;							
	SendInput( 1, &Input, sizeof(INPUT) );				

	Sleep( 50 );

	ZeroMemory(&Input,sizeof(INPUT));									
	Input.type        = INPUT_MOUSE;									
	Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;							
	SendInput( 1, &Input, sizeof(INPUT) );								
}

float vlen( vec3_t a, vec3_t b ) {
	return ( float ) sqrt( ( ( a[0] - b[0] ) * ( a[0] - b[0] ) ) + ( ( a[1] - b[1] ) * ( a[1] - b[1] ) ) );
}

int FindPossibleTarget( ) {
	int i = 0;
	int target_num = -1;

	float max_sq;
	
	/*
	 * target is always selected as "closest to crosshair"
	 * (dir.x*viewaxis.f.x + dir.y*viewaxis.f.y + dir.z*viewaxis.f.z)*2) / dir.x*dir.x+dir.y+dir.y+dir.z+dir.z
	 * would do it...
	 * check within fov too ~360/2*pi*acos(dir.dot(viewaxis.f))
	 */
	for ( i; i < 32; i++ ) {
		centity_t* entity = wsw.entity( i );
			
		if ( !IsEnemy( entity ) )
			continue;

		vec3_t orig;
		int head_top = 40;
		VectorSet( orig, entity->ent.origin[0], entity->ent.origin[1], entity->ent.origin[2] + head_top );

		vec3_t dir; 
		VectorSubtract( entity->ent.origin, wsw.refdef->vieworg, dir ); 

		if ( IsVisible( orig ) ) {
			// calculate fov
			float len = Q_rsqrt( DotProduct( dir, dir ) );
			for ( int i = 0; i < 3; i++ )
				dir[i] *= len;

			// exclude targets out of range when we aim with the lasergun
			if ( wsw.me( )->current.weapon == WEAP_LASERGUN ) {
				if ( len < 0.0012 ) // len 
					continue;
			}

			// 57.3: 360 / ( 2 * M_PI )
			float ang = 57.3 * acos( DotProduct( dir, wsw.refdef->viewaxis[0] ) );
			float vert = ( 57.3 * asin( DotProduct( dir, wsw.refdef->viewaxis[0] ) ) ) - 90;

			if ( vert < 0 )
				vert *= -1;

			if ( vert <= 20 ) {
				// check fov if within ~20
				if ( ang <= ( wsw.me( )->current.weapon == WEAP_ELECTROBOLT ) ? cvars[EB_FOV]->integer : cvars[LG_FOV]->integer ) {
					float fq = dir[0] * wsw.refdef->viewaxis[0][0] + dir[1] * wsw.refdef->viewaxis[0][1] + dir[2] * wsw.refdef->viewaxis[0][2];
					float sq = ( fq * fq ) / ( dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2] );
					
					if ( fq < 0 )
						sq *= -1;
						
					// is this potentional target closer to xhair?
					if ( sq >  max_sq ) {
						target_num = i;
						max_sq = sq;
					}
				}
			}
		}
	}

	return target_num;
}

void CG_RenderScene( float frameTime, float realFrameTime, int realTime, unsigned int serverTime, float stereo_separation, unsigned int extrapolationTime ) {
	exports->RenderView( frameTime, realFrameTime, realTime, serverTime, stereo_separation, extrapolationTime );
	wsw.serverTime = serverTime;

	float max_sq;
	
	// set viewangles staticly, it's the only static offset we need
	if ( wsw.refdef && !wsw.cg ) {
		DWORD view = ( DWORD ) wsw.refdef - offsetof( cg_viewdef_t, refdef );
		wsw.cg = ( cg_state_t* ) ( view - offsetof( cg_state_t, view ) );
	}

	if ( !wsw.cg )
		return;

	static bool first = false;
	if ( !first ) {
		RegisterCvars( );
		first = true;
	}

	static mufont_s* font = imports->SCR_RegisterFont( "bitstream_10" );

	for ( int i = 0; i < 3; i++ ) {
		wsw.cg->kickangles[i].v_pitch = 0;
		wsw.cg->kickangles[i].v_roll = 0;
	}

	if ( wsw.IsInGame( ) ) {
		int i = 0;
		int target_num = FindPossibleTarget( );

		/*
		 * there is plenty of room for improvement here
		 * - calculate muzzle
		 * - adjust for falling opponents, or accelerated with a jumppad or so..
		 * - most weapons are not instant, rg has a delay too; predict them?
		 * - must do !important! reduce impact on y-axis, too obvious aiming
		 */
		if ( ( mouseHold || ( wsw.me( )->current.weapon == WEAP_ELECTROBOLT ) ) && target_num != -1 ) {
			// provide aim assistance w. valid target
			centity_t* target = wsw.entity( target_num );
			float* target_pos = target->ent.origin;
			target_pos[2] += 25;

			vec3_t angle;
			VectorSubtract( target_pos, wsw.refdef->vieworg, angle );
			vectoangles( angle, angle );

			/* 
				get viewangles; this is how it is stored in memory:

				refdef_t refdef; -- what we got
				float fracDistFOV;
				vec3_t origin;
				vec3_t angles; -- need! :o
			*/
			DWORD frac = ( ( DWORD ) wsw.refdef + sizeof( refdef_t ) );
			float* angles = ( float* ) ( frac + ( sizeof( vec3_t ) + 4 ) );

			angle[1] = AngMod180(angle[1] - angles[1]); // yaw
			angle[0] = AngMod180(angle[0] - angles[0]); // pitch

			// got the angle needed to correct my own aim, let's humanize it now
			float mx = cl->viewangles[1] - last_x;
			float my = cl->viewangles[0] - last_y;

			assist( mx, angle[1] );
			assist( my, angle[0] );

			if ( wsw.me( )->current.weapon == WEAP_LASERGUN ) {
				if ( !cvars[LG_HORONLY]->integer )
					cl->viewangles[0] += angle[0];
			} else 
				cl->viewangles[0] += angle[0];

			cl->viewangles[1] += angle[1];

			last_x = cl->viewangles[1];
			last_y = cl->viewangles[0];
		}

		int xhair_clnum = -1;

		// do trigger if trace matches a target
		vec3_t end;
		VectorMA( wsw.cg->view.origin, 8000, wsw.refdef->viewaxis[0], end );

		trace_t t;
		vec3_t vec3_origin = { 0, 0, 0 };
		exports->Trace( &t, wsw.cg->view.origin, vec3_origin, vec3_origin, end, wsw.playerNum , MASK_SHOT );
			
		if ( t.ent != 0 ) {
			xhair_clnum = ( t.ent - 1 ); // skip ent to get the correct one
		}

		// automatic trigger with electrobolt weap 
		if ( xhair_clnum != -1 && wsw.freshSnap && wsw.freshSnap->playerState.weaponState != 9 ) {
			if ( ( wsw.me( )->current.weapon == WEAP_ELECTROBOLT || wsw.me( )->current.weapon == WEAP_INSTAGUN ) && IsEnemy( wsw.entity( xhair_clnum ) ) ) {
				_beginthread( SimulateMouseClick, 0, 0 );
			}
		}

		// esp 
		for ( i; i < 32; i++ ) {
			centity_t* entity = wsw.entity( i );
			
			if ( !IsEnemy( entity ) )
				continue;

			// display icon on enemies with a different color
			char p[128];
			vec2_t scr;

			vec3_t orig;
			int head_top = 40;
			VectorSet( orig, entity->ent.origin[0], entity->ent.origin[1], entity->ent.origin[2] + head_top );

			// determe best possible candidate as aim target
			vec3_t dir; 
			VectorSubtract( entity->ent.origin, wsw.refdef->vieworg, dir ); 
			// this is completely wrong, we need to track the body instead (ps; testing purpose, fix later)
			// warsow has 1 hitbox, no seperate head hitbox, tracking the head would be useless -> get center of body

			bool vis = IsVisible( orig );

			if ( wsw.toScreen( imports, orig, scr ) ) {
				// adjust with size
				scr[0] -= floorf( 19 / 2 );
				vec4_t* clr = ( vec4_t* ) &( ( vis ) ? colorYellow : colorCyan );

				// todo: use head instead
				/*if ( vis )
					scr[1] += 15;*/ // adjust the icon when visible

				if ( wsw.me( )->current.weapon != WEAP_ELECTROBOLT && target_num == i )
					clr =  ( vec4_t* ) &colorOrange;
				else if ( wsw.me( )->current.weapon == WEAP_ELECTROBOLT && xhair_clnum == i )
					clr = ( vec4_t* ) &colorBlue;

				imports->R_DrawStretchPic( scr[0], scr[1], 19, 19, 0, 0, 1 , 1, *clr, 
					GetMediaShader( wsw.cgs->media.shaderTeamMateIndicator ) );
			}
		}

		imports->SCR_DrawString( 20, 20, 0, "wswhook (c) mowl", font, ( vec_t* ) colorWhite );
	}
}

void ( __cdecl* o_R_AddEntityToScene ) ( const struct entity_s* ent );
void __cdecl h_R_AddEntityToScene( const struct entity_s* ent ) {
	if ( !wsw.entities && ent->model && ent->rtype == RT_MODEL ) {
		if ( !_strnicmp( *( char** ) ent->model, "models/players/", 15 ) ) {
			centity_t* cent = ( centity_t* ) ( ( DWORD ) ent - offsetof( centity_t, ent ) );
			wsw.entities = cent - cent->current.number;

			/*client_info* info = CL_GetClientInfo( cent->current.number );
			info->pos = ( float* ) ent->origin;*/

			o_R_AddEntityToScene( ent );
			DetourRemove( ( PBYTE ) h_R_AddEntityToScene, ( PBYTE ) o_R_AddEntityToScene );

			return;
		}
	}

	o_R_AddEntityToScene( ent );
}

void ( __cdecl* o_R_RenderView ) ( const refdef_t* fd );
void __cdecl h_R_RenderView( const refdef_t* fd ) {
	wsw.refdef = ( refdef_t* ) fd;
	o_R_RenderView( fd );

	// DetourRemove( ( PBYTE ) h_R_RenderView, ( PBYTE ) o_R_RenderView );
}

void ( __cdecl* o_CL_GameModule_R_RegisterWorldModel ) ( const char* model );
void __cdecl h_CL_GameModule_R_RegisterWorldModel( const char* model ) {
	cg_static_t* temp = 0;
	wsw.cgs = ( cg_static_t* ) ( ( int ) model - ( int ) &temp->configStrings[CS_WORLDMODEL] );

	o_CL_GameModule_R_RegisterWorldModel( model );
	delete temp;

	DetourRemove( ( PBYTE ) h_CL_GameModule_R_RegisterWorldModel, ( PBYTE ) o_CL_GameModule_R_RegisterWorldModel );
}

void Static_Hooks( ) {
	Detour( ad_addent, R_AddEntityToScene ); // cg_entities
	Detour( ad_renv, R_RenderView );
	Detour( ad_regwm, CL_GameModule_R_RegisterWorldModel ); // cgs
}

void CG_Shutdown( void ) {
	exports->Shutdown( );

	//Static_Hooks( );
	Trap_GetProc( );
}

void CG_NewFrameSnap( snapshot_t* frame, snapshot_t* lerp ) {
	//wsw.cg = ( cg_state_t* ) ( ( DWORD ) frame - offsetof( cg_state_t, frame ) );
	wsw.freshSnap = frame;

	exports->NewFrameSnapshot( frame, lerp );
}

cgame_export_t* Redirect_Api( cgame_import_t *import ) {
	/*
	.text:100254A7                 mov     dword_101CABC0, offset sub_1000AF00
	.text:100254B1                 mov     eax, offset dword_101CAB98
	.text:100254B6                 pop     esi
	.text:100254B7                 retn
	*/

	imports = new cgame_import_t( ( *import ) );
	cgame_export_t* ret = o_cgameapi( import );
	exports = new cgame_export_t( ( *ret ) );

	ret->RenderView = &CG_RenderScene;
	ret->Init = &CG_Init;
	ret->Shutdown = &CG_Shutdown;
	ret->NewFrameSnapshot = &CG_NewFrameSnap;

	return ret;
}

LRESULT CALLBACK MouseTrap( int code, WPARAM w_param, LPARAM l_param ) {
    MOUSEHOOKSTRUCT* m = ( MOUSEHOOKSTRUCT* ) l_param;

	// catch our mouse handle, only provide assistance when mouse1 is pressed
    if ( m != NULL ) {
        if ( w_param == WM_LBUTTONDOWN )
			mouseHold = true;
		else if ( w_param == WM_LBUTTONUP )
			mouseHold = false;
    }

    return CallNextHookEx( wmMouse, code, w_param, l_param );
}

FARPROC ( __stdcall* o_GetProcAddress )( HMODULE, LPCSTR );
FARPROC __stdcall h_GetProcAddress( HMODULE mod, LPCSTR proc ) {
	char modIdent[MAX_PATH];
	if ( GetModuleFileName( mod, modIdent, sizeof( modIdent ) ) ) {
		if ( HIWORD( proc ) ) {
			if( !strcmp( proc, "GetProcAddress" ) )
				return ( ( FARPROC ) h_GetProcAddress );

			if ( strstr( modIdent, "cgame_x86.dll" ) && !strcmp( proc, "GetCGameAPI" ) ) {
				// Setup our mouse hook
				wmMouse = SetWindowsHookEx( WH_MOUSE_LL, MouseTrap, GetModuleHandle( NULL ), 0 );
				Static_Hooks( );

				o_cgameapi = ( cgame_export_t* ( __cdecl* ) ( cgame_import_t* ) ) o_GetProcAddress( mod, proc );
				return ( PROC ) Redirect_Api;
			}
		}
	}

	return o_GetProcAddress( mod, proc );
}

void Trap_GetProc( void ) {
	void* orig = ( void* ) GetProcAddress( GetModuleHandle( "__rem__" ), "__rem__" );

    PIMAGE_DOS_HEADER header = ( PIMAGE_DOS_HEADER ) GetModuleHandle( NULL ); 
	PIMAGE_NT_HEADERS nt = ( PIMAGE_NT_HEADERS ) ( ( DWORD ) header + ( DWORD ) header->e_lfanew );
	PIMAGE_IMPORT_DESCRIPTOR impInfo = ( PIMAGE_IMPORT_DESCRIPTOR ) ( ( DWORD ) header + ( DWORD ) nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress ); 

    if ( nt->Signature != IMAGE_NT_SIGNATURE || impInfo == ( PIMAGE_IMPORT_DESCRIPTOR )nt ) 
        return;

    while ( true ) { 
		if ( impInfo->Name == NULL )
			break;

        char* temp = ( char* ) ( ( DWORD ) header + ( DWORD ) impInfo->Name );
        if( !stricmp( temp, "__rem__" ) ) 
            break; 

        impInfo++; 
    } 

    if ( !impInfo->Name ) 
        return;

	PIMAGE_THUNK_DATA state = ( PIMAGE_THUNK_DATA ) ( ( DWORD ) header + ( DWORD ) impInfo->FirstThunk ); 
    while ( state->u1.Function ) { 
        if ( state->u1.Function == ( DWORD ) orig ) { 
			DWORD p;

            VirtualProtect( ( void* ) &state->u1.Function, sizeof( DWORD ), PAGE_EXECUTE_READWRITE, &p ); 
            state->u1.Function = ( DWORD ) &h_GetProcAddress; 
            VirtualProtect( ( void* ) &state->u1.Function, sizeof( DWORD ), p, &p ); 

			o_GetProcAddress = ( FARPROC ( __stdcall* )( HMODULE, LPCSTR )) orig;
        } 

        state++; 
    } 
}

void GoInSpyModus( HMODULE hMod ) {
	PIMAGE_DOS_HEADER dosHeader = ( PIMAGE_DOS_HEADER ) hMod;
	PIMAGE_NT_HEADERS ntHeaders = ( PIMAGE_NT_HEADERS ) ( ( long ) hMod + ( ( PIMAGE_DOS_HEADER ) hMod )->e_lfanew);

	DWORD temp = sizeof( IMAGE_DOS_HEADER );
	DWORD prot;

	if ( !VirtualProtect( dosHeader, temp, PAGE_READWRITE, &prot ) )
		return;

	for ( int i = 0; i < temp; i++ )
		*( unsigned char* )( ( unsigned char* ) dosHeader + i ) = 0;

	temp = sizeof( IMAGE_NT_HEADERS );
	if ( !( ntHeaders && VirtualProtect( ntHeaders, temp, PAGE_READWRITE, &prot ) ) )
		return;

	for ( int i = 0; i < temp; i++ )
		*( unsigned char* )( ( unsigned char* ) ntHeaders + i ) = 0;


}

int __stdcall DllMain( HMODULE hMod, DWORD reason, LPVOID reserved ) {
	if ( reason == DLL_PROCESS_ATTACH ) {
		DisableThreadLibraryCalls( hMod );
		GoInSpyModus( hMod );

		Trap_GetProc( );
	}

	if ( reason == DLL_PROCESS_DETACH ) {
		if ( wmMouse )
			UnhookWindowsHookEx( wmMouse );
	}

	return 1;
}