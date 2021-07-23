/*
 * Copyright (c) 2019-2020, Ryan Grieb <ryan.m.grieb@gmail.com>
 * Copyright (c) 2020-2021, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/String.h>
#include <LibCore/DateTime.h>
#include <LibGUI/Frame.h>
#include <LibGUI/Widget.h>

namespace GUI {

class Calendar final : public GUI::Frame {
    C_OBJECT(Calendar)

public:
    enum Mode {
        Week,
        Month,
        Year
    };

    enum Format {
        ShortMonthYear,
        LongMonthYear,
        MonthOnly,
        YearOnly
    };

    enum class TopBarView {
        None,
        Year,
        MonthAndYear
    };

    Calendar(Core::DateTime date_time = Core::DateTime::now(), Mode mode = Month);
    virtual ~Calendar() override;

    void set_selected_date(Core::DateTime date_time);
    Core::DateTime selected_date() const { return m_selected_date; }

    void set_view_date(unsigned year, unsigned month)
    {
        m_view_year = year;
        m_view_month = month;
    }
    unsigned view_year() const { return m_view_year; }
    unsigned view_month() const { return m_view_month; }

    String formatted_date(Format format = LongMonthYear);

    Mode mode() const { return m_mode; }
    void set_mode(Mode const&);

    void update_tiles(unsigned year, unsigned month);
    void update_tiles(Core::DateTime const&);

    void set_grid(bool);
    bool has_grid() const { return m_grid; }

    void set_top_bar_display(TopBarView const& view) { m_top_bar_display = view; }
    bool is_showing_year() const { return m_top_bar_display == TopBarView::Year; }
    bool is_showing_month_and_year() const { return m_top_bar_display == TopBarView::MonthAndYear; }

    void set_show_days_of_the_week(bool b) { m_show_days = b; }
    bool is_showing_days_of_the_week() const { return m_show_days; }

    void set_show_day_numbers(bool b) { m_show_day_numbers = b; }
    bool is_showing_day_numbers() const { return m_show_day_numbers; }

    void jump_forwards_one_week();
    void jump_backwards_one_week();

    Gfx::IntSize unadjusted_tile_size() const { return m_unadjusted_tile_size; }
    void set_unadjusted_tile_size(int width, int height)
    {
        m_unadjusted_tile_size.set_width(width);
        m_unadjusted_tile_size.set_height(height);
    }

    Function<void()> on_tile_click;
    Function<void()> on_tile_doubleclick;
    Function<void()> on_month_click;

private:
    virtual void resize_event(GUI::ResizeEvent&) override;
    virtual void paint_event(GUI::PaintEvent&) override;
    virtual void mousemove_event(GUI::MouseEvent&) override;
    virtual void mousedown_event(MouseEvent&) override;
    virtual void mouseup_event(MouseEvent&) override;
    virtual void doubleclick_event(MouseEvent&);
    virtual void leave_event(Core::Event&) override;

    static Vector<Core::DateTime> get_dates_of_current_week(Core::DateTime const& date);

    struct Day {
        String name;
        int width { 0 };
        int height { 16 };
    };
    Vector<Day> m_days;

    struct MonthTile {
        String name;
        Gfx::IntRect rect;
        int width { 0 };
        int height { 0 };
        bool is_hovered { false };
        bool is_being_pressed { false };
    };
    Vector<MonthTile> m_months;

    struct Tile {
        Core::DateTime date_time;
        Gfx::IntRect rect;
        int width { 0 };
        int height { 0 };
        bool is_today { false };
        bool is_selected { false };
        bool is_hovered { false };
        bool is_outside_selected_month { false };
    };
    Vector<Tile> m_tiles[12];

    bool m_grid { true };
    TopBarView m_top_bar_display { TopBarView::MonthAndYear };
    bool m_show_day_numbers { false };
    bool m_show_days { true };
    bool m_show_month_tiles { false };
    int m_currently_pressed_index { -1 };
    unsigned m_view_year;
    unsigned m_view_month;
    Core::DateTime m_selected_date;
    Core::DateTime m_previous_selected_date;
    Core::DateTime m_date_for_week_view;
    Gfx::IntSize m_unadjusted_tile_size;
    Gfx::IntSize m_event_size;
    Gfx::IntSize m_month_size[12];
    Mode m_mode { Month };
};

}
