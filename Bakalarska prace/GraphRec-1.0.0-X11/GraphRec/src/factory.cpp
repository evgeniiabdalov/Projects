/* GraphRec
 * Copyright (c) 2009 Petr Koupy <petr.koupy@gmail.com>
 *
 * GNU General Public License Usage
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtCore/QStringList>
#include <QtCore/QString>

#include "factory.h"
#include "servant.h"
#include "parsers/multirobotparser.h"
#include "parsers/graphrecparser.h"
#include "validators/multirobotvalidator.h"
#include "validators/pebblevalidator.h"
#include "layouters/fruchtermanreingoldlayouter.h"
#include "layouters/kamadakawailayouter.h"
#include "recorders/rasterimagerecorder.h"
#include "recorders/svgimagerecorder.h"
#include "recorders/ffmpegvideorecorder.h"
#include "savers/multirobotsaver.h"
#include "savers/graphrecsaver.h"

Factory::Factory()
{
}

QStringList Factory::GetServantNames(Factory::ServantType type)
{
    QStringList names;
    // Note: Names of servants of the same type must be unique.
    switch (type)
    {
        case Factory::sParser:
            names.append(MultirobotParser::GetName());
            names.append(GraphRecParser::GetName());
            break;
        case Factory::sValidator:
            names.append(MultirobotValidator::GetName());
            names.append(PebbleValidator::GetName());
            break;
        case Factory::sLayouter:
            names.append(FruchtermanReingoldLayouter::GetName());
            names.append(KamadaKawaiLayouter::GetName());
            break;
        case Factory::sRecorder:
            names.append(RasterImageRecorder::GetName());
            names.append(SVGImageRecorder::GetName());
            names.append(FFmpegVideoRecorder::GetName());
            break;
        case Factory::sSaver:
            names.append(MultirobotSaver::GetName());
            names.append(GraphRecSaver::GetName());
            break;
        default:
            break;
    }
    return names;
}

Servant* Factory::CreateServant(Factory::ServantType type, const QString& name)
{
    switch (type)
    {
        case Factory::sParser:
            if (name == MultirobotParser::GetName())
                return new MultirobotParser();
            else if (name == GraphRecParser::GetName())
                return new GraphRecParser();
            else
                return NULL;
        case Factory::sValidator:
            if (name == MultirobotValidator::GetName())
                return new MultirobotValidator();
            else if (name == PebbleValidator::GetName())
                return new PebbleValidator();
            else
                return NULL;
            break;
        case Factory::sLayouter:
            if (name == FruchtermanReingoldLayouter::GetName())
                return new FruchtermanReingoldLayouter();
            else if (name == KamadaKawaiLayouter::GetName())
                return new KamadaKawaiLayouter();
            else
                return NULL;
            break;
        case Factory::sRecorder:
            if (name == RasterImageRecorder::GetName())
                return new RasterImageRecorder();
            else if (name == SVGImageRecorder::GetName())
                return new SVGImageRecorder();
            else if (name == FFmpegVideoRecorder::GetName())
                return new FFmpegVideoRecorder();
            else
                return NULL;
            break;
        case Factory::sSaver:
            if (name == MultirobotSaver::GetName())
                return new MultirobotSaver();
            else if (name == GraphRecSaver::GetName())
                return new GraphRecSaver();
            else
                return NULL;
            break;
        default:
            return NULL;
    }
    return NULL;
}
