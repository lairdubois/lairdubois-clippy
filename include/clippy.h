#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LIBCLIPPY_EXPORTS
#define DLL_EXPORTS __declspec(dllexport)
#else
#define DLL_EXPORTS
#endif

DLL_EXPORTS void c_clear_subjects(void);
DLL_EXPORTS void c_append_subject(const int64_t *cpath, size_t len);

DLL_EXPORTS void c_clear_clips(void);
DLL_EXPORTS void c_append_clip(const int64_t *cpath, size_t len);

DLL_EXPORTS void c_compute_union(void);
DLL_EXPORTS void c_compute_difference(void);
DLL_EXPORTS void c_compute_intersection(void);
DLL_EXPORTS void c_compute_outers(void);

DLL_EXPORTS void c_clear_solution(void);
DLL_EXPORTS size_t c_get_solution_len(void);
DLL_EXPORTS size_t c_get_solution_cpath_len_at(int index);
DLL_EXPORTS int64_t* c_get_solution_cpath_at(int index);

DLL_EXPORTS int c_is_cpath_positive(const int64_t* cpath, size_t len);
DLL_EXPORTS double c_get_cpath_area(const int64_t* cpath, size_t len);

DLL_EXPORTS void c_free_cpath(const int64_t* cpath);

DLL_EXPORTS char* c_version(void);

#ifdef __cplusplus
}
#endif