#include "Ring.h"

#include <cmath>

Ring::Ring(double thick, double r_out, double r_inner)
{
    this->thick     = thick;
    this->r_out     = r_out;
    this->r_inner   = r_inner;
}

bool Ring::is_rim_surface(double x, double y, double z, double z_rad)
{
    return this->is_rim_side(x, y, z, z_rad) || this->is_rim_edge(x, y, z, z_rad);
}

bool Ring::is_rim_edge(double x, double y, double z, double z_rad)
{
    double edge_xdist = sqrt(pow(this->r_out, 2) - pow(z, 2)) *
        cos(this->z_rad);
    double thick_vis = this->thick * fabs(sin(z_rad));

    x = ((M_PI * 0.0 < z_rad) && (z_rad < M_PI * 1.0)) ? x : -x;

    bool b_ascend = 
        ((M_PI * 0.0 < z_rad) && (z_rad < M_PI * 0.5)) ||
        ((M_PI * 1.0 < z_rad) && (z_rad < M_PI * 1.5));

    bool b_x_on_edge = b_ascend ? 
        ((edge_xdist - thick_vis / 2) < x) && (x < (edge_xdist + thick_vis / 2)) :
        (x < (edge_xdist + thick_vis / 2)) && ((edge_xdist - thick_vis / 2) < x);
    return b_x_on_edge;
}

bool Ring::is_rim_side(double x, double y, double z, double z_rad)
{
    double thick_vis = this->thick * fabs(sin(z_rad));
    bool b_ascend = 
        ((M_PI * 0.0 < z_rad) && (z_rad < M_PI * 0.5)) ||
        ((M_PI * 1.0 < z_rad) && (z_rad < M_PI * 1.5));
    b_ascend ? (x += thick_vis / 2) : (x -= thick_vis / 2);

    double radius = sqrt((pow(x, 2) / fabs(cos(z_rad))) + pow(z, 2));
    bool b_incl = radius < this->r_out;
    bool b_excl = radius < this->r_inner;
    return b_incl && !b_excl;
}

