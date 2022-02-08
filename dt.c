#include <stdio.h>

struct dt {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
};

const int numDaysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
void init_dt(struct dt* dtObj) {
	dtObj->year = 2022;
	dtObj->month = 1;
	dtObj->day = 1;
	dtObj->hour = 0;
	dtObj->minute = 0;
	dtObj->second = 0;
}

void advance_dt(struct dt* dtObj) {
	++dtObj->second;
	
	// Minute has passed
	if (dtObj->second == 60) {
		dtObj->second = 0;
		++dtObj->minute;
		
		// Hour has passed
		if (dtObj->minute == 60) {
			dtObj->minute = 0;
			++dtObj->hour;
			
			// Day has passed
			if (dtObj->hour == 24) {
				dtObj->hour = 0;
				++dtObj->day;
				
				// Month has passed
				if (dtObj->day > numDaysInMonth[dtObj->month - 1]) {
					// If not a leap day
					if (!(dtObj->month == 2 && dtObj->year % 4 == 0 && dtObj->day == 29)) {
						dtObj->day = 1;
						++dtObj->month;
					}
					
					// Year has passed
					if (dtObj->month > 12) {
						dtObj->month = 1;
						++dtObj->year;
					}		
				}
			}
		}
	}
}

void print_dt(struct dt* dtObj, int timeMode) {
	if (timeMode == 1) {
		lcd_pos(1,9);
		lcd_put(32);
		lcd_pos(1,10);
		lcd_put(32);
	}
	
	// PRINT DATE
	char buf[17];
	lcd_pos(0,0);
	sprintf(buf, "%02d / %02d / %04d",
				 dtObj->month,
				 dtObj->day,
				 dtObj->year);
	lcd_puts2(buf);
	avr_wait(15);
	
	// PRINT TIME
	char buf2[17];
	lcd_pos(1,0);
	
	// 24 Hour
	if (timeMode == 1) {
		sprintf(buf2, "%02d:%02d:%02d",
					 dtObj->hour,
					 dtObj->minute,
					 dtObj->second);
		lcd_puts2(buf2);
	}
	
	// AM/PM
	else {
		int newHour = dtObj->hour;
		int AM = 1;
		if (newHour == 0) {
			newHour = 12;	
		}
		
		// If hour is greater than 12 then PM
		if (dtObj->hour >= 12) {
			if (dtObj->hour > 12) {
				newHour = dtObj->hour - 12;
			}
			AM = 0;
		}
		
		sprintf(buf2, "%02d:%02d:%02d",
					 newHour,
					 dtObj->minute,
					 dtObj->second);
		lcd_puts2(buf2);
		
		// Print AM/PM
		lcd_pos(1,9);
		if (AM)
			// Print 'A'
			lcd_put(65);
		else
			// Print 'P'
			lcd_put(80);
		
		// Print 'M'
		lcd_pos(1,10);
		lcd_put(77);
	}
	avr_wait(15);
}
