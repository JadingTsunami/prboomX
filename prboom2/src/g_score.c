#include "g_score.h"
#include "doomdef.h"
#include "lprintf.h"
#include "math.h"
#include "g_game.h"
#include "doomstat.h"
#include "v_video.h"

static long long int streak_bonus = 0;
static int streak_timeout = 0;
static dboolean keep_score = false;
static long long int total_playerscore = 0;
static long long int level_playerscore = 0;
static dboolean in_streak = false;
static int g_scorecfg[SCORE_CFG_LAST] = { 0 };

static char scoremsg[SCORE_MSG_SIZE] = { 0 };
static int scoretime = TICRATE;

dboolean G_ShouldKeepScore()
{
    return keep_score;
}

/* FIXME, this should be cleaned up, right now it works like:
 * Duration < 0: Clear message (blank)
 * Duration == 0: Show default message
 * Duration > 0: Show given message for duration
 *  duration > 0 && points > 0: Append "+points" to message
 *  duration > 0 && points < 0: Don't append anything to message
*/
static void G_Message(const char* msg, int duration, long long int points)
{
    static unsigned int durleft = 0;
    if (duration < 0) {
        /* clear */
        durleft = 0;
        scoremsg[0] = '\0';
        return;
    }
    if (duration == 0 && durleft > 0 && scoremsg[0]) {
        durleft--;
    } else if (msg) {
        /* clip to 3 seconds max */
        duration = MIN(duration, TICRATE*3);
        durleft = duration;
        if (points >= 0)
            snprintf(scoremsg, SCORE_MSG_SIZE, "%s +%lld", msg, points);
        else
            strncpy(scoremsg, msg, SCORE_MSG_SIZE);
            
    } else {
        /* no new message */
        if (in_streak && streak_timeout > 0)
            snprintf(scoremsg, SCORE_MSG_SIZE, "Score: %lld (Damage Streak!)", level_playerscore);
        else
            snprintf(scoremsg, SCORE_MSG_SIZE, "Score: %lld", level_playerscore);
    }
}

const char* G_GetScoreMessage()
{
    return scoremsg;
}

int G_GetScoreColor()
{
    /* todo: move to config variables */
#define SCORE_TIERS (12)
    static int score_color_map[SCORE_TIERS+1][2] = {
        {250, CR_BRICK},
        {500, CR_TAN},
        {1000, CR_BLUE2},
        {2500, CR_GREEN},
        {5000, CR_BROWN},
        {10000, CR_GOLD},
        {20000, CR_RED},
        {50000, CR_BLUE},
        {100000, CR_ORANGE},
        {250000, CR_YELLOW},
        {500000, CR_PURPLE},
        {1000000, CR_WHITE},
        {-1, CR_LIMIT}
    };

    if (in_streak) {
        if (streak_timeout < g_scorecfg[SCORE_CFG_MIN_BREAK]) {
            return CR_GRAY;
        }

        for (int i = 0; score_color_map[i][0] > 0; i++) {
            if (streak_bonus < score_color_map[i][0])
                return score_color_map[i][1];
        }
        return score_color_map[SCORE_TIERS-1][1];
    } else {
        return CR_BRICK;
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
    G_Message(NULL, 0, -1);
}

void G_ScoreInit()
{
    /* TODO: Can load this externally later */
    total_playerscore = 0;
    level_playerscore = 0;
    in_streak = false;
    g_scorecfg[SCORE_CFG_TIMEOUT] = 3*TICRATE;
    g_scorecfg[SCORE_CFG_MIN_BREAK] = 1*TICRATE;
    g_scorecfg[SCORE_CFG_SECRET_POINTS] = 1000;
    g_scorecfg[SCORE_CFG_SECRET_STREAK_EXTENSION] = 15*TICRATE;
    g_scorecfg[SCORE_CFG_ITEM_POINTS] = 5;
    g_scorecfg[SCORE_CFG_SPECIAL_ITEM_POINTS] = 200;
    g_scorecfg[SCORE_CFG_OVERKILL_POINTS] = 100;
    g_scorecfg[SCORE_CFG_100P_ITEMS] = 1000;
    g_scorecfg[SCORE_CFG_100P_SECRETS] = 2500;
    g_scorecfg[SCORE_CFG_100P_KILLS] = 2500;
    g_scorecfg[SCORE_CFG_100P_MAX] = 25000;

    keep_score = true;
}

void G_ScoreReset(dboolean clear_total)
{
    if (clear_total)
        total_playerscore = 0;
    level_playerscore = 0;
    in_streak = false;
    streak_bonus = 0;
    G_Message(NULL, -1, -1);
}

int G_GetStreakTimeLeft()
{
    return streak_timeout;
}

long long int G_GetLevelScore()
{
    return level_playerscore;
}

long long int G_GetTotalScore()
{
    return total_playerscore;
}

static long long int G_CalculateStreak()
{
    return (long long int) (1.8*pow((streak_bonus/8.0),1.4));
}

static void G_BreakStreak(dboolean keep_bonus)
{
    if (keep_bonus)
        level_playerscore += G_CalculateStreak();

    if (in_streak) {
        char msg[SCORE_MSG_SIZE];
        sprintf(msg,"STREAK %s! +%lld", keep_bonus ? "BONUS" : "BROKEN!", keep_bonus ? G_CalculateStreak() : 0);
        G_Message(msg, TICRATE*2, -1);
    }

    in_streak = false;
    streak_bonus = 0;
}

static void G_AccumulateStreak(int adder)
{
    streak_bonus += adder;
}

static void G_ScoreLevelDone()
{
    total_playerscore += level_playerscore;
}

void G_RegisterScoreEvent(g_score_event_t event, int arg)
{
    if(!keep_score) return;
    switch(event) {
        case SCORE_EVT_ENEMY_OVERKILL:
            in_streak = true;
            streak_timeout = MAX(streak_timeout, g_scorecfg[SCORE_CFG_TIMEOUT]);
            G_Message("OVERKILL!", TICRATE*1.5, g_scorecfg[SCORE_CFG_OVERKILL_POINTS]);
            G_AccumulateStreak(arg);
            level_playerscore += arg + g_scorecfg[SCORE_CFG_OVERKILL_POINTS];
            break;
        case SCORE_EVT_ENEMY_DAMAGED:
            in_streak = true;
            streak_timeout = MAX(streak_timeout, g_scorecfg[SCORE_CFG_TIMEOUT]);
            /* temporary */
            if (scoretime > 0) scoremsg[0] = '\0';
            /* end temporary */
            G_AccumulateStreak(arg);
            level_playerscore += arg;
            break;
        case SCORE_EVT_PLAYER_DAMAGED:
            G_BreakStreak(streak_timeout <= g_scorecfg[SCORE_CFG_MIN_BREAK]);
            break;
        case SCORE_EVT_SECRET_FOUND:
            streak_timeout = MAX(streak_timeout, g_scorecfg[SCORE_CFG_SECRET_STREAK_EXTENSION]);
            if (in_streak)
                G_Message("SECRET FOUND! (DAMAGE STREAK EXTENDED)", TICRATE*1.5, g_scorecfg[SCORE_CFG_SECRET_POINTS]);
            else
                G_Message("SECRET FOUND! (DAMAGE STREAK STARTED)", TICRATE*1.5, g_scorecfg[SCORE_CFG_SECRET_POINTS]);
            in_streak = true;
            level_playerscore += g_scorecfg[SCORE_CFG_SECRET_POINTS];
            break;
        case SCORE_EVT_SPECIAL_ITEM_GOT:
            /* item get does NOT start a new streak */
            streak_timeout = MAX(streak_timeout, g_scorecfg[SCORE_CFG_TIMEOUT]);
            G_Message("POWERUP!", TICRATE*1.5, g_scorecfg[SCORE_CFG_SPECIAL_ITEM_POINTS]);
            level_playerscore += g_scorecfg[SCORE_CFG_SPECIAL_ITEM_POINTS];
            break;
        case SCORE_EVT_ITEM_GOT:
            level_playerscore += g_scorecfg[SCORE_CFG_ITEM_POINTS];
            /* WARN: intentional fall through */
        case SCORE_EVT_NONCOUNTABLE_ITEM_GOT:
            /* item get does NOT start a new streak */
            streak_timeout = MAX(streak_timeout, g_scorecfg[SCORE_CFG_TIMEOUT]);
            break;
        case SCORE_EVT_ZOMBIE_DAMAGED:
            /* zombie damage does NOT start a new streak */
            streak_timeout = MAX(streak_timeout, g_scorecfg[SCORE_CFG_TIMEOUT]);
            break;
        case SCORE_EVT_STREAK_TIMEOUT:
            G_BreakStreak(true);
            break;
        case SCORE_EVT_LEVEL_DONE:
            G_BreakStreak(true);
            G_ScoreLevelDone();
            break;
        case SCORE_EVT_100P_ITEMS:
            G_Message("ALL ITEMS FOUND!", TICRATE*2, g_scorecfg[SCORE_CFG_100P_ITEMS]);
            level_playerscore += g_scorecfg[SCORE_CFG_100P_ITEMS];
            break;
        case SCORE_EVT_100P_SECRETS:
            G_Message("ALL SECRETS FOUND!", TICRATE*2, g_scorecfg[SCORE_CFG_100P_SECRETS]);
            level_playerscore += g_scorecfg[SCORE_CFG_100P_SECRETS];
            break;
        case SCORE_EVT_100P_KILLS:
            G_Message("ALL MONSTERS KILLED!", TICRATE*2, g_scorecfg[SCORE_CFG_100P_KILLS]);
            level_playerscore += g_scorecfg[SCORE_CFG_100P_KILLS];
            break;
        case SCORE_EVT_100P_MAX:
            G_Message("100%% MAX ACHIEVED", TICRATE*3, g_scorecfg[SCORE_CFG_100P_MAX]);
            level_playerscore += g_scorecfg[SCORE_CFG_100P_MAX];
            break;
        default:
            lprintf(LO_WARN, "Invalid score event registered: %d (arg: %d)", event, arg);
            break;
    }
}
