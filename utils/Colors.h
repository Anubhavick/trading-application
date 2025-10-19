#ifndef COLORS_H
#define COLORS_H

#include <string>

// ANSI Color codes for terminal output
namespace Colors {
    // Reset
    const std::string RESET = "\033[0m";
    
    // Regular Colors
    const std::string BLACK = "\033[0;30m";
    const std::string RED = "\033[0;31m";
    const std::string GREEN = "\033[0;32m";
    const std::string YELLOW = "\033[0;33m";
    const std::string BLUE = "\033[0;34m";
    const std::string MAGENTA = "\033[0;35m";
    const std::string CYAN = "\033[0;36m";
    const std::string WHITE = "\033[0;37m";
    
    // Bold Colors
    const std::string BOLD_BLACK = "\033[1;30m";
    const std::string BOLD_RED = "\033[1;31m";
    const std::string BOLD_GREEN = "\033[1;32m";
    const std::string BOLD_YELLOW = "\033[1;33m";
    const std::string BOLD_BLUE = "\033[1;34m";
    const std::string BOLD_MAGENTA = "\033[1;35m";
    const std::string BOLD_CYAN = "\033[1;36m";
    const std::string BOLD_WHITE = "\033[1;37m";
    
    // Background Colors
    const std::string BG_BLACK = "\033[40m";
    const std::string BG_RED = "\033[41m";
    const std::string BG_GREEN = "\033[42m";
    const std::string BG_YELLOW = "\033[43m";
    const std::string BG_BLUE = "\033[44m";
    const std::string BG_MAGENTA = "\033[45m";
    const std::string BG_CYAN = "\033[46m";
    const std::string BG_WHITE = "\033[47m";
    
    // Styles
    const std::string BOLD = "\033[1m";
    const std::string DIM = "\033[2m";
    const std::string UNDERLINE = "\033[4m";
    const std::string BLINK = "\033[5m";
    const std::string REVERSE = "\033[7m";
    
    // Trading specific colors
    const std::string PROFIT = BOLD_GREEN;
    const std::string LOSS = BOLD_RED;
    const std::string NEUTRAL = BOLD_CYAN;
    const std::string HEADER = BOLD_BLUE;
    const std::string SUCCESS = GREEN;
    const std::string ERROR = RED;
    const std::string WARNING = YELLOW;
    const std::string INFO = CYAN;
}

// Unicode symbols for better UI
namespace Symbols {
    const std::string ARROW_RIGHT = "→";
    const std::string ARROW_UP = "↑";
    const std::string ARROW_DOWN = "↓";
    const std::string BULLET = "•";
    const std::string CHECK = "✓";
    const std::string CROSS = "✗";
    const std::string STAR = "★";
    const std::string CIRCLE = "●";
    const std::string SQUARE = "■";
    const std::string DIAMOND = "◆";
    const std::string CHART_UP = "📈";
    const std::string CHART_DOWN = "📉";
    const std::string DOLLAR = "$";
    const std::string PERCENT = "%";
    const std::string PLUS = "+";
    const std::string MINUS = "-";
}

#endif
