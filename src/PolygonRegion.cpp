/***************************************************************************
 *
 * Project:  OpenCPN Weather Routing plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2016 by Sean D'Epagnier                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <wx/wx.h>

#include "PolygonRegion.h"

#if !defined(snprintf) && defined(_MSC_VER)
#define snprintf _snprintf
#endif

Contour::Contour(const std::list<Point> &in_points)
{
    n = in_points.size();
    points = new float[2*n];
    int i = 0;
    for(std::list<Point>::const_iterator it = in_points.begin();
        it != in_points.end(); it++) {
        points[i++] = it->x;
        points[i++] = it->y;
    }
}

void Contour::Init(const float *p, int c)
{
    n = c;
    points = new float[2*n];
    memcpy(points, p, n * 2 * sizeof *points);
}

bool Contour::CCW()
{
    float total = 0;
    for(int i=0; i<2*n; i+=2) {
        int pn = i < 2*(n-1) ? i + 2 : 0;
        total += (points[pn+0] - points[i+0]) * (points[pn+1] + points[i+1]);
    }
    return total < 0;
}

void Contour::Reverse()
{
    for(int i=0; i<n-1; i+=2) {
        int j = 2*(n-1) - i;
        float x = points[i], y = points[i+1];
        points[i] = points[j], points[i+1] = points[j+1];
        points[j] = x, points[j+1] = y;
    }
}

void Contour::Simplify(float epsilon)
{
    int p = 0;
    float lx = points[2*(n-1)], ly = points[2*(n-1)+1];
    float x = points[0], y = points[1];
    for(int i=0; i<2*n; i+=2) {
        int j = i + 2 < 2*n ? i + 2 : 0;
        float nx = points[j], ny = points[j+1];

        float x0 = lx - x, y0 = ly - y;
        float x1 = nx - x, y1 = ny - y;
        float c = x0 * y1 - x1 * y0;
        if(fabsf(c) >= epsilon) {
            points[p] = x;
            points[p+1] = y;
            lx = x, ly = y;
            p+=2;
        }
        
        x = nx, y = ny;
    }
    n = p/2;
}

/* for convex or concave polygons but not self-intersecting */
PolygonRegion::PolygonRegion(int n, float *points)
{
    Contour contour(points, n);
    if(!contour.CCW())
        contour.Reverse();
    contours.push_back(contour);
    InitMem();
}

struct IncompleteContour
{
    IncompleteContour(Segment &segment) {
        points.push_back(segment.p[0]);
        points.push_back(segment.p[1]);
    }
    std::list<Point> points;
};

PolygonRegion::PolygonRegion(std::list<Segment> &segments)
{
#if 0
    for(std::list<Segment>::iterator it = segments.begin();
        it != segments.end(); it++) {
        printf("%f %f\n%f %f\n\n", it->p[0].x, it->p[0].y, it->p[1].x, it->p[1].y);
    }
#endif
    
    std::list<IncompleteContour*> icontours;
fullreset:
    while(!segments.empty()) {
        Segment a = segments.front();
        segments.pop_front();
        IncompleteContour *ic = new IncompleteContour(a);

    reset:
        for(std::list<IncompleteContour*>::iterator it = icontours.begin();
            it != icontours.end(); it++) {
            if((*it)->points.front() == ic->points.back()) {
                if((*it)->points.back() == ic->points.front()) {
                    // completed contour
                    (*it)->points.pop_front();
                    (*it)->points.pop_back();
                    ic->points.splice(ic->points.end(), (*it)->points);
                    contours.push_back(Contour(ic->points));
                    delete *it;
                    delete ic;
                    icontours.erase(it);
                    goto fullreset;
                }
                (*it)->points.pop_front();
                ic->points.splice(ic->points.end(), (*it)->points);
                delete *it;
                icontours.erase(it);
                goto reset;
            } else if((*it)->points.back() == ic->points.front()) {
                ic->points.pop_front();
                (*it)->points.splice((*it)->points.end(), ic->points);
                delete ic;
                ic = *it;
                icontours.erase(it);
                goto reset;
            }
        }
        icontours.push_back(ic);
    }

    if(icontours.size())
        printf("PolygonRegion: incomplete contours discarded\n");

    for(std::list<IncompleteContour*>::iterator it = icontours.begin();
        it != icontours.end(); it++)
#if 0
        if(it->points.front() == it->points.back()) {
            // completed contour
            it->points.pop_front();
            contours.push_back(Contour(it->points));
        } else
#endif
        {
            for(std::list<Point>::iterator it2 = (*it)->points.begin();
                it2 != (*it)->points.end(); it2++)
                printf("%f %f\n", it2->x, it2->y);
            printf("\n");
        }

    InitMem();

//    PolygonRegion empty;
//    Union(empty); // merge overlapping contours
}

std::list<std::string> Split(const std::string str, char c)
{
    std::list<std::string> l;
    unsigned int lpos = 0;
    while(lpos < str.size()) {
        unsigned int pos = str.find(c, lpos);
        l.push_back(std::string(str, lpos, pos - lpos));
        lpos = pos + 1;
    }
    return l;
}

PolygonRegion::PolygonRegion(const std::string &str)
{
    std::list<std::string> scontours = Split(str, ';');
    for(std::list<std::string>::iterator it = scontours.begin();
        it != scontours.end(); it++) {
        std::list<std::string> coords = Split(*it, ',');
        int n = coords.size(), i = 0;
        float *points = new float[n];
        for(std::list<std::string>::iterator it2 = coords.begin();
            it2 != coords.end(); it2++)
            points[i++] = strtod(it2->c_str(), 0);

        contours.push_back(Contour(points, n/2));
        delete [] points;
    }
    
}

void PolygonRegion::Print()
{
    for(std::list<Contour>::iterator it = contours.begin();
        it != contours.end(); it++) {
        for(int i=0; i<it->n; i++)
            printf("%f %f\n", it->points[2*i+0], it->points[2*i+1]);
        printf("\n");
    }
}

std::string PolygonRegion::toString()
{
    std::string str;
    for(std::list<Contour>::iterator it = contours.begin();
        it != contours.end(); it++) {
        for(int i = 0; i<it->n; i++) {
            float xc = it->points[2*i+0], yc = it->points[2*i+1];
            char coords[100];
            snprintf(coords, sizeof coords, "%12f,%12f,", xc, yc);
            str += coords;
        }
        str += ";";
    }
    return str;
}

bool PolygonRegion::Contains(float x, float y)
{
    int total = 0;
    for(std::list<Contour>::iterator it = contours.begin();
        it != contours.end(); it++) {
        unsigned int l = it->n-1;
        float xl = it->points[2*l+0], yl = it->points[2*l+1];
        for(int i = 0; i<it->n; i++) {
            float xc = it->points[2*i+0], yc = it->points[2*i+1];
            
            float x0, x1, y0, y1;
            
            if(xl < xc)
                x0 = xl, x1 = xc, y0 = yl, y1 = yc;
            else
                x0 = xc, x1 = xl, y0 = yc, y1 = yl;
            
            if(x >= x0) {
                if(x == x0) {
                    if(x == x1) {
                        if(y <= y0 && y > y1)
                            total++;
                    } else if(y <= y0)
                        total++;
                } else if(x < x1) { // x > x0
                    if(y <= y0) {
                        if(y <= y1)
                            total++;
                        else {
                            float dx = x - x0, dy = y - y0, sx = x1 - x0, sy = y1 - y0;
                            if(dy * sx >= sy * dx)
                                total++;
                        }
                        
                    } else if(y <= y1) {
                        float dx = x - x0, dy = y - y0, sx = x1 - x0, sy = y1 - y0;
                        if(dy * sx <= sy * dx)
                            total++;
                    }
                }
            }
            
            xl = xc, yl = yc;
        }
    }   
    return total&1;
}

void PolygonRegion::Intersect(PolygonRegion &region)
{
    Put(region, TESS_WINDING_ABS_GEQ_TWO, false);
}

void PolygonRegion::Union(PolygonRegion &region)
{
    Put(region, TESS_WINDING_POSITIVE, false);
}

void PolygonRegion::Subtract(PolygonRegion &region)
{
    Put(region, TESS_WINDING_POSITIVE, true);
}

void PolygonRegion::Simplify(float epsilon)
{
    std::list<Contour>::iterator it = contours.begin();
    while(it != contours.end()) {
        it->Simplify(epsilon);
        if(it->n < 3)
            it = contours.erase(it);
        else
            it++;
    }
}

#if 0
struct MemPool
{
    unsigned char* buf;
    unsigned int bufsize, size;

    struct MemPool *next;
};

static void* poolAlloc( void* userData, unsigned int size )
{
    size = (size+0x7) & ~0x7;

    const int bufsize = 64*1024;
    struct MemPool** pool = (struct MemPool**)userData;
    if(!*pool || (*pool)->size + size >= (*pool)->bufsize) {
        MemPool *new_pool = new MemPool;
        new_pool->bufsize = size > bufsize ? size : bufsize;
        new_pool->buf = new unsigned char [new_pool->bufsize];
        new_pool->next = *pool;
        *pool = new_pool;
    }
    
    unsigned char* ptr = (*pool)->buf + (*pool)->size;
    (*pool)->size += size;
    return ptr;
}

static void FreeMemPool( MemPool* pool )
{
    if(pool) {
        delete [] pool->buf;
        FreeMemPool(pool->next);
    }
}

static void poolFree( void* userData, void* ptr )
{
    if(ptr == userData)
        FreeMemPool((MemPool*)userData);
}
#endif
static void* stdAlloc(void* userData, unsigned int size)
{
//	int* allocated = ( int*)userData;
	TESS_NOTUSED(userData);
//	*allocated += (int)size;
	return malloc(size);
}

static void stdFree(void* userData, void* ptr)
{
	TESS_NOTUSED(userData);
	free(ptr);
}

TESStesselator* PolygonRegion::Tesselate(bool triangles)
{
    TESSalloc ma;

//    ma.memalloc = poolAlloc;
//    ma.memfree = poolFree;
    ma.memalloc = stdAlloc;
    ma.memfree = stdFree;
    ma.extraVertices = 256; // realloc not provided, allow 256 extra vertic

    TESStesselator *tess = tessNewTess(&ma);
    
    PutContours(tess, false);
    int success;
    if(triangles)
        success = tessTesselate(tess, TESS_WINDING_POSITIVE, TESS_POLYGONS, 3, 2, 0);
    else
        success = tessTesselate(tess, TESS_WINDING_POSITIVE, TESS_BOUNDARY_CONTOURS, 0, 0, 0);

    if(!success) {
        tessDeleteTess(tess);
        return NULL;
    }

//    printf("Memory used: %.1f kB\n", allocated/1024.0f);

    return tess;
}

void PolygonRegion::Put( const PolygonRegion& region, int winding_rule, bool reverse)
{
    TESSalloc ma;
//    ma.memalloc = poolAlloc;
//    ma.memfree = poolFree;
//    ma.userData = (void*)&pool;
    ma.memalloc = stdAlloc;
    ma.memfree = stdFree;
    ma.extraVertices = 256; // realloc not provided, allow 256 extra vertic

    TESStesselator *tess = tessNewTess(&ma);

    PutContours(tess, false);
    region.PutContours(tess, reverse);

    tessTesselate(tess, winding_rule, TESS_BOUNDARY_CONTOURS, 0, 0, 0);

    contours.clear();
    
    const float* verts = tessGetVertices(tess);
//    const int* vinds = tessGetVertexIndices(tess);
//    const int nverts = tessGetVertexCount(tess);
    const int* elems = tessGetElements(tess);
    const int nelems = tessGetElementCount(tess);

    for (int i = 0; i < nelems; i++)
    {
        int b = elems[i*2];
        int n = elems[i*2+1];

        contours.push_back(Contour(&verts[b*2], n));
    }

    tessDeleteTess(tess);
}

void PolygonRegion::PutContours(TESStesselator *tess, bool reverse) const
{
    for(std::list<Contour>::const_iterator it = contours.begin();
        it != contours.end(); it++) {
        if(reverse) {
            Contour c = *it;
            c.Reverse();
            tessAddContour(tess, 2, c.points, sizeof(float)*2, it->n);
        } else
            tessAddContour(tess, 2, it->points, sizeof(float)*2, it->n);
    }
}

void PolygonRegion::AllocateMem()
{
    if(mem)
        return;
    memsize = 1024*1024;
    mem = new unsigned char[memsize];
}
