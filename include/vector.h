#pragma once

// vector kiểu dữ liệu int
struct vector_int {
    int* data = nullptr;
    int size = 0;
    int capacity = 2;
};


//Khởi tạo vector
void init (vector_int& v);

//Khởi tạo vector với kích cỡ
void init (vector_int& v, int size);

//Khởi tạo vector với kích cỡ và giá trị
void init (vector_int& v, int size, int value);



// trả về vector
vector_int create_vector();

// trả về vector với kích cỡ
vector_int create_vector(int size);

// trả về vector với kích cỡ và giá trị
vector_int create_vector(int size, int value);



// giải phóng bộ nhớ cho vector
void free (vector_int& v);

// cấp phát bộ nhớ cho vector
void reserve (vector_int& v, int newCapacity);

// trả về 1 vector giống dữ liệu nhưng khác địa chỉ bộ nhớ
vector_int copy (const vector_int& v);

// lấy giá trị tại chỉ số
int get (const vector_int& v, int index);

// đặt giá trị tại chỉ số
bool set (vector_int& v, int index, int value);

// thêm phần tử vào cuối vector
void push_back (vector_int& v, int value);

// xóa phần tử ở cuối vector
void pop_back (vector_int& v);

