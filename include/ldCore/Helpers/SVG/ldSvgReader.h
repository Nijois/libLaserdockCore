/**
    libLaserdockCore
    Copyright(c) 2018 Wicked Lasers

    This file is part of libLaserdockCore.

    libLaserdockCore is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libLaserdockCore is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libLaserdockCore.  If not, see <https://www.gnu.org/licenses/>.
**/

// ldSvgReader.h
// Created by Eric Brugère on 10/12/16.
// Copyright (c) 2016 Wicked Lasers. All rights reserved.

#ifndef ldSvgReader_H
#define ldSvgReader_H

#include <vector>

#include "ldCore/Helpers/BezierCurve/ldBezierCurveObject.h"
#include "ldCore/Helpers/Maths/ldMaths.h"

class LDCORESHARED_EXPORT ldSvgReader
{
public:
    enum class Type {
        // no resize, no data change -> developper mode: be sure of what you are doing
        Dev,
        // maximize to [-1,1]x[-1,1] regarding min and max values
        Maximize,
        // keep the svg size as the space to resize to [-1,1]x[-1,1].
        // auto re-align if portrait/landscape
        // If a point is outside of the svg frame, ldSvgReader::Type::Maximize mode is auto-activated
        SvgFrame
    };

    static int totalPoints(const ldBezierPaths &bezierPaths);
    static int totalPoints(const ld3dBezierCurves &bezierCurves);
    static ldBezierCurveObject loadSvg(QString qtfilename, Type type = Type::Maximize, float snapDistance = -1, bool isTranslate = true,
                                       float fixedMinX = -1.f, float fixedMaxX = -1.f, float fixedMinY = -1.f, float fixedMaxY = -1.f);
    static ldBezierPathsSequence loadSvgSequence(const QString &dirPath, Type p_type = Type::Maximize, float snapDistance = -1, const QString &filePrefix = "", int masksize = -1);
    static ldBezierPathsSequence loadSvgSequenceFixedScale(float fixedMinX, float fixedMaxX, float fixedMinY, float fixedMaxY, int size, int masksize, char const* qtbasefilename, Type p_type = Type::Maximize, float snapDistance = -1);
    static ldBezierPathsSequence loadFromJSArrayText(const char* text);
    static ldRect svgDim(const ldBezierPaths &bezierPaths);
    static ldRect3 svgDim(const ld3dBezierCurves &bezierCurves);
    static ldRect svgDimSequence(const ldBezierPathsSequence &bezierCurves);
    static ldRect svgDimByInterpolation(const ldBezierPaths &bezierPaths, int p_interpolate = 100);
    static ldRect3 svgDimByInterpolation(const ld3dBezierCurves &bezierCurves, int p_interpolate = 100);

private:
    static void debugStatCurves(ldBezierPaths bezierPaths);
    static ldBezierPaths snapCurves(const ldBezierPaths &bezierPaths, float snapDistance);
};

#endif // ldSvgReader_H

