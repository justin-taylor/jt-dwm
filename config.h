/* See LICENSE file for copyright and license details. */

/*
 * Use xmodmap -pke
 */

#include <X11/XF86keysym.h>
#include "bstack.c"
#include "grid.c"
#include "bstackhoriz.c"

/* appearance */
#define BOTTOM_BAR_HEIGHT    13
static const char font[]            = "-*-terminus-medium-r-*-*-16-*-*-*-*-*-*-*";
static const char normbordercolor[] = "#333333";
static const char normbgcolor[]     = "#051121";
static const char normfgcolor[]     = "#FFFFFF";
static const char selbordercolor[]  = "#00FF00";
static const char selbgcolor[]      = "#1793D1";
static const char selfgcolor[]      = "#FFFFFF";
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const Bool showbar           = False;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */
static const Bool viewontag         = True;     /* False means bottom bar */

/* tagging */
static const char *tags[] = { "Term", "Web", "Chat", "Mail" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "ScudCloud",        NULL,       NULL,       1 << 1 | 1,       False,      0 },
	{ "google-chrome",     NULL,       NULL,      1 << 1,       False,      0 },
	{ "Thunderbird",     NULL,       NULL,        1 << 3,       False,      0 },
	{ "gnome-terminal",     NULL,       NULL,     1,       False,      2 },
};


static void
switchMon(const Arg *arg)
{
	tagmon(arg);
	focusmon(arg);
}


#include "cycle.c"

/* layout(s) */
static const float mfact      = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = False; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "TTT",      bstack },
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[] = { "dmenu_run", "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "gnome-terminal", NULL };

static const char *raisevolcmd[]        = {"amixer", "-q", "set", "Master", "5%+", NULL};
static const char *lowervolcmd[]        = {"amixer", "-q", "set", "Master", "5%-", NULL};
static const char *mutevolcmd[]         = {"amixer", "-q", "set", "Master", "toggle", NULL};
static const char *mpdnext[]         	= {"piano", "skip", NULL};
static const char *mpdprev[]         	= {"piano", "rate", "bad", NULL};
static const char *mpdtoggle[]         	= {"piano", "playpause", NULL};
static const char *destroysession[]     = {"killall", "/bin/bash", NULL};

static const char *keyboardlightup[] 	= {"sudo", "kbd_backlight", "up", NULL};
static const char *keyboardlightdown[] 	= {"sudo", "kbd_backlight", "down", NULL};

static const char *backlightup[] = {"brightness", "inc", NULL};
static const char *backlightdown[] = {"brightness", "dec", NULL};
static const char *ejectcdrom[] = {"eject", NULL};
static const char *lockscreen[] = {"dm-tool", "lock", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{0, 				XF86XK_AudioMute, 		spawn, {.v = mutevolcmd } },
	{0, 				XF86XK_AudioRaiseVolume, 	  spawn, {.v = raisevolcmd } },
	{0, 				XF86XK_AudioLowerVolume, 	  spawn, {.v = lowervolcmd } },
	{0, 				XF86XK_AudioPlay, 		      spawn, {.v = mpdtoggle } },
	{0, 				XF86XK_AudioNext, 		      spawn, {.v = mpdnext } },
	{0, 				XF86XK_AudioPrev, 		      spawn, {.v = mpdprev } },
  
	{0, 				XF86XK_KbdBrightnessUp, 	  spawn, {.v = keyboardlightup } },
	{0, 				XF86XK_KbdBrightnessDown, 	spawn, {.v = keyboardlightdown } },
  
	{0, 				XF86XK_MonBrightnessUp,   	    spawn, {.v = backlightup } },
	{0, 				XF86XK_MonBrightnessDown, 	    spawn, {.v = backlightdown } },
	{0, 				XF86XK_Eject, 	                    spawn, {.v = ejectcdrom} },

	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_u,      incnmaster,     {.i = -1 } },
  { MODKEY|ShiftMask,             XK_l,      spawn,          {.v = lockscreen} },

	{ MODKEY,                       XK_minus,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_equal,      setmfact,       {.f = +0.05} },

	{ MODKEY,                       XK_Tab,    focusmon,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },

	{ MODKEY,                       XK_b,               setlayout,  {.v = &layouts[0]} }, /* TTT bstack */
	{ MODKEY,                       XK_t,               setlayout,  {.v = &layouts[1]} }, /* []= tile */
	{ 0,                            XF86XK_LaunchA,     setlayout,  {.v = &layouts[2]} }, /* NULL */ 
	{ MODKEY,                       XK_m,               setlayout,  {.v = &layouts[3]} }, /* [M] monocle */
	{ 0,                            XF86XK_LaunchB,     setlayout,  {.v = &layouts[4]} }, /* === bstackhorziz */
	{ MODKEY,                       XK_g,               setlayout,  {.v = &layouts[5]} }, /* HHH grid */

	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  switchMon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, switchMon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_h,      cycle,          {.i = -1} },
	{ MODKEY|ShiftMask,             XK_l,      cycle,          {.i = +1} },

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};


