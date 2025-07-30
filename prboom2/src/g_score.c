#include "g_score.h"
#include "doomdef.h"
#include "lprintf.h"
#include "math.h"
#include "g_game.h"
#include "doomstat.h"

static long long int streak_bonus = 0;
static int streak_timeout = 0;
static dboolean keep_score = false;

/* temporary */
#define SCORE_MSG_SIZE (256)
static char scoremsg[SCORE_MSG_SIZE] = { 0 };
static int scoretime = TICRATE;

static void G_Message(const char* msg, int duration)
{
    static unsigned int durleft = 0;
    if (duration < 0) {
        /* clear */
        durleft = 0;
        scoremsg[0] = '\0';
        return;
    }
    if (duration == 0 && durleft > 0 && scoremsg[0]) {
        doom_printf(scoremsg);
        durleft--;
    } else if (msg) {
        /* clip to 3 seconds max */
        duration = MIN(duration, TICRATE*3);
        durleft = duration;
        strncpy(scoremsg, msg, SCORE_MSG_SIZE);
        doom_printf(scoremsg);
    } else {
        /* no new message */
        if (in_streak && streak_timeout > 0)
            doom_printf("Score: %lld (time left: %d)", global_playerscore, streak_timeout);
        else
            doom_printf("Score: %lld", global_playerscore);
    }
}

void G_ScoreTicker()
{
    if(!keep_score) return;

    if (in_streak && !paused && !menuactive) {
        if (streak_timeout > 0) {
            streak_timeout--;
        } else {
            G_RegisterScoreEvent(SCORE_EVT_STREAK_TIMEOUT, 0);
        }
        scoretime = TICRATE;
    }
    G_Message(NULL, 0);
}

void G_ScoreInit()
{
    /* TODO: Can load this externally later */
    long long int global_playerscore = 0;
    dboolean in_streak = false;
    g_scorecfg[SCORE_CFG_TIMEOUT] = 3*TICRATE;
    g_scorecfg[SCORE_CFG_MIN_BREAK] = 1*TICRATE;
    g_scorecfg[SCORE_CFG_SECRET_POINTS] = 1000;
    g_scorecfg[SCORE_CFG_SECRET_STREAK_EXTENSION] = 15*TICRATE;
    keep_score = true;
}

void G_ScoreReset()
{
    global_playerscore = 0;
    in_streak = false;
    G_Message(NULL, -1);
}

int G_GetStreakTimeLeft()
{
    return streak_timeout;
}

long long int G_GetScore()
{
    return global_playerscore;
}

static long long int G_CalculateStreak()
{
    return (long long int) (2*pow((streak_bonus/6.0),1.5));
}

static void G_BreakStreak(dboolean keep_bonus)
{
    in_streak = false;
    if (keep_bonus)
        global_playerscore += G_CalculateStreak();

    /* temporary */
    {
        char msg[SCORE_MSG_SIZE];
        sprintf(msg,"STREAK %s! +%lld", keep_bonus ? "BONUS" : "BROKEN!", keep_bonus ? G_CalculateStreak() : 0);
        G_Message(msg, TICRATE*2);
    }
    /* end temporary */

    streak_bonus = 0;
}

static void G_AccumulateStreak(int adder)
{
    streak_bonus += adder;
}

void G_RegisterScoreEvent(g_score_event_t event, int arg)
{
    if(!keep_score) return;
    switch(event) {
        case SCORE_EVT_ENEMY_DAMAGED:
            in_streak = true;
            streak_timeout = g_scorecfg[SCORE_CFG_TIMEOUT];
            /* temporary */
            if (scoretime > 0) scoremsg[0] = '\0';
            /* end temporary */
            G_AccumulateStreak(arg);
            global_playerscore += arg;
            break;
        case SCORE_EVT_PLAYER_DAMAGED:
            G_BreakStreak(streak_timeout <= g_scorecfg[SCORE_CFG_MIN_BREAK]);
            break;
        case SCORE_EVT_SECRET_FOUND:
            streak_timeout = g_scorecfg[SCORE_CFG_SECRET_STREAK_EXTENSION];
            global_playerscore += g_scorecfg[SCORE_CFG_SECRET_POINTS];
            break;
        case SCORE_EVT_ITEM_GOT:
        case SCORE_EVT_ZOMBIE_DAMAGED:
            streak_timeout = g_scorecfg[SCORE_CFG_TIMEOUT];
            break;
        case SCORE_EVT_STREAK_TIMEOUT:
        case SCORE_EVT_LEVEL_DONE:
            G_BreakStreak(true);
            break;
        default:
            lprintf(LO_WARN, "Invalid score event registered: %d (arg: %d)", event, arg);
            break;
    }
}
