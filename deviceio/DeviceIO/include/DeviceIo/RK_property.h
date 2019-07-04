#ifndef __RK_PROPERTY_H__
#define __RK_PROPERTY_H__

#ifdef __cplusplus
extern "C" {
#endif

int RK_property_init(void);
int RK_property_get(const char *key, char *value, const char *def);
int RK_property_set(const char *key, const char *value);
void RK_property_print(void);

#ifdef __cplusplus
}
#endif

#endif
