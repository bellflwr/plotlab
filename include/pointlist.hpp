#ifndef POINTLIST_H
#define POINTLIST_H

#include "directives.hpp"
#include "project.hpp"

namespace plotlab {
class PointList {
  public:
    PointList();
    void draw_window(project& proj);
    void draw_directive(directive* dir, int& id);
    void draw_directive(point_directive* dir, int& id);
    void draw_directive(struct draw_directive* dir, int& id);
    void draw_directive(bezier_directive* dir, int& id);
};
} // namespace plotlab

#endif /* POINTLIST_H */
