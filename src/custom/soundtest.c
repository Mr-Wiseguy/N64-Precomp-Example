#include <ultra64.h>
#include <mus/libmus.h>
#include <types.h>

void drawSmallString(Gfx **dl, int x, int y, const char* string);
void drawSmallStringCol(Gfx **dl, int x, int y, const char* string, unsigned char r, unsigned char g, unsigned char b);

extern struct ControllerData gControllerData;
extern struct GfxContext gGfxContext;

musHandle play_sound(u32 number, s32 arg1, s32 volume, s32 pan);

s32 sound = 0x0D;
musHandle handle = 0;

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

s32 hex_char_from_nibble(s32 val)
{
    if (val >= 0x0A)
    {
        return val + ('A' - 0x0A);
    }
    else
    {
        return val + '0';
    }
}

void schedule_gfx_task();

void soundtest_draw()
{
    Gfx *dlHead = gGfxContext.dlHead;
    char text[9];
    text[0] = 'S';
    text[1] = 'o';
    text[2] = 'u';
    text[3] = 'n';
    text[4] = 'd';
    text[5] = ':';
    text[6] = hex_char_from_nibble((sound >> 4) & 0xF);
    text[7] = hex_char_from_nibble((sound >> 0) & 0xF);
    text[8] = '\0'; 
    
    gDPSetCycleType(dlHead++, G_CYC_1CYCLE);
    gDPSetRenderMode(dlHead++, G_RM_TEX_EDGE, G_RM_TEX_EDGE2);
    gDPSetTexturePersp(dlHead++, G_TP_NONE);
    gDPSetTextureFilter(dlHead++, G_TF_POINT);
    gDPSetTextureLUT(dlHead++, G_TT_NONE);
    drawSmallStringCol(&dlHead, 100, 30, text, 255, 255, 255);

    gGfxContext.dlHead = dlHead;

    schedule_gfx_task();
}
