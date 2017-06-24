//
// PROJECT:         Aspia Remote Desktop
// FILE:            ui/base/comboboxex.cc
// LICENSE:         See top-level directory
// PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
//

#include "ui/base/comboboxex.h"

namespace aspia {

UiComboBoxEx::UiComboBoxEx(HWND hwnd)
{
    Attach(hwnd);
}

int UiComboBoxEx::InsertItem(const std::wstring& text,
                             int item_index,
                             int image_index,
                             int indent,
                             LPARAM lparam)
{
    COMBOBOXEXITEMW item;

    memset(&item, 0, sizeof(item));

    item.mask           = CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_INDENT |
                              CBEIF_LPARAM | CBEIF_TEXT;
    item.pszText        = const_cast<LPWSTR>(text.c_str());
    item.iItem          = item_index;
    item.iImage         = image_index;
    item.iSelectedImage = image_index;
    item.iIndent        = indent;
    item.lParam         = lparam;

    return SendMessageW(hwnd(), CBEM_INSERTITEM, 0, reinterpret_cast<LPARAM>(&item));
}

int UiComboBoxEx::AddItem(const std::wstring& text,
                          int image_index,
                          int indent,
                          LPARAM lparam)
{
    return InsertItem(text, -1, image_index, indent, lparam);
}

int UiComboBoxEx::DeleteItem(int item_index)
{
    return SendMessageW(hwnd(), CBEM_DELETEITEM, item_index, 0);
}

int UiComboBoxEx::DeleteItemWithData(LPARAM lparam)
{
    int item_index = GetItemWithData(lparam);

    if (item_index != CB_ERR)
        return DeleteItem(item_index);

    return CB_ERR;
}

void UiComboBoxEx::DeleteAllItems()
{
    SendMessageW(hwnd(), CB_RESETCONTENT, 0, 0);
}

LPARAM UiComboBoxEx::GetItemData(int item_index)
{
    COMBOBOXEXITEMW item;
    memset(&item, 0, sizeof(item));

    item.mask  = CBEIF_LPARAM;
    item.iItem = item_index;

    if (SendMessageW(hwnd(), CBEM_GETITEM, 0, reinterpret_cast<LPARAM>(&item)) != 0)
    {
        return item.lParam;
    }

    return -1;
}

LRESULT UiComboBoxEx::SetItemData(int item_index, LPARAM lparam)
{
    COMBOBOXEXITEMW item;

    memset(&item, 0, sizeof(item));

    item.mask   = CBEIF_LPARAM;
    item.iItem  = item_index;
    item.lParam = lparam;

    return SendMessageW(hwnd(), CBEM_SETITEM, 0, reinterpret_cast<LPARAM>(&item));
}

std::wstring UiComboBoxEx::GetItemText(int item_index)
{
    COMBOBOXEXITEMW item;
    memset(&item, 0, sizeof(item));

    item.mask = CBEIF_TEXT;
    item.iItem = item_index;

    wchar_t buffer[256] = { 0 };

    item.pszText = buffer;
    item.cchTextMax = _countof(buffer);

    if (SendMessageW(hwnd(), CBEM_GETITEM, 0, reinterpret_cast<LPARAM>(&item)) != 0)
        return buffer;

    return std::wstring();
}

int UiComboBoxEx::GetItemImage(int item_index)
{
    COMBOBOXEXITEMW item;
    memset(&item, 0, sizeof(item));

    item.mask = CBEIF_IMAGE;
    item.iItem = item_index;

    if (SendMessageW(hwnd(), CBEM_GETITEM, 0, reinterpret_cast<LPARAM>(&item)) != 0)
    {
        return item.iImage;
    }

    return -1;
}

LRESULT UiComboBoxEx::SetItemText(int item_index, const std::wstring& text)
{
    COMBOBOXEXITEMW item;
    memset(&item, 0, sizeof(item));

    item.mask    = CBEIF_TEXT;
    item.iItem   = item_index;
    item.pszText = const_cast<LPWSTR>(text.c_str());

    return SendMessageW(hwnd(), CBEM_SETITEM, 0, reinterpret_cast<LPARAM>(&item));
}

HIMAGELIST UiComboBoxEx::SetImageList(HIMAGELIST imagelist)
{
    return reinterpret_cast<HIMAGELIST>(SendMessageW(hwnd(),
                                                     CBEM_SETIMAGELIST,
                                                     0,
                                                     reinterpret_cast<LPARAM>(imagelist)));
}

HIMAGELIST UiComboBoxEx::GetImageList()
{
    return reinterpret_cast<HIMAGELIST>(SendMessageW(hwnd(), CBEM_GETIMAGELIST, 0, 0));
}

int UiComboBoxEx::GetItemCount()
{
    return SendMessageW(hwnd(), CB_GETCOUNT, 0, 0);
}

int UiComboBoxEx::GetSelectedItem()
{
    return SendMessageW(hwnd(), CB_GETCURSEL, 0, 0);
}

int UiComboBoxEx::GetItemWithData(LPARAM lparam)
{
    int count = GetItemCount();

    for (int index = 0; index < count; ++index)
    {
        if (GetItemData(index) == lparam)
            return index;
    }

    return CB_ERR;
}

void UiComboBoxEx::SelectItem(int item_index)
{
    SendMessageW(hwnd(), CB_SETCURSEL, item_index, 0);
}

void UiComboBoxEx::SelectItemData(LPARAM lparam)
{
    int item_index = GetItemWithData(lparam);

    if (item_index != CB_ERR)
        SelectItem(item_index);
}

} // namespace aspia