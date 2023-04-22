#include <cstdbool>
#include <cstdio>
#include <cmath>
#include <ctime>

#include "termbox2.h"

#include "logging.h"

#include "Ring.h"

Ring * ring;
double g_spin = M_PI / 4; // Per second
double z_rad;

void draw_coin(void);

int main()
{
    LOG_PRINTF("Program init");
    tb_init();
    tb_set_output_mode(TB_OUTPUT_NORMAL);

    LOG_PRINTF("Make ring");
    ring = new Ring(3.0, 20.0, 15.5);

    LOG_PRINTF("Initial draw");
    draw_coin();
    tb_present();

    LOG_PRINTF("Init clock");
    clock_t stop1 = clock();
    clock_t stop2;
    struct tb_event event = { 0 };

    LOG_PRINTF("Begin main loop");
    bool loop_once = false;
    while(TB_EVENT_KEY != event.type)
    {
        tb_clear();
        tb_peek_event(&event, 0);

        stop2 = clock();
        z_rad += g_spin * ((double)(stop2 - stop1) / CLOCKS_PER_SEC);
        z_rad = fmod(z_rad, M_PI * 2);
        draw_coin();

        int term_y = tb_height() - 3;
        tb_printf(2, term_y++, TB_WHITE, TB_BLACK, "Elapsed: %.02lf", stop1);
        tb_printf(2, term_y++, TB_WHITE, TB_BLACK, "Delta: %.02lf", stop2 - stop1);
        tb_print(2, term_y++, TB_WHITE, TB_BLACK, "Press any key to exit");
        tb_present();

        stop1 = stop2;

        if(!loop_once)
        {
            LOG_PRINTF("Looped once");
        }
    } 

    LOG_PRINTF("Deinit, exit");
    tb_shutdown();
    return 0;
}

void draw_coin()
{
    for(int x = 0; x < tb_width(); ++x)
    {
        for(int z = 0; z < tb_height(); ++z)
        {
            double off_x = (double)(x - (tb_width() / 2));
            double off_z = (double)(z - (tb_height() / 2));
#if 0
            char c = ring->is_rim_surface(off_x / 2, 0, off_z) ? '#' : ' ';
            tb_set_cell(x, z, c, TB_YELLOW, TB_BLACK);
#endif
            if(ring->is_rim_edge(off_x / 2, 0, off_z, z_rad))
            {
                tb_set_cell(x, z, '#', TB_YELLOW, TB_BLACK);
            }
            else if(ring->is_rim_side(off_x / 2, 0, off_z, z_rad))
            {
                tb_set_cell(x, z, '#', TB_GREEN, TB_BLACK);
            }
            else
            {
            }
        }
    }
}

void test_is_rim_side_should_know_from_coords()
{
    LOG_PRINTF("%lf %lf %lf are rim side %d", 
            0.0, 0.0, 18.0, ring->is_rim_side(0.0, 0.0, 18.0, z_rad));
    LOG_PRINTF("%lf %lf %lf are rim side %d", 
            -5.0, 0.0, -5.0, ring->is_rim_side(-5.0, 0.0, -5.0, z_rad));
    LOG_PRINTF("%lf %lf %lf are rim side %d", 
            -5.0, 0.0, 30.0, ring->is_rim_side(-5.0, 0.0, 30.0, z_rad));
}

