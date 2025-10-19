#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include <string>

// ANSI Color Codes
namespace Color {
    const std::string RESET = "\033[0m";
    const std::string BOLD = "\033[1m";
    const std::string DIM = "\033[2m";
    const std::string UNDERLINE = "\033[4m";
    
    // Foreground colors
    const std::string BLACK = "\033[30m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";
    
    // Bright colors
    const std::string BRIGHT_BLACK = "\033[90m";
    const std::string BRIGHT_RED = "\033[91m";
    const std::string BRIGHT_GREEN = "\033[92m";
    const std::string BRIGHT_YELLOW = "\033[93m";
    const std::string BRIGHT_BLUE = "\033[94m";
    const std::string BRIGHT_MAGENTA = "\033[95m";
    const std::string BRIGHT_CYAN = "\033[96m";
    const std::string BRIGHT_WHITE = "\033[97m";
    
    // Background colors
    const std::string BG_BLACK = "\033[40m";
    const std::string BG_RED = "\033[41m";
    const std::string BG_GREEN = "\033[42m";
    const std::string BG_YELLOW = "\033[43m";
    const std::string BG_BLUE = "\033[44m";
    const std::string BG_MAGENTA = "\033[45m";
    const std::string BG_CYAN = "\033[46m";
    const std::string BG_WHITE = "\033[47m";
}

// Unicode Symbols
namespace Symbol {
    const std::string CHECKMARK = "✓";
    const std::string CROSS = "✗";
    const std::string ARROW_RIGHT = "→";
    const std::string ARROW_UP = "↑";
    const std::string ARROW_DOWN = "↓";
    const std::string BULLET = "•";
    const std::string STAR = "★";
    const std::string DOLLAR = "💵";
    const std::string CHART_UP = "📈";
    const std::string CHART_DOWN = "📉";
    const std::string MONEY_BAG = "💰";
    const std::string TROPHY = "🏆";
    const std::string FIRE = "🔥";
    const std::string ROCKET = "🚀";
    const std::string WARNING = "⚠";
    const std::string INFO = "ℹ";
    const std::string GEAR = "⚙";
    const std::string USER = "👤";
    const std::string ADMIN = "👑";
    const std::string TRADER = "💼";
    const std::string LOCK = "🔒";
    const std::string KEY = "🔑";
    const std::string BELL = "🔔";
    const std::string TARGET = "🎯";
}

// Console utilities
class Console {
public:
    static void clear() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
    
    static void printHeader(const std::string& title) {
        std::cout << Color::BOLD << Color::BRIGHT_CYAN;
        std::cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║" << std::string(20, ' ') << title << std::string(20, ' ') << "║\n";
        std::cout << "╚════════════════════════════════════════════════════════════════╝\n";
        std::cout << Color::RESET;
    }
    
    static void printSectionHeader(const std::string& title) {
        std::cout << Color::BOLD << Color::BRIGHT_YELLOW;
        std::cout << "\n" << Symbol::STAR << " " << title << " " << Symbol::STAR << "\n";
        std::cout << Color::RESET;
    }
    
    static void printSuccess(const std::string& message) {
        std::cout << Color::BRIGHT_GREEN << Symbol::CHECKMARK << " " 
                  << message << Color::RESET << std::endl;
    }
    
    static void printError(const std::string& message) {
        std::cout << Color::BRIGHT_RED << Symbol::CROSS << " " 
                  << message << Color::RESET << std::endl;
    }
    
    static void printWarning(const std::string& message) {
        std::cout << Color::BRIGHT_YELLOW << Symbol::WARNING << " " 
                  << message << Color::RESET << std::endl;
    }
    
    static void printInfo(const std::string& message) {
        std::cout << Color::BRIGHT_CYAN << Symbol::INFO << " " 
                  << message << Color::RESET << std::endl;
    }
    
    static void printPrice(double price, double change) {
        if (change > 0) {
            std::cout << Color::BRIGHT_GREEN << Symbol::ARROW_UP << " $" 
                      << price << Color::RESET;
        } else if (change < 0) {
            std::cout << Color::BRIGHT_RED << Symbol::ARROW_DOWN << " $" 
                      << price << Color::RESET;
        } else {
            std::cout << Color::BRIGHT_WHITE << "$" << price << Color::RESET;
        }
    }
    
    static void printMenuOption(int number, const std::string& text, const std::string& icon = "") {
        std::cout << Color::BRIGHT_CYAN << "  [" << number << "] " << Color::RESET;
        if (!icon.empty()) {
            std::cout << icon << " ";
        }
        std::cout << Color::BRIGHT_WHITE << text << Color::RESET << std::endl;
    }
    
    static void printDivider(const std::string& pattern = "─", int length = 80) {
        std::cout << Color::DIM << Color::BRIGHT_BLACK;
        for (int i = 0; i < length; i++) {
            std::cout << pattern;
        }
        std::cout << Color::RESET << std::endl;
    }
    
    static void printTableHeader(const std::string& col1, const std::string& col2, 
                                 const std::string& col3, const std::string& col4,
                                 const std::string& col5 = "") {
        std::cout << Color::BOLD << Color::BRIGHT_MAGENTA;
        std::cout << col1 << col2 << col3 << col4;
        if (!col5.empty()) std::cout << col5;
        std::cout << Color::RESET << std::endl;
    }
};

#endif
