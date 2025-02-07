/*
 * LEAF Visual Novel System For MGL2
 * Simple File Extractor
 * (c) Copyright 2024
 * 
 * $Id: pakextract.c,v 1.0 2024/09/01
 */
#include <bits/time.h>  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>


#include "pakconv.h"
#include "leafpack.h"
#include "extract.h"
#include "pack.h"

void extract_files(LeafPack *, const char *);
void pack_files(const char *, const char *);
void parse_scenario(const char*, const char*);

u_char *scn_data = NULL; 
u_char *scn_text = NULL;
unsigned char* fonttable_buffer = NULL;
size_t fonttable_size = 0;
size_t size_text = 0;

int
main(int argc, char *argv[])
{
    LeafPack *leafpack;

    if (argc < 4) {
        fprintf(stderr, "usage:\ne {.pak path, ouput dir} - extract files\np {pack name} - pack files\nf {.pac path, ouput dir} - unpack scenario files\nr {input1, input2, output} - pack scenario\n");
        exit(1);
    }

    if(strcmp(argv[1],"e") == 0)
    {
        if ((leafpack = leafpack_new(argv[2])) == NULL) {
            fprintf(stderr, "Can't open package %s.\n", argv[1]);
            exit(1);
        }

        extract_files(leafpack, argv[3]);

        leafpack_delete(leafpack);

        return 0;
    } 
    
    else if(strcmp(argv[1],"p") == 0)
    {
        pack_files(argv[2], argv[2]);
        return 0;
    }   
    
    else if(strcmp(argv[1],"f") == 0)
    {
        parse_scenario(argv[2], argv[3]);
        return 0;
    }
    else if(strcmp(argv[1],"r") == 0)
    {
        ReverseParse(argv[2], argv[3]);
        return 0;
    }
    else {
        fprintf(stderr, "usage: you should kys now\n");
        exit(1);
    }
}


void parse_scenario(const char* pac, const char* output_dir)
{
    int index;
    u_char *data;
    size_t file_len;
    LeafPack* lp = leafpack_new(pac);
    
    FILE *fonttable = fopen("sizfont.tbl", "rb");
    if(!fonttable)
    {
        printf("Can't open fonttable!\n");
        return;
    }
    fseek(fonttable,0 ,SEEK_END);
    fonttable_size = ftell(fonttable);
    fseek(fonttable,0,SEEK_SET );
    fonttable_buffer = (unsigned char*)malloc(fonttable_size);
    fread(fonttable_buffer, 1, fonttable_size, fonttable);
    fclose(fonttable);
    
    struct stat st = {0};
    if (stat(output_dir, &st) == -1) 
        mkdir(output_dir, 0700);
    
    for(int i = 0; i < lp->file_num; i++)
    {
        char* name_pac = lp->files[i].name;
        char* ext = strchr(name_pac, '.');

        if(strcasecmp(ext, ".dat"))
            continue;
        
        if(strcasecmp(name_pac, "SCN001.dat"))
            continue;
        printf("Parsing %s\n", name_pac);
        
        
        if ((index  = leafpack_find(lp, name_pac)) < 0 || (data = leafpack_extract(lp, index, NULL)) == NULL) 
        {
            fprintf(stderr, "can't load scenario\n");
            exit(1);
        }
            u_char *p_scn  = data + GET_SHORT(data) * 16;
            u_char *p_text = data + GET_SHORT(data+2) * 16;
            
            size_t size_scn  = GET_LONG(p_scn);
            size_text = GET_LONG(p_text);
        
            scn_data = (u_char*)malloc(size_scn);
            scn_text = (u_char*)malloc(size_text);
            
            for(int i = 0, j = 0; i < size_scn; i++ )
            {
                if(j == 16)
                {
                    printf("\n");
                    j = 0;
                }
                printf("%02x ", p_scn[i]);
                j++;
            }
            printf("\n");
            
            leafpack_lzs2(p_scn + 4, scn_data, size_scn );
            leafpack_lzs2(p_text + 4, scn_text, size_text );
            
            char name[256];
            // *ext = '\0';
            snprintf(name,sizeof(name),"%s/%s.txt", output_dir, name_pac);

            ScenParse(name, scn_data + GET_SHORT(scn_data + (1 + 1 ) * 2));
            
            free(data);
            free(scn_text);
            free(scn_data);
        }
        free(text_buf);
        free(fonttable_buffer);
}




void pack_files(const char* pack, const char *input_dir) {

    int fileCount = 0;
    struct dirent *entryCount;
    DIR* dir = opendir(input_dir);
    
    while ((entryCount = readdir(dir)) != NULL) {
        if (entryCount->d_type == DT_REG) {
            fileCount++;
        }
    }
    closedir(dir);
    dir = opendir(input_dir);
    struct dirent *entry;
    PakWriter* pw = pakwriter_new(pack, fileCount);
    
    while ((entry = readdir(dir)) != NULL) {
        char file_path[1024];
        FILE *file;
        u_char *data;
        size_t size;

        if (entry->d_type != DT_REG) continue;

        snprintf(file_path, sizeof(file_path), "%s/%s", input_dir, entry->d_name);
        printf("packing %s/%s\n", input_dir, entry->d_name);
        
        file = fopen(file_path, "rb");
        if (file == NULL) {
            fprintf(stderr, "Can't open file %s.\n", file_path);
            continue;
        }
        
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        fseek(file, 0, SEEK_SET);

        data = (u_char *)malloc(size);
        if (data == NULL) {
            fprintf(stderr, "Error while allocation memory for file %s.\n", file_path);
            fclose(file);
            continue;
        }

        fread(data, 1, size, file);
        fclose(file);

        put_file(pw, data, size, entry->d_name);

        free(data);
        printf("done\n");
        // pw->file_num++;
    }

    printf("All done\n");
    closedir(dir);

    pakwriter_delete(pw);
}


void
extract_files(LeafPack *lp, const char *output_dir)
{
    int i;
    char output_path[1024];

    struct stat st = {0};
    if (stat(output_dir, &st) == -1) 
        mkdir(output_dir, 0700);
    
    for (i = 0; i < lp->file_num; i++) {
        char *name = lp->files[i].name;
        int size;
        u_char *data;
        FILE *outfile;

        fprintf(stderr, "Extracting %s...\n", name);

        int idx = leafpack_find(lp, name);
        if (idx < 0) {
            fprintf(stderr, "Can't find file: %s\n", name);
            continue;
        }
        data = leafpack_extract(lp, idx, &size);
        if (data == NULL) {
            fprintf(stderr, "Can't extract file: %s\n", name);
            continue;
        }
        
        snprintf(output_path, sizeof(output_path), "%s/%s", output_dir, name);

        outfile = fopen(output_path, "wb");
        if (outfile == NULL) {
            fprintf(stderr, "Can't open output file: %s\n", output_path);
            free(data);
            continue;
        }

        fwrite(data, 1, size, outfile);
        fclose(outfile);

        free(data);

        fprintf(stderr, "done.\n");
    }
}
