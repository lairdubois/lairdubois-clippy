#include <stddef.h>
#include <stdint.h>

#ifdef LIBCLIPPY_EXPORTS
#define LIBCLIPPY_API __declspec(dllexport)
#else
#define LIBCLIPPY_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

LIBCLIPPY_API void c_clear_subjects(void);
LIBCLIPPY_API void c_append_subject(const int64_t *coords, size_t len);

LIBCLIPPY_API void c_clear_clips(void);
LIBCLIPPY_API void c_append_clip(const int64_t *coords, size_t len);

LIBCLIPPY_API size_t c_compute_union(void);
LIBCLIPPY_API size_t c_compute_difference(void);

LIBCLIPPY_API void c_clear_solution(void);
LIBCLIPPY_API size_t c_get_solution_len();
LIBCLIPPY_API size_t c_get_solution_path_len_at(int index);
LIBCLIPPY_API int64_t* c_get_solution_path_coords_at(int index);

LIBCLIPPY_API void c_free_coords(const int64_t* coords);


#ifdef __cplusplus
}
#endif