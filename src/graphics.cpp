/*****************************************************************************
 *
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2014 Artem Pavlenko
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/

// mapnik
#include <mapnik/debug.hpp>
#include <mapnik/graphics.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/global.hpp>
#include <mapnik/color.hpp>

// agg
#include "agg_rendering_buffer.h"
#include "agg_pixfmt_rgba.h"
#include "agg_color_rgba.h"

#ifdef HAVE_CAIRO
#include <mapnik/cairo/cairo_context.hpp>
#endif

namespace mapnik
{
image_32::image_32(int width,int height)
   : data_(width,height),
     painted_(false) {}


image_32::image_32(image_32 const& rhs)
   : data_(rhs.data_),
     painted_(rhs.painted_) {}

image_32::image_32(image_data_rgba8 && data)
    : data_(std::move(data)),
      painted_(false) {}

image_32::~image_32() {}

void image_32::composite_pixel(unsigned op, int x,int y, unsigned c, unsigned cover, double opacity)
{
    using color_type = agg::rgba8;
    using value_type = color_type::value_type;
    using order_type = agg::order_rgba;
    using blender_type = agg::comp_op_adaptor_rgba<color_type,order_type>;

    if (checkBounds(x,y))
    {
        unsigned rgba = data_(x,y);
        unsigned ca = (unsigned)(((c >> 24) & 0xff) * opacity);
        unsigned cb = (c >> 16 ) & 0xff;
        unsigned cg = (c >> 8) & 0xff;
        unsigned cr = (c & 0xff);
        blender_type::blend_pix(op, (value_type*)&rgba, cr, cg, cb, ca, cover);
        data_(x,y) = rgba;
    }
}

}
