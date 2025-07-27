#include "g_score.h"
#include "doomdef.h"
#include "lprintf.h"
#include "math.h"

static long long int streak_bonus = 0;
static int streak_timeout = 0;

void G_ScoreTicker()
{
    if (in_streak) {
        if (streak_timeout > 0) {
            streak_timeout--;
        } else {
            G_RegisterEvent(SCORE_EVT_STREAK_TIMEOUT, 0);
        }
    }
}

void G_ScoreInit()
{
    /* TODO: Can load this externally later */
    long long int global_playerscore = 0;
    dboolean in_streak = false;
    g_scorecfg[SCORE_CFG_TIMEOUT] = 3*TICRATE;
    g_scorecfg[SCORE_CFG_MIN_BREAK] = 1*TICRATE;
}

void G_ScoreReset()
{
    long long int global_playerscore = 0;
    dboolean in_streak = false;
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
    streak_bonus = 0;
}

static void G_AccumulateStreak(int adder)
{
    streak_bonus += adder;
}

void G_RegisterEvent(g_score_event_t event, int arg)
{
    switch(event) {
        case SCORE_EVT_ENEMY_DAMAGED:
            if (!in_streak) {
                in_streak = true;
                streak_timeout = g_scorecfg[SCORE_CFG_TIMEOUT];
            }
            G_AccumulateStreak(arg);
            global_playerscore += arg;
            break;
        case SCORE_EVT_PLAYER_DAMAGED:
            G_BreakStreak(streak_timeout <= g_scorecfg[SCORE_CFG_MIN_BREAK]);
            break;
        case SCORE_EVT_ITEM_GOT:
        case SCORE_EVT_SECRET_FOUND:
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
