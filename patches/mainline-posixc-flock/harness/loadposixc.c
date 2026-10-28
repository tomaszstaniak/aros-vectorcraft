/* Run one command with a given posixc.library file loaded, without touching
 * SYS:Libs. A resident posixc.library (system processes keep one open) is
 * renamed in the library list for the duration of the command, so that only
 * the loaded file answers to "posixc.library"; its openers keep their bases.
 * posixc registers nothing global at open (fd.library is only opened), so a
 * second instance is safe. Uses only exec and dos, never posixc.
 *   loadposixc <posixc.library path> <result file> <command line> */
#include <proto/exec.h>
#include <proto/dos.h>
#include <dos/dostags.h>
#include <exec/execbase.h>

static char *hidden_name;

static void unhide(struct Node *resident)
{
    if (resident && hidden_name) {
        Forbid();
        resident->ln_Name = hidden_name;
        Permit();
    }
}

int main(int argc, char **argv)
{
    struct Library *base, *loaded;
    struct Node *resident;
    BPTR out;
    LONG rc;

    if (argc != 4) {
        PutStr("loadposixc LIBRARY RESULTFILE COMMAND\n");
        return 20;
    }
    Forbid();
    resident = FindName(&SysBase->LibList, "posixc.library");
    if (resident) {
        hidden_name = resident->ln_Name;
        resident->ln_Name = (char *)"posixc.library (hidden by loadposixc)";
    }
    Permit();
    /* posixc has a per-task base and refuses openers without a C run-time,
       like this program; the failed open still leaves the loaded library in
       the list, where the test program, a C program, then finds it. */
    base = OpenLibrary(argv[1], 0);
    Forbid();
    loaded = (struct Library *)FindName(&SysBase->LibList, "posixc.library");
    Permit();
    if (!loaded || (struct Node *)loaded == resident) {
        Printf("loadposixc: %s did not load (%ld)\n", argv[1], IoErr());
        if (base) CloseLibrary(base);
        unhide(resident);
        return 20;
    }
    out = Open(argv[2], MODE_NEWFILE);
    if (!out) {
        if (base) CloseLibrary(base);
        unhide(resident);
        return 20;
    }
    FPrintf(out, "library %s version %ld.%ld at %p (%s), resident %s\n", argv[1],
            (LONG)loaded->lib_Version, (LONG)loaded->lib_Revision, loaded,
            loaded->lib_IdString ? (char *)loaded->lib_IdString : "?",
            resident ? "hidden" : "none");
    Flush(out);
    rc = SystemTags(argv[3], SYS_Output, out, SYS_Error, out, TAG_END);
    FPrintf(out, "command rc %ld\n", rc);
    Close(out);
    unhide(resident);
    if (base) CloseLibrary(base);
    Printf("loadposixc: rc %ld\n", rc);
    return 0;
}
