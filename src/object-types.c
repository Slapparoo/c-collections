#include "object-types.h"

PChar PString_code_formatString(const PChar format, ...) {
    va_list ap;
    va_start(ap, format);
    u64 needed = vsnprintf(NULL, 0, format, ap) + 1;
    va_end(ap);

    PChar res = Pointers.malloc(needed);
    va_start(ap, format);
    vsprintf(res, format, ap);
    va_end(ap);

    return res;
}

int ErrorConsole_print(const PChar format, ...) {
    va_list ap;
    va_start(ap, format);
    int res = vfprintf(stderr, format, ap);
    va_end(ap);
    return res;
}

int ErrorConsole_error(const PChar message) {
    return Files.format(stderr, ANSI_BOLD_RED "[ERROR] " ANSI_DEFAULT "%s\n", message);
}

struct Pointer_code Pointers = {
    &memchr,
    &memcmp,
    &memcpy,
    &memmove,
    &memset,
    &calloc,
    &free,
    &malloc,
    &realloc
};

struct PString_code PStrings = {
    &strcat,
    &strncat,
    &strchr,
    &strcmp,
    &strncmp,
    &strcoll,
    &strcpy,
    &strncpy,
    &strcspn,
    &strerror,
    &strlen,
    &strpbrk,
    &strchr,
    &strspn,
    &strstr,
    &strtok,
    &strxfrm,
    &sprintf,
    &vsprintf,
    &sscanf,
    &snprintf,
    &vsnprintf,
    // null,
    &PString_code_formatString,
    &atof,
    &atoi,
    &atol,
    &strtod,
    &strtol,
    &strtoul,
    &mblen,
    &mbtowc,
    &wcstombs,
    &wctomb,
};

struct File_code Files = {
    &fclose,
    &clearerr,
    &feof,
    &ferror,
    &fflush,
    &fgetpos,
    &fopen,
    &fread,
    &freopen,
    &fseek,
    &fsetpos,
    &ftell,
    &fwrite,
    &remove,
    &rename,
    &rewind,
    &setbuf,
    &setvbuf,
    &tmpfile,
    // &tmpnam,
    &fprintf,
    &vfprintf,
    &fscanf,
    &fgetc,
    &fgets,
    &fputc,
    &fputs,
    &ungetc
};

struct Console_code Console = {
    &printf,
    &vprintf,
    &scanf,
    &getchar,
    // &gets,
    &putchar,
    &puts
};

struct ErrorConsole_code ErrorConsole = {
    &perror,
     &ErrorConsole_print,
     &ErrorConsole_error
};

struct Hashes_code Hashes = {
    &hashPointer,
    &hash64,
    &hashPChar,
    &hashBoolean
};


struct System_code System = {
    &abort,
    &atexit,
    &exit,
    &getenv,
    &system,
    &bsearch,
    &qsort,
};

#ifdef TEST_OBJECT_TYPES

#include <assert.h>

int main(int argc, char const *argv[])
{
    assert(&Pointers);
    assert(Pointers.calloc);
    assert(Pointers.compare);
    return 0;
}

#endif
