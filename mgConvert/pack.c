#include "pack.h"
#include "leafpack.h"
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <iconv.h>
#include <sys/types.h>
#include "pakconv.h"

char* textbuf;

unsigned char* scnbuffer;
size_t scnsize;

unsigned char* textbuffer;
size_t textsize;

FILE *textFile;
int bytenum;

typedef struct {
    int size;
    const void* context;
    int offset;
    bool withoffset;
} write_args;

#define scnwrite(...)  var_writescnbuf((write_args){__VA_ARGS__})
#define textwrite(...) var_writetextbuf((write_args){__VA_ARGS__})

void base_writebuf(unsigned char** buffer, size_t *bufsize, int size, const void* content, int offset, bool withoffset)
{
    if(!withoffset)
    {
        *buffer = (unsigned char*)realloc(*buffer, *bufsize + size);
        memcpy(*buffer + *bufsize, content, size);
        *bufsize += size;  
    }
    else 
    {
        memcpy(*buffer + offset, content, size);
    }
}

void base_writescnbuf(int size, const void* content, int offset, bool withoffset)
{
    base_writebuf(&scnbuffer, &scnsize, size, content, offset, withoffset);
}

void base_writetextbuf(int size, const void* content, int offset, bool withoffset)
{
    base_writebuf(&textbuffer, &textsize, size, content, offset, withoffset);
}

void var_writescnbuf(write_args in)
{
    int offset_out = in.offset ? in.offset : 0;
    return(base_writescnbuf(in.size, in.context, offset_out, in.withoffset));
}

void var_writetextbuf(write_args in)
{
    int offset_out = in.offset ? in.offset : 0;
    bool withoffset_out = in.withoffset ? in.withoffset : 0;
    return(base_writetextbuf(in.size, in.context, offset_out, withoffset_out));
}


int GetIndexByChar(unsigned char* in)
{
    int code = -1;
    if(in[0] == 0xa1 && in[1] == 0xa1)
        return 0;
    for(int i = 0; i < fonttable_size; i+=2)
    {
        if(fonttable_buffer[i] == in[0] && fonttable_buffer[i+1] == in[1])
        {
            code = i / 2;
            break;
        }
    }
    if(code == -1)
        printf("Code for symbol \"%s\" codes  not found!\n", in);
    return code;
}

unsigned char *ConvertToEUCJP(unsigned char* a)
{
    size_t in_bytes, out_bytes;
    iconv_t cd;
    static unsigned char buf[10];
   // unsigned char *in = a;
    unsigned char* out = buf;
    cd = iconv_open("EUC-JP", "UTF-8");
    
    in_bytes = 3;
    out_bytes = sizeof(buf);
    memset(buf, 0, sizeof(buf));
    
    iconv(cd, (char**)&a, &in_bytes, (char **)&out, &out_bytes);
    
    iconv_close(cd);
    
    return buf;
}

int GetHeaderCount()
{
    int count = 0;
    char* buf = strdup(textbuf);
    while((buf = strstr(buf, "begin")) ) 
    {
        count++;
        buf += 5;
    }
    return count;
}

void ParseMsg(int number)
{
    int c = 0;
    char* tmp = NULL;
    if(textbuf != NULL)
        tmp = strdup(textbuf);
    while((tmp = strstr(tmp, "begin")) && c != number) 
    {
        c++;
        tmp += 5;
    }
    if(tmp && c == number)
    {
        unsigned char binbufa[2] = {bytenum & 0xFF, (bytenum >> 8) & 0xFF};
        textwrite(2, binbufa, 0, true);
        printf("%02x %02x\n", binbufa[0], binbufa[1]);
        
        int offseta = textsize;
        unsigned char binbuf[2] = {offseta & 0xFF, (offseta >> 8) & 0xFF};
        textwrite(2, binbuf, c*2+2, true); 
        
        char* line = strtok(tmp, "\n");
        while(line != NULL)
        {
            if(strncmp(line,"{", 1) == 0)
            {
                bool ends_with_nl;
                size_t len = strlen(line);
                if( len >= 2 && line[len - 2] == 'n' && line[len - 1] == 'l')
                    ends_with_nl = true;
                else ends_with_nl = false;
                
                int start_from;
                if(line[0] == '{')
                    start_from = 1;
                else start_from = 0;
                
                for(int i = start_from; i < len; i+=3)
                {
                    if(ends_with_nl && i + 3 > (len - 2))
                    {
                        unsigned char binBuff = 0x72;
                        textwrite(1, &binBuff);  
                        break;
                    }
                    
                    if(line[i] == '}' || line[i+1] == '}' || line[i+2] == '}') 
                        continue;
                    unsigned char utf8_char[3] = {line[i], line[i+1], line[i+2]};
                    int code = GetIndexByChar(ConvertToEUCJP(utf8_char));
                    unsigned char byte1 = (code >> 8) & 0x7f | 0x80 ;
                    unsigned char byte2 = code & 0xFF;
                    unsigned char buf[2] = {byte1, byte2};
                    textwrite(2, buf);                     
                }
            }
            else if(strncmp(line,"keywait", strlen("keywait")) == 0)
            {
                unsigned char binBuff = 0x6b;
                textwrite(1, &binBuff);
            }
            else if(strncmp(line,"pagewait", strlen("pagewait")) == 0)
            {
                unsigned char binBuff = 0x70;
                textwrite(1, &binBuff);
            }
            else if(strncmp(line,"end", 3) == 0)
            {
                unsigned char binBuff = 0x24;
                textwrite(1, &binBuff);
                break;
            }
            else if(strncmp(line,"disppos:", strlen("disppos:")) == 0)
            {
                int commandBuffer;
                sscanf(line+strlen("disppos:"), "%x", &commandBuffer);
                unsigned char binBuff[2] = {0x58, commandBuffer};
                textwrite(2, binBuff);
            }
            else if(strncmp(line,"dispspeed:", strlen("dispspeed:")) == 0)
            {
                int commandBuffer;
                sscanf(line+strlen("dispspeed:"), "%x", &commandBuffer);
                unsigned char binBuff[2] = {0x73, commandBuffer};
                textwrite(2, binBuff);
            }
             
            else if(strncmp(line,"nl", 2) == 0)
            {
                unsigned char binBuff = 0x72;
                textwrite(1, &binBuff);
            }
            line = strtok(NULL,"\n");
        }
        if (c == bytenum - 1)
        {
            int offsetlast = textsize;
            //fseek(outFile1, bytenum * 2 + 2, SEEK_SET);
            unsigned char binBuff[2] = {offsetlast & 0xFF, (offsetlast >> 8) & 0xFF};
            textwrite(2, binBuff, bytenum * 2 + 2, true);
        }
    }
    
}

void ReverseParse(const char *inputFile1, const char *inputFile2) 
{
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
    
    
    FILE *cmdFile = fopen(inputFile1, "r");
    textFile = fopen(inputFile2, "r");
    
    fseek(textFile, 0, SEEK_END);
    size_t ftextsize = ftell(textFile);
    fseek(textFile, 0, SEEK_SET);
    textbuf = (char*)malloc(ftextsize);
    fread(textbuf, 1, ftextsize, textFile);
    
    // FILE *outFile = fopen(outputFile, "wb");
    // outFile1 = fopen("text", "wb");
    
    if (!cmdFile || !textFile) {
        fprintf(stderr, "Error opening files.\n");
        return;
    }
    
    bytenum = GetHeaderCount();
    for (int i = 0; i < bytenum + 2; i++)
    {
        unsigned char header[2] = {0x00, 0x00};
        textwrite(2, header);
        
    }
    
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    unsigned char header[11] = {0x03, 0x00, 0x0a, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    scnwrite(11, header);
    while ((read = getline(&line, &len, cmdFile)) != -1) {
            printf("readline %s", line);
            if(line[read - 1] == '\n')
                line[read - 1] = '\0';
        
            if(strncmp(line,"BGMplay:" , strlen("BGMplay:")) == 0)
            {
                unsigned int commandBuffer[1];
                sscanf(line+strlen("BGMplay:"), "%x",commandBuffer);
                printf("%#02x\n", commandBuffer[0]);
                unsigned char binBuff[2] = { 0x6e, commandBuffer[0]};
                scnwrite(2, binBuff);
            }
            else if(strncmp(line,"bgload:" , strlen("bgload:")) == 0)
            {
                unsigned int commandBuffer[1];
                sscanf(line+strlen("bgload:"), "%d",commandBuffer);
                printf("%d\n", commandBuffer[0]);
                unsigned char binBuff[2] = {0x0a, commandBuffer[0]};
                scnwrite(2, binBuff);
            }   
            else if(strncmp(line,"dispproc:" , strlen("dispproc:")) == 0)
            {
                unsigned int commandBuffer;
                sscanf(line+strlen("dispproc:"), "%x",&commandBuffer);
                printf("%#02x\n", commandBuffer);
                unsigned char binBuff[2] = {0x38, commandBuffer};
                scnwrite(2, binBuff);
            } 
            else if(strncmp(line,"return2choise" , strlen("return2choise")) == 0)
            {
                unsigned char binBuff = 0x07;
                scnwrite(1, &binBuff);
            }
            else if(strncmp(line,"msg:",strlen("msg:")) == 0)
            {
                int commandBuffer;
                sscanf(line+strlen("msg:"), "%d", &commandBuffer);
                unsigned char binBuff[2] = {0x54, commandBuffer};
                ParseMsg(commandBuffer);
                scnwrite(2, binBuff);
            }
            else if(strncmp(line, "if flag:", strlen("if flag:")) == 0)
            {
                unsigned int commandBuffer[3];
                sscanf(line+strlen("if flag:"), "%x %x %x",&commandBuffer[0], &commandBuffer[1], &commandBuffer[2]);
                printf("%#02x %#02x %#02x\n", commandBuffer[0], commandBuffer[1], commandBuffer[2]);
                unsigned char binBuff[4] = {0x3d, commandBuffer[0], commandBuffer[1], commandBuffer[2]};
                scnwrite(4, binBuff);
            }
            else if(strncmp(line, "if !flag:", strlen("if !flag:")) == 0)
            {
                unsigned int commandBuffer[3];
                sscanf(line+strlen("if !flag:"), "%x %x %x",&commandBuffer[0], &commandBuffer[1], &commandBuffer[2]);
                printf("%#02x %#02x %#02x\n", commandBuffer[0], commandBuffer[1], commandBuffer[2]);
                unsigned char binBuff[4] = {0x3e, commandBuffer[0], commandBuffer[1], commandBuffer[2]};
                scnwrite(4, binBuff);
            }
            else if(strncmp(line, "Unknown:", strlen("Unknown:")) == 0)
            {
                unsigned int commandBuffer[2];
                int matched = sscanf(line+strlen("Unknown:"), "%x %x",&commandBuffer[0], &commandBuffer[1]);
                printf("%d %#02x %#02x \n",matched, commandBuffer[0], commandBuffer[1]);
                if (matched==1){
                    unsigned char binBuff[1] = {commandBuffer[0]};
                    scnwrite(1, binBuff);
                }
                else if(matched==2){
                    unsigned char binBuff[2] = {commandBuffer[0], commandBuffer[1]};
                    scnwrite(2, binBuff);
                }
                
            }
            else if(strncmp(line,"jump:" , strlen("jump:")) == 0)
            {
                unsigned int commandBuffer[2];
                sscanf(line+strlen("jump:"), "%d %d",&commandBuffer[0], &commandBuffer[1]);
                printf("%d %d\n", commandBuffer[0], commandBuffer[1]);
                unsigned char binBuff[3] = {0x04, commandBuffer[0], commandBuffer[1]};
                scnwrite(3, binBuff);
            }
            else if(strncmp(line, "choisemsg:",strlen("choisemsg:")) == 0)
            {
                unsigned int commandBuffer[2];
                sscanf(line+strlen("choisemsg:"), "%d %d",&commandBuffer[0], &commandBuffer[1]);
                printf("%d %d\n", commandBuffer[0], commandBuffer[1]);
                unsigned char binBuff[3] = {0x05, commandBuffer[1], commandBuffer[0]};
                ParseMsg(commandBuffer[1]);
                scnwrite(3, binBuff);
                for(int i = 0; i < commandBuffer[0]; i++)
                {
                    int choiseMsg, choiseOffset;
                    read = getline(&line, &len, cmdFile);
                    printf("readline %s", line);
                    sscanf(line+strlen("\tchoise:"), "%d %d",&choiseMsg, &choiseOffset);
                    printf("%d %d\n", choiseMsg, choiseOffset);
                    ParseMsg(choiseMsg);
                    unsigned char binBuff[2] = {choiseMsg, choiseOffset};
                    scnwrite(2, binBuff);
                }
            }
            else if(strncmp(line,"datbchoice" , strlen("datbchoice")) == 0)
            {
                unsigned char binBuff = 0x28;
                scnwrite(1, &binBuff);
            }
            else if(strncmp(line,"END" , strlen("END")) == 0)
            {
                unsigned char binBuff[2] = {0x00,0xff};
                scnwrite(2, binBuff);
            }
    }
    unsigned char fsize = textsize;
    unsigned char fsize2 = textsize - 1;
    scnwrite(1, &fsize2, 6, true);
    scnwrite(1, &fsize, 8, true);
    u_char *compressedData = (u_char *)malloc(scnsize);
    size_t compsize = 0;
    
    printf("DONE. textsize %ld scnsize %ld\n", textsize, scnsize);
    
    leafpack_lsz2_compress(scnbuffer, compressedData, scnsize, &compsize);
    printf("%ld\n", compsize);
    
    for(int i = 0, j = 0; i < compsize; i++ )
    {
        if(j == 16)
        {
            printf("\n");
            j = 0;
        }
        printf("%02x ", compressedData[i]);
        j++;
    }
//     
    printf("\nscn bytes:\n");
//     for(int i = 0, j = 0; i < scnsize; i++ )
//     {
//         if(j == 16)
//         {
//             printf("\n");
//             j = 0;
//         }
//         printf("%02x ", scnbuffer[i]);
//         j++;
//     }
//     printf("\n");

    fclose(cmdFile);    
    fclose(textFile);
    free(line);
    free(textbuffer);
    free(scnbuffer);

}
