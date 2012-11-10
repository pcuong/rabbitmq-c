/* vim:set ft=c ts=2 sw=2 sts=2 et cindent: */
/** @file */
/*
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MIT
 *
 * Portions created by Alan Antonuk are Copyright (c) 2012-2013
 * Alan Antonuk. All Rights Reserved.
 *
 * Portions created by VMware are Copyright (c) 2007-2012 VMware, Inc.
 * All Rights Reserved.
 *
 * Portions created by Tony Garnock-Jones are Copyright (c) 2009-2010
 * VMware, Inc. and Tony Garnock-Jones. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * ***** END LICENSE BLOCK *****
 */

#ifndef AMQP_H
#define AMQP_H

#ifdef __cplusplus
#define AMQP_BEGIN_DECLS extern "C" {
#define AMQP_END_DECLS }
#else
/** @cond Hide from Doxygen */
#define AMQP_BEGIN_DECLS
#define AMQP_END_DECLS
/** @endcond */
#endif


/**
 * @internal
 * Important API decorators:
 *  AMQP_PUBLIC_FUNCTION - a public API function
 *  AMQP_PUBLIC_VARIABLE - a public API external variable
 *  AMQP_CALL - calling convension (used on Win32)
 */

#if defined(_WIN32) && defined(_MSC_VER)
# if defined(AMQP_BUILD) && !defined(AMQP_STATIC)
#  define AMQP_PUBLIC_FUNCTION __declspec(dllexport)
#  define AMQP_PUBLIC_VARIABLE __declspec(dllexport) extern
# else
#  define AMQP_PUBLIC_FUNCTION
#  if !defined(AMQP_STATIC)
#   define AMQP_PUBLIC_VARIABLE __declspec(dllimport) extern
#  else
#   define AMQP_PUBLIC_VARIABLE extern
#  endif
# endif
# define AMQP_CALL __cdecl

#elif defined(_WIN32) && defined(__BORLANDC__)
# if defined(AMQP_BUILD) && !defined(AMQP_STATIC)
#  define AMQP_PUBLIC_FUNCTION __declspec(dllexport)
#  define AMQP_PUBLIC_VARIABLE __declspec(dllexport) extern
# else
#  define AMQP_PUBLIC_FUNCTION
#  if !defined(AMQP_STATIC)
#   define AMQP_PUBLIC_VARIABLE __declspec(dllimport) extern
#  else
#   define AMQP_PUBLIC_VARIABLE extern
#  endif
# endif
# define AMQP_CALL __cdecl

#elif defined(_WIN32) && defined(__MINGW32__)
# if defined(AMQP_BUILD) && !defined(AMQP_STATIC)
#  define AMQP_PUBLIC_FUNCTION __declspec(dllexport)
#  define AMQP_PUBLIC_VARIABLE __declspec(dllexport) extern
# else
#  define AMQP_PUBLIC_FUNCTION
#  if !defined(AMQP_STATIC)
#   define AMQP_PUBLIC_VARIABLE __declspec(dllimport) extern
#  else
#   define AMQP_PUBLIC_VARIABLE extern
#  endif
# endif
# define AMQP_CALL __cdecl

#elif defined(_WIN32) && defined(__CYGWIN__)
# if defined(AMQP_BUILD) && !defined(AMQP_STATIC)
#  define AMQP_PUBLIC_FUNCTION __declspec(dllexport)
#  define AMQP_PUBLIC_VARIABLE __declspec(dllexport)
# else
#  define AMQP_PUBLIC_FUNCTION
#  if !defined(AMQP_STATIC)
#   define AMQP_PUBLIC_VARIABLE __declspec(dllimport) extern
#  else
#   define AMQP_PUBLIC_VARIABLE extern
#  endif
# endif
# define AMQP_CALL __cdecl

#elif defined(__GNUC__) && __GNUC__ >= 4
# include <sys/uio.h>
# define AMQP_PUBLIC_FUNCTION \
  __attribute__ ((visibility ("default")))
# define AMQP_PUBLIC_VARIABLE \
  __attribute__ ((visibility ("default"))) extern
# define AMQP_CALL
#else
/** @cond Hide from Doxygen */
# define AMQP_PUBLIC_FUNCTION
# define AMQP_PUBLIC_VARIABLE extern
# define AMQP_CALL
/** @endcond */
#endif

#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)
# define AMQP_DEPRECATED(function) \
  function __attribute__ ((__deprecated__))
#elif defined(_MSC_VER)
# define AMQP_DEPRECATED(function) \
  __declspec(deprecated) function
#else
# define AMQP_DEPRECATED(function)
#endif

/* Define ssize_t on Win32/64 platforms
   See: http://lists.cs.uiuc.edu/pipermail/llvmdev/2010-April/030649.html for details
   */
#if !defined(_W64)
#if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
#define _W64 __w64
#else
#define _W64
#endif
#endif

#ifdef _MSC_VER
#ifdef _WIN64
typedef __int64 ssize_t;
#else
typedef _W64 int ssize_t;
#endif
#endif

#include <stddef.h>
#include <stdint.h>

struct timeval;

AMQP_BEGIN_DECLS

/**
 * Default frame size
 */
#define AMQP_DEFAULT_FRAME_SIZE 131072

/**
 * Default maximum number of channels.
 */
#define AMQP_DEFAULT_MAX_CHANNELS 0

/**
 * Default heartbeat
 */
#define AMQP_DEFAULT_HEARTBEAT 0

/**
 * boolean type
 */
typedef int amqp_boolean_t;

/**
 * Method number
 */
typedef uint32_t amqp_method_number_t;

/**
 * Bitmask for flags
 */
typedef uint32_t amqp_flags_t;

/**
 * Channel type
 */
typedef uint16_t amqp_channel_t;

/**
 * Structure for holding a buffer and its size
 */
typedef struct amqp_bytes_t_ {
  size_t len;   /**< length of the buffer */
  void *bytes;  /**< pointer to the buffer data */
} amqp_bytes_t;

/**
 * For holding a decimal value
 */
typedef struct amqp_decimal_t_ {
  uint8_t decimals;   /**< the location of the decimal point */
  uint32_t value;     /**< the value before the decimal point is applied */
} amqp_decimal_t;

/**
 * An AMQP Field Table
 *
 * An amqp field table is a set of key-value pairs.
 *  Key are up to 128-byte UTF-8 character strings
 *  Values can of one or more types
 */
typedef struct amqp_table_t_ {
  int num_entries;                      /**< number of entries in the table */
  struct amqp_table_entry_t_ *entries;  /**< linked list of entries */
} amqp_table_t;

/**
 * An AMQP Field Array
 *
 * A repeated set of field values, all must be of the same type
 */
typedef struct amqp_array_t_ {
  int num_entries;                      /**< Number of entries in the table */
  struct amqp_field_value_t_ *entries;  /**< linked list of field values */
} amqp_array_t;

/*
  0-9   0-9-1   Qpid/Rabbit  Type               Remarks
---------------------------------------------------------------------------
        t       t            Boolean
        b       b            Signed 8-bit
        B                    Unsigned 8-bit
        U       s            Signed 16-bit      (A1)
        u                    Unsigned 16-bit
  I     I       I            Signed 32-bit
        i                    Unsigned 32-bit
        L       l            Signed 64-bit      (B)
        l                    Unsigned 64-bit
        f       f            32-bit float
        d       d            64-bit float
  D     D       D            Decimal
        s                    Short string       (A2)
  S     S       S            Long string
        A                    Nested Array
  T     T       T            Timestamp (u64)
  F     F       F            Nested Table
  V     V       V            Void
                x            Byte array

Remarks:

 A1, A2: Notice how the types **CONFLICT** here. In Qpid and Rabbit,
         's' means a signed 16-bit integer; in 0-9-1, it means a
          short string.

 B: Notice how the signednesses **CONFLICT** here. In Qpid and Rabbit,
    'l' means a signed 64-bit integer; in 0-9-1, it means an unsigned
    64-bit integer.

I'm going with the Qpid/Rabbit types, where there's a conflict, and
the 0-9-1 types otherwise. 0-8 is a subset of 0-9, which is a subset
of the other two, so this will work for both 0-8 and 0-9-1 branches of
the code.
*/

/**
 * A field table value
 */
typedef struct amqp_field_value_t_ {
  uint8_t kind;             /**< the type of the entry /sa amqp_field_value_kind_t */
  union {
    amqp_boolean_t boolean;   /**< boolean type AMQP_FIELD_KIND_BOOLEAN */
    int8_t i8;                /**< int8_t type AMQP_FIELD_KIND_I8 */
    uint8_t u8;               /**< uint8_t type AMQP_FIELD_KIND_U8 */
    int16_t i16;              /**< int16_t type AMQP_FIELD_KIND_I16 */
    uint16_t u16;             /**< uint16_t type AMQP_FIELD_KIND_U16 */
    int32_t i32;              /**< int32_t type AMQP_FIELD_KIND_I32 */
    uint32_t u32;             /**< uint32_t type AMQP_FIELD_KIND_U32 */
    int64_t i64;              /**< int64_t type AMQP_FIELD_KIND_I64 */
    uint64_t u64;             /**< uint64_t type AMQP_FIELD_KIND_U64, AMQP_FIELD_KIND_TIMESTAMP */
    float f32;                /**< float type AMQP_FIELD_KIND_F32 */
    double f64;               /**< double type AMQP_FIELD_KIND_F64 */
    amqp_decimal_t decimal;   /**< amqp_decimal_t AMQP_FIELD_KIND_DECIMAL */
    amqp_bytes_t bytes;       /**< amqp_bytes_t type AMQP_FIELD_KIND_UTF8, AMQP_FIELD_KIND_BYTES */
    amqp_table_t table;       /**< amqp_table_t type AMQP_FIELD_KIND_TABLE */
    amqp_array_t array;       /**< amqp_array_t type AMQP_FIELD_KIND_ARRAY */
  } value;              /**< a union of the value */
} amqp_field_value_t;

/**
 * An entry in a field-table
 */
typedef struct amqp_table_entry_t_ {
  amqp_bytes_t key;           /**< the table entry key. Its a null-terminated UTF-8 string,
                               * with a maximum size of 128 bytes */
  amqp_field_value_t value;   /**< the table entry values */
} amqp_table_entry_t;

/**
 * Field value types
 */
typedef enum {
  AMQP_FIELD_KIND_BOOLEAN = 't',  /**< boolean type. 0 = false, 1 = true @see amqp_boolean_t */
  AMQP_FIELD_KIND_I8 = 'b',       /**< 8-bit signed integer, datatype: int8_t */
  AMQP_FIELD_KIND_U8 = 'B',       /**< 8-bit unsigned integer, datatype: uint8_t */
  AMQP_FIELD_KIND_I16 = 's',      /**< 16-bit signed integer, datatype: int16_t */
  AMQP_FIELD_KIND_U16 = 'u',      /**< 16-bit unsigned integer, datatype: uint16_t */
  AMQP_FIELD_KIND_I32 = 'I',      /**< 32-bit signed integer, datatype: int32_t */
  AMQP_FIELD_KIND_U32 = 'i',      /**< 32-bit unsigned integer, datatype: uint32_t */
  AMQP_FIELD_KIND_I64 = 'l',      /**< 64-bit signed integer, datatype: int64_t */
  AMQP_FIELD_KIND_U64 = 'L',      /**< 64-bit unsigned integer, datatype: uint64_t */
  AMQP_FIELD_KIND_F32 = 'f',      /**< single-precision floating point value, datatype: float */
  AMQP_FIELD_KIND_F64 = 'd',      /**< double-precision floating point value, datatype: double */
  AMQP_FIELD_KIND_DECIMAL = 'D',  /**< amqp-decimal value, datatype: amqp_decimal_t */
  AMQP_FIELD_KIND_UTF8 = 'S',     /**< UTF-8 null-terminated character string, datatype: amqp_bytes_t */
  AMQP_FIELD_KIND_ARRAY = 'A',    /**< field array (repeated values of another datatype. datatype: amqp_array_t */
  AMQP_FIELD_KIND_TIMESTAMP = 'T',/**< 64-bit timestamp. datatype uint64_t */
  AMQP_FIELD_KIND_TABLE = 'F',    /**< field table. encapsulates a table inside a table entry. datatype: amqp_table_t */
  AMQP_FIELD_KIND_VOID = 'V',     /**< empty entry */
  AMQP_FIELD_KIND_BYTES = 'x'     /**< unformatted byte string, datatype: amqp_bytes_t */
} amqp_field_value_kind_t;

/**
 * A list of allocation blocks
 */
typedef struct amqp_pool_blocklist_t_ {
  int num_blocks;     /**< Number of blocks in the block list */
  void **blocklist;   /**< Array of memory blocks */
} amqp_pool_blocklist_t;

/**
 * A memory pool
 */
typedef struct amqp_pool_t_ {
  size_t pagesize;            /**< the size of the page in bytes.
                               *  allocations less than or equal to this size are
                               *    allocated in the pages block list
                               *  allocations greater than this are allocated in their
                               *   own block in the large_blocks block list */

  amqp_pool_blocklist_t pages;        /**< blocks that are the size of pagesize */
  amqp_pool_blocklist_t large_blocks; /**< allocations larger than the pagesize */

  int next_page;      /**< an index to the next unused page block */
  char *alloc_block;  /**< pointer to the current allocation block */
  size_t alloc_used;  /**< number of bytes in the current allocation block that has been used */
} amqp_pool_t;

/**
 * An amqp method
 */
typedef struct amqp_method_t_ {
  amqp_method_number_t id;      /**< the method id number */
  void *decoded;                /**< pointer to the decoded method,
                                 *    cast to the appropriate type to use */
} amqp_method_t;

/**
 * An AMQP frame
 */
typedef struct amqp_frame_t_ {
  uint8_t frame_type;       /**< frame type. The types:
                             * - AMQP_FRAME_METHOD - use the method union member
                             * - AMQP_FRAME_HEADER - use the properties union member
                             * - AMQP_FRAME_BODY - use the body_fragment union member
                             */
  amqp_channel_t channel;   /**< the channel the frame was received on */
  union {
    amqp_method_t method;   /**< a method, use if frame_type == AMQP_FRAME_METHOD */
    struct {
      uint16_t class_id;    /**< the class for the properties */
      uint64_t body_size;   /**< size of the body in bytes */
      void *decoded;        /**< the decoded properties */
      amqp_bytes_t raw;     /**< amqp-encoded properties structure */
    } properties;           /**< message header, a.k.a., properties,
                                  use if frame_type == AMQP_FRAME_HEADER */
    amqp_bytes_t body_fragment; /**< a body fragment, use if frame_type == AMQP_FRAME_BODY */
    struct {
      uint8_t transport_high;           /**< @internal first byte of handshake */
      uint8_t transport_low;            /**< @internal second byte of handshake */
      uint8_t protocol_version_major;   /**< @internal third byte of handshake */
      uint8_t protocol_version_minor;   /**< @internal fourth byte of handshake */
    } protocol_header;    /**< Used only when doing the initial handshake with the broker,
                                don't use otherwise */
  } payload;              /**< the payload of the frame */
} amqp_frame_t;

/**
 * Response type
 */
typedef enum amqp_response_type_enum_ {
  AMQP_RESPONSE_NONE = 0,         /**< the library got an EOF from the socket */
  AMQP_RESPONSE_NORMAL,           /**< response normal, the RPC completed successfully */
  AMQP_RESPONSE_LIBRARY_EXCEPTION,/**< library error, an error occurred in the library, examine the library_error */
  AMQP_RESPONSE_SERVER_EXCEPTION  /**< server exception, the broker returned an error, check replay */
} amqp_response_type_enum;

/**
 * Reply from a RPC method on the broker
 */
typedef struct amqp_rpc_reply_t_ {
  amqp_response_type_enum reply_type;   /**< the reply type:
                                         * - AMQP_RESPONSE_NORMAL - the RPC completed successfully
                                         * - AMQP_RESPONSE_SERVER_EXCEPTION - the broker returned
                                         *     an exception, check the reply field
                                         * - AMQP_RESPONSE_LIBRARY_EXCEPTION - the library
                                         *    encountered an error, check the library_error field
                                         */
  amqp_method_t reply;                  /**< in case of AMQP_RESPONSE_SERVER_EXCEPTION this
                                         * field will be set to the method returned from the broker */
  int library_error;                    /**< in case of AMQP_RESPONSE_LIBRARY_EXCEPTION this
                                         *    field will be set to an error code. An error
                                         *     string can be retrieved using amqp_error_string */
} amqp_rpc_reply_t;

/**
 * SASL method type
 */
typedef enum amqp_sasl_method_enum_ {
  AMQP_SASL_METHOD_PLAIN = 0      /**< the PLAIN SASL method for authentication to the broker */
} amqp_sasl_method_enum;

/**
 * connection state object
 */
typedef struct amqp_connection_state_t_ *amqp_connection_state_t;

typedef struct amqp_socket_t_ amqp_socket_t;

typedef enum amqp_status_enum_
{
  AMQP_STATUS_OK =                         0x0,
  AMQP_STATUS_NO_MEMORY =                 -0x0001,
  AMQP_STATUS_BAD_AMQP_DATA =             -0x0002,
  AMQP_STATUS_UNKNOWN_CLASS =             -0x0003,
  AMQP_STATUS_UNKNOWN_METHOD =            -0x0004,
  AMQP_STATUS_HOSTNAME_RESOLUTION_FAILED= -0x0005,
  AMQP_STATUS_INCOMPATIBLE_AMQP_VERSION = -0x0006,
  AMQP_STATUS_CONNECTION_CLOSED =         -0x0007,
  AMQP_STATUS_BAD_URL =                   -0x0008,
  AMQP_STATUS_SOCKET_ERROR =              -0x0009,
  AMQP_STATUS_INVALID_PARAMETER =         -0x000A,
  AMQP_STATUS_TABLE_TOO_BIG =             -0x000B,
  AMQP_STATUS_WRONG_METHOD =              -0x000C,
  AMQP_STATUS_TIMEOUT =                   -0x000D,
  AMQP_STATUS_TIMER_FAILURE =             -0x000E,

  AMQP_STATUS_TCP_ERROR =                 -0x0100,
  AMQP_STATUS_TCP_SOCKETLIB_INIT_ERROR =  -0x0101,

  AMQP_STATUS_SSL_ERROR =                 -0x0200,
  AMQP_STATUS_SSL_HOSTNAME_VERIFY_FAILED= -0x0201,
  AMQP_STATUS_SSL_PEER_VERIFY_FAILED =    -0x0202,
  AMQP_STATUS_SSL_CONNECTION_FAILED =     -0x0203
} amqp_status_enum;

/**
 * Gets the version of rabbitmq-c
 *
 * @return string representation of the library. Statically allocated, does not need to be freed
 */
AMQP_PUBLIC_FUNCTION
char const *
AMQP_CALL amqp_version(void);

/**
 * Empty bytes structure
 */
AMQP_PUBLIC_VARIABLE const amqp_bytes_t amqp_empty_bytes;

/**
 * Empty table structure
 */
AMQP_PUBLIC_VARIABLE const amqp_table_t amqp_empty_table;

/**
 * Empty table array structure
 */
AMQP_PUBLIC_VARIABLE const amqp_array_t amqp_empty_array;

/* Compatibility macros for the above, to avoid the need to update
   code written against earlier versions of librabbitmq. */
#define AMQP_EMPTY_BYTES amqp_empty_bytes   /**< @deprecated use amqp_empty_bytes instead. This is here only for backwards compatibility */
#define AMQP_EMPTY_TABLE amqp_empty_table   /**< @deprecated use amqp_emtpy_table instead. This is here only for backwards compatibility */
#define AMQP_EMPTY_ARRAY amqp_empty_array   /**< @deprecated use amqp_empty_array instead. This is here only for backwards compatibility */

/**
 * Initializes a amqp_pool_t memory allocation pool
 *
 * Readies an allocation pool for use. An amqp_pool_t
 * must be initialized before use
 *
 * @param [in] pool the amqp_pool_t structure to initialize.
 *              Calling this function on a pool a pool that has
 *              already been initialized will result in undefined
 *              behavior
 * @param [in] pagesize the unit size that the pool will allocate
 *              memory chunks in. Anything allocated against the pool
 *              with a requested size will be carved out of a block
 *              this size. Allocations larger than this will be
 *              allocated individually
 */
AMQP_PUBLIC_FUNCTION
void
AMQP_CALL init_amqp_pool(amqp_pool_t *pool, size_t pagesize);

/**
 * Recycles an amqp_pool_t memory allocation pool
 *
 * Recycles the space allocate by the pool
 *
 * This invalidates all allocations made against the pool
 * before the call was made. Use of those pointers will
 * have undefined behavior.
 *
 * Note: this may or may not release memory, if you're looking
 * to free memory try using empty_amqp_pool
 *
 * @param [in] pool the amqp_pool_t to recycle
 */
AMQP_PUBLIC_FUNCTION
void
AMQP_CALL recycle_amqp_pool(amqp_pool_t *pool);

/**
 * Destroys an amqp_pool_t memory allocation pool
 *
 * This is a frees all memory associated with an amqp_pool_t
 *
 * @param [in] pool the amqp_pool_t to empty
 */
AMQP_PUBLIC_FUNCTION
void
AMQP_CALL empty_amqp_pool(amqp_pool_t *pool);

/**
 * Allocates a block of memory from an amqp_pool_t memory pool
 *
 * @param [in] pool the allocation pool to allocate the memory from
 * @param [in] amount the size of the allocation in bytes
 * @return a pointer to the memory block, or NULL if the allocation cannot
 *          be satisified
 */
AMQP_PUBLIC_FUNCTION
void *
AMQP_CALL amqp_pool_alloc(amqp_pool_t *pool, size_t amount);

/**
 * Allocates a block of memory from an amqp_pool_t to an amqp_bytes_t
 *
 * @param [in] pool the allocation pool to allocate the memory from
 * @param [in] amount the size of the allocation in bytes
 * @param [in] output the location to store the pointer. On success
 *              output.bytes will be set to the beginning of the buffer
 *              output.len will be set to amount
 *              On error output.bytes will be set to NULL and output.len
 *              set to 0
 */
AMQP_PUBLIC_FUNCTION
void
AMQP_CALL amqp_pool_alloc_bytes(amqp_pool_t *pool, size_t amount, amqp_bytes_t *output);

/**
 * Wraps a c string in an amqp_bytes_t
 *
 * Takes a string, calculates its length and creates an
 * amqp_bytes_t that points to it. The string is not duplicated.
 *
 * For a given input cstr, The amqp_bytes_t output.bytes is the
 * same as cstr, output.len is the length of the string not including
 * the \0 terminator
 *
 * This function uses strlen() internally so cstr must be properly
 * terminated
 *
 * @param [in] cstr the c string to wrap
 * @return an amqp_bytes_t that describes the string
 */
AMQP_PUBLIC_FUNCTION
amqp_bytes_t
AMQP_CALL amqp_cstring_bytes(char const *cstr);

/**
 * Duplicates an amqp_bytes_t buffer.
 *
 * The buffer is cloned and the contents copied.
 *
 * The memory associated with the output is allocated
 * with amqp_bytes_malloc and should be freed with
 * amqp_bytes_free
 *
 * @param [in] src
 * @return a clone of the src
 */
AMQP_PUBLIC_FUNCTION
amqp_bytes_t
AMQP_CALL amqp_bytes_malloc_dup(amqp_bytes_t src);

/**
 * Allocates a amqp_bytes_t buffer
 *
 * Creates an amqp_bytes_t buffer of the specified amount
 *
 * @param [in] amount the size of the buffer in bytes
 * @returns an amqp_bytes_t with amount bytes allocated.
 *           output.bytes will be set to NULL on error
 */
AMQP_PUBLIC_FUNCTION
amqp_bytes_t
AMQP_CALL amqp_bytes_malloc(size_t amount);

/**
 * Frees an amqp_bytes_t buffer
 *
 * Frees a buffer allocated with amqp_bytes_malloc or amqp_bytes_malloc_dup
 *
 * Calling amqp_bytes_free on buffers not allocated with one
 * of those two functions will result in undefined behavior
 *
 * @param [in] bytes the buffer to free
 */
AMQP_PUBLIC_FUNCTION
void
AMQP_CALL amqp_bytes_free(amqp_bytes_t bytes);

/**
 * Creates a new amqp_connection_state_t object
 *
 * amqp_connection_state_t objects created with this function
 * should be freed with amqp_destroy_connection
 */
AMQP_PUBLIC_FUNCTION
amqp_connection_state_t
AMQP_CALL amqp_new_connection(void);

/**
 * Gets the sockfd associated with the connection
 *
 * @param [in] state the connection object
 * @returns the sockfd associated with the connection, > 0 if
 *           the sockfd has not been set
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_get_sockfd(amqp_connection_state_t state);


/**
 * Sets the sockfd associated with the connection
 *
 * @param [in] state the connection object
 * @param [in] sockfd the socket
 */
AMQP_DEPRECATED(
  AMQP_PUBLIC_FUNCTION
  void
  AMQP_CALL amqp_set_sockfd(amqp_connection_state_t state, int sockfd)
);

AMQP_PUBLIC_FUNCTION
void
AMQP_CALL amqp_set_socket(amqp_connection_state_t state, amqp_socket_t *socket);

/**
 * Tune various client-side connection parameters
 *
 * @param [in] state the connection object
 * @param [in] channel_max the maximum number of channels.
 *              The largest this can be is 65535
 * @param [in] frame_max the maximum size of an frame.
 *              The smallest this can be is 4096
 *              The largest this can be is 2147483647
 *              Unless you know what you're doing the recommended
 *              size is 131072
 * @param [in] heartbeat the number of seconds between heartbeats
 *              NOTE: rabbitmq-c does not support heartbeats,
 *              setting this will not have any effect
 *
 * @return 0 on success, anything else indicates error
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_tune_connection(amqp_connection_state_t state,
                               int channel_max,
                               int frame_max,
                               int heartbeat);

/**
 * Get the maximum number of channels the connection can handle
 *
 * This number can be changed using the amqp_tune_connection function
 *
 * @param [in] state the connection object
 * @return the maximum number of channels. 0 if there is no limit
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_get_channel_max(amqp_connection_state_t state);

/**
 * Destroys a connection object
 *
 * Destroys a connection object created with amqp_new_connection
 * This function will free all memory and close any sockets
 * associated with the connection
 *
 * @param [in] state the connection object
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_destroy_connection(amqp_connection_state_t state);

/**
 * Handle input
 *
 * For a given input buffer and connection state potentially decode
 * a frame from it
 *
 * @param [in] state the connection object
 * @param [in] received_data a buffer of the data to be decoded
 * @param [in] decoded_frame the frame
 * @return 0 on success, 0 > on error
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_handle_input(amqp_connection_state_t state,
                            amqp_bytes_t received_data,
                            amqp_frame_t *decoded_frame);

/**
 * Check to see if the connection is in a state it can release its internal buffers
 *
 * Call this to check before calling amqp_release_buffers.
 *
 * Alternatively call amqp_maybe_release_buffers to do this all in one step
 *
 * @param [in] state the connection object
 * @returns TRUE if the buffers can be released FALSE otherwise
 */
AMQP_PUBLIC_FUNCTION
amqp_boolean_t
AMQP_CALL amqp_release_buffers_ok(amqp_connection_state_t state);

/**
 * Release connect object internal buffers
 *
 * Call amqp_release_buffers_ok before calling this to ensure
 * the connection is in a state that it can release the buffers.
 * failing to do this will cause the program to abort.
 *
 * @param [in] state the connection object
 */
AMQP_PUBLIC_FUNCTION
void
AMQP_CALL amqp_release_buffers(amqp_connection_state_t state);

/**
 * Release buffers if possible
 *
 * Releases interal connection buffers if it is possible
 *
 * @param [in] state the connection object
 */
AMQP_PUBLIC_FUNCTION
void
AMQP_CALL amqp_maybe_release_buffers(amqp_connection_state_t state);

/**
 * Send a frame to the broker
 *
 * @param [in] state the connection object
 * @param [in] frame the frame to send to the broker
 * @return 0 on success, 0 > on error
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_send_frame(amqp_connection_state_t state, amqp_frame_t const *frame);

/**
 * Compare two table entries
 *
 * Works just like strcmp(), comparing two the table keys, datatype, then values
 *
 * @param [in] entry1 the entry on the left
 * @param [in] entry2 the entry on the right
 * @return 0 if entries are equal, 0 < if left is greater, 0 > if right is greater
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_table_entry_cmp(void const *entry1, void const *entry2);

/**
 * Attempt to open a socket
 *
 * Attempts to open a socket to hostname on portnumber
 *
 * @param [in] hostname this can be a hostname or IP address.
 *              Both IPv4 and IPv6 are acceptable
 * @param [in] portnumber the port to connect on. RabbitMQ brokers
 *              listen on port 5672, and 5671 for SSL
 * @return the sockfd, or 0 > on error
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_open_socket(char const *hostname, int portnumber);

/**
 * Send AMQP handshake to broker
 *
 * @param [in] state the connection object
 * @return -1 on error, a positive value on success
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_send_header(amqp_connection_state_t state);

/**
 * Checks to see if there are any incoming frames ready to be read
 *
 * If this function returns true, amqp_simple_wait_method will
 * return a new frame without potentially entering a blocking read()
 *
 * @param [in] state the connection object
 * @return TRUE if there are frames enqueued, FALSE otherwise
 */
AMQP_PUBLIC_FUNCTION
amqp_boolean_t
AMQP_CALL amqp_frames_enqueued(amqp_connection_state_t state);

/**
 * Waits for a single frame from the broker
 *
 * @param [in] state the connection object
 * @param [out] decoded_frame the frame
 * @return 0 on success, 0 > on error
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_simple_wait_frame(amqp_connection_state_t state,
                                 amqp_frame_t *decoded_frame);

/**
 * Waits for a specific method from the broker
 *
 * Waits for a single method on a channel from the broker.
 * If a frame is received that does not match expected_channel
 * or expected_method the program will abort
 *
 * @param [in] state the connection object
 * @param [in] expected_channel the channel that the method should be delivered on
 * @param [in] expected_method the method to wait for
 * @param [out] output the method
 * @returns 0 on success, 0 < on failure
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_simple_wait_frame_noblock(amqp_connection_state_t state,
                                         amqp_frame_t *decoded_frame,
                                         struct timeval *tv);

AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_simple_wait_method(amqp_connection_state_t state,
                                  amqp_channel_t expected_channel,
                                  amqp_method_number_t expected_method,
                                  amqp_method_t *output);

/**
 * Sends a method to the broker
 *
 * @param [in] state the connection object
 * @param [in] channel the channel object
 * @param [in] id the method number
 * @param [in] decoded the method object
 * @returns 0 on success, 0 < on failure
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_send_method(amqp_connection_state_t state,
                           amqp_channel_t channel,
                           amqp_method_number_t id,
                           void *decoded);

/**
 * Sends a method to the broker and waits for a method response
 *
 * @param [in] state the connection object
 * @param [in] channel the channel object
 * @param [in] request_id the method number of the request
 * @param [in] expected_reply_ids a 0 terminated array of expected response method numbers
 * @param [in] decoded_request_method the method to be sent to the broker
 * @return a amqp_rpc_reply_t which contains the broker response
 */
AMQP_PUBLIC_FUNCTION
amqp_rpc_reply_t
AMQP_CALL amqp_simple_rpc(amqp_connection_state_t state,
                          amqp_channel_t channel,
                          amqp_method_number_t request_id,
                          amqp_method_number_t *expected_reply_ids,
                          void *decoded_request_method);

/**
 * Sends a method to the broker and waits for a method reponse
 *
 * @param [in] state the connection object
 * @param [in] channel the channel object
 * @param [in] request_id the method number of the request
 * @param [in] reply_id the method number expected in response
 * @param [in] decoded_request_method the request method
 * @return a pointer to the method, or NULL if an error occurs
 */
AMQP_PUBLIC_FUNCTION
void *
AMQP_CALL amqp_simple_rpc_decoded(amqp_connection_state_t state,
                                  amqp_channel_t channel,
                                  amqp_method_number_t request_id,
                                  amqp_method_number_t reply_id,
                                  void *decoded_request_method);

/**
 * Get the last global amqp_rpc_reply
 *
 * The API methods corresponding to most synchronous AMQP methods
 * return a pointer to the decoded method result.  Upon error, they
 * return NULL, and we need some way of discovering what, if anything,
 * went wrong. amqp_get_rpc_reply() returns the most recent
 * amqp_rpc_reply_t instance corresponding to such an API operation
 * for the given connection.
 *
 * Only use it for operations that do not themselves return
 * amqp_rpc_reply_t; operations that do return amqp_rpc_reply_t
 * generally do NOT update this per-connection-global amqp_rpc_reply_t
 * instance.
 *
 * @param [in] state the connection object
 * @return the most recent amqp_rpc_reply_t
 */
AMQP_PUBLIC_FUNCTION
amqp_rpc_reply_t
AMQP_CALL amqp_get_rpc_reply(amqp_connection_state_t state);

/**
 * Login to the broker
 *
 * After using amqp_open_socket and amqp_set_sockfd, call
 * amqp_login to complete connecting to the broker
 *
 * @param [in] state the connection object
 * @param [in] vhost the virtual host on the broker. The default virtual
 *              host on most brokers is "/"
 * @param [in] channel_max the maximum number of channels to request of
 *              the broker for this connection. 0 means no limit, this is a
 *              good default.
 * @param [in] frame_max the maximum size of an AMQP frame on the wire to
 *              request of the broker for this connection. 4096 is the minimum
 *              size, 2^31-1 is the maximum, a good default is 131072 (128KB).
 * @param [in] heartbeat the number of seconds between heartbeat frames to
 *              request of the broker. A value of 0 disables heartbeats.
 *              NOTE: rabbitmq-c does not support heartbeats, your best bet
 *              is not to implement this.
 * @param [in] sasl_method the SASL method to authenticate with the broker.
 *              followed by the authentication information.
 *              For AMQP_SASL_METHOD_PLAIN, the AMQP_SASL_METHOD_PLAIN
 *              should be followed by two arguments in this order:
 *              const char* username, and const char* password.
 * @return amqp_rpc_reply_t indicating success or failure.
 */
AMQP_PUBLIC_FUNCTION
amqp_rpc_reply_t
AMQP_CALL amqp_login(amqp_connection_state_t state, char const *vhost,
                     int channel_max, int frame_max, int heartbeat,
                     amqp_sasl_method_enum sasl_method, ...);

AMQP_PUBLIC_FUNCTION
amqp_rpc_reply_t
AMQP_CALL amqp_login_with_properties(amqp_connection_state_t state, char const *vhost,
                                     int channel_max, int frame_max, int heartbeat,
                                     const amqp_table_t *properties, amqp_sasl_method_enum sasl_method, ...);

struct amqp_basic_properties_t_;

/**
 * Publish a message to the broker
 *
 * @param [in] state the connection object
 * @param [in] channel the channel identifier
 * @param [in] exchange the exchange on the broker to publish to
 * @param [in] routing_key the routing key to use when publishing the message
 * @param [in] mandatory indicate to the broker that the message MUST be routed
 *              to a queue. If the broker cannot do this it should respond with
 *              a basic.reject method.
 * @param [in] immediate indicate to the broker that the message MUST be delivered
 *              to a consumer immediately. If the broker cannot do this it should
 *              response with a basic.reject method.
 * @param [in] properties the properties associated with the message
 * @param [in] body the message body
 * @return 0 on success, 0 > on failure to transmit the message to the broker.
 *          since basic.publish is an async method, this return value will not
 *          indicate whether the broker encountered an error, such as a non-existant
 *          exchange, or a failure in processing due to a mandatory or immediate flag.
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_basic_publish(amqp_connection_state_t state, amqp_channel_t channel,
                             amqp_bytes_t exchange, amqp_bytes_t routing_key,
                             amqp_boolean_t mandatory, amqp_boolean_t immediate,
                             struct amqp_basic_properties_t_ const *properties,
                             amqp_bytes_t body);

/**
 * Closes an channel
 *
 * @param [in] state the connection object
 * @param [in] channel the channel identifier
 * @param [in] code the reason for closing the channel, AMQP_REPLY_SUCCESS is a good default
 * @return amqp_rpc_reply_t indicating success or failure
 */
AMQP_PUBLIC_FUNCTION
amqp_rpc_reply_t
AMQP_CALL amqp_channel_close(amqp_connection_state_t state, amqp_channel_t channel,
                             int code);

/**
 * Closes the entire connection
 *
 * Implicitly closes all channels and informs the broker the connection
 * is being closed, after receiving acknowldgement from the broker it closes
 * the socket.
 *
 * @param [in] state the connection object
 * @param [in] code the reason code for closing the connection. AMQP_REPLY_SUCCESS is a good default.
 * @return amqp_rpc_reply_t indicating the result
 */
AMQP_PUBLIC_FUNCTION
amqp_rpc_reply_t
AMQP_CALL amqp_connection_close(amqp_connection_state_t state, int code);

/**
 * Acknowledges a message
 *
 * Does a basic.ack on a received message
 *
 * @param [in] state the connection object
 * @param [in] channel the channel identifier
 * @param [in] delivery_tag the delivery take of the message to be ack'd
 * @param [in] multiple if true, ack all messages up to this delivery tag, if
 *              false ack only this delivery tag
 * @return 0 on success,  0 > on failing to send the ack to the broker.
 *            this will not indicate failure if something goes wrong on the broker
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_basic_ack(amqp_connection_state_t state, amqp_channel_t channel,
                         uint64_t delivery_tag, amqp_boolean_t multiple);

/**
 * Do a basic.get
 *
 * Synchonously polls the broker for a message in a queue, and
 * retrieves the message if a message is in the queue.
 *
 * @param [in] state the connection object
 * @param [in] channel the channel identifier to use
 * @param [in] queue the queue name to retrieve from
 * @param [in] no_ack if true the message is automatically ack'ed
 *              if false amqp_basic_ack should be called once the message
 *              retrieved has been processed
 * @return amqp_rpc_reply indicating success or failure
 */
AMQP_PUBLIC_FUNCTION
amqp_rpc_reply_t
AMQP_CALL amqp_basic_get(amqp_connection_state_t state, amqp_channel_t channel,
                         amqp_bytes_t queue, amqp_boolean_t no_ack);

/**
 * Do a basic.reject
 *
 * Actively reject a message that has been delivered
 *
 * @param [in] state the connection object
 * @param [in] channel the channel identifier
 * @param [in] delivery_tag the delivery tag of the message to reject
 * @param [in] requeue indicate to the broker whether it should requeue the
 *              message or just discard it.
 * @return 0 on success, 0 > on failing to send the reject method to the broker.
 *          This will not indicate failure if something goes wrong on the broker.
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_basic_reject(amqp_connection_state_t state, amqp_channel_t channel,
                            uint64_t delivery_tag, amqp_boolean_t requeue);

/**
 * Check to see if there is data left in the receive buffer
 *
 * Can be used to see if there is data still in the buffer, if so
 * calling amqp_simple_wait_frame will not immediately enter a
 * blocking read.
 *
 * @param [in] state the connection object
 * @return true if there is data in the recieve buffer, false otherwise
 */
AMQP_PUBLIC_FUNCTION
amqp_boolean_t
AMQP_CALL amqp_data_in_buffer(amqp_connection_state_t state);

/**
 * Get the error string for the given error code.
 *
 * @deprecated This function has been deprecated in favor of
 *  \ref amqp_error_string2() which returns statically allocated
 *  string which do not need to be freed by the caller.
 *
 * The returned string resides on the heap; the caller is responsible
 * for freeing it.
 *
 * @param [in] err return error code
 * @return the error string
 */
AMQP_DEPRECATED(
  AMQP_PUBLIC_FUNCTION
  char *
  AMQP_CALL amqp_error_string(int err)
);

AMQP_PUBLIC_FUNCTION
const char *
AMQP_CALL amqp_error_string2(int err);


/**
 * Deserializes an amqp_table_t from AMQP wireformat
 *
 * This is an internal function and is not typically used by
 * client applications
 *
 * @param [in] encoded the buffer containing the serialized data
 * @param [in] pool memory pool used to allocate the table entries from
 * @param [in] output the amqp_table_t structure to fill in. Any existing
 *             entries will be erased
 * @param [in,out] offset The offset into the encoded buffer to start
 *                 reading the serialized table. It will be updated
 *                 by this function to end of the table
 * @return 0 on success, > 1 on error
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_decode_table(amqp_bytes_t encoded, amqp_pool_t *pool,
                            amqp_table_t *output, size_t *offset);

/**
 * Serializes an amqp_table_t to the AMQP wireformat
 *
 * This is an internal function and is not typically used by
 * client applications
 *
 * @param [in] encoded the buffer where to serialize the table to
 * @param [in] input the amqp_table_t to serialize
 * @param [in,out] offset The offset into the encoded buffer to start
 *                 writing the serialized table. It will be updated
 *                 by this function to where writing left off
 * @return 0 on success, > 1 on error
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_encode_table(amqp_bytes_t encoded, amqp_table_t *input, size_t *offset);

/**
 * Parameters used to connect to the RabbitMQ broker
 */
struct amqp_connection_info {
  char *user;                 /**< the username to authenticate with the broker, default on most broker is 'guest' */
  char *password;             /**< the password to authenticate with the broker, default on most brokers is 'guest' */
  char *host;                 /**< the hostname of the broker */
  char *vhost;                /**< the virtual host on the broker to connect to, a good default is "/" */
  int port;                   /**< the port that the broker is listening on, default on most brokers is 5672 */
  amqp_boolean_t ssl;
};

/**
 * Initialze an amqp_connection_info to default values
 *
 * The default values are:
 * - user: "guest"
 * - password: "guest"
 * - host: "localhost"
 * - vhost: "/"
 * - port: 5672
 *
 * @param [out] parsed the connection info to set defaults on
 */
AMQP_PUBLIC_FUNCTION
void
AMQP_CALL amqp_default_connection_info(struct amqp_connection_info *parsed);

/**
 * Parse a connection URL
 *
 * An amqp connection url takes the form:
 *
 * amqp://[$USERNAME[:$PASSWORD]\@]$HOST[:$PORT]/[$VHOST]
 *
 * Examples:
 *  amqp://guest:guest\@localhost:5672//
 *  amqp://guest:guest\@localhost/myvhost
 *
 * @param [in] url URI to parse
 * @param [out] parsed the connection info gleaned from the URI
 * @returns 0 on success, anything else indicates failure
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_parse_url(char *url, struct amqp_connection_info *parsed);

/* socket API */

/**
 * Open a socket connection.
 *
 * This function opens a socket connection returned from amqp_tcp_socket_new()
 * or amqp_ssl_socket_new(). This function should be called after setting
 * socket options and prior to assigning the socket to an AMQP connection with
 * amqp_set_socket().
 *
 * \param [in,out] self A socket object.
 * \param [in] host Connect to this host.
 * \param [in] port Connect on this remote port.
 *
 * \return Zero upon success, non-zero otherwise.
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL
amqp_socket_open(amqp_socket_t *self, const char *host, int port);

/**
 * Close a socket connection and free resources.
 *
 * This function closes a socket connection and releases any resources used by
 * the object. After calling this function the specified socket should no
 * longer be referenced.
 *
 * \param [in,out] self A socket object.
 *
 * \return Zero upon success, non-zero otherwise.
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL
amqp_socket_close(amqp_socket_t *self);

/**
 * Retrieve an error code for the last socket operation.
 *
 * At the time of writing, this interface is not well supported and is subject
 * to changes!
 *
 * \param [in,out] self A socket object.
 *
 * \return Zero upon success, an opaque error code otherwise
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL
amqp_socket_error(amqp_socket_t *self);

/**
 * Get the socket descriptor in use by a socket object.
 *
 * Retrieve the underlying socket descriptor. This function can be used to
 * perform low-level socket operations that aren't supported by the socket
 * interface. Use with caution!
 *
 * \param [in,out] self A socket object.
 *
 * \return The underlying socket descriptor.
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL
amqp_socket_get_sockfd(amqp_socket_t *self);

AMQP_END_DECLS

#include <amqp_framing.h>

#endif /* AMQP_H */
