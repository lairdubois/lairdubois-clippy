#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

LIBCLIPPY_EXPORTS void c_clear_subjects(void);
LIBCLIPPY_EXPORTS void c_append_subject(const int64_t *coords, size_t len);

LIBCLIPPY_EXPORTS void c_clear_clips(void);
LIBCLIPPY_EXPORTS void c_append_clip(const int64_t *coords, size_t len);

LIBCLIPPY_EXPORTS size_t c_compute_union(void);
LIBCLIPPY_EXPORTS size_t c_compute_difference(void);

LIBCLIPPY_EXPORTS void c_clear_solution(void);
LIBCLIPPY_EXPORTS size_t c_get_solution_len();
LIBCLIPPY_EXPORTS size_t c_get_solution_path_len_at(int index);
LIBCLIPPY_EXPORTS int64_t* c_get_solution_path_coords_at(int index);

LIBCLIPPY_EXPORTS void c_free_coords(const int64_t* coords);

#ifdef __cplusplus
}
#endif