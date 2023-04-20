#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "termbox2.h"

#include "logging.h"

typedef struct g_coin
{
    double  thick;
    double  r_out;
    double  r_inner;
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
        g_coin.z_rad = fmod(g_coin.z_rad, M_PI * 2);
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
    g_coin.r_out      = 20.0;
    g_coin.r_inner = 15.5;
    g_coin.thick  = 3.0;
    g_coin.z_rad  = 0;
}

void draw_g_coin()
{
    for(int x = 0; x < tb_width(); ++x)
    {
        for(int z = 0; z < tb_height(); ++z)
        {
            double off_x = (double)(x - (tb_width() / 2));
            double off_z = (double)(z - (tb_height() / 2));
#if 0
            char c = is_rim_surface(&g_coin, off_x / 2, 0, off_z) ? '#' : ' ';
            tb_set_cell(x, z, c, TB_YELLOW, TB_BLACK);
#endif
            if(is_rim_edge(&g_coin, off_x / 2, 0, off_z))
            {
                tb_set_cell(x, z, '#', TB_YELLOW, TB_BLACK);
            }
            else if(is_rim_side(&g_coin, off_x / 2, 0, off_z))
            {
                tb_set_cell(x, z, '#', TB_GREEN, TB_BLACK);
            }
            else
            {
            }
        }
    }
}

static bool is_rim_surface(coin_t * p_coin, double x, double y, double z)
{
    //return is_rim_side(p_coin, x, y, z);
    //return is_rim_edge(p_coin, x, y, z);
    return is_rim_side(p_coin, x, y, z) || is_rim_edge(p_coin, x, y, z);
}

static bool is_rim_edge(coin_t * p_coin, double x, double y, double z)
{
    double edge_xdist = sqrt(pow(p_coin->r_out, 2) - pow(z, 2)) *
                                            cos(p_coin->z_rad);
    double thick_vis = p_coin->thick * fabs(sin(p_coin->z_rad));

    x = ((M_PI * 0.0 < p_coin->z_rad) && (p_coin->z_rad < M_PI * 1.0)) ? x : -x;

    bool b_ascend = 
        ((M_PI * 0.0 < p_coin->z_rad) && (p_coin->z_rad < M_PI * 0.5)) ||
        ((M_PI * 1.0 < p_coin->z_rad) && (p_coin->z_rad < M_PI * 1.5));

    bool b_x_on_edge = b_ascend ? 
        ((edge_xdist - thick_vis / 2) < x) && (x < (edge_xdist + thick_vis / 2)) :
        (x < (edge_xdist + thick_vis / 2)) && ((edge_xdist - thick_vis / 2) < x);
    return b_x_on_edge;
}

static bool is_rim_side(coin_t * p_coin, double x, double y, double z)
{
    double thick_vis = p_coin->thick * fabs(sin(p_coin->z_rad));
    bool b_ascend = 
        ((M_PI * 0.0 < p_coin->z_rad) && (p_coin->z_rad < M_PI * 0.5)) ||
        ((M_PI * 1.0 < p_coin->z_rad) && (p_coin->z_rad < M_PI * 1.5));
    b_ascend ? (x += thick_vis / 2) : (x -= thick_vis / 2);

    double radius = sqrt((pow(x, 2) / fabs(cos(p_coin->z_rad))) + pow(z, 2));
    bool b_incl = radius < p_coin->r_out;
    bool b_excl = radius < p_coin->r_inner;
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

