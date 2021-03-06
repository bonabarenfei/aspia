//
// Aspia Project
// Copyright (C) 2019 Dmitry Chapyshev <dmitry@aspia.ru>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
//

#ifndef DESKTOP__DIFF_BLOCK_16BPP_C_H
#define DESKTOP__DIFF_BLOCK_16BPP_C_H

#include <cstdint>

namespace desktop {

uint8_t diffFullBlock_16bpp_32x32_C(
    const uint8_t* image1, const uint8_t* image2, int bytes_per_row);

uint8_t diffFullBlock_16bpp_16x16_C(
    const uint8_t* image1, const uint8_t* image2, int bytes_per_row);

uint8_t diffFullBlock_16bpp_8x8_C(
    const uint8_t* image1, const uint8_t* image2, int bytes_per_row);

} // namespace desktop

#endif // DESKTOP__DIFF_BLOCK_16BPP_C_H
