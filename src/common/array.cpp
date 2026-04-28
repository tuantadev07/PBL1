#include "common/array.h"


namespace {
    int* _copy_array (const int* oldArr, int curSize, int newCapacity) {    
        int* newArr = new int[newCapacity];
        
        for (int i=0; i<curSize; ++i) {
            newArr[i] = oldArr[i];
        }
        
        return newArr;
    }

    void _resize (Array& v) {
        v.capacity *= 2;
        
        int* newData = _copy_array (v.data, v.size, v.capacity);
        
        delete[] v.data;
        v.data = newData;
    }   
}


void init_array (Array& v) {
    free_array(v);

    v.capacity = 2;
    v.size = 0;
    v.data = new int[v.capacity];
}

void init_array (Array& v, int size) {
    free_array(v);

    if (size <= 0) {
        init_array(v);
        return;
    }

    v.capacity = size;
    v.size = size;
    v.data = new int[v.capacity];

     for (int i=0; i<size; ++i) {
        v.data[i] = 0;
    }
}

void init_array (Array& v, int size, int value) {
    free_array(v);

    if (size <= 0) {
        init_array(v);
        return;
    }

    v.capacity = size;
    v.size = size;
    v.data = new int[v.capacity];

    for (int i=0; i<size; ++i) {
        v.data[i] = value;
    }
}

Array create_array() {
    Array v;
    init_array(v);

    return v;
}

Array create_array(int size) {
    Array v;
    init_array(v, size);

    return v;
}

Array create_array(int size, int value) {
    Array v;
    init_array(v, size, value);
    
    return v;
}


void free_array (Array& v) {
    delete[] v.data;
    v.data = nullptr;
    v.size = 0;
    v.capacity = 0;
}

void reserve (Array& v, int newCapacity) {
    if (newCapacity < 2) {
        newCapacity = 2;
    }
    if (newCapacity <= v.capacity) {
        return;
    }

    v.capacity = newCapacity;
    int* newData = _copy_array(v.data, v.size, v.capacity);

    delete[] v.data;
    v.data = newData;
}

Array copy (const Array& v) {
    Array newV;

    newV.size = v.size;
    newV.capacity = v.capacity;
    newV.data = _copy_array(v.data, v.size, v.capacity);

    return newV;
}

int get (const Array& v, int index) {
    return v.data[index];
}

bool set (Array& v, int index, int value) {
    if (index < 0  || index >= v.size) {
        return false;
    }

    v.data[index] = value;
    return true;
}

void push_back (Array& v, int value) {
    if (v.data == nullptr) {
        init_array(v);
    }
    if (v.size == v.capacity) {
        _resize(v);
    }

    v.data[v.size++] = value;
}

void pop_back (Array& v) {
    if (v.size > 0) {
        --v.size;
    }
}