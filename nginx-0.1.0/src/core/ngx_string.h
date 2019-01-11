
/*
 * Copyright (C) Igor Sysoev
 */


#ifndef _NGX_STRING_H_INCLUDED_
#define _NGX_STRING_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef struct {
    size_t    len;
    u_char   *data;
} ngx_str_t;

// 初始化字符串/空串
#define ngx_string(str)  { sizeof(str) - 1, (u_char *) str }
#define ngx_null_string  { 0, NULL }


#if (WIN32)

// 忽略大小写比较字符串的n位
#define ngx_strncasecmp(s1, s2, n)                                           \
                           strnicmp((const char *) s1, (const char *) s2, n)
// 忽略大小写比较字符串
#define ngx_strcasecmp(s1, s2)                                               \
                           stricmp((const char *) s1, (const char *) s2)

#define ngx_snprintf       _snprintf
#define ngx_vsnprintf      _vsnprintf

#else

// 忽略大小写比较字符串的n位
#define ngx_strncasecmp(s1, s2, n)                                           \
                           strncasecmp((const char *) s1, (const char *) s2, n)
// 忽略大小写比较字符串
#define ngx_strcasecmp(s1, s2)                                               \
                           strcasecmp((const char *) s1, (const char *) s2)

#define ngx_snprintf       snprintf
#define ngx_vsnprintf      vsnprintf

#endif


#define ngx_strncmp(s1, s2, n)                                               \
                            strncmp((const char *) s1, (const char *) s2, n)

/* msvc and icc compile strcmp() to inline loop */
#define ngx_strcmp(s1, s2)  strcmp((const char *) s1, (const char *) s2)

#define ngx_strstr(s1, s2)  strstr((const char *) s1, (const char *) s2)
#define ngx_strlen(s)       strlen((const char *) s)

/*
 * msvc and icc compile memset() to the inline "rep stos"
 * while ZeroMemory() and bzero() are the calls.
 * icc may also inline several mov's of a zeroed register for small blocks.
 */
// 字符串清0
#define ngx_memzero(buf, n)       memset(buf, 0, n)

/* msvc and icc compile memcpy() to the inline "rep movs" */
// 拷贝
#define ngx_memcpy(dst, src, n)   memcpy(dst, src, n)
#define ngx_cpymem(dst, src, n)   ((u_char *) memcpy(dst, src, n)) + n

/* msvc and icc compile memcmp() to the inline loop */
#define ngx_memcmp                memcmp

// 从src copy n-1个字符（遇0结束）到dst，并给dst后补一个'\0'，返回'\0'位置 */
u_char *ngx_cpystrn(u_char *dst, u_char *src, size_t n);
// 比较n个字符串
ngx_int_t ngx_rstrncmp(u_char *s1, u_char *s2, size_t n);

// 字符串转整型，不能有正负号，不能有无关字符
ngx_int_t ngx_atoi(u_char *line, size_t n);
// 十六进制转整型，不能有无关符号，支持大小写
ngx_int_t ngx_hextoi(u_char *line, size_t n);
// 二进制md5，转字符型md5
void ngx_md5_text(u_char *text, u_char *md5);

// 取base64转换长度
#define ngx_base64_encoded_length(len)  (((len + 2) / 3) * 4)
#define ngx_base64_decoded_length(len)  (((len + 3) / 4) * 3)

// base64编码
void ngx_encode_base64(ngx_str_t *src, ngx_str_t *dst);
// base64解码
ngx_int_t ngx_decode_base64(ngx_str_t *src, ngx_str_t *dst);


#define  ngx_qsort                qsort


#define  ngx_value_helper(n)      #n
#define  ngx_value(n)             ngx_value_helper(n)


#endif /* _NGX_STRING_H_INCLUDED_ */
