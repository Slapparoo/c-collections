#ifndef _OBJECT_TYPES_HH
#define _OBJECT_TYPES_HH


#include "string.h"
#include <inttypes.h>
#include "stdio.h"
#include <stdarg.h>
#include <stdlib.h>
#include "data-types.h"
#include "ctype.h"

#define startsWith(string, sequence) strncmp(string, sequence, strlen(sequence)) == 0

PChar PString_code_formatString(const PChar format, ...);
int ErrorConsole_print(const PChar format, ...);
int ErrorConsole_error(const PChar message);

typedef struct System_code {
    // Causes an abnormal program termination.
    void (*abort)(void); // void abort(void)
    // Causes the specified function func to be called when the program terminates normally.
    int (*atexit)(void (*func)(void)); //int atexit(void (*func)(void))
    // Causes the program to terminate normally.
    void (*exit)(int status); // void exit(int status)
    // Searches for the environment string pointed to by name and returns the associated value to the string.
    PChar (*getenv)(const PChar name); // PChar getenv(const PChar name)
    // The command specified by string is passed to the host environment to be executed by the command processor.
    int (*system)(const PChar string); // int system(const PChar string)
    // Performs a binary search.
    Pointer (*bsearch)(const Pointer key, const Pointer base, u64 nitems, u64 size, int (*compar)(const Pointer , const Pointer ));
    // Pointer bsearch(const Pointer key, const Pointer base, u64 nitems, u64 size, int (*compar)(const Pointer , const Pointer ))
    // Sorts an array.
    void (*qsort)(Pointer base, u64 nitems, u64 size, int (*compar)(const Pointer , const Pointer));
    // void qsort(Pointer base, u64 nitems, u64 size, int (*compar)(const Pointer , const Pointer))
} System_code;

typedef struct Math_code {
    // Returns the absolute value of x.
    int (*abs)(int x); // int abs(int x)
    // Divides numer (numerator) by denom (denominator).
    div_t (*div)(int numer, int denom); // div_t div(int numer, int denom)
    // Returns the absolute value of x.
    i64 (*labs)(i64 x); // i64 labs(i64 x)
    // Divides numer (numerator) by denom (denominator).
    ldiv_t (*ldiv)(i64 numer, i64 denom); // ldiv_t ldiv(i64 numer, i64 denom)
    // Returns a pseudo-random number in the range of 0 to RAND_MAX.
    int (*rand)(void); // int rand(void)
    // This function seeds the random number generator used by the function rand.
    void (*srand)(u32 seed); // void srand(u32 seed)
} Math_code;

struct Pointer_code {
    // Searches for the first occurrence of the character c (an unsigned char) in the first n bytes of the string pointed to, by the argument str.
    Pointer (*find)(const Pointer string, int c, u64 n);
    // Compares the first n bytes of str1 and str2.
    int (*compare)(const Pointer str1, const Pointer str2, u64 n);
    // Copies n characters from src to dest.
    Pointer (*copy)(Pointer dest, const Pointer src, u64 n);
    // Another function to copy n characters from str2 to str1.
    // This function returns a pointer to the destination store str1 pointer.
    Pointer (*move)(Pointer dest, const Pointer src, u64 n);
    // Copies the character c (an unsigned char) to the first n characters of the string pointed to, by the argument str.
    Pointer (*setAll)(Pointer string, int c, u64 n);
    // Allocates the requested memory and returns a pointer to it.
    Pointer (*calloc)(u64 nitems, u64 size); // Pointer calloc(u64 nitems, u64 size)
    // Deallocates the memory previously allocated by a call to calloc, malloc, or realloc.
    void (*free)(Pointer ptr); // void free(Pointer ptr)
    // Allocates the requested memory and returns a pointer to it.
    Pointer (*malloc)(u64 size); // Pointer malloc(u64 size)
    // Attempts to resize the memory block pointed to by ptr that was previously allocated with a call to malloc or calloc.
    Pointer (*realloc)(Pointer ptr, u64 size); // Pointer realloc(Pointer ptr, u64 size)
    // Copy the contents of one pointer to the other
    void (*swap)(Pointer a, Pointer b, i32 size); 

} Pointer_code;

typedef struct PString_code {
    // Appends the string pointed to, by src to the end of the string pointed to by dest.
    PChar (*append)(PChar dest, const PChar src);
    // Appends the string pointed to, by src to the end of the string pointed to, by dest up to n characters long.
    PChar (*appendTo)(PChar dest, const PChar src, u64 n);
    // Searches for the first occurrence of the character c (an unsigned char) in the string pointed to, by the argument str.
	PChar (*find)(const PChar string, int c);
    // Compares the string pointed to, by str1 to the string pointed to by str2.
    int (*compare)(const PChar str1, const PChar str2);
    // Compares at most the first n bytes of str1 and str2.
    int (*compareTo)(const PChar str1, const PChar str2, u64 n);
    // Compares string str1 to str2. The result is dependent on the LC_COLLATE setting of the location.
    int (*collateCompare)(const PChar str1, const PChar str2);
    // Copies the string pointed to, by src to dest.
	PChar (*copy)(PChar dest, const PChar src);
    // Copies up to n characters from the string pointed to, by src to dest.
    PChar (*copyTo)(PChar dest, const PChar src, u64 n);
    // Calculates the length of the initial segment of str1 which consists entirely of characters not in str2.
    u64 (*noMatchLength)(const PChar str1, const PChar str2);
    // Searches an internal array for the error number errnum and returns a pointer to an error message string.
    PChar (*getError)(int errnum);
    // Computes the length of the string str up to but not including the terminating null character.
	u64 (*length)(const PChar str);
    // Finds the first character in the string str1 that matches any character specified in str2.
    PChar (*matchAny)(const PChar str1, const PChar str2);
    // Searches for the last occurrence of the character c (an unsigned char) in the string pointed to by the argument str.
    PChar (*findLast)(const PChar str, int c);
    // Calculates the length of the initial segment of str1 which consists entirely of characters in str2.
    u64 (*matchLength)(const PChar str1, const PChar str2);
    // Finds the first occurrence of the entire string needle (not including the terminating null character) which appears in the string haystack.
    PChar (*findFirst)(const PChar haystack, const PChar needle);
    // Breaks string str into a series of tokens separated by delim.
	PChar (*split)(PChar str, const PChar delim);
    // Transforms the first n characters of the string src into current locale and places them in the string dest.    
    u64 (*transform)(PChar dest, const PChar src, u64 n);
    // Sends formatted output to a string.
    int (*format)(PChar str, const PChar format, ...); // int sprintf(PChar str, const PChar format, ...)
    // Sends formatted output to a string using an argument list.
    int (*vformat)(PChar str, const PChar format, va_list arg); // int vsprintf(PChar str, const PChar format, va_list arg)
    // Reads formatted input from a string.
    int (*read)(const PChar str, const PChar format, ...); // int sscanf(const PChar str, const PChar format, ...)
    //  return the maximum number of characters (including at the end of null character) to be written to buffer
    u64 (*formatLength)(PChar str, u64 size, const PChar format, ...); // int snprintf(PChar str, u64 size, const PChar format, ...);

    u64 (*vformatLength)(PChar str, u64 size, const PChar format, va_list ap); //int vsnprintf(PChar str, u64 size, const PChar format, va_list ap);
    // malloc a buffer of the correct length and return the fomatted strimg, the String will need to be freed by the caller
    PChar (*formatString)(const PChar format, ...);
    // Converts the string pointed to, by the argument str to a floating-point number (type double).
    f64 (*toF64)(const PChar str); //double atof(const PChar str)
    // Converts the string pointed to, by the argument str to an integer (type int).
    int (*toI32)(const PChar str); // int atoi(const PChar str)
    // Converts the string pointed to, by the argument str to a i64eger (type i64).
    i64 (*toI64)(const PChar str); // i64 atol(const PChar str)
    // Converts the string pointed to, by the argument str to a floating-point number (type double).
    f64 (*strtod)(const PChar str, PChar *endptr); // f64 strtod(const PChar str, PChar *endptr)
    // Converts the string pointed to, by the argument str to a i64eger (type i64).
    i64 (*strtol)(const PChar str, PChar *endptr, int base); // i64 strtol(const PChar str, PChar *endptr, int base)
    // Converts the string pointed to, by the argument str to an unsigned i64eger (type unsigned i64).
    u64 (*strtoul)(const PChar str, PChar *endptr, int base); //u64 (*strtoul)(const PChar str, PChar *endptr, int base)
    // Returns the length of a multibyte character pointed to by the argument str.
    int (*mblen)(const PChar str, u64 n); //int mblen(const PChar str, u64 n)
    // Converts the string of multibyte characters pointed to by the argument str to the array pointed to by pwcs.
    //@TODO find out what schar_t is
    // u64 (*mbstowcs)(i16 *pwcs, const PChar str, u64 n); // u64 mbstowcs(schar_t *pwcs, const PChar str, u64 n)
    // Examines the multibyte character pointed to by the argument str.
    int (*mbtowc)(i16 *pwc, const PChar str, u64 n); // int mbtowc(whcar_t *pwc, const PChar str, u64 n)
    // Converts the codes stored in the array pwcs to multibyte characters and stores them in the string str.
    u64 (*wcstombs)(PChar str, const wchar_t *pwcs, u64 n); // u64 wcstombs(PChar str, const wchar_t *pwcs, u64 n)
    // Examines the code which corresponds to a multibyte character given by the argument wchar.
    int (*wctomb)(PChar str, wchar_t wchar); // int wctomb(PChar str, wchar_t wchar)
    // return a new string where "replace" has been replaced with "with"
    PChar (*replace)(const PChar string, const PChar replace, const PChar with);
    PChar (*clone)(const PChar string);
    // convert to upper case
    PChar (*toUpper)(PChar string);
    // conver to lower case
    PChar (*toLower)(PChar string);
    // create a subString (this allocs memory)
    PChar (*subString)(PChar string, u32 len);

} PString_code;

typedef struct File_code {
    // Closes the stream. All buffers are flushed.
    int (*close)(File stream); // int fclose(FILE *stream)
    // Clears the end-of-file and error indicators for the given stream.
    void (*clearerr)(File stream); // void clearerr(File stream)
    // Tests the end-of-file indicator for the given stream.
	int (*eof)(File stream); //int feof(FILE *stream)
    // Tests the error indicator for the given stream.
	int (*error)(File stream); // int ferror(FILE *stream)
    // Flushes the output buffer of a stream.
    int (*flush)(File stream); // int fflush(FILE *stream)
    // Gets the current file position of the stream and writes it to pos.
    int (*getPos)(File stream, FilePos pos); // int fgetpos(FILE *stream, fpos_t *pos)
    // Opens the filename pointed to by filename using the given mode.
    File (*open)(const PChar filename, const PChar mode); // FILE *fopen(const PChar filename, const PChar mode)
    // Reads data from the given stream into the array pointed to by ptr.
    u64 (*readPointer)(Pointer ptr, u64 elementSize, u64 numberOfElements, File stream); // u64 fread(Pointer ptr, u64 size, u64 nmemb, FILE *stream)
    // Associates a new filename with the given open stream and same time closing the old file in stream.
    File (*reopen)(const PChar filename, const PChar mode, File stream); // FILE *freopen(const PChar filename, const PChar mode, FILE *stream)
    // Sets the file position of the stream to the given offset. The argument offset signifies the number of bytes to seek from the given whence position.
    int (*seek)(File stream, i64 offset, int whence); // int fseek(FILE *stream, i64 offset, int whence)
    // Sets the file position of the given stream to the given position. The argument pos is a position given by the function fgetpos.
    int (*setPos)(File stream, const FilePos pos); // int fsetpos(FILE *stream, const fpos_t *pos)
    // Returns the current file position of the given stream.
    i64 (*tellPos)(File stream); // i64 ftell(FILE *stream)
    // Writes data from the array pointed to by ptr to the given stream.
    u64 (*printPointer)(const Pointer ptr, u64 size, u64 nmemb, File stream); // u64 fwrite(const Pointer ptr, u64 size, u64 nmemb, FILE *stream)
    // Deletes the given filename so that it is no longer accessible.
    int (*remove)(const PChar filename); // int remove(const PChar filename)
    // Causes the filename referred to, by old_filename to be changed to new_filename.
    int (*rename)(const PChar old_filename, const PChar new_filename); // int rename(const PChar old_filename, const PChar new_filename)
    // Sets the file position to the beginning of the file of the given stream.
    void (*rewind)(File stream); // void rewind(FILE *stream)
    // Defines how a stream should be buffered.
    void (*setbuf)(File stream, PChar buffer); // void setbuf(FILE *stream, PChar buffer)
    // Another function to define how a stream should be buffered.
    int (*setvbuf)(File stream, PChar buffer, int mode, u64 size); // int setvbuf(FILE *stream, PChar buffer, int mode, u64 size)
    // Creates a temporary file in binary update mode (wb+).
    File (*tmpFile)(void); // FILE *tmpfile(void)
    // Generates and returns a valid temporary filename which does not exist.
    // PChar (*tmpName)(PChar str); // PChar tmpnam(PChar str)
    // Sends formatted output to a stream.
    int (*format)(File stream, const PChar format, ...); // int fprintf(FILE *stream, const PChar format, ...)
    // Sends formatted output to a stream using an argument list.
    int (*vformat)(File stream, const PChar format, va_list arg); // int vfprintf(FILE *stream, const PChar format, va_list arg)
    // Reads formatted input from a stream.
    int (*read)(File stream, const PChar format, ...); // int fscanf(FILE *stream, const PChar format, ...)
    // Gets the next character (an unsigned char) from the specified stream and advances the position indicator for the stream.
    int (*readChar)(File stream); // int fgetc(FILE *stream)
    // Reads a line from the specified stream and stores it into the string pointed to by str. It stops when either (n-1) characters are read, the newline character is read, or the end-of-file is reached, whichever comes first.
    PChar (*readString)(PChar str, int n, File stream); // PChar fgets(PChar str, int n, FILE *stream)
    // Writes a character (an unsigned char) specified by the argument char to the specified stream and advances the position indicator for the stream.
    int (*printChar)(int, File stream); // int fputc(int char, FILE *stream)
    // Writes a string to the specified stream up to but not including the null character.
    int (*printString)(const PChar str, File stream); // int fputs(const PChar str, FILE *stream)
    // Gets the next character (an unsigned char) from the specified stream and advances the position indicator for the stream.
    // int (*readChar)(File stream); // int getc(FILE *stream)
    // Writes a character (an unsigned char) specified by the argument char to the specified stream and advances the position indicator for the stream.
    // int (*putChar)(int char, File stream); // int putc(int char, FILE *stream)
    // Pushes the character char (an unsigned char) onto the specified stream so that the next character is read.
    int (*unreadChar)(int , File stream); // int ungetc(int char, FILE *stream)

    u64 (*size)(File stream);
} File_code;

typedef struct Console_code {
    // Sends formatted output to stdout.
    int (*format)(const PChar format, ...); // int printf(const PChar format, ...)
    // Sends formatted output to stdout using an argument list.
    int (*vformat)(const PChar format, va_list arg); // int vprintf(const PChar format, va_list arg)
    // Reads formatted input from stdin.
    int (*read)(const PChar format, ...); // int scanf(const PChar format, ...)
    // Gets a character (an unsigned char) from stdin.
    int (*readChar)(void); // int getchar(void)
    // Reads a line from stdin and stores it into the string pointed to by, str. It stops when either the newline character is read or when the end-of-file is reached, whichever comes first.
    // String (*readString)(String str); // PChar gets(PChar str)
    // Writes a character (an unsigned char) specified by the argument char to stdout.
    int (*printChar)(int); // int putchar(int char)
    // Writes a string to stdout up to but not including the null character. A newline character is appended to the output.
    int (*println)(const PChar str); // int puts(const PChar str)
} Console_code;

typedef struct ErrorConsole_code {
    // Prints a descriptive error message to stderr. First the string str is printed followed by a colon and then a space.
	void (*println)(const PChar str); // void perror(const PChar str)
    int (*format)(PChar format, ...);
    int (*error)(PChar message);
} ErrorConsole_code;

typedef struct Hashes_code {
    i64 (*hashPointer)(Pointer val);
    i64 (*hash64)(i64 val);
    i64 (*hashPchar)(PChar str);
    i64 (*hashBoolean)(boolean b);
} Hashes_code;


struct PString_code PStrings;
struct Pointer_code Pointers;
struct System_code System;
struct Console_code Console;
struct ErrorConsole_code ErrorConsole;
struct File_code Files;
struct Hashes_code Hashes;

#endif      