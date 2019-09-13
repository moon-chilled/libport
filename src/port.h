#ifndef PORT_H
#define PORT_H

#ifdef _WIN32
# ifdef LIBPORT_IMPLEMENTATION
#  define LIBPORT_FUNC extern __declspec(dllexport)
# else
#  define LIBPORT_FUNC extern __declspec(dllimport)
# endif
#else
# define LIBPORT_FUNC extern
#endif

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <sys/types.h> //for ssize_t.  Need to get it another way on windows.

typedef struct P6State P6State;

typedef enum {
	P6Any,
	P6Nil,
	P6Int,
	P6Num,
	P6Str,
	P6Bool,
	P6Error,
	P6Sub,
	P6List,
} P6Type;

// we store a P6Type in a uint32_t, so have to make sure it's the same size so
// nothing breaks.  Not much is lost by storing it that way instead of as
// uint8_t because of padding/alignment.
static_assert (sizeof(P6Type) == sizeof(uint32_t), "enums are not sized to 32 bits");

typedef struct P6Val P6Val;
struct P6Val {
	union {
		void *any;
		int64_t integer;
		double num;
		char *str;
		bool boolean;
		char *error_msg;
		struct {
			void *addr;

			ssize_t arity; // negative => variadic
			P6Type *arguments;
		} sub;

		struct {
			P6Val *vals;
			size_t len;
		} list;
	};

	P6Type type;
};


LIBPORT_FUNC P6State *p6_init(void);
LIBPORT_FUNC void p6_deinit(P6State *state);
LIBPORT_FUNC P6Val *p6eval(P6State *state, char *text);
LIBPORT_FUNC P6Val *p6_call(P6State *state, P6Val *fun, P6Val *argument_list);

LIBPORT_FUNC P6Val *p6_make_nil(void);
LIBPORT_FUNC P6Val *p6_make_any(void *any);
LIBPORT_FUNC P6Val *p6_make_int(int64_t integer);
LIBPORT_FUNC P6Val *p6_make_num(double num);
LIBPORT_FUNC P6Val *p6_make_str(char *str);
LIBPORT_FUNC P6Val *p6_make_bool(bool boolean);
LIBPORT_FUNC P6Val *p6_make_error(char *msg);
LIBPORT_FUNC P6Val *p6_make_sub(void *address, const P6Type *arguments, ssize_t arity);
LIBPORT_FUNC P6Val *p6_make_list(const P6Val *vals, size_t num_vals);

LIBPORT_FUNC P6Type p6_typeof(const P6Val *val);
LIBPORT_FUNC void p6_val_free(P6Val *val);

LIBPORT_FUNC P6Val *p6_val_copy(const P6Val *val);
LIBPORT_FUNC void p6_list_append(P6Val *list, P6Val *item);

#endif//PORT_H
