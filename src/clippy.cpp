#include "clipper2/clipper.core.h"
#include "clipper2/clipper.h"
#include <clippy.h>

using namespace Clipper2Lib;

#ifdef __cplusplus
extern "C" {
#endif

Paths64 subjects, clips, solution;

void c_clear_subjects(void) {
  subjects.clear();
}

void c_append_subject(const int64_t *coords, size_t len) {

  std::vector<int64_t> list;
  list.reserve(len);
  list.assign(coords, coords + len);

  subjects.push_back(MakePath(list));
}

void c_clear_clips(void) {
  clips.clear();
}

void c_append_clip(const int64_t *coords, size_t len) {

  std::vector<int64_t> list;
  list.reserve(len);
  list.assign(coords, coords + len);

  clips.push_back(MakePath(list));
}

size_t c_compute_union(void) {
  solution = Union(subjects, clips, FillRule::NonZero);
  return solution.size();
}

size_t c_compute_difference(void) {
  solution = Difference(subjects, clips, FillRule::NonZero);
  return solution.size();
}

void c_clear_solution(void) {
  solution.clear();
}

size_t c_get_solution_len() {
  return solution.size();
}

size_t c_get_solution_path_len_at(int index) {
  return solution[index].size();
}

int64_t* c_get_solution_path_coords_at(int index) {

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

#ifdef __cplusplus
}
#endif