/* See LICENSE file for copyright and license details. */

// Needed for media keys
#include "X11/XF86keysym.h"

/* appearance */
static const char *fonts[] = { "DejaVuSansMono:size=10" };
static const char dmenufont[]       = "DejaVuSansMono:size=10";
static const char normbordercolor[] = "#928374";
static const char normbgcolor[]     = "#282828";
static const char normfgcolor[]     = "#ebdbb2";
static const char selbordercolor[]  = "#928374";
static const char selbgcolor[]      = "#282828";
static const char selfgcolor[]      = "#ebdbb2";
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */

/* tagging */
static const char *tags[] = { "DWM" };

static void togglesound(const Arg *arg);
static void alternatescreen(const Arg *arg);
static void changeredshift(const Arg *arg);

/* key definitions */
#define MODKEY Mod1Mask
#define WINKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ WINKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ WINKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *term[]  = { "st", NULL };
static const char *slock[]  = { "slock", NULL };
static const char *web[] = { "firefox", NULL };
static const char *vold[] = { "amixer", "-q", "set", "Master", "5%-", "unmute", NULL };
static const char *volu[] = { "amixer", "-q", "set", "Master", "5%+", "unmute", NULL };
static const char *bklu[] = { "xbacklight", "-steps", "1", "-time", "0", "-inc", "5", NULL };
static const char *bkld[] = { "xbacklight", "-steps", "1", "-time", "0", "-dec", "5", NULL };
static const char *shtdwn[] = { "sudo", "shutdown", "-h", "now", NULL };

/*
 * Rules for the keyboard shortcuts:
 * - use the WINKEY for the shortcuts (so that we have no conflicts with cli apps)
 * - use the ShifMask to avoid pressing the shortcut by accident
 */
static Key keys[] = {
	/* modifier                     key                             function            argument */
	{ WINKEY,                       XK_p,                           spawn,              {.v = dmenucmd } },
	{ WINKEY,                       XK_l,                           spawn,              {.v = slock } },
	{ WINKEY,                       XK_w,                           spawn,              {.v = web } },
	{ WINKEY,                       XK_t,                           spawn,              {.v = term } },
	{ WINKEY,                       XK_j,                           focusstack,         {.i = +1 } },
	{ WINKEY,                       XK_k,                           focusstack,         {.i = -1 } },
	{ WINKEY,                       XK_Tab,                         zoomswap,           {0} },
	{ WINKEY,                       XK_q,                           togglebar,          {0} },
	{ WINKEY,	                    XK_s,                           alternatescreen,    {0} },
	{ WINKEY,	                    XK_r,                           changeredshift,     {0} },
	{ WINKEY,	                    XK_b,                           spawn,              {.v = bklu } },
	{ WINKEY|ShiftMask,	            XK_b,                           spawn,              {.v = bkld } },
	{ WINKEY|ShiftMask,             XK_q,                           quit,               {0} },
	{ WINKEY|ShiftMask,             XK_c,                           killclient,         {0} },
	{ WINKEY|ShiftMask,             XK_k,                           spawn,              {.v = shtdwn } },

    // thinkpad media keys (can be found with `xmodmap -pke`)
	{ 0,	                        XF86XK_MonBrightnessDown,       spawn,              {.v = bkld } },
	{ 0,	                        XF86XK_MonBrightnessUp,         spawn,              {.v = bklu } },
	{ 0,	                        XF86XK_AudioMute,               togglesound,        {0} },
	{ 0,	                        XF86XK_AudioLowerVolume,        spawn,              {.v = vold } },
	{ 0,	                        XF86XK_AudioRaiseVolume,        spawn,              {.v = volu } },
};

// funtion that alternates between LVDS1 (laptop screen) and VGA1 (VGA output)
static int screen_id = 0;
void alternatescreen(const Arg *arg) {
    if (screen_id % 2 == 0) {
        system("xrandr --output LVDS1 --auto && xrandr --output VGA1 --off");
    } else {
        system("xrandr --output VGA1 --auto && xrandr --output LVDS1 --off");
    }
    screen_id++;
}

// function that controls the redshift status
static int redShift = 0;
void changeredshift(const Arg *arg) {
    redShift = (redShift + 1) % 3;
    switch(redShift) {
        case 2:
            system("redshift -O 3200");
            break;
        case 1:
            system("redshift -O 3800");
            break;
        case 0:
        default:
            system("redshift -x");
    }
}

// function that controls if the sound is muted
static int isMute = 1;
void togglesound(const Arg *arg) {
    if(isMute == 1) {
        system("amixer set Master unmute");
        system("amixer set Speaker unmute");
        system("amixer set Headphone unmute");
        isMute = 0;
    } else {
        system("amixer set Master mute");
        isMute = 1;
    }
}
