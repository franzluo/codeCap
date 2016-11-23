/*****************************************
 Copyright (c) 2001-2003
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/

/**
	@file dbgimplementation.c
	@brief User mode test application, debugging print implementation.
*/

/*
  **********************************************
  DISCLAIMER:

  - THIS IS TEST CODE, provided as sample code
  to help you understand what you should do to
  develop your own application based in RMFP.

  - This is NOT production grade code; It is not
  even a library, so any API defined here, CAN
  and WILL CHANGE without notice.

  **********************************************
*/

#ifndef ALLOW_OS_CODE
#define ALLOW_OS_CODE
#endif

#include <stdio.h>
#include <stdarg.h>

#define ENABLE_TIMESTAMPS (1)

#if ((defined WITH_INSTRUMENT_FUNC) && (WITH_INSTRUMENT_FUNC == 1))
// for best results, use pretty print when using instruments to indent the logs
#define PRETTY_PRINT (1)
#else
#define PRETTY_PRINT (0)
#endif

#define PRETTY_PRINT_MAX_FILENAME_LEN (20)

#ifdef _DEBUG
int verbose_stdout = 1;
int verbose_stderr = 1;
#else
int verbose_stdout = 0;
int verbose_stderr = 0;
#endif

#define NORMALMSG stdout
#define ERRORMSG  stderr

int instrument_call_nesting = 0;

#define RMDBG_MAX_STRING 1024

static char str[RMDBG_MAX_STRING];


#if ((RMCOMPILERID==RMCOMPILERID_GCC) || (RMCOMPILERID==RMCOMPILERID_ARMELF_GCC) || (RMCOMPILERID==RMCOMPILERID_MIPSEL_GCC))

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static inline void print_timestamp(FILE *stream)
{
	return;
}

static inline int pretty_print(bool PrettyPrint, char *pOutputString, unsigned int MaxSize, const char *pFileName, int FileLine)
{
	int len;
	int i;

	if (PrettyPrint) {

		char buffer[PRETTY_PRINT_MAX_FILENAME_LEN];

		len = strlen(pFileName);

		if (len > PRETTY_PRINT_MAX_FILENAME_LEN) {
			for (i = 0; i < PRETTY_PRINT_MAX_FILENAME_LEN; i++)
				buffer[i] = pFileName[(len - PRETTY_PRINT_MAX_FILENAME_LEN) + i];

			buffer[i] = 0;

			buffer[0] = '.';
			buffer[1] = '.';
			buffer[2] = '.';
		}
		else {
			for (i = 0; i < len; i++)
				buffer[i] = pFileName[i];

			buffer[i] = 0;
		}

		len = 0;

		snprintf((char *)(pOutputString + len), MaxSize, "[%20s:%4ld] ", (char *)buffer, FileLine);

	}
	else {

		len = 0;

		snprintf((char *)(pOutputString + len), MaxSize, "[%s:%ld] ", (char *)pFileName, FileLine);
	}

	len = strlen(pOutputString);

#if ((defined WITH_INSTRUMENT_FUNC) && (WITH_INSTRUMENT_FUNC == 1))
	snprintf((char *)(pOutputString + len), MaxSize, " ");

	len = strlen(pOutputString);

	for (i = 0; ((i < instrument_call_nesting) && (len + i < MaxSize)); i++)
		pOutputString[len+i] = '-';

	pOutputString[len+i] = 0;

	len = strlen(pOutputString);

	snprintf((char *)(pOutputString + len), MaxSize, " ");
#endif

	len = strlen(pOutputString);

	return len;
}





#ifdef RMNOTIFY_WITHOUT_COLORS

const char SET_ERROR[] = "";
const char SET_INFO[]  = "";
const char SET_STD[]   = "";

#else //RMNOTIFY_WITHOUT_COLORS

const char SET_ERROR[] = "\033[31;1m";  // bold / red
const char SET_INFO[]  = "\033[32;1m";  // bold / green
const char SET_STD[]   = "\033[0m";     // clear attributes

#endif


#elif (RMCOMPILERID==RMCOMPILERID_VISUALC)

#include <windows.h>
#include <stdio.h>

#else

NOTCOMPILABLE

#endif

#if ((defined WITH_INSTRUMENT_FUNC) && (WITH_INSTRUMENT_FUNC == 1))

/*
  THE NESTING INDENTATION IS VALID ONLY IN A MONOTHREADED ENVIRONMENT
*/


/*
  adapted from set_rua_test/test/instrument.c
*/


#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FUNCTIONS		2000
#define MAX_FUNCTION_NAME	50

typedef struct {
	unsigned int address;
	char funcName[MAX_FUNCTION_NAME+1];
} func_t;

static void initSymbol( char *image ) __attribute__ ((no_instrument_function));
static int lookupSymbol( unsigned int address )__attribute__ ((no_instrument_function));
static void addSymbol( unsigned int address ) __attribute__ ((no_instrument_function));
static int translateFunctionFromSymbol( unsigned int address, char *func ) __attribute__ ((no_instrument_function));

void test_rmfp_enable_instrumentation(int enable) __attribute__ ((no_instrument_function));

func_t       functions[MAX_FUNCTIONS];
unsigned int totals[MAX_FUNCTIONS];

int instrument_enable = 1;
char imageName[MAX_FUNCTION_NAME];

/* Function prototypes with attributes */
void main_constructor( void )
	__attribute__ ((no_instrument_function, constructor));

void main_destructor( void )
	__attribute__ ((no_instrument_function, destructor));

void __cyg_profile_func_enter( void *, void * )
	__attribute__ ((no_instrument_function));

void __cyg_profile_func_exit( void *, void * )
	__attribute__ ((no_instrument_function));

void main_constructor( void )
{
	char *p=getenv("ELFIK");


	fprintf(stdout, "%s()\n", __FUNCTION__);

	if (p)
		initSymbol(p);
	else {

		instrument_enable = 0;

		fprintf(stdout,"ELFIK env variable not found, disable backtrace printing\n");
	}
}

void main_destructor( void )
{

	fprintf(stdout, "%s()\n", __FUNCTION__);

}

void __cyg_profile_func_enter( void *this, void *callsite )
{

	instrument_call_nesting++;

	if (instrument_enable) {
		int i;

		addSymbol((int)this);
		addSymbol((int)callsite);

		for (i=0;i<instrument_call_nesting;i++) printf(" ");

		printf("\014\033[1;32m%s calling %s\033[0m\n",
		       functions[lookupSymbol((int)callsite)].funcName,
		       functions[lookupSymbol((int)this)].funcName);
	}
}

void __cyg_profile_func_exit( void *this, void *callsite )
{

	instrument_call_nesting--;

	if (instrument_enable) {
		int i;

		addSymbol((int)this);
		addSymbol((int)callsite);

		for (i=0;i<instrument_call_nesting;i++) printf(" ");

		printf("\014\033[1;32m%s returning to %s\033[0m\n",
		       functions[lookupSymbol((int)this)].funcName,
		       functions[lookupSymbol((int)callsite)].funcName);
	}
}

void test_rmfp_enable_instrumentation(int enable)
{
	printf("\014\033[1;32minstrument %s!\033[0m\n",enable?"on":"off");
	instrument_enable = enable;
}

/*
  Internal functions
*/



static void initSymbol( char *image )
{
	int from;

	strcpy( imageName, image );

	for ( from = 0 ; from < MAX_FUNCTIONS ; from++ ) {
		functions[from].address = 0;
		functions[from].funcName[0] = 0;
		totals[from] = 0;
	}

	return;
}


static int lookupSymbol( unsigned int address )
{
	int index;

	for (index = 0 ; index < MAX_FUNCTIONS ; index++) {
		if (functions[index].address == 0) break;
		if (functions[index].address == address) return index;
	}

	assert(0);

	return 0;
}

static int translateFunctionFromSymbol( unsigned int address, char *func )
{
	FILE *p;
	char line[100];
	int  len, i, times=0;

	sprintf( line, "addr2line -e %s -f -s 0x%x", imageName, address );

	p = popen( line, "r" );

	if (p == NULL) {
		sprintf(func,"%p",(void *)address);
		goto done;
	}
	else {

		len = fread( line, 99, 1, p );
		if (line[0]=='?' ) {
			pclose(p);
			sprintf(func,"%p",(void *)address);
			goto done;
		}

		i = 0;
		while ( (unsigned)i < strlen(line) ) {

			if ((line[i] == 0x0d) || (line[i] == 0x0a)) {
				if (times) {
					func[i] = ']';
					func[i+1] = 0;
					break;
				}
				else
					func[i]='[';

				times++;
			} else {
				func[i] = line[i];
			}

			i++;

		}

		pclose(p);

	}

 done:
	//printf("remembering %p as %s\n",(void *)address,func);
	return 1;
}

static void addSymbol( unsigned int address )
{
	int index;

	for (index = 0 ; index < MAX_FUNCTIONS ; index++) {
		if (functions[index].address == address) return;
		if (functions[index].address == 0) break;
	}

	if (index < MAX_FUNCTIONS) {
		functions[index].address = address;
		translateFunctionFromSymbol( address, functions[index].funcName );
	} else {
		assert( 0 );
	}

	return;
}


#endif




#if 0
int console_output(const char *format, ...);
int file_output(FILE *fptr, const char *format, ...);
int DebugLevel = 1;

int console_output(const char *format, ...)
{
	va_list ap;
	int res;

	if (verbose_stdout != 0) {
		va_start(ap, format);
		res = vprintf(format, ap);
		va_end(ap);
		return(res);
	}
	return(0);
}

int file_output(FILE *fptr, const char *format, ...)
{
	va_list ap;
	int res;

	if (((fptr != stderr) && (verbose_stdout != 0)) ||
		((fptr == stderr) && (verbose_stderr != 0))) {
		va_start(ap, format);
		res = vfprintf(fptr, format, ap);
		va_end(ap);
		return(res);
	}
	return(0);
}
#endif


