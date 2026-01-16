#include "rtklib.h"

/**
 * @brief Display a message to the user
 * 
 * This function formats and displays a message to the application's message area.
 * If an empty format string is provided, it processes pending application messages.
 * 
 * @param format The printf-style format string
 * @param ... Variable arguments corresponding to the format specifiers
 * @return Non-zero if execution should be aborted, zero otherwise
 */
extern int showmsg(char *format, ...)
{
    va_list args;
    char buffer[1024];
    
    if (*format) {
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);
        
        // Output the message (implementation dependent)
        fprintf(stderr, "%s\n", buffer);
    }
    
    // Process any pending messages/events
    #ifdef _WIN32
    // Windows-specific message processing could go here
    #endif
    
    return 0; // Return 0 to continue execution
}

/**
 * @brief Set the time span for progress tracking
 * 
 * Sets the start and end times used for calculating progress percentages.
 * These values are used by settime() to update progress indicators.
 * 
 * @param ts Start time of the operation
 * @param te End time of the operation
 */
extern void settspan(gtime_t ts, gtime_t te)
{
    // Store the start and end times in static or global variables
    static gtime_t start_time = {0};
    static gtime_t end_time = {0};
    
    start_time = ts;
    end_time = te;
}

/**
 * @brief Update progress based on current time
 * 
 * Updates the progress indicator based on the current time relative to
 * the time span set by settspan(). Periodically processes application
 * messages to keep the UI responsive.
 * 
 * @param time Current time of the operation
 */
extern void settime(gtime_t time)
{
    static int counter = 0;
    static gtime_t start_time = {0};
    static gtime_t end_time = {0};
    
    // Calculate progress as percentage of time span
    double total_span, current_progress;
    
    if (end_time.time != 0 && start_time.time != 0 && 
        (total_span = timediff(end_time, start_time)) > 0.0) {
        
        current_progress = timediff(time, start_time) / total_span * 100.0;
        
        // Here you would update your progress indicator
        // e.g., printf("Progress: %.1f%%\r", current_progress);
    }
    
    // Process messages occasionally to keep UI responsive
    if (++counter % 23 == 0) {
        #ifdef _WIN32
        // Windows-specific message processing could go here
        #endif
    }
}