#include "freetype_manager.h"
#include <stdio.h>

FreeTypeManager ft_manager;

int initFreeType(FreeTypeManager *ft_manager, const char *font_file, int pixel_size) {
    FT_Error error;

    error = FT_Init_FreeType(&ft_manager->library);
    if (error) {
        fprintf(stderr, "Ошибка инициализации FreeType\n");
        return -1;
    }

    error = FT_New_Face(ft_manager->library, font_file, 0, &ft_manager->face);
    if (error) {
        fprintf(stderr, "Ошибка загрузки шрифта: %s\n", font_file);
        FT_Done_FreeType(ft_manager->library);
        return -1;
    }

    error = FT_Set_Pixel_Sizes(ft_manager->face, 0, pixel_size);
    if (error) {
        fprintf(stderr, "Ошибка установки размера шрифта\n");
        FT_Done_Face(ft_manager->face);
        FT_Done_FreeType(ft_manager->library);
        return -1;
    }

    printf("Freetype inited!\n");
    return 0;
}

void cleanupFreeType(FreeTypeManager *ft_manager) {
    free(ft_manager->bitmap_data);
    FT_Done_Face(ft_manager->face);
    FT_Done_FreeType(ft_manager->library);
}

void getChar(FreeTypeManager *ft_manager, int code) {
    FT_Error error;

    error = FT_Load_Char(ft_manager->face, code, FT_LOAD_RENDER );
    if (error) {
        fprintf(stderr, "Ошибка рендеринга символа\n");
        return;
    }

    FT_Bitmap *bitmap = &ft_manager->face->glyph->bitmap;

    int fixed_width = 24;
    int fixed_height = 25;
    int byte_width = (fixed_width + 7) / 8;

    if (ft_manager->bitmap_data != NULL) {
        free(ft_manager->bitmap_data);
    }

    ft_manager->bitmap_data = malloc(byte_width * fixed_height);
    if (!ft_manager->bitmap_data) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return;
    }

    memset(ft_manager->bitmap_data, 0, byte_width * fixed_height);

    int offset_x = (fixed_width - bitmap->width) / 2;
    int offset_y = (fixed_height - bitmap->rows) / 2;

    if (bitmap->width > 24 || bitmap->rows > 25)
        return;
    
    for (int i = 0; i < bitmap->rows; i++) {
        for (int j = 0; j < bitmap->width; j++) {
            unsigned char gray = bitmap->buffer[i * bitmap->pitch + j];
            if (gray > 128) {
                int x = j + offset_x;
                int y = i + offset_y;
                ft_manager->bitmap_data[y * byte_width + (x / 8)] |= (0x80 >> (x % 8));
            }
        }
    }

}

