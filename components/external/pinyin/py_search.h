#ifndef _PY_SEARCH_H_
#define _PY_SEARCH_H_

#include <stdint.h>

#define PY_LIST_2 // PY_LIST_1,PY_LIST_2

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _py_mb
{
    const char *msg; // ƴ����
#ifdef PY_LIST_2
    uint8_t wlen; // �ִʳ���
#endif
    const char *mb; // ���
}py_mb_t;

typedef struct _py_num
{
    const char *msg; // ���ִ�
    uint16_t index; // ������
}py_num_t;

uint8_t py_mb_search(const char *msg, const py_mb_t **list, uint8_t size);
uint8_t py_num_search(const char *msg, const py_mb_t **list, uint8_t size);

#ifdef __cplusplus
}
#endif

#endif
