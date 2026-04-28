#include <iostream>

// Định nghĩa nhanh
#define RESET        "\033[0m"
#define BOLD_WHITE   "\033[1;37m"
#define BG_RED       "\033[41m"

int main() {
    // In chữ trắng (đậm) trên nền đỏ
    std::cout << BG_RED << BOLD_WHITE << " [ ERROR ] " << RESET 
              << " He thong bi gian doan! " << std::endl;
              
    return 0;
}
