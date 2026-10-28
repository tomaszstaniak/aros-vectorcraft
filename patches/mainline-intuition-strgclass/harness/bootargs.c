/* Print the kernel arguments of this boot (bootloader.resource BL_Args),
   to record which GRUB entry was booted. */
#include <proto/exec.h>
#include <proto/bootloader.h>
#include <aros/bootloader.h>
#include <exec/lists.h>
#include <stdio.h>

APTR BootLoaderBase;

int main(void)
{
    struct List *args;
    struct Node *n;
    STRPTR name;

    BootLoaderBase = OpenResource("bootloader.resource");
    if (!BootLoaderBase)
    {
        printf("no bootloader.resource\n");
        return 20;
    }
    name = (STRPTR)GetBootInfo(BL_LoaderName);
    printf("loader: %s\n", name ? (char *)name : "(none)");
    args = (struct List *)GetBootInfo(BL_Args);
    if (args)
        for (n = args->lh_Head; n->ln_Succ; n = n->ln_Succ)
            printf("arg: %s\n", n->ln_Name);
    return 0;
}
