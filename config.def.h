/* Taken from https://github.com/djpohly/dwl/issues/466 */
#define COLOR(hex)    { ((hex >> 24) & 0xFF) / 255.0f, \
    ((hex >> 16) & 0xFF) / 255.0f, \
    ((hex >> 8) & 0xFF) / 255.0f, \
    (hex & 0xFF) / 255.0f }
/* appearance */
static const int sloppyfocus               = 1;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const unsigned int borderpx         = 1;  /* border pixel of windows */
static const float rootcolor[]             = COLOR(0x00000000);
static const float bordercolor[]           = COLOR(0x29394fff);
static const float focuscolor[]            = COLOR(0x3c5372ff);
static const float urgentcolor[]           = COLOR(0x00000000);
/* This conforms to the xdg-protocol. Set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = {0.1f, 0.1f, 0.1f, 1.0f}; /* You can also use glsl colors */
static const char kblayout_file[]          = "/tmp/dwl-kblayout";

/* tagging - TAGCOUNT must be no greater than 31 */
#define TAGCOUNT (9)

/* logging */
static int log_level = WLR_ERROR;

/* Autostart */
static const char *const autostart[] = {
    "waybar", NULL,
    "wpaperd", "-d", NULL,
    // "mpvpaper", "*", "-f", "-o", "loop", "/home/fiend/media/wallpapers/ezgif-1-b087d0d036.gif", NULL,
    "kitty", NULL, // terminal
    "dropbox", NULL, // dropbox cloud
    "eval", "`ssh-agent -s`", NULL,
    "ssh-add", "/home/fiend/.ssh/github_full_access_ed25519", NULL,
    "ssh-add", "/home/fiend/.ssh/nl_srv", NULL,
    NULL // terminate
};

static const char *const env[] = {
    "QT_QPA_PLATFORM", "wayland",
    "XDG_CURRENT_DESKTOP", "wlr",
};

/* NOTE: ALWAYS keep a rule declared even if you don't use rules (e.g leave at least one example) */
static const Rule rules[] = {
    /* app_id             title       tags mask     isfloating   monitor */
    /* examples: */
    { "firefox",                NULL,       1 << 1,       0,           -1 }, /* Start on ONLY tag "2" */
    { "google-chrome-stable",   NULL,       1 << 1,       0,           -1 }, /* Start on ONLY tag "2" */
    { "telegram-desktop",       NULL,       1 << 2,       0,           -1 }, /* Start on ONLY tag "3" */
    { "keepassxc",              NULL,       1 << 3,       0,           -1 }, /* Start on ONLY tag "4" */
};

/* layout(s) */
static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },
    { "><>",      NULL },    /* no layout function means floating behavior */
    { "[M]",      monocle },
};

/* monitors */
/* (x=-1, y=-1) is reserved as an "autoconfigure" monitor position indicator
 * WARNING: negative values other than (-1, -1) cause problems with Xwayland clients
 * https://gitlab.freedesktop.org/xorg/xserver/-/issues/899
 */
/* NOTE: ALWAYS add a fallback rule, even if you are completely sure it won't be used */
static const MonitorRule monrules[] = {
    /* name       mfact  nmaster scale layout       rotate/reflect                x    y */
    /* example of a HiDPI laptop monitor:
       { "eDP-1",    0.5f,  1,      2,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
       */
    /* defaults */
    { NULL,       0.55f, 1,      1,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
};

/* keyboard */
static const struct xkb_rule_names xkb_rules = {
    /* can specify fields: rules, model, layout, variant, options */
    /* example:
       .options = "ctrl:nocaps",
       */
    .layout = "us,ru",
    .options = "ctrl:swapcaps,grp:toggle",
};

static const int repeat_rate = 25;
static const int repeat_delay = 600;

/* Trackpad */
static const int tap_to_click            = 1;
static const int tap_and_drag            = 1;
static const int drag_lock               = 1;
static const int natural_scrolling       = 0;
static const int disable_while_typing    = 1;
static const int left_handed             = 0;
static const int middle_button_emulation = 0;
/* You can choose between:
   LIBINPUT_CONFIG_SCROLL_NO_SCROLL
   LIBINPUT_CONFIG_SCROLL_2FG
   LIBINPUT_CONFIG_SCROLL_EDGE
   LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN
   */
static const enum libinput_config_scroll_method scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;

/* You can choose between:
   LIBINPUT_CONFIG_CLICK_METHOD_NONE
   LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS
   LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER
   */
static const enum libinput_config_click_method click_method = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;

/* You can choose between:
   LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
   LIBINPUT_CONFIG_SEND_EVENTS_DISABLED
   LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE
   */
static const uint32_t send_events_mode = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;

/* You can choose between:
   LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT
   LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE
   */
static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
static const double accel_speed = 0.0;

/* You can choose between:
   LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
   LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
   */
static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

/* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
#define MODKEY WLR_MODIFIER_ALT

#define TAGKEYS(KEY,SKEY,TAG) \
{ MODKEY,                    KEY,            view,            {.ui = 1 << TAG} }, \
{ MODKEY|WLR_MODIFIER_CTRL,  KEY,            toggleview,      {.ui = 1 << TAG} }, \
{ MODKEY|WLR_MODIFIER_SHIFT, SKEY,           tag,             {.ui = 1 << TAG} }, \
{ MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT,SKEY,toggletag, {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]    = { "kitty", NULL };
static const char *menucmd[]    = { "bemenu-run",
                                    "-C", "-T", "--single-instance",
                                    "-l", "10", "--scrollbar", "autohide",
                                    "-c", "-I", "--fixed-height", "-R", "10", "-M", "400",
                                    "-w", "--binding", "vim", "--vim-esc-exits",
                                    "-p", "run",
                                    "--fb", "#212e3f",
                                    "--ff", "#d6d6d7",
                                    "--nb", "#212e3f",
                                    "--nf", "#d6d6d7",
                                    "--tb", "#212e3f",
                                    "--hb", "#212e3f",
                                    "--tf", "#c94f6d",
                                    "--hf", "#86abdc",
                                    "--af", "#d6d6d7",
                                    "--ab", "#212e3f",
                                    "--scb", "#212e3f",
                                    "--scf", "#3c5372",
                                    NULL };
static const char *browsercmd[] = { "firefox", NULL };

static const Arg screenshot = SHCMD("grim -l 1 -g \"$(slurp)\" - | tee \"/home/fiend/media/screenshots/$(date +'%F_%T').png\" | wl-copy");
static const Arg colorpick = SHCMD("grim -g \"$(slurp -p)\" -t ppm - | convert - -format '%[pixel:p{0,0}]' txt:- | wl-copy");
static const Arg volumeup   = SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +10%");
static const Arg volumedown = SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -10%");

static const Key keys[] = {
    /* Note that Shift changes certain key codes: c -> C, 2 -> at, etc. */
    /* modifier                  key                 function        argument */
    { MODKEY,                    XKB_KEY_p,             spawn,              {.v = menucmd} },
    { MODKEY,                    XKB_KEY_q,             spawn,              {.v = termcmd} },
    { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_F,             spawn,              {.v = browsercmd} },
    { MODKEY,                    XKB_KEY_b,             togglebar,          {0} },
    { MODKEY,                    XKB_KEY_z,             spawn,              volumedown },
    { MODKEY,                    XKB_KEY_x,             spawn,              volumeup },
    { MODKEY,                    XKB_KEY_s,             spawn,              screenshot },
    { MODKEY,                    XKB_KEY_c,             spawn,              colorpick },
    { MODKEY,                    XKB_KEY_j,             focusstack,         {.i = +1} },
    { MODKEY,                    XKB_KEY_k,             focusstack,         {.i = -1} },
    { MODKEY,                    XKB_KEY_h,             setmfact,           {.f = -0.05f} },
    { MODKEY,                    XKB_KEY_l,             setmfact,           {.f = +0.05f} },
    { MODKEY,                    XKB_KEY_Return,        zoom,               {0} },
    { MODKEY,                    XKB_KEY_Tab,           view,               {0} },
    { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_C,             killclient,         {0} },
    { MODKEY,                    XKB_KEY_t,             setlayout,          {.v = &layouts[0]} },
    { MODKEY,                    XKB_KEY_f,             setlayout,          {.v = &layouts[1]} },
    { MODKEY,                    XKB_KEY_m,             setlayout,          {.v = &layouts[2]} },
    { MODKEY,                    XKB_KEY_space,         setlayout,          {0} },
    { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_space,         togglefloating,     {0} },
    { MODKEY,                    XKB_KEY_e,             togglefullscreen,   {0} },
    { MODKEY,                    XKB_KEY_0,             view,               {.ui = ~0} },
    { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_parenright,    tag,                {.ui = ~0} },
    { MODKEY,                    XKB_KEY_bracketleft,   focusmon,           {.i = WLR_DIRECTION_LEFT} },
    { MODKEY,                    XKB_KEY_bracketright,  focusmon,           {.i = WLR_DIRECTION_RIGHT} },
    { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_braceleft,     tagmon,             {.i = WLR_DIRECTION_LEFT} },
    { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_braceright,    tagmon,             {.i = WLR_DIRECTION_RIGHT} },
    TAGKEYS(                     XKB_KEY_1,             XKB_KEY_exclam,         0),
    TAGKEYS(                     XKB_KEY_2,             XKB_KEY_at,             1),
    TAGKEYS(                     XKB_KEY_3,             XKB_KEY_numbersign,     2),
    TAGKEYS(                     XKB_KEY_4,             XKB_KEY_dollar,         3),
    TAGKEYS(                     XKB_KEY_5,             XKB_KEY_percent,        4),
    TAGKEYS(                     XKB_KEY_6,             XKB_KEY_asciicircum,    5),
    TAGKEYS(                     XKB_KEY_7,             XKB_KEY_ampersand,      6),
    TAGKEYS(                     XKB_KEY_8,             XKB_KEY_asterisk,       7),
    TAGKEYS(                     XKB_KEY_9,             XKB_KEY_parenleft,      8),
    { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Q,             quit,               {0} },

    /* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
    { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_Terminate_Server, quit, {0} },
    /* Ctrl-Alt-Fx is used to switch to another VT, if you don't know what a VT is
     * do not remove them.
     */
#define CHVT(n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_XF86Switch_VT_##n, chvt, {.ui = (n)} }
    CHVT(1), CHVT(2), CHVT(3), CHVT(4), CHVT(5), CHVT(6),
    CHVT(7), CHVT(8), CHVT(9), CHVT(10), CHVT(11), CHVT(12),
};

static const Button buttons[] = {
    { MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove} },
    { MODKEY, BTN_MIDDLE, togglefloating, {0} },
    { MODKEY, BTN_RIGHT,  moveresize,     {.ui = CurResize} },
};
