#include "../include/dynamic_array.hpp" 
using namespace std;

//Given just for reference. Only the class name and function names should match.
class dynamic_array_implementation : public dynamic_array
{
private:
    int size;
    int capacity;
    int *arr;
    double increase_factor;
    double decrease_factor;

public:
    dynamic_array_implementation();
    int get_size();
    int get_capacity();
    void set_increase_factor_table_size(double);
    void set_load_factor_reduction(double);
    void append(int);
    void pop();
    int get(int);
    ~dynamic_array_implementation();
};

dynamic_array_implementation::dynamic_array_implementation()
{
    size = 0;
    capacity = 0;
    increase_factor = 2;
    decrease_factor = 0.25;
}
int dynamic_array_implementation::get_size()
{
    return this->size;
}
int dynamic_array_implementation::get_capacity()
{
    return this->capacity;
}

void dynamic_array_implementation::set_increase_factor_table_size(double increase_factor)
{
    this->increase_factor = increase_factor;
    return;
}
void dynamic_array_implementation::set_load_factor_reduction(double decrease_factor)
{
    this->decrease_factor = decrease_factor;
    return;
}

void dynamic_array_implementation::append(int element)
{
    // Initially when the array is empty
    if(this->capacity == 0)
    {   
        // Allocate a single memory location and assign value

        this->arr = new int[1];
        this->capacity += 1;
        this->arr[0] = element;
        this->size += 1;

    }
    
    // Array already exists
    else
    {
        // If the array size is equal to the capacity
        if(this->capacity == this->size)
        {
            // Allocate a new and larger memory location and copy the existy elements

            double newCapacity = this->capacity * this->increase_factor;

            if((int)newCapacity == newCapacity)
            {
                this->capacity = (int)newCapacity;
            }
            else
            {
                this->capacity = (int)newCapacity + 1;
            }
            int* newArr = new int[this->capacity];
            for(int i=0;i<this->size;i++)
            {
                newArr[i] = this->arr[i];
            }
            newArr[this->size] = element;
            delete this->arr;
            this->arr = newArr;
            this->size += 1;
        }
        
        // Just assign element to next memory location
        else
        {
            this->arr[this->size] = element;
            this->size += 1;
        }

    }
    
    return;
}

void dynamic_array_implementation::pop()
{
    // If array is already empty return
    if(this->size == 0){
        return;
    }
    this->size -= 1;
    double loadFactor = (double)this->size/(double)this->capacity;
    
    
    // If the loadFactor is less than or equal to the decrease_factor then contract
    // the array to a smaller size and copy the elements 
    if(loadFactor <= this->decrease_factor)
    {
        double newCapacity = this->capacity * decrease_factor * this->increase_factor;

        // If new capacity is not a round number ,i have taken floor of that number

        // If the new capacity is not zero, create new array and copy elements 
        if((int) newCapacity != 0)
        {
            int* newArr = new int[(int) newCapacity];

            for(int i=0;i<this->size;i++)
            {
                newArr[i] = this->arr[i];
            }
            
            delete this->arr;
            this->arr = newArr;
            
        }
        // If new capacity is zero remove the array
        else
        {
            delete this->arr;
        }
        this->capacity = (int) newCapacity;
            
    }


    return;
}
int dynamic_array_implementation::get(int index)
{
    return this->arr[index];
}

dynamic_array_implementation::~dynamic_array_implementation()
{
    if (capacity > 0)
    {
        delete[] arr;
    }
}