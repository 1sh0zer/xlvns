#include "extract.h"
#include "pakconv.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iconv.h>
#include <stdbool.h>

#define HexToDig(c) ((tolower(c) >= 'a')? (tolower(c) -'a' + 10) : (c - '0'))
#define HexToDig2(c,c2) (HexToDig(c)*16 + HexToDig(c2))
#define Dig(c1,c2) (c1 - '0') * 10 + (c2 - '0')

unsigned char* text_buf = NULL;

unsigned char* GetCharAtIndex(int code)
{
    int index = code * 2;
    
    if(index + 1 < fonttable_size)
    {
        if(text_buf)
            free(text_buf);
        text_buf = (unsigned char*)malloc(2);
        text_buf[0] = fonttable_buffer[index];
        text_buf[1] = fonttable_buffer[index+1];
        return text_buf;
    }
    else
    {
        printf("Code %d out of bounds\n", code);
        return 0;
    }
}

unsigned char* ConvertToUTF8(int code)
{
    size_t in_bytes, out_bytes;
    iconv_t cd;
    char buf[10];
    unsigned char* in = GetCharAtIndex(code);
    char* out = buf;
    cd = iconv_open("UTF-8", "EUC-JP");
    
    in_bytes = 2;
    out_bytes = sizeof(buf);
    memset(buf, 0, sizeof(buf));
    
    iconv(cd, (char**)&in, &in_bytes, &out, &out_bytes);
    
    iconv_close(cd);
    
    return strdup(buf);
}

void ParseText(const u_char *c,FILE *msgFile)
{
    bool isFirst = true;
    if (!msgFile) {
        fprintf(stderr, "Error opening log file\n");
        return;
    }
    fprintf(msgFile, "begin\n");

    while(1) {
        if (c[0] & 0x80) {
            if(isFirst){
                fprintf(msgFile,"{");
                isFirst = false;
            }
            int FontCode = ((c[0] & 0x7f) << 8) + c[1];
            fprintf(msgFile, "%s", ConvertToUTF8(FontCode));
            c += 2;
            if(!(c[0] & 0x80))
               fprintf(msgFile,"}");
        } else switch (c[0]) {

        case '$': /* '$' メッセージ終了 */
            fprintf(msgFile, "end\n\n");
            c++;
            //fclose(msgFile);  // Закрываем файл перед завершением функции
            return;
            
        case 'r': /* 改行 */
            fprintf(msgFile, "nl\n");
            isFirst = true;
            c++;
            break;
            
        case 'p': /* ページ更新待ち */
            fprintf(msgFile, "\npagewait\n");
            c++;
            break;
            
        case 'k':
        case 'K': /* キー入力待ち */
            fprintf(msgFile, "\nkeywait\n");
            c++;
            break;
            
        case '0': /* スクリプトバグで '0' が余分についているところがある */
            fprintf(msgFile, "Ignoring '0'.\n");
            c++;
            break;
            
        case 'C': /* キャラクタ交換 */
            fprintf(msgFile, "swap:%c %c%c\n", c[1], c[2], c[3]);
            c += 4;
            break;
            
        case 'B': /* 背景表示 */
            fprintf(msgFile, "bg:%c%c %d %d\n", c[1], c[2], Dig(c[3],c[4]), Dig(c[5],c[6]));
            c += 7;
            break;
            
        case 'S': /* 背景つきキャラクタ表示 */
            fprintf(msgFile, "changebg %c %c%c %c%c %d %d\n",
                    c[1], c[2],c[3],c[4],c[5], Dig(c[6],c[7]), Dig(c[8], c[9]));
            c += 10;
            break;
            
        case 'D': /* キャラクタ全消去後表示 */
            fprintf(msgFile, "clear: %c %c%c\n", c[1], c[2], c[3]);
            c += 4;
            break;
            
        case 'A':
        case 'a':
            fprintf(msgFile, "3char:%c %c%c %c %c%c %c %c%c\n",
                    c[1],c[2],c[3],  c[4],c[5],c[6], c[7],c[8],c[9]);
            c += 10;
            break;
            
        case 'Q': /* 画面を揺らす */
            fprintf(msgFile, "shake\n");
            c++;
            break;
            
        case 'E': /* 背景表示2 ? */
            fprintf(msgFile, "bgload:%c %c %d %d\n",
                    c[1], c[2], Dig(c[3], c[4]), Dig(c[5], c[6]));
            c += 7;
            break;
            
        case 'F': /* フラッシュ */
            fprintf(msgFile, "flash\n");
            c++;
            break;
            
        case 'V': /* ビジュアルシーン表示 */
            fprintf(msgFile, "scene:%c%c %d %d\n",
                    c[1], c[2], Dig(c[3], c[4]), Dig(c[5], c[6]));
            c += 7;
            break;
            
        case 'H': /* Hシーン表示 */
            fprintf(msgFile, "hscene:%c%c %d %d\n",
                    c[1], c[2], Dig(c[3], c[4]), Dig(c[5], c[6]));
            c += 7;
            break;
            
        case 'M': /* BGM 関連 */
            {
                int c1 = c[1];
                c += 2;
                if (c1 == 'f') {
                    fprintf(msgFile, "BGMfade.\n");
                } else if (c1 == 'n') {
                    int c2 = *c++;
                    int c3 = *c++;
                    fprintf(msgFile, "BGMplaynext:%c%c\n", c2, c3);
                } else if (c1 == 'w') {
                    fprintf(msgFile, "BGMfwait\n");
                } else if (c1 >= '0' && c1 <= '2') {
                    int c2 = *c++;
                    fprintf(msgFile, "BGMplay:%c%c\n", c1, c2);
                } else if (c1 == 's') {
                    fprintf(msgFile, "BGMstop.\n");
                } else {
                    fprintf(msgFile, "Unknown BGM command: %#02x.\n", c1);
                }
            }
            break;
            
        case 'P': /* PCM 関連 */
            {
                int c1 = c[1];
                c += 2;
                if (c1 == 'l') {
                    int c2 = *c++;
                    int c3 = *c++;
                    fprintf(msgFile, "PCMload:%c%c\n", c2, c3);
                } else if (c1 >= '0' && c1 <= '9') {
                    int c2 = *c++;
                    int c3 = *c++;
                    int c4 = *c++;
                    fprintf(msgFile, "PCMplay:%c%c %c%c\n", c1, c2, c3, c4);
                } else if (c1 == 'f') {
                    fprintf(msgFile, "PCMfo\n");
                } else if (c1 == 'w') {
                    fprintf(msgFile, "PCMwait\n");
                } else if (c1 == 's') {
                    fprintf(msgFile, "PCMstop\n");
                } else {
                    fprintf(msgFile, "Unknown PCM command: %#02x.\n", c1);
                }
                break;
            }
            
        case 'X': /* 'X' 表示オフセット指定 */
            fprintf(msgFile, "disppos:%#02x\n", c[1]);
            c += 2;
            break;
            
        case 's': /* 's' 表示速度指定 */
            fprintf(msgFile, "dispspeed:%#02x\n", c[1]);
            c += 2;
            break;
            
        default:
            fprintf(msgFile, "Unknown command: %#02x.\n", c[0]);
            return;
        }
    } /* while(1) */

    fclose(msgFile);
}


void ScenParse(const char* fname, u_char* c)
{
    FILE *scnFile = fopen(fname, "w");
    
    const char* ext = strtok(fname, ".txt");
    char name[256];
    snprintf(name, sizeof(name), "%s_text.txt", ext);
    
    FILE *msgFile = fopen(name, "w");
    
    if (!scnFile) {
        fprintf(stderr, "Error opening log file\n");
        return;
    }

    while (1) {
        switch (c[0]) {
        case 0x00:
            fprintf(scnFile, "END\n");
            if(scnFile)
                fclose(scnFile);
            return;

        case 0x01:
            switch (c[1]) {
            case 0x01:
                fprintf(scnFile, "WtDmsg:%d\n", c[2]);
                break;
            case 0x02:
                fprintf(scnFile, "DtWmsg:%d\n", c[2]);
                break;
            case 0x03:
                fprintf(scnFile, "TD:%#02x\n", c[2]);
                break;
            case 0x04:
                fprintf(scnFile, "Wmsg:%d\n", c[2]);
                break;
            default:
                fprintf(scnFile, "Unknown 0x01 command (%#02x,%#02x)\n", c[1], c[2]);
                fclose(scnFile);
                return;
            }
            c += 3;
            break;

        case 0x03:
            fprintf(scnFile, "Unknown:%#02x %#02x\n", c[0], c[1]);
            c += 2;
            break;

        case 0x04:
            fprintf(scnFile, "jump:%d %d\n", c[1], c[2]);
            c+=3;
            break;

        case 0x05:
        {
            int i;
            fprintf(scnFile, "choisemsg:%d %d", c[2], c[1]);
            ParseText(scn_text + GET_SHORT(scn_text + (c[1]+1) * 2), msgFile);
            printf("Reading bytes %02x, %02x got %d for message %d\n", (scn_text + (c[1] + 1) * 2)[0], (scn_text + (c[1] + 1) * 2)[1], GET_SHORT(scn_text + (c[1] + 1 ) * 2), c[1]);
            for (i = 0; i < c[2]; i++) 
            {
                fprintf(scnFile, "\n\tchoise:%d %d", c[3+i*2], c[4+i*2]);
                printf("Reading bytes %02x, %02x got %d for message %d\n", (scn_text + (c[3+i*2] + 1) * 2)[0], (scn_text + (c[3+i*2] + 1) * 2)[1], GET_SHORT(scn_text + (c[3+i*2] + 1 ) * 2), c[3+i*2]);
                ParseText(scn_text + GET_SHORT(scn_text + (c[3+i*2]+1) * 2), msgFile);
            }//WTF HELP ME
            fprintf(scnFile, "\n");
            c = c + 3 + c[2]*2 + c[4 + 0];
            break;
        }
        case 0x06:
            fprintf(scnFile, "Unknown:%#02x\n", c[0]);
            c++;
            break;

        case 0x07:
            fprintf(scnFile, "return2choise\n");
            c++;
            break;

        case 0x0a:
            fprintf(scnFile, "bgload:%d\n", c[1]);
            c += 2;
            break;

        case 0x14:
            fprintf(scnFile, "clear:%d\n", c[1]);
            c += 2;
            break;

        case 0x16:
            fprintf(scnFile, "hscnload:MAX_H%d.LFG\n", c[1]);
            c += 2;
            break;

        case 0x22:
            fprintf(scnFile, "charload:MAX_C%#02x.LFG : %#02x\n", c[1], c[2]);
            c += 3;
            break;

        case 0x24:
            fprintf(scnFile, "charload2:MAX_C%#02x.LFG\n", c[1]);
            c += 3;
            break;

        case 0x28:
            fprintf(scnFile, "datbchoice\n");
            c++;
            break;

        case 0x38:
            fprintf(scnFile, "dispproc: %#02x\n", c[1]);
            c += 2;
            break;

        case 0x3d:
            fprintf(scnFile, "if flag:%#02x %#02x %#02x\n", c[1], c[2], c[3]);
            c += 4;
            break;

        case 0x3e:
            fprintf(scnFile, "if !flag:%#02x %#02x %#02x\n", c[1], c[2], c[3]);
            c += 4;
            break;

        case 0x47:
            fprintf(scnFile, "flag:%#02x = 0x%#02x\n", c[1], c[2]);
            c += 3;
            break;

        case 0x48:
            fprintf(scnFile, "flag:%#02x += 0x%#02x\n", c[1], c[2]);
            c += 3;
            break;

        case 0x54:
            fprintf(scnFile, "msg:%d\n", c[1]);
            ParseText(scn_text + GET_SHORT(scn_text + (c[1]+1) * 2), msgFile);
            printf("Reading bytes %02x, %02x got %d for message %d\n", (scn_text + (c[1] + 1) * 2)[0], (scn_text + (c[1] + 1) * 2)[1], GET_SHORT(scn_text + (c[1] + 1) * 2), c[1]);
            c += 2;
            break;

        case 0x5a:
        case 0x5c:
        case 0x61:
        case 0x62:
        case 0x60:
        case 0x63:
        case 0x64:
        case 0x65:
        case 0x66:
            fprintf(scnFile, "Unknown:%#02x\n", c[0]);
            c++;
            break;

        case 0x6e:
            fprintf(scnFile, "BGMplay:%#02x\n", c[1]);
            c += 2;
            break;

        case 0x6f:
        case 0x73:
            fprintf(scnFile, "Unknown:%#02x\n", c[0]);
            c++;
            break;

        case 0x7e:
            fprintf(scnFile, "endnumset %#02x\n", c[1]);
            c += 2;
            break;

        case 0x7c:
            fprintf(scnFile, "endingrelu");
            c++;
            break;

        case 0x7d:
            fprintf(scnFile, "endingbgm:%#02x %d\n", c[0], c[1]);
            c += 2;
            break;

        case 0xff:
            fprintf(scnFile, "Accessed an unreachable location?? 0xff \n");
            // fclose(scnFile);
            c++;
            break;

        default:
            fprintf(scnFile, "Uncaught command %#02x\n", c[0]);
            c++;
            //fclose(scnFile);
            break;;
        }

    } /* while(1) */
}
