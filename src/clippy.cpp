#include "clipper2/clipper.core.h"
#include "clipper2/clipper.h"

#include <clippy.h>

using namespace Clipper2Lib;

Path64 MakePath64(const int64_t *cpath, size_t len) {

  std::vector<int64_t> list;
  list.reserve(len);
  list.assign(cpath, cpath + len);

  return MakePath(list);
}

#ifdef __cplusplus
extern "C" {
#endif

Paths64 subjects, clips, solution;

DLL_EXPORTS void c_clear_subjects(void) {
  subjects.clear();
}

DLL_EXPORTS void c_append_subject(const int64_t *cpath, size_t len) {
  subjects.push_back(MakePath64(cpath, len));
}

DLL_EXPORTS void c_clear_clips(void) {
  clips.clear();
}

DLL_EXPORTS void c_append_clip(const int64_t *cpath, size_t len) {
  clips.push_back(MakePath64(cpath, len));
}

DLL_EXPORTS size_t c_compute_union(void) {
  solution = Union(subjects, clips, FillRule::NonZero);
  return solution.size();
}

DLL_EXPORTS size_t c_compute_difference(void) {
  solution = Difference(subjects, clips, FillRule::NonZero);
  return solution.size();
}

DLL_EXPORTS size_t c_compute_intersection(void) {
  solution = Difference(subjects, clips, FillRule::NonZero);
  return solution.size();
}

DLL_EXPORTS void c_clear_solution(void) {
  solution.clear();
}

DLL_EXPORTS size_t c_get_solution_len() {
  return solution.size();
}

DLL_EXPORTS size_t c_get_solution_cpath_len_at(int index) {
  return solution[index].size();
}

DLL_EXPORTS int64_t* c_get_solution_cpath_at(int index) {

  Path64 path = solution.at(index);

  auto *cpath = (int64_t *)malloc(sizeof(int64_t) * path.size() * 2);
  if (cpath != nullptr) {
    int i = 0;
    for (Point64 point : path) {
      cpath[i] = point.x;
      cpath[i + 1] = point.y;
      i = i + 2;
    }
  }

  return cpath;
}

DLL_EXPORTS bool c_is_cpath_positive(const int64_t* cpath, size_t len) {
  return IsPositive(MakePath64(cpath, len));
}

DLL_EXPORTS double c_get_cpath_area(const int64_t* cpath, size_t len) {
  return Area(MakePath64(cpath, len));
}

DLL_EXPORTS void c_free_cpath(const int64_t* cpath) {
  free((void *) cpath);
}

#ifdef __cplusplus
}
#endif