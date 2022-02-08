/*
 * Project 2.c
 *
 * Created: 1/19/2022 3:43:18 PM
 * Author : Joseph
 */ 

#include <avr/io.h>
#include "avr.c"
#include "lcd.c"
#include "dt.c"
#include <stdio.h>

int get_key();
int is_pressed(int, int);
int validate_key(int);
void collect_data(struct dt*, int, int);
int update(char[], int, int, int);
int update_helper();
int validate_dt(int, int, int, int, int, int, int, int);

// Don't want 4 8 12 13 15 16
int keyMapping[] = {0, 1, 2, 3, 0, 4, 5, 6, 0, 7, 8, 9, 0, 0, 0, 0, 0};

int main(void)
{
	avr_init();
	lcd_init();
	lcd_clr();
	
	struct dt control;
	init_dt(&control);
	
	int k;
	int timeMode = 1;

    while (1) 
    {
		k = get_key();
		
		
		// If user presses 'A' then they want to set date
		if (k == 4) {
			collect_data(&control, 1, timeMode);
		}
		
		// If user presses 'B' then they want to set time
		else if (k == 8) {
			collect_data(&control, 0, timeMode);
		}
		
	    	// If user presses 'D' then they want to switch time mode
		else if (k == 16) {
			if (timeMode == 1)
				 timeMode = 0;
			else
				timeMode = 1;
		}
		
		print_dt(&control, timeMode);
		advance_dt(&control);
		avr_wait(950); // Accounting for the delay in is_pressed and time to run code
    }
}

int get_key() {
	int i, j;
	for(i = 0; i < 4; ++i) {
		for (j = 0; j < 4; ++j) {
			if (is_pressed(i, j)) {
				return i*4+j+1;
			}
		}
	}
	return 0;
}

int is_pressed(int r, int c) {
	// Set all pins to N/C
	DDRC = 0;
	PORTC = 0;
	
	// Set r to 0 (DDR 1 PORT 0)
	SET_BIT(DDRC, r);
	CLR_BIT(PORTC, r);
	
	// Set c to w1 (DDR 0 PORT 1)
	CLR_BIT(DDRC, c+4);
	SET_BIT(PORTC, c+4);
	
	// Check to see if button was pressed
	avr_wait(1);
	if (GET_BIT(PINC, c+4) == 0) {
		return 1;
	}
	return 0;
}

int validate_key(int k) {
	if (k == 4 || k == 8 || k == 12 || k == 13 || k == 15 || k == 16) {
		return 0;
	}
	
	return 1;
}

void collect_data(struct dt* dtObj, int key, int timeMode) {
	int tempYear = dtObj->year;
	int tempMonth = dtObj->month;
	int tempDay = dtObj->day;
	int tempHour = dtObj->hour;
	int tempMinute = dtObj->minute;
	int tempSecond = dtObj->second;

	int y1;
	int y2;
	int d_t;
	int AM = 1;
	
	while (1) {
		
		// User wants to set date
		if (key == 1) {
				
			// MONTH
			char m1[] = "Month MM:";
			tempMonth = update(m1, 0, 10, 1);
		
			// DAY
			char m2[] = "Day DD:";
			tempDay = update(m2, 0, 8, 1);

			// YEAR - need to run twice since update function only collects two inputs
			char m3[] = "Year YYYY:";
			y1 = update(m3, 0, 11, 1);
			y2 = update(m3, 0, 13, 0);
			tempYear = y1 * 100 + y2;
			
			lcd_clr();
			d_t = 1;
		}
		// User wants to set time
		else {
		
			// HOUR
			char m4[] = "Hour HH:";
			tempHour = update(m4, 0, 9, 1);
			
			// MINUTE
			char m5[] = "Minute MM:";
			tempMinute = update(m5, 0, 11, 1);

			// SECOND
			char m6[] = "Second SS:";
			tempSecond = update(m6, 0, 11, 1);
			
			// AM/PM
			if (!timeMode) {
				char m7[] = "AM/PM (1/2):";
				
				lcd_clr();
				lcd_pos(0,0);
				lcd_puts2(m7);
				
				int k = 0;
				while (k != 1 && k != 2) {
					k = get_key();
					if (k == 2) {
						AM = 0;
					}
				}
			}			
			
			lcd_clr();
			d_t = 0;
		}
		
		// Validate information 
		if (validate_dt(tempYear, tempMonth, tempDay, tempHour, tempMinute, tempSecond, timeMode, d_t)) {
			dtObj->year = tempYear;
			dtObj->month = tempMonth;
			dtObj->day = tempDay;
			dtObj->minute = tempMinute;
			dtObj->second = tempSecond;
			
			// IF AM/PM
			if (timeMode == 0) {
				// I want to store time as 24 hour but output it as AM/PM
				// If 12AM then we store hour as 0
				if (AM && tempHour == 12) {
					tempHour = 0;
				}
				// If 1PM or greater we need to add 12 to store as 24 hour
				else if (!AM && tempHour != 12) {
					tempHour = tempHour + 12;
				}
			}
			dtObj->hour = tempHour;
			
			break;
		}
		else {
			char m8[] = "Invalid Data";
			lcd_clr();
			lcd_puts2(m8);
			avr_wait(2000);
		}
	}
}

// The key is only used to check if update is being called when collecting a year
// If not collecting the year (key == 1) then refresh screen
// Otherwise do not refresh screen
// r and c refer to row and column of LCD screen
int update(char m[], int r, int c, int key) {
	int temp;
	int tempVar;
	
	if (key) {
		lcd_clr();
		lcd_puts2(m);
	}
	while (1) {
		temp = update_helper(r, c);
		if (temp != -1) {
			tempVar = temp;
			break;
		}
	}
	while (1) {
		temp = update_helper(r, c+1);
		if (temp != -1) {
			tempVar = tempVar * 10;
			tempVar = tempVar + temp;
			break;
		}
	}

	return tempVar;
}

int update_helper(int r, int c) {
	int k;
	int temp = -1;
	
	k = get_key();
	if (k != 0) {
		if (validate_key(k)) {
			temp = keyMapping[k];
			
			lcd_pos(r,c);
			lcd_put(temp+48);
			avr_wait(500);
		}
	}
	
	return temp;
}

int validate_dt(int tempYear, int tempMonth, int tempDay, int tempHour, int tempMinute, int tempSecond, int timeMode, int d_t) {
	// If validating date
	if (d_t) {
		// If month is in range
		if (tempMonth > 12 || tempMonth < 1) {
			return 0;
		}
	
		// If day is in range
		if (tempDay > numDaysInMonth[tempMonth - 1]) {
			// Check if leap year
			if (!(tempDay == 29 && tempMonth == 2 && tempYear % 4 == 0)) {
				return 0;
			}
		}
	}
	// If validating time
	else {
		// If hour is in range
		// If 24 hour clock
		if (timeMode == 1) {
			if (tempHour > 23 || tempHour < 0) {
				return 0;
			}
		}
		else {
			if (tempHour > 12 || tempHour < 1) {
				return 0;
			}
		}
	
		// If minute is in range
		if (tempMinute > 59 || tempMinute < 0) {
			return 0;
		}
	
		// If second is in range
		if (tempSecond > 59 || tempSecond < 0) {
			return 0;
		}
	}
	return 1;
}
