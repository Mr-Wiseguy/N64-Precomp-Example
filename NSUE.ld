OUTPUT_ARCH (mips)

#define BEGIN_SEG(name, addr) \
    _##name##SegmentStart = ADDR(.name); \
    _##name##SegmentRomStart = __romPos; \
    .name addr : AT(__romPos)

#define END_SEG(name) \
    _##name##SegmentEnd = ADDR(.name) + SIZEOF(.name); \
    _##name##SegmentRomEnd = __romPos + SIZEOF(.name); \
    _##name##SegmentSize = _##name##SegmentEnd - _##name##SegmentStart; \
    __romPos += SIZEOF(.name);

#define BEGIN_NOLOAD(name) \
    _##name##SegmentNoloadStart = ADDR(.name.noload); \
    .name.noload (NOLOAD) :

#define END_NOLOAD(name) \
    _##name##SegmentNoloadEnd = ADDR(.name.noload) + SIZEOF(.name.noload);

#define PATCH(segment, symbol, offset) \
    .segment##symbol##offset##_patch symbol + offset : AT(_##segment##SegmentRomStart + symbol - _##segment##SegmentStart + offset)
    
#define CODE_PATCH(segment, symbol, offset) \
    .segment##symbol##offset##_patch symbol + offset : AT(code_##segment##_ROM_START + symbol - code_##segment##_VRAM + offset)

#define JAL_HOOK(segment, symbol, offset, helper, name) \
    .segment##symbol##offset##_patch symbol + offset : AT(code_##segment##_ROM_START + symbol - code_##segment##_VRAM + offset) \
    { \
        name = .; \
        BYTE(0x0C); \
        BYTE((helper >> 18) & 0xFF); \
        BYTE((helper >> 10) & 0xFF); \
        BYTE((helper >> 2)  & 0xFF); \
    }
    
#define ROM_PATCH(address) \
    .rom##address##_patch : AT(address)

SECTIONS
{
    __romPos = 0;

    BEGIN_SEG(baserom, 0)
    {
        BASEROM;
    }
    END_SEG(baserom)
    baseromEnd = __romPos;
    extRamStart = 0xFFFFFFFF80400000;
    . = extRamStart;
    BEGIN_SEG(custom, .)
    {
        BUILD_DIR/src/custom/soundtest_helper.o(.text);
        BUILD_DIR/src/custom/soundtest.o(.text);
        BUILD_DIR/src/custom/drawing.o(.text);
        BUILD_DIR/src/custom/soundtest.o(.*data*);
        BUILD_DIR/src/custom/drawing.o(.*data*);
    }
    END_SEG(custom)

    CODE_PATCH(codesegs0_1, osLeoDiskInit, 0x0)
    {
        BUILD_DIR/src/patches/dma_code.o(.text);
    }

    /* Replace a call to create_scheduler with a call to DMA custom code into ext ram */
    JAL_HOOK(codesegs0_1, thread6_unk, 40, dma_code, dma_hook)
    
    /* Replace a call so that soundtest_helper is called instead */
    JAL_HOOK(codeseg2, func_8005AAE0, 44, soundtest_helper, soundtest_hook)
    
    /* Replace a call so that soundtest_helper is called instead */
    JAL_HOOK(codeseg2, func_80046B40, 0x148, soundtest_draw, schedule_gfx_hook_1)
    
    /* Replace a call so that soundtest_helper is called instead */
    JAL_HOOK(codeseg2, func_8007F22C, 0x100, soundtest_draw, schedule_gfx_hook_2)

    ROM_PATCH(0x000000)
    {
        BUILD_DIR/src/patches/header.o(.data);
    }

    /* Discard everything not specifically mentioned above. */
    /DISCARD/ :
    {
        *(*);
    }
}
