#if !defined NULL
#define NULL 0
#endif

#if !defined (STACKARRAY_H)
#define STACKARRAY_H

template < class T >
class StackArray
{

   private:
      int max_stack;
      T** items;
      int sz;

      void arrayResize(int new_size);

   public:
      StackArray();
      ~StackArray();

      bool isEmpty();
      int size();
      void popAll();

      T* peek();
      void push(T* item);
      T* pop();

};

template < class T >
StackArray<T>::StackArray()
{
   max_stack = 2;
   items = new T*[max_stack]; 
   sz = 0; 
}

template < class T >
StackArray<T>::~StackArray()
{
   delete[] items;
}

template < class T >
bool StackArray<T>::isEmpty() 
{
   return sz == 0;
}  

template < class T >
int StackArray<T>::size()
{
   return sz;
}

template < class T >
T* StackArray<T>::peek()
{
   T* item = NULL;
   if (!isEmpty()) 
   {
      item = items[sz - 1];
   }
   return item;
}  

template < class T >
void StackArray<T>::push(T* item)
{
   if (sz == max_stack) 
   {
      arrayResize(2*max_stack); 
   } 
 
   items[sz] = item;
   sz++;
}  

template < class T >
T* StackArray<T>::pop()
{
   T* item = NULL;

   if (!isEmpty()) 
   {
      item = items[sz - 1];
      items[sz - 1] = NULL;
      sz--;
   }

   return item;
} 

template < class T >
void StackArray<T>::arrayResize(int new_size)
{
   max_stack = new_size;
   T** temp = new T*[max_stack];

   for (int i = 0; i < sz; i++)
   {
      temp[i] = items[i];
   }

   delete[] items;
   items = temp;
}

template < class T >
void StackArray<T>::popAll()
{
   delete[] items;

   max_stack = 2;
   items = new T*[max_stack]; 
   sz = 0; 
}  

#endif
