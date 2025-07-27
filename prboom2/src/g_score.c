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
static char scoremsg[256] = { 0 };
static int scoretime = TICRATE;

void G_ScoreTicker()
{
    if(!keep_score) return;

    if (in_streak && !paused && !menuactive) {
        if (streak_timeout > 0) {
            streak_timeout--;
        } else {
            G_RegisterScoreEvent(SCORE_EVT_STREAK_TIMEOUT, 0);
        }
        /* temporary */
        doom_printf("Score: %lld (time left: %d)", global_playerscore, streak_timeout);
        /* end temporary */
        scoretime = TICRATE;
    } else {
        /* temporary */
        doom_printf("Score: %lld", global_playerscore);
        /* end temporary */
    }
    /* temporary */
    if (scoremsg[0] && scoretime > 0) {
        doom_printf(scoremsg);
        scoretime--;
    }
    /* end temporary */
}

void G_ScoreInit()
{
    /* TODO: Can load this externally later */
    long long int global_playerscore = 0;
    dboolean in_streak = false;
    g_scorecfg[SCORE_CFG_TIMEOUT] = 3*TICRATE;
    g_scorecfg[SCORE_CFG_MIN_BREAK] = 1*TICRATE;
    keep_score = true;
}

void G_ScoreReset()
{
    global_playerscore = 0;
    in_streak = false;
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
    sprintf(scoremsg, "STREAK %s! +%lld", keep_bonus ? "BONUS" : "BROKEN!", keep_bonus ? G_CalculateStreak() : 0);
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
        case SCORE_EVT_ITEM_GOT:
        case SCORE_EVT_SECRET_FOUND:
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
