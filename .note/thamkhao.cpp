#include <windows.h>

#include <algorithm>
#include <cctype>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#define SO_LUONG_BAN_AN 12 // GIOI HAN SO LUONG BAN
#define FULL true          // BAN DA DUOC CHON
#define EMPTY false        // BAN TRONG
#define PAID true          // DA THANH TOAN
#define UNPAID false       // CHUA THANH TOAN
#define MAX_ACCOUNTS 100   // GIOI HAN TAI KHOAN
#define MAX_BILLS 1000     // GIOI HAN HOA DON

#define italic "\x1b[3m"
#define bold "\x1b[1m"
#define off "\x1b[0m"

#define red "\x1b[31m"
#define yellow "\x1b[33m"
#define green "\x1b[32m"
#define blue "\x1b[34m"
#define purple "\x1b[35m"
#define purple1 "\x1b[38;5;5m"
#define cyan "\x1b[36m"
#define white1 "\x1b[37m"
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

using namespace std;

//  HAM KHAC
void xoamanhinh();
bool laMaMonAnHopLe(const string &ma);
string DinhdangString(string s);
bool soNguyen(string s);
bool KiemtraNamNhuan(int year);
bool LaNgayHopLe(int dd, int mm, int yy);

// MA HOA DON
string ma_hoadon(bool tang);
void lay_mahoadon(int code_bill[], int &bill_count);
void khoitao_mahoadon();

// QUAN LY MENU
void suaMenu();
void inSuaMenu();
void inMenuAdmin();
void inMenuAdminNuoc();
void inMenuAdminThucAn();
void inMenuStaff();

// QUAN LY HOA DON
void ghiHoaDonFULL(int ban);
void ghiHoaDonEMPTY(int ban);
void datHoaDon(int ban);
void inHoaDon(int ban);
void inQuanLyHoaDon();

// QUAN LY BAN AN
void inSoDoBan();
void chonBan();

// BIEN TOAN CUC
string name_user;
bool isadmin = false;
int current_count = -1;

// LOGIN
void printRestaurant();
void printWelcomeBack();
void printLoadingBar();
void printLoginForm();
void manHinhChinh();

// STAFF VA ADMIN
void staff();
void admin();
void inStaffChucNang();
void inAdminChucNang();

struct login { // dang nhap
    string tai_khoan;
    string mat_khau;
    char tt;
    void lay_taikhoan(login acc[], int &acc_count);
    void ktrataikhoan();
};
login taikhoan;
// --------------------------- IN RA MAN HINH ---------------------------
void gotoXY(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

// --------------------------- QUAN LY MENU ---------------------------

struct MonAn {
    string ma;
    string ten;
    string don_vi;
    int gia;
    bool trang_thai;
    MonAn *next;
};

struct Menu {
    MonAn *head = nullptr;
    void themMon();
    void xoaMon();
    void suaMon();
};
Menu menu;

MonAn *timMonTheoMa(string maMon);

void inMenuStaff();
void inMenuAdmin();
void suaMenu();

// --------------------------- QUAN LY HOA DON ---------------------------
struct ChiTietHoaDon {
    string maMon;
    int soLuong;
    ChiTietHoaDon *next;
    ChiTietHoaDon(string ma, int sl) : maMon(ma), soLuong(sl), next(nullptr) {}
};

struct HoaDon {
    string code;
    ChiTietHoaDon *head;
    int tongTien = 0;
    bool trangThai = UNPAID;
    int soLuongMon = 0;
    HoaDon() : head(nullptr) {}
    void themMon(string maMon, int soLuong, int gia);
    void xoaMon(string maMon, int gia);
    void taoHoaDon();
};

// BIEN TOAN CUC
HoaDon bill[SO_LUONG_BAN_AN + 1]; // hoa don hien tai chua thanh toan
bool tableStatus[SO_LUONG_BAN_AN + 1] = {EMPTY};

void ghiHoaDonEMPTY(int ban);
void ghiHoaDonFULL(int ban);
bool KiemtraNamNhuan(int year);
bool LaNgayHopLe(int dd, int mm, int yy);
void docMenuTuFile();
void tracuumagd();
void luuMenuvaoFile();
void tach_ngay(int code, int &dd, int &mm, int &yy);
int so_sanh_ngay(int dd1, int mm1, int yy1, int dd2, int mm2, int yy2);
bool trong_khoang_ngay(int code, int dd_bd, int mm_bd, int yy_bd, int dd_kt, int mm_kt, int yy_kt);
void tracuu_hoadon();
void tracuu_doanhthu();
void tracuu_nhanvien();
void thongkeMonTheoMagd();
void admin();
void staff();

// --------------------------- MAIN ---------------------------

int main() {
    docMenuTuFile();
    printLoadingBar();
    printWelcomeBack();
    while (true) {
        isadmin = false;
        taikhoan.ktrataikhoan();
        if (isadmin) {
            admin();
            luuMenuvaoFile();
        } else {
            staff();
        }
    }
    return 0;
}

// -------------------------- CAC HAM KHAC ---------------------------
MonAn *timMonTheoMa(string maMon) {
    MonAn *current = menu.head;
    while (current != nullptr) {
        if (current->ma == maMon) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void xoamanhinh() {
    system("cls");
    if (isadmin) {
        gotoXY(40, 1);
        cout << purple << "◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆";
        gotoXY(40, 2);
        cout << purple "◆" << bold green
             << "               👤 ADMIN: " << setw(40) << left << name_user
             << off << purple << " ◆";
        gotoXY(40, 3);
        cout << purple << "◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆"
             << off;
    } else {
        gotoXY(40, 1);
        cout << purple << "◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆";
        gotoXY(40, 2);
        cout << purple "◆" << bold green << "               👤 STAFF: " << setw(40) << left << name_user
             << off << purple << " ◆";
        gotoXY(40, 3);
        cout << purple << "◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆"
             << off;
    }
    return;
}

bool laMaMonAnHopLe(const string &ma) {
    return ma.length() == 3 && (ma[0] == 'F' || ma[0] == 'D') && isdigit(ma[1]) && isdigit(ma[2]);
}

string DinhdangString(string s) {
    stringstream ss(s);
    string res, temp;
    while (ss >> temp) {
        res += temp + " ";
    }
    return res.empty() ? res : res.substr(0, res.length() - 1);
}

bool soNguyen(string s) {
    if (s.empty())
        return false;
    for (char c : s)
        if (!isdigit(c))
            return false;
    return true;
}

// --------------------------------- STAFF VA ADMIN ---------------------------

void staff() {
    SetConsoleOutputCP(CP_UTF8);
    string choice;
    while (true) {
        inStaffChucNang();
        getline(cin, choice);

        if (choice == "0") {
            return;
        } else if (choice == "1") {
            chonBan();
        } else {
            gotoXY(40, 15);
            cout << red << "✘ Lựa chọn không hợp lệ. Nhấn Enter để thử lại..."
                 << off;
            cin.get();
        }
    }
}

void admin() {
    string choice;

    while (true) {
        inAdminChucNang(); // In menu giao diện admin

        gotoXY(65, 21); // Đặt con trỏ tại vị trí nhập
        getline(cin, choice);
        choice = DinhdangString(choice);
        if (choice != "1" && choice != "2" && choice != "0") {
            gotoXY(30, 22);
            cout << red << "✘ Lựa chọn không hợp lệ. Nhấn Enter để thử lại..."
                 << off;
            cin.get();
            continue;
        }

        if (choice == "0") {
            return;
        }
        if (choice == "1") {
            suaMenu();
        } else if (choice == "2") {
            string subChoice;

            while (true) {
                inQuanLyHoaDon();
                getline(cin, subChoice);

                if (subChoice == "1") {
                    tracuu_doanhthu();
                } else if (subChoice == "2") {
                    tracuumagd();
                } else if (subChoice == "3") {
                    tracuu_hoadon();
                } else if (subChoice == "4") {
                    tracuu_nhanvien();
                } else if (subChoice == "5") {
                    thongkeMonTheoMagd();
                } else if (subChoice == "0") {
                    break;
                } else {
                    gotoXY(32, 27);
                    cout << red
                         << "✘ Lựa chọn không hợp lệ. Nhấn Enter để thử lại..."
                         << off;
                    cin.get();
                }
            }
        } else {
            gotoXY(30, 23);
            cout << red << "✘ Lựa chọn không hợp lệ. Nhấn Enter để thử lại..."
                 << off;
            cin.get();
        }
    }
}

// --------------------- IN ADMIN STAFF CHUC NANG -------------------------

void inAdminChucNang() {
    // Hiển thị tiếng Việt
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    string adminName = name_user;

    int x = 30, y = 4;
    xoamanhinh();

    // Header với logo admin
    gotoXY(x, y);
    cout << blue << bold;
    cout << "╔══════════════════════════════════════════════════════════════════════════════════════╗";

    gotoXY(x, y + 1);
    cout << "║                                                                                      ║";

    gotoXY(x, y + 2);
    cout << "║                                        " << purple << bold << "👑 MENU 👑" << blue << "                                    ║";

    gotoXY(x, y + 3);
    cout << "║                                                                                      ║";

    gotoXY(x, y + 4);
    cout << "╠══════════════════════════════════════════════════════════════════════════════════════╣";

    // Thông tin admin
    gotoXY(x, y + 5);
    cout << "║  " << green << bold << "Xin chào, Quản trị viên: " << yellow << adminName << blue;

    int nameLen = adminName.length();
    int padding = 59 - nameLen;
    for (int i = 0; i < padding; i++)
        cout << " ";
    cout << "║";

    gotoXY(x, y + 6);
    cout << "║                                                                                      ║";

    gotoXY(x, y + 7);
    cout << "╠══════════════════════════════════════════════════════════════════════════════════════╣";

    // Menu chức năng với icon
    gotoXY(x, y + 8);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 9);
    cout << "║    " << yellow << bold << "🍽️ 1. " << yellow
         << "Chỉnh sửa menu món ăn" << blue;
    for (int i = 0; i < 55; i++)
        cout << " ";
    cout << "║";

    gotoXY(x, y + 10);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 11);
    cout << "║    " << yellow << bold << "📊 2. " << yellow
         << "Xem thống kê hóa đơn" << blue;
    for (int i = 0; i < 56; i++)
        cout << " ";
    cout << "║";

    gotoXY(x, y + 12);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 13);
    cout << "║    " << red << bold << "🚪 0. " << red << "Đăng xuất" << blue;
    for (int i = 0; i < 67; i++)
        cout << " ";
    cout << "║";

    gotoXY(x, y + 14);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 15);
    cout << "╚══════════════════════════════════════════════════════════════════════════════════════╝";

    // Hướng dẫn nhập
    gotoXY(x, y + 17);
    cout << green << bold << "➤ " << off << "Vui lòng nhập lựa chọn của bạn: " << off;
}

void inStaffChucNang() {
    xoamanhinh();
    int x = 40, y = 4;
    gotoXY(x, y);
    cout << blue << bold << "╔═══════════════════════════════════════════════════════╗" << off;

    gotoXY(x, y + 1);
    cout << blue << bold << "║                                                       ║" << off;

    gotoXY(x, y + 2);
    cout << blue << bold << "║                   " << purple1 << "MENU NHÂN VIÊN" << blue << "                      ║" << off;

    gotoXY(x, y + 3);
    cout << blue << bold << "║                                                       ║" << off;

    gotoXY(x, y + 4);
    cout << blue << bold << "╠═══════════════════════════════════════════════════════╣" << off;

    gotoXY(x, y + 5);
    cout << blue << bold << "║ Xin chào, Nhân viên: " << white1 << name_user << blue;
    for (int i = 0; i < 33 - name_user.length(); i++)
        cout << " ";
    cout << "║" << off;

    gotoXY(x, y + 6);
    cout << blue << bold << "╠═══════════════════════════════════════════════════════╣" << off;

    // Menu chức năng
    gotoXY(x, y + 7);
    cout << blue << bold << "║ " << yellow << "1. Đặt món hoặc thanh toán                            " << blue << bold << "║" << off;
    gotoXY(x, y + 8);
    cout << blue << bold << "║ " << red << "0. Đăng xuất                                          " << blue
         << bold << "║" << off;
    gotoXY(x, y + 9);
    cout << blue << bold << "╚═══════════════════════════════════════════════════════╝" << off;
    gotoXY(x, y + 10);
    cout << green << bold << "➤ Nhập lựa chọn của bạn: " << off;
}

// -------------------------- MAN HINH CHINH ---------------------------
void manHinhChinh() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    system("cls");
    printRestaurant();
    printLoginForm();
}
void printWelcomeBack() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    int x = 30, y = 5;
    // In logo chào mừng
    gotoXY(x, y);
    printf(red "██╗    ██╗███████╗██╗      ██████╗ ██████╗ ███╗   ███╗███████╗");
    Sleep(50);
    gotoXY(x, y + 1);
    printf(red "██║    ██║██╔════╝██║     ██╔════╝██╔═══██╗████╗ ████║██╔════╝");
    Sleep(50);
    gotoXY(x, y + 2);
    printf(red "██║ █╗ ██║█████╗  ██║     ██║     ██║   ██║██╔████╔██║█████╗");
    Sleep(50);
    gotoXY(x, y + 3);
    printf(red "██║███╗██║██╔══╝  ██║     ██║     ██║   ██║██║╚██╔╝██║██╔══╝");
    Sleep(50);
    gotoXY(x, y + 4);
    printf(red "╚███╔███╔╝███████╗███████╗╚██████╗╚██████╔╝██║ ╚═╝ ██║███████╗");
    Sleep(50);
    gotoXY(x, y + 5);
    printf(red " ╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝");
    gotoXY(x + 8, y + 6);
    printf(green "𝙲𝚑𝚊𝚘 𝚖𝚞𝚗𝚐 𝚋𝚊𝚗 𝚍𝚎𝚗 𝚟𝚘𝚒 𝚑𝚎 𝚝𝚑𝚘𝚗𝚐 𝚚𝚞𝚊𝚗 𝚕𝚢 𝚗𝚑𝚊 𝚑𝚊𝚗𝚐!" off);
    gotoXY(x + 8, y + 7);
    printf(green "𝙲𝚑𝚞𝚌 𝚋𝚊𝚗 𝚗𝚐𝚊𝚢 𝚖𝚘𝚒 𝚝𝚘𝚝 𝚕𝚊𝚗𝚑, 𝚕𝚊𝚖 𝚟𝚒𝚎𝚌 𝚑𝚒𝚎𝚞 𝚚𝚞𝚊!" off);
    cin.get();
}
void printLoadingBar() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    int x = 40, y = 14;
    gotoXY(x, y);
    printf(blue "╔════════════════════════════╗" off);
    gotoXY(x, y + 1);
    printf(blue "║                            ║" off);
    gotoXY(x, y + 2);
    printf(blue "╚════════════════════════════╝" off);
    gotoXY(x + 1, y + 1);
    for (int i = 1; i <= 28; ++i) {
        cout << "█";
        Sleep(50);
    }
    gotoXY(x, y + 3);
    printf("Nhấn Enter để tiếp tục...");
}

void printRestaurant() {
    SetConsoleOutputCP(CP_UTF8);
    int x = 0, y = 1;
    gotoXY(x, y);
    cout << red << R"(
                ██████╗ ███████╗███████╗████████╗ █████╗ ██╗   ██╗██████╗  █████╗ ███╗   ██╗████████╗
                ██╔══██╗██╔════╝██╔════╝╚══██╔══╝██╔══██╗██║   ██║██╔══██╗██╔══██╗████╗  ██║╚══██╔══╝
                ██████╔╝█████╗  ███████╗   ██║   ███████║██║   ██║██████╔╝███████║██╔██╗ ██║   ██║
                ██╔══██╗██╔══╝  ╚════██║   ██║   ██╔══██║██║   ██║██╔══██╗██╔══██║██║╚██╗██║   ██║
                ██║  ██║███████╗███████║   ██║   ██║  ██║╚██████╔╝██║  ██║██║  ██║██║ ╚████║   ██║
                ╚═╝  ╚═╝╚══════╝╚══════╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝   ╚═╝
    )";
}

void printLoginForm() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int x = 10, y = 8;
    gotoXY(x, y);
    printf(blue "╔══════════════════════════════════════════════════════════════════════════════════════════════════╗" off);
    gotoXY(x, y + 1);
    printf(blue "║                                      " purple "╔════════════════════╗" blue "                                      ║" off);
    gotoXY(x, y + 2);
    printf(blue "║                                      " purple "║" bold "      ĐĂNG NHẬP     " off purple "║" blue "                                      ║" off);
    gotoXY(x, y + 3);
    printf(blue "║                                      " purple "╚════════════════════╝" blue "                                      ║" off);
    gotoXY(x, y + 4);
    printf(blue "║                                                                                                  ║" off);
    gotoXY(x, y + 5);
    printf(blue "║   " off bold "Tên đăng nhập:" off "                                                                                 " blue "║" off);
    gotoXY(x, y + 6);
    printf(blue "║   " off "┌──────────────────────────────────────────────────────────────────────────────────────┐       " blue "║" off);
    gotoXY(x, y + 7);
    printf(blue "║   " off "│                                                                                      │       " blue "║" off);
    gotoXY(x, y + 8);
    printf(blue "║   " off "└──────────────────────────────────────────────────────────────────────────────────────┘       " blue "║" off);
    gotoXY(x, y + 9);
    printf(blue "║                                                                                              " blue "    ║" off);
    gotoXY(x, y + 10);
    printf(blue "║   " off bold "Mật khẩu:" off "                                                              " blue "                        ║" off);
    gotoXY(x, y + 11);
    printf(blue "║   " off "┌──────────────────────────────────────────────────────────────────────────────────────┐       " blue "║" off);
    gotoXY(x, y + 12);
    printf(blue "║   " off "│                                                                                      │       " blue "║" off);
    gotoXY(x, y + 13);
    printf(blue "║   " off "└──────────────────────────────────────────────────────────────────────────────────────┘       " blue "║" off);
    gotoXY(x, y + 14);
    printf(blue "╠══════════════════════════════════════════════════════════════════════════════════════════════════╣" off);
    gotoXY(x, y + 15);
    printf(blue "║   " purple bold "THÔNG TIN SINH VIÊN" off "                                                              " blue "              ║" off);
    gotoXY(x, y + 16);
    printf(blue "║   " off "┌────────────────────────────────────────────────────────────────────────────────────────────┐ " blue "║" off);
    gotoXY(x, y + 17);
    printf(blue "║   " off "│ " purple bold "1. LÊ MINH TRÍ" off "                          │ " green "Lớp: 24T_DT3    MSSV: 102240170" off "                  │ " blue "║" off);
    gotoXY(x, y + 18);
    printf(blue "║   " off "│ " purple bold "2. TRẦN THỊ HẠNH NGUYÊN" off "                 │ " green "Lớp: 24T_DT3    MSSV: 102240154" off "                  │ " blue "║" off);
    gotoXY(x, y + 19);
    printf(blue "║   " off "└────────────────────────────────────────────────────────────────────────────────────────────┘ " blue "║" off);
    gotoXY(x, y + 20);
    printf(blue "║   " red bold "Giảng viên hướng dẫn: NGUYỄN THỊ MINH HỶ" off "                                                     " blue "  ║" off);
    gotoXY(x, y + 21);
    printf(blue "╚══════════════════════════════════════════════════════════════════════════════════════════════════╝" off);
}

// --------------------------- LOGIN ---------------------------

void login::lay_taikhoan(login acc[],
                         int &acc_count) { // lay thong tin account tu file
    fstream file("account.txt");
    if (!file.is_open()) {
        cout << " Vui long kiem tra lai file\n";
        return;
    }
    string line;
    acc_count = 0;
    while (getline(file, line) && acc_count < MAX_ACCOUNTS) {
        int pos1, pos2;
        pos1 = line.find("-");
        pos2 = line.find("|");
        acc[acc_count].tai_khoan = line.substr(0, pos1);
        acc[acc_count].mat_khau = line.substr(pos1 + 1, pos2 - pos1 - 1);
        acc[acc_count].tt = line.back();
        acc_count++;
    }
    file.close();
}

void login::ktrataikhoan() {
    SetConsoleOutputCP(CP_UTF8);
    xoamanhinh();
    manHinhChinh();
    int x = 16, y = 15;
    string tai_khoan, mat_khau;
    gotoXY(x, y);
    getline(cin, tai_khoan);
    gotoXY(x, y + 5);
    getline(cin, mat_khau);
    login acc[MAX_ACCOUNTS];
    int acc_count;
    lay_taikhoan(acc, acc_count);
    bool check = false;
    for (int i = 0; i < acc_count; i++) {
        if (acc[i].tai_khoan == tai_khoan && acc[i].mat_khau == mat_khau) {
            check = true;
            if (acc[i].tt == 'a')
                isadmin = true;
            name_user = tai_khoan;
            break;
        }
    }
    if (!check) {
        gotoXY(x, y + 15);
        cout << red << "✘ Tên đăng nhập hoặc mật khẩu không đúng!" << off;
        gotoXY(x, y + 16);
        cout << green << bold << "➤ Nhấn Enter để thử lại..." << off;
        cin.get();
        ktrataikhoan();
    }
}

// --------------------------- QUAN LY MENU ---------------------------

void Menu::themMon() {
    int x, y;
    string action;
    while (true) {
        x = 105;
        y = 4;
        xoamanhinh();
        inMenuAdmin();

        // In bảng chọn loại món
        gotoXY(x, y++);
        cout << blue << bold << "╔══════════════════════════════════════════╗";
        gotoXY(x, y++);
        cout << "║       " << purple << bold << "➕  THÊM MÓN ĂN MỚI VÀO MENU" << blue << "       ║";
        gotoXY(x, y++);
        cout << "╠══════════════════════════════════════════╣";
        gotoXY(x, y++);
        cout << "║   1. Thêm nước uống 🥤                   ║";
        gotoXY(x, y++);
        cout << "║   2. Thêm món ăn 🍽️                      ║";
        gotoXY(x, y++);
        cout << "║   0. Thoát                               ║";
        gotoXY(x, y++);
        cout << "╚══════════════════════════════════════════╝" << off;

        gotoXY(x + 2, y++);
        cout << green << "➤ Nhập hành động: " << off;
        getline(cin, action);
        action = DinhdangString(action);
        if (action == "0")
            return;
        if (action != "1" && action != "2") {
            gotoXY(x + 2, y++);
            cout << red << "✘ Lựa chọn không hợp lệ. Vui lòng nhập lại." << off;
            cin.get();
            continue;
        }

        xoamanhinh();
        x = 105;
        y = 4;
        if (action == "1")
            inMenuAdminNuoc();
        else
            inMenuAdminThucAn();
        bool check = false;

        // Nhập mã món
        string ma;
        while (true) {
            gotoXY(x, y++);
            cout << green << bold << "➤ Nhập mã món mới (2 chữ số):" << off;
            getline(cin, ma);
            ma = DinhdangString(ma);
            if (ma == "0") {
                check = true;
                break;
            }
            if (action == "2")
                ma = "F" + ma;
            else
                ma = "D" + ma;

            if (!laMaMonAnHopLe(ma)) {
                gotoXY(x, y++);
                cout << red << "✘ Mã không hợp lệ! Vui lòng nhập lại.\n"
                     << off;
                continue;
            }

            MonAn *temp = head;
            while (temp && temp->ma != ma)
                temp = temp->next;

            if (temp != NULL) {
                gotoXY(x, y++);
                cout << red << "✘ Mã đã tồn tại. Vui lòng nhập mã khác." << off;
                continue;
            }
            break;
        }
        if (check == true)
            continue;

        // Nhập tên món
        string ten;
        gotoXY(x, y++);
        cout << green << "➤ Nhập tên món: " << off;
        getline(cin, ten);
        ten = DinhdangString(ten);

        // Nhap don vi
        string don_vi;
        gotoXY(x, y++);
        cout << green << "➤ Nhập đơn vị: " << off;
        getline(cin, don_vi);
        don_vi = DinhdangString(don_vi);

        // Nhập giá món
        string gia_temp;
        while (true) {
            gotoXY(x, y++);
            cout << green << "➤ Nhập giá món: " << off;
            getline(cin, gia_temp);
            gia_temp = DinhdangString(gia_temp);
            if (soNguyen(gia_temp) && gia_temp.length() < 8)
                break;
            gotoXY(x, y++);
            cout << red << "✘ Giá không hợp lệ. Vui lòng nhập lại!" << endl
                 << off;
        }

        // Nhập trạng thái
        string trang_thai_temp;
        while (true) {
            gotoXY(x, y++);
            cout << "0. Không phục vụ | 1. Phục vụ";
            gotoXY(x, y++);
            cout << "➤ Nhập trạng thái món ăn: ";
            getline(cin, trang_thai_temp);
            trang_thai_temp = DinhdangString(trang_thai_temp);
            if (trang_thai_temp == "1" || trang_thai_temp == "0")
                break;
            gotoXY(x, y++);
            cout << red << "✘ Trạng thái không hợp lệ. Nhập lại!" << off;
        }

        // Xac nhan thong tin
        xoamanhinh();
        x = 105;
        y = 4;
        if (action == "1")
            inMenuAdminNuoc();
        else
            inMenuAdminThucAn();
        gotoXY(x, y++);
        cout << yellow << "➤ Bạn đang thêm món: " << ten;
        gotoXY(x, y++);
        cout << "Mã: " << ma << " - Giá: " << gia_temp;
        gotoXY(x, y++);
        cout << "Đơn vị: " << don_vi;
        gotoXY(x, y++);
        cout << "Trạng thái: "
             << (trang_thai_temp == "1" ? "Phục vụ" : "Không phục vụ") << off;
        gotoXY(x, y++);
        cout << italic << "Nhấn " << green << "Enter" << off << italic
             << " để xác nhận. Nhấn phím khác để hủy." << off;
        string confirm;
        getline(cin, confirm);
        if (!confirm.empty()) {
            gotoXY(x, y++);
            cout << red << "✘ Hủy thao tác thêm món." << off;
            gotoXY(x, y++);
            cin.get();
            continue;
        }

        // Tạo node mới
        MonAn *newNode = new MonAn;
        newNode->ma = ma;
        newNode->ten = ten;
        newNode->gia = stoi(gia_temp);
        newNode->trang_thai = (trang_thai_temp == "1");
        newNode->don_vi = don_vi;
        newNode->next = nullptr;

        // Chèn vào danh sách theo thứ tự mã
        if (!head || ma < head->ma) {
            newNode->next = head;
            head = newNode;
        } else {
            MonAn *temp = head;
            while (temp->next && temp->next->ma < ma) {
                temp = temp->next;
            }
            newNode->next = temp->next;
            temp->next = newNode;
        }
    }

    // Hiển thị lại menu
    xoamanhinh();
    inMenuAdmin();
    gotoXY(x + 2, 23);
    cout << green << bold << "✔ Thêm món mới thành công! Nhấn Enter để tiếp tục..." << off;
    cin.get();
}

void Menu::xoaMon() {
    int x, y;
    string ma;
    MonAn *temp = head, *prev = nullptr;
    while (true) {
        xoamanhinh();
        inMenuAdmin();
        x = 105;
        y = 4;
        gotoXY(x, y++);
        cout << blue << bold << "╔═══════════════════════════════════════════════╗";
        gotoXY(x, y++);
        cout << "║          " << red << bold << "🗑️  XÓA MÓN ĂN KHỎI MENU" << blue << "             ║";
        gotoXY(x, y++);
        cout << "╚═══════════════════════════════════════════════╝" << off;
        gotoXY(x + 2, y++);
        cout << green << bold << "➤ Nhập mã món bạn muốn xoá: " << off;
        getline(cin, ma);
        if (ma == "0")
            return;

        while (temp && temp->ma != ma) {
            prev = temp;
            temp = temp->next;
        }
        if (!temp) {
            temp = head;
            prev = nullptr;
            gotoXY(x + 2, y++);
            cout << red << "➤ Mã không hợp lệ. Vui lòng thử lại!" << off;
            continue;
        }
        gotoXY(x + 2, y++);
        cout << yellow << "➤ Bạn đang xoá món: " << temp->ten << " - Giá: " << temp->gia << off;
        gotoXY(x + 2, y++);
        cout << italic << "Nhấn " << green << "Enter" << off << italic << " để xác nhận xoá. " << off;

        string confirm;
        getline(cin, confirm);
        if (!confirm.empty()) {
            gotoXY(x + 2, y++);
            cout << red << "Hủy thao tác xoá món." << off;
            continue;
        }
        break;
    }
    if (prev)
        prev->next = temp->next;
    else
        head = temp->next;
    delete temp;
    inMenuAdmin();
    gotoXY(x, 4);
    cout << purple << bold << "Xoá món thành công! Nhấn Enter để tiếp tục...";
    cin.get();
}

void Menu::suaMon() {
    int x, y;

    string ma;
    bool check = false;
    MonAn *temp = nullptr;

    while (true) {
        xoamanhinh();
        inMenuAdmin();
        x = 105;
        y = 4;
        gotoXY(x, y++);
        cout << blue << bold
             << "╔═══════════════════════════════════════════════╗";
        gotoXY(x, y++);
        cout << "║            " << purple << bold << "✏️  SỬA MÓN ĂN TRONG MENU" << blue << "          ║";
        gotoXY(x, y++);
        cout << "╚═══════════════════════════════════════════════╝" << off;
        gotoXY(x + 2, y++);
        cout << green << bold << "➤ Nhập mã món bạn muốn sửa: " << off;
        getline(cin, ma);
        if (ma == "0")
            return;

        if (!laMaMonAnHopLe(ma)) {
            gotoXY(x + 2, y++);
            cout << red << "✘ Mã không hợp lệ. Vui lòng thử lại!" << off;
            cin.get();
            continue;
        }

        temp = head;
        while (temp && temp->ma != ma)
            temp = temp->next;

        if (!temp) {
            gotoXY(x + 2, y++);
            cout << red << "✘ Không tìm thấy món ăn có mã này!" << off;
            cin.get();
            continue;
        }

        // In menu lựa chọn
        string action;
        while (true) {
            xoamanhinh();
            inMenuAdmin();

            int y = 4;
            int x = 105;

            gotoXY(x, y++);
            cout << blue << bold << "╔═════════════════════════════════════════════╗";
            gotoXY(x, y++);
            cout << "║         " << purple << bold << "⚙️  CHỌN THÀNH PHẦN CẦN SỬA" << blue << "         ║";
            gotoXY(x, y++);
            cout << "╠═════════════════════════════════════════════╣";
            gotoXY(x, y++);
            cout << "║   1. Sửa tên món ăn                         ║";
            gotoXY(x, y++);
            cout << "║   2. Sửa giá món ăn                         ║";
            gotoXY(x, y++);
            cout << "║   3. Sửa trạng thái món ăn                  ║";
            gotoXY(x, y++);
            cout << "║   4. Sửa đơn vị món ăn                      ║";
            gotoXY(x, y++);
            cout << "║   0. Thoát                                  ║";
            gotoXY(x, y++);
            cout << "╚═════════════════════════════════════════════╝" << off;

            // Hiển thị tên món đang sửa
            gotoXY(x + 2, y++);
            cout << green << bold << "➤ " << yellow << temp->ma << ": "
                 << temp->ten << " - " << temp->gia << " VND" << off;

            // Nhập lựa chọn
            gotoXY(x + 2, y++);
            cout << green << "➤ Nhập lựa chọn: " << off;
            getline(cin, action);

            if (action == "0") {
                check = true;
                break;
            }

            if (action == "1") {
                gotoXY(x + 2, y++);
                cout << yellow << "➤ Nhập tên món ăn mới: ";
                string ten;
                getline(cin, ten);
                temp->ten = DinhdangString(ten);
                gotoXY(x + 2, y++);
                cout << green << "✔ Đã cập nhật tên món ăn!" << off;
            } else if (action == "2") {
                string gia_str;
                int gia;
                while (true) {
                    gotoXY(x + 2, y++);
                    cout << yellow << "➤ Nhập giá món ăn mới: ";
                    getline(cin, gia_str);
                    if (soNguyen(gia_str)) {
                        gia = stoi(gia_str);
                        temp->gia = gia;
                        gotoXY(x + 2, y++);
                        cout << green << "✔ Đã cập nhật giá món ăn!" << off;
                        break;
                    } else {
                        gotoXY(x + 2, y++);
                        cout << red << "✘ Giá không hợp lệ. Nhập lại!" << off;
                    }
                }
            } else if (action == "3") {
                string input;
                while (true) {
                    gotoXY(x + 2, y++);
                    cout << yellow << "0. Tạm ngừng | 1. Phục vụ";
                    gotoXY(x + 2, y++);
                    cout << "➤ Nhập trạng thái mới: ";
                    getline(cin, input);
                    if (input == "0" || input == "1") {
                        temp->trang_thai = (input == "1");
                        gotoXY(x + 2, y++);
                        cout << green << "✔ Đã cập nhật trạng thái món ăn!"
                             << off;
                        break;
                    } else {
                        gotoXY(x + 2, y++);
                        cout << red << "✘ Trạng thái không hợp lệ!" << off;
                    }
                }
            } else if (action == "4") {
                gotoXY(x + 2, y++);
                cout << yellow << "➤ Nhập đơn vị món ăn mới: ";
                string dv;
                getline(cin, dv);
                temp->don_vi = DinhdangString(dv);
                gotoXY(x + 2, y++);
                cout << green << "✔ Đã cập nhật đơn vị món ăn!" << off;
            } else {
                gotoXY(x + 2, y++);
                cout << red << "✘ Lựa chọn không hợp lệ. Vui lòng thử lại!"
                     << off;
            }

            gotoXY(x + 2, y++);
            cout << italic << "Nhấn Enter để tiếp tục sửa món..." << off;
            cin.get();
        }
        if (check)
            continue;
        break;
    }
}

// --------------------------- IN MENU ---------------------------

void inMenuStaff() {
    xoamanhinh();
    SetConsoleOutputCP(CP_UTF8);
    xoamanhinh();

    int x = 5, y = 4;
    gotoXY(x, y++);
    cout << blue << bold << "╔═════════════════════════════════════════════════════════════════════════════╗"
         << off;
    gotoXY(x, y++);
    cout << blue << bold << "║" << string(25, ' ') << yellow << "📋 MENU MON AN PHUC VU" << blue << string(30, ' ') << "║" << off;
    gotoXY(x, y++);
    cout << blue << bold << "╠══════╦════════╦════════════╦═════════════════════════════════════╦═══════════╣"
         << off;
    gotoXY(x, y++);
    cout << blue << bold << "║ STT  ║  MÃ    ║  ĐƠN VỊ    ║         TÊN MÓN ĂN                  ║ GIÁ (VNĐ) ║" << off;
    gotoXY(x, y++);
    cout << blue << bold << "╠══════╬════════╬════════════╬════════════════════════════════════╬═══════════╣" << off;

    MonAn *temp = menu.head;
    int stt = 1;
    while (temp) {
        if (temp->trang_thai) {
            string dishColor = (temp->ma[0] == 'D') ? yellow : purple;

            gotoXY(x, y++);
            cout << blue << bold << "║ " << setw(4) << left << stt << " ║ "
                 << dishColor << setw(6) << left << temp->ma << off;
            cout << blue << bold << " ║ " << dishColor << setw(10) << left
                 << temp->don_vi << off;
            cout << blue << bold << " ║ " << dishColor << setw(34) << left
                 << temp->ten << off;
            cout << blue << bold << " ║ " << dishColor << setw(9) << left
                 << temp->gia << off;
            cout << blue << bold << " ║" << off;

            stt++;
        }
        temp = temp->next;
    }

    gotoXY(x, y++);
    cout << blue << bold << "╚══════╩════════╩════════════╩════════════════════════════════════╩═══════════╝" << off;
}

void inMenuAdmin() {
    xoamanhinh();
    SetConsoleOutputCP(CP_UTF8);
    xoamanhinh();

    int x = 5, y = 4;
    gotoXY(x, y++);
    cout << blue << bold << "╔════════════════════════════════════════════════════════════════════════════════════════════════╗" << off;
    gotoXY(x, y++);
    cout << blue << bold << "║" << string(37, ' ') << yellow << "📋 MENU MÓN ĂN HIỆN CÓ" << blue << string(37, ' ') << "║" << off;
    gotoXY(x, y++);
    cout << blue << bold << "╠══════╦════════╦════════════╦════════════════════════════════════╦═══════════╦══════════════════╣" << off;
    gotoXY(x, y++);
    cout << blue << bold << "║ STT  ║  MÃ    ║  ĐƠN VỊ    ║         TÊN MÓN ĂN                 ║ GIÁ (VNĐ) ║   TRẠNG THÁI     ║" << off;
    gotoXY(x, y++);
    cout << blue << bold << "╠══════╬════════╬════════════╬════════════════════════════════════╬═══════════╬══════════════════╣"
         << off;

    MonAn *temp = menu.head;
    int stt = 1;
    while (temp) {
        string trangThai = temp->trang_thai ? (green + string("Phuc vu") + blue + bold) : (red + string("Tam ngung") + blue + bold);
        string dishColor = (temp->ma[0] == 'D') ? yellow : purple;

        gotoXY(x, y++);
        cout << blue << bold << "║ " << setw(4) << left << stt << " ║ " << dishColor << setw(6) << left << temp->ma << off;
        cout << blue << bold << " ║ " << dishColor << setw(10) << left << temp->don_vi << off;
        cout << blue << bold << " ║ " << dishColor << setw(34) << left << temp->ten << off;
        cout << blue << bold << " ║ " << dishColor << setw(9) << left << temp->gia << off;
        cout << blue << bold << " ║ " << setw(30) << left << trangThai << " ║" << off;

        temp = temp->next;
        stt++;
    }

    gotoXY(x, y++);
    cout << blue << bold << "╚══════╩════════╩════════════╩════════════════════════════════════╩═══════════╩══════════════════╝" << off;
    gotoXY(x, y++);
    cout << green << italic << "Nhấn 0 để thoát về menu chính" << off;
}

void inMenuAdminNuoc() {
    xoamanhinh();
    SetConsoleOutputCP(CP_UTF8);
    system("cls");

    int x = 5, y = 4;
    gotoXY(x, y++);
    cout << blue << bold << "╔════════════════════════════════════════════════════════════════════════════════════════════════╗" << off;
    gotoXY(x, y++);
    cout << blue << bold << "║" << string(37, ' ') << yellow << "📋 MENU MÓN ĂN HIỆN CÓ" << blue << string(37, ' ') << "║" << off;
    gotoXY(x, y++);
    cout << blue << bold << "╠══════╦════════╦════════════╦════════════════════════════════════╦═══════════╦══════════════════╣"
         << off;
    gotoXY(x, y++);
    cout << blue << bold << "║ STT  ║  MÃ    ║  ĐƠN VỊ    ║         TÊN MÓN ĂN                 ║ GIÁ (VNĐ) ║   TRẠNG THÁI     ║"
         << off;
    gotoXY(x, y++);
    cout << blue << bold << "╠══════╬════════╬════════════╬════════════════════════════════════╬═══════════╬══════════════════╣"
         << off;

    MonAn *temp = menu.head;
    int stt = 1;
    string dishColor = yellow; // Màu cho nước uống
    while (temp) {
        string trangThai = temp->trang_thai
                               ? (green + string("Phuc vu") + blue + bold)
                               : (red + string("Tam ngung") + blue + bold);

        if (temp->ma[0] != 'F') {
            gotoXY(x, y++);
            cout << blue << bold << "║ " << setw(4) << left << stt << " ║ " << dishColor << setw(6) << left << temp->ma << off;
            cout << blue << bold << " ║ " << dishColor << setw(10) << left << temp->don_vi << off;
            cout << blue << bold << " ║ " << dishColor << setw(34) << left << temp->ten << off;
            cout << blue << bold << " ║ " << dishColor << setw(9) << left << temp->gia << off;
            cout << blue << bold << " ║ " << setw(30) << left << trangThai << " ║" << off;
        }
        temp = temp->next;
        stt++;
    }
    gotoXY(x, y++);
    cout << blue << bold
         << "╚══════╩════════╩════════════╩════════════════════════════════════╩═══════════╩══════════════════╝"
         << off;
    gotoXY(x, y++);
    cout << green << italic << "Nhấn 0 để thoát" << off;
}

void inMenuAdminThucAn() {
    xoamanhinh();
    SetConsoleOutputCP(CP_UTF8);
    system("cls");

    int x = 5, y = 4;
    gotoXY(x, y++);
    cout << blue << bold << "╔════════════════════════════════════════════════════════════════════════════════════════════════╗" << off;
    gotoXY(x, y++);
    cout << blue << bold << "║" << string(37, ' ') << yellow << "📋 MENU MÓN ĂN HIỆN CÓ" << blue << string(37, ' ') << "║" << off;
    gotoXY(x, y++);
    cout << blue << bold << "╠══════╦════════╦════════════╦════════════════════════════════════╦═══════════╦══════════════════╣" << off;
    gotoXY(x, y++);
    cout << blue << bold << "║ STT  ║  MÃ    ║  ĐƠN VỊ    ║         TÊN MÓN ĂN                 ║ GIÁ (VNĐ) ║   TRẠNG THÁI     ║" << off;
    gotoXY(x, y++);
    cout << blue << bold << "╠══════╬════════╬════════════╬════════════════════════════════════╬═══════════╬══════════════════╣" << off;

    MonAn *temp = menu.head;
    int stt = 1;
    string dishColor = yellow; // Màu cho nước uống
    while (temp) {
        string trangThai = temp->trang_thai ? (green + string("Phuc vu") + blue + bold) : (red + string("Tam ngung") + blue + bold);

        if (temp->ma[0] != 'D') {
            gotoXY(x, y++);
            cout << blue << bold << "║ " << setw(4) << left << stt << " ║ " << dishColor << setw(6) << left << temp->ma << off;
            cout << blue << bold << " ║ " << dishColor << setw(10) << left << temp->don_vi << off;
            cout << blue << bold << " ║ " << dishColor << setw(34) << left << temp->ten << off;
            cout << blue << bold << " ║ " << dishColor << setw(9) << left << temp->gia << off;
            cout << blue << bold << " ║ " << setw(30) << left << trangThai << " ║" << off;
        }
        temp = temp->next;
        stt++;
    }

    gotoXY(x, y++);
    cout << blue << bold << "╚══════╩════════╩════════════╩════════════════════════════════════╩═══════════╩══════════════════╝" << off;
    gotoXY(x, y++);
    cout << green << italic << "Nhấn 0 để thoát về menu chính" << off;
}
// ---------------------------------------- CHINH SUA MENU ------------------------------------------
void inSuaMenu() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    int x = 30, y = 4;
    gotoXY(x, y);
    cout << blue << bold << "╔══════════════════════════════════════════════════════════════════════════════════════╗";
    gotoXY(x, y + 1);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 2);
    cout << "║                                   " << purple << bold << "✏️ CHỈNH SỬA MENU    " << blue << "                              ║";
    gotoXY(x, y + 3);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 4);
    cout << "╠══════════════════════════════════════════════════════════════════════════════════════╣";
    gotoXY(x, y + 5);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 6);
    cout << "║    " << yellow << bold << "🍽️  1. " << yellow << "Thêm món ăn" << blue;
    for (int i = 0; i < 64; i++)
        cout << " ";
    cout << "║";
    gotoXY(x, y + 7);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 8);
    cout << "║    " << yellow << bold << "✏️  2. " << yellow << "Sửa món ăn"
         << blue;
    for (int i = 0; i < 65; i++)
        cout << " ";
    cout << "║";
    gotoXY(x, y + 9);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 10);
    cout << "║    " << yellow << bold << "🗑️  3. " << yellow << "Xóa món ăn" << blue;
    for (int i = 0; i < 64; i++)
        cout << " ";
    cout << "║";
    gotoXY(x, y + 11);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 12);
    cout << "║    " << red << bold << "🚪  0. " << red << "Thoát" << blue;
    for (int i = 0; i < 71; i++)
        cout << " ";
    cout << "║";
    gotoXY(x, y + 13);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 14);
    cout << "╚══════════════════════════════════════════════════════════════════════════════════════╝";
    gotoXY(x + 2, y + 16);
    cout << green << bold << "➤ " << off << italic << "Nhập hành động bạn muốn làm: " << off;
}

void suaMenu() {
    string action;
    do {
        while (true) {
            xoamanhinh();
            inSuaMenu();
            getline(cin, action);
            if (action.length() != 1 || (action != "0" && action != "1" && action != "2" && action != "3")) {
                gotoXY(32, 21);
                cout << red << bold << "➤ Không hợp lệ. Nhấn Enter để tiếp tục: ";
                cin.get();
                continue;
            }
            break;
        }
        if (action == "0")
            break;
        if (action == "1") {
            menu.themMon();
            break;
        } else if (action == "2")
            menu.suaMon();
        else if (action == "3")
            menu.xoaMon();
        xoamanhinh();
        cout << green << bold << "Đã chỉnh sửa thành công!\n";
        inMenuAdmin();
    } while (action != "0");
    return;
}

// ------------------------ QUAN LY HOA DON -------------------------
void inQuanLyHoaDon() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    int x = 30, y = 4;
    xoamanhinh();
    gotoXY(x, y);
    cout << cyan << bold;
    cout << "╔══════════════════════════════════════════════════════════════════════════════════════╗";
    gotoXY(x, y + 1);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 2);
    cout << "║                              " << purple << "🧾 QUẢN LÝ HÓA ĐƠN 🧾" << cyan << "                                   ║";
    gotoXY(x, y + 3);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 4);
    cout << "╠══════════════════════════════════════════════════════════════════════════════════════╣";
    gotoXY(x, y + 5);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 6);
    cout << "║                            " << white1 << bold << "Chọn chức năng bạn muốn:" << cyan << "                                  ║";
    gotoXY(x, y + 7);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 8);
    cout << "╠══════════════════════════════════════════════════════════════════════════════════════╣";
    gotoXY(x, y + 9);
    cout << blue << "║                                                                                      ║";
    gotoXY(x, y + 10);
    cout << "║     " << yellow << bold << "1. " << yellow << "📝 Doanh thu       " << blue;
    for (int i = 0; i < 59; i++)
        cout << " ";
    cout << "║";
    gotoXY(x, y + 11);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 12);
    cout << "║     " << yellow << bold << "2. " << yellow << "📊 Tra cứu hóa đơn" << blue;
    for (int i = 0; i < 60; i++)
        cout << " ";
    cout << "║";
    gotoXY(x, y + 13);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 14);
    cout << "║     " << yellow << bold << "3. " << yellow << "📅 Tra cứu hóa đơn theo ngày" << blue;
    for (int i = 0; i < 50; i++)
        cout << " ";
    cout << "║";

    gotoXY(x, y + 15);
    cout << "║                                                                                      ║";

    gotoXY(x, y + 16);
    cout << "║     " << yellow << bold << "4. " << yellow << "👥 Tra cứu nhân viên      " << blue;
    for (int i = 0; i < 52; i++)
        cout << " ";
    cout << "║";

    gotoXY(x, y + 17);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 18);
    cout << "║     " << yellow << bold << "5. " << yellow << "🍜 Tra cứu món ăn            " << blue;
    for (int i = 0; i < 52; i++)
        cout << " ";
    cout << "║";
    gotoXY(x, y + 19);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 20);
    cout << "║     " << red << bold << "0. " << red << "🔙 Thoát màn hình     " << blue;
    for (int i = 0; i < 56; i++)
        cout << " ";
    cout << "║";
    gotoXY(x, y + 21);
    cout << "║                                                                                      ║";
    gotoXY(x, y + 22);
    cout << "╚══════════════════════════════════════════════════════════════════════════════════════╝" << off;
    gotoXY(x + 2, y + 23);
    cout << green << bold << "➤ " << off << white1 << bold << "Nhập lựa chọn của bạn: " << off;
}

void HoaDon::themMon(string maMon, int soLuong, int gia) {
    ChiTietHoaDon *current = head;
    while (current != nullptr) {
        if (current->maMon == maMon) {
            current->soLuong += soLuong;
            tongTien += gia * soLuong;
            return;
        }
        current = current->next;
    }

    ChiTietHoaDon *newNode = new ChiTietHoaDon(maMon, soLuong);
    newNode->next = head;
    head = newNode;
    tongTien += gia * soLuong;
    soLuongMon++;
}

void HoaDon::xoaMon(string maMon, int gia) {
    ChiTietHoaDon *current = head;
    ChiTietHoaDon *prev = nullptr;

    while (current != nullptr) {
        if (current->maMon == maMon) {
            if (prev == nullptr) {
                head = current->next;
            } else {
                prev->next = current->next;
            }

            tongTien -= gia * current->soLuong;
            soLuongMon--;
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }
    cout << "Khong tim thay mon an trong hoa don\n";
}

void HoaDon::taoHoaDon() {
    int x = 90, y = 4;
    int cnt = 0;
    string maMon;
    xoamanhinh();
    inMenuStaff();
    while (true) {
        gotoXY(x, y++);
        cout << green << "➤ Nhập mã món (00 để dừng): " << off;
        getline(cin, maMon);
        maMon = DinhdangString(maMon);
        if (maMon == "00")
            break;
        if (!laMaMonAnHopLe(maMon)) {
            gotoXY(x, y++);
            cout << red << "✘ Mã món ăn không hợp lệ!" << off;
            continue;
        }
        MonAn *temp = timMonTheoMa(maMon);
        if (!temp || !temp->trang_thai) {
            gotoXY(x, y++);
            cout << red << "✘ Món ăn không tồn tại hoặc ngừng phục vụ." << off;
            continue;
        }
        string soLuongStr;
        int soLuong;
        while (true) {
            gotoXY(x, y++);
            cout << green << "➤ Nhập số lượng: " << off;
            getline(cin, soLuongStr);
            if (!soNguyen(soLuongStr) || stoi(soLuongStr) <= 0) {
                gotoXY(x, y++);
                cout << red << "✘ Số lượng phải là số nguyên dương." << off;
                continue;
            }
            soLuong = stoi(soLuongStr);
            break;
        }
        themMon(maMon, soLuong, temp->gia);
        cnt++;
        if (cnt == 5)
            break;
    }
    string trangThaiStr;
    while (true) {
        gotoXY(x, y++);
        cout << green << "➤ Thanh toán (0: chưa, 1: đã): " << off;
        getline(cin, trangThaiStr);
        if (trangThaiStr != "0" && trangThaiStr != "1") {
            gotoXY(x, y++);
            cout << red << "✘ Vui lòng nhập 0 hoặc 1." << off;
            continue;
        }
        trangThai = (trangThaiStr == "1");
        break;
    }
}

void inHoaDon(int ban) {
    gotoXY(0, 4);
    time_t now = time(0);
    tm *ltm = localtime(&now);
    bill[ban].code = ma_hoadon(false);
    int temp = stoi(bill[ban].code);
    temp++;
    cout << "╔════════════════════════════════════════════════════════════════╗\n";
    cout << "║                       HOÁ ĐƠN THANH TOÁN                       ║\n";
    cout << "║----------------------------------------------------------------║\n";
    cout << "║Mã hoá đơn: " << setw(52) << left << temp << "║\n";
    cout << "║Thu ngân: " << setw(54) << left << name_user << "║\n";
    cout << "║Bàn: " << setw(59) << left << ban << "║\n";
    cout << "║Ngày: " << setw(2) << ltm->tm_mday << "/" << setw(2) << 1 + ltm->tm_mon << "/" << 1900 + ltm->tm_year << setw(52) << right << "║\n";
    cout << "║CHI TIẾT HOÁ ĐƠN                                                ║\n";
    cout << "║----------------------------------------------------------------║\n";
    cout << "║MÃ   TÊN MÓN ĂN                GIÁ      SỐ LƯỢNG   THÀNH TIỀN   ║\n";
    ChiTietHoaDon *current = bill[ban].head;
    while (current != nullptr) {
        MonAn *mon = timMonTheoMa(current->maMon);
        if (mon != nullptr) {
            int thanhTien = mon->gia * current->soLuong;
            cout << "║" << setw(4) << left << mon->ma << " " << setw(25)
                 << mon->ten << " " << setw(8) << mon->gia << " " << setw(10)
                 << current->soLuong << " " << setw(13) << thanhTien << "║\n";
        }
        current = current->next;
    }
    cout << "║----------------------------------------------------------------║\n";
    cout << "║TỔNG TIỀN: " << bill[ban].tongTien << " VND " << setw(46 - (to_string(bill[ban].tongTien).length() - 6)) << right << "║\n";
    if (bill[ban].trangThai == PAID) {
        cout << "║TRẠNG THÁI: " << setw(55) << left << "Đã thanh toán";
    } else {
        cout << "║TRẠNG THÁI: " << setw(54) << left << "Chưa thanh toán";
    }
    cout << "║\n";
    cout << "╚════════════════════════════════════════════════════════════════╝" << off;
}

void datHoaDon(int ban) {
    string maMon;
    string soLuong;
    inMenuStaff();
    cout << '\n';
    cout << green "➤ Nhập mã món (00 để dừng): \n";
    while (true) {
        cout << green "Mã món: ";
        getline(cin, maMon);
        maMon = DinhdangString(maMon);
        if (maMon == "00")
            break;
        MonAn *temp = timMonTheoMa(maMon);
        if (!temp || !temp->trang_thai) {
            cout << red << "✘ Mã không hợp lệ! Vui lòng nhập lại.\n"
                 << off;
            continue;
        }
        cout << "Số lượng: ";
        getline(cin, soLuong);
        soLuong = DinhdangString(soLuong);
        if (!soNguyen(soLuong) || stoi(soLuong) <= 0) {
            cout << "Số lượng phải là số nguyên dương\n";
            continue;
        }

        bill[ban].themMon(maMon, stoi(soLuong), temp->gia);
    }
}

void ghiHoaDonEMPTY(int ban) {
    SetConsoleOutputCP(CP_UTF8);
    xoamanhinh();
    inMenuStaff();
    int x = 90, y = 4;
    gotoXY(x, y++);
    cout << blue << bold << "╔═══════════════════════════════════════════════════════╗" << off;
    gotoXY(x, y++);
    cout << blue << bold << "║            " << yellow << "📝 GHI HÓA ĐƠN - BÀN " << setw(2) << ban << blue << "                    ║" << off;
    gotoXY(x, y++);
    cout << blue << bold << "╠═══════════════════════════════════════════════════════╣" << off;
    gotoXY(x, y++);
    cout << blue << bold << "║ " << yellow << "1. Đặt món ăn" << blue << string(41, ' ') << "║" << off;
    gotoXY(x, y++);
    cout << blue << bold << "║ " << red << "0. Thoát" << blue << string(46, ' ') << "║" << off;
    gotoXY(x, y++);
    cout << blue << bold << "╚═══════════════════════════════════════════════════════╝" << off;
    gotoXY(x + 2, y++);
    cout << green << bold << "➤ Nhập hành động bạn muốn thực hiện: " << off;
    string input;
    int action;
    while (true) {
        getline(cin, input);
        input = DinhdangString(input);
        if (!soNguyen(input)) {
            gotoXY(x + 2, y + 1);
            cout << red << "✘ Vui lòng nhập số nguyên hợp lệ!" << off;
            continue;
        }
        action = stoi(input);
        if (action < 0 || action > 1) {
            gotoXY(x + 2, y + 1);
            cout << red << "✘ Lựa chọn không hợp lệ. Vui lòng nhập lại!" << off;
            continue;
        }
        break;
    }
    if (action == 0)
        return;
    else if (action == 1) {
        bill[ban].taoHoaDon();
        tableStatus[ban] = FULL;
        inHoaDon(ban);
    }
}

void ghiHoaDonFULL(int ban) {
    SetConsoleOutputCP(CP_UTF8);
    xoamanhinh();
    inHoaDon(ban);
    int x = 90, y = 4;
    gotoXY(x, y++);
    cout << blue << bold << "╔═══════════════════════════════════════════════════════╗" << off;
    gotoXY(x, y++);
    cout << blue << bold << "║            " << yellow << "📝 GHI HÓA ĐƠN - BÀN " << setw(2) << ban << blue << "                    ║" << off;
    gotoXY(x, y++);
    cout << blue << bold << "╠═══════════════════════════════════════════════════════╣" << off;
    gotoXY(x, y++);
    cout << blue << bold << "║ " << yellow << "1. Đặt thêm món ăn" << blue << string(36, ' ') << "║" << off;
    gotoXY(x, y++);
    cout << blue << bold << "║ " << yellow << "2. Xóa món ăn" << blue << string(41, ' ') << "║" << off;
    gotoXY(x, y++);
    cout << blue << bold << "║ " << yellow << "3. Thanh toán" << blue << string(41, ' ') << "║" << off;
    gotoXY(x, y++);
    cout << blue << bold << "║ " << red << "0. Thoát" << blue << string(46, ' ') << "║" << off;
    gotoXY(x, y++);
    cout << blue << bold << "╚═══════════════════════════════════════════════════════╝" << off;
    gotoXY(x, y++);
    cout << green << bold << "➤ Nhập hành động bạn muốn thực hiện: " << off;

    string actionStr;
    int action;
    while (true) {
        getline(cin, actionStr);
        actionStr = DinhdangString(actionStr);
        if (!soNguyen(actionStr)) {
            gotoXY(x, y + 1);
            cout << red << "✘ Dữ liệu nhập không hợp lệ. Vui lòng nhập lại!"
                 << off;
            continue;
        }
        action = stoi(actionStr);
        if (action < 0 || action > 3) {
            gotoXY(x, y + 1);
            cout << red << "✘ Lựa chọn không hợp lệ. Vui lòng nhập lại!" << off;
            continue;
        }
        break;
    }
    switch (action) {
    case 0:
        return;
    case 1:
        datHoaDon(ban);
        ghiHoaDonFULL(ban);
        break;
    case 2: {
        xoamanhinh();
        inHoaDon(ban);
        cout << '\n';
        cout << green << bold << "➤ Nhập mã món cần xóa: " << off;
        string maMon;
        getline(cin, maMon);
        maMon = DinhdangString(maMon);
        MonAn *mon = timMonTheoMa(maMon);

        if (!mon || !laMaMonAnHopLe(maMon)) {
            // gotoXY(x, 24);
            cout << red << "✘ Mã món không hợp lệ!" << off;
        } else {
            ChiTietHoaDon *ct = bill[ban].head;
            bool found = false;
            while (ct) {
                if (ct->maMon == maMon) {
                    found = true;
                    break;
                }
                ct = ct->next;
            }
            if (!found) {
                // gotoXY(x, 24);
                cout << red << "✘ Món này không có trong hóa đơn!" << off;
            } else {
                bill[ban].xoaMon(maMon, mon->gia);
                // gotoXY(x, 24);
                cout << green << "✔ Xóa món thành công!" << off;
            }
        }
        cin.get();
        ghiHoaDonFULL(ban);
        break;
    }
    case 3: {
        bill[ban].trangThai = PAID;
        bill[ban].code = ma_hoadon(true);
        string filename = bill[ban].code + ".txt";

        ofstream outfile("luu_magd.txt", ios::app);
        outfile << bill[ban].code << "\n";
        outfile.close();
        string filepath = "Ma_giao_dich/" + filename;
        ofstream billfile(filepath);
        if (billfile.is_open()) {
            time_t now = time(0);
            tm *ltm = localtime(&now);
            billfile << "HÓA ĐƠN THANH TOÁN\n";
            billfile << "------------------------------------------------------------------\n";
            billfile << "Mã hóa đơn: " << bill[ban].code << "\n";
            billfile << "Thu ngân: " << name_user << "\n";
            billfile << "Bàn: " << ban << "\n";
            billfile << "Ngày: " << ltm->tm_mday << "/" << (ltm->tm_mon + 1) << "/" << (ltm->tm_year + 1900) << "\n";
            billfile << "            CHI TIẾT HÓA ĐƠN\n";
            billfile << "------------------------------------------------------------------\n";
            billfile << "MÃ   TÊN MÓN ĂN                GIÁ      SỐ LƯỢNG   THÀNH TIỀN   \n";
            ChiTietHoaDon *current = bill[ban].head;
            int giamgia = (bill[ban].tongTien >= 2000000) ? 20 : 0;
            while (current != nullptr) {
                MonAn *mon = timMonTheoMa(current->maMon);
                if (mon != nullptr) {
                    int thanhTien = mon->gia * current->soLuong;
                    billfile << setw(4) << left << mon->ma << " "
                             << setw(25) << mon->ten << " " << setw(8)
                             << mon->gia << " " << setw(10)
                             << current->soLuong << " " << setw(13)
                             << thanhTien << "\n";
                }
                current = current->next;
            }
            billfile << "------------------------------------------------------------------\n";
            billfile << "Giảm giá: " << giamgia << "%\n";
            billfile << "TỔNG TIỀN: " << (giamgia ? bill[ban].tongTien * 80 / 100 : bill[ban].tongTien) << " VND\n";
            billfile << "TRẠNG THÁI: Đã thanh toán\n";
            billfile << "------------------------------------------------------------------\n\n";
            billfile.close();
        } else {
            cout << red << "✘ Không thể tạo file hóa đơn!" << off << endl;
        }
        xoamanhinh();
        inHoaDon(ban);
        y = 4;
        x = 80;
        gotoXY(x, y);
        printf(green "┌┬┐┬ ┬┌─┐┌┐┌┬ ┬  ┌┬┐┌─┐┌─┐┌┐┌  ┌┬┐┬ ┬┌─┐┌┐┌┬ ┬  ┌─┐┌─┐┌┐┌┌─┐");
        gotoXY(x, y + 1);
        printf(green " │ ├─┤├─┤│││├─┤   │ │ │├─┤│││   │ ├─┤├─┤│││├─┤  │  │ │││││ ┬");
        gotoXY(x, y + 2);
        printf(green " ┴ ┴ ┴┴ ┴┘└┘┴ ┴   ┴ └─┘┴ ┴┘└┘   ┴ ┴ ┴┴ ┴┘└┘┴ ┴  └─┘└─┘┘└┘└─┘");
        ChiTietHoaDon *cur = bill[ban].head;
        while (cur != nullptr) {
            ChiTietHoaDon *tmp = cur;
            cur = cur->next;
            delete tmp;
        }
        bill[ban].head = nullptr;
        bill[ban].tongTien = 0;
        bill[ban].trangThai = EMPTY;
        bill[ban].code = "";
        tableStatus[ban] = EMPTY;
        cin.get();
        break;
    }
    }
}
// --------------------------- CHỌN BÀN ---------------------------
void chonBan() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    while (true) {
        xoamanhinh();
        inSoDoBan();
        int x = 90, y = 4;

        gotoXY(x, y++);
        cout << cyan << "╔═══════════════════════════════════════════════════════╗" << off;
        gotoXY(x, y++);
        cout << cyan << "║                  " << yellow << "🧾 CHỌN BÀN ĂN" << cyan << "                       ║" << off;
        gotoXY(x, y++);
        cout << cyan << "╚═══════════════════════════════════════════════════════╝" << off;
        gotoXY(x, y++);
        cout << yellow italic << "➤  Chọn bàn bạn muốn đặt: ";
        string input;
        getline(cin, input);
        input = DinhdangString(input);
        gotoXY(x, y++);
        if (!soNguyen(input)) {
            gotoXY(x, y++);
            cout << red << "✘ Đầu vào không đúng. Nhấn Enter để thử lại..." << off;
            cin.get();
            continue;
        }
        if (input == "0")
            return;
        int ban = stoi(input);
        if (ban < 1 || ban > SO_LUONG_BAN_AN) {
            gotoXY(x, y++);
            cout << red << "✘ Số bàn không hợp lệ. Vui lòng chọn lại." << off;
            gotoXY(x, y++);
            system("pause");
            continue;
        }
        inMenuStaff();
        if (tableStatus[ban] == EMPTY)
            ghiHoaDonEMPTY(ban);
        else
            ghiHoaDonFULL(ban);
    }
}

void inSoDoBan() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    system("cls");
    int tempx = 10, tempy = 6;
    gotoXY(tempx, tempy - 2);
    int posX, posY;
    cout << cyan << bold << "╔══════════════════════════════ SƠ ĐỒ BÀN ĂN ═════════════════════════════╗" << off;
    for (int i = 1; i <= SO_LUONG_BAN_AN; ++i) {
        int hang = (i - 1) / 4;
        int cot = (i - 1) % 4;
        posX = tempx + cot * 20;
        posY = tempy + hang * 5;
        string maBan = "B" + (i < 10 ? "0" + to_string(i) : to_string(i));
        string trangThaiStr;
        string icon;
        if (tableStatus[i] == EMPTY) {
            trangThaiStr = green + string("Trống") + off;
        } else if (tableStatus[i] == FULL) {
            trangThaiStr = yellow + string("Đã đặt") + off;
        }
        gotoXY(posX, posY);
        cout << blue << "╔════════════╗";
        gotoXY(posX, posY + 1);
        cout << "║            ║";
        gotoXY(posX, posY + 2);
        cout << "║            ║";
        gotoXY(posX, posY + 3);
        cout << "╚════════════╝" << off;
        gotoXY(posX + 4, posY + 1);
        cout << white1 << bold << maBan << off;
        gotoXY(posX + 2, posY + 2);
        cout << trangThaiStr;
    }
    gotoXY(tempx, posY + 4);
    cout << green italic << "Nhấn 0 để thoát.";
}

// --------------------------- TRA CỨU NGÀY ---------------------------
void tach_ngay(int code, int &dd, int &mm, int &yy) {
    dd = code / 10000000;
    mm = (code / 100000) % 100;
    yy = (code / 1000) % 100;
}
int so_sanh_ngay(int dd1, int mm1, int yy1, int dd2, int mm2, int yy2) {
    if (yy1 < yy2)
        return -1;
    if (yy1 > yy2)
        return 1;
    if (mm1 < mm2)
        return -1;
    if (mm1 > mm2)
        return 1;
    if (dd1 < dd2)
        return -1;
    if (dd1 > dd2)
        return 1;
    return 0;
}
bool trong_khoang_ngay(int code, int dd_bd, int mm_bd, int yy_bd, int dd_kt,
                       int mm_kt, int yy_kt) {
    int dd, mm, yy;
    tach_ngay(code, dd, mm, yy);
    if (so_sanh_ngay(dd, mm, yy, dd_bd, mm_bd, yy_bd) < 0)
        return false;
    if (so_sanh_ngay(dd, mm, yy, dd_kt, mm_kt, yy_kt) > 0)
        return false;
    return true;
}
// --------------------------- TRA CỨU HÓA ĐƠN ---------------------------
void tracuumagd() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    xoamanhinh();
    int x = 80, y = 4;
    gotoXY(x, y++);
    cout << blue << bold << "╔═══════════════════════════════════════════════════════╗" << off;
    gotoXY(x, y++);
    cout << blue << bold << "║             " << yellow << "🔍 TRA CỨU HÓA ĐƠN THEO MÃ" << blue << "                ║" << off;
    gotoXY(x, y++);
    cout << blue << bold << "╚═══════════════════════════════════════════════════════╝" << off;
    gotoXY(x + 2, y++);
    cout << green << bold << "➤ Nhập mã giao dịch: " << off;
    string ma;
    getline(cin, ma);
    string filename = "Ma_giao_dich/" + ma + ".txt";
    ifstream file(filename);
    if (!file) {
        gotoXY(x + 2, y + 1);
        cout << red << "✘ Mã giao dịch không tồn tại!" << off;
        gotoXY(x + 2, y + 2);
        cout << green << bold << "➤ Nhấn Enter để tiếp tục..." << off;
        cin.get();
        return;
    }
    x = 10;
    y = 4;
    gotoXY(x, y++);
    cout << cyan << bold << "╔═══════════════════════════════════════════════════════╗" << off;
    gotoXY(x, y++);
    cout << cyan << bold << "║             " << yellow << "🧾 CHI TIẾT HÓA ĐƠN" << cyan << "                       ║" << off;
    gotoXY(x, y++);
    cout << cyan << bold << "╚═══════════════════════════════════════════════════════╝" << off;
    string line;
    while (getline(file, line)) {
        gotoXY(x + 2, y++);
        cout << white1 << line << off;
    }
    file.close();
    gotoXY(x + 2, y + 1);
    cout << green << bold << "➤ Nhấn Enter để tiếp tục..." << off;
    cin.get();
}

void tracuu_hoadon() {
    SetConsoleOutputCP(CP_UTF8);
    xoamanhinh();
    int x = 4, y = 4;
    gotoXY(x, y++);
    cout << blue << bold << "╔════════════════════════════════════════════════════════════╗" << off;
    gotoXY(x, y++);
    cout << blue << bold << "║            " << yellow << "📅 TRA CỨU HÓA ĐƠN THEO NGÀY" << blue << "                    ║" << off;
    gotoXY(x, y++);
    cout << blue << bold << "╚════════════════════════════════════════════════════════════╝" << off;

    int dd_bd, mm_bd, yy_bd;
    int dd_kt, mm_kt, yy_kt;
    string input;
    do {
        gotoXY(x, y++);
        cout << green << "➤ Nhập ngày bắt đầu (dd mm yy): " << off;
        getline(cin >> ws, input);
        stringstream ss(input);
        string thua;
        if (!(ss >> dd_bd >> mm_bd >> yy_bd)) {
            gotoXY(x, y++);
            cout << red << "✘ Định dạng không hợp lệ!" << off;
            continue;
        }
        if (ss >> thua) {
            gotoXY(x, y++);
            cout << red << "✘ Định dạng không hợp lệ!" << off;
            continue;
        }

        if (!LaNgayHopLe(dd_bd, mm_bd, yy_bd)) {
            gotoXY(x, y++);
            cout << red << "✘ Ngày bắt đầu không hợp lệ. Vui lòng nhập lại."
                 << off;
            continue;
        }
        break;
    } while (true);
    do {
        gotoXY(x, y++);
        cout << green << "➤ Nhập ngày kết thúc (dd mm yy): " << off;
        getline(cin >> ws, input);

        stringstream ss(input);
        string thua;

        if (!(ss >> dd_kt >> mm_kt >> yy_kt)) {
            gotoXY(x, y++);
            cout << red << "✘ Định dạng không hợp lệ!" << off;
            continue;
        }
        if (ss >> thua) {
            gotoXY(x, y++);
            cout << red << "✘ Định dạng không hợp lệ!" << off;
            continue;
        }
        if (!LaNgayHopLe(dd_kt, mm_kt, yy_kt)) {
            gotoXY(x, y++);
            cout << red << "✘ Ngày kết thúc không hợp lệ. Vui lòng nhập lại."
                 << off;
            continue;
        }
        break;
    } while (true);

    fstream infile("luu_magd.txt");
    if (!infile.is_open()) {
        gotoXY(x + 2, y++);
        cout << red << "✘ Không thể mở file luu_magd.txt!" << off;
        gotoXY(x + 2, y++);
        cout << green << bold << "➤ Nhấn Enter để tiếp tục..." << off;
        cin.get();
        return;
    }
    xoamanhinh();
    x = 4;
    y = 4;
    gotoXY(x, y++);
    cout << cyan << bold << "╔════════════════════════════════════════════════════════════╗" << off;
    gotoXY(x, y++);
    cout << cyan << bold << "║        " << yellow << "📋 DANH SÁCH HÓA ĐƠN TRONG KHOẢNG NGÀY" << cyan << "              ║" << off;
    gotoXY(x, y++);
    cout << cyan << bold << "╚════════════════════════════════════════════════════════════╝" << off;

    string line;
    bool found = false;
    while (getline(infile, line)) {
        x = 0;
        if (line.empty())
            continue;
        int code = stoi(line);
        if (trong_khoang_ngay(code, dd_bd, mm_bd, yy_bd, dd_kt, mm_kt, yy_kt)) {
            found = true;
            string filename = "Ma_giao_dich/" + line + ".txt";
            fstream billfile(filename);
            if (!billfile.is_open()) {
                gotoXY(x, y++);
                cout << red << "✘ Không thể mở file " << filename << "!" << off;
                continue;
            }
            gotoXY(x, y++);
            cout << yellow << bold << "══════════ Hóa đơn: " << line << " ══════════" << off << endl;
            string bill_line;
            while (getline(billfile, bill_line)) {
                gotoXY(x, y++);
                cout << white1 << "    " << bill_line << off << endl;
            }
            gotoXY(x, y++);
            cout << cyan << "────────────────────────────────────────────────────────────" << off << endl;
            y++;
            billfile.close();
        }
    }
    if (!found) {
        gotoXY(x + 2, y++);
        cout << red
             << "✘ Không tìm thấy hóa đơn nào trong khoảng thời gian này!"
             << off;
    }
    infile.close();
    gotoXY(x + 2, y++);
    cout << green << bold << "➤ Nhấn Enter để tiếp tục..." << off;
    cin.get();
}

void tracuu_doanhthu() {
    xoamanhinh();
    SetConsoleOutputCP(CP_UTF8);
    int x = 4, y = 4;
    gotoXY(x, y++);
    int tong = 0;
    int dd_bd, mm_bd, yy_bd;
    int dd_kt, mm_kt, yy_kt;
    string input;
    do {
        gotoXY(x, y++);
        cout << green << "➤ Nhập ngày bắt đầu (dd mm yy): " << off;
        getline(cin >> ws, input);
        stringstream ss(input);
        string thua;
        if (!(ss >> dd_bd >> mm_bd >> yy_bd)) {
            gotoXY(x, y++);
            cout << red << "✘ Định dạng không hợp lệ!" << off;
            continue;
        }
        if (ss >> thua) {
            gotoXY(x, y++);
            cout << red << "✘ Định dạng không hợp lệ!" << off;
            continue;
        }
        if (!LaNgayHopLe(dd_bd, mm_bd, yy_bd)) {
            gotoXY(x, y++);
            cout << red << "✘ Ngày bắt đầu không hợp lệ. Vui lòng nhập lại."
                 << off;
            continue;
        }
        break;
    } while (true);
    do {
        gotoXY(x, y++);
        cout << green << "➤ Nhập ngày kết thúc (dd mm yy): " << off;
        getline(cin >> ws, input);
        stringstream ss(input);
        string thua;
        if (!(ss >> dd_kt >> mm_kt >> yy_kt)) {
            gotoXY(x, y++);
            cout << red << "✘ Định dạng không hợp lệ!" << off;
            continue;
        }
        if (ss >> thua) {
            gotoXY(x, y++);
            cout << red << "✘ Định dạng không hợp lệ!" << off;
            continue;
        }
        if (!LaNgayHopLe(dd_kt, mm_kt, yy_kt)) {
            gotoXY(x, y++);
            cout << red << "✘ Ngày kết thúc không hợp lệ. Vui lòng nhập lại."
                 << off;
            continue;
        }
        break; // nhập đúng
    } while (true);

    fstream infile("luu_magd.txt");
    if (!infile.is_open()) {
        gotoXY(x, y++);
        cout << red << "✘ Không thể mở file luu_magd.txt!" << off;
        gotoXY(x, y++);
        cout << green << bold << "➤ Nhấn Enter để tiếp tục..." << off;
        cin.ignore();
        cin.get();
        return;
    }
    y++;
    gotoXY(x, y++);
    cout << cyan << bold << "╔════════════════════════════════════════════════════════════╗" << off;
    gotoXY(x, y++);
    cout << cyan << bold << "║                  💰 DOANH THU THEO NGÀY                    ║" << off;
    gotoXY(x, y++);
    cout << cyan << bold << "╚════════════════════════════════════════════════════════════╝" << off;

    string line;
    while (getline(infile, line)) {
        if (line.empty())
            continue;
        int code = stoi(line);
        if (trong_khoang_ngay(code, dd_bd, mm_bd, yy_bd, dd_kt, mm_kt, yy_kt)) {
            string filename = "Ma_giao_dich/" + line + ".txt";
            fstream billfile(filename);
            if (!billfile.is_open()) {
                gotoXY(x, y++);
                cout << red << "✘ Không thể mở file " << filename << "!" << off;
                continue;
            }

            int line_cnt = 0;
            string line_save[100];
            string bill_line;
            while (getline(billfile, bill_line)) {
                line_cnt++;
                line_save[line_cnt] = bill_line;
            }

            size_t start = line_save[line_cnt - 3].find_first_of("0123456789");
            size_t end =
                line_save[line_cnt - 3].find_first_not_of("0123456789", start);
            string price = line_save[line_cnt - 3].substr(start, end - start);

            gotoXY(x, y++);
            cout << yellow << "✔ Hóa đơn " << line << " có doanh thu: " << price
                 << " VND" << off;

            tong += stoi(price);
            billfile.close();
        }
    }
    y++;
    gotoXY(x, y++);
    cout << cyan
         << "--------------------------------------------------------------"
         << off;
    gotoXY(x, y++);
    cout << white1 << "🧮 Tổng doanh thu từ ngày " << dd_bd << "/" << mm_bd
         << "/" << yy_bd << " đến " << dd_kt << "/" << mm_kt << "/" << yy_kt
         << ": " << bold << green << tong << " VND" << off;

    y++;
    gotoXY(x, y++);
    cout << green << bold << "➤ Nhấn Enter để tiếp tục..." << off;
    cin.get();
    infile.close();
}
// -------------------------------- MA HOA DON --------------------------------
string ma_hoadon(bool tang) {
    if (current_count == -1)
        khoitao_mahoadon();

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    int dd = t->tm_mday;
    int mm = t->tm_mon + 1;
    int yy = t->tm_year % 100;
    int stt = current_count + (tang ? 1 : 0);

    if (tang)
        current_count++;

    char buffer[16];
    sprintf(buffer, "%02d%02d%02d%03d", dd, mm, yy, stt);
    return string(buffer);
}

void lay_mahoadon(int code_bill[], int &bill_count) { // lay ma hoa don
    fstream file("luu_magd.txt");
    string line;
    bill_count = 0;

    while (getline(file, line) && bill_count < MAX_BILLS) {
        if (all_of(line.begin(), line.end(), ::isdigit)) {
            code_bill[bill_count++] = stoi(line);
        } else {
            cout << "Ma giao dich khong hop le " << line;
            return;
        }
    }
    if (bill_count == 0)
        code_bill[bill_count++] = 0;
    return;
}

void khoitao_mahoadon() {
    int code_bill[MAX_BILLS];
    int bill_count;
    lay_mahoadon(code_bill, bill_count);

    if (bill_count == 0) {
        current_count = 0;
    } else {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        int dd_system = t->tm_mday;
        int mm_system = t->tm_mon + 1;
        int yy_system = t->tm_year % 100;
        int last_code = code_bill[bill_count - 1];

        int dd_bill = last_code / 10000000;
        int mm_bill = (last_code / 100000) % 100;
        int yy_bill = (last_code / 1000) % 100;

        if (dd_bill != dd_system || mm_bill != mm_system ||
            yy_bill != yy_system) {
            current_count = 0;
        } else {
            current_count = last_code % 1000;
        }
    }
}

// --------------- LAM VIEC TREN FILE --------------------------------
void docMenuTuFile() {
    ifstream file("menu.txt");
    string ma, ten, gia, trangThai, don_vi;
    menu.head = nullptr;
    MonAn *endNode = nullptr;
    if (!file.is_open()) {
        cout << red << "✘ Không thể mở file menu.txt!" << off;
        return;
    }

    while (getline(file, ma, '|') && getline(file, ten, '|') && getline(file, gia, '|') && getline(file, don_vi, '|') && getline(file, trangThai)) {
        MonAn *newNode = new MonAn;
        newNode->ma = ma;
        newNode->ten = ten;
        newNode->gia = stoi(gia);
        newNode->don_vi = don_vi;
        newNode->trang_thai = (trangThai == "1") ? 1 : 0;
        newNode->next = nullptr;

        if (endNode == nullptr) {
            menu.head = newNode;
        } else {
            endNode->next = newNode;
        }
        endNode = newNode;
    }
}

void luuMenuvaoFile() {
    ofstream file("menu.txt", ios::trunc);
    MonAn *temp = menu.head;
    while (temp) {
        if (!temp->ma.empty() && !temp->ten.empty()) {
            file << temp->ma << "|" << temp->ten << "|" << temp->gia << "|" << temp->don_vi << "|" << temp->trang_thai << "\n";
        }
        temp = temp->next;
    }
    file.close();
}
void tracuu_nhanvien() {
    xoamanhinh();
    SetConsoleOutputCP(CP_UTF8);

    int x = 4, y = 4;
    string nhanvien;
    int dd_bd, mm_bd, yy_bd;
    int dd_kt, mm_kt, yy_kt;

    gotoXY(x, y++);
    cout << green << "➤ Nhập tên nhân viên (thu ngân): " << off;
    getline(cin >> ws, nhanvien);

    string input;
    do {
        gotoXY(x, y++);
        cout << green << "➤ Nhập ngày bắt đầu (dd mm yy): " << off;
        getline(cin >> ws, input);

        stringstream ss(input);
        string thua;

        if (!(ss >> dd_bd >> mm_bd >> yy_bd)) {
            gotoXY(x, y++);
            cout << red << "✘ Định dạng không hợp lệ!" << off;
            continue;
        }

        if (ss >> thua) {
            gotoXY(x, y++);
            cout << red << "✘ Định dạng không hợp lệ!" << off;
            continue;
        }

        if (!LaNgayHopLe(dd_bd, mm_bd, yy_bd)) {
            gotoXY(x, y++);
            cout << red << "✘ Ngày bắt đầu không hợp lệ!" << off;
            continue;
        }
        break;

    } while (true);

    // Nhập ngày kết thúc
    do {
        gotoXY(x, y++);
        cout << green << "➤ Nhập ngày kết thúc (dd mm yy): " << off;
        getline(cin >> ws, input);
        stringstream ss(input);
        string thua;
        if (!(ss >> dd_kt >> mm_kt >> yy_kt)) {
            gotoXY(x, y++);
            cout << red << "✘ Định dạng không hợp lệ!" << off;
            continue;
        }
        if (ss >> thua) {
            gotoXY(x, y++);
            cout << red << "✘ Định dạng không hợp lệ!" << off;
            continue;
        }
        if (!LaNgayHopLe(dd_kt, mm_kt, yy_kt)) {
            gotoXY(x, y++);
            cout << red << "✘ Ngày kết thúc không hợp lệ!" << off;
            continue;
        }
        break;
    } while (true);
    fstream infile("luu_magd.txt");
    if (!infile.is_open()) {
        gotoXY(x, y++);
        cout << red << "✘ Không thể mở file luu_magd.txt!" << off;
        gotoXY(x, y++);
        cout << green << bold << "➤ Nhấn Enter để tiếp tục..." << off;
        cin.ignore();
        cin.get();
        return;
    }
    int tong = 0;
    string magd;
    y++;
    gotoXY(x, y++);
    cout << cyan << bold << "╔════════════════════════════════════════════════════════════╗" << off;
    gotoXY(x, y++);
    cout << cyan << bold << "║        💼 DOANH THU THEO NHÂN VIÊN VÀ NGÀY                 ║" << off;
    gotoXY(x, y++);
    cout << cyan << bold << "╚════════════════════════════════════════════════════════════╝" << off;

    while (getline(infile, magd)) {
        if (magd.empty())
            continue;
        int code = stoi(magd);
        if (!trong_khoang_ngay(code, dd_bd, mm_bd, yy_bd, dd_kt, mm_kt, yy_kt))
            continue;
        string filename = "Ma_giao_dich/" + magd + ".txt";
        fstream file(filename);
        if (!file.is_open()) {
            gotoXY(x, y++);
            cout << red << "✘ Không thể mở file " << filename << "!" << off;
            continue;
        }
        string line;
        bool nhanvien_trung = false;
        string doanhthu = "";
        while (getline(file, line)) {
            if (line.find("Thu ngân:") != string::npos) {
                string ten = line.substr(line.find(":") + 1);
                ten.erase(0, ten.find_first_not_of(" \t"));
                if (ten == nhanvien) {
                    nhanvien_trung = true;
                }
            }
            if (line.find("TỔNG TIỀN:") != string::npos && nhanvien_trung) {
                size_t pos = line.find(":") + 1;
                doanhthu = line.substr(pos);
                doanhthu.erase(remove(doanhthu.begin(), doanhthu.end(), 'V'), doanhthu.end());
                doanhthu.erase(remove(doanhthu.begin(), doanhthu.end(), 'N'), doanhthu.end());
                doanhthu.erase(remove(doanhthu.begin(), doanhthu.end(), 'D'), doanhthu.end());
                doanhthu.erase(remove(doanhthu.begin(), doanhthu.end(), ' '), doanhthu.end());
                tong += stoi(doanhthu);
                break;
            }
        }
        if (nhanvien_trung) {
            gotoXY(x, y++);
            cout << yellow << "✔ Hóa đơn " << magd << " có doanh thu: " << doanhthu << " VND" << off;
        }
        file.close();
    }

    y++;
    gotoXY(x, y++);
    cout << cyan << "--------------------------------------------------------------" << off;
    gotoXY(x, y++);
    cout << white1 << "🧮 Tổng doanh thu của \"" << nhanvien << "\" từ " << dd_bd << "/" << mm_bd << "/" << yy_bd << " đến " << dd_kt << "/"
         << mm_kt << "/" << yy_kt << ": " << bold << green << tong << " VND" << off;
    y++;
    gotoXY(x, y++);
    cout << green << bold << "➤ Nhấn Enter để tiếp tục..." << off;
    cin.get();
    infile.close();
}
// Hàm thống kê món theo mã món, trong khoảng thời gian nhất định
void thongkeMonTheoMagd() {
    SetConsoleOutputCP(CP_UTF8);
    xoamanhinh();
    inMenuAdmin();
    string maMon;
    int dd_bd, mm_bd, yy_bd;
    int dd_kt, mm_kt, yy_kt;
    int x = 4, y = 30;
    do {
        gotoXY(x, y++);
        cout << green << "➤ Nhập mã món cần thống kê (0 để thoát): " << off;
        getline(cin >> ws, maMon);
        if (maMon == "0")
            return;
        if (!laMaMonAnHopLe(maMon)) {
            gotoXY(x, y++);
            cout << red << "✘ Mã món không hợp lệ. Vui lòng thử lại!" << off;
        }
    } while (!laMaMonAnHopLe(maMon));
    // Nhập ngày bắt đầu
    string input;
    do {
        gotoXY(x, y++);
        cout << green << "➤ Nhập ngày bắt đầu (dd mm yy): " << off;
        getline(cin >> ws, input);

        stringstream ss(input);
        string thua;

        if ((ss >> dd_bd >> mm_bd >> yy_bd) && !(ss >> thua) &&
            LaNgayHopLe(dd_bd, mm_bd, yy_bd)) {
            break;
        }

        gotoXY(x, y++);
        cout << red << "✘ Ngày không hợp lệ!" << off;
    } while (true);
    do {
        gotoXY(x, y++);
        cout << green << "➤ Nhập ngày kết thúc (dd mm yy): " << off;
        getline(cin >> ws, input);
        stringstream ss(input);
        string thua;
        if ((ss >> dd_kt >> mm_kt >> yy_kt) && !(ss >> thua) &&
            LaNgayHopLe(dd_kt, mm_kt, yy_kt)) {
            break;
        }
        gotoXY(x, y++);
        cout << red << "✘ Ngày không hợp lệ!" << off;
    } while (true);
    fstream infile("luu_magd.txt");
    if (!infile.is_open()) {
        gotoXY(x, y++);
        cout << red << "✘ Không thể mở file luu_magd.txt!" << off;
        cin.ignore();
        cin.get();
        return;
    }
    string magd;
    int tongSoLan = 0;
    int tongDoanhThu = 0;
    gotoXY(x, y++);
    cout << cyan << bold << "╔════════════════════════════════════════════════════════════╗" << off;
    gotoXY(x, y++);
    cout << cyan << bold << "║         📦 THỐNG KÊ MÓN " << maMon << " THEO HÓA ĐƠN                   ║" << off;
    gotoXY(x, y++);
    cout << cyan << bold << "╚════════════════════════════════════════════════════════════╝" << off;

    while (getline(infile, magd)) {
        if (magd.empty())
            continue;
        int code = stoi(magd);
        if (!trong_khoang_ngay(code, dd_bd, mm_bd, yy_bd, dd_kt, mm_kt, yy_kt))
            continue;
        string filename = "Ma_giao_dich/" + magd + ".txt";
        fstream file(filename);
        if (!file.is_open()) {
            gotoXY(x, y++);
            cout << red << "✘ Không thể mở file " << filename << "!" << off;
            continue;
        }
        string line;
        bool found = false;
        int soLuong = 0;
        int doanhThuMon = 0;
        while (getline(file, line)) {
            if (line.substr(0, 4) == maMon + " ") {
                found = true;

                string parts[100];
                int nParts = 0;

                stringstream ss(line);
                string word;
                while (ss >> word && nParts < 100) {
                    parts[nParts++] = word;
                }

                int giaIndex = -1;
                for (int i = 2; i < nParts; ++i) {
                    bool laSo = true;
                    for (char c : parts[i]) {
                        if (!isdigit(c)) {
                            laSo = false;
                            break;
                        }
                    }
                    if (laSo) {
                        giaIndex = i;
                        break;
                    }
                }

                if (giaIndex != -1 && giaIndex + 1 < nParts) {
                    int gia = stoi(parts[giaIndex]);
                    soLuong = stoi(parts[giaIndex + 1]);
                    doanhThuMon = gia * soLuong;
                    tongSoLan += soLuong;
                    tongDoanhThu += doanhThuMon;
                }

                break;
            }
        }
        file.close();
    }

    gotoXY(x, y++);
    cout << cyan << "--------------------------------------------------------------" << off;
    gotoXY(x, y++);
    cout << green << bold << "🧮 Tổng số lần gọi món: " << tongSoLan << ", Tổng doanh thu: " << tongDoanhThu << " VND" << off;
    y++;
    gotoXY(x, y++);
    cout << '\n';
    cout << green << "➤ Nhấn Enter để tiếp tục..." << off;
    cin.get();
    infile.close();
}
bool KiemtraNamNhuan(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
bool LaNgayHopLe(int dd, int mm, int yy) {
    if (yy < 1 || mm < 1 || mm > 12 || dd < 1)
        return false;

    int NgayTrongThang[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (mm == 2 && KiemtraNamNhuan(yy))
        NgayTrongThang[2] = 29;

    return dd <= NgayTrongThang[mm];
}