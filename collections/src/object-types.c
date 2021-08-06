#include "collection_base.h"


/**
 * @brief Swap the value from a to b
 * 
 * @param dest 
 * @param src 
 * @param size int bytes
 */
void Pointers_swap(Pointer a, Pointer b, i32 size) {
    if (size < 0 || !a || !b) {
        return;
    }

    switch (size) {
        case 1: {
            u8 tmp = *(u8*)a; 
            *(u8*)a = *(u8*)b; 
            *(u8*)b = tmp; 
            break;
        }
        case 2: {
            u16 tmp = *(u16*)a; 
            *(u16*)a = *(u16*)b; 
            *(u16*)b = tmp; 
            break;
        }
        case 4: {
            u32 tmp = *(u32*)a; 
            *(u32*)a = *(u32*)b; 
            *(u32*)b = tmp; 
            break;
        }
        case 8: {
            u64 tmp = *(u64*)a; 
            *(u64*)a = *(u64*)b; 
            *(u64*)b = tmp; 
            break;
        }
        default: {
            Pointer tmp = Pointers.malloc(size);
            Pointers.copy(tmp, a, size);
            Pointers.copy(a, b, size);
            Pointers.copy(b, tmp, size);
            Pointers.free(tmp);
        }
    }
}

//Fn_allocate -- typedef void (*Fn_allocate)(const u64 itemSize, const u64 numberOfItems, Fn_release);
Pointer Pointers_malloc_allocate(const u64 itemSize, const u64 numberOfItems, PFn_release release) {
    // where to store the release
    
    return calloc(itemSize, numberOfItems);
}


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

// #include <assert.h>

PChar PString_code_replace(const PChar string, const PChar replace, const PChar with) {
    if (!string || !replace || !with) {
        // printDebug("%s", "1");
        return null;
    } 

    int lenString = PStrings.length(string);
    int lenReplace = PStrings.length(replace);

    if (!lenString || !lenReplace ) {
        // printDebug("%s", "2");
        return null;
    } 

	int lenWith = PStrings.length(with);
    PChar strPointer = string;

    int count = 0;
    PChar tmp = 0;
	for (count = 0; ((tmp = PStrings.findFirst(strPointer, replace)) != null); ++count) {
		strPointer = tmp + lenReplace;
	}

    if (!count) {
        // no matches found
        return PStrings.formatString("%s", string);
    }

	u64 newSize = PStrings.length(string) + (lenWith - lenReplace) * count + 1;
    PChar front = string, res = Pointers.malloc(newSize);
    strPointer = res;

	while (count--) {
        // assert(front); 
        // assert(front >= string);
        // assert(front <= string + lenString);

        // assert(strPointer); 
        // assert(strPointer >= res); 
        // assert(strPointer <= res + newSize);
        PChar sourceP = front;

		front = PStrings.findFirst(front, replace);
		PStrings.copyTo(strPointer, sourceP, front - sourceP);
        strPointer += (front - sourceP);
        PStrings.copy(strPointer, with);

        strPointer += lenWith;
        front += lenReplace;
	}
    if (PStrings.length(front)) {
        PStrings.copy(strPointer, front);
    }

	return res;
}

// #pragma clang diagnostic ignore "-Wincompatible-pointer-types"
// #pragma clang diagnostic ignore "-Wincompatible-function-pointer-types"

struct Pointer_code Pointers = {
    &memchr,
    &memcmp,
    &memcpy,
    &memmove,
    &memset,
    &calloc,
    &free,
    &malloc,
    &realloc,
    &Pointers_swap
};

/**
 * @brief return a clone of the string
 * the caller is responsible for freeing the memory
 * @param string 
 * @return PChar 
 */
PChar PChar_clone(PChar string) {
    PChar res = Pointers.malloc(PStrings.length(string) +1);
    PStrings.copy(res, string);
    return res;
}

/**
 * @brief return a clone of the string
 * the caller is responsible for freeing the memory
 * @param string 
 * @return PChar 
 */
PChar PChar_subString(PChar string, u32 len) {
    PChar res = Pointers.malloc(len +1);
    // for (int i = 0; i < len; i++) {
    //     res[i] = string[i];
    // }
    PStrings.copyTo(res, string, len);
    res[len] = 0;
    return res;
}

/**
 * @brief Convert to upperCase - this will update the string contents to upper case
 * if this is undesirable clone it first;
 * 
 * @param string 
 * @return PChar 
 */
PChar PChar_toUpper(PChar string) {
    for (int i = 0; i < PStrings.length(string); i++) {
        string[i] = toupper(string[i]);
    }
    return string;
}
/**
 * @brief Convert to lowerCase - this will update the string contents to lower case
 * if this is undesirable clone it first;
 * 
 * @param string 
 * @return PChar 
 */
PChar PChar_toLower(PChar string) {
    for (int i = 0; i < PStrings.length(string); i++) {
        string[i] = tolower(string[i]);
    }
    return string;
}

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
    &PString_code_replace,
    &PChar_clone,
    &PChar_toUpper,
    &PChar_toLower,
    &PChar_subString
};

u64 File_size(File f) {
    Files.seek(f, 0L, SEEK_END);
    u64 size = ftell(f);
    Files.seek(f, 0L, SEEK_SET);
    return size;
}

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
    &ungetc,
    &File_size
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
    &hash$Pointer,
    &hash$64,
    &hash$PChar,
    &hash$Boolean
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

// #pragma clang diagnostic warning "-Wincompatible-pointer-types"

#ifdef TEST_OBJECT_TYPES

#include <assert.h>

int main(int argc, char const *argv[])
{
    assert(&Pointers);
    assert(Pointers.calloc);
    assert(Pointers.compare == &memcmp);
    assert(Pointers.copy = &memcpy);

    PChar result = PStrings.replace("some long string", "o", "e");
    Console.println(result);
    assert(!PStrings.compare(result, "seme leng string"));
    Pointers.free(result);

    result = PStrings.replace("The kids have a pet goat.", "goat", "cat");
    Console.println(result);
    assert(!PStrings.compare(result, "The kids have a pet cat."));
    
    Pointers.free(result);



    return 0;
}

#endif
