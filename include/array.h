#pragma once

// array kiểu dữ liệu int
struct Array {
    int* data = nullptr;
    int size = 0;
    int capacity = 2;
};


//Khởi tạo array
void init_array (Array& v);
//Khởi tạo array với kích cỡ
void init_array (Array& v, int size);
//Khởi tạo array với kích cỡ và giá trị
void init_array (Array& v, int size, int value);



// trả về array
Array create_array();
// trả về array với kích cỡ
Array create_array(int size);
// trả về array với kích cỡ và giá trị
Array create_array(int size, int value);



// giải phóng bộ nhớ cho array
void free_array (Array& v);
// cấp phát bộ nhớ cho array
void reserve (Array& v, int newCapacity);
// trả về 1 array giống dữ liệu nhưng khác địa chỉ bộ nhớ
Array copy (const Array& v);
// lấy giá trị tại chỉ số
int get (const Array& v, int index);
// đặt giá trị tại chỉ số
bool set (Array& v, int index, int value);
// thêm phần tử vào cuối array
void push_back (Array& v, int value);
// xóa phần tử ở cuối array
void pop_back (Array& v);

