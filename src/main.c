#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "termbox2.h"

#include "logging.h"

typedef struct g_coin
{
    double  thick;
    double  r;
    double  r_empt;
    double  z_rad;
} coin_t;

double g_spin = M_PI / 4; // Per second
coin_t g_coin;

void define_g_coin(void);
void draw_g_coin(void);
static bool is_rim_surface(coin_t * p_coin, double x, double y, double z);
static bool is_rim_edge(coin_t * p_coin, double x, double y, double z);
static bool is_rim_side(coin_t * p_coin, double x, double y, double z);

int main()
{
    tb_init();
    tb_set_output_mode(TB_OUTPUT_NORMAL);

    define_g_coin();
    draw_g_coin();
    tb_present();

    clock_t stop1 = clock();
    clock_t stop2;
    struct tb_event event;
    while(TB_EVENT_KEY != event.type)
    {
        tb_clear();
        tb_peek_event(&event, 0);

        stop2 = clock();
        g_coin.z_rad += g_spin * ((double)(stop2 - stop1) / CLOCKS_PER_SEC);
        draw_g_coin();

        int term_y = tb_height() - 3;
        tb_printf(2, term_y++, TB_WHITE, TB_BLACK, "Elapsed: %.02lf", stop1);
        tb_printf(2, term_y++, TB_WHITE, TB_BLACK, "Delta: %.02lf", stop2 - stop1);
        tb_print(2, term_y++, TB_WHITE, TB_BLACK, "Press any key to exit");
        tb_present();

        stop1 = stop2;
    } 

    tb_shutdown();
    return 0;
}

void define_g_coin()
{
    g_coin.r      = 20.0;
    g_coin.r_empt = 17.5;
    g_coin.thick  = 1.0;
    g_coin.z_rad  = 45;
}

void draw_g_coin()
{
    for(int x = 0; x < tb_width(); ++x)
    {
        for(int z = 0; z < tb_height(); ++z)
        {
            double off_x = (double)(x - (tb_width() / 2));
            double off_z = (double)(z - (tb_height() / 2));
            char c = is_rim_surface(&g_coin, off_x / 2, 0, off_z) ? '#' : ' ';
            tb_set_cell(x, z, c, TB_YELLOW, TB_BLACK);
        }
    }
}

static bool is_rim_surface(coin_t * p_coin, double x, double y, double z)
{
    return is_rim_side(p_coin, x, y, z) || is_rim_edge(p_coin, x, y, z);
}

static bool is_rim_edge(coin_t * p_coin, double x, double y, double z)
{
    double thick_vis = p_coin->thick * fabs(sin(p_coin->z_rad));
    for(double i = thick_vis; i > 0.0; i -= 0.2)
    {
        if(is_rim_side(p_coin, x - i, y, z) 
        || is_rim_side(p_coin, x + i, y, z))
        {
            return true;
        }
    }
    return false;
}

static bool is_rim_side(coin_t * p_coin, double x, double y, double z)
{
    double radius = sqrt((pow(x, 2) / fabs(cos(p_coin->z_rad))) + pow(z, 2));
    bool b_incl = radius < p_coin->r;
    bool b_excl = radius < p_coin->r_empt;
    return b_incl && !b_excl;
}

void test_is_rim_side_should_know_from_coords()
{
    LOG_PRINTF("%lf %lf %lf are rim side %d", 
            0.0, 0.0, 18.0, is_rim_side(&g_coin, 0.0, 0.0, 18.0));
    LOG_PRINTF("%lf %lf %lf are rim side %d", 
            -5.0, 0.0, -5.0, is_rim_side(&g_coin, -5.0, 0.0, -5.0));
    LOG_PRINTF("%lf %lf %lf are rim side %d", 
            -5.0, 0.0, 30.0, is_rim_side(&g_coin, -5.0, 0.0, 30.0));
}

