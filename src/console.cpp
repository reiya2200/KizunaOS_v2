#include "main.h"

char console_buf[sizeof(Console)];
Console* console;

Console::Console(PixelWriter &pixel_writer, const PixelColor &fore_color, const PixelColor &back_color)
    : pixel_writer_{pixel_writer}, fore_color_{fore_color}, back_color_{back_color}, strings_{}, cursor_row_{0}, cursor_column_{0} {}

void Console::PrintString(const char *str)
{
    while (*str != '\0')
    {
        if (*str == '\n')
        {
            NewLine();
        }
        else if (*str == '\r')
        {
            cursor_column_ = 0;
        }
        else if (cursor_column_ < kColumns - 1)
        {
            if (*str == '\t')
            {
                for (int s_i = 0; s_i < 4; ++s_i)
                {
                    if (cursor_column_ < kColumns - 1)
                    {
                        WriteChar(8 * cursor_column_, 16 * cursor_row_, ' ', fore_color_);
                        strings_[cursor_row_][cursor_column_] = ' ';
                    }
                    else
                    {
                        NewLine();
                        WriteChar(8 * cursor_column_, 16 * cursor_row_, ' ', fore_color_);
                        strings_[cursor_row_][cursor_column_] = ' ';
                    }
                    ++cursor_column_;
                }
            }
            else
            {
                WriteChar(8 * cursor_column_, 16 * cursor_row_, *str, fore_color_);
                strings_[cursor_row_][cursor_column_] = *str;
                ++cursor_column_;
            }
        }
        else
        {
            NewLine();
            WriteChar(8 * cursor_column_, 16 * cursor_row_, *str, fore_color_);
            strings_[cursor_row_][cursor_column_] = *str;
            ++cursor_column_;
        }
        ++str;
    }
}

void Console::NewLine()
{
    cursor_column_ = 0;
    if (cursor_row_ < kRows - 1)
        ++cursor_row_;
    else
    {
        for (int y = 0; y < 16 * kRows; ++y)
            for (int x = 0; x < 8 * kColumns; ++x)
                pixel_writer_.Write(x, y, back_color_);
        for (int row = 0; row < kRows - 1; ++row)
        {
            memcpy(strings_[row], strings_[row + 1], kColumns + 1);
            WriteString(0, 16 * row, strings_[row], fore_color_);
        }
        memset(strings_[kRows - 1], 0, kColumns + 1);
    }
}