#include "clipper2/clipper.core.h"
#include "clipper2/clipper.h"

#include <clippy.h>

using namespace Clipper2Lib;

// Data manipulators

static Path64 ConvertCPath(const int64_t* cpath) {
  Path64 result;
  if (!cpath) return result;
  const int64_t* v = cpath;
  size_t cnt = *v;
  v += 2;
  result.reserve(cnt);
  for (size_t i = 0; i < cnt; ++i) {
    int64_t x = *v++, y = *v++;
    result.push_back(Point64(x, y));
  }
  return result;
}

static Paths64 ConvertCPaths(const int64_t* cpaths) {
  Paths64 result;
  if (!cpaths) return result;
  const int64_t* v = cpaths;
  ++v;
  size_t cnt = *v++;
  result.reserve(cnt);
  for (size_t i = 0; i < cnt; ++i) {
    result.push_back(ConvertCPath(v));
  }
  return result;
}

static void GetPathCountAndCPathsArrayLen(const Paths64 &paths, size_t &cnt, size_t &array_len) {
  array_len = 2;
  cnt = 0;
  for (const Path64 &path: paths) {
    if (path.empty()) continue;
    array_len += path.size() * 2 + 2;
    ++cnt;
  }
}

static size_t GetPolyPath64ArrayLen(const PolyPath64 &polypath) {
  size_t result = 2; // poly_length + child_count
  result += polypath.Polygon().size() * 2;
  // + nested children
  for (size_t i = 0; i < polypath.Count(); ++i) {
    result += GetPolyPath64ArrayLen(*polypath[i]);
  }
  return result;
}

static void GetPolytreeCountAndCArrayLen(const PolyTree64 &tree, size_t &cnt, size_t &array_len) {
  cnt = tree.Count(); // nb: top level count only
  array_len = GetPolyPath64ArrayLen(tree);
}

static int64_t* CreateCPaths(const Paths64 &paths) {

  /*

   Paths
    |counter|path1|path2|...|pathN
    |L  ,N  |     |     |...|

    L = Array length
    N = Number of paths

   Path
    |counter|coord1|coord2|...|coordN
    |N  ,0  |x1, y1|x2, y2|...|xN, yN

    N = Number of coords

   */

  size_t cnt = 0, array_len = 0;
  GetPathCountAndCPathsArrayLen(paths, cnt, array_len);
  int64_t *result = new int64_t[array_len], *v = result;
  *v++ = static_cast<int64_t>(array_len);
  *v++ = static_cast<int64_t>(cnt);
  for (const Path64 &path: paths) {
    if (path.empty()) continue;
    *v++ = static_cast<int64_t>(path.size());
    *v++ = 0;
    for (const Point64 &pt: path) {
      *v++ = pt.x;
      *v++ = pt.y;
    }
  }
  return result;
}

static void CreateCPolyPath(const PolyPath64* polypath, int64_t* &v) {

  /*

   PolyPath
    |counter|coord1|coord2|...|coordN|child1|child2|...|childC|
    |N  ,C  |x1, y1|x2, y2|...|xN, yN|                        |

    N = Number of coords
    C = Number of children

   */

  *v++ = static_cast<int64_t>(polypath->Polygon().size());
  *v++ = static_cast<int64_t>(polypath->Count());
  for (const Point64 &pt: polypath->Polygon()) {
    *v++ = static_cast<int64_t>(pt.x);
    *v++ = static_cast<int64_t>(pt.y);
  }
  for (size_t i = 0; i < polypath->Count(); ++i) {
    CreateCPolyPath(polypath->Child(i), v);
  }
}

static int64_t* CreateCPolyTree(const PolyTree64 &polytree) {

  /*

   PolyTree
    |counter|child1|child2|...|childC|
    |L  , C |                        |

    L = Array length
    C = Number of children

   */

  size_t cnt, array_len;
  GetPolytreeCountAndCArrayLen(polytree, cnt, array_len);
  if (!cnt) return nullptr;
  int64_t *result = new int64_t[array_len], *v = result;
  *v++ = static_cast<int64_t>(array_len);
  *v++ = static_cast<int64_t>(polytree.Count());
  for (size_t i = 0; i < polytree.Count(); ++i) {
    CreateCPolyPath(polytree.Child(i), v);
  }
  return result;
}

// Execution

static void ExecuteBooleanOp(ClipType clip_type, Paths64 &subjects, Paths64 &clips, Paths64 &solution) {
  Clipper64 clipper;
  clipper.PreserveCollinear(false);
  clipper.AddSubject(subjects);
  clipper.AddClip(clips);
  clipper.Execute(clip_type, FillRule::NonZero, solution);
}

static void ExecuteBooleanOp(ClipType clip_type, Paths64 &subjects, Paths64 &clips, PolyTree64 &tree_solution) {
  Clipper64 clipper;
  clipper.PreserveCollinear(false);
  clipper.AddSubject(subjects);
  clipper.AddClip(clips);
  clipper.Execute(clip_type, FillRule::NonZero, tree_solution);
}

#ifdef __cplusplus
extern "C" {
#endif

Paths64 subjects, clips, solution;
PolyTree64 tree_solution;

DLL_EXPORTS void c_clear_subjects(void) {
  subjects.clear();
}

DLL_EXPORTS void c_append_subject(const int64_t *cpath) {
  subjects.push_back(ConvertCPath(cpath));
}


DLL_EXPORTS void c_clear_clips(void) {
  clips.clear();
}

DLL_EXPORTS void c_append_clip(const int64_t *cpath) {
  clips.push_back(ConvertCPath(cpath));
}


DLL_EXPORTS void c_compute_union(void) {
  ExecuteBooleanOp(ClipType::Union, subjects, clips, solution);
}

DLL_EXPORTS void c_compute_difference(void) {
  ExecuteBooleanOp(ClipType::Difference, subjects, clips, solution);
}

DLL_EXPORTS void c_compute_intersection(void) {
  ExecuteBooleanOp(ClipType::Intersection, subjects, clips, solution);
}

DLL_EXPORTS void c_compute_outers(void) {
  PolyTree64 polytree;
  ExecuteBooleanOp(ClipType::Union, subjects, clips, polytree);
  for (const auto& polypath : polytree) {
    solution.push_back(polypath->Polygon());
  }
}

DLL_EXPORTS void c_compute_tree(void) {
  ExecuteBooleanOp(ClipType::Union, subjects, clips, tree_solution);
}


DLL_EXPORTS void c_clear_solution(void) {
  solution.clear();
}

DLL_EXPORTS int64_t* c_get_solution(void) {
  return CreateCPaths(solution);
}

DLL_EXPORTS void c_clear_tree_solution(void) {
  tree_solution.Clear();
}

DLL_EXPORTS int64_t* c_get_tree_solution(void) {
  return CreateCPolyTree(tree_solution);
}


DLL_EXPORTS int c_is_cpath_positive(const int64_t* cpath) {
  return IsPositive(ConvertCPath(cpath)) ? 1 : 0;
}

DLL_EXPORTS double c_get_cpath_area(const int64_t* cpath) {
  return Area(ConvertCPath(cpath));
}


DLL_EXPORTS void c_dispose_array64(const int64_t* p) {
  delete[] p;
}


DLL_EXPORTS char* c_version(void) {
  return (char *)CLIPPER2_VERSION;
}

#ifdef __cplusplus
}
#endif