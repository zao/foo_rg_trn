#include <helpers/foobar2000+atl.h>
#include <helpers/atl-misc.h>
#include <helpers/DarkMode.h>

#include "resource.h"

#include <Rpc.h>

#include <atlbase.h>
#include <atlapp.h>
#include <atldlgs.h>
#include <atlcrack.h>

#include <map>
#include <vector>

typedef cfg_int_t<t_int8> cfg_int8;

namespace guid_playback_order {
// Default
// bfc61179-49ad-4e95-8d60-a22706485505
static const GUID default_guid = { 0xbfc61179, 0x49ad, 0x4e95, { 0x8d, 0x60, 0xa2, 0x27, 0x06, 0x48, 0x55, 0x05 } };
// Repeat (playlist)
// 681cc6ea-60ae-4bf9-913b-bb5f4e864f2a
static const GUID repeat_list_guid = { 0x681cc6ea, 0x60ae, 0x4bf9, { 0x91, 0x3b, 0xbb, 0x5f, 0x4e, 0x86, 0x4f, 0x2a } };
// Repeat (track)
// 4bf4b280-0bb4-4dd0-8e84-37c3209c3da2
static const GUID repeat_track_guid = { 0x4bf4b280,
                                        0x0bb4,
                                        0x4dd0,
                                        { 0x8e, 0x84, 0x37, 0xc3, 0x20, 0x9c, 0x3d, 0xa2 } };
// Random
// 0b03b6e8-1337-4328-871d-a418f1b57a6b
static const GUID random_guid = { 0xb03b6e8, 0x1337, 0x4328, { 0x87, 0x1d, 0xa4, 0x18, 0xf1, 0xb5, 0x7a, 0x6b } };
// Shuffle (track)
// c5cf4a57-8c01-480c-b334-3619645ada8b
static const GUID shuffle_track_guid = { 0xc5cf4a57,
                                         0x8c01,
                                         0x480c,
                                         { 0xb3, 0x34, 0x36, 0x19, 0x64, 0x5a, 0xda, 0x8b } };
// Shuffle (album)
// 499e0b08-c887-48c1-9cca-27377c8bfd30
static const GUID shuffle_album_guid = { 0x499e0b08,
                                         0xc887,
                                         0x48c1,
                                         { 0x9c, 0xca, 0x27, 0x37, 0x7c, 0x8b, 0xfd, 0x30 } };
// Shuffle (directory)
// 83c37600-d725-4727-b53c-bdeffe5f8dc7
static const GUID shuffle_directory_guid = { 0x83c37600,
                                             0xd725,
                                             0x4727,
                                             { 0xb5, 0x3c, 0xbd, 0xef, 0xfe, 0x5f, 0x8d, 0xc7 } };
}

// {9CAB2FC1-87C9-43d5-89F6-2CF3ACD245D1}
static const GUID cfg_mode_default_guid = { 0x9cab2fc1,
                                            0x87c9,
                                            0x43d5,
                                            { 0x89, 0xf6, 0x2c, 0xf3, 0xac, 0xd2, 0x45, 0xd1 } };
static cfg_int8 cfg_mode_default(cfg_mode_default_guid, 0);

// {170C35DE-D789-4e15-B5BA-3C15401BF7C5}
static const GUID cfg_mode_repeat_list_guid = { 0x170c35de,
                                                0xd789,
                                                0x4e15,
                                                { 0xb5, 0xba, 0x3c, 0x15, 0x40, 0x1b, 0xf7, 0xc5 } };
static cfg_int8 cfg_mode_repeat_list(cfg_mode_repeat_list_guid, 0);

// {7F778F14-7FE5-44c3-8494-7867DB959FEB}
static const GUID cfg_mode_repeat_track_guid = { 0x7f778f14,
                                                 0x7fe5,
                                                 0x44c3,
                                                 { 0x84, 0x94, 0x78, 0x67, 0xdb, 0x95, 0x9f, 0xeb } };
static cfg_int8 cfg_mode_repeat_track(cfg_mode_repeat_track_guid, 0);

// {9DFE3B29-D901-48b3-9FB9-8F248C9BAF13}
static const GUID cfg_mode_random_guid = { 0x9dfe3b29,
                                           0xd901,
                                           0x48b3,
                                           { 0x9f, 0xb9, 0x8f, 0x24, 0x8c, 0x9b, 0xaf, 0x13 } };
static cfg_int8 cfg_mode_random(cfg_mode_random_guid, 0);

// {84984D83-EB6C-43ab-BD1B-07D8CE3A232E}
static const GUID cfg_mode_shuffle_album_guid = { 0x84984d83,
                                                  0xeb6c,
                                                  0x43ab,
                                                  { 0xbd, 0x1b, 0x7, 0xd8, 0xce, 0x3a, 0x23, 0x2e } };
static cfg_int8 cfg_mode_shuffle_album(cfg_mode_shuffle_album_guid, 0);

// {FE2393EB-4BC1-4661-92FB-42FF83D6B8D6}
static const GUID cfg_mode_shuffle_directory_guid = { 0xfe2393eb,
                                                      0x4bc1,
                                                      0x4661,
                                                      { 0x92, 0xfb, 0x42, 0xff, 0x83, 0xd6, 0xb8, 0xd6 } };
static cfg_int8 cfg_mode_shuffle_directory(cfg_mode_shuffle_directory_guid, 0);

// {66E4C506-3FC8-4210-BF00-22C0059AC49B}
static const GUID cfg_mode_shuffle_track_guid = { 0x66e4c506,
                                                  0x3fc8,
                                                  0x4210,
                                                  { 0xbf, 0x0, 0x22, 0xc0, 0x5, 0x9a, 0xc4, 0x9b } };
static cfg_int8 cfg_mode_shuffle_track(cfg_mode_shuffle_track_guid, 0);

class playlist_rg_override_callback : public playlist_callback_single_static
{
  public:
    unsigned get_flags() { return flag_on_playback_order_changed; }
    void on_items_added(t_size start,
                        const pfc::list_base_const_t<metadb_handle_ptr>& p_data,
                        const bit_array& p_selection)
    {
    } // inside any of these methods, you can call playlist APIs to get exact info about what happened (but only methods
      // that read playlist state, not those that modify it)
    void on_items_reordered(const t_size* order, t_size count) {
    } // changes selection too; doesnt actually change set of items that are selected or item having focus, just changes
      // their order
    virtual void on_items_removing(const bit_array& p_mask, t_size p_old_count, t_size p_new_count) {
    } // called before actually removing them
    virtual void on_items_removed(const bit_array& p_mask, t_size p_old_count, t_size p_new_count) {}
    virtual void on_items_selection_change(const bit_array& p_affected, const bit_array& p_state) {}
    virtual void on_item_focus_change(t_size p_from, t_size p_to) {
    } // focus may be -1 when no item has focus; reminder: focus may also change on other callbacks
    virtual void on_items_modified(const bit_array& p_mask) {}
    virtual void on_items_modified_fromplayback(const bit_array& p_mask, play_control::t_display_level p_level) {}
    virtual void on_items_replaced(const bit_array& p_mask,
                                   const pfc::list_base_const_t<playlist_callback::t_on_items_replaced_entry>& p_data)
    {
    }
    virtual void on_item_ensure_visible(t_size p_idx) {}

    virtual void on_playlist_switch() {}
    virtual void on_playlist_renamed(const char* p_new_name, t_size p_new_name_len) {}
    virtual void on_playlist_locked(bool p_locked) {}

    virtual void on_default_format_changed() {}
    virtual void on_playback_order_changed(t_size p_new_index);
};

void
playlist_rg_override_callback::on_playback_order_changed(t_size p_new_index)
{
    static_api_ptr_t<playlist_manager> pm;
    static_api_ptr_t<replaygain_manager> rgm;
    // service_ptr_t<playlist_manager> pm;
    // service_ptr_t<replaygain_manager> rgm;
    //	GUID g = pm->playback_order_get_guid(p_new_index);
    t_replaygain_config rgc;
    rgm->get_core_settings(rgc);
    t_int8 mode;
    switch (p_new_index) {
        case 0:
            mode = cfg_mode_default;
            break;
        case 1:
            mode = cfg_mode_repeat_list;
            break;
        case 2:
            mode = cfg_mode_repeat_track;
            break;
        case 3:
            mode = cfg_mode_random;
            break;
        case 4:
            mode = cfg_mode_shuffle_album;
            break;
        case 5:
            mode = cfg_mode_shuffle_directory;
            break;
        case 6:
            mode = cfg_mode_shuffle_track;
            break;
    }
    switch (mode) {
        case 0:
            rgc.m_source_mode = rgc.source_mode_album;
            break;
        case 1:
            rgc.m_source_mode = rgc.source_mode_track;
            break;
        case 2:
            rgc.m_source_mode = rgc.source_mode_none;
            break;
    }
    rgm->set_core_settings(rgc);
}

class rg_override_page
  : public CDialogImpl<rg_override_page>
  , public preferences_page_instance
{
  public:
    rg_override_page(preferences_page_callback::ptr callback);

    enum
    {
        IDD = IDD_RGPAGE
    };

    BEGIN_MSG_MAP_EX(rg_override_page)
    MSG_WM_INITDIALOG(on_init_dialog)
    COMMAND_RANGE_CODE_HANDLER_EX(IDC_DAlbum, IDC_STNone, BN_CLICKED, on_radio_clicked)
    END_MSG_MAP()

    t_uint32 get_state();
    void apply();
    void reset();

  private:
    BOOL on_init_dialog(CWindow, LPARAM);
    void on_radio_clicked(UINT, int id, CWindow control);

    bool has_changed();

    fb2k::CDarkModeHooks dark_hooks;

    enum
    {
        entry_count = 7
    };
    int states[entry_count];
    cfg_int8* cfgs[entry_count];
    preferences_page_callback::ptr const callback;
};

struct rg_override_page_impl : preferences_page_impl<rg_override_page>
{
    virtual const char* get_name() { return "ReplayGain override"; }
    virtual GUID get_guid() { return guid; }
    virtual GUID get_parent_guid() { return preferences_page::guid_playback; }

    static const GUID guid;
};

// {B02E4A78-7780-43ab-9A46-40E5D2743F5B}
const GUID rg_override_page_impl::guid = { 0xb02e4a78,
                                           0x7780,
                                           0x43ab,
                                           { 0x9a, 0x46, 0x40, 0xe5, 0xd2, 0x74, 0x3f, 0x5b } };

INT_PTR CALLBACK
ConfigProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

rg_override_page::rg_override_page(preferences_page_callback::ptr callback)
  : callback(callback)
{
    cfg_int8* cfgs[] = { &cfg_mode_default,       &cfg_mode_repeat_list,       &cfg_mode_repeat_track, &cfg_mode_random,
                         &cfg_mode_shuffle_album, &cfg_mode_shuffle_directory, &cfg_mode_shuffle_track };
    std::copy_n(cfgs, entry_count, this->cfgs);
}

BOOL
rg_override_page::on_init_dialog(CWindow, LPARAM)
{
    dark_hooks.AddDialogWithControls(*this);
    static_api_ptr_t<playlist_manager> pm;

    for (int i = 0; i < entry_count; ++i) {
        SetDlgItemTextW(IDC_D + i, pfc::stringcvt::string_wide_from_utf8(pm->playback_order_get_name(i)));
    }
    reset();
    return TRUE;
}

bool
rg_override_page::has_changed()
{
    bool ret = false;
    for (int i = 0; i < entry_count; ++i)
        ret |= *cfgs[i] != states[i];
    return ret;
}

void
rg_override_page::on_radio_clicked(UINT, int id, CWindow control)
{
    int idx = id - IDC_DAlbum;
    states[idx / 3] = idx % 3;
    if (has_changed())
        callback->on_state_changed();
}

t_uint32
rg_override_page::get_state()
{
    t_uint32 state = preferences_state::resettable | preferences_state::dark_mode_supported;
    if (has_changed())
        state |= preferences_state::changed;
    return state;
}

void
rg_override_page::apply()
{
    for (int i = 0; i < entry_count; ++i)
        *cfgs[i] = states[i];
}

void
rg_override_page::reset()
{
    for (int i = 0; i < entry_count; ++i) {
        DWORD target = IDC_DAlbum + i * 3;
        states[i] = *cfgs[i];
        for (int off = 0; off < 3; ++off)
            CheckDlgButton(target + off, states[i] == off);
    }
}

static play_callback_static_factory_t<playlist_rg_override_callback> g_callback;
static preferences_page_factory_t<rg_override_page_impl> g_page;

DECLARE_COMPONENT_VERSION("ReplayGain override", "0.1.5", "Zao");
VALIDATE_COMPONENT_FILENAME("foo_rg_trn.dll");
