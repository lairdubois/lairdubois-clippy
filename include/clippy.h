#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LIBCLIPPY_EXPORTS
#define DLL_EXPORTS __declspec(dllexport)
#else
#define DLL_EXPORTS
#endif

DLL_EXPORTS void c_clear_subjects(void);
DLL_EXPORTS void c_append_subject(const int64_t *coords, size_t len);

DLL_EXPORTS void c_clear_clips(void);
DLL_EXPORTS void c_append_clip(const int64_t *coords, size_t len);

DLL_EXPORTS size_t c_compute_union(void);
DLL_EXPORTS size_t c_compute_difference(void);

DLL_EXPORTS void c_clear_solution(void);
DLL_EXPORTS size_t c_get_solution_len();
DLL_EXPORTS size_t c_get_solution_path_len_at(int index);
DLL_EXPORTS int64_t* c_get_solution_path_coords_at(int index);

DLL_EXPORTS void c_free_coords(const int64_t* coords);

#ifdef __cplusplus
}
#endif