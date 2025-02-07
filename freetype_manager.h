#ifndef FREETYPE_MANAGER_H
#define FREETYPE_MANAGER_H

#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct {
    FT_Library library;
    FT_Face face;
    u_char *bitmap_data;
} FreeTypeManager;

extern FreeTypeManager ft_manager;

int initFreeType(FreeTypeManager *ft_manager, const char *font_file, int pixel_size);

void cleanupFreeType(FreeTypeManager *ft_manager);

void getChar(FreeTypeManager *ft_manager, int code);

#endif // FREETYPE_MANAGER_H
