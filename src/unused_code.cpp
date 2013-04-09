#define EPSILON (2e-10)
#define EPSILON2 (2e-20)
int TestIntersectionGoodButSlow(Position *p1, Position *p2, Position *p3, Position *p4)
{
    double x1 = p1->lon, x2 = p2->lon, x3 = p3->lon, x4 = p4->lon;
    double y1 = p1->lat, y2 = p2->lat, y3 = p3->lat, y4 = p4->lat;

    /* right way around world */
    if(fabs(x1 - x2) > 180) {
        if(x1 < 0)      x1 += 360;
        else if(x2 < 0) x2 += 360;
        /* put other segment in our coords */
        if(x3 < 0)
            x3 += 360, x4 += 360;
    }

    /* right way around for this segment */
    if(fabs(x3 - x4) > 180) {
        if(x3 < 0)      x3 += 360; else if(x3 >= 360) x3 -= 360;
        else if(x4 < 0) x4 += 360; else if(x4 >= 360) x4 -= 360;
        if(x1 < 0)
            x1 += 360, x2 += 360;
    }

    /* quick test to avoid calculations if segments are far apart */
    if((x3 > x1 && x3 > x2 && x4 > x1 && x4 > x2) ||
       (x3 < x1 && x3 < x2 && x4 < x1 && x4 < x2) ||
       (y3 > y1 && y3 > y2 && y4 > y1 && y4 > y2) ||
       (y3 < y1 && y3 < y2 && y4 < y1 && y4 < y2))
        return 0;

    /* error for zero segments */
    if((x1 == x2 && y1 == y2))
        return -2;
    if((x3 == x4 && y3 == y4))
        return 2;

    double dem = (x1*(y4-y3)+x2*(y3-y4)+(x4-x3)*y2+(x3-x4)*y1);

    if(fabs(dem) < EPSILON2) /* parallel or really close to parallel */
        return 2;

    /* find intersection point */
    double x = (x1*(x3*y4-x4*y3+y2*(x4-x3)) + x2*(x4*y3-x3*y4+y1*(x3-x4)))/dem;
    double y = (y1*(x3*y4-x4*y3+x2*(y3-y4)) + y2*(x4*y3-x3*y4+x1*(y4-y3)))/dem;

    double xa, ya, xb, yb, xc, yc, da, db, dc;
    xb = x1 - x , yb = y1 - y , db = xb*xb + yb*yb;
    xc = x2 - x , yc = y2 - y , dc = xc*xc + yc*yc;

    if(db < EPSILON2 || dc < EPSILON2)
        return 2; /* -2 or 2? */

    xa = x1 - x2, ya = y1 - y2, da = xa*xa + ya*ya;

    if(db > da || dc > da)
        return 0;

    xb = x3 - x , yb = y3 - y , db = xb*xb + yb*yb;
    xc = x4 - x , yc = y4 - y , dc = xc*xc + yc*yc;

    if(db < EPSILON2 || dc < EPSILON2)
        return 2;

    xa = x3 - x4, ya = y3 - y4, da = xa*xa + ya*ya;

    if(db > da || dc > da)
        return 0;

    return dem < 0 ? 1 : -1;
}

int TestIntersection(Position *p1, Position *p2, Position *p3, Position *p4)
{
    double x1 = p1->lon, x2 = p2->lon, x3 = p3->lon, x4 = p4->lon;
    double y1 = p1->lat, y2 = p2->lat, y3 = p3->lat, y4 = p4->lat;

    /* right way around world */
    if(fabs(x1 - x2) > 180) {
        if(x1 < 0)      x1 += 360;
        else if(x2 < 0) x2 += 360;
        /* put other segment in our coords */
        if(x3 < 0)
            x3 += 360, x4 += 360;
    }

    /* right way around for this segment */
    if(fabs(x3 - x4) > 180) {
        if(x3 < 0)      x3 += 360; else if(x3 >= 360) x3 -= 360;
        else if(x4 < 0) x4 += 360; else if(x4 >= 360) x4 -= 360;
        if(x1 < 0)
            x1 += 360, x2 += 360;
    }

#if 0
    /* quick test to avoid calculations if segments are far apart */
    if((x3 > x1 && x3 > x2 && x4 > x1 && x4 > x2) ||
       (x3 < x1 && x3 < x2 && x4 < x1 && x4 < x2) ||
       (y3 > y1 && y3 > y2 && y4 > y1 && y4 > y2) ||
       (y3 < y1 && y3 < y2 && y4 < y1 && y4 < y2))
        return 0;
#endif

    double ax = x2 - x1, ay = y2 - y1;
    double bx = x3 - x4, by = y3 - y4;
    double cx = x1 - x3, cy = y1 - y3;

    double denom = ay * bx - ax * by;

#define EPS 2e-10
#define EPS2 2e-4
    if(fabs(denom) < EPS) { /* parallel or really close to parallel */
        if(fabs((y1 + by*ax*x3) - (y3 + ay*bx*x1)) > EPS2)
            return 0; /* different intercepts, no intersection */

        /* we already know from initial test we are overlapping,
           for parallel line segments, there is no way to tell
           which direction the intersection occurs */
        if(!ax && !ay) /* first segment is a zero segment */
            return -2;
        else
            return 2;
    }

    double recip = 1 / denom;
    double na = (by * cx - bx * cy) * recip;
    if(na < -EPS2 || na > 1 + EPS2)
        return 0;

    double nb = (ax * cy - ay * cx) * recip;
    if(nb < -EPS2 || nb > 1 + EPS2)
        return 0;

    /* too close to call.. floating point loses bits with arithmetic so
       in this case we must avoid potential false guesses */
    if(na < EPS2 || na > 1 - EPS2 ||
       nb < EPS2 || nb > 1 - EPS2)
        return 2;

    return denom < 0 ? -1 : 1;
}
