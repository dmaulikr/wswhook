#pragma once

#define Detour( address, ident ) DetourFunction( ( PBYTE ) address, ( PBYTE ) h_##ident ); __asm mov [o_##ident], eax

class CWarsow {
public:
	bool IsInGame( ) {
		return ( entities && cgs && refdef );
	};

	centity_t* entity( int clientNum ) {
		return &entities[clientNum + 1];
	};

	centity_t* me( ) {
		return this->entity( playerNum );
	};

	cg_clientInfo_t* ci( int clientNum ) {
		return &cgs->clientInfo[clientNum];
	};

	centity_t* entities;
	int playerNum;
	refdef_t* refdef;
	cg_static_t* cgs;
	cg_state_t* cg;
	snapshot_t* freshSnap;

	float serverTime;

	bool toScreen( cgame_import_t* imports, vec3_t in, vec2_t out ) {
		// check if we draw behind us
		vec3_t sub;
        VectorSubtract( in, this->refdef->vieworg, sub );
		float z = DotProduct( sub, this->refdef->viewaxis[( 2 + 1 ) % 3] );

		if ( z <= 0 )
			return false;

		imports->R_TransformVectorToScreen( this->refdef, in, out );

		if ( ( out[0] < 0 || out[0] > this->cgs->vidWidth ) || ( out[1] < 0 || out[1] > this->cgs->vidHeight ) )
			return false;

		return true;
	};
};

extern CWarsow wsw;

char *va( const char *format, ... );

extern const vec4_t colorCyan;
extern const vec4_t colorYellow;
extern const vec4_t colorBlue;
extern const vec4_t colorWhite;
extern const vec4_t colorOrange;