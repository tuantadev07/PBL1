#include "vector.h"


namespace {
    int* _copy_array (const int* oldArr, int curSize, int newCapacity) {    
        int* newArr = new int[newCapacity];
        
        for (int i=0; i<curSize; ++i) {
            newArr[i] = oldArr[i];
        }
        
        return newArr;
    }

    void _resize (vector_int& v) {
        v.capacity *= 2;
        
        int* newData = _copy_array (v.data, v.size, v.capacity);
        
        delete[] v.data;
        v.data = newData;
    }   
}


void init (vector_int& v) {
    free(v);

    v.capacity = 2;
    v.size = 0;
    v.data = new int[v.capacity];
}

void init (vector_int& v, int size) {
    free(v);

    if (size <= 0) {
        init(v);
        return;
    }

    v.capacity = size;
    v.size = size;
    v.data = new int[v.capacity];

     for (int i=0; i<size; ++i) {
        v.data[i] = 0;
    }
}

void init (vector_int& v, int size, int value) {
    free(v);

    if (size <= 0) {
        init(v);
        return;
    }

    v.capacity = size;
    v.size = size;
    v.data = new int[v.capacity];

    for (int i=0; i<size; ++i) {
        v.data[i] = value;
    }
}

vector_int create_vector() {
    vector_int v;
    init(v);

    return v;
}

vector_int create_vector(int size) {
    vector_int v;
    init(v, size);

    return v;
}

vector_int create_vector(int size, int value) {
    vector_int v;
    init(v, size, value);
    
    return v;
}


void free (vector_int& v) {
    delete[] v.data;
    v.data = nullptr;
    v.size = 0;
    v.capacity = 0;
}

void reserve (vector_int& v, int newCapacity) {
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

vector_int copy (const vector_int& v) {
    vector_int newV;

    newV.size = v.size;
    newV.capacity = v.capacity;
    newV.data = _copy_array(v.data, v.size, v.capacity);

    return newV;
}

int get (const vector_int& v, int index) {
    return v.data[index];
}

bool set (vector_int& v, int index, int value) {
    if (index < 0  || index >= v.size) {
        return false;
    }

    v.data[index] = value;
    return true;
}

void push_back (vector_int& v, int value) {
    if (v.data == nullptr) {
        init(v);
    }
    if (v.size == v.capacity) {
        _resize(v);
    }

    v.data[v.size++] = value;
}

void pop_back (vector_int& v) {
    if (v.size > 0) {
        --v.size;
    }
}