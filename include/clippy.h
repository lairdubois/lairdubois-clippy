#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void c_clear_subjects(void);
void c_append_subject(const int64_t *coords, size_t len);

void c_clear_clips(void);
void c_append_clip(const int64_t *coords, size_t len);

size_t c_compute_union(void);
size_t c_compute_difference(void);

void c_clear_solution(void);
size_t c_get_solution_len();
size_t c_get_solution_path_len_at(int index);
int64_t* c_get_solution_path_coords_at(int index);

#ifdef __cplusplus
}
#endif