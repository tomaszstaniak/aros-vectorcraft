/* ASL file requester with a short or a long initial pattern.
   Usage: asltest short|long <logfile>
   The requester opens on RAM:; the tester types a file name and presses
   Return. The result (file, drawer, pattern) goes to <logfile>, followed
   by an allocation loop that walks the heap. */

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/asl.h>
#include <libraries/asl.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    char pattern[400];
    struct FileRequester *fr;
    FILE *log;
    BOOL ok;
    int i;

    if (argc != 3)
        return 20;
    if (strcmp(argv[1], "long") == 0)
    {
        /* 298 characters, like VectorCraft's Open pattern */
        strcpy(pattern, "#?.(");
        while (strlen(pattern) < 290)
            strcat(pattern, "ext|");
        strcat(pattern, "svg|pdf)");
        while (strlen(pattern) < 298)
            strcat(pattern, "x");
    }
    else
        strcpy(pattern, "#?.(txt|svg)");

    log = fopen(argv[2], "w");
    if (!log)
        return 20;
    fprintf(log, "mode=%s initial pattern length=%lu\n", argv[1], (unsigned long)strlen(pattern));
    fflush(log);

    fr = AllocAslRequestTags(ASL_FileRequest,
                             ASLFR_TitleText, (IPTR)"asltest",
                             ASLFR_InitialDrawer, (IPTR)"RAM:",
                             ASLFR_InitialFile, (IPTR)"start",
                             ASLFR_InitialPattern, (IPTR)pattern,
                             ASLFR_DoPatterns, TRUE,
                             TAG_DONE);
    if (!fr)
    {
        fprintf(log, "AllocAslRequest failed\n");
        fclose(log);
        return 20;
    }
    ok = AslRequest(fr, NULL);
    fprintf(log, "AslRequest=%d file=[%s] drawer=[%s]\n", (int)ok,
            fr->fr_File ? (char *)fr->fr_File : "(null)",
            fr->fr_Drawer ? (char *)fr->fr_Drawer : "(null)");
    fprintf(log, "pattern length=%lu pattern=[%s]\n",
            fr->fr_Pattern ? (unsigned long)strlen((char *)fr->fr_Pattern) : 0UL,
            fr->fr_Pattern ? (char *)fr->fr_Pattern : "(null)");
    fflush(log);
    FreeAslRequest(fr);

    for (i = 0; i < 2000; i++)
    {
        APTR p = AllocVec(16 + (i % 61) * 8, MEMF_ANY);
        if (!p)
            break;
        FreeVec(p);
    }
    fprintf(log, "allocation loop done (%d)\nEND\n", i);
    fclose(log);
    return 0;
}
