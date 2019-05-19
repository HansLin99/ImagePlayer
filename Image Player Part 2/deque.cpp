/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */

/* Constructor for the Deque class */
template <class T>
Deque<T>::Deque(){
    //k1 point to the front
    k1 = 0;
    //k2 point to the end
    k2 = -1;
    //between k1 and k2 is the data
}

/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T const& newItem)
{
    data.push_back(newItem);
    k2++;
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes. 
 *
 * @return The leftmost item of the Deque.
 */
template <class T>
T Deque<T>::popL()
{
    T popT = data[k1];
//    data[k1] = 0;
    k1++;
    if (k2 - k1 + 1 <= k1) {
        vector<T> temp;
        for (int i=k1; i <= k2; i++) {
            temp.push_back(data[i]);
        }
        data = temp;
        k1 = 0;
        k2 = data.size()-1;

    }
    if (isEmpty()) {
        k1 = 0;
        k2 = -1;
    }
    return popT;
}
/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The rightmost item of the Deque.
 */
template <class T>
T Deque<T>::popR()
{
    T popT = data[k2];
    data.pop_back();
    k2--;
    if ( k2 - k1 + 1 <= k1) {
        vector<T> temp;
        for (int i=k1; i <= k2; i++) {
            temp.push_back(data[i]);
        }
        data = temp;
        k1 = 0;
        k2 = data.size()-1;
    }
    if (isEmpty()) {
        k1 = 0;
        k2 = -1;
    }
    return popT;

}

/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the left of the deque.
 */
template <class T>
T Deque<T>::peekL()
{
    return data[k1];
}

/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR()
{
    return data[k2];
}

/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const
{
    return (k2 + 1) == k1;
}
