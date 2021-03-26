#include <types.h>

/* Buttons */

#define CONT_A      0x8000
#define CONT_B      0x4000
#define CONT_G	    0x2000
#define CONT_START  0x1000
#define CONT_UP     0x0800
#define CONT_DOWN   0x0400
#define CONT_LEFT   0x0200
#define CONT_RIGHT  0x0100
#define CONT_L      0x0020
#define CONT_R      0x0010
#define CONT_E      0x0008
#define CONT_D      0x0004
#define CONT_C      0x0002
#define CONT_F      0x0001

/* Nintendo's official button names */

#define A_BUTTON	CONT_A
#define B_BUTTON	CONT_B
#define L_TRIG		CONT_L
#define R_TRIG		CONT_R
#define Z_TRIG		CONT_G
#define START_BUTTON	CONT_START
#define U_JPAD		CONT_UP
#define L_JPAD		CONT_LEFT
#define R_JPAD		CONT_RIGHT
#define D_JPAD		CONT_DOWN
#define U_CBUTTONS	CONT_E
#define L_CBUTTONS	CONT_C
#define R_CBUTTONS	CONT_F
#define D_CBUTTONS	CONT_D

struct ControllerData {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 vec3f_magnitude;
    /* 0x0C */ s16 buttonHeld;
    /* 0x0E */ s16 buttonPressed;
    /* 0x10 */ s16 buttonReleased;
};

extern struct ControllerData gControllerData;

int play_sound(u32 number, s32 arg1, s32 volume, s32 pan);
int MusHandleStop(int handle, int speed);

s32 sound = 0x0D;
int handle = 0;

void soundtest()
{
    if (gControllerData.buttonPressed & L_JPAD)
    {
        sound--;
        if (sound < 0) sound = 0;
    }
    if (gControllerData.buttonPressed & R_JPAD)
    {   
        sound++;
        if (sound > 0xa1) sound = 0xa1;
    }
    if (gControllerData.buttonPressed & U_JPAD)
    {
        MusHandleStop(handle, 0);
        handle = play_sound(sound, 0x00, 0x80, 0x80);
    }
    if (gControllerData.buttonPressed & D_JPAD)
    {
        MusHandleStop(handle, 0);
    }
}
