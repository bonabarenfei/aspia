//
// PROJECT:         Aspia
// FILE:            codec/cursor_decoder.cc
// LICENSE:         GNU Lesser General Public License 2.1
// PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
//

#include "codec/cursor_decoder.h"

#include <QDebug>

namespace aspia {

bool CursorDecoder::DecompressCursor(const proto::desktop::CursorShape& cursor_shape,
                                     quint8* image)
{
    const quint8* src = reinterpret_cast<const quint8*>(cursor_shape.data().data());
    const size_t src_size = cursor_shape.data().size();
    const size_t row_size = cursor_shape.width() * sizeof(quint32);

    // Consume all the data in the message.
    bool decompress_again = true;
    size_t used = 0;

    int row_y = 0;
    size_t row_pos = 0;

    while (decompress_again && used < src_size)
    {
        if (row_y > cursor_shape.height() - 1)
        {
            qWarning("Too much data is received for the given rectangle");
            return false;
        }

        size_t written = 0;
        size_t consumed = 0;

        decompress_again = decompressor_.process(src + used,
                                                 src_size - used,
                                                 image + row_pos,
                                                 row_size - row_pos,
                                                 &consumed,
                                                 &written);
        used += consumed;
        row_pos += written;

        if (row_pos == row_size)
        {
            ++row_y;
            row_pos = 0;
            image += row_size;
        }
    }

    decompressor_.reset();
    return true;
}

std::shared_ptr<MouseCursor> CursorDecoder::Decode(const proto::desktop::CursorShape& cursor_shape)
{
    size_t cache_index;

    if (cursor_shape.flags() & proto::desktop::CursorShape::CACHE)
    {
        // Bits 0-4 contain the cursor position in the cache.
        cache_index = cursor_shape.flags() & 0x1F;
    }
    else
    {
        QSize size(cursor_shape.width(), cursor_shape.height());

        if (size.width()  <= 0 || size.width()  > (std::numeric_limits<qint16>::max() / 2) ||
            size.height() <= 0 || size.height() > (std::numeric_limits<qint16>::max() / 2))
        {
            qWarning() << "Cursor dimensions are out of bounds for SetCursor: "
                       << size.width() << "x" << size.height();
            return nullptr;
        }

        size_t image_size = size.width() * size.height() * sizeof(quint32);
        std::unique_ptr<quint8[]> image = std::make_unique<quint8[]>(image_size);

        if (!DecompressCursor(cursor_shape, image.get()))
            return nullptr;

        std::unique_ptr<MouseCursor> mouse_cursor =
            MouseCursor::create(std::move(image),
                                size,
                                QPoint(cursor_shape.hotspot_x(), cursor_shape.hotspot_y()));

        if (cursor_shape.flags() & proto::desktop::CursorShape::RESET_CACHE)
        {
            size_t cache_size = cursor_shape.flags() & 0x1F;

            if (!MouseCursorCache::isValidCacheSize(cache_size))
                return nullptr;

            cache_ = std::make_unique<MouseCursorCache>(cache_size);
        }

        if (!cache_)
        {
            qWarning("Host did not send cache reset command");
            return nullptr;
        }

        cache_index = cache_->add(std::move(mouse_cursor));
    }

    return cache_->Get(cache_index);
}

} // namespace aspia
