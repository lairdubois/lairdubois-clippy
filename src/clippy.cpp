#include "clipper2/clipper.core.h"
#include "clipper2/clipper.h"


#include <clippy.h>

using namespace Clipper2Lib;

#ifdef __cplusplus
extern "C" {
#endif

Paths64 subjects, clips, solution;

LIBCLIPPY_EXPORTS void c_clear_subjects(void) {
  subjects.clear();
}

LIBCLIPPY_EXPORTS void c_append_subject(const int64_t *coords, size_t len) {

  std::vector<int64_t> list;
  list.reserve(len);
  list.assign(coords, coords + len);

  subjects.push_back(MakePath(list));
}

LIBCLIPPY_EXPORTS void c_clear_clips(void) {
  clips.clear();
}

LIBCLIPPY_EXPORTS void c_append_clip(const int64_t *coords, size_t len) {

  std::vector<int64_t> list;
  list.reserve(len);
  list.assign(coords, coords + len);

  clips.push_back(MakePath(list));
}

LIBCLIPPY_EXPORTS size_t c_compute_union(void) {
  solution = Union(subjects, clips, FillRule::NonZero);
  return solution.size();
}

LIBCLIPPY_EXPORTS size_t c_compute_difference(void) {
  solution = Difference(subjects, clips, FillRule::NonZero);
  return solution.size();
}

LIBCLIPPY_EXPORTS void c_clear_solution(void) {
  solution.clear();
}

LIBCLIPPY_EXPORTS size_t c_get_solution_len() {
  return solution.size();
}

LIBCLIPPY_EXPORTS size_t c_get_solution_path_len_at(int index) {
  return solution[index].size();
}

LIBCLIPPY_EXPORTS int64_t* c_get_solution_path_coords_at(int index) {

  Path64 path = solution[index];

  auto *coords = (int64_t *)malloc(sizeof(int64_t) * path.size() * 2);
  int i = 0;
  for (Point64 point : path) {
    coords[i] = point.x;
    coords[i + 1] = point.y;
    i = i + 2;
  }

  return coords;
}

LIBCLIPPY_EXPORTS void c_free_coords(const int64_t* coords) {
    free((void *) coords);
}

#ifdef __cplusplus
}
#endif