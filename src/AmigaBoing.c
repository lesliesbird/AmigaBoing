#include "pebble.h"

#define TOP_X 2
#define TOP_Y 10
#define BOTTOM_X 88
#define BOTTOM_Y 64
#define BALL_SPEED 40
#define PATTERN 3
//default Traditional bounce
#ifdef PBL_COLOR
Window *window;
BitmapLayer *boinglayer;
AppTimer *timer_handle;
TextLayer *TimeLayer;
TextLayer *DateLayer;
GBitmap *grid;
BitmapLayer *grid_layer;
GBitmap *boing;
GRect boing_ball_start;

static char TimeText[] = "00:00:00";
static char DateText[] = "Xxxxxxxxx 00";
static char ZeroField[] = " 0";

int ball_x = TOP_X;
int ball_y = TOP_Y;
int ball_x_direction = 1;
int ball_y_direction = 1;
int ball_rotation = -1;
int boing_frame = 1;
int y_accel;
int bpattern = PATTERN;

void accel_tap_handler(AccelAxisType axis, int32_t direction) {

    if ((axis == 1) && (direction == -1)) {
        bpattern++;
        if (bpattern == 4) bpattern = 0;
    }
}

void handle_timer() {
    
    int local_top_y;
    
    switch (bpattern) {
            
        case 0:
            //Boing ball short bounce
            local_top_y = TOP_Y + 10;
            ball_x = ball_x + ball_x_direction;
            
            if (ball_x < TOP_X) ball_x = TOP_X;
            if (ball_x > BOTTOM_X) ball_x = BOTTOM_X;
            
            if (ball_y < local_top_y) ball_y = local_top_y;
            if (ball_y > BOTTOM_Y) ball_y = BOTTOM_Y;
            y_accel = (ball_y / TOP_Y) * 2;
            if (ball_y_direction == 1) {
                ball_y = ball_y + y_accel;
            } else {
                ball_y = ball_y - y_accel;
            }
            
            
            if ((ball_x == TOP_X) || (ball_x == BOTTOM_X)) {
                
                ball_y_direction = 1;
                
                if (ball_x_direction == 1) {
                    ball_x_direction = -1;
                    ball_rotation = 1;
                } else {
                    ball_x_direction = 1;
                    ball_rotation = -1;
                }
            }
            if ((ball_y <= local_top_y) || (ball_y >= BOTTOM_Y)) {
                
                if (ball_y_direction == 1) {
                    ball_y_direction = -1;
                } else {
                    ball_y_direction = 1;
                }
            }
            break;
            
        case 1:
            //Pong type bounce
            local_top_y = TOP_Y - 4;
            
            ball_x = ball_x + ball_x_direction;
            ball_y = ball_y + ball_y_direction;
            
            if (ball_x < TOP_X) ball_x = TOP_X;
            if (ball_x > BOTTOM_X) ball_x = BOTTOM_X;
            
            if (ball_y < local_top_y) ball_y = local_top_y;
            if (ball_y > BOTTOM_Y) ball_y = BOTTOM_Y;
            
            
            if ((ball_x == TOP_X) || (ball_x == BOTTOM_X)) {
                
                if (ball_x_direction == 1) {
                    ball_x_direction = -1;
                    ball_rotation = 1;
                } else {
                    ball_x_direction = 1;
                    ball_rotation = -1;
                }
            }
            if ((ball_y == local_top_y) || (ball_y == BOTTOM_Y)) {
                
                if (ball_y_direction == 1) {
                    ball_y_direction = -1;
                } else {
                    ball_y_direction = 1;
                }
            }
            break;
            
        case 2:
            //Pong type rapid bounce
            local_top_y = TOP_Y - 4;
            ball_x = ball_x + ball_x_direction;
            ball_y = ball_y + ball_y_direction;
            
            if (ball_x < TOP_X) ball_x = TOP_X;
            if (ball_x > BOTTOM_X) ball_x = BOTTOM_X;
            
            if (ball_y < local_top_y) ball_y = local_top_y;
            if (ball_y > BOTTOM_Y) ball_y = BOTTOM_Y;
            
            if ((ball_x == TOP_X) || (ball_x == BOTTOM_X)) {
                
                if (ball_x_direction == 3) {
                    ball_x_direction = -3;
                    ball_rotation = 1;
                } else {
                    ball_x_direction = 3;
                    ball_rotation = -1;
                }
            }
            if ((ball_y == local_top_y) || (ball_y == BOTTOM_Y)) {
                
                if (ball_y_direction == 5) {
                    ball_y_direction = -5;
                } else {
                    ball_y_direction = 5;
                }
            }
            break;
            
        default:
            //Traditional Boing ball heavy bounce
            ball_x = ball_x + ball_x_direction;
            
            if (ball_x < TOP_X) ball_x = TOP_X;
            if (ball_x > BOTTOM_X) ball_x = BOTTOM_X;
            
            if (ball_y < TOP_Y) ball_y = TOP_Y;
            if (ball_y > BOTTOM_Y) ball_y = BOTTOM_Y;
            
            y_accel = (ball_y / TOP_Y);
            if (ball_y_direction == 1) {
                ball_y = ball_y + y_accel;
            } else {
                ball_y = ball_y - y_accel;
            }
            
            
            if ((ball_x == TOP_X) || (ball_x == BOTTOM_X)) {
                
                ball_y_direction = 1;
                
                if (ball_x_direction == 1) {
                    ball_x_direction = -1;
                    ball_rotation = 1;
                } else {
                    ball_x_direction = 1;
                    ball_rotation = -1;
                }
            }
            if ((ball_y <= TOP_Y) || (ball_y >= BOTTOM_Y)) {
                
                if (ball_y_direction == 1) {
                    ball_y_direction = -1;
                } else {
                    ball_y_direction = 1;
                }
            }
            break;
    }
    

				
		boing_frame = boing_frame + ball_rotation;
		
		if (boing_frame == 6) boing_frame = 1;
		if (boing_frame == 0) boing_frame = 5;
		
        gbitmap_destroy(boing);
        boing = gbitmap_create_with_resource(boing_frame);

        bitmap_layer_set_bitmap(boinglayer, boing);
    
        layer_set_frame(bitmap_layer_get_layer(boinglayer), GRect(ball_x, ball_y, 66, 52));
        layer_mark_dirty(window_get_root_layer(window));
    
        timer_handle = app_timer_register(BALL_SPEED, handle_timer, NULL);
}

void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
	
    char *TimeFormat;
    char *DateFormat;
    
	int loop = 0, count = 0, ismatch = 0, datesize;

	if (clock_is_24h_style()) {
		TimeFormat = "%R:%S";
	} else {
		
		TimeFormat = "%I:%M:%S";
	}
	
	strftime(TimeText, sizeof(TimeText), TimeFormat, tick_time);
	if (!clock_is_24h_style() && (TimeText[0] == '0')) {
		memmove(TimeText, &TimeText[1], sizeof(TimeText) - 1);
    }
	
	text_layer_set_text(TimeLayer, TimeText);
    
    DateFormat = "%B %d";
    datesize = sizeof(DateText);
    
	strftime(DateText, sizeof(DateText), DateFormat, tick_time);
	
	while ((ismatch == 0) && (loop < datesize))
	{
		
		if (ZeroField[count] == DateText[loop])
		{
			count = count + 1;
			if (count == 2)
			{
				ismatch = loop;
			}
		}
		else
		{
			count = 0;
		}
		loop = loop + 1;
	}
	
	if (ismatch > 0) {
		DateText[ismatch] = DateText[ismatch + 1];
		DateText[ismatch + 1] = '\0';
	}
    
	text_layer_set_text(DateLayer, DateText);
    
}

void handle_init(void) {
	
	window = window_create();
	window_stack_push(window, true /* Animated */);
	Layer *window_layer = window_get_root_layer(window);
 
    
    timer_handle = app_timer_register(BALL_SPEED, handle_timer, NULL);
    grid = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_GRID);
    grid_layer = bitmap_layer_create(layer_get_frame(window_layer));
    bitmap_layer_set_bitmap(grid_layer, grid);
	layer_add_child(window_layer, bitmap_layer_get_layer(grid_layer));

	boing_ball_start = GRect(ball_x, ball_y, 66, 52);
                    
                    
    //randomize ball starting position and bounce
    srand(time(NULL));
    ball_x = (rand() % (BOTTOM_X - TOP_X)) + TOP_X;
    ball_y = (rand() % (BOTTOM_Y - TOP_Y)) + TOP_Y;
    if (rand() % 2 == 2) {
        ball_x_direction = -1;
    } else {
        ball_x_direction = 1;
    }
    if (rand() % 2 == 2) {
        ball_y_direction = -1;
    } else {
        ball_y_direction = 1;
    }
 

    boing = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FRAME6);
    boinglayer = bitmap_layer_create(boing_ball_start);
    bitmap_layer_set_compositing_mode(boinglayer, GCompOpSet);
    bitmap_layer_set_bitmap(boinglayer, boing);
    
    layer_add_child(bitmap_layer_get_layer(grid_layer), bitmap_layer_get_layer(boinglayer));


    TimeLayer = text_layer_create(GRect(0, 129, 144, 40));
    text_layer_set_text_color(TimeLayer, GColorWhite);
    text_layer_set_background_color(TimeLayer, GColorClear);
	text_layer_set_text_alignment(TimeLayer, GTextAlignmentCenter);
	text_layer_set_font(TimeLayer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OLDTOPAZ_SUBSET_19)));
	
	layer_add_child(window_layer, text_layer_get_layer(TimeLayer));
	
    DateLayer = text_layer_create(GRect(0, 150, 144, 40));
    text_layer_set_text_color(DateLayer, GColorFromHEX(0xff8800));
    text_layer_set_background_color(DateLayer, GColorClear);
	text_layer_set_text_alignment(DateLayer, GTextAlignmentCenter);
	text_layer_set_font(DateLayer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OLDTOPAZ_SUBSET_14)));
		
	layer_add_child(window_layer, text_layer_get_layer(DateLayer));

    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    handle_second_tick(current_time, SECOND_UNIT);
    
    
    tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
    accel_tap_service_subscribe(accel_tap_handler);
    	
	
}

void handle_deinit(void) {
    
    tick_timer_service_unsubscribe();
    accel_tap_service_unsubscribe();
    text_layer_destroy(TimeLayer);
    text_layer_destroy(DateLayer);
    gbitmap_destroy(grid);
    bitmap_layer_destroy(grid_layer);
    gbitmap_destroy(boing);
    bitmap_layer_destroy(boinglayer);
    window_destroy(window);

}

int main(void) {
    
    handle_init();
    app_event_loop();
    handle_deinit();
    
}
#else

Window *window;
BitmapLayer *boinglayer1;
BitmapLayer *boinglayer2;
AppTimer *timer_handle;
TextLayer *TimeLayer;
TextLayer *DateLayer;
GBitmap *grid;
BitmapLayer *grid_layer;
GBitmap *boing1;
GBitmap *boing2;
GRect boing_ball_start;

static char TimeText[] = "00:00:00";
static char DateText[] = "Xxxxxxxxx 00";
static char ZeroField[] = " 0";

int ball_x = TOP_X;
int ball_y = TOP_Y;
int ball_x_direction = 1;
int ball_y_direction = 1;
int ball_rotation = -1;
int boing_frame = 1;
int boing_pair, y_accel;
int bpattern = PATTERN;

void accel_tap_handler(AccelAxisType axis, int32_t direction) {

    if ((axis == 1) && (direction == -1)) {
        bpattern++;
        if (bpattern == 4) bpattern = 0;
    }
}

void handle_timer() {
    
    int local_top_y;
    
    switch (bpattern) {
            
        case 0:
            //Boing ball short bounce
            local_top_y = TOP_Y + 10;
            ball_x = ball_x + ball_x_direction;
            
            if (ball_x < TOP_X) ball_x = TOP_X;
            if (ball_x > BOTTOM_X) ball_x = BOTTOM_X;
            
            if (ball_y < local_top_y) ball_y = local_top_y;
            if (ball_y > BOTTOM_Y) ball_y = BOTTOM_Y;
            y_accel = (ball_y / TOP_Y) * 2;
            if (ball_y_direction == 1) {
                ball_y = ball_y + y_accel;
            } else {
                ball_y = ball_y - y_accel;
            }
            
            
            if ((ball_x == TOP_X) || (ball_x == BOTTOM_X)) {
                
                ball_y_direction = 1;
                
                if (ball_x_direction == 1) {
                    ball_x_direction = -1;
                    ball_rotation = 1;
                } else {
                    ball_x_direction = 1;
                    ball_rotation = -1;
                }
            }
            if ((ball_y <= local_top_y) || (ball_y >= BOTTOM_Y)) {
                
                if (ball_y_direction == 1) {
                    ball_y_direction = -1;
                } else {
                    ball_y_direction = 1;
                }
            }
            break;
            
        case 1:
            //Pong type bounce
            local_top_y = TOP_Y - 4;
            
            ball_x = ball_x + ball_x_direction;
            ball_y = ball_y + ball_y_direction;
            
            if (ball_x < TOP_X) ball_x = TOP_X;
            if (ball_x > BOTTOM_X) ball_x = BOTTOM_X;
            
            if (ball_y < local_top_y) ball_y = local_top_y;
            if (ball_y > BOTTOM_Y) ball_y = BOTTOM_Y;
            
            
            if ((ball_x == TOP_X) || (ball_x == BOTTOM_X)) {
                
                if (ball_x_direction == 1) {
                    ball_x_direction = -1;
                    ball_rotation = 1;
                } else {
                    ball_x_direction = 1;
                    ball_rotation = -1;
                }
            }
            if ((ball_y == local_top_y) || (ball_y == BOTTOM_Y)) {
                
                if (ball_y_direction == 1) {
                    ball_y_direction = -1;
                } else {
                    ball_y_direction = 1;
                }
            }
            break;
            
        case 2:
            //Pong type rapid bounce
            local_top_y = TOP_Y - 4;
            ball_x = ball_x + ball_x_direction;
            ball_y = ball_y + ball_y_direction;
            
            if (ball_x < TOP_X) ball_x = TOP_X;
            if (ball_x > BOTTOM_X) ball_x = BOTTOM_X;
            
            if (ball_y < local_top_y) ball_y = local_top_y;
            if (ball_y > BOTTOM_Y) ball_y = BOTTOM_Y;
            
            if ((ball_x == TOP_X) || (ball_x == BOTTOM_X)) {
                
                if (ball_x_direction == 3) {
                    ball_x_direction = -3;
                    ball_rotation = 1;
                } else {
                    ball_x_direction = 3;
                    ball_rotation = -1;
                }
            }
            if ((ball_y == local_top_y) || (ball_y == BOTTOM_Y)) {
                
                if (ball_y_direction == 5) {
                    ball_y_direction = -5;
                } else {
                    ball_y_direction = 5;
                }
            }
            break;
            
        default:
            //Traditional Boing ball heavy bounce
            ball_x = ball_x + ball_x_direction;
            
            if (ball_x < TOP_X) ball_x = TOP_X;
            if (ball_x > BOTTOM_X) ball_x = BOTTOM_X;
            
            if (ball_y < TOP_Y) ball_y = TOP_Y;
            if (ball_y > BOTTOM_Y) ball_y = BOTTOM_Y;
            
            y_accel = (ball_y / TOP_Y);
            if (ball_y_direction == 1) {
                ball_y = ball_y + y_accel;
            } else {
                ball_y = ball_y - y_accel;
            }
            
            
            if ((ball_x == TOP_X) || (ball_x == BOTTOM_X)) {
                
                ball_y_direction = 1;
                
                if (ball_x_direction == 1) {
                    ball_x_direction = -1;
                    ball_rotation = 1;
                } else {
                    ball_x_direction = 1;
                    ball_rotation = -1;
                }
            }
            if ((ball_y <= TOP_Y) || (ball_y >= BOTTOM_Y)) {
                
                if (ball_y_direction == 1) {
                    ball_y_direction = -1;
                } else {
                    ball_y_direction = 1;
                }
            }
            break;
    }
    
		layer_set_frame(bitmap_layer_get_layer(boinglayer1), GRect(ball_x, ball_y, 66, 52));
		layer_set_frame(bitmap_layer_get_layer(boinglayer2), GRect(ball_x, ball_y, 66, 52));
			
		boing_frame = boing_frame + ball_rotation;
		
		if (boing_frame == 6) boing_frame = 1;
		if (boing_frame == 0) boing_frame = 5;
		
		boing_pair = boing_frame * 2;
		
        gbitmap_destroy(boing1);
        gbitmap_destroy(boing2);
    

        boing1 = gbitmap_create_with_resource(boing_pair-1);
        boing2 = gbitmap_create_with_resource(boing_pair);

		layer_mark_dirty(window_get_root_layer(window));

        timer_handle = app_timer_register(BALL_SPEED, handle_timer, NULL);
}

void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
	
    char *TimeFormat;
    char *DateFormat;
    
	int loop = 0, count = 0, ismatch = 0, datesize;

	if (clock_is_24h_style()) {
		TimeFormat = "%R:%S";
	} else {
		
		TimeFormat = "%I:%M:%S";
	}
	
	strftime(TimeText, sizeof(TimeText), TimeFormat, tick_time);
	if (!clock_is_24h_style() && (TimeText[0] == '0')) {
		memmove(TimeText, &TimeText[1], sizeof(TimeText) - 1);
    }
	
	text_layer_set_text(TimeLayer, TimeText);
    
    DateFormat = "%B %d";
    datesize = sizeof(DateText);
    
	strftime(DateText, sizeof(DateText), DateFormat, tick_time);
	
	while ((ismatch == 0) && (loop < datesize))
	{
		
		if (ZeroField[count] == DateText[loop])
		{
			count = count + 1;
			if (count == 2)
			{
				ismatch = loop;
			}
		}
		else
		{
			count = 0;
		}
		loop = loop + 1;
	}
	
	if (ismatch > 0) {
		DateText[ismatch] = DateText[ismatch + 1];
		DateText[ismatch + 1] = '\0';
	}
    
	text_layer_set_text(DateLayer, DateText);
    
}

void handle_init(void) {
	
	window = window_create();
	window_stack_push(window, true /* Animated */);
	Layer *window_layer = window_get_root_layer(window);
 
    
    timer_handle = app_timer_register(BALL_SPEED, handle_timer, NULL);
    grid = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_GRID);
    grid_layer = bitmap_layer_create(layer_get_frame(window_layer));
    bitmap_layer_set_bitmap(grid_layer, grid);
	layer_add_child(window_layer, bitmap_layer_get_layer(grid_layer));

	boing_ball_start = GRect(ball_x, ball_y, 66, 52);
                    
                    
    //randomize ball starting position and bounce
    srand(time(NULL));
    ball_x = (rand() % (BOTTOM_X - TOP_X)) + TOP_X;
    ball_y = (rand() % (BOTTOM_Y - TOP_Y)) + TOP_Y;
    if (rand() % 2 == 2) {
        ball_x_direction = -1;
    } else {
        ball_x_direction = 1;
    }
    if (rand() % 2 == 2) {
        ball_y_direction = -1;
    } else {
        ball_y_direction = 1;
    }
                    

    boing1 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FRAME1_WHITE);
    boing2 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FRAME1_BLACK);

    boinglayer1 = bitmap_layer_create(boing_ball_start);
    boinglayer2 = bitmap_layer_create(boing_ball_start);

	bitmap_layer_set_compositing_mode(boinglayer1, GCompOpOr);
	bitmap_layer_set_compositing_mode(boinglayer2, GCompOpClear);
	bitmap_layer_set_bitmap(boinglayer1, boing1);
	bitmap_layer_set_bitmap(boinglayer2, boing2);

	layer_add_child(bitmap_layer_get_layer(grid_layer), bitmap_layer_get_layer(boinglayer1));
	layer_add_child(bitmap_layer_get_layer(grid_layer), bitmap_layer_get_layer(boinglayer2));

    TimeLayer = text_layer_create(GRect(0, 129, 144, 40));
    text_layer_set_text_color(TimeLayer, GColorBlack);
	text_layer_set_background_color(TimeLayer, GColorClear);
	text_layer_set_text_alignment(TimeLayer, GTextAlignmentCenter);
	text_layer_set_font(TimeLayer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OLDTOPAZ_SUBSET_19)));
	
	layer_add_child(window_layer, text_layer_get_layer(TimeLayer));
	
    DateLayer = text_layer_create(GRect(0, 150, 144, 40));
    text_layer_set_text_color(DateLayer, GColorBlack);
    text_layer_set_background_color(DateLayer, GColorClear);
	text_layer_set_text_alignment(DateLayer, GTextAlignmentCenter);
	text_layer_set_font(DateLayer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OLDTOPAZ_SUBSET_14)));
		
	layer_add_child(window_layer, text_layer_get_layer(DateLayer));
    
    
    
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    handle_second_tick(current_time, SECOND_UNIT);
    
    
    tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
    accel_tap_service_subscribe(accel_tap_handler);
    	
	
}

void handle_deinit(void) {
    
    tick_timer_service_unsubscribe();
    accel_tap_service_unsubscribe();
    text_layer_destroy(TimeLayer);
    text_layer_destroy(DateLayer);
    gbitmap_destroy(grid);
    bitmap_layer_destroy(grid_layer);
    gbitmap_destroy(boing1);
    gbitmap_destroy(boing2);
    bitmap_layer_destroy(boinglayer1);
    bitmap_layer_destroy(boinglayer2);
    window_destroy(window);

}

int main(void) {
    
    handle_init();
    app_event_loop();
    handle_deinit();
    
}
#endif
