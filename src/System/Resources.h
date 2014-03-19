#pragma once
#include "cinder/CinderResources.h"

//#define RES_MY_RES			CINDER_RESOURCE( ../resources/, image_name.png, 128, IMAGE )
#define RES_SGUI_FONT           CINDER_RESOURCE( ../../../libraries/cinder_0.8.5_vc2010/blocks/SimpleGUI/resources/, pf_tempesta_seven.ttf, 128, CI_FONT )
#define RES_STATS_FONT          CINDER_RESOURCE( ../resources/, ProggySquare.ttf,       129, CI_FONT )

#define RES_CUBE_OBJ            CINDER_RESOURCE( ../resources/, cube.obj,               130, CI_OBJ )
#define RES_SPHERE_OBJ          CINDER_RESOURCE( ../resources/, sphere.obj,             132, CI_OBJ )
#define RES_SQUARE_OBJ          CINDER_RESOURCE( ../resources/, square.obj,             133, CI_OBJ )
#define RES_SHIP_OBJ            CINDER_RESOURCE( ../resources/, ship.obj,               134, CI_OBJ )

#define RES_READY_TEX           CINDER_RESOURCE( ../resources/, ready.png,              136, CI_TEX )
#define RES_GO_TEX              CINDER_RESOURCE( ../resources/, go.png,                 137, CI_TEX )
#define RES_P1_WINS_TEX         CINDER_RESOURCE( ../resources/, win1.png,               138, CI_TEX )
#define RES_P2_WINS_TEX         CINDER_RESOURCE( ../resources/, win2.png,               139, CI_TEX )
#define RES_P3_WINS_TEX         CINDER_RESOURCE( ../resources/, win3.png,               140, CI_TEX )
#define RES_P4_WINS_TEX         CINDER_RESOURCE( ../resources/, win4.png,               141, CI_TEX )
#define RES_PLAYER_TEX          CINDER_RESOURCE( ../resources/, player4.png,            142, CI_TEX )
#define RES_TITLE_TEX           CINDER_RESOURCE( ../resources/, logo.png,               143, CI_TEX )
#define RES_CREDITS_TEX         CINDER_RESOURCE( ../resources/, credits.png,            144, CI_TEX )

#define RES_DASH_TEX            CINDER_RESOURCE( ../resources/, dash.png,               149, CI_TEX )
#define RES_0_TEX               CINDER_RESOURCE( ../resources/, s0.png,                 150, CI_TEX )
#define RES_1_TEX               CINDER_RESOURCE( ../resources/, s1.png,                 151, CI_TEX )
#define RES_2_TEX               CINDER_RESOURCE( ../resources/, s2.png,                 152, CI_TEX )
#define RES_3_TEX               CINDER_RESOURCE( ../resources/, s3.png,                 153, CI_TEX )
#define RES_4_TEX               CINDER_RESOURCE( ../resources/, s4.png,                 154, CI_TEX )
#define RES_5_TEX               CINDER_RESOURCE( ../resources/, s5.png,                 155, CI_TEX )
#define RES_6_TEX               CINDER_RESOURCE( ../resources/, s6.png,                 156, CI_TEX )
#define RES_7_TEX               CINDER_RESOURCE( ../resources/, s7.png,                 157, CI_TEX )
#define RES_8_TEX               CINDER_RESOURCE( ../resources/, s8.png,                 158, CI_TEX )
#define RES_9_TEX               CINDER_RESOURCE( ../resources/, s9.png,                 159, CI_TEX )

#ifdef __APPLE__    // Apple devices only load MP3's via cinder

#define RES_HIT1_SOUND          CINDER_RESOURCE( ../resources/, Hit_Hurt1.mp3,          160, CI_AUDIO )
#define RES_HIT2_SOUND          CINDER_RESOURCE( ../resources/, Hit_Hurt2.mp3,          161, CI_AUDIO )
#define RES_EXPLOSION1_SOUND    CINDER_RESOURCE( ../resources/, Explosion1.mp3,         162, CI_AUDIO )
#define RES_EXPLOSION2_SOUND    CINDER_RESOURCE( ../resources/, Explosion2.mp3,         163, CI_AUDIO )
#define RES_EXPLOSION3_SOUND    CINDER_RESOURCE( ../resources/, Explosion3.mp3,         164, CI_AUDIO )

#define RES_BG_INTRO_SOUND      CINDER_RESOURCE( ../resources/, BG_intro.mp3,           170, CI_AUDIO )
#define RES_BG1_SOUND           CINDER_RESOURCE( ../resources/, BG1.mp3,                171, CI_AUDIO )
#define RES_BG2_SOUND           CINDER_RESOURCE( ../resources/, BG2.mp3,                172, CI_AUDIO )
#define RES_BG3_SOUND           CINDER_RESOURCE( ../resources/, BG3.mp3,                173, CI_AUDIO )
#define RES_BG4_SOUND           CINDER_RESOURCE( ../resources/, BG4.mp3,                174, CI_AUDIO )
#define RES_BG5_SOUND           CINDER_RESOURCE( ../resources/, BG5.mp3,                175, CI_AUDIO )
#define RES_BG6_SOUND           CINDER_RESOURCE( ../resources/, BG6.mp3,                176, CI_AUDIO )
#define RES_BG7_SOUND           CINDER_RESOURCE( ../resources/, BG7.mp3,                177, CI_AUDIO )
#define RES_BG8_SOUND           CINDER_RESOURCE( ../resources/, BG8.mp3,                178, CI_AUDIO )

#else               // Windows only loads WAV's via cinder

#define RES_HIT1_SOUND          CINDER_RESOURCE( ../resources/, Hit_Hurt1.wav,          160, CI_AUDIO )
#define RES_HIT2_SOUND          CINDER_RESOURCE( ../resources/, Hit_Hurt2.wav,          161, CI_AUDIO )
#define RES_EXPLOSION1_SOUND    CINDER_RESOURCE( ../resources/, Explosion1.wav,         162, CI_AUDIO )
#define RES_EXPLOSION2_SOUND    CINDER_RESOURCE( ../resources/, Explosion2.wav,         163, CI_AUDIO )
#define RES_EXPLOSION3_SOUND    CINDER_RESOURCE( ../resources/, Explosion3.wav,         164, CI_AUDIO )

#define RES_BG_INTRO_SOUND      CINDER_RESOURCE( ../resources/, BG_intro.wav,           170, CI_AUDIO )
#define RES_BG1_SOUND           CINDER_RESOURCE( ../resources/, BG1.wav,                171, CI_AUDIO )
#define RES_BG2_SOUND           CINDER_RESOURCE( ../resources/, BG2.wav,                172, CI_AUDIO )
#define RES_BG3_SOUND           CINDER_RESOURCE( ../resources/, BG3.wav,                173, CI_AUDIO )
#define RES_BG4_SOUND           CINDER_RESOURCE( ../resources/, BG4.wav,                174, CI_AUDIO )
#define RES_BG5_SOUND           CINDER_RESOURCE( ../resources/, BG5.wav,                175, CI_AUDIO )
#define RES_BG6_SOUND           CINDER_RESOURCE( ../resources/, BG6.wav,                176, CI_AUDIO )
#define RES_BG7_SOUND           CINDER_RESOURCE( ../resources/, BG7.wav,                177, CI_AUDIO )
#define RES_BG8_SOUND           CINDER_RESOURCE( ../resources/, BG8.wav,                178, CI_AUDIO )

#endif

#define RES_VERT_SHADER         CINDER_RESOURCE( ../resources/, vert.glsl,              190, CI_SHADER )
#define RES_FRAG_SHADER         CINDER_RESOURCE( ../resources/, frag.glsl,              191, CI_SHADER )
